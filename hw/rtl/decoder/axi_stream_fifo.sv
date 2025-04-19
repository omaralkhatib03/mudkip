`timescale 1ns/1ps

module axi_stream_fifo #(
    parameter FIFO_DEPTH = 4,
    parameter PIPELINE_ONLY = 0,
    parameter SKID          = 0
) (
    input wire              clk,
    input wire              rst_n,
    axi_stream_if.slave     in,
    axi_stream_if.master    out
);


    generate
        if (PIPELINE_ONLY)
        begin : pipeline_gen

            pipeline #(
                .DATA_WIDTH(in.TOTAL_WIDTH + 1 + in.PARALLELISM),
                .PIPE_LINE(SKID)
            ) pipeline_I (
                .clk        (clk),
                .rst_n      (rst_n),
                .in_data    ({in.data, in.last, in.mask}),
                .in_valid   (in.valid),
                .in_ready   (in.ready),
                .out_data   ({out.data, out.last, out.mask}),
                .out_valid  (out.valid),
                .out_ready  (out.ready)
            );

        end
        else
        begin : fifo_gen

            logic fifo_full;

            /* verilator lint_off PINMISSING */ // (Over/Under)flow Pins
            basic_sync_fifo #(
                .DATA_WIDTH(in.TOTAL_WIDTH + 1 + in.PARALLELISM),
                .DEPTH(FIFO_DEPTH)
            ) input_fifo_I (
                .clk        (clk),
                .rst_n      (rst_n),
                .din        ({in.data, in.last, in.mask}),
                .shift_in   (in.valid),
                .shift_out  (out.ready),
                .dout       ({out.data, out.last, out.mask}),
                .valid      (out.valid),
                .full       (fifo_full)
            );
            /* verilator lint_off PINMISSING */ // (Over/Under)flow Pins

            assign in.ready = !fifo_full;
        end
    endgenerate

endmodule
