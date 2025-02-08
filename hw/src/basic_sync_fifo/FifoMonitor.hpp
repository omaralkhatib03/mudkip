#pragma once

#include "Monitor.hpp"
#include <cassert>

namespace xvl
{

struct FifoMonitorIntf
{
  xvl::Signal<32> dout{};
  xvl::Signal<1>  overflow;
  xvl::Signal<1>  underflow;
  xvl::Signal<1>  full;
  xvl::Signal<1>  empty;
  xvl::Signal<1>  valid{};
};

class FifoMonitor : public Monitor<FifoMonitorIntf>
{

public:
  using Monitor<FifoMonitorIntf>::Monitor;

  void reset() override {};

  FifoMonitorIntf getCurrent() override
  {
    return theCurrentIntf;
  }

  void next() override
  {
    this->get("dout", theCurrentIntf.dout);
    this->get("valid", theCurrentIntf.valid);
    this->get("overflow", theCurrentIntf.overflow);
    this->get("underflow", theCurrentIntf.underflow);

    if (theCurrentIntf.valid[0])
    {
      aCounter++;
      return;
    }

    if (aCounter % 2)
    {
      this->add(theCurrentIntf);
      aCounter++;
    }

    assert(theCurrentIntf.underflow[0] == 0);
    assert(theCurrentIntf.overflow[0] == 0);
  };

  FifoMonitorIntf & getCurrentIntf()
  {
      return theCurrentIntf;
  }

private:
  FifoMonitorIntf theCurrentIntf{};
  int aCounter = 0;
};



}

