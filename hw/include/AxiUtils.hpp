#pragma once

#include "Signal.hpp"
#include <cstdint>
#include <memory>


namespace sim
{

template <typename AxiMasterDriverT, size_t DATA_WIDTH, size_t ADDR_WDITH>
void writeToAddress(std::shared_ptr<AxiMasterDriverT> anAxiMasterDriver, uint32_t anAddress, Signal<DATA_WIDTH> aData)
{
    auto myStim         = AxiMasterDriverIntf<DATA_WIDTH, ADDR_WDITH>();
    myStim.waddr        = anAddress;
    myStim.wavalid    = 1;

    myStim.wdata        = aData;
    myStim.wvalid     = 1;

    myStim.arvalid    =    0;

    anAxiMasterDriver->add(myStim);
}

template <typename AxiMasterDriverT, size_t DATA_WIDTH, size_t ADDR_WDITH>
void readAddress(std::shared_ptr<AxiMasterDriverT> anAxiMasterDriver, uint32_t anAddress)
{
    auto myStim         = AxiMasterDriverIntf<DATA_WIDTH, ADDR_WDITH>();

    myStim.wavalid    = 0;
    myStim.wvalid     = 0;

    myStim.raddr        = anAddress;
    myStim.arvalid    = 1;

    anAxiMasterDriver->add(myStim);
}



}
