`timescale 1ns / 1ps

module spmv_network_op_tb  #(
    parameter IN_WIDTH     /*verilator public */ = 59,
    parameter ID_WIDTH     /*verilator public */ = 13,
    parameter LOCATION     /*verilator public */ = 29,
    parameter PARALLELISM  /*verilator public */ = 38,
    parameter OUT_WIDTH    /*verilator public */ = IN_WIDTH + 1  // at most one addition but we allow overriding
) (
    // verilator lint_off unused
    input wire                      clk,
    input wire                      rst_n,
    // verilator lint_on unused

    input wire [ID_WIDTH-1:0]       in_a_id,
    input wire [IN_WIDTH-1:0]       in_a_val,
    input wire [ID_WIDTH-1:0]       in_b_id,
    input wire [IN_WIDTH-1:0]       in_b_val,
    input wire                      in_valid,
    output logic                    in_ready,

    output logic [ID_WIDTH-1:0]     a_id,
    output logic [OUT_WIDTH-1:0]    a_val,
    output logic                    a_valid,

    output logic [ID_WIDTH-1:0]     b_id,
    output logic [OUT_WIDTH-1:0]    b_val,
    output logic                    b_valid,

    input wire                      ready
);

    network_if #(
        .IN_WIDTH(IN_WIDTH),
        .ID_WIDTH(ID_WIDTH)
    ) net_if_in();

    network_if #(
        .IN_WIDTH(OUT_WIDTH),
        .ID_WIDTH(ID_WIDTH)
    ) net_if_out();

    always_comb
    begin
        net_if_in.a.id      = in_a_id;
        net_if_in.a.val     = in_a_val;
        net_if_in.a.valid   = in_valid;

        net_if_in.b.id      = in_b_id;
        net_if_in.b.val     = in_b_val;
        net_if_in.b.valid   = in_valid;

        in_ready            = net_if_in.ready;

        a_id                = net_if_out.a.id;
        a_val               = net_if_out.a.val;
        a_valid             = net_if_out.a.valid;

        b_id                = net_if_out.b.id;
        b_val               = net_if_out.b.val;
        b_valid             = net_if_out.b.valid;

        net_if_out.ready    = ready;
    end

    spmv_network_op #(
        .LOCATION(LOCATION),
        .PARALLELISM(PARALLELISM)
    ) dut_I (
        .in(net_if_in),
        .out(net_if_out)
    );

endmodule

