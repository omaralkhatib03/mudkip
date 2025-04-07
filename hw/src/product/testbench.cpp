#include "Simulation.hpp"
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <format>
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
static constexpr int TEST_SIZE      = 10;
static std::mt19937 rng(839602695);

#pragma pack(push, 1)
template<size_t DATA_WIDTH, size_t E_WIDTH, size_t FRAC_WIDTH, size_t PARALLELISM>
struct FloatOpIf
{   
    // Input 
    bool                                    in_valid;
    std::array<unsigned long, PARALLELISM>  a;
    std::array<unsigned long, PARALLELISM>  b;
    bool                                    ready;

    // Output 
    bool                                    in_ready;
    std::array<unsigned long, PARALLELISM>  out;
    bool                                    valid;

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
            this->theDevice->a[i] = aStim.a[i];
            this->theDevice->b[i] = aStim.b[i];
        }

        this->theDevice->in_valid  = aStim.in_valid;
        this->theDevice->ready = aStim.ready;
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
                VL_PRINTF("Seein: %x\n", this->theDevice->out[i]);
            }

            theCurrentIntf.valid = this->theDevice->valid;
            this->add(theCurrentIntf);
            return;
        }

    }
FloatOpT theCurrentIntf{};
};

template<typename StimT, typename anOp>
struct FloatOpTest 
{
    using DriverT   = FloatOpDriver;
    using MonitorT  = FloatOpMonitor; 
    
    FloatOpTest(const std::string & aTestName = "") :
        theSimulation {  
            std::format("product{}{}", ((aTestName != "") ? "_" : ""), aTestName),
            sim::RunType::Release, 
            sim::TraceOption::TraceOn, 
            sim::ResetType::RANDOM_RESET, 
            10000
        },
        theVectorDriver{std::make_shared<DriverT>()},
        theVectorMonitor{std::make_shared<MonitorT>()}
    {
        theSimulation.addDriver(theVectorDriver);
        theSimulation.addMonitor(theVectorMonitor);
    }
    
    std::vector<StimT> getExpectedData(const std::vector<StimT> & aVectorA, const std::vector<StimT> & aVectorB)
    {
        std::vector<StimT> myOut(aVectorA.size());

        for (int i = 0; i < aVectorA.size(); i++)
        {
            xip_fpo_init2(myOut[i], E_WIDTH, FRAC_WIDTH);
            xip_fpo_mul(myOut[i], aVectorA[i], aVectorB[i]);
        }

        return myOut;
    }


    FloatOpT getWriteVectorChunk(std::vector<StimT> & aVectorA, std::vector<StimT> & aVectorB, int anIndex) // Chunk starting from anIndex
    {
        FloatOpT myStim = {0};
        myStim.in_valid = 1;

        for (int i = anIndex; i < anIndex + PARALLELISM; i++)
        {
            myStim.a[i - anIndex]   = xfpo_to_unsigned_long(aVectorA[i]);
            myStim.b[i - anIndex]   = xfpo_to_unsigned_long(aVectorB[i]);
        }
        myStim.ready = 1;
        return myStim;
    }
    
    void compareData(std::vector<StimT> & aVectorA, std::vector<StimT> & aVectorB)
    {
        std::vector<StimT> myExpectedOut = getExpectedData(aVectorA, aVectorB);
        auto myCapturedData = theVectorMonitor->getQueue();
        
        xip_fpo_t myXfpo;
        xip_fpo_init2(myXfpo, E_WIDTH, FRAC_WIDTH);
        auto myChunkCounter = 0;

        while (!myCapturedData.empty())
        {
            for (int i = 0; i < PARALLELISM; i++)
            {
                // auto myExpectedLong = xfpo_to_unsigned_long(myExpectedOut[myChunkCounter*PARALLELISM+i]);
                // ASSERT_EQ(myExpectedLong, myCapturedData.front().out[i]) 
                //     << "i: " << std::hex << i
                //     << ", Expected: 0x" << std::hex << myExpectedLong 
                //     << ", Got: 0x" << std::hex << myCapturedData.front().out[i];
                
                unsigned_long_to_xfpo(myXfpo, myCapturedData.front().out[i]);

                ASSERT_EQ(*myXfpo[0]._xip_fpo_d, *myExpectedOut[myChunkCounter*PARALLELISM+i][0]._xip_fpo_d)
                    << "i: " << std::hex << i
                    << ", Expected: 0x" << std::hex << *myExpectedOut[myChunkCounter*PARALLELISM+i][0]._xip_fpo_d 
                    << ", Got: 0x" << std::hex << *myXfpo[0]._xip_fpo_d;

                ASSERT_EQ(myXfpo[0]._xip_fpo_exp, myExpectedOut[myChunkCounter*PARALLELISM+i][0]._xip_fpo_exp)
                    << "i: " << std::hex << i
                    << ", Expected: 0x" << std::hex << myExpectedOut[myChunkCounter*PARALLELISM+i][0]._xip_fpo_exp 
                    << ", Got: 0x" << std::hex << myXfpo[0]._xip_fpo_exp;
            }
            myCapturedData.pop();
        }

        xip_fpo_clear(myXfpo);
    }

