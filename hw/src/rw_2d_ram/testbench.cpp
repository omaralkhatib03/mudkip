#include <cassert>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <cstring>
#include <xsi.h>
#include "Simulation.hpp"
#include "RamDriver.hpp"
#include "RamMonitor.hpp"

int main (int argc, char *argv[])
{
  using RamDriverT   = xvl::RamDriver;
  using RamMonitorT  = xvl::RamMonitor;

  auto myRamDriver           = std::make_shared<RamDriverT>("valid_addr_ps", "r_addr", "valid_w", "w_addr", "w_data");
  auto myRamMonitor          = std::make_shared<RamMonitorT>("r_data", "valid_data", "ready_pl");
  auto mySimulation           = xvl::Simulation("xsim.dir/work.rw_2d_ram/xsimk.so", SIMENGINE_LIB_NAME);

  mySimulation.addDriver(myRamDriver);
  mySimulation.addMonitor(myRamMonitor);
  
  for (unsigned int i = 0; i < 32; i++)
  {
    myRamDriver->add({{0}, {0}, {1}, {i}, {i+1}});
  }

  for (unsigned int i = 0; i < 32; i++)
  {
    myRamDriver->add({{1}, {i}, {0}, {0}, {0}});
  }

  mySimulation.simulate([&](){
    return myRamMonitor->getQueue().size() >= 32;
  });

  int cnt = 0;
  while (!myRamMonitor->getQueue().empty())
  {
    std::cout << cnt << ": " << myRamMonitor->getQueue().front().r_data[0] << std::endl;
    assert(cnt+1 ==  myRamMonitor->getQueue().front().r_data[0]);
    myRamMonitor->getQueue().pop();
    cnt++;
  }

  return 0;
}

