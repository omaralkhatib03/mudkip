#pragma once

#include <cstdint>
#include <gtest/gtest.h>
#include <iostream>
#include <queue>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

namespace sim
{

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

constexpr int nearest_to_P(int n, int P) 
{

    if (n % P == 0) 
    {
        return n;
    }

    return static_cast<int>(ceil(static_cast<double>(n) / P)) * P;
}

static inline unsigned int initialize_rng() 
{
    unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    std::cout << "Random Seed for this run: " << seed << std::endl;  // Print the seed
    return seed; 
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

inline std::string getTestName()
{
    const ::testing::TestInfo* myTestInfo =
            ::testing::UnitTest::GetInstance()->current_test_info();

    if (myTestInfo)
    {
            return myTestInfo->name();
    }
    else
    {
        throw std::runtime_error("What ? Could not get test name");
    }
}


};
