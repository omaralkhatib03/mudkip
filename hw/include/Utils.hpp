#pragma once

#include <cstdint>
#include <gtest/gtest.h>
#include <iostream>
#include <queue>


constexpr unsigned flog2(unsigned x)
{
    return x == 1 ? 0 : 1+flog2(x >> 1);
}

constexpr unsigned clog2(unsigned x)
{
    return x == 1 ? 0 : flog2(x - 1) + 1;
}

constexpr int ceil(float num)
{
    return (static_cast<float>(static_cast<int32_t>(num)) == num)
        ? static_cast<int32_t>(num)
        : static_cast<int32_t>(num) + ((num > 0) ? 1 : 0);
}

constexpr int ceil_deiv(float a, float b)
{
    return ceil(a / b);
}

template <typename T>
void compareQueues(std::queue<T>& aExpected, std::queue<T>& aActual, bool aCheckEquality = true) {
    
    std::function<void(const T & aExpected, const T & aActual)> myWhileFunction = [](const T & aExpected, const T & aActual)
    {
        EXPECT_EQ(aExpected, aActual) << "Queue mismatch!";
    };

    if (!aCheckEquality)
    {
        myWhileFunction = [](const T & aExpected, const T & aActual)
        {
            std::cout << std::hex;
            std::cout << "Expected: " << (uint32_t) aExpected << " Recieved: " << (uint32_t) aActual << std::endl;
        };
    }

    while (!aExpected.empty() && !aActual.empty()) 
    {
        T myExpectedValue = aExpected.front();
        T myActualValue = aActual.front();
        
        myWhileFunction(myExpectedValue, myActualValue);

        aExpected.pop();
        aActual.pop();
    }
    
    EXPECT_EQ(aExpected.size(), 0);
    EXPECT_EQ(aActual.size(), 0);
    EXPECT_EQ(aActual.size(), 0);
}


