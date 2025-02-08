#pragma once

#include "Driver.hpp"
#include "Signal.hpp"

namespace xvl
{

struct FifoDriverIntf
{
  xvl::Signal<32> din;
  xvl::Signal<1>  shift_in;
  xvl::Signal<1>  shift_out;
};

class FifoDriver : public Driver<FifoDriverIntf>
{

public:

  using Driver<FifoDriverIntf>::Driver;

  void driveFifoIntf(FifoDriverIntf aFifoIntf)
  {
    this->drive("din", aFifoIntf.din);
    this->drive("shift_in", aFifoIntf.shift_in);
    this->drive("shift_out", aFifoIntf.shift_out);
  }

  void reset() override
  {
    driveFifoIntf({{0}, {0}, {0}});
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
  };

};

}
