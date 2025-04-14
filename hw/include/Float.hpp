#pragma  once
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <format>
#include <functional>
#include <gtest/gtest.h>
#include <random>
#include <verilated.h>
#include "Simulation.hpp"
#include "Utils.hpp"
#include "floating_point_v7_1_bitacc_cmodel.h"
#include "FloatOps.h"
#include "Controller.hpp"

namespace sim
{

static std::mt19937 rng(1069295940);

inline std::string MapToHexString(const std::map<uint64_t, uint64_t>& m) {
    std::ostringstream oss;
    oss << "{\n";
    for (const auto& [key, value] : m) {
        oss << "  [0x" << std::hex << key << "] = 0x" << value << "\n";
    }
    oss << "}";
    return oss.str();
}

template<size_t DATA_WIDTHT, size_t E_WIDTHT, size_t FRAC_WIDTHT, size_t PARALLELISMT>
struct FloatOpIf
{
    // Parameters
    static constexpr int PARALLELISM = PARALLELISMT;
    static constexpr int DATA_WIDTH = DATA_WIDTHT;
    static constexpr int E_WIDTH = E_WIDTHT;
    static constexpr int FRAC_WIDTH = FRAC_WIDTHT;

    // Input
    bool                                    in_valid;
    std::array<unsigned long, PARALLELISM>  a;
    std::array<unsigned long, PARALLELISM>  b;
    bool                                    ready;
    std::array<bool, PARALLELISM>           in_mask;
    bool                                    in_tlast;

    // Output
    bool                                    in_ready;
    std::array<unsigned long, PARALLELISM>  out;
    bool                                    valid;
    std::array<bool, PARALLELISM>           tkeep;
    bool                                    tlast;
};

template<typename DeviceT, typename FloatOpT>
class FloatOpDriver : public sim::Controller<DeviceT, FloatOpT>
{
    using sim::Controller<DeviceT, FloatOpT>::Controller;

    void driveFifoIntf(FloatOpT aStim)
    {
        for (int i = 0; i < aStim.a.size(); i++)
        {
            this->theDevice->a[i]       = aStim.a[i];
            this->theDevice->b[i]       = aStim.b[i];
            this->theDevice->in_mask    |= (0x1 & aStim.in_mask[i]) << i;
        }

        this->theDevice->in_valid   = aStim.in_valid;
        this->theDevice->ready      = aStim.ready;
        this->theDevice->in_tlast   = aStim.in_tlast;
    }

    void reset() override
    {
        FloatOpT aResetStim  = {0};
        aResetStim.ready   = 1;
        driveFifoIntf(aResetStim);
    }

    void next() override
    {
        if (!this->isControllerEmpty() && this->theDevice->ready)
        {
            driveFifoIntf(this->front());
            this->pop();
            return;
        }
        reset();
    }

};

template<typename DeviceT, typename FloatOpT>
class FloatOpMonitor : public sim::Controller<DeviceT, FloatOpT>
{
public:
    using sim::Controller<DeviceT, FloatOpT>::Controller;

    void reset() override {}

    void next() override
    {
        if (this->theDevice->valid)
        {
            for (int i = 0; i < theCurrentIntf.out.size(); i++)
            {
                theCurrentIntf.out[i] = this->theDevice->out[i];
                theCurrentIntf.tkeep[i] = sim::get_bit(this->theDevice->tkeep, i);
            }

            theCurrentIntf.valid = this->theDevice->valid;
            theCurrentIntf.tlast = this->theDevice->tlast;
            this->add(theCurrentIntf);
            return;
        }

    }
FloatOpT theCurrentIntf{};
};

template<typename FloatIfT, typename DeviceT, typename StimT>
struct FloatOpTest
{
    using DriverT       = FloatOpDriver<DeviceT, FloatIfT>;
    using MonitorT      = FloatOpMonitor<DeviceT, FloatIfT>;
    using FloatFuncT    = std::function<void(StimT, const StimT, const StimT)>;

