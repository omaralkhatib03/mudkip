#pragma once

#include "ControllerBase.hpp"
#include "Signal.hpp"
#include "xsi_loader.h"
#include "Global.hpp"
#include <cstring>
#include <functional>
#include <iostream>
#include <vector>
#include <xsi.h>

namespace xvl
{

static const auto theDefaultWaitSim = [](){return true;};

class Simulation
{
  enum class SIMULATION_STATE
  {
    NOT_INITIALISED = 0,
    INITIALISED = 1
  };

  using ControllerT = ControllerBase;
public:

  Simulation() = delete;
  Simulation(std::string_view aDesignLib, std::string_view aSimLib);

  void simulate(std::function<bool()> aPredicate = theDefaultWaitSim, size_t aWaitValue = 1, size_t anIncrement = 1);
  void resetDrivers();

  void addDriver(std::shared_ptr<ControllerT> aController);
  void addMonitor(std::shared_ptr<ControllerT> aController);

  bool getStatus() const;
  std::string getErrorInfo() const;

  virtual ~Simulation()
  {
    std::cout << "Simulation Info: \n";
    std::cout << theSimulation->get_error_info() << "\n";
    theSimulation->close();
  };

  void restartSim();

private:

  void run_cycle(int aCycles);
  void run_half_cycle(const XsiBit & aClkValue);

  bool isSimulationOver(std::function<bool()> aPredicate = theDefaultWaitSim);
  void initialiseSimulation();

  std::vector<std::shared_ptr<ControllerT>> theMonitors;
  std::vector<std::shared_ptr<ControllerT>> theDrivers;
  std::shared_ptr<Xsi::Loader> theSimulation;

  PortNumber theClk;
  PortNumber theReset;

  s_xsi_setup_info theDeviceInfo;

  SIMULATION_STATE theSimulationState;
};

}
