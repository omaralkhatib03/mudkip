#pragma once
#include "Controller.hpp"
#include "Utils.hpp"

template <typename DeviceT, typename InterfaceT>
class RowDecoderMonitor : public sim::Controller<DeviceT, InterfaceT>
{
public:
    using sim::Controller<DeviceT, InterfaceT>::Controller;

    void reset() override
    {
        theCurrentIntf = {};
    }

    void next() override
    {
        if (this->theDevice->row_ids_valid && this->theDevice->row_ids_ready)
        {
            for (size_t i = 0; i < theCurrentIntf.row_ids_data.size(); ++i)
            {
                if (!this->theDevice->row_ids_last)
                {
                    theCurrentIntf.row_ids_data[i] = this->theDevice->row_ids_data[i];
                    continue;
                }

                if (sim::get_bit(this->theDevice->row_ids_bytemask, i))
                {
                    theCurrentIntf.row_ids_data[i] = this->theDevice->row_ids_data[i];
                }
            }

            theCurrentIntf.row_ids_valid    = this->theDevice->row_ids_valid;
            theCurrentIntf.row_ids_ready    = this->theDevice->row_ids_ready;
            theCurrentIntf.row_ids_last     = this->theDevice->row_ids_last;
            theCurrentIntf.row_ids_bytemask = this->theDevice->row_ids_bytemask;

            this->add(theCurrentIntf);
        }
    }

    InterfaceT theCurrentIntf{};
};
