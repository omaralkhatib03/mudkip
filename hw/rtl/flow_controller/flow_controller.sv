`timescale 1ns/1ps


// Shows live which ports are missing
module flow_controller #(
    parameter DATA_WIDTH    = 32,
    parameter INPUT_PORTS   = 3,
    parameter OUTPUT_PORTS  = 3,
    localparam PORT_POINTER = $clog2(INPUT_PORTS)
) (
    input wire                      clk,
    input wire                      rst_n,

    input wire [DATA_WIDTH-1:0]     din[INPUT_PORTS-1:0],
    input wire [PORT_POINTER-1:0]   addr[INPUT_PORTS-1:0],
    input wire [INPUT_PORTS-1:0]    valid_in,
    output logic                    ready,

    output logic [DATA_WIDTH-1:0]   dout[OUTPUT_PORTS-1:0],
    output logic [OUTPUT_PORTS-1:0] currently_valid,
    output logic                    valid,
    input wire                      front_ready
);

endmodule
