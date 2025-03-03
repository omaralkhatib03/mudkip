#pragma once

#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include "ControllerBase.hpp"
#include "verilated.h"
#include "verilated_fst_c.h"

namespace sim
{

typedef uint64_t SimTimeT;

static const auto theDefaultWaitSim = [](){return true;};

enum class ResetType
{
    RANDOM_RESET    = 2,
    ONE_RESET         = 1,
    ZERO_RESET        = 0
};

enum class RunType
{
    Debug                 = 1,
    Release             = 0
};

enum class TraceOption
{
    TraceOn             = 1,
    TraceOff            = 0
};

template <DeviceT DutT>
class Simulation
{
public:
    using ControllerT = ControllerBase<DutT>;

    Simulation(
        std::string aWaveName = "dump",
        RunType aRunOption = RunType::Debug,
        TraceOption aTraceOption = TraceOption::TraceOn,
        ResetType aResetOption = ResetType::RANDOM_RESET,
        uint32_t aMaxSimTime = 100
    );

    Simulation(
        int argc, char *argv[],
        std::string aWaveName = "dump",
        RunType aRunOption = RunType::Debug,
        TraceOption aTraceOption = TraceOption::TraceOn,
        ResetType aResetOption = ResetType::RANDOM_RESET,
        uint32_t aMaxSimTime = 100
    );

    void simulate(std::function<bool()> aPredicate = theDefaultWaitSim, size_t aWaitValue = 1, size_t anIncrement = 1);

    void resetDrivers();
    void addDriver(std::shared_ptr<ControllerT> aController);
    void addMonitor(std::shared_ptr<ControllerT> aController);

    bool getStatus() const;
    std::string getErrorInfo() const;

    virtual ~Simulation(){};

    void restartSim();

private:

    void run_cycle(int aCycles);
    void run_half_cycle();
    bool isSimulationOver(std::function<bool()> aPredicate = theDefaultWaitSim);
    void initialiseSimulation();
    void dump();

    std::vector<std::shared_ptr<ControllerT>> theMonitors;
    std::vector<std::shared_ptr<ControllerT>> theDrivers;
    std::shared_ptr<VerilatedContext> theVerilatedContext;
    std::shared_ptr<DutT> theDut;
    std::shared_ptr<VerilatedFstC> theTrace;
    SimTimeT theSimTime;
    uint32_t theMaxSimTime;
};

template<DeviceT DutT>
Simulation<DutT>::Simulation(
    std::string aWaveName,
    RunType aRunOption,
    TraceOption aTraceOption,
    ResetType aResetOption,
    uint32_t aMaxSimTime
)
    : theMonitors{},
        theDrivers{},
        theVerilatedContext(
            new VerilatedContext,
            [&](VerilatedContext * aContext)
            {
                delete aContext;
            }
        ),
        theDut(
            new DutT,
            [&](DutT* aDut)
            {
                aDut->final();
            }
        ),
        theTrace(
            new VerilatedFstC,
            [&](VerilatedFstC* aTrace)
            {
                aTrace->close();
                delete aTrace;
            }
        ),
        theSimTime{0},
        theMaxSimTime{aMaxSimTime}
{
        theVerilatedContext->debug(static_cast<int>(aRunOption));
        theVerilatedContext->randReset(static_cast<int>(aResetOption));
        theVerilatedContext->traceEverOn(static_cast<bool>(aTraceOption));

        Verilated::mkdir("waves");

        theDut->trace(theTrace.get(), 99);
        theTrace->open(("waves/" + aWaveName + ".fst").c_str());

    #ifndef COMBINATIONAL
        theDut->rst_n = 1;
    #endif // !COMBINATIONAL
}

template<DeviceT DutT>
Simulation<DutT>::Simulation(
    int argc,
    char *argv[],
    std::string aWaveName,
    RunType aRunOption,
    TraceOption aTraceOption,
    ResetType aResetOption,
    uint32_t aMaxCycles
)
    : sim::Simulation<DutT>(aWaveName, aRunOption, aTraceOption, aResetOption, aMaxCycles)
{
     Verilated::commandArgs(argc, argv);
}

template<DeviceT DutT>
bool Simulation<DutT>::isSimulationOver(std::function<bool()> aPredicate)
{
    bool myAnsr = true;

    for (auto & myDriver : theDrivers)
    {
        myAnsr &= myDriver->isControllerEmpty();
    }

    return myAnsr && aPredicate();
}

template<DeviceT DutT>
void Simulation<DutT>::dump()
{
    theTrace->dump(theSimTime);
    theSimTime++;
}

template<DeviceT DutT>
void Simulation<DutT>::run_half_cycle()
{
    #ifndef COMBINATIONAL
    theDut->clk ^= 1;
    #endif // !COMBINATIONAL
    theDut->eval();
}

template<DeviceT DutT>
void Simulation<DutT>::resetDrivers()
{
        for (auto & myDriver : theDrivers)
        {
            myDriver->reset();
        }
}

template<DeviceT DutT>
void Simulation<DutT>::addDriver(std::shared_ptr<ControllerT> aController)
{
    theDrivers.push_back(aController);
    aController->init(theDut);
}

template<DeviceT DutT>
void Simulation<DutT>::addMonitor(std::shared_ptr<ControllerT> aController)
{
    theMonitors.push_back(aController);
    aController->init(theDut);
}

template<DeviceT DutT>
void Simulation<DutT>::Simulation::initialiseSimulation()
{
    run_cycle(1);

    for (int i = 0; i < 4; i++)
    {
        #ifndef COMBINATIONAL
            theDut->rst_n = 0;
        #endif // !COMBINATIONAL
        resetDrivers();
        run_half_cycle();
        dump();
    }

    #ifndef COMBINATIONAL
    theDut->rst_n = 1;
    #endif // !COMBINATIONAL

    run_cycle(1);
}

template<DeviceT DutT>
void Simulation<DutT>::run_cycle(int aCycles)
{
    for (int i = 0 ; i < aCycles; i++)
    {
        run_half_cycle();
        dump();
        run_half_cycle();
        dump();
    }
}

template<DeviceT DutT>
void Simulation<DutT>::simulate(std::function<bool()> aPredicate, size_t aWaitValue, size_t anIncrement)
{
    initialiseSimulation();

    bool myStopSimulation = !isSimulationOver(aPredicate);
    while ((myStopSimulation || (aWaitValue > 0)) && theSimTime < theMaxSimTime)
    {
        run_half_cycle();

        #ifdef COMBINATIONAL
            if (1)
        #endif // COMBINATIONAL
        #ifndef COMBINATIONAL
            if (theDut->clk)
        #endif // !COMBINATIONAL
        {
            aWaitValue -= !myStopSimulation;

            for (auto & myDriver : theDrivers)
            {
                myDriver->next();
            }

            theDut->eval();

            for (auto & myMonitor : theMonitors)
            {
                myMonitor->next();
            }

        }

        dump();

        myStopSimulation = !isSimulationOver(aPredicate);
    }

}

}

