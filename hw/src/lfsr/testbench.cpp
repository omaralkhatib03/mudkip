#define COMBINATIONAL

#include "Signal.hpp"
#include "Controller.hpp"
#include <memory>
#include "Simulation.hpp"
#include <verilated.h>
#include "Vlfsr.h"
#include "Vlfsr_lfsr.h"

using DeviceT = Vlfsr;

static constexpr size_t StateWidth = Vlfsr_lfsr::LFSR_WIDTH;
static constexpr size_t DataWidth = Vlfsr_lfsr::DATA_WIDTH;

#pragma pack(push, 1)
struct LfsrIntf
{
    sim::Signal<DataWidth> theData;
    sim::Signal<StateWidth> theState;
};
#pragma pack(pop)

class LfsrDriver : public sim::Controller<DeviceT, LfsrIntf>
{
public:
    using Controller<DeviceT, LfsrIntf>::Controller;

    void driveFifoIntf(LfsrIntf anLfsrIntf)
    {
        this->theDevice->state_in     =     *anLfsrIntf.theState.data();
        this->theDevice->data_in        =     *anLfsrIntf.theData.data();
    }

    void reset() override
    {
        driveFifoIntf({0x0, 1});
    }

    void next() override
    {
        this->driveFifoIntf({rand(), this->theDevice->state_out});
    }

};

int main (int argc, char *argv[])
{
    auto mySimulation     = sim::Simulation<DeviceT>(argc, argv, "lfsr", sim::RunType::Release);
    auto myDriver             = std::make_shared<LfsrDriver>();

    mySimulation.addDriver(myDriver);
    mySimulation.simulate(sim::theDefaultWaitSim, 100);
}

