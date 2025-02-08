#include <cstdlib>
#include <iostream>
#include <ostream>
#include <stdlib.h>
#include <cstring>
#include <xsi.h>
#include "Simulation.hpp"

int main (int argc, char *argv[])
{

  xvl::Simulation theSimulation("xsim.dir/work.vector_adder_tb/xsimk.so", "libxv_simulator_kernel.so");
  theSimulation.simulate();

  if (theSimulation.getStatus() != xsiNormal)
  {
    std::cout << theSimulation.getStatus() << "\n";
    std::cout << theSimulation.getErrorInfo() << "\n";
  }

  std::cout << "PASSED !\n";

  return 0;
}

