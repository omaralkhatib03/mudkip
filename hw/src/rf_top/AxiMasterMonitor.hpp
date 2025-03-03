#pragma once

#include "Controller.hpp"
#include "Defintions.hpp"
#include "Signal.hpp"
#include <cassert>

namespace sim
{


template <size_t DataWidth>
struct AxiMasterMonitorIntf
{

    bool                                        waready;

    bool                                        wvalid;
    bool                                        wready;
    bool                                        wresp;

    bool                                        bvalid;

    bool                                        arready;

    sim::Signal<DataWidth>    rdata;
    bool                                        rvalid;

};

template <DeviceT DutT, size_t DataWidth>
class AxiMasterMonitor: public Controller<DutT, AxiMasterMonitorIntf<DataWidth>>
{
public:
    using Controller<DutT, AxiMasterMonitorIntf<DataWidth>>::Controller;
    void reset() override {}

    void next() override
    {
        theCurrentIntf.rdata        = this->theDevice->rdata;
        theCurrentIntf.rvalid     = this->theDevice->rvalid;

        theCurrentIntf.waready    = this->theDevice->waready;

        theCurrentIntf.wvalid    = this->theDevice->wvalid;
        theCurrentIntf.wready    = this->theDevice->wready;
        theCurrentIntf.wresp     = this->theDevice->wresp;

        theCurrentIntf.bvalid    = this->theDevice->bvalid;
        theCurrentIntf.arready = this->theDevice->arready;

        if (theCurrentIntf.rvalid)
        {
            this->add(theCurrentIntf);
        }
    }

    AxiMasterMonitorIntf<DataWidth> theCurrentIntf{};
};


}

