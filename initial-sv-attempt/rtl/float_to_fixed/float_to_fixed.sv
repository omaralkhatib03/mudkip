
`timescale 1ns/1ps


module float_to_fixed #(
    parameter FLOAT         /* verilator public */ =  1, // if 0 then double else 1
    parameter DATA_WIDTH    /* verilator public */ =  20, // [4, 64]
    parameter FRAC_WIDTH    /* verilator public */ =  12,
    parameter PARALLELISM   /* verilator public */ =  4,
    parameter DELAY         /* verilator public */ =  2,
    localparam IN_WDITH     /* verilator public */ =  FLOAT ? 32 : 64
) (
    input wire                      clk,
    // verilator lint_off unused
    input wire                      rst_n,
    // verilator lint_on unused

    input wire                      in_valid,
    output logic                    in_ready,
    input wire                      in_tlast,
    input wire [PARALLELISM-1:0]    in_mask,

    input wire [IN_WDITH-1:0]       a[PARALLELISM-1:0],

    output logic [DATA_WIDTH-1:0]   out[PARALLELISM-1:0],
    output logic                    valid,
    input logic                     ready,
    output logic                    tlast,
    output logic [PARALLELISM-1:0]  tkeep

);

    `ifdef VERILATOR

        import "DPI-C" function byte dpi_float_to_fixed(input int data_width, input int frac_width, input longint a, output bit [DATA_WIDTH-1:0] res);

        logic [DATA_WIDTH-1:0]                  out_inter[PARALLELISM-1:0];

        // verilator lint_off unused
        byte                                    ret_value[PARALLELISM-1:0];
        // verilator lint_on unused

        logic [PARALLELISM-1:0][DATA_WIDTH-1:0] out_flat_b;
        logic [PARALLELISM-1:0][DATA_WIDTH-1:0] out_flat_r;

        always_comb
        begin
            for (int i = 0; i < PARALLELISM; i++)
            begin
                // verilator lint_off width
                ret_value[i]    = dpi_float_to_fixed(DATA_WIDTH, FRAC_WIDTH, a[i], out_inter[i]);
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

