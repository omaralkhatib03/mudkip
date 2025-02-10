#include <cstdlib>
#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <cstring>
#include <xsi.h>
#include "Simulation.hpp"

int main (int argc, char *argv[])
{

  xvl::Simulation theSimulation("xsim.dir/work.fl_vadd_top/xsimk.so", SIMENGINE_LIB_NAME);
  theSimulation.simulate();

  if (theSimulation.getStatus() != xsiNormal)
  {
    std::cout << theSimulation.getStatus() << "\n";
    std::cout << theSimulation.getErrorInfo() << "\n";
  }

  std::cout << "PASSED !\n";

  return 0;
}

