`timescale 1ns / 1ps

module network_if_copier #(
    parameter DELAY         = 1,
    parameter FIFO_DEPTH    = 2
) (
    input wire clk,
    input wire rst_n,
    network_if.slave  in,
    network_if.master out
);

    generate
        if (DELAY == 0)
        begin : comb_gen

            always_comb
            begin
                out.val      = out.IN_WIDTH'(in.val);
                out.id       = out.ID_WIDTH'(in.id);
                out.valid    = in.valid;
                in.ready     = out.ready;
            end

        end
        else
        begin : delay_gen

            pipeline #(
                .DATA_WIDTH(out.IN_WIDTH + out.ID_WIDTH),
                .PIPE_LINE(1)
            ) pipeline_0_I (
                .clk        (clk),
                .rst_n      (rst_n),
                .in_data    ({out.ID_WIDTH'(in.id), out.IN_WIDTH'(in.val)}),
                .in_valid   (in.valid),
                .in_ready   (in.ready),
                .out_data   ({out.id, out.val}),
                .out_valid  (out.valid),
                .out_ready  (out.ready)
            );

        end
    endgenerate

endmodule
