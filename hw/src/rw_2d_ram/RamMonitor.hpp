#pragma once

#include "Monitor.hpp"
#include <cassert>

namespace xvl
{

struct RamMonitorIntf
{
  xvl::Signal<32> r_data{};
  xvl::Signal<1>  valid_data;
  xvl::Signal<1>  ready_pl;
};

class RamMonitor : public Monitor<RamMonitorIntf>
{

public:
  using Monitor<RamMonitorIntf>::Monitor;

  void reset() override {};

  RamMonitorIntf getCurrent() override
  {
    return theCurrentIntf;
  }

  void next() override
  {
    this->get("r_data", theCurrentIntf.r_data);
    this->get("valid_data", theCurrentIntf.valid_data);
  
    if (theCurrentIntf.valid_data[0])
    {
      this->add(theCurrentIntf);
    }

  };

  RamMonitorIntf & getCurrentIntf()
  {
      return theCurrentIntf;
  }

private:
  RamMonitorIntf theCurrentIntf{};
  int aCounter = 0;
};



}

