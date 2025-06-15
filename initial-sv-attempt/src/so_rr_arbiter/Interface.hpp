#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

template <size_t DataWidth, std::size_t NumInputs>
struct SoRrArbiterIf
{
    // Inputs
    uint32_t req : NumInputs;
    std::array<uint8_t, NumInputs> in;
    bool in_ready;

    // Outputs
    uint32_t dout : DataWidth;
    bool valid;
    bool ready;
};


