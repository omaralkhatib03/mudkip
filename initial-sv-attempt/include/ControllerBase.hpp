#pragma once

#include <memory>

namespace sim
{

template <typename DutT>
class ControllerBase
{
public:

    virtual bool isControllerEmpty() = 0;
    virtual void next() = 0;
    virtual void init(std::shared_ptr<DutT> aDevice) = 0;

    virtual void reset() = 0; // Monitors dont need to implement this, only drivers do

    virtual ~ControllerBase(){};

};

}
