#include <iostream>
#include <ostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ap_int.h>
#include <array>
#include <cstdint>
#include <cstdio>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include <sys/types.h>
#include "read_matrix.hpp"
#include <vector>
#include <cstring>


void makeRandomCsCMatrix(int **c_beg, int **r_idx, data_t **val, int m, int n, int nnz) 
{

    int *temp_rows = (int *)malloc(nnz * sizeof(int));
    int *temp_cols = (int *)malloc(nnz * sizeof(int));
    data_t *temp_vals = (data_t *)malloc(nnz * sizeof(data_t));

    for (int i = 0; i < nnz; ++i) {
        int unique = 0;
        while (!unique) {
            temp_rows[i] = rand() % m;
            temp_cols[i] = rand() % n;
            unique = 1;
            for (int j = 0; j < i; ++j) {
                if (temp_rows[i] == temp_rows[j] && temp_cols[i] == temp_cols[j]) {
                    unique = 0;
                    break;
                }
            }
        }
        temp_vals[i] = (data_t)(rand() % 255); 
    }

    for (int i = 0; i < nnz - 1; ++i) {
        for (int j = 0; j < nnz - i - 1; ++j) {
            if (temp_cols[j] > temp_cols[j + 1] ||
                (temp_cols[j] == temp_cols[j + 1] && temp_rows[j] > temp_rows[j + 1])) {

                int temp = temp_rows[j];
                temp_rows[j] = temp_rows[j + 1];
                temp_rows[j + 1] = temp;

                temp = temp_cols[j];
                temp_cols[j] = temp_cols[j + 1];
                temp_cols[j + 1] = temp;

                data_t temp_val = temp_vals[j];
                temp_vals[j] = temp_vals[j + 1];
                temp_vals[j + 1] = temp_val;
            }
        }
    }

    *c_beg = (int *)malloc((n + 1) * sizeof(int));
    *r_idx = (int *)malloc(nnz * sizeof(int));
    *val = (data_t *)malloc(nnz * sizeof(data_t));

    for (int i = 0; i <= n; ++i) {
        (*c_beg)[i] = 0;
    }

    for (int i = 0; i < nnz; ++i) {
        (*r_idx)[i] = temp_rows[i];
        (*val)[i] = temp_vals[i];
        (*c_beg)[temp_cols[i] + 1]++;
    }

    for (int i = 1; i <= n; ++i) {
        (*c_beg)[i] += (*c_beg)[i - 1];
    }

    free(temp_rows);
    free(temp_cols);
    free(temp_vals);
}

void printMatrix(int *beg, int *idx, data_t *val, int n) {
    printf("Index: \t\t\t\t");
    for (int i = 0; i <= n; ++i)
    {
        printf("%d\t", i); 
    }

    printf("\nRow Offsets (r_beg): \t\t");
    for (int i = 0; i <= n; ++i) {
        printf("%d \t", beg[i]);
    }

    printf("\nColumn Indicies (c_idx): \t");
    for (int i = 0; i < n; ++i) {
        for (int j = beg[i]; j < beg[i + 1]; ++j) {
            printf("%d \t", idx[j]);
        }
    }

    printf("\nMatrix Elements (c_val): \t");
    for (int i = 0; i < n; ++i) {
        for (int j = beg[i]; j < beg[i + 1]; ++j) {
            printf("%.f \t", val[j]);
        }
    }
    printf("\n");
}

void printVector(data_t * x, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%i: %2.f \n", i, x[i]);
    }
}

void csCToCsR(int *c_beg, int *r_idx, data_t *val, int m, int n, int **r_beg, int **c_idx, data_t **out, int nnz) {
    *r_beg = (int *)malloc((m + 1) * sizeof(int));
    *c_idx = (int *)malloc(nnz * sizeof(int));
    *out = (data_t *)malloc(nnz * sizeof(data_t));

    memset(*r_beg, 0, (m + 1) * sizeof(int));

    for (int i = 0; i < nnz; ++i) {
        (*r_beg)[r_idx[i] + 1]++;
    }

    for (int i = 1; i <= m; ++i) {
        (*r_beg)[i] += (*r_beg)[i - 1];
    }

    int *temp_row_start = (int *)malloc(m * sizeof(int));
    memcpy(temp_row_start, *r_beg, m * sizeof(int)); 

    for (int col = 0; col < n; ++col) {
        for (int i = c_beg[col]; i < c_beg[col + 1]; ++i) {
            int row = r_idx[i];
            int pos = temp_row_start[row]++;

            (*c_idx)[pos] = col;
            (*out)[pos] = val[i];
        }
    }

    free(temp_row_start); 
}

