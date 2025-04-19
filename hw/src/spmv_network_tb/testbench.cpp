
#include "Utils.hpp"
#include "Vspmv_network_tb.h"
#include "Vspmv_network_tb_spmv_network_tb.h"
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <format>
#include <gtest/gtest.h>
#include <random>
#include "Controller.hpp"
#include "Simulation.hpp"
#include "Utils.hpp"
#include "Float.hpp"
#include "algorithm"

using DeviceT = Vspmv_network_tb;

static constexpr uint64_t TESTED_NET_WIDTH_MAX  = 64;
static constexpr uint64_t TEST_SIZE             = 10e2;
static constexpr uint64_t NETWORK_WIDTH         = Vspmv_network_tb_spmv_network_tb::NETWORK_WIDTH;
static constexpr uint64_t IN_WIDTH              = Vspmv_network_tb_spmv_network_tb::IN_WIDTH;
static constexpr uint64_t ID_WIDTH              = Vspmv_network_tb_spmv_network_tb::ID_WIDTH;
static constexpr uint64_t OUT_WIDTH             = Vspmv_network_tb_spmv_network_tb::OUT_WIDTH;

struct SpMvNetworkIf
{
    static constexpr uint64_t PARALLELISM  = NETWORK_WIDTH;

    std::array<uint32_t, NETWORK_WIDTH>     in_id;
    std::array<uint64_t, NETWORK_WIDTH>     in_val;

    uint64_t                                in_valid : NETWORK_WIDTH;
    uint64_t                                in_ready : NETWORK_WIDTH;

    std::array<uint32_t, NETWORK_WIDTH>     out_id;
    std::array<uint64_t, NETWORK_WIDTH>     out_val;

    uint64_t                                out_valid : NETWORK_WIDTH;
    uint64_t                                out_ready : NETWORK_WIDTH;

    friend bool operator==(const SpMvNetworkIf &lhs, const SpMvNetworkIf &rhs)
    {
        return lhs.out_id == rhs.out_id &&
               lhs.out_val == rhs.out_val;
    }

    friend std::ostream& operator<<(std::ostream& out, const SpMvNetworkIf& f)
    {
        out << "SpMvNetworkIf {\n";
        out << "  in_id     : " << f.in_id << "\n";
        out << "  in_val    : " << std::hex << f.in_val << std::dec << "\n";
        out << "  in_valid  : " << f.in_valid << "\n";
        out << "  in_ready  : " << f.in_ready << "\n";
        out << "  out_id    : " << f.out_id << "\n";
        out << "  out_val   : " << std::hex << f.out_val << std::dec << "\n";
        out << "  out_valid : " << std::bitset<NETWORK_WIDTH>(f.out_valid) << "\n";
        out << "  out_ready : " << std::bitset<NETWORK_WIDTH>(f.out_ready) << "\n";
        out << "}";
        return out;
    }
};


class SpMvNetworkDriver : public sim::Controller<DeviceT, SpMvNetworkIf>
{
public:
    using sim::Controller<DeviceT, SpMvNetworkIf>::Controller;

    SpMvNetworkDriver()
        : sim::Controller<DeviceT, SpMvNetworkIf>()

    {
    }

    void driveIntf(SpMvNetworkIf aStim)
    {
        for (int i = 0; i < NETWORK_WIDTH; i++)
        {
            theDevice->in_val[i] = aStim.in_val[i];
            theDevice->in_id[i] = aStim.in_id[i];
        }

        theDevice->in_valid = aStim.in_valid;

        theDevice->out_ready = aStim.out_ready;
    }

    void reset() override
    {
        SpMvNetworkIf resetStim = {0};
        resetStim.out_ready = 0x0;
        theDevice->out_ready = resetStim.out_ready;
        driveIntf(resetStim);
    }

    void next() override
    {
        auto myReady = 0xffffffffffffffff;
        if (!isControllerEmpty())
        {
            this->getQueue().front().out_ready = myReady;

            driveIntf(front());

            this->getQueue().front().in_valid &= ~theDevice->in_ready;

            if (this->getQueue().front().in_valid == 0)
                pop();

            return;
        }
        SpMvNetworkIf aStim{0};
        aStim.out_ready = myReady;
        driveIntf(aStim);
    }

    std::uniform_int_distribution<uint64_t> theRandomDist{0, (1ULL << NETWORK_WIDTH) - 1};
};

class SpMvNetworkMonitor : public sim::Controller<DeviceT, SpMvNetworkIf>
{
public:
    using sim::Controller<DeviceT, SpMvNetworkIf>::Controller;

    void reset() override {}

    void next() override
    {
        if (theDevice->out_valid)
        {
            for (int i = 0; i < NETWORK_WIDTH; i++)
            {
                theCurrentIntf.out_val[i] = theDevice->out_val[i];
                theCurrentIntf.out_id[i] = theDevice->out_id[i];
            }

            theCurrentIntf.out_valid = theDevice->out_valid;
            theCurrentIntf.out_ready = theDevice->out_ready;

            this->add(theCurrentIntf);
        }
    }

    SpMvNetworkIf theCurrentIntf{};
};

class SpMvNetworkTest {
public:
    using DriverT = SpMvNetworkDriver;
    using MonitorT = SpMvNetworkMonitor;
    using RandomVecFuncT = sim::FloatOpTest<SpMvNetworkIf, DeviceT, uint64_t>;

