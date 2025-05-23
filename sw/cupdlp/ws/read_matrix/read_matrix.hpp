#include <ap_int.h>
#include <array>
#include <cmath>
#include <cstdio>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <sys/types.h>

using data_t = float;

constexpr unsigned flog2(unsigned x) {
    unsigned r = 0;
    while (x >>= 1) ++r;
    return r;
}

constexpr unsigned clog2(unsigned x) {
    return x <= 1 ? 0 : flog2(x - 1) + 1;
}


constexpr int DDR_WIDTH =  512;
constexpr int ELEMENT_WIDTH = sizeof(data_t) * 8;
constexpr int EL_PER_DDR =  DDR_WIDTH / ELEMENT_WIDTH;
constexpr int PTR_WIDTH = clog2(EL_PER_DDR + 1);

using uint512_t = ap_uint<512>;
using uintDDR_t = ap_uint<DDR_WIDTH>;

using ddr_stream_t = hls::stream<uintDDR_t>;

template<typename T>
using ddr_arr_t = std::array<T, EL_PER_DDR>;
using arr_ptr_t = ap_uint<PTR_WIDTH>;

template<typename T>
using ddr_arr_internal_t = T[EL_PER_DDR];

template<typename T>
using arr_stream_t = hls::stream<ddr_arr_t<T>>; 

void csr_to_2d(
    uintDDR_t   * r_beg,  
    uintDDR_t   * c_idx,
    uintDDR_t   * c_val,
    data_t      * x,
    uintDDR_t   * r_idx, 
    uintDDR_t   * c_ids, 
    uintDDR_t   * vals,    
    int m,
    int nnz
);
