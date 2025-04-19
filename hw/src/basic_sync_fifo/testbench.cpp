#include "FifoDriver.hpp"
#include "FifoMonitor.hpp"
#include "Simulation.hpp"
#include <cstdint>
#include <memory>
#include <verilated.h>
#include "Vbasic_sync_fifo.h"
#include <gtest/gtest.h>
#include <queue>
#include <cstdlib>  // For rand()


using DeviceT           = Vbasic_sync_fifo;
using FifoMonitorT      = sim::FifoMonitor<DeviceT>;
using FifoDriverT       = sim::FifoDriver<DeviceT>;

class FifoTest : public ::testing::Test {
protected:
    void SetUp() override {
        mySimulation = sim::Simulation<DeviceT>("basic_sync_fifo", sim::RunType::Release,
                                                sim::TraceOption::TraceOn, sim::ResetType::RANDOM_RESET, 10000);

        myFifoDriver = std::make_shared<FifoDriverT>();
        myFifoMonitor = std::make_shared<FifoMonitorT>();

        mySimulation.addDriver(myFifoDriver);
        mySimulation.addMonitor(myFifoMonitor);
    }

    void TearDown() override {
    }

    sim::Simulation<DeviceT> mySimulation;
    std::shared_ptr<FifoDriverT> myFifoDriver;
    std::shared_ptr<FifoMonitorT> myFifoMonitor;
};

TEST_F(FifoTest, BasicPushAndPop) {
    myFifoDriver->add({0xdeadbeef, 1, 0});
    myFifoDriver->add({0, 0, 1});

    mySimulation.simulate([&]() {
        return myFifoMonitor->getQueue().size() > 0;
    }, 5);

    sim::FifoMonitorIntf aCapturedData = myFifoMonitor->getQueue().front();
    std::cout << std::hex;
    std::cout << "Expected data: " << 0xdeadbeef << " CapturedSignal: " << aCapturedData.dout[0] << std::endl;

    EXPECT_EQ(aCapturedData.dout[0], 0xdeadbeef);

    myFifoMonitor->getQueue().pop();
}

TEST_F(FifoTest, FifoFullCondition) {
    std::queue<uint32_t> myInputQueue;
    for (int i = 0; i < 4; i++) // Assuming FIFO Depth is 64
    {
        auto myValue = rand();
        myFifoDriver->add({myValue, 1, 0});
        myInputQueue.push(myValue);
    }

    for (int i = 0; i < 4; i++) // Assuming FIFO Depth is 64
    {
        myFifoDriver->add({rand(), 0, 1}); // This should trigger overflow
    }

    mySimulation.simulate([&]() {
        return myFifoMonitor->getQueue().size() >= 4;
    }, 10);

    auto aCapturedQueue = myFifoMonitor->getQueue();
    EXPECT_EQ(aCapturedQueue.size(), myInputQueue.size());

    while (!aCapturedQueue.empty())
    {
        EXPECT_EQ(myInputQueue.front(), aCapturedQueue.front().dout[0]);
        aCapturedQueue.pop();
        myInputQueue.pop();
    }
}

TEST_F(FifoTest, FifoEmptyCondition) {
    for (int i = 0; i < 64; i++) {
        myFifoDriver->add({rand(), 0, 1}); 
    }

    mySimulation.simulate([&]() {
        return myFifoMonitor->getQueue().size() == 0;
    }, 10);

    EXPECT_EQ(myFifoMonitor->getQueue().size(), 0);
}

TEST_F(FifoTest, LatencyTest) {
    for (int i = 0; i < 64; i++) {
        myFifoDriver->add({rand(), 1, 0}); // Push data into FIFO
    }

    mySimulation.simulate([&]() {
        return myFifoMonitor->getQueue().size() > 0;
    }, 10);

    sim::FifoMonitorIntf aCapturedData = myFifoMonitor->getQueue().front();
    std::cout << "Captured data at latency 0: " << aCapturedData.dout[0] << std::endl;
    EXPECT_EQ(aCapturedData.dout[0], aCapturedData.dout[0]); 
}

TEST_F(FifoTest, RandomStressTest) {
    std::queue<uint32_t> randomInputQueue;
    for (int i = 0; i < 50; i++) {
        auto myValue = i;
        myFifoDriver->add({myValue, 1, 0});
        randomInputQueue.push(myValue);
    }

    for (int i = 0; i < 50; i++) {
        myFifoDriver->add({rand(), 0, 1}); 
    }

    mySimulation.simulate([&]() {
        return myFifoMonitor->getQueue().size() >= 50;
    }, 10);

    auto capturedQueue = myFifoMonitor->getQueue();
    EXPECT_EQ(capturedQueue.size(), randomInputQueue.size());

    while (!capturedQueue.empty())
    {
        EXPECT_EQ(randomInputQueue.front(), capturedQueue.front().dout[0]);
        capturedQueue.pop();
        randomInputQueue.pop();
    }
}

