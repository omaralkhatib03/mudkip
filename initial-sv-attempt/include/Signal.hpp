#pragma once

#include <cstddef>
#include <cstdint>
#include <array>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <ostream>

namespace sim
{

static constexpr uint32_t UINT32_BITS = 32;

static constexpr uint32_t getWords(int aWidth)
{
    return (aWidth + UINT32_BITS - 1) / UINT32_BITS;
}

#pragma pack(push, 1)
template<size_t Width>
class Signal
: public std::array<uint32_t, getWords(Width)>
{
    static_assert(Width > 0, "Cannot create signal of length 0");

public:
    using std::array<uint32_t, getWords(Width)>::array;

    Signal();
    Signal(std::initializer_list<uint32_t> anInitList);

    Signal(const uint32_t aValue);

    explicit operator uint32_t() const {
            if (this->size() > 1)
                std::cerr << "Warning: Casing Signal which is larger than 32 bits, Signal is " << this->size() << " bits";

            return (*this)[0];
    }

    friend std::ostream& operator<<(std::ostream& os, const Signal<Width>& signal) {
        os << std::hex;
        os << "{ ";
        for (size_t i = 0; i < signal.size(); ++i) {
            os << signal[i];
            if (i < signal.size() - 1) os << ", ";
        }
        os << " }";
        return os;
    }

    virtual ~Signal(){};
};
#pragma pack(pop)

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
Signal<Width>::Signal(uint32_t aValue)
    : Signal()
{
        static_assert(getWords(Width) > 0, "Signal must have at least one word.");

        (*this)[0] = aValue;
        for (size_t i = 1; i < getWords(Width); ++i)
        {
            (*this)[i] = 0;
        }
}

template<size_t Width>
Signal<Width>::Signal()
    : std::array<uint32_t, getWords(Width)>{}
{}

}
