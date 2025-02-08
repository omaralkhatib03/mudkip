#include <cassert>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <cstring>
#include <xsi.h>
#include "FifoDriver.hpp"
#include "FifoMonitor.hpp"
#include "Simulation.hpp"

int main (int argc, char *argv[])
{
  using FifoDriverT   = xvl::FifoDriver;
  using FifoMonitorT  = xvl::FifoMonitor;

  auto myFifoDriver           = std::make_shared<FifoDriverT>("din", "shift_in", "shift_out");
  auto myFifoMonitor          = std::make_shared<FifoMonitorT>("shift_out", "dout", "full", "empty", "overflow", "underflow", "valid");
  auto mySimulation           = xvl::Simulation("xsim.dir/work.basic_sync_fifo/xsimk.so", SIMENGINE_LIB_NAME);

  mySimulation.addDriver(myFifoDriver);
  mySimulation.addMonitor(myFifoMonitor);

  myFifoDriver->add({{0xdeadbeef}, {1}, {0}});
  myFifoDriver->add({{0}, {0}, {1}});

  mySimulation.simulate([&](){
    return myFifoMonitor->getQueue().size() > 0;
  });

  xvl::FifoMonitorIntf aCapturedData = myFifoMonitor->getQueue().front();

  std::cout << std::hex;
  std::cout << "Expected data: " << 0xdeadbeef << " CapturedSignal: " << aCapturedData.dout[0] << std::endl;
  assert(aCapturedData.dout[0] == 0xdeadbeef);

  // Should Fail. This is for testing purposes
  /*assert(aCapturedData.dout[0] == 0xdeadbeea);*/

  myFifoMonitor->getQueue().pop();

  return 0;
}

