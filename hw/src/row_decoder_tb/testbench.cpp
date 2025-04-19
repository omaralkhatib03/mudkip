#include "Simulation.hpp"
#include "Utils.hpp"
#include "Vrow_decoder_tb.h"
#include "Vrow_decoder_tb_row_decoder_tb.h"
#include <array>
#include <cstdint>
#include <format>
#include <gtest/gtest.h>
#include "Interface.hpp"
#include "Driver.hpp"
#include "Monitor.hpp"

using DeviceT = Vrow_decoder_tb;

constexpr size_t IN_PARALLEL = Vrow_decoder_tb_row_decoder_tb::IN_PARALLEL;
constexpr size_t OUT_PARALLEL = Vrow_decoder_tb_row_decoder_tb::OUT_PARALLEL;

using DeviceT = Vrow_decoder_tb;

using InterfaceT = RowDecoderInterface<IN_PARALLEL, OUT_PARALLEL>;
using DriverT    = RowDecoderDriver<DeviceT, InterfaceT>;
using MonitorT   = RowDecoderMonitor<DeviceT, InterfaceT>;

class RowDecoderTest
{
public:
    RowDecoderTest(const std::string &testName = "", const uint64_t aSeed = -1)
        : theSimulation{
              std::format("row_decoder_test{}{}", ((testName != "") ? "_" : ""), testName),
              sim::RunType::Release,
              sim::TraceOption::TraceOn,
              sim::ResetType::RANDOM_RESET,
              static_cast<uint32_t>(10e4)},
          theMonitor{std::make_shared<MonitorT>()},
          theRandomDist{0, 0x10},
          theRngEngine{aSeed == -1 ? sim::initialize_rng() : aSeed},
          theDriver{std::make_shared<DriverT>(theRngEngine)}
    {
        theSimulation.addDriver(theDriver);
        theSimulation.addMonitor(theMonitor);

        if (aSeed != -1)
            std::cout << "Row Decoder Test Seed: " << aSeed << std::endl;
    }

    // Not very functional of you, tom clarke would not approve
    void addTestCase(const uint32_t aTestSize, uint64_t aSeed = -1)
    {
        auto aRandomOffset = theRandomDist(theRngEngine);
        InterfaceT stimulus = {0};

        std::vector<uint32_t> inputData;
        inputData.push_back(aRandomOffset);

        for (int i = 1; i < aTestSize; i++)
        {
            inputData.push_back(inputData[i - 1] + theRandomDist(theRngEngine));
        }

        for (int i = 0; i < aTestSize; i++)
        {
            std::cout << "i: " << i << " offset: " << inputData[i] << " Minus Bias: " << inputData[i] - aRandomOffset << std::endl;

        }

        for (int i = 1; i < aTestSize; i++)
        {
            std::cout << "id: " << i - 1 << " Freq: " << inputData[i] - inputData[i - 1] << std::endl;
        }


        size_t totalChunks = (aTestSize + IN_PARALLEL - 1) / IN_PARALLEL;
        std::cout << "Total Chunks: " << totalChunks << std::endl;

        for (size_t chunk = 0; chunk < totalChunks; chunk++)
        {

            for (size_t i = 0; i < IN_PARALLEL; i++)
            {
                size_t index = chunk * IN_PARALLEL + i;
                if (index < inputData.size())
                {
                    stimulus.r_beg_data[i] = inputData[index];
                }
            }

            stimulus.r_beg_valid = true;
            stimulus.r_beg_last = (chunk == totalChunks - 1);

            size_t validEntries = std::min(IN_PARALLEL, inputData.size() - chunk * IN_PARALLEL);
            stimulus.r_beg_bytemask = (1ULL << validEntries) - 1;

            theDriver->add(stimulus);
        }

        std::vector<uint32_t> outputVector;

        for (size_t i = 0; i < aTestSize - 1; i++)
        {
            uint32_t delta = inputData[i + 1] - inputData[i];
            for (uint32_t j = 0; j < delta; j++)
            {
                outputVector.push_back(i);
            }
        }

        for (auto val : outputVector)
        {
            theExpectedOutput.push(val);
        }
    }


    void simulate()
    {
        theSimulation.simulate([&]() {
            return theMonitor->getQueue().size() >= sim::ceil( ( float ) theExpectedOutput.size() / OUT_PARALLEL);
            // return theDriver->getQueue().empty();
        }, 10000);

        std::queue<uint32_t> capturedOutput;

        while (!theMonitor->getQueue().empty())
        {
            auto myFront = theMonitor->getQueue().front();

            for (size_t i = 0; i < OUT_PARALLEL; ++i)
            {
                if (!myFront.row_ids_last)
                {
                    capturedOutput.push(myFront.row_ids_data[i]);
                    continue;
                }

                if (sim::get_bit(myFront.row_ids_bytemask, i))
                {
                    capturedOutput.push(myFront.row_ids_data[i]);
                }
            }

            theMonitor->getQueue().pop();
        }

        sim::compareQueues(theExpectedOutput, capturedOutput);
    }

private:
    sim::Simulation<DeviceT> theSimulation;
    std::shared_ptr<MonitorT> theMonitor;
    std::queue<uint32_t> theExpectedOutput;
    std::uniform_int_distribution<uint32_t> theRandomDist;
    std::mt19937 theRngEngine;
    std::shared_ptr<DriverT> theDriver;
};

TEST(RowDecoderTest, BasicTest)
{
    RowDecoderTest test(sim::getTestName(), 1117728895);

    test.addTestCase(16);

    test.simulate();
};

TEST(RowDecoderTest, SeededTest_0)
{
    RowDecoderTest test(sim::getTestName(), 3030260183);
    test.addTestCase(10);

    test.simulate();
};

TEST(RowDecoderTest, SeededTest_1)
{
    RowDecoderTest test(sim::getTestName(), 1175644673);

    test.addTestCase(10);

    test.simulate();
};

TEST(RowDecoderTest, RandomTest)
{
    RowDecoderTest test(sim::getTestName());
    test.addTestCase(10);

    test.simulate();
};

TEST(RowDecoderTest, LongerRandomTest)
{
    RowDecoderTest test(sim::getTestName());
    test.addTestCase(50);

    test.simulate();
};

TEST(RowDecoderTest, TwoVectorTest)
{
    RowDecoderTest test(sim::getTestName());

    test.addTestCase(50);
    test.addTestCase(50);

    test.simulate();
};


TEST(RowDecoderTest, LongTwoVectorTest)
{
    RowDecoderTest test(sim::getTestName());

    test.addTestCase(100);
    test.addTestCase(100);

    test.simulate();
};


