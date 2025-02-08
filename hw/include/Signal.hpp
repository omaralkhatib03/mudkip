#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <array>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <vector>
#include <xsi.h>

namespace xvl
{

using XsiBit = s_xsi_vlog_logicval;
using XsiSignal = std::vector<XsiBit>;

static constexpr uint32_t UINT32_BITS = 32;

static constexpr uint32_t getWords(int aWidth)
{
  return (aWidth + UINT32_BITS - 1) / UINT32_BITS;
}

template<size_t Width>
class Signal
: public std::array<uint32_t, getWords(Width)>
{
  static_assert(Width > 0, "Cannot create signal of length 0");

public:
  using std::array<uint32_t, getWords(Width)>::array;

  Signal();
  explicit Signal(XsiSignal anXsiSignal);
  Signal(std::initializer_list<uint32_t> anInitList);

  operator XsiSignal() const;

  virtual ~Signal(){};

};

template<size_t Width>
Signal<Width>::Signal(std::initializer_list<uint32_t> anInitList)
  : std::array<uint32_t, getWords(Width)>{}
{
  if (anInitList.size() > this->size())
  {
    std::cerr << "Initilaiser list for signal is too large, truncating end of list" << std::endl;
  }

  size_t idx = 0;

  for (auto value : anInitList) {
    if (idx < this->size()) {
      this->operator[](idx) = value;
      ++idx;
      continue;
    }
    break;
  }

}

template<size_t Width>
Signal<Width>::Signal()
  : std::array<uint32_t, getWords(Width)>{}
{}

template<size_t Width>
Signal<Width>::Signal(XsiSignal anXsiSignal)
  : Signal()
{
  std::transform(anXsiSignal.begin(), anXsiSignal.end(), this->begin(), [](XsiBit aBit){
    if (aBit.bVal)
    {
      std::cerr << "Non two-state value being casted to signal" << std::endl;
    }
    return aBit.aVal;
  });
}

template<size_t Width>
Signal<Width>::operator XsiSignal() const
{
  auto myReturnSignal = XsiSignal(this->size(), {0, 0});
  std::transform(this->begin(), this->end(), myReturnSignal.begin(), [](uint32_t aUint32){
    return XsiBit({aUint32, 0});
  });
  return myReturnSignal;
}

}
