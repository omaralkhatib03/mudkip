`timescale 1ns/1ps

import spmv_pkg::*;
import "DPI-C" function byte dpi_fmul(input int exp_prec, input int mant_prec, input long a, input long b,  input long result);

module product #(
    parameter FLOAT         = 1,
    parameter DATA_WIDTH    = 32, // [4, 64]
    parameter E_WIDTH       = 8,
    parameter FRAC_WIDTH    = 24, 
    parameter PARALLELISM   = 4,
    parameter DELAY         = 2
) (
    input wire                      clk,
    input wire                      rst_n,

    input wire                      in_valid,
    output logic                    in_ready,

    input wire [DATA_WIDTH-1:0]     a[PARALLELISM-1:0],
    input wire [DATA_WIDTH-1:0]     b[PARALLELISM-1:0],

    output logic [DATA_WIDTH-1:0]   out[PARALLELISM-1:0],
    output logic                    valid,
    input logic                     ready

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
                out_flat_b[i]   = out_inter[i];
                ret_value[i]    = dpi_fmul(E_WIDTH, FRAC_WIDTH, $bits(long)'(a[i]), $bits(long)'(b[i]), $bits(long)'(out_inter[i]));
            end
        end
        
        delay #(
            .DATAWIDTH(PARALLELISM*DATA_WIDTH + 1),
            .DELAY(DELAY)
        ) delay_I (
            .clk(clk),
            .in({out_flat_b, in_valid}),
            .out({out_flat_r, valid})
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

