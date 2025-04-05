`timescale 1ns/1ps

module product #(
    parameter FLOAT         = 1,
    parameter DATA_WIDTH    = 32,
    parameter E_WIDTH       = 8,
    parameter FRAC_WIDTH    = 23,
    parameter PARALLELISM   = 4,
    parameter DELAY         = 2,
    localparam OUT_WIDTH    = FLOAT ? DATA_WIDTH : 2*DATA_WIDTH
) (
    input wire                      clk,

    input wire                      in_valid,
    output logic                    in_ready,

    input wire [DATA_WIDTH-1:0]     a[PARALLELISM-1:0],
    input wire [DATA_WIDTH-1:0]     b[PARALLELISM-1:0],

    output logic [OUT_WIDTH-1:0]    out[PARALLELISM-1:0],
    output logic                    valid,
    input logic                     ready

);

    logic [OUT_WIDTH-1:0]                   out_inter[PARALLELISM-1:0];

    logic [PARALLELISM-1:0][OUT_WIDTH-1:0]  out_flat_b;
    logic [PARALLELISM-1:0][OUT_WIDTH-1:0]  out_flat_r;

    always_comb
    begin
        for (int i = 0; i < PARALLELISM; i++)
        begin
            out_flat_b[i] = out_inter[i];
        end
    end

    generate
      for (genvar i = 0; i < PARALLELISM; i++)
      begin : fp_mult_gen

        `ifdef  VERILATOR

            /* verilator lint_off PINMISSING */
            fp_mult #(
                .BIT_SIZE(DATA_WIDTH),
                .EXPONENT(E_WIDTH),
                .MANITSSA(FRAC_WIDTH)
            ) fp_mult_I (
                .a_operand(a[i]),
                .b_operand(b[i]),
                .result(out_inter[i])
            );
            /* verilator lint_on PINMISSING */

        `endif
        `ifndef VERILATOR // I.e HW
            // TODO: ADD IP INST, Manually for now
        `endif

      end

    endgenerate

    `ifdef VERILATOR

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

    `else
        // TODO: Add IP Instance
    `endif

    assign in_ready = ready;

endmodule
