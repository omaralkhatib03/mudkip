#include <gtest/gtest.h>
#include "Test.hpp"
#include "TestUtils.hpp"

TEST(SoRrArbTest, CasesTest) 
{
    auto myTest = SoRrArbiterTest(sim::getTestName());

    myTest.addTestCase(0b0001, { 0x10, 0x20, },
                       true); // Case 1: Single request
    myTest.addTestCase(0b1010, { 0xA0, 0xB0, },
                       true); // Case 2: Requests at 1,3
    myTest.addTestCase(0b1111, { 0x88, 0x99, },
                       true); // Case 3: All requests (Adjusted)
    myTest.addTestCase(0b0000, { 0x00, 0x00, },
                       true); // Case 4: No requests
    myTest.addTestCase(0x0100, { 0x50, 0x60, },
                       false); // Case 5: Request with ready = false
    myTest.addTestCase(0x0110, { 0x11, 0x22, },
                       true); // Case 6: Requests at 1,2
}

TEST(SoRrArbTest, VectorRamLikeAccessTest) 
{
    auto myTest = SoRrArbiterTest(sim::getTestName());
    
    for (int i = 0; i < 4; i++)
    {
        myTest.addTestCase(0xF, {static_cast<uint8_t>((i*4)), static_cast<uint8_t>((i*4) + 2)}, true);
    }

    myTest.simulate();
}

TEST(SoRrArbTest, VectorRamLikeAccessIn4) 
{
    auto myTest = SoRrArbiterTest(sim::getTestName());
    
    for (int i = 0; i < 4; i++)
    {
        myTest.addTestCase(0x5, {   static_cast<uint8_t>((i*4)), static_cast<uint8_t>((i*4) + 1), 
                                    static_cast<uint8_t>((i*4) + 2), static_cast<uint8_t>((i*4) + 3)}, true);
    }

    myTest.simulate();
}
