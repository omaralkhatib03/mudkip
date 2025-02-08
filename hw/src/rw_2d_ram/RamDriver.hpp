#pragma once

#include "Driver.hpp"
#include "Signal.hpp"

namespace xvl
{

struct RamDriverIntf
{
  xvl::Signal<1> valid_addr_ps;
  xvl::Signal<5> r_addr;
  xvl::Signal<1> valid_w;
  xvl::Signal<5> w_addr;
  xvl::Signal<32> w_data;
};

/**/
class RamDriver : public Driver<RamDriverIntf>
{

public:

  using Driver<RamDriverIntf>::Driver;

  void driveRamIntf(RamDriverIntf aRamIntf)
  {
    this->drive("valid_addr_ps", aRamIntf.valid_addr_ps);
    this->drive("r_addr", aRamIntf.r_addr);
    this->drive("valid_w", aRamIntf.valid_w);
    this->drive("w_addr", aRamIntf.w_addr);
    this->drive("w_data", aRamIntf.w_data);
  }

  void reset() override
  {
    driveRamIntf({{0}, {0}, {0}, {0}, {0}});
  }

  void next() override
  {
    if (!this->isControllerEmpty())
    {
      driveRamIntf(this->front());
      this->pop();
      return;
    }
    reset();
  };

};

}
