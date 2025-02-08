#pragma once

#include <memory>
#include <queue>
#include <stdexcept>
#include <format>
#include <string>
#include <string_view>
#include <unordered_map>
#include "Global.hpp"
#include "ControllerBase.hpp"
#include "Signal.hpp"
#include "xsi_loader.h"

namespace xvl
{

inline void getPortFromXsi(PortNumber & aPortNumberReference, std::string_view aPortName, Xsi::Loader & aDesignLoader)
{
  aPortNumberReference    = aDesignLoader.get_port_number(aPortName.data());
  if (aPortNumberReference < 0) throw std::runtime_error(std::format("What ? Could not obtain %s", aPortName.data()));
}

template<typename InterfaceT>
class Controller : public ControllerBase
{
public:
  using QueueT = std::queue<InterfaceT>;

  Controller() = delete;

  template<typename... T>
  Controller(T && ... aSignalNames)
    : theQueue{},
      theDesignLoader(NULL),
      thePortMap{{aSignalNames, -1}...}
  {}

  bool isControllerEmpty() override;
  void init(std::shared_ptr<Xsi::Loader> aDesignLoader) override;

  InterfaceT pop();
  InterfaceT front() const;
  void add(InterfaceT anElement);

  virtual InterfaceT getCurrent(){return InterfaceT(); };
  virtual ~Controller() {};

  template<size_t Width>
  void get(std::string_view aPort, Signal<Width> & aSignal)
  {
    XsiSignal aCapturedSignal(aSignal.size(), {0, 0});
    this->theDesignLoader->get_value(this->thePortMap.at(aPort.data()), aCapturedSignal.data());
    aSignal = static_cast<Signal<Width>>(aCapturedSignal); 
  }

  std::queue<InterfaceT> & getQueue();

protected:
  QueueT                                      theQueue;
  std::shared_ptr<Xsi::Loader>                theDesignLoader;
  std::unordered_map<std::string, PortNumber> thePortMap;
};

template <typename InterfaceT>
std::queue<InterfaceT> & Controller<InterfaceT>::getQueue()
{
  return this->theQueue;
}

template <typename InterfaceT>
bool Controller<InterfaceT>::isControllerEmpty()
{
  return theQueue.empty();
}

template <typename  InterfaceT>
void Controller<InterfaceT>::add(InterfaceT anElement)
{
  theQueue.push(anElement);
}

template <typename InterfaceT>
InterfaceT Controller<InterfaceT>::pop()
{
  auto myFront = theQueue.front();
  theQueue.pop();
  return myFront;
}

template <typename InterfaceT>
InterfaceT Controller<InterfaceT>::front() const
{
  return theQueue.front();
}

template <typename InterfaceT>
void Controller<InterfaceT>::init(std::shared_ptr<Xsi::Loader> aDesignLoader)
{
  theDesignLoader = aDesignLoader;

  for (auto & aPort : thePortMap)
  {
    aPort.second = theDesignLoader->get_port_number(aPort.first.c_str());
  }
}

}
