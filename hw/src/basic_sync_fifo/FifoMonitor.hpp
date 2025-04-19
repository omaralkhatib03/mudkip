#pragma once

#include "Controller.hpp"
#include "Signal.hpp"
#include <cassert>

namespace sim
{

struct FifoMonitorIntf
{
    sim::Signal<32> dout{};
    bool overflow;
    bool underflow;
    bool full;
    bool empty;
    bool valid{};
};

template <typename DutT>
class FifoMonitor : public Controller<DutT, FifoMonitorIntf>
{
public:
    using Controller<DutT, FifoMonitorIntf>::Controller;

    void reset() override {}

    void next() override
    {
        theCurrentIntf.dout = this->theDevice->dout;
        theCurrentIntf.valid = this->theDevice->valid;

        theCurrentIntf.overflow = this->theDevice->overflow;
        theCurrentIntf.underflow = this->theDevice->underflow;

        if (theCurrentIntf.valid && this->theDevice->shift_out)
        {
            this->add(theCurrentIntf);
            counter++; 
        }
    }

    FifoMonitorIntf theCurrentIntf{};
    int counter = 1;
};


}

