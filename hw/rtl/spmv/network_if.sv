`timescale 1ns / 1ps

interface network_if;

    parameter IN_WIDTH      = 32;
    parameter ID_WIDTH      = 32;

    typedef struct {
        logic [ID_WIDTH-1:0]    id;
        logic [IN_WIDTH-1:0]    val;
        logic                   valid;
    } spmv_network_struct;
    
    spmv_network_struct a;
    spmv_network_struct b;
    logic               ready;
    
    modport slave(
        input  a,
        input  b,
        output ready
    );
    
    modport master(
        output  a,
        output  b,
        input ready
    );

endinterface


