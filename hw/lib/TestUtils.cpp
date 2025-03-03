#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

namespace sim
{

std::string getTestName()
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

}