int randInRange(int min, int max) {
    return min + rand() % (max - min);
}

void makeRandomRowPtrs(int *r_idx, int m, int nnz) {
    int non_zero_per_row = nnz / m;
    int remainder = nnz % m;
    
    r_idx[0] = 0;
    
    for (int i = 1; i < m; i++) {
        r_idx[i] = r_idx[i - 1] + non_zero_per_row + (i <= remainder ? 1 : 0);
    }
    
    r_idx[m] = nnz;
}

void makeRandomCidx(int *c_idx, int nnz, int n) {
    for (int i = 0; i < nnz; i++) {
        c_idx[i] = randInRange(0, n);
    }
}

void makeRandomValues(data_t *vals, int nnz) {
    for (int i = 0; i < nnz; i++) {
        vals[i] = static_cast<data_t>(randInRange(1, 100));
    }
}

void makeRandomCsr(int *r_idx, int *c_idx, data_t *vals, int m, int n, int nnz) {
    makeRandomRowPtrs(r_idx, m, nnz);
    makeRandomCidx(c_idx, nnz, n);
    makeRandomValues(vals, nnz);
}

template<typename T>
uintDDR_t pack_array_to_ddr(const T arr[EL_PER_DDR]) {
    uintDDR_t result = 0;

    for (int i = 0; i < EL_PER_DDR; ++i) {
        uint64_t temp = 0;
        std::memcpy(&temp, &arr[i], sizeof(T));
        result |= (uintDDR_t(temp) << (i * ELEMENT_WIDTH));
    }

    return result;
}
template<typename T>
std::vector<uintDDR_t> pack_vector_to_ddr(const T* data, int total_elements) {
    int ddr_words = (total_elements + EL_PER_DDR - 1) / EL_PER_DDR;
    std::vector<uintDDR_t> ddr_data(ddr_words);

    for (int i = 0; i < ddr_words; i++) {
        T buffer[EL_PER_DDR] = {0};
        for (int j = 0; j < EL_PER_DDR; j++) {
            int idx = i * EL_PER_DDR + j;
            if (idx < total_elements)
                buffer[j] = data[idx];
        }
        ddr_data[i] = pack_array_to_ddr(buffer);
    }
    return ddr_data;
}

template<typename T>
std::vector<T> unpack_ddr_to_vector(const uintDDR_t* ddr_data, int total_elements) {
    int ddr_words = (total_elements + EL_PER_DDR - 1) / EL_PER_DDR;
    std::vector<T> output(total_elements);

    for (int i = 0; i < ddr_words; i++) {
        T unpacked[EL_PER_DDR] = {0};
        std::memcpy(unpacked, &ddr_data[i], sizeof(uintDDR_t));
        for (int j = 0; j < EL_PER_DDR; j++) {
            int idx = i * EL_PER_DDR + j;
            if (idx < total_elements)
                output[idx] = unpacked[j];
        }
    }
    return output;
}

template<typename T>
T* malloc_aligned(int num_elements) {
    return (T*) malloc(sizeof(T) * num_elements);
}

void paddToDDR(int * r_beg, int * c_idx, data_t * val)
{
    int nnz = r_beg[0];
    int pad = (EL_PER_DDR - (nnz % EL_PER_DDR)) % EL_PER_DDR;
    for (int i = 0; i < pad; ++i) {
        c_idx[nnz + i] = 0;
        val[nnz + i] = 0;
    }
    r_beg[0] += pad;
}

int updateRowPtrSize(int n)
{
    return n % (EL_PER_DDR - 1) == 0 ? n : n + ((EL_PER_DDR - 1) - (n % (EL_PER_DDR - 1)));
}

int updateSize(int n)
{
    return (n + EL_PER_DDR - 1) & ~(EL_PER_DDR - 1);
}

