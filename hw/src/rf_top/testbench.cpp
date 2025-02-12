#include "Simulation.hpp"
#include <memory>
#include <verilated.h>
#include "Vrf_top.h"

using DeviceT     = Vrf_top;

int main (int argc, char *argv[])
{
  auto mySimulation   = sim::Simulation<DeviceT>(argc, argv, "rf_top", sim::RunType::Release);

}

