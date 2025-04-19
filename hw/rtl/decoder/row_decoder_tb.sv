`timescale 1ns/1ps

module row_decoder_tb #(
    parameter OFFSET       /*verilator public */ = 0,
    parameter DATA_WIDTH   /*verilator public */ = 32,
    parameter IN_PARALLEL  /*verilator public */ = 16,
    parameter OUT_PARALLEL /*verilator public */ = 8
)(
    input wire clk,
    input wire rst_n,

    input  wire [DATA_WIDTH-1:0] r_beg_data   [IN_PARALLEL-1:0],
    input  wire                  r_beg_valid,
    input  wire                  r_beg_last,
    input  wire [IN_PARALLEL-1:0] r_beg_bytemask,
    output logic                 r_beg_ready,

    output logic [DATA_WIDTH-1:0] row_ids_data [OUT_PARALLEL-1:0],
    output logic                  row_ids_valid,
    output logic                  row_ids_last,
    output logic [OUT_PARALLEL-1:0] row_ids_bytemask,
    input  wire                   row_ids_ready
);

    // Instantiate the stream interfaces
    axi_stream_if #(
        .DATA_WIDTH(DATA_WIDTH),
        .PARALLELISM(IN_PARALLEL)
    ) r_beg_if();

    axi_stream_if #(
        .DATA_WIDTH(DATA_WIDTH),
        .PARALLELISM(OUT_PARALLEL)
    ) row_ids_if();

    generate
        genvar i;
        for (i = 0; i < IN_PARALLEL; i = i + 1) begin
            assign r_beg_if.data[i] = r_beg_data[i];
        end
    endgenerate

    assign r_beg_if.valid     = r_beg_valid;
    assign r_beg_if.last      = r_beg_last;
    assign r_beg_if.mask       = r_beg_bytemask;
    assign r_beg_ready        = r_beg_if.ready;

    generate
        for (i = 0; i < OUT_PARALLEL; i = i + 1) begin
            assign row_ids_data[i] = row_ids_if.data[i];
        end
    endgenerate

    assign row_ids_valid    = row_ids_if.valid;
    assign row_ids_last     = row_ids_if.last;
    assign row_ids_bytemask = row_ids_if.mask;
    assign row_ids_if.ready = row_ids_ready;

    row_decoder #(
        .OFFSET(OFFSET)
    ) dut_I (
        .clk(clk),
        .rst_n(rst_n),
        .r_beg(r_beg_if),
        .row_ids(row_ids_if)
    );

endmodule