    void feedSimulation(std::vector<StimT> & aVectorA, std::vector<StimT> & aVectorB)
    {
        for (int i = 0; i < aVectorA.size(); i += PARALLELISM)
        { 
            auto myStim = getWriteVectorChunk(aVectorA, aVectorB, i);
            theVectorDriver->add(myStim);
        }

        theSimulation.simulate([&]() {
            return theVectorDriver->getQueue().size() == 0;
        }, 10);

        compareData(aVectorA, aVectorB);
    }

    virtual ~FloatOpTest() = default;
    
    sim::Simulation<DeviceT> theSimulation;
    std::shared_ptr<DriverT> theVectorDriver;
    std::shared_ptr<MonitorT> theVectorMonitor;
};

template <typename T>
std::vector<T> getRandomVector(size_t aVectorSize = -1)
{
    auto myTestSize = sim::nearest_to_P(TEST_SIZE, PARALLELISM);
    std::vector<T> aOut(myTestSize, 0);
    constexpr double max_value = static_cast<double>((1ULL << (E_WIDTH + FRAC_WIDTH - 1)) - 1);

    for (int i = 0; i < TEST_SIZE; i++)
    {
        // aOut[i] = std::generate_canonical<double, 128>(rng) * max_value; 
        aOut[i] = 3.14239; 
    }

    return aOut;
}

std::vector<xip_fpo_t> floatToDFUINT (const std::vector<float> & aFloatVector) 
{
    std::vector<xip_fpo_t> out(aFloatVector.size());

    for (int i = 0; i < aFloatVector.size(); i++)
    {
        xip_fpo_init2(out[i], E_WIDTH, FRAC_WIDTH);
        xip_fpo_set_flt(out[i], aFloatVector[i]);
    }
    
    return out;
}

int main (int argc, char *argv[]) 
{
    using VectorFloatTestT = FloatOpTest<xip_fpo_t, decltype(xip_fpo_mul)>; 

    auto theTest = VectorFloatTestT();
    
    std::vector<float> aVectorA = getRandomVector<float>();
    std::vector<float> aVectorB = getRandomVector<float>(aVectorA.size());

    auto aVectorUnionisedA = floatToDFUINT(aVectorA);
    auto aVectorUnionisedB = floatToDFUINT(aVectorB);

    xip_fpo_t myXfpo;
    xip_fpo_init2(myXfpo, E_WIDTH, FRAC_WIDTH);
    auto myChunkCounter = 0;

    VL_PRINTF("Input Vectors : \n");
    for (int i = 0; i < aVectorA.size(); i++)
    {
        xip_fpo_mul(myXfpo, aVectorUnionisedA[i], aVectorUnionisedB[i]);
        VL_PRINTF("i: %d, A: %lx, B: %lx, C: %lx\n", i, xfpo_to_unsigned_long(aVectorUnionisedA[i]), xfpo_to_unsigned_long(aVectorUnionisedB[i]), xfpo_to_unsigned_long(myXfpo));
    }
    VL_PRINTF("\n");
    xip_fpo_clear(myXfpo); 

    theTest.feedSimulation(aVectorUnionisedA, aVectorUnionisedB);

    return 0;
}

