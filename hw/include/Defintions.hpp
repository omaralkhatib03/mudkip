#pragma once

namespace sim 
{

template<typename DutT>
concept DeviceT = requires (DutT aDut)
{
  {aDut.rst_n};
  {aDut.clk};
};

}
