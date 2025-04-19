`timescale 1ns/1ps

import "DPI-C" function byte dpi_fmul(input int exp_prec, input int mant_prec, input longint a, input longint b, output longint result);

module fproduct #(
    parameter FLOAT         /* verilator public */ =  1,
    parameter DATA_WIDTH    /* verilator public */ =  20, // [4, 64]
    parameter E_WIDTH       /* verilator public */ =  8,
    parameter FRAC_WIDTH    /* verilator public */ =  12,
    parameter PARALLELISM   /* verilator public */ =  4,
    parameter DELAY         /* verilator public */ =  2
) (
    input wire                      clk,
    // verilator lint_off unused 
    input wire                      rst_n,
    // verilator lint_on unused

    input wire                      in_valid,
    output logic                    in_ready,
    input wire                      in_tlast,
    input wire [PARALLELISM-1:0]    in_mask,

    input wire [DATA_WIDTH-1:0]     a[PARALLELISM-1:0],
    input wire [DATA_WIDTH-1:0]     b[PARALLELISM-1:0],

    output logic [DATA_WIDTH-1:0]   out[PARALLELISM-1:0],
    output logic                    valid,
    input logic                     ready,
    output logic                    tlast,
    output logic [PARALLELISM-1:0]  tkeep

);

    `ifdef VERILATOR

        logic [DATA_WIDTH-1:0]                  out_inter[PARALLELISM-1:0];
        byte                                    ret_value[PARALLELISM-1:0];

        logic [PARALLELISM-1:0][DATA_WIDTH-1:0] out_flat_b;
        logic [PARALLELISM-1:0][DATA_WIDTH-1:0] out_flat_r;

        always_comb
        begin
            for (int i = 0; i < PARALLELISM; i++)
            begin
                // verilator lint_off width
                ret_value[i]    = dpi_fmul(E_WIDTH, FRAC_WIDTH, a[i], b[i], out_inter[i]);
                // verilator lint_on width
                out_flat_b[i]   = out_inter[i];
            end
        end

        delay #(
            .DATAWIDTH(PARALLELISM*DATA_WIDTH + 2 + PARALLELISM),
            .DELAY(DELAY)
        ) delay_I (
            .clk(clk),
            .in({out_flat_b, in_valid, in_tlast, in_mask}),
            .out({out_flat_r, valid, tlast, tkeep})
        );

        always_comb
        begin
            for (int i = 0; i < PARALLELISM; i++)
            begin
                out[i] = out_flat_r[i];
            end
        end

        assign in_ready = ready;

    `else // !VERILATOR

    `endif // VERILATOR

endmodule

