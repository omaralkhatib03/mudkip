
#include "Utils.hpp"
#include "Vspmv_network_tb.h"
#include "Vspmv_network_tb_spmv_network_tb.h"
#include <algorithm>
#include <cstdint>
#include <format>
#include <gtest/gtest.h>
#include <random>
#include "Controller.hpp"
#include "Simulation.hpp"
#include "Utils.hpp"
#include "Float.hpp"

using DeviceT = Vspmv_network_tb;

static constexpr uint64_t TESTED_NET_WIDTH_MAX  = 64;
static constexpr uint64_t NETWORK_WIDTH         = Vspmv_network_tb_spmv_network_tb::NETWORK_WIDTH;
static constexpr uint64_t IN_WIDTH              = Vspmv_network_tb_spmv_network_tb::IN_WIDTH;
static constexpr uint64_t ID_WIDTH              = Vspmv_network_tb_spmv_network_tb::ID_WIDTH;
static constexpr uint64_t OUT_WIDTH             = Vspmv_network_tb_spmv_network_tb::OUT_WIDTH;

struct SpMvNetworkIf
{
    static constexpr uint64_t PARALLELISM  = NETWORK_WIDTH;

    std::array<uint32_t, NETWORK_WIDTH>     in_id;
    std::array<uint32_t, NETWORK_WIDTH>     in_val;

    uint64_t                                in_valid : NETWORK_WIDTH;
    uint64_t                                in_ready : NETWORK_WIDTH;

    std::array<uint32_t, NETWORK_WIDTH>     out_id;
    std::array<uint32_t, NETWORK_WIDTH>     out_val;

    uint64_t                                out_valid : NETWORK_WIDTH;
    uint64_t                                out_ready : NETWORK_WIDTH;

    friend bool operator==(const SpMvNetworkIf &lhs, const SpMvNetworkIf &rhs)
    {
        return lhs.out_id == rhs.out_id &&
               lhs.out_val == rhs.out_val &&
               lhs.out_valid == rhs.out_valid;
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

    void driveIntf(SpMvNetworkIf aStim)
    {
        for (int i = 0; i < NETWORK_WIDTH; i++)
        {
            theDevice->in_val[i] = aStim.in_val[i];
            theDevice->in_id[i] = aStim.in_id[i];
        }

        theDevice->in_valid = aStim.in_valid;
    }

    void reset() override
    {
        SpMvNetworkIf resetStim = {};
        resetStim.out_ready = 0x3f;
        theDevice->out_ready = resetStim.out_ready;
        driveIntf(resetStim);
    }

    void next() override
    {
        if (!isControllerEmpty())
        {
            driveIntf(front());
            if (theDevice->in_ready)
                pop();
        }
        else
        {
            reset();
        }
    }
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

            add(theCurrentIntf);
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
        auto aRandomIdVector    = RandomVecFuncT::getRandomVector<uint64_t>(numCases, 10, seed);
        auto aRandomValueVector = RandomVecFuncT::getRandomVector<uint64_t>(numCases, 2, seed);
        std::sort(aRandomIdVector.begin(), aRandomIdVector.end());
        writeVector(aRandomIdVector, aRandomValueVector);
    }
    
    void writeVector(const std::vector<uint64_t> & anIdVectorA, const std::vector<uint64_t> & aValueVectorA)
    {
        ASSERT_EQ(anIdVectorA.size(), aValueVectorA.size());
        
        SpMvNetworkIf aStim{};

        for (int i = 0; i < anIdVectorA.size(); i+=NETWORK_WIDTH)
        {
            for (int j = i; j < i + NETWORK_WIDTH; j++)
            {
                aStim.in_val[j - i] = aValueVectorA[j];  
                aStim.in_id[j - i] = anIdVectorA[j];  
            }

            #pragma clang diagnostic push
            #pragma clang diagnostic ignored "-Wbitfield-constant-conversion"
            #pragma clang diagnostic ignored "-Woverflow"
            aStim.in_valid = (1UL << IN_WIDTH) - 1UL;
            #pragma clang diagnostic pop
            theDriver->add(aStim);
        }
    }

    void simulate()
    {
        // theSimulation.simulate(
        //     [&]() { return theMonitor->getQueue().size() >= theExpectedOutput.size(); }, 10);

        theSimulation.simulate(
            [&]() { return theDriver->getQueue().size() == 0; }, 10);

        // sim::compareQueues<SpMvNetworkIf>(theExpectedOutput, theMonitor->getQueue());
    }

private:
    sim::Simulation<DeviceT> theSimulation;
    std::shared_ptr<DriverT> theDriver;
    std::shared_ptr<MonitorT> theMonitor;
    std::queue<SpMvNetworkIf> theExpectedOutput;
};

TEST(SpMvNetworkTest, BasicTest)
{
    auto tmp = sim::getTestName();
    auto test = SpMvNetworkTest(tmp);
    test.addTestCase(10);
    test.simulate();
}

