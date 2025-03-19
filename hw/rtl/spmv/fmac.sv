`timescale 1ns/1ps

// TODO: Fuse Multiply and ADD to reduce error, Currently just MAC

module fmac #(
    parameter FLOAT             = 1,
    parameter EXP_WIDTH         = 8,
    parameter MANTISSA_WIDTH    = 23,
    parameter DATA_WIDTH        = 32,
    localparam ACC_WIDTH         = 2*DATA_WIDTH
) (
    input wire                      clk,
    input wire                      rst_n,

    input wire [2*DATA_WIDTH-1:0]   val,            // Addition is double
    input wire [DATA_WIDTH-1:0]     multiplicand,   // is float
    input wire                      in_valid,
    output logic                    in_ready,
    input wire                      reset,  // reset acc

    output logic [2*DATA_WIDTH-1:0] acc,
    output logic                    valid,  // Is current Accumulation valid
    input wire                      ready,
    output logic                    done    // Response to in_valid or reset
);


endmodule;