int test(int m, int n, int nnz)
{
    data_t sparsity = (float) nnz / (m * n);
    //   data_t sparsity = 0.0000000627;
    nnz = (unsigned) (sparsity * m * n);
    clock_t c0;
    clock_t c1;
    data_t runtime_diff_ms;

    m = updateSize(m) - 1;
    n = updateSize(n); // doesn't matter iirc
    nnz = updateSize(nnz); 

    printf("m x n: %i x %i\n",(int) m, (int) n);

    printf("sparsity: %e\n", sparsity);

    int * c_beg = NULL;
    int * r_idx = NULL;
    data_t * r_val = NULL;

    int * r_beg = NULL;
    int * c_idx = NULL;
    data_t * c_val = NULL;

    printf("nnz: %e\n", (float) nnz);
    
    std::cout << "Mallocing Input Vector \n";
    data_t * x = (data_t * ) malloc(sizeof(data_t) * n);

    for (int i = 0; i < n; i++)
    {
        x[i] = 1;
    }

    // printf("Input Vector (x): \n");
    // printVector(x, n);
    // printf("\n");

    std::cout << "Mallocing Output Vector ... \n";
    data_t * out = (data_t * ) malloc(sizeof(data_t) * m);
    memset(out, 0, sizeof(data_t) * m);

    std::cout << "Mallocing HW Output Vector ... \n";
    data_t * hw_out = (data_t * ) malloc(sizeof(data_t) * m);
    memset(hw_out, 0, sizeof(data_t) * m);

    std::cout << "Building Random CSC Matrix ... \n";
    makeRandomCsCMatrix(&c_beg, &r_idx, &r_val, m, n, nnz);

    std::cout << "Converting to CSR ... \n";
    csCToCsR(c_beg, r_idx, r_val, m, n, &r_beg, &c_idx, &c_val, nnz);

    printf("Matrix A:\n");
    printMatrix(r_beg, c_idx, c_val, m);
    printf("\n");

    auto r_beg_ddr  = pack_vector_to_ddr(r_beg, m + 1);
    auto c_idx_ddr  = pack_vector_to_ddr(c_idx, nnz);
    auto c_val_ddr  = pack_vector_to_ddr(c_val, nnz);

    int ddr_words = (nnz + EL_PER_DDR - 1) / EL_PER_DDR;
    uintDDR_t* r_idx_ddr = malloc_aligned<uintDDR_t>(ddr_words);
    uintDDR_t* c_ids_ddr = malloc_aligned<uintDDR_t>(ddr_words);
    uintDDR_t* vals_ddr  = malloc_aligned<uintDDR_t>(ddr_words);

    csr_to_2d(
        r_beg_ddr.data(),
        c_idx_ddr.data(),
        c_val_ddr.data(),
        x,
        r_idx_ddr,
        c_ids_ddr,
        vals_ddr,
        m,
        nnz
    );

    auto r_idx_vec     = unpack_ddr_to_vector<int>(r_idx_ddr, nnz);
    auto c_idx_vec_out = unpack_ddr_to_vector<int>(c_ids_ddr, nnz);
    auto val_vec_out   = unpack_ddr_to_vector<data_t>(vals_ddr, nnz);

    std::vector<int> r_idx_gold(nnz);
    for (int i = 0; i < m; i++) {
        for (int j = r_beg[i]; j < r_beg[i + 1]; j++) {
            r_idx_gold[j] = i;
        }
    }

    bool success = true;
    for (int i = 0; i < nnz; i++) {
        if (r_idx_vec[i] != r_idx_gold[i] || c_idx_vec_out[i] != c_idx[i] || val_vec_out[i] != c_val[i]) {
            std::cout << "i=" << i
                      << " HW(r=" << r_idx_vec[i] << ", c=" << c_idx_vec_out[i] << ", v=" << val_vec_out[i] << ")"
                      << " != GOLD(r=" << r_idx_gold[i] << ", c=" << c_idx[i] << ", v=" << c_val[i] << ")\n";
            success = false;
        }
    }

    if (success) {
        std::cout << "✅ Test passed: Hardware matches golden reference\n";
    } else {
        std::cout << "❌ Test failed: Mismatch found\n";
    }

    return success ? 0 : 1;

    free(r_val);
    free(c_beg);
    free(c_idx);
    free(c_val);
    free(r_beg);
    free(out);
    free(hw_out);
    free(x);
    free(r_idx_ddr);
    free(c_ids_ddr);
    free(vals_ddr);

}


int main(int argc, char *argv[])
{

    data_t m    = 8;
    data_t n    = 8;
    data_t nnz  = 16;

    bool passed = 1;
    // passed &= test(m, n, nnz);
    // passed &= test(16, n, nnz);
    // passed &= test(16, 8, 64);
    // passed &= test(16, 8, 64);
    // passed &= test(32,32, 16);
    // passed &= test(32, 16, 64);
    passed &= test(300, 300, 271);

    return passed;
}
