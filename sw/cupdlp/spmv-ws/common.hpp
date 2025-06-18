using data_t = float;

#define BLOCK_SIZE 16 

#define MAX_VECTOR 32

#define MAX_ROWS 32
#define MIN_ROWS 10

#define MAX_NNZ 3000
#define MIN_NNZ 10

#define SPMV_UFACTOR 9
#define BIND_FACTOR 8
#define S 18
#define A 4


void load_x(data_t* x, data_t * x_local) {
    for (int i = 0; i < MAX_VECTOR; i++) {
#pragma HLS LOOP_TRIPCOUNT min=MIN_ROWS max=MAX_ROWS
        // #pragma HLS PIPELINE II=1
        // #pragma HLS PIPELINE II=2
        // #pragma HLS UNROLL factor=LD_X_FACTOR
        x_local[i] = x[i];
    }
}
