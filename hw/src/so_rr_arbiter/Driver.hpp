#pragma once

#include "Controller.hpp"

template <typename DeviceT, typename InterfaceT>

class SoRrArbiterDriver : public sim::Controller<DeviceT, InterfaceT>
{
    using sim::Controller<DeviceT, InterfaceT>::Controller;

    void driveFifoIntf(InterfaceT aStim)
    {
        memcpy(&this->theDevice->in, aStim.in.data(), aStim.in.size());
        this->theDevice->ready = aStim.ready;
        this->theDevice->req = aStim.req;
    }

    void reset() override
    {
        InterfaceT aResetStim = { 0 };
        aResetStim.ready = 1;
        driveFifoIntf(aResetStim);
    }

    void next() override
    {
        if (!this->isControllerEmpty())
        {
            driveFifoIntf(this->front());

            if (this->theDevice->in_ready)
            {
                this->pop();
            }

            return;
        }
        reset();
    }
};