    FloatOpTest(FloatFuncT aFloatOp, const std::string & aTestName = "") :
        theSimulation {
            std::format("product{}{}", ((aTestName != "") ? "_" : ""), aTestName),
            sim::RunType::Release,
            sim::TraceOption::TraceOn,
            sim::ResetType::RANDOM_RESET,
            10000
        },
        theFloatOpDriver{std::make_shared<DriverT>()},
        theFloatOpMonitor{std::make_shared<MonitorT>()},
        theFloatOp{aFloatOp}
    {
        theSimulation.addDriver(theFloatOpDriver);
        theSimulation.addMonitor(theFloatOpMonitor);
    }

    std::vector<StimT> getExpectedData(std::vector<StimT> & aVectorA,std::vector<StimT> & aVectorB)
    {
        std::vector<StimT> myOut(aVectorA.size());

        for (int i = 0; i < aVectorA.size(); i++)
        {
            xip_fpo_init2(myOut[i], FloatIfT::E_WIDTH, FloatIfT::FRAC_WIDTH);
            theFloatOp(myOut[i], aVectorA[i], aVectorB[i]);
        }

        // VL_PRINTF("Expectation Vectors : \n");
        // for (int i = 0; i < aVectorA.size(); i++)
        // {
            // VL_PRINTF("i: %d, A: %lx, B: %lx, C: %lx\n",
            //           i,
            //           xfpo_to_unsigned_long(aVectorA[i]),
            //           xfpo_to_unsigned_long(aVectorB[i]),
            //           xfpo_to_unsigned_long(myOut[i]));
        // }
        // VL_PRINTF("\n");

        return myOut;
    }

    FloatIfT getWriteVectorChunk(std::vector<StimT> & aVectorA, std::vector<StimT> & aVectorB, int anIndex, int aTestSize) // Chunk starting from anIndex
    {
        FloatIfT myStim                 = {0};
        myStim.in_valid                 = 1;

        for (int i = anIndex; i < anIndex + FloatIfT::PARALLELISM; i++)
        {
            myStim.a[i - anIndex]       = xfpo_to_unsigned_long(aVectorA[i]);
            myStim.b[i - anIndex]       = xfpo_to_unsigned_long(aVectorB[i]);
        }

        myStim.ready                    = 1;
        myStim.in_tlast                 = (anIndex + FloatIfT::PARALLELISM) >= aTestSize;
        uint32_t in_mask                = (1UL << (aVectorA.size() - aTestSize)) - 1;

        for (int i = 0; i < FloatIfT::PARALLELISM; i++)
            myStim.in_mask[i]           = sim::get_bit(in_mask, i);

        return myStim;
    }

