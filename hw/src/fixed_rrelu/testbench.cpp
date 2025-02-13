#include "Signal.hpp"
#include "Controller.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include "Simulation.hpp"
#include <verilated.h>
#include "Vfixed_rrelu.h"
#include "Vfixed_rrelu_fixed_rrelu.h"

using DeviceT = Vfixed_rrelu;

static constexpr size_t DATA_IN_0_PARALLELISM_DIM_0   =  Vfixed_rrelu_fixed_rrelu::DATA_IN_0_PARALLELISM_DIM_0;
static constexpr size_t DATA_IN_0_PARALLELISM_DIM_1   =  Vfixed_rrelu_fixed_rrelu::DATA_IN_0_PARALLELISM_DIM_1;

static constexpr size_t DATA_OUT_0_PARALLELISM_DIM_0  =  Vfixed_rrelu_fixed_rrelu::DATA_OUT_0_PARALLELISM_DIM_0;
static constexpr size_t DATA_OUT_0_PARALLELISM_DIM_1  =  Vfixed_rrelu_fixed_rrelu::DATA_OUT_0_PARALLELISM_DIM_1;

static constexpr size_t DATA_IN_WIDTH                 =  Vfixed_rrelu_fixed_rrelu::DATA_IN_0_PRECISION_0;
static constexpr size_t DATA_OUT_WIDTH                =  Vfixed_rrelu_fixed_rrelu::DATA_OUT_0_PRECISION_0;

static constexpr size_t DIN_WIDTH                     = DATA_IN_WIDTH * DATA_IN_0_PARALLELISM_DIM_1 * DATA_IN_0_PARALLELISM_DIM_0;
static constexpr size_t DOUT_WIDTH                    = DATA_OUT_WIDTH * DATA_OUT_0_PARALLELISM_DIM_1 * DATA_OUT_0_PARALLELISM_DIM_0;

#pragma pack(push, 1) 
template<size_t DinWidth, size_t DoutWidth>
struct DataValidIntf 
{
  sim::Signal<DinWidth> DataIn;
  bool ValidIn;

  sim::Signal<DoutWidth> DataOut;
  bool ValidOut;
};
#pragma pack(pop)

using FixedRReluIntfT = DataValidIntf<DIN_WIDTH, DOUT_WIDTH>;

class FixedRreluDriver : public sim::Controller<DeviceT, FixedRReluIntfT>
{
public:  
  using Controller<DeviceT, FixedRReluIntfT>::Controller;

  void driveFifoIntf(FixedRReluIntfT aDataValid)
  {
    this->theDevice->data_in_0_valid   =  aDataValid.ValidIn;
    memcpy(&this->theDevice->data_in_0, aDataValid.DataIn.data(), DIN_WIDTH / 8);
  }

  void reset() override
  {
    this->theDevice->data_out_0_ready = 1;
    this->theDevice->data_in_0_valid = 0;
  }

  void next() override
  {
    if (!this->isControllerEmpty())
    {
      driveFifoIntf(this->front());
      this->pop();
      return;
    }
    reset();
  }

};

class FixedRreluMonitor: public sim::Controller<DeviceT, FixedRReluIntfT>
{
public:  
  using Controller<DeviceT, FixedRReluIntfT>::Controller;

  void reset() override {}

  void next() override
  {
    if (this->theDevice->data_out_0_valid)
    {
      memcpy(theCurrentIntf.DataOut.data(), &this->theDevice->data_out_0, DOUT_WIDTH / 8);
      this->add(theCurrentIntf);
      return;
    }

  }

  FixedRReluIntfT theCurrentIntf{};
};

int main (int argc, char *argv[])
{
  auto mySimulation   = sim::Simulation<DeviceT>(argc, argv, "fixed_rrelu", sim::RunType::Release, sim::TraceOption::TraceOn, sim::ResetType::RANDOM_RESET, 500);
  auto myDriver       = std::make_shared<FixedRreluDriver>();
  auto myMonitor      = std::make_shared<FixedRreluMonitor>();

  mySimulation.addDriver(myDriver);
  mySimulation.addDriver(myMonitor);
  
  std::queue<uint8_t> myInputQueue = {};

  for (int i = 0; i < 50; i++)
  {
    auto myValue = -1 * (rand() % 255);
    myDriver->add({myValue, 1});     // Feeds in -1 * rand()
    myInputQueue.push(myValue);
  }

  mySimulation.simulate([&](){
    return myMonitor->getQueue().size() >= 50;
  }, 10);
  
  auto aCapturedQueue = myMonitor->getQueue();

  /*assert(aCapturedQueue.size() == myInputQueue.size());  */
  std::cout << "in, out" << std::endl;
  std::cout << std::hex;

  while (!aCapturedQueue.empty())
  {
    std::cout << (uint32_t) myInputQueue.front() << "," << aCapturedQueue.front().DataOut[0] << std::endl;
    aCapturedQueue.pop();
    myInputQueue.pop();
  }

}

