#include "AxiMasterDriver.hpp"
#include "AxiMasterMonitor.hpp"
#include "Simulation.hpp"
#include <cstddef>
#include <verilated.h>
#include "Vrf_top.h"
#include "Vrf_top_rf_top.h"
#include "AxiUtils.hpp"

using DeviceT                                             = Vrf_top;

static constexpr size_t DATA_WIDTH    = Vrf_top_rf_top::DATA_WIDTH;
static constexpr size_t ADDR_WIDTH    = Vrf_top_rf_top::ADDR_WIDTH;

using RfDriverT = sim::AxiMasterDriver<DeviceT, DATA_WIDTH, ADDR_WIDTH>;
using RfMonitorT = sim::AxiMasterMonitor<DeviceT, DATA_WIDTH>;

using MonitorIfT = sim::AxiMasterMonitorIntf<DATA_WIDTH>;

int main (int argc, char *argv[])
{
    auto mySimulation     = sim::Simulation<DeviceT>(argc, argv, "rf_top", sim::RunType::Release);
    auto myRfDriver         = std::make_shared<RfDriverT>();
    auto myRfMonitor        = std::make_shared<RfMonitorT>();

    mySimulation.addDriver(myRfDriver);
    mySimulation.addMonitor(myRfMonitor);

    sim::writeToAddress<RfDriverT, DATA_WIDTH, ADDR_WIDTH>(myRfDriver, 1, 32);
    sim::readAddress<RfDriverT, DATA_WIDTH, ADDR_WIDTH>(myRfDriver, 1);

    mySimulation.simulate([&](){
        return myRfMonitor->getQueue().size() > 0;
    }, 5);

    MonitorIfT aCapturedData = myRfMonitor->getQueue().front();

    std::cout << aCapturedData.rdata << std::endl;

}