    void compareData(std::vector<StimT> & aVectorA, std::vector<StimT> & aVectorB)
    {
        std::vector<StimT> myExpectedOut = getExpectedData(aVectorA, aVectorB);
        auto myCapturedData = theFloatOpMonitor->getQueue();

        xip_fpo_t myXfpo;
        xip_fpo_init2(myXfpo, FloatIfT::E_WIDTH, FloatIfT::FRAC_WIDTH);
        auto myChunkCounter = 0;

        while (!myCapturedData.empty())
        {
            for (int i = 0; i < FloatIfT::PARALLELISM; i++)
            {
                if (myCapturedData.front().tlast && !(myCapturedData.front().tkeep[i])) // Data not valid
                    continue;

                auto myExpectedLong = xfpo_to_unsigned_long(myExpectedOut[myChunkCounter*FloatIfT::PARALLELISM+i]);

                ASSERT_EQ(myExpectedLong, myCapturedData.front().out[i])
                    << "Chunk: " << myChunkCounter
                    << ", i: " << i
                    << ", index: " << myChunkCounter*FloatIfT::PARALLELISM+i
                    << ", Expected: 0x" << std::hex << myExpectedLong
                    << ", Got: 0x" << std::hex << myCapturedData.front().out[i];

                unsigned_long_to_xfpo(myXfpo, myCapturedData.front().out[i]);

                ASSERT_EQ(xip_fpo_get_flt(myXfpo), xip_fpo_get_flt(myExpectedOut[myChunkCounter*FloatIfT::PARALLELISM+i]))
                    << "Chunk: " << myChunkCounter
                    << ", i: " << i
                    << ", index: " << myChunkCounter*FloatIfT::PARALLELISM+i;

                float myTempValue = xip_fpo_get_flt(myExpectedOut[myChunkCounter*FloatIfT::PARALLELISM+i]);

                ASSERT_EQ(myXfpo[0]._xip_fpo_exp, myExpectedOut[myChunkCounter*FloatIfT::PARALLELISM+i][0]._xip_fpo_exp)
                    << "Chunk: " << myChunkCounter
                    << ", i: " << i
                    << ", index: " << myChunkCounter*FloatIfT::PARALLELISM+i
                    << ", Expected: 0x" << std::hex << myExpectedOut[myChunkCounter*FloatIfT::PARALLELISM+i][0]._xip_fpo_exp
                    << ", Got: 0x" << std::hex << myXfpo[0]._xip_fpo_exp;

                if (std::abs(myTempValue) == INFINITY || myTempValue == NAN)
                    continue;

                ASSERT_EQ(*myXfpo[0]._xip_fpo_d, *myExpectedOut[myChunkCounter*FloatIfT::PARALLELISM+i][0]._xip_fpo_d)
                    << "Chunk: " << myChunkCounter
                    << ", i: " << i
                    << ", index: " << myChunkCounter*FloatIfT::PARALLELISM+i
                    << ", Expected: 0x" << std::hex << *myExpectedOut[myChunkCounter*FloatIfT::PARALLELISM+i][0]._xip_fpo_d
                    << ", Got: 0x" << std::hex << *myXfpo[0]._xip_fpo_d;

            }
            myCapturedData.pop();
            myChunkCounter++;
        }

        xip_fpo_clear(myXfpo);
    }

    void run(std::vector<StimT> & aVectorA, std::vector<StimT> & aVectorB, const int aTestSize)
    {
        for (int i = 0; i < aVectorA.size(); i += FloatIfT::PARALLELISM)
        {
            auto myStim = getWriteVectorChunk(aVectorA, aVectorB, i, aTestSize);
            theFloatOpDriver->add(myStim);
        }

        auto myExpectedQueueSize = ceil(static_cast<float>(aVectorA.size()) / FloatIfT::PARALLELISM);

        theSimulation.simulate([&]() {
            return theFloatOpMonitor->getQueue().size() >= myExpectedQueueSize;
        }, 10);

        EXPECT_EQ(theFloatOpMonitor->getQueue().size(), myExpectedQueueSize);

        compareData(aVectorA, aVectorB);
    }

    template <typename T = double>
    static std::vector<T> getRandomVector(const size_t aVectorSize, const unsigned long aMaxValue, const int32_t seed = -1)
    {

        auto myTestSize = sim::nearest_to_P(aVectorSize, FloatIfT::PARALLELISM);
        std::vector<T> aOut(myTestSize, 0);

        std::uniform_real_distribution<> dis(0., aMaxValue);

        for (int i = 0; i < aVectorSize; i++)
        {
            aOut[i] = dis(rng);
        }

        return aOut;
    }

    static std::vector<xip_fpo_t> floatToDFUINT (const std::vector<float> & aFloatVector, const int myEWidth, const int myFracWidth)
    {
        std::vector<xip_fpo_t> out(aFloatVector.size());

        for (int i = 0; i < aFloatVector.size(); i++)
        {
            xip_fpo_init2(out[i], myEWidth, myFracWidth);
            xip_fpo_set_flt(out[i], aFloatVector[i]);
        }

        return out;
    }

    virtual ~FloatOpTest() = default;

private:
    sim::Simulation<DeviceT>    theSimulation;
    std::shared_ptr<DriverT>    theFloatOpDriver;
    std::shared_ptr<MonitorT>   theFloatOpMonitor;
    FloatFuncT                  theFloatOp;
};

}
