`timescale 1ns/1ps

import spmv_pkg::*;
import "DPI-C" function byte dpi_fmul(input int exp_prec, input int mant_prec, input longint a, input longint b, output longint result);

module product #(
    parameter FLOAT         /* verilator public */ =  1,
    parameter DATA_WIDTH    /* verilator public */ =  16, // [4, 64]
    parameter E_WIDTH       /* verilator public */ =  5,
    parameter FRAC_WIDTH    /* verilator public */ =  11, 
    parameter PARALLELISM   /* verilator public */ =  4,
    parameter DELAY         /* verilator public */ =  2
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
        logic                                   valid_inter;

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
        
        // always_ff @(posedge clk) 
        // begin 
        //     $display("A: %h", a[0], " A_P: %h", $bits(long)'(a[0])); 
        // end

    `else // !VERILATOR
        
    `endif // VERILATOR

endmodule

