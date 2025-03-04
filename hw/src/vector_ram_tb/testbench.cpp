
#include "Controller.hpp"
#include "Signal.hpp"
#include "Simulation.hpp"
#include "Vvector_ram_tb.h"
#include "Vvector_ram_tb_vector_ram_tb.h"
#include "verilated.h"
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <format>
#include <memory>
#include <gtest/gtest.h>

using DeviceT = Vvector_ram_tb;

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

static constexpr int NUMBER_OF_RAMS = Vvector_ram_tb_vector_ram_tb::NUMBER_OF_RAMS; 
static constexpr int RAM_FIFO_DEPTH = Vvector_ram_tb_vector_ram_tb::RAM_FIFO_DEPTH;
static constexpr int VECTOR_LENGTH  = Vvector_ram_tb_vector_ram_tb::VECTOR_LENGTH;
static constexpr int DATA_WIDTH     = Vvector_ram_tb_vector_ram_tb::DATA_WIDTH;
static constexpr int PARALLELISM    = Vvector_ram_tb_vector_ram_tb::PARALLELISM;
static constexpr int ADDR_WIDTH     = clog2(VECTOR_LENGTH);

#pragma pack(push, 1)
template<size_t DataWidth, size_t AddressWidth, size_t Parallelism>
struct VectorRamIf 
{
    // Inputs
    std::array<uint32_t, Parallelism>       addr;
    std::array<uint32_t, Parallelism>       wdata;
    bool                                    write;
    bool                                    valid;
    bool                                    rready;
    
    // Outputs
    std::array<uint32_t, Parallelism>       rdata;
    bool                                    rvalid;
    bool                                    ready;
};
#pragma pack(pop)

using VectorRamT = VectorRamIf<DATA_WIDTH, ADDR_WIDTH, PARALLELISM>;

class VectorDriver : public sim::Controller<DeviceT, VectorRamT>
{
    using Controller<DeviceT, VectorRamT>::Controller;
    
    void driveFifoIntf(VectorRamT aStim)
    {
        /*memcpy(&this->theDevice->addr, aStim.addr.data(), ceil_deiv(ADDR_WIDTH * PARALLELISM, 8));*/
        /*memcpy(&this->theDevice->wdata, aStim.wdata.data(), ceil_deiv(DATA_WIDTH * PARALLELISM, 8));*/

        for (int i = 0; i < PARALLELISM; i++)
        {
            this->theDevice->addr[i]    = aStim.addr[i];
            this->theDevice->wdata[i]   = aStim.wdata[i];
        }

        this->theDevice->valid  = aStim.valid;
        this->theDevice->write  = aStim.write;
        this->theDevice->rready = aStim.rready;
    }

    void reset() override
    {
        VectorRamT aResetStim  = {0};
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

class VectorRamMonitor : public sim::Controller<DeviceT, VectorRamT>
{
public:
    using Controller<DeviceT, VectorRamT>::Controller;

    void reset() override {}

    void next() override
    {
        if (this->theDevice->rvalid)
        {
            /*memcpy(&this->theDevice->rdata, theCurrentIntf.rdata.data(), ceil_deiv(DATA_WIDTH * PARALLELISM, 8));*/

            for (int i = 0; i < PARALLELISM; i++)
            {
                theCurrentIntf.rdata[i] = this->theDevice->rdata[i];
            }

            theCurrentIntf.rvalid = this->theDevice->rvalid;
            this->add(theCurrentIntf);
            return;
        }

    }

    VectorRamT theCurrentIntf{};
};

template<typename StimT = float>
struct VectorRamTest 
{
    using DriverT   = VectorDriver;
    using MonitorT  = VectorRamMonitor; 
    using VectorT   = std::array<StimT, VECTOR_LENGTH>;

    static constexpr int NUMBER_OF_CHUNKS = VECTOR_LENGTH / PARALLELISM;
    
    VectorRamTest(const std::string & aTestName = "") :
        theSimulation {  
            std::format("vector_ram_test{}{}", ((aTestName != "") ? "_" : ""), aTestName),
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
    
    VectorRamT getWriteVectorChunk(const VectorT & aVector, int anIndex) // Chunk starting from anIndex
    {
        VectorRamT myStim = {0};
        myStim.valid = 1;
        myStim.write = 1;

        for (int i = anIndex; i < anIndex + PARALLELISM; i++)
        {
            myStim.wdata[i - anIndex]   = aVector[i];
            myStim.addr[i - anIndex]    = i;
        }

        return myStim;
    }

    VectorRamT getReadVectorChunk(int anIndex) // Chunk starting from anIndex
    {
        VectorRamT myStim = {0};
        myStim.valid = 1;

        for (int i = anIndex; i < anIndex + PARALLELISM; i++)
        {
            myStim.addr[i - anIndex]  = i;
        }

        return myStim;
    }

    void writeVector(VectorT aVector)
    {
        for (int i = 0; i < VECTOR_LENGTH; i += PARALLELISM)
        { 
            auto myStim = getWriteVectorChunk(aVector, i);
            theVectorDriver->add(myStim);
        }

        theSimulation.simulate([&](){
            return theVectorDriver->getQueue().size() == 0;
        });
    }
    
    VectorT readVector()
    {
        for (int i = 0; i < VECTOR_LENGTH; i+=PARALLELISM)
        {
            auto aStim = getReadVectorChunk(i);
            theVectorDriver->add(aStim);
        }

        theSimulation.simulate([&](){
            return theVectorMonitor->getQueue().size() >= NUMBER_OF_CHUNKS;
        });

        auto myChunksQueue = theVectorMonitor->getQueue();
        VectorT myReturnVector;
        
        while (!myChunksQueue.empty())
        {
            assert (myChunksQueue.front().rvalid);
            for (int i = 0; i < PARALLELISM; i++)
            {
                myReturnVector[i] = myChunksQueue.front().rdata[i];
            }
            myChunksQueue.pop();
        }

        return myReturnVector; 
    }

    virtual ~VectorRamTest() = default;
    
    sim::Simulation<DeviceT> theSimulation;
    std::shared_ptr<DriverT> theVectorDriver;
    std::shared_ptr<MonitorT> theVectorMonitor;
};


template <typename T>
std::array<T, VECTOR_LENGTH> getRandomVector()
{
    std::array<T, VECTOR_LENGTH> aOut = {0};

    for (int i = 0; i < VECTOR_LENGTH; i++)
    {
        aOut[i] = rand(); 
    }

    return aOut;
}

int main (int argc, char *argv[]) 
{
    using VectorFloatTestT = VectorRamTest<float>; 

    auto theTest = VectorFloatTestT();
    
    VectorFloatTestT::VectorT aVectorX = getRandomVector<float>();
    
    VL_PRINTF("Input Vector: ");
    for (int i = 0; i < VECTOR_LENGTH; i++)
    {
        VL_PRINTF("%x ", static_cast<int>(aVectorX[i]));
    }

    VL_PRINTF("\n");

    theTest.writeVector(aVectorX);
    
    auto aReturnedVector = theTest.readVector();

    VL_PRINTF("Recieved Vector: ");
    for (int i = 0; i < VECTOR_LENGTH; i++)
    {
        VL_PRINTF("%x ", static_cast<int>(aReturnedVector[i]));
    }

    VL_PRINTF("\n");



    return 0;
}











