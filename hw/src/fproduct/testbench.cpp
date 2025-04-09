#include "Vfproduct.h"
#include "Vfproduct_fproduct.h"
#include "Float.hpp"

using DeviceT                                   = Vfproduct;
static constexpr int DATA_WIDTH                 = Vfproduct_fproduct::DATA_WIDTH;
static constexpr int E_WIDTH                    = Vfproduct_fproduct::E_WIDTH;
static constexpr int FRAC_WIDTH                 = Vfproduct_fproduct::FRAC_WIDTH;
static constexpr int PARALLELISM                = Vfproduct_fproduct::PARALLELISM;
static constexpr int TEST_SIZE                  = 10e4;
static constexpr unsigned long long MAX_VALUE   = (1ULL << (E_WIDTH + FRAC_WIDTH)) - 1;
using FloatIfT                                  = FloatOpIf<DATA_WIDTH, E_WIDTH, FRAC_WIDTH, PARALLELISM>;

int main (int argc, char *argv[])
{
    using VectorFloatTestT      = FloatOpTest<FloatIfT, DeviceT, xip_fpo_t>;

    auto theTest                = VectorFloatTestT(xip_fpo_mul);

    VL_PRINTF("Test Size: %d\n", TEST_SIZE);

    std::vector<float> aVectorA = VectorFloatTestT::getRandomVector<float>(TEST_SIZE, MAX_VALUE);
    std::vector<float> aVectorB = VectorFloatTestT::getRandomVector<float>(TEST_SIZE, MAX_VALUE);

    auto aVectorUnionisedA      = VectorFloatTestT::floatToDFUINT(aVectorA, E_WIDTH, FRAC_WIDTH);
    auto aVectorUnionisedB      = VectorFloatTestT::floatToDFUINT(aVectorB, E_WIDTH, FRAC_WIDTH);
    auto myChunkCounter         = 0;

    theTest.run(aVectorUnionisedA, aVectorUnionisedB, TEST_SIZE);

    return 0;
}
