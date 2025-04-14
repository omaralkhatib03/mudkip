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

inline uint64_t set_bit(uint64_t aNumber, size_t anIndex) {
    return aNumber | (1U << anIndex);
}

inline bool get_bit(uint64_t aNumber, size_t anIndex) {
    return (aNumber >> anIndex) & 1U;
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
            std::cout << "Expected: " << aExpected << "\nRecieved: " << aActual << std::endl;
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

    EXPECT_EQ(0, aExpected.size());
    EXPECT_EQ(aExpected.size(), aActual.size());
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

template <typename T, std::size_t N>
std::ostream& operator<<(std::ostream& os, const std::array<T, N>& arr)
{
    os << "[";
    for (std::size_t i = 0; i < N; ++i)
    {
        os << arr[i];
        if (i != N - 1)
            os << ", ";
    }
    os << "]";
    return os;
}


