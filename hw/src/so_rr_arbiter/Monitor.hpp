#pragma once

#include "Controller.hpp"

template <typename DeviceT, typename InterfaceT>
class SoRrArbMonitor : public sim::Controller<DeviceT, InterfaceT>
{
public:
    using sim::Controller<DeviceT, InterfaceT>::Controller;

    void reset() override
    {
    }

    void next() override
    {
        if (this->theDevice->valid)
        {
            theCurrentIntf.dout = this->theDevice->dout;

            this->add(theCurrentIntf);
            return;
        }
    }
    InterfaceT theCurrentIntf{};
};


