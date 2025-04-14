`timescale 1ns / 1ps

module spmv_network_tb
#(
    parameter NETWORK_WIDTH /*verilator public */ = 32, // Cant test super large values
    parameter IN_WIDTH      /*verilator public */ = 59,
    parameter ID_WIDTH      /*verilator public */ = 5,
    parameter OUT_WIDTH     /*verilator public */ = IN_WIDTH + $clog2(NETWORK_WIDTH),
    parameter FIFO_DEPTH                          = 16
) (
    input  wire                         clk,

    // verilator lint_off unused
    input  wire                         rst_n,
    // verilator lint_on unused

    input  wire [NETWORK_WIDTH-1:0]                 in_valid,
    input  wire [ID_WIDTH-1:0]                      in_id[NETWORK_WIDTH-1:0],
    input  wire [IN_WIDTH-1:0]                      in_val[NETWORK_WIDTH-1:0],
    output logic [NETWORK_WIDTH-1:0]                in_ready,

    output logic [NETWORK_WIDTH-1:0]                out_valid,
    output logic [ID_WIDTH-1:0]                     out_id[NETWORK_WIDTH-1:0],
    output logic [OUT_WIDTH-1:0]                    out_val[NETWORK_WIDTH-1:0],
    input  wire [NETWORK_WIDTH-1:0]                 out_ready
);

    network_if #(.IN_WIDTH(IN_WIDTH), .ID_WIDTH(ID_WIDTH)) net_if_in [NETWORK_WIDTH-1:0] ();
    network_if #(.IN_WIDTH(OUT_WIDTH), .ID_WIDTH(ID_WIDTH)) net_if_out [NETWORK_WIDTH-1:0] ();

    genvar i;

    generate
        for (i = 0; i < NETWORK_WIDTH; i++) begin : bind_io
            always_comb begin
                net_if_in[i].id                     = in_id[i];
                net_if_in[i].val                    = in_val[i];
                net_if_in[i].valid                  = in_valid[i];
                in_ready[i]                         = net_if_in[i].ready;

                out_id[i]                           = net_if_out[i].id;
                out_val[i]                          = net_if_out[i].val;
                out_valid[i]                        = net_if_out[i].valid;

                net_if_out[i].ready                 = out_ready[i];
            end
        end
    endgenerate

    spmv_reduction_network #(
        .NETWORK_WIDTH  (NETWORK_WIDTH),
        .FIFO_DEPTH     (FIFO_DEPTH)
    ) dut_I (
        .clk(clk),
        .rst_n(rst_n),
        .in(net_if_in),
        .out(net_if_out)
    );

endmodule

