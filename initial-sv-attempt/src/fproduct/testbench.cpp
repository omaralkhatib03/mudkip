#include "Utils.hpp"
#include "Vfproduct.h"
#include "Vfproduct_fproduct.h"
#include "Float.hpp"
#include <random>

using DeviceT                                   = Vfproduct;
static constexpr int DATA_WIDTH                 = Vfproduct_fproduct::DATA_WIDTH;
static constexpr int E_WIDTH                    = Vfproduct_fproduct::E_WIDTH;
static constexpr int FRAC_WIDTH                 = Vfproduct_fproduct::FRAC_WIDTH;
static constexpr int PARALLELISM                = Vfproduct_fproduct::PARALLELISM;
static constexpr int TEST_SIZE                  = 1e4;
static constexpr unsigned long long MAX_VALUE   = (1ULL << (E_WIDTH + FRAC_WIDTH)) - 1;
using FloatIfT                                  = sim::FloatOpIf<DATA_WIDTH, E_WIDTH, FRAC_WIDTH, PARALLELISM>;

TEST(FloatingPointOps, FloatProduct)
{
    using VectorFloatTestT      = sim::FloatOpTest<FloatIfT, DeviceT, xip_fpo_t>;
    
    std::mt19937 myEngine(sim::initialize_rng());

    auto theTest                = VectorFloatTestT(xip_fpo_mul);
    VL_PRINTF("Test Size: %d\n", TEST_SIZE);

    std::vector<float> aVectorA = VectorFloatTestT::getRandomVector<float>(TEST_SIZE, MAX_VALUE, myEngine);
    std::vector<float> aVectorB = VectorFloatTestT::getRandomVector<float>(TEST_SIZE, MAX_VALUE, myEngine);

    auto aVectorUnionisedA      = VectorFloatTestT::floatToDFUINT(aVectorA, E_WIDTH, FRAC_WIDTH);
    auto aVectorUnionisedB      = VectorFloatTestT::floatToDFUINT(aVectorB, E_WIDTH, FRAC_WIDTH);
    auto myChunkCounter         = 0;

    theTest.run(aVectorUnionisedA, aVectorUnionisedB, TEST_SIZE);
}