    SpMvNetworkTest(const std::string& testName = "")
        : theSimulation{
              std::format("SpMvNetworkTest{}{}", ((testName != "") ? "_" : ""), testName),
              sim::RunType::Release, sim::TraceOption::TraceOn,
              sim::ResetType::RANDOM_RESET, 1000000 },
          theDriver{ std::make_shared<DriverT>() },
          theMonitor{ std::make_shared<MonitorT>() }
    {
        theSimulation.addDriver(theDriver);
        theSimulation.addMonitor(theMonitor);
    }

    void addTestCase(int numCases = 1000, long seed = -1)
    {

        std::mt19937 myEngine(seed == -1 ? sim::initialize_rng() : seed);
        auto aRandomIdVector    = RandomVecFuncT::getRandomVector<uint64_t>(numCases, (1ULL << ID_WIDTH)-1 , myEngine);
        auto aRandomValueVector = RandomVecFuncT::getRandomVector<uint64_t>(numCases, (1ULL << (IN_WIDTH - 5))-1, myEngine);
        std::sort(aRandomIdVector.begin(), aRandomIdVector.end());
        writeVector(aRandomIdVector, aRandomValueVector, numCases, myEngine);
    }

    void writeVector(const std::vector<uint64_t> & anIdVectorA,
                     const std::vector<uint64_t> & aValueVectorA,
                     const uint64_t aTestSize,
                     auto anEngine)
    {
        ASSERT_EQ(anIdVectorA.size(), aValueVectorA.size());

        SpMvNetworkIf aStim{};
        std::uniform_int_distribution<uint64_t> aRandomDist{0, (1ULL << NETWORK_WIDTH) - 1};

        for (int i = 0; i < anIdVectorA.size(); i+=NETWORK_WIDTH)
        {
            for (int j = i; j < i + NETWORK_WIDTH; j++)
            {
                aStim.in_val[j - i] = aValueVectorA[j];
                aStim.in_id[j - i] = anIdVectorA[j];
            }

            if (i + NETWORK_WIDTH > aTestSize)
            {
                aStim.in_valid = aRandomDist(anEngine) & ((1ULL << (aTestSize - i)) - 1);
            }
            else
            {
                aStim.in_valid = aRandomDist(anEngine);
            }

            theDriver->add(aStim);
        }

        computeExpectedOutput();
    }

    std::map<uint64_t, uint64_t> idToSum(std::queue<SpMvNetworkIf> aQueue, bool aFromOutput = false)
    {
        std::map<uint64_t, uint64_t> id_to_sum;

        while (!aQueue.empty())
        {
            auto myFrontStim = aQueue.front();

            for (int i = 0; i < NETWORK_WIDTH; i++)
            {
                if (aFromOutput)
                {
                    if (sim::get_bit(myFrontStim.out_valid, i) && sim::get_bit(myFrontStim.out_ready, i))
                        id_to_sum[myFrontStim.out_id[i]] += myFrontStim.out_val[i];

                    continue;
                }

                if (sim::get_bit(myFrontStim.in_valid, i))
                    id_to_sum[myFrontStim.in_id[i]] += myFrontStim.in_val[i];
            }

            aQueue.pop();
        }

        return id_to_sum;
    }

    void computeExpectedOutput()
    {
        theExpectedOutput = idToSum(theDriver->getQueue());
    }

    void simulate(uint64_t aTestSize)
    {
        int counter = 0;
        theSimulation.simulate(
            [&]() { return counter++ >= aTestSize*100; }, 10
        );

        std::vector<uint64_t> theRecievedIds;
        std::vector<uint64_t> theRecievedElements;

        auto theRecievedSums = idToSum(theMonitor->getQueue(), true);

        EXPECT_EQ(theExpectedOutput, theRecievedSums)
            << "The Expected Output: "
            << sim::MapToHexString(theExpectedOutput)
            << std::endl
            << "The Recieved Output: "
            << sim::MapToHexString(theRecievedSums);

        // for (int i = 0; i < theRecievedIds.size(); i++)
        // {
        //     std::cout << "i: " << i << " RcvdId: " << theRecievedIds[i] << " RcvdOut: " << theRecievedElements[i] << std::endl;
        // }
    }

private:
    sim::Simulation<DeviceT> theSimulation;
    std::shared_ptr<DriverT> theDriver;
    std::shared_ptr<MonitorT> theMonitor;
    std::map<uint64_t, uint64_t> theExpectedOutput;
};

TEST(SpMvNetworkTest, BasicTest)
{
    auto tmp = sim::getTestName();
    auto test = SpMvNetworkTest(tmp);
    test.addTestCase(TEST_SIZE, 3960514134);
    test.simulate(TEST_SIZE);
}

TEST(SpMvNetworkTest, BasicTest_1)
{
    auto tmp = sim::getTestName();
    auto test = SpMvNetworkTest(tmp);
    test.addTestCase(TEST_SIZE);
    test.simulate(TEST_SIZE);
}

TEST(SpMvNetworkTest, BasicTest_2)
{
    auto tmp = sim::getTestName();
    auto test = SpMvNetworkTest(tmp);

    test.addTestCase(TEST_SIZE);
    test.addTestCase(TEST_SIZE);

    test.simulate(TEST_SIZE);
}
