#include "FifoDriver.hpp"
#include "FifoMonitor.hpp"
#include "Simulation.hpp"
#include <memory>
#include <verilated.h>
#include "Vbasic_sync_fifo.h"

using DeviceT     = Vbasic_sync_fifo;
using FifoMonitorT = sim::FifoMonitor<DeviceT>; 
using FifoDriverT = sim::FifoDriver<DeviceT>; 

int main (int argc, char *argv[])
{
  auto mySimulation   = sim::Simulation<DeviceT>(argc, argv, "basic_sync_fifo", sim::RunType::Release);

  auto myFifoDriver   = std::make_shared<FifoDriverT>();
  auto myFifoMonitor  = std::make_shared<FifoMonitorT>();
  
  mySimulation.addDriver(myFifoDriver);
  mySimulation.addMonitor(myFifoMonitor);

  myFifoDriver->add({0xdeadbeef, 1, 0});
  myFifoDriver->add({0, 0, 0});
  myFifoDriver->add({0, 0, 0});
  myFifoDriver->add({0, 0, 1});

  mySimulation.simulate([&](){
    return myFifoMonitor->getQueue().size() > 0;
  }, 5);
  
  sim::FifoMonitorIntf aCapturedData = myFifoMonitor->getQueue().front();

  std::cout << std::hex;
  std::cout << "Expected data: " << 0xdeadbeef << " CapturedSignal: " << aCapturedData.dout[0] << std::endl;

  assert(aCapturedData.dout[0] == 0xdeadbeef);
  
  myFifoMonitor->getQueue().pop();
}

