#include <gtest/gtest.h>
#include <sys/types.h>

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "Interface.hpp"
#include <format>
#include "Simulation.hpp"
#include "Utils.hpp"
#include "Vso_rr_arbiter.h"
#include "Vso_rr_arbiter_so_rr_arbiter.h"
#include "Driver.hpp"
#include "Monitor.hpp"

using DeviceT = Vso_rr_arbiter;

constexpr size_t NUM_INPUTS = Vso_rr_arbiter_so_rr_arbiter::NUM_INPUTS;
constexpr size_t DATA_WIDTH = Vso_rr_arbiter_so_rr_arbiter::DATA_WIDTH;

using SoRrArbiterIfT = SoRrArbiterIf<DATA_WIDTH, NUM_INPUTS>;

template <typename DataT = uint8_t>
class SoRrArbiterTest
{
public:

    using DriverT = SoRrArbiterDriver<DeviceT, SoRrArbiterIfT>;
    using MonitorT = SoRrArbMonitor<DeviceT, SoRrArbiterIfT>;

    SoRrArbiterTest(const std::string &aTestName = "")
        : theSimulation{ std::format("so_rr_arb_test{}{}", ((aTestName != "") ? "_" : ""), aTestName),
                         sim::RunType::Release, sim::TraceOption::TraceOn, sim::ResetType::RANDOM_RESET, 10000 },
          theArbDriver{ std::make_shared<DriverT>() },
          theArbMonitor{ std::make_shared<MonitorT>() }
    {
        theSimulation.addDriver(theArbDriver);
        theSimulation.addMonitor(theArbMonitor);
    }

    virtual ~SoRrArbiterTest() = default;

    void addTestCase(uint32_t aReq, std::array<DataT, NUM_INPUTS> anIn, bool aReady)
    {
        SoRrArbiterIfT myStim = {};
        myStim.req = aReq;
        memcpy(myStim.in.data(), anIn.data(), NUM_INPUTS);
        myStim.ready = aReady;
        this->theArbDriver->add(myStim);

        for (size_t i = 0; i < NUM_INPUTS; i++)
        {
            if (aReq & (1 << i))
            {
                theExpectedOutput.push(anIn[i]);
            }
        }
    }
    
    void simulate()
    {
        theSimulation.simulate(
        [&]() { return theArbMonitor->getQueue().size() >= theExpectedOutput.size(); }, 10);

        auto myTmp = theArbMonitor->getQueue();
        auto myTmpQueue = std::queue<DataT>();

        while (!myTmp.empty())
        {
            myTmpQueue.push(myTmp.front().dout);
            myTmp.pop();
        }

        sim::compareQueues<DataT>(this->theExpectedOutput, myTmpQueue);
    }

private:
    sim::Simulation<DeviceT> theSimulation;
    std::shared_ptr<DriverT> theArbDriver;
    std::shared_ptr<MonitorT> theArbMonitor;
    std::queue<DataT> theExpectedOutput;
};


