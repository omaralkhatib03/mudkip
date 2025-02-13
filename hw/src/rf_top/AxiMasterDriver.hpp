#pragma once

#include "Controller.hpp"
#include "Defintions.hpp"
#include "Signal.hpp"
#include <cstddef>
#include <cstdlib>

namespace sim 
{

template <size_t DataWidth, size_t AddrWidth>
struct AxiMasterDriverIntf 
{

  sim::Signal<AddrWidth>  waddr;
  bool                    wavalid;

  sim::Signal<DataWidth>  wdata;
  bool                    wvalid;

  bool                    bready; 

  sim::Signal<AddrWidth>  raddr;
  bool                    arvalid;

  bool                    rready;
};

enum class ReadyTestType {
  RANDOM_READY = 0,
  ALWAYS_READY = 1
};

template <DeviceT DutT, size_t DataWidth, size_t AddrWidth>
class AxiMasterDriver : public Controller<DutT, AxiMasterDriverIntf<DataWidth, AddrWidth>>
{
public:  
  using Controller<DutT, AxiMasterDriverIntf<DataWidth, AddrWidth>>::Controller;
  
  AxiMasterDriver(ReadyTestType aReadyTestType = ReadyTestType::ALWAYS_READY)
    : Controller<DutT, AxiMasterDriverIntf<DataWidth, AddrWidth>>(),
      theTestType{aReadyTestType}
    {}

  void driveAxiMM(AxiMasterDriverIntf<DataWidth, AddrWidth> anAxiRequest)
  {
    this->theDevice->waddr    = *anAxiRequest.waddr.data();
    this->theDevice->wavalid  = anAxiRequest.wavalid;
    this->theDevice->bready  = anAxiRequest.bready;
    this->theDevice->wdata    = *anAxiRequest.wdata.data();
    this->theDevice->wvalid   = anAxiRequest.wvalid;
    this->theDevice->arvalid  = anAxiRequest.arvalid;
    this->theDevice->raddr  = *anAxiRequest.raddr.data();
  }

  void reset() override
  {
    this->theDevice->wavalid  = 0;
    this->theDevice->wvalid   = 0;
    this->theDevice->arvalid  = 0;
  }

  void next() override
  {
    if (!this->isControllerEmpty())
    {
      driveAxiMM(this->front());
      this->pop();

      if (theTestType == ReadyTestType::RANDOM_READY)
      {
        this->theDevice->rready = counter % 2;          
        this->theDevice->bready = counter % 2;          
        counter++;
      }
      else 
      {
        this->theDevice->rready = 1;          
        this->theDevice->bready = 1;          
      }

      return;
    }

    
    reset();
  }


private:
  ReadyTestType theTestType;
  int counter = 0;
};

}
