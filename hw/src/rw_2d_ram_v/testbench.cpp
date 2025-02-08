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

  auto myRamDriver           = std::make_shared<RamDriverT>("s_axis_arvalid", "s_axis_raddr", "s_axis_rready", "s_axis_awvalid", "s_axis_wvalid", "s_axis_waddr", "s_axis_wdata", "s_axis_arready");
  auto myRamMonitor          = std::make_shared<RamMonitorT>("s_axis_rdata", "s_axis_rvalid", "s_axis_arready");
  auto mySimulation          = xvl::Simulation("xsim.dir/work.rw_2d_ram_v/xsimk.so", SIMENGINE_LIB_NAME);

  mySimulation.addDriver(myRamDriver);
  mySimulation.addMonitor(myRamMonitor);
  
  for (unsigned int i = 0; i < 32; i++)
  {
    myRamDriver->add({{0}, {0}, {0}, {1}, {1}, {i}, {i+1}});
  }

  for (unsigned int i = 0; i < 32; i++)
  {
    myRamDriver->add({{1}, {i}, {static_cast<unsigned>((rand()) % 2)}, {0}, {0}});
  }

  mySimulation.simulate([&](){
    return myRamMonitor->getQueue().size() >= 32;
  });

  int cnt = 0;
  while (!myRamMonitor->getQueue().empty())
  {
    /*std::cout << "Address: "<< cnt << ": " << myRamMonitor->getQueue().front().s_axis_rdata[0] << std::endl;*/
    assert(cnt+1 ==  myRamMonitor->getQueue().front().s_axis_rdata[0]);
    myRamMonitor->getQueue().pop();
    cnt++;
  }
  
  for (unsigned int i = 0; i < 32; i++)
  {
    myRamDriver->add({{0}, {0}, {0}, {1}, {1}, {i}, {i+1}});
  }

  for (unsigned int i = 0; i < 32; i++)
  {
    myRamDriver->add({{1}, {i}, {1}, {0}, {0}});
  }

  mySimulation.simulate([&](){
    return myRamMonitor->getQueue().size() >= 32;
  });

  cnt = 0;
  while (!myRamMonitor->getQueue().empty())
  {
    /*std::cout << "Address: "<< cnt << ": " << myRamMonitor->getQueue().front().s_axis_rdata[0] << std::endl;*/
    assert(cnt+1 ==  myRamMonitor->getQueue().front().s_axis_rdata[0]);
    myRamMonitor->getQueue().pop();
    cnt++;
  }

  return 0;
}

