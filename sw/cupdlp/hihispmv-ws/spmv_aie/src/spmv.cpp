#include <adf.h>
#include "adf/new_frontend/types.h"
#include "spmv.h"
#include "spmv_kernel.h"

using namespace adf;

spmv_graph spmv_I;
// LoadVector load_vectors_I;

#if defined(__AIESIM__) || defined(__X86SIM__)

#include <vector>
#include <string>
#include <fstream>

std::vector<int> readIntVector(const std::string &filename) {
    std::ifstream in(filename);
    std::vector<int> vec;
    int val;
    while (in >> val) vec.push_back(val);
    return vec;
}

std::vector<float> readFloatVector(const std::string &filename) {
    std::ifstream in(filename);
    std::vector<float> vec;
    float val;
    while (in >> val) vec.push_back(val);
    return vec;
}


bool validate(const std::vector<float> &ref, const std::vector<float> &test) 
{
    if (ref.size() != (test.size() - (ref.size() % ELEMENETS_PER_CHUNK != 0)))
    {
        std::cout << "Test is wrong size: " << test.size() << " Ref has size: " << ref.size() << std::endl;
        return false;
    }

    const float eps = 1e-6;

    for (int i = 0; i < ref.size(); ++i) {

        if (std::abs(ref[i] - test[i]) > eps) {
            std::cerr << "Mismatch at index " << i << ": " << ref[i] << " vs " << test[i] << "\n";
            // return false;
        }
    }
    return true;
}


int main(void) 
{
    int m = 27;
    int n = 32;
    int nnz = 83;


    auto r_ptr  = readIntVector("../../data/r_ptr.txt");
    auto c_idx  = readIntVector("../../data/c_idx.txt");
    auto vals   = readFloatVector("../../data/vals.txt");
    auto x      = readFloatVector("../../data/x.txt");

    std::vector<float> y(m, 0.0f);

    for (size_t row = 0; row < y.size(); ++row) 
    {
        for (int idx = r_ptr[row]; idx < r_ptr[row + 1]; ++idx) 
        {
            y[row] += vals[idx] * x[c_idx[idx]];
        }
    }

    // Then run kernel
    spmv_I.init();
    spmv_I.setLength(m + 1);
    spmv_I.setNNZ(nnz);
    spmv_I.run(1);
    spmv_I.end();

#if defined(__X86SIM__)
    auto ai_hw = readFloatVector("x86simulator_output/data/y_out.txt");

    bool val = validate(y, ai_hw);

    for (int i = 0 ; i < m; i++)
    {
        std::cout << "i: " <<  i << " y: " << y[i] << " hw: " << ai_hw[i] << std::endl;
    }

    if (val)
        return 0;
    else
        return 1;

#elif defined(__AIESIM__)
    return 0;
#endif

}

#endif
