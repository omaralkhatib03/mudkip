#pragma once

#include "Monitor.hpp"
#include <cassert>

namespace xvl
{

struct RamMonitorIntf
{
  xvl::Signal<32> s_axis_rdata{};
  xvl::Signal<1>  s_axis_rvalid;
  xvl::Signal<1>  s_axis_arready;
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
    this->get("s_axis_rdata", theCurrentIntf.s_axis_rdata);
    this->get("s_axis_rvalid", theCurrentIntf.s_axis_rvalid);
    this->get("s_axis_arready", theCurrentIntf.s_axis_arready);
     
    if (theCurrentIntf.s_axis_rvalid[0])
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

