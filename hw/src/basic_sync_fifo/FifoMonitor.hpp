#pragma once

#include "Controller.hpp"
#include "Defintions.hpp"
#include "Signal.hpp"
#include <cassert>

namespace sim 
{

struct FifoMonitorIntf
{
  sim::Signal<32> dout{};
  bool overflow;
  bool underflow;
  bool full;
  bool empty;
  bool valid{};
};

template <DeviceT DutT>
class FifoMonitor : public Controller<DutT, FifoMonitorIntf>
{
public:  
  using Controller<DutT, FifoMonitorIntf>::Controller;
  
  void reset() override {}

  void next() override
  {
    theCurrentIntf.dout = this->theDevice->dout;
    theCurrentIntf.valid = this->theDevice->valid;

    theCurrentIntf.overflow = this->theDevice->overflow;
    theCurrentIntf.underflow = this->theDevice->underflow;
    
    if (theCurrentIntf.valid)
    {
      this->add(theCurrentIntf);
    }

    assert(!theCurrentIntf.underflow);
    assert(!theCurrentIntf.overflow);
  }

  FifoMonitorIntf theCurrentIntf{};

};


}

