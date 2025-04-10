#define COMBINATIONAL
#include "Utils.hpp"
#include "Vspmv_network_op_tb.h"
#include "Vspmv_network_op_tb_spmv_network_op_tb.h"
#include <cstdint>
#include <format>
#include <random>
#include "Controller.hpp"
#include "Simulation.hpp"

using  DeviceT = Vspmv_network_op_tb;

// TODO: Possibly increase verified BITWIDTH
static constexpr uint64_t MAX_TESTED_BITWIDTH   = 64;
static constexpr uint64_t MAX_UINT_ARRAY_LEN    = MAX_TESTED_BITWIDTH / 32;
static constexpr uint64_t MAX_VECTOR_LENGTH     = 4096;
static constexpr uint64_t IN_WIDTH              = Vspmv_network_op_tb_spmv_network_op_tb::IN_WIDTH;
static constexpr uint64_t ID_WIDTH              = Vspmv_network_op_tb_spmv_network_op_tb::ID_WIDTH;
static constexpr uint64_t LOCATION              = Vspmv_network_op_tb_spmv_network_op_tb::LOCATION;
static constexpr uint64_t PARALLELISM           = Vspmv_network_op_tb_spmv_network_op_tb::PARALLELISM;
static constexpr uint64_t OUT_WIDTH             = Vspmv_network_op_tb_spmv_network_op_tb::OUT_WIDTH;
static constexpr bool TOWARDS_CENTER            = LOCATION > sim::ceil(static_cast<float>(PARALLELISM) / 2);

#if IN_WIDTH > 32
    #define LONG
#elif IN_WIDTH > 64
    #define ARRAY_LIKE
#endif

struct SpMvNetworkOpIf
{
    // Inputs
    uint32_t                                        in_a_id : ID_WIDTH;

    #ifdef ARRAY_LIKE
        std::array<uint32_t, MAX_UINT_ARRAY_LEN>    in_a_val;
    #else
        uint64_t                                    in_a_val : IN_WIDTH;
    #endif // LONG
    //
    uint32_t                                        in_b_id : ID_WIDTH;

    #ifdef ARRAY_LIKE
        std::array<uint32_t, MAX_UINT_ARRAY_LEN>    in_b_val;
    #else
        uint64_t                                    in_b_val : IN_WIDTH;
    #endif // LONG
    //

    bool                                            in_valid;
    bool                                            ready;

    // Outputs
    uint32_t                                        a_id : ID_WIDTH;
    #ifdef ARRAY_LIKE
        std::array<uint32_t, MAX_UINT_ARRAY_LEN>    a_val;
    #else
        uint64_t                                    a_val : IN_WIDTH;
    #endif // LONG
    //

    bool                                            a_valid;
    uint32_t                                        b_id : ID_WIDTH;

    #ifdef ARRAY_LIKE
        std::array<uint32_t, MAX_UINT_ARRAY_LEN>    b_val;
    #else
        uint64_t                                    b_val: IN_WIDTH;
    #endif // LONG
    //

    bool                                            b_valid;
    bool                                            in_ready;

    friend bool operator==(const SpMvNetworkOpIf & lhs, const SpMvNetworkOpIf & rhs)
    {
        return  lhs.a_id     == rhs.a_id &&
                lhs.a_val    == rhs.a_val &&
                lhs.a_valid  == rhs.a_valid &&
                lhs.b_id     == rhs.b_id &&
                lhs.b_val    == rhs.b_val &&
                lhs.b_valid  == rhs.b_valid;
    }

    friend std::ostream& operator<<(std::ostream& out, const SpMvNetworkOpIf& f)
    {
        out << "SpMvNetworkOpIf {\n";
        out << "  in_a_id: "   << f.in_a_id << "\n";
        out << "  in_a_val: "  << std::hex <<  f.in_a_val << "\n";
        out << "  in_b_id: "   << f.in_b_id << "\n";
        out << "  in_b_val: "  << f.in_b_val << "\n";
        out << "  in_valid: "  << std::boolalpha << f.in_valid << "\n";

        out << "  a_id: "      << f.a_id << "\n";
        out << "  a_val: "     << f.a_val << "\n";
        out << "  a_valid: "   << std::boolalpha << f.a_valid << "\n";

        out << "  b_id: "      << f.b_id << "\n";
        out << "  b_val: "     << f.b_val << "\n";
        out << "  b_valid: "   << std::boolalpha << f.b_valid << "\n";

        out << "}";
        return out;
    }
};

class SpMvNetworkOpDriver : public sim::Controller<DeviceT, SpMvNetworkOpIf>
{
public:
    using sim::Controller<DeviceT, SpMvNetworkOpIf>::Controller;

    void driveIntf(SpMvNetworkOpIf aStim)
    {
        theDevice->in_valid     = aStim.in_valid;
        theDevice->in_b_id      = aStim.in_b_id;

        theDevice->in_a_id     = aStim.in_a_id;

        #ifdef ARRAY_LIKE
        for (int i = 0 ; i < IN_WIDTH / 32; i++)
        {
            this->theDevice->in_a_val[i] = aStim.in_a_val[i];
            this->theDevice->in_b_val[i] = aStim.in_b_val[i];
        }
        #else
           this->theDevice->in_b_val = aStim.in_b_val;
           this->theDevice->in_a_val = aStim.in_a_val;
        #endif // ARRAY_LIKE

        this->theDevice->ready = 1;
    }

    void reset() override
    {
        auto myRstStim  = SpMvNetworkOpIf{0};
        myRstStim.ready = 1;
        driveIntf(myRstStim);
    }

    void next() override
    {
        if (!this->isControllerEmpty())
        {
            driveIntf(this->front());

            if (this->theDevice->in_ready)
                this->pop();

            return;
        }
        reset();
    }

};

