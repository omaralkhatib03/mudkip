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
    ) net_if_in_a();

    network_if #(
        .IN_WIDTH(IN_WIDTH),
        .ID_WIDTH(ID_WIDTH)
    ) net_if_in_b();

    network_if #(
        .IN_WIDTH(OUT_WIDTH),
        .ID_WIDTH(ID_WIDTH)
    ) net_if_out_a();

    network_if #(
        .IN_WIDTH(OUT_WIDTH),
        .ID_WIDTH(ID_WIDTH)
    ) net_if_out_b();

    always_comb
    begin
        net_if_in_a.id      = in_a_id;
        net_if_in_a.val     = in_a_val;
        net_if_in_a.valid   = in_valid;

        net_if_in_b.id      = in_b_id;
        net_if_in_b.val     = in_b_val;
        net_if_in_b.valid   = in_valid;

        in_ready            = net_if_in_a.ready && net_if_in_b.ready;

        a_id                = net_if_out_a.id;
        a_val               = net_if_out_a.val;
        a_valid             = net_if_out_a.valid;

        b_id                = net_if_out_b.id;
        b_val               = net_if_out_b.val;
        b_valid             = net_if_out_b.valid;

        net_if_out_a.ready  = ready;
        net_if_out_b.ready  = ready;
    end

    spmv_network_op #(
        .LOCATION(LOCATION),
        .PARALLELISM(PARALLELISM)
    ) dut_I (
        .in_a(net_if_in_a),
        .in_b(net_if_in_b),
        .out_a(net_if_out_a),
        .out_b(net_if_out_b)
    );

endmodule

