#pragma once

#include <string_view>
#include "Controller.hpp"
#include "Signal.hpp"

namespace xvl
{

template<typename InterfaceT>
class Driver : public Controller<InterfaceT>
{
public:
  using CapturedQueueT = std::queue<InterfaceT>;
  using Controller<InterfaceT>::Controller;

  void drive(std::string_view aPort, XsiSignal anXsiSignal);
};

template <typename InterfaceT> 
void Driver<InterfaceT>::drive(std::string_view aPort, XsiSignal anXsiSignal)
{
  this->theDesignLoader->put_value(this->thePortMap.at(aPort.data()), anXsiSignal.data());
}

}
