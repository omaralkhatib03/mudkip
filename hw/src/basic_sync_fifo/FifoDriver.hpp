#pragma once

#include "Controller.hpp"
#include "Defintions.hpp"
#include "Signal.hpp"
#include <cstring>

namespace sim 
{

struct FifoDriverIntf
{
  sim::Signal<32> din;
  bool  shift_in;
  bool  shift_out;
};

template <DeviceT DutT>
class FifoDriver : public Controller<DutT, FifoDriverIntf>
{
public:  
  using Controller<DutT, FifoDriverIntf>::Controller;

  void driveFifoIntf(FifoDriverIntf aFifoIntf)
  {
    memcpy(&this->theDevice->din, aFifoIntf.din.data(), 4);
    this->theDevice->shift_in     =   aFifoIntf.shift_in;
    this->theDevice->shift_out    =   aFifoIntf.shift_out;
  }

  void reset() override
  {
    this->theDevice->din = 0;
    this->theDevice->shift_in = 0;
    this->theDevice->shift_out = 0;
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

}
