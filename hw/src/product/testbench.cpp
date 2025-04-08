#include "Simulation.hpp"
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <format>
#include <functional>
#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <verilated.h>
#include "Utils.hpp"
#include "Vproduct.h"
#include "Vproduct_product.h"
#include "Controller.hpp"
#include "floating_point_v7_1_bitacc_cmodel.h"
#include <random>
#include "FloatOps.h"

using DeviceT                       = Vproduct;
static constexpr int DATA_WIDTH     = Vproduct_product::DATA_WIDTH;
static constexpr int E_WIDTH        = Vproduct_product::E_WIDTH;
static constexpr int FRAC_WIDTH     = Vproduct_product::FRAC_WIDTH;
static constexpr int PARALLELISM    = Vproduct_product::PARALLELISM;
static constexpr int TEST_SIZE      = 10e4;
static constexpr double MAX_VALUE   = static_cast<double>((1ULL << (E_WIDTH + FRAC_WIDTH)) - 1);
static                              std::mt19937 rng(sim::initialize_rng());
static                              std::uniform_real_distribution<> dis(.0, MAX_VALUE);

#pragma pack(push, 1)
template<size_t DATA_WIDTH, size_t E_WIDTH, size_t FRAC_WIDTH, size_t PARALLELISM>
struct FloatOpIf
{
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
#pragma pack(pop)

using FloatOpT = FloatOpIf<DATA_WIDTH, E_WIDTH, FRAC_WIDTH, PARALLELISM>;

class FloatOpDriver : public sim::Controller<DeviceT, FloatOpT>
{
    using Controller<DeviceT, FloatOpT>::Controller;

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

class FloatOpMonitor : public sim::Controller<DeviceT, FloatOpT>
{
public:
    using Controller<DeviceT, FloatOpT>::Controller;

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


template<typename StimT>
struct FloatOpTest
{
    using DriverT       = FloatOpDriver;
    using MonitorT      = FloatOpMonitor;
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
            xip_fpo_init2(myOut[i], E_WIDTH, FRAC_WIDTH);
            theFloatOp(myOut[i], aVectorA[i], aVectorB[i]);
        }

        VL_PRINTF("Expectation Vectors : \n");
        for (int i = 0; i < aVectorA.size(); i++)
        {
            VL_PRINTF("i: %d, A: %lx, B: %lx, C: %lx\n",
                      i,
                      xfpo_to_unsigned_long(aVectorA[i]),
                      xfpo_to_unsigned_long(aVectorB[i]),
                      xfpo_to_unsigned_long(myOut[i]));
        }
        VL_PRINTF("\n");

        return myOut;
    }

    FloatOpT getWriteVectorChunk(std::vector<StimT> & aVectorA, std::vector<StimT> & aVectorB, int anIndex) // Chunk starting from anIndex
    {
        FloatOpT myStim                 = {0};
        myStim.in_valid                 = 1;

        for (int i = anIndex; i < anIndex + PARALLELISM; i++)
        {
            myStim.a[i - anIndex]       = xfpo_to_unsigned_long(aVectorA[i]);
            myStim.b[i - anIndex]       = xfpo_to_unsigned_long(aVectorB[i]);
        }

        myStim.ready                    = 1;
        myStim.in_tlast                 = (anIndex + PARALLELISM) >= TEST_SIZE;
        uint32_t in_mask                = (1UL << (aVectorA.size() - TEST_SIZE)) - 1;

        for (int i = 0; i < PARALLELISM; i++)
            myStim.in_mask[i]           = sim::get_bit(in_mask, i);

        return myStim;
    }

