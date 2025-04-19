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

            logic fifo_full;

            /* verilator lint_off PINMISSING */ // (Over/Under)flow Pins
            basic_sync_fifo #(
                .DATA_WIDTH     (out.IN_WIDTH + out.ID_WIDTH),
                .DEPTH          (FIFO_DEPTH),
                .READ_LATENCY   (DELAY)
            ) input_fifo_I      (
                .clk            (clk),
                .rst_n          (rst_n),
                .din            ({out.ID_WIDTH'(in.id), out.IN_WIDTH'(in.val)}),
                .shift_in       (in.valid),
                .shift_out      (out.ready),
                .valid          (out.valid),
                .dout           ({out.id, out.val}),
                .full           (fifo_full)
            );
            /* verilator lint_on PINMISSING */ // (Over/Under)flow Pins

            assign in.ready     = !fifo_full;
        end
    endgenerate

endmodule
