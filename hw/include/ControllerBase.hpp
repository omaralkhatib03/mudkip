#pragma once

#include "xsi_loader.h"
#include <memory>

namespace xvl
{

class ControllerBase
{
public:

  virtual bool isControllerEmpty() = 0;
  virtual void next() = 0;
  virtual void init(std::shared_ptr<Xsi::Loader> aDesignLoader) = 0;
  virtual void reset() = 0; // Monitors dont need to implement this, only drivers do

  virtual ~ControllerBase(){};

};


}
