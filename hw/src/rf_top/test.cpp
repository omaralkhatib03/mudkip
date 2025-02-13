#include <array>
#include <climits>
#include <cmath>
#include <cstdint>
#include <gtest/gtest.h>
#include "AxiMasterDriver.hpp"
#include "AxiMasterMonitor.hpp"
#include "Simulation.hpp"
#include <cstddef>
#include "TestUtils.hpp"
#include <queue>
#include <verilated.h>
#include "Vrf_top.h"
#include "Vrf_top_rf_top.h"
#include "AxiUtils.hpp"

using DeviceT                       = Vrf_top;

static constexpr size_t DATA_WIDTH  = Vrf_top_rf_top::DATA_WIDTH;
static constexpr size_t ADDR_WIDTH  = Vrf_top_rf_top::ADDR_WIDTH;
static constexpr size_t MAX_ADDR = (2 << ADDR_WIDTH) - 1;

using RfDriverT = sim::AxiMasterDriver<DeviceT, DATA_WIDTH, ADDR_WIDTH>;
using RfMonitorT = sim::AxiMasterMonitor<DeviceT, DATA_WIDTH>;

using MonitorIfT = sim::AxiMasterMonitorIntf<DATA_WIDTH>;

enum class RWCommandEnum
{
  ReadCommand   = 0,
  WriteCommand = 1
};

TEST(RfTopTests, SimpleRW) {
  auto mySimulation   = sim::Simulation<DeviceT>("rf_top", sim::RunType::Release);
  auto myRfDriver     = std::make_shared<RfDriverT>();
  auto myRfMonitor    = std::make_shared<RfMonitorT>();

  mySimulation.addDriver(myRfDriver);
  mySimulation.addMonitor(myRfMonitor);

  sim::writeToAddress<RfDriverT, DATA_WIDTH, ADDR_WIDTH>(myRfDriver, 1, 32);
  sim::readAddress<RfDriverT, DATA_WIDTH, ADDR_WIDTH>(myRfDriver, 1);

  mySimulation.simulate([&](){
    return myRfMonitor->getQueue().size() > 0;
  }, 5);

  MonitorIfT aCapturedData = myRfMonitor->getQueue().front();

  EXPECT_EQ(static_cast<uint32_t>(aCapturedData.rdata), 32); 

}

TEST(RfTopTests, RandomRW) {
  
  std::string myTestName = sim::getTestName();
  static constexpr size_t MAX_TEST_LENGTH = 50;

  auto mySimulation   = sim::Simulation<DeviceT>("rf_top_"+myTestName, sim::RunType::Release, sim::TraceOption::TraceOn, sim::ResetType::RANDOM_RESET, 500);
  auto myRfDriver     = std::make_shared<RfDriverT>(sim::ReadyTestType::RANDOM_READY);
  auto myRfMonitor    = std::make_shared<RfMonitorT>();

  auto myExpectedQueue = std::queue<uint32_t>();

  mySimulation.addDriver(myRfDriver);
  mySimulation.addMonitor(myRfMonitor);

  std::array<uint32_t, MAX_ADDR> mySimpleModel = std::array<uint32_t, MAX_ADDR>();

  for (int i = 0; i < MAX_TEST_LENGTH; i++)
  {
    auto myAddress  = rand() % 4;
    auto myData     = rand() % UINT_MAX;

    RWCommandEnum aCommand = static_cast<RWCommandEnum>(rand() % 2);

    if (aCommand == RWCommandEnum::WriteCommand)
    {
      sim::writeToAddress<RfDriverT, DATA_WIDTH, ADDR_WIDTH>(myRfDriver, myAddress, myData);
      mySimpleModel[myAddress] = myData;
    }
    else 
    {
      sim::readAddress<RfDriverT, DATA_WIDTH, ADDR_WIDTH>(myRfDriver, myAddress);
      myExpectedQueue.push(mySimpleModel[myAddress]);
    }
  }

  mySimulation.simulate([&](){
    return myRfMonitor->getQueue().size() > MAX_TEST_LENGTH;
  }, 5);

  auto aCapturedQ = myRfMonitor->getQueue();

  EXPECT_EQ(aCapturedQ.size(), myExpectedQueue.size());

  while (!aCapturedQ.empty()) 
  {
    EXPECT_EQ(static_cast<uint32_t>(aCapturedQ.front().rdata), myExpectedQueue.front()); 
    aCapturedQ.pop();
    myExpectedQueue.pop();
  }

}
