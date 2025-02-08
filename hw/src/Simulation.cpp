#include "Global.hpp"
#include "Simulation.hpp"

namespace xvl 
{

bool Simulation::isSimulationOver(std::function<bool()> aPredicate)
{
  bool myAnsr = true;

  for (auto & myDriver : theDrivers)
  {
    myAnsr &= myDriver->isControllerEmpty();
  }

  return myAnsr && aPredicate();
}

Simulation::Simulation(std::string_view aDesignLib, std::string_view aSimLib) :
  theMonitors{},
  theDrivers{},
  theSimulation{std::make_shared<Xsi::Loader>(aDesignLib.data(), aSimLib.data())},
  theSimulationState{SIMULATION_STATE::NOT_INITIALISED}
{
  memset(&theDeviceInfo, 0, sizeof(theDeviceInfo));
  theDeviceInfo.logFileName = (char *) WDB_FILE_NAME.c_str();

  try {
    theSimulation->open(&theDeviceInfo);
  } catch (std::exception& e) {
    std::cerr << "What ? Could not open XsiInstance \n";
    std::cerr << e.what();
    throw;
  }

  theSimulation->trace_all();

  theReset  = theSimulation->get_port_number(TOP_RESET_NAME.c_str());
  if (theReset < 0) throw std::runtime_error("What ? Could not obtain global reset");
  theClk    = theSimulation->get_port_number(TOP_CLK_NAME.c_str());
  if (theClk < 0) throw std::runtime_error("What ? Could not obtain global clock");

  // Sanity Check
  theSimulation->put_value(theReset, &HIGH);
}

void Simulation::run_half_cycle(const XsiBit & aClkValue)
{
  theSimulation->put_value(theClk, &aClkValue);
  theSimulation->run(1);
}

void Simulation::run_cycle(int aCycles)
{
  for (int i = 0 ; i < aCycles; i++)
  {
    run_half_cycle(HIGH);
    run_half_cycle(LOW);
  }

  if (theSimulation->get_status() != xsiNormal)
  {
    throw std::runtime_error("What? call to run_cycle with non-Normal Status: " + std::to_string(theSimulation->get_status()));
  }
}

void Simulation::addDriver(std::shared_ptr<ControllerT> aController)
{
  theDrivers.push_back(aController);
  aController->init(theSimulation);
}

void Simulation::addMonitor(std::shared_ptr<ControllerT> aController)
{
  theMonitors.push_back(aController);
  aController->init(theSimulation);
}

bool Simulation::getStatus() const
{
  return theSimulation->get_status();
}

std::string Simulation::getErrorInfo() const
{
  return theSimulation->get_error_info();
}

void Simulation::resetDrivers() 
{
    for (auto & myDriver : theDrivers)
    {
      myDriver->reset();
    }
}

void Simulation::initialiseSimulation() {
  theSimulationState = SIMULATION_STATE::INITIALISED;
  theSimulation->put_value(theReset, &LOW);
  run_cycle(1);
  resetDrivers();
  run_cycle(1);
  theSimulation->put_value(theReset, &HIGH);
  run_cycle(5);
}

void Simulation::simulate(std::function<bool()> aPredicate, size_t aWaitValue, size_t anIncrement)
{
  if (theSimulationState == SIMULATION_STATE::NOT_INITIALISED)
  {
    initialiseSimulation();
  }

  while (!isSimulationOver(aPredicate))
  {
    run_half_cycle(HIGH);
    for (auto & myMonitor : theMonitors)
    {
      myMonitor->next();
    }

    for (auto & myDriver : theDrivers)
    {
      myDriver->next();
    }
    run_half_cycle(LOW);
  }

  run_cycle(aWaitValue);
}

void Simulation::restartSim()
{
  theSimulationState = SIMULATION_STATE::NOT_INITIALISED;  
  theSimulation->restart();
}

}

