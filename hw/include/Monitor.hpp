#pragma once

#include <cstring>
#include <queue>
#include "Controller.hpp"

namespace xvl
{

// only here for backwards compatability
template<typename InterfaceT>
class Monitor : public Controller<InterfaceT>
{
public:
  using CapturedQueueT = std::queue<InterfaceT>;
  using Controller<InterfaceT>::Controller;

};

}
