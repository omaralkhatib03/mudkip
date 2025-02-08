#pragma once

#include "Driver.hpp"
#include "Signal.hpp"

namespace xvl
{

struct RamDriverIntf
{
  xvl::Signal<1> s_axis_arvalid;
  xvl::Signal<5> s_axis_raddr;
  xvl::Signal<1> s_axis_rready;

  xvl::Signal<1> s_axis_awvalid;
  xvl::Signal<1> s_axis_wvalid;
  xvl::Signal<5> s_axis_waddr;

  xvl::Signal<32> s_axis_wdata;

};

class RamDriver : public Driver<RamDriverIntf>
{

public:
  using Driver<RamDriverIntf>::Driver;

  void driveRamIntf(RamDriverIntf aRamIntf)
  {
    this->drive("s_axis_arvalid", aRamIntf.s_axis_arvalid);
    this->drive("s_axis_raddr", aRamIntf.s_axis_raddr);
    this->drive("s_axis_rready", aRamIntf.s_axis_rready);

    this->drive("s_axis_awvalid", aRamIntf.s_axis_awvalid);
    this->drive("s_axis_wvalid", aRamIntf.s_axis_wvalid);
    this->drive("s_axis_waddr", aRamIntf.s_axis_waddr);

    this->drive("s_axis_wdata", aRamIntf.s_axis_wdata);
  }

  void reset() override
  {
    driveRamIntf({{0}, {0}, {0}, {0}, {0}, {0}, {0}});
  }

  void next() override
  {
    this->get("s_axis_arready", s_axis_arready);
    
    if (s_axis_arready[0])
    {
      return;
    }

    if (!this->isControllerEmpty())
    {
      driveRamIntf(this->front());
      this->pop();
      return;
    }

    reset();
  };


  xvl::Signal<31> s_axis_arready;
};

}
