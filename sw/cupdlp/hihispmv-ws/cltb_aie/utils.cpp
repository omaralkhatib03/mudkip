#include "utils.hpp"
#include "BSLogger.hpp"
#include <cstdio>
#include <cstdlib>
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

void spMvCsC(int *c_beg, int *r_idx, data_t *val, int n, data_t * x, data_t ** out)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = c_beg[i]; j < c_beg[i + 1]; j++)
        {
            (*out)[r_idx[j]] += val[j] * x[i];
        }
    }
}

void spMvCsR(int *r_beg, int *c_idx, data_t *c_val, int m, data_t * x, data_t * out)
{
    LOG_INIT_CERR();

    // log(LOG_DEBUG) << "Entering CSR Compute\n";

    for (int i = 0; i < m; i++)
    {
        // log(LOG_DEBUG) << "M: " << i <<" \n";
        for (int j = r_beg[i]; j < r_beg[i + 1]; j++)
        {
            // log(LOG_DEBUG) << "j: " << j <<" \n";
            out[i] += c_val[j] * x[c_idx[j]]; // Multiply and Accumulate (MAC)
        }
    }
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
