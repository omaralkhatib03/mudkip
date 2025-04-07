#include "Simulation.hpp"
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <format>
#include <memory>
#include <vector>
#include <verilated.h>
#include "Utils.hpp"
#include "Vproduct.h"
#include "Vproduct_product.h"
#include "Controller.hpp"
#include "floating_point_v7_1_bitacc_cmodel.h"
#include "FloatOps.h"

using DeviceT                       = Vproduct;
static constexpr int DATA_WIDTH     = Vproduct_product::DATA_WIDTH;
static constexpr int E_WIDTH        = Vproduct_product::E_WIDTH;
static constexpr int FRAC_WIDTH     = Vproduct_product::FRAC_WIDTH;
static constexpr int PARALLELISM    = Vproduct_product::PARALLELISM;
static constexpr int MAX_TEST_SIZE  = 4;

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

class VectorRamMonitor : public sim::Controller<DeviceT, FloatOpT>
{
public:
    using Controller<DeviceT, FloatOpT>::Controller;

    void reset() override {}

    void next() override
    {
        if (this->theDevice->valid)
        {
            memcpy(&this->theDevice->out, theCurrentIntf.out.data(), sim::ceil_deiv(DATA_WIDTH * PARALLELISM, 8));
            theCurrentIntf.valid = this->theDevice->valid;
            this->add(theCurrentIntf);
            return;
        }

    }

    FloatOpT theCurrentIntf{};
};

template<typename StimT>
struct FloatOpTest 
{
    using DriverT   = FloatOpDriver;
    using MonitorT  = VectorRamMonitor; 
    
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

    void feedSimulation(std::vector<StimT> & aVectorA, std::vector<StimT> & aVectorB, size_t aVectorLength)
    {
        for (int i = 0; i < aVectorLength; i += PARALLELISM)
        { 
            auto myStim = getWriteVectorChunk(aVectorA, aVectorB, i);
            theVectorDriver->add(myStim);
        }

        theSimulation.simulate([&]() {
            return theVectorDriver->getQueue().size() == 0;
        }, 3);
    }

    virtual ~FloatOpTest() = default;
    
    sim::Simulation<DeviceT> theSimulation;
    std::shared_ptr<DriverT> theVectorDriver;
    std::shared_ptr<MonitorT> theVectorMonitor;
};

template <typename T>
std::vector<T> getRandomVector(size_t aVectorSize = -1)
{
    std::vector<T> aOut;

    // auto aRandomVectorSize = (aVectorSize == -1) ? int(rand()) % MAX_TEST_SIZE : aVectorSize;
    auto aRandomVectorSize = 1; 
    aOut.resize(aRandomVectorSize * PARALLELISM);

    for (int i = 0; i < aRandomVectorSize; i++)
    {
        aOut[i] = 72; 
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
    using VectorFloatTestT = FloatOpTest<xip_fpo_t>; 

    auto theTest = VectorFloatTestT();
    
    std::vector<float> aVectorA = getRandomVector<float>();
    std::vector<float> aVectorB = getRandomVector<float>(aVectorA.size());

    auto aVectorUnionisedA = floatToDFUINT(aVectorA);
    auto aVectorUnionisedB = floatToDFUINT(aVectorB);

    VL_PRINTF("Input Vectors : \n");
    for (int i = 0; i < aVectorA.size(); i++)
    {
        VL_PRINTF("A: i: %d, %lx\n", i, xfpo_to_unsigned_long(aVectorUnionisedA[i]));
        VL_PRINTF("B: i: %d, %lx\n", i, xfpo_to_unsigned_long(aVectorUnionisedB[i]));
    }
    VL_PRINTF("\n");
    
    theTest.feedSimulation(aVectorUnionisedA, aVectorUnionisedB, aVectorA.size());

    return 0;
}

