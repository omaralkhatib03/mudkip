`timescale 1ns/1ps

// TODO: Add fifos

module spmv_kernel_top #(
    parameter   VECTOR_LENGTH  /*verilator public*/ = 32,
    parameter   DATA_WIDTH     /*verilator public*/ = 32,
    parameter   PARALLELISM    /*verilator public*/ = 4,
    parameter   RELEASE_MODE   /*verilator public*/ = 0,
    parameter   FLOAT                               = 0,
    parameter   E_WIDTH                             = 8,
    parameter   FRAC_WIDTH                          = 23,
    parameter   NUMBER_OF_RAMS                      = 4,
    parameter   RAM_FIFO_DEPTH                      = 4,
    // verilator lint_off UNUSEDPARAM
    localparam  ADDR_WIDTH                          = $clog2(VECTOR_LENGTH)
    // verilator lint_on UNUSEDPARAM
) (
    input wire      clk,
    input wire      rst_n,

    input wire      ping,
    input wire      en,
    output logic    done,

    axi_stream_if.slave val,
    axi_stream_if.slave r_beg,
    axi_stream_if.slave c_idx,

    // Dangerous Interface, Used for testing only. Unused in Release Mode
    // verilator lint_off UNUSED
    input wire          cfg_en,
    vector_ram_if.slave cfg,
    // verilator lint_on UNUSED

    vector_ram_if.slave rom_x,
    vector_ram_if.slave rom_x_n
);

    read_matrix #(
        .EL_PER_DDR(PARALLELISM),
        .OFFSET(0)
    ) (
    .clk        (),
    .rst_n      (),

    .r_beg_ddr  (),
    .c_idx_ddr  (),
    .c_val_ddr  (),

    .c_idx      (),
    .val        (),
    .r_idx      (),
    .valid      (),
    .ready      (),
    .mask       (),
    .last       ()

)    


endmodule

