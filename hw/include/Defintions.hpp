#pragma once

namespace sim 
{

// Yeah I forgot about combinational circuits, here for shits and giggles
template<typename DutT>
concept DeviceT = requires (DutT aDut)
{
    {aDut};
  /*{aDut.rst_n};*/
  /*{aDut.clk};*/
};

}
