#include "adf/new_frontend/adf.h"
#include "adf/new_frontend/types.h"
#include "spmv_kernel.h"
#include <adf.h>

using namespace adf;

class spmv_graph : public graph 
{
private:
    kernel k_mult;
    kernel k_acc;

void make_connections(std::string s = "")
{
    x_in = input_plio::create("x_in" + s, adf::plio_32_bits, "data/x" + s + ".txt");
    c_idx_in = input_plio::create("c_idx_in" + s, plio_32_bits, "data/c_idx" + s + ".txt");
    vals_in = input_plio::create("vals_in" + s, plio_32_bits, "data/vals" + s + ".txt");
    r_ptr_in = input_plio::create("r_ptr_in" + s, plio_32_bits, "data/r_ptr" + s + ".txt");
    y_out = output_plio::create("y_out" + s, plio_32_bits, "data/y_out" + s + ".txt");

    k_mult = kernel::create(mult_products);
    k_acc = kernel::create(row_accumulate);

    source(k_mult) = "spmv_kernel.cc";
    source(k_acc) = "spmv_kernel.cc";

    runtime<ratio>(k_mult) = 0.5;
    runtime<ratio>(k_acc) = 0.5;

    connect<stream>(c_idx_in.out[0], k_mult.in[0]);
    connect<stream>(vals_in.out[0], k_mult.in[1]);
    connect<>(x_in.out[0], k_mult.in[2]);

    connect<>(r_ptr_in.out[0], k_acc.in[0]);
    connect<stream>(k_mult.out[0], k_acc.in[1]);
    connect<stream>(k_acc.out[0], y_out.in[0]);

    dimensions(k_acc.in[0]) = {HW_SIDE_LEN};
    dimensions(k_mult.in[2]) = {HW_SIDE_LEN};
}

public:
    input_plio x_in;
    input_plio c_idx_in;
    input_plio vals_in;
    input_plio r_ptr_in;

    output_plio y_out;
    
    spmv_graph(int id) {
        make_connections(std::to_string(id));
    }

    spmv_graph() 
    {
        make_connections();
    }
    
    void setLength(int length)
    {
        this->update(k_acc.in[2], length);
    }

    void setNNZ(int nnz)
    {
        this->update(k_mult.in[3], nnz);
    }

};


class top_graph : public adf::graph {
public:
    static constexpr int M = 4;  // number of tiles
    spmv_graph* spmv_tiles[M];

    top_graph() {
        for (int i = 0; i < M; ++i) {
            spmv_tiles[i] = new spmv_graph(i);
        }
    }
};


//
// using namespace adf;
//
// class spmv_graph : public graph 
// {
// private:
//     kernel k_mult;
//     kernel k_acc;
//     int vector_length;
//
//     void make_connections()
//     {
//         x_in = input_plio::create("x_in", plio_32_bits, "data/x.txt");
//         c_idx_in = input_plio::create("c_idx_in", plio_32_bits, "data/c_idx.txt");
//         vals_in = input_plio::create("vals_in", plio_32_bits, "data/vals.txt");
//         r_ptr_in = input_plio::create("r_ptr_in", plio_32_bits, "data/r_ptr.txt");
//         y_out = output_plio::create("y_out", plio_32_bits, "data/y_out.txt");
//
//         k_mult = kernel::create(mult_products);
//         k_acc = kernel::create(row_accumulate);
//
//         source(k_mult) = "spmv_kernel.cc";
//         source(k_acc) = "spmv_kernel.cc";
//
//         runtime<ratio>(k_mult) = 0.5;
//         runtime<ratio>(k_acc) = 0.5;
//
//         connect< window<ELEMENETS_PER_CHUNK * sizeof(data_t)> >(c_idx_in.out[0], k_mult.in[0]);
//         connect< window<ELEMENETS_PER_CHUNK * sizeof(data_t)> >(vals_in.out[0], k_mult.in[1]);
//         connect< stream >(x_in.out[0], k_mult.in[2]);
//
//         connect< window<ELEMENETS_PER_CHUNK * sizeof(data_t)> >(r_ptr_in.out[0], k_acc.in[0]);
//         connect< window<ELEMENETS_PER_CHUNK * sizeof(data_t)> >(k_mult.out[0], k_acc.in[1]);
//
//         connect< window<ELEMENETS_PER_CHUNK * sizeof(data_t)> >(k_acc.out[0], y_out.in[0]);
//
//     }
//
// public:
//     input_plio x_in;
//     input_plio c_idx_in;
//     input_plio vals_in;
//     input_plio r_ptr_in;
//     port<direction::in> vector_length_port;
//
//     output_plio y_out;
//
//     spmv_graph() 
//     {
//         make_connections();
//     }
//
//     spmv_graph(int vec_len) : 
//         vector_length(vec_len)
//     {
//         this->update(vector_length_port, vector_length);
//         make_connections();
//     }
//
//     void setLength(int length)
//     {
//         this->update(k_acc.in[2], vector_length);
//         this->update(k_mult.in[3], vector_length);
//     }
//
//     int getLength()
//     {
//         return vector_length;
//     }
// };

// class LoadVector : public graph 
// {
// private:
//     kernel k_x_mem;
//     int vector_length;
// public: 

//     input_plio x_in;

//     LoadVector()
//     {
//         x_in = input_plio::create("x_in", plio_32_bits, "data/x.txt");

//         k_x_mem = kernel::create(load_x);

//         source(k_x_mem) = "spmv_kernel.cc";

//         runtime<ratio>(k_x_mem) = 0.3;
//         connect< stream >(x_in.out[0], k_x_mem.in[0]);
//     }

//     void setVectorLength(int vector_length)
//     {
//         this->update(k_x_mem.in[1], vector_length);
         
//     }
// };