class SpMvNetworkOpMonitor : public sim::Controller<DeviceT, SpMvNetworkOpIf>
{
public:
    using sim::Controller<DeviceT, SpMvNetworkOpIf>::Controller;

    void reset() override {}

    void next() override
    {
        theCurrentIntf.b_valid  = theDevice->b_valid;
        theCurrentIntf.b_id     = theDevice->b_id;

        theCurrentIntf.a_valid  = theDevice->a_valid;
        theCurrentIntf.a_id     = theDevice->a_id;

        #ifdef ARRAY_LIKE
        for (int i = 0 ; i < IN_WIDTH / 32; i++)
        {
            theCurrentIntf.a_val[i] = this->theDevice->a_val[i]
            theCurrentIntf.b_val[i] = this->theDevice->b_val[i]
        }
        #else
            theCurrentIntf.b_val = this->theDevice->b_val;
            theCurrentIntf.a_val = this->theDevice->a_val;
        #endif // ARRAY_LIKE

        if (theDevice->b_valid || theDevice->a_valid)
        {
            if (!theDevice->b_valid)
            {
                theCurrentIntf.b_val = 0;
            }

            this->add(theCurrentIntf);
        }
    }

    SpMvNetworkOpIf theCurrentIntf{};

};

template <typename DataT = uint8_t>
class SpMvNetworkOpTest {
public:

    using DriverT = SpMvNetworkOpDriver;
    using MonitorT = SpMvNetworkOpMonitor;

    SpMvNetworkOpTest(const std::string &aTestName = "")
        : theSimulation{ std::format("SpMvNetworkOpTest{}{}", ((aTestName != "") ? "_" : ""), aTestName),
                         sim::RunType::Release, sim::TraceOption::TraceOn, sim::ResetType::RANDOM_RESET, 1000000 },
          theSpMvNetwrkOpDriver{ std::make_shared<DriverT>() },
          theSpMvNetwrkOpMonitor{ std::make_shared<MonitorT>() }
    {
        theSimulation.addDriver(theSpMvNetwrkOpDriver);
        theSimulation.addMonitor(theSpMvNetwrkOpMonitor);
    }

    virtual ~SpMvNetworkOpTest() = default;


    void addTestCase(int aNumberOfCases = 1, long aSeed = -1)
    {
        auto myRng = std::mt19937((aSeed != -1) ? aSeed : sim::initialize_rng());

        auto myDisInt = std::uniform_int_distribution<uint64_t>(0, (1UL << IN_WIDTH));
        auto myDisIntId = std::uniform_int_distribution<uint64_t>(0, (1UL << ID_WIDTH));

        for (int i = 0; i < aNumberOfCases; i++)
        {
            SpMvNetworkOpIf myStim = {};

            myStim.in_valid     = myDisInt(myRng) % 2;
            myStim.in_a_val     = myDisInt(myRng);
            myStim.in_b_val     = myDisInt(myRng);

            myStim.in_a_id      = std::abs((int) myDisIntId(myRng) % 10); // 1/10 chance that they are equal
            myStim.in_b_id      = std::abs((int) myDisIntId(myRng) % 10);

            theSpMvNetwrkOpDriver->add(myStim);
        }

        computeExpectedOutput();
    }

    void computeExpectedOutput()
    {
        auto myCopiedQueue(theSpMvNetwrkOpDriver->getQueue());

        while (!myCopiedQueue.empty())
        {
            auto myExpectedOut = SpMvNetworkOpIf{};
            auto myTopStim = myCopiedQueue.front();
            myCopiedQueue.pop();

            if (!myTopStim.in_valid)
                continue;

            if (myTopStim.in_a_id == myTopStim.in_b_id)
            {
                myExpectedOut.a_id  = myTopStim.in_b_id;
                myExpectedOut.b_id  = myTopStim.in_b_id;
                myExpectedOut.a_val = myTopStim.in_a_val + myTopStim.in_b_val;

                if (TOWARDS_CENTER)
                {
                    myExpectedOut.a_valid = myTopStim.in_valid;
                    myExpectedOut.b_valid = 0;
                }
                else
                {
                    myExpectedOut.b_valid   = myTopStim.in_valid;
                    myExpectedOut.b_val     = myExpectedOut.a_val;
                    myExpectedOut.a_valid   = 0;
                }
            }
            else
            {
                myExpectedOut.a_id      = myTopStim.in_a_id;
                myExpectedOut.b_id      = myTopStim.in_b_id;
                myExpectedOut.a_val     = myTopStim.in_a_val;
                myExpectedOut.b_val     = myTopStim.in_b_val;
                myExpectedOut.a_valid   = myTopStim.in_valid;
                myExpectedOut.b_valid   = myTopStim.in_valid;
            }

            theExpectedOutput.push(myExpectedOut);
        }
    }

    void simulate()
    {
        theSimulation.simulate(
        [&]() { return theSpMvNetwrkOpMonitor->getQueue().size() >= theExpectedOutput.size(); }, 10);
        sim::compareQueues<SpMvNetworkOpIf>(this->theExpectedOutput, theSpMvNetwrkOpMonitor->getQueue());
    }

private:
    sim::Simulation<DeviceT> theSimulation;
    std::shared_ptr<DriverT> theSpMvNetwrkOpDriver;
    std::shared_ptr<MonitorT> theSpMvNetwrkOpMonitor;
    std::queue<SpMvNetworkOpIf> theExpectedOutput;
};

TEST(SpMvNetworkOpTest, CasesTest)
{
    auto myTest = SpMvNetworkOpTest(sim::getTestName());
    myTest.addTestCase(1e4);
    myTest.simulate();
}