    void compareData(std::vector<StimT> & aVectorA, std::vector<StimT> & aVectorB)
    {
        std::vector<StimT> myExpectedOut = getExpectedData(aVectorA, aVectorB);
        auto myCapturedData = theFloatOpMonitor->getQueue();

        xip_fpo_t myXfpo;
        xip_fpo_init2(myXfpo, E_WIDTH, FRAC_WIDTH);
        auto myChunkCounter = 0;

        while (!myCapturedData.empty())
        {
            for (int i = 0; i < PARALLELISM; i++)
            {
                if (myCapturedData.front().tlast && !(myCapturedData.front().tkeep[i])) // Data not valid
                    continue;

                auto myExpectedLong = xfpo_to_unsigned_long(myExpectedOut[myChunkCounter*PARALLELISM+i]);

                ASSERT_EQ(myExpectedLong, myCapturedData.front().out[i])
                    << "Chunk: " << myChunkCounter
                    << ", i: " << i
                    << ", index: " << myChunkCounter*PARALLELISM+i
                    << ", Expected: 0x" << std::hex << myExpectedLong
                    << ", Got: 0x" << std::hex << myCapturedData.front().out[i];

                unsigned_long_to_xfpo(myXfpo, myCapturedData.front().out[i]);

                ASSERT_EQ(xip_fpo_get_flt(myXfpo), xip_fpo_get_flt(myExpectedOut[myChunkCounter*PARALLELISM+i]))
                    << "Chunk: " << myChunkCounter
                    << ", i: " << i
                    << ", index: " << myChunkCounter*PARALLELISM+i;

                float myTempValue = xip_fpo_get_flt(myExpectedOut[myChunkCounter*PARALLELISM+i]);

                ASSERT_EQ(myXfpo[0]._xip_fpo_exp, myExpectedOut[myChunkCounter*PARALLELISM+i][0]._xip_fpo_exp)
                    << "Chunk: " << myChunkCounter
                    << ", i: " << i
                    << ", index: " << myChunkCounter*PARALLELISM+i
                    << ", Expected: 0x" << std::hex << myExpectedOut[myChunkCounter*PARALLELISM+i][0]._xip_fpo_exp
                    << ", Got: 0x" << std::hex << myXfpo[0]._xip_fpo_exp;

                if (std::abs(myTempValue) == INFINITY || myTempValue == NAN)
                    continue;

                ASSERT_EQ(*myXfpo[0]._xip_fpo_d, *myExpectedOut[myChunkCounter*PARALLELISM+i][0]._xip_fpo_d)
                    << "Chunk: " << myChunkCounter
                    << ", i: " << i
                    << ", index: " << myChunkCounter*PARALLELISM+i
                    << ", Expected: 0x" << std::hex << *myExpectedOut[myChunkCounter*PARALLELISM+i][0]._xip_fpo_d
                    << ", Got: 0x" << std::hex << *myXfpo[0]._xip_fpo_d;

            }
            myCapturedData.pop();
            myChunkCounter++;
        }

        xip_fpo_clear(myXfpo);
    }

    void run(std::vector<StimT> & aVectorA, std::vector<StimT> & aVectorB)
    {
        for (int i = 0; i < aVectorA.size(); i += PARALLELISM)
        {
            auto myStim = getWriteVectorChunk(aVectorA, aVectorB, i);
            theFloatOpDriver->add(myStim);
        }

        theSimulation.simulate([&]() {
            return theFloatOpMonitor->getQueue().size() >= ceil(static_cast<float>(aVectorA.size()) / PARALLELISM);
        }, 10);

        EXPECT_EQ(theFloatOpMonitor->getQueue().size(), ceil(static_cast<float>(aVectorA.size()) / PARALLELISM));

        compareData(aVectorA, aVectorB);
    }

    template <typename T>
    static std::vector<T> getRandomVector(size_t aVectorSize = -1)
    {
        auto myTestSize = sim::nearest_to_P(TEST_SIZE, PARALLELISM);
        std::vector<T> aOut(myTestSize, 0);

        for (int i = 0; i < TEST_SIZE; i++)
        {
            aOut[i] = dis(rng);
        }

        return aOut;
    }

    static std::vector<xip_fpo_t> floatToDFUINT (const std::vector<float> & aFloatVector)
    {
        std::vector<xip_fpo_t> out(aFloatVector.size());

        for (int i = 0; i < aFloatVector.size(); i++)
        {
            xip_fpo_init2(out[i], E_WIDTH, FRAC_WIDTH);
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

int main (int argc, char *argv[])
{
    using VectorFloatTestT      = FloatOpTest<xip_fpo_t>;

    auto theTest                = VectorFloatTestT(xip_fpo_mul);

    VL_PRINTF("Test Size: %d\n", TEST_SIZE);

    std::vector<float> aVectorA = VectorFloatTestT::getRandomVector<float>();
    std::vector<float> aVectorB = VectorFloatTestT::getRandomVector<float>(aVectorA.size());

    auto aVectorUnionisedA      = VectorFloatTestT::floatToDFUINT(aVectorA);
    auto aVectorUnionisedB      = VectorFloatTestT::floatToDFUINT(aVectorB);
    auto myChunkCounter         = 0;

    theTest.run(aVectorUnionisedA, aVectorUnionisedB);

    return 0;
}
