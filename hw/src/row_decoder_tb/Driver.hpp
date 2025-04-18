#pragma once

#include "Controller.hpp"
#include "Float.hpp"
#include <cstdint>
#include <random>

template <typename DeviceT, typename InterfaceT>
class RowDecoderDriver : public sim::Controller<DeviceT, InterfaceT>
{
    using sim::Controller<DeviceT, InterfaceT>::Controller;

    void driveAxiSlave(const InterfaceT& stimulus)
    {
        for (size_t i = 0; i < stimulus.r_beg_data.size(); ++i)
        {
            this->theDevice->r_beg_data[i] = stimulus.r_beg_data[i];
        }

        this->theDevice->r_beg_valid    = stimulus.r_beg_valid;
        this->theDevice->r_beg_last     = stimulus.r_beg_last;
        this->theDevice->r_beg_bytemask = stimulus.r_beg_bytemask;

        this->theDevice->row_ids_ready = stimulus.row_ids_ready;
    }

    void reset() override
    {
        InterfaceT resetStimulus = {};
        resetStimulus.r_beg_valid = 0;
        resetStimulus.r_beg_last = 0;
        resetStimulus.r_beg_bytemask = 0;
        resetStimulus.row_ids_ready = 0;

        driveAxiSlave(resetStimulus);
    }

    void next() override
    {
        // auto myReady = theRandomDist(sim::rng) % 2;
        auto myReady = 1;

        if (!this->isControllerEmpty())
        {
            auto stim = this->front();
            stim.row_ids_ready = myReady;
            driveAxiSlave(stim);

            if (this->theDevice->r_beg_ready)
            {
                this->pop();
            }

            return;
        }
        auto aStim = InterfaceT{0};
        aStim.row_ids_ready = myReady;
        driveAxiSlave(aStim);
    }

    std::uniform_int_distribution<uint64_t> theRandomDist{0, INT_FAST32_MAX};

};


