`timescale 1ns / 1ps


module delay #(
  parameter DATAWIDTH = 32,
  parameter DELAY = 0
  ) (
    /* verilator lint_off UNUSED */
    input wire clk,
    /* verilator lint_on UNUSED */
    input wire [DATAWIDTH-1:0] in,
    output wire [DATAWIDTH-1:0] out
  );


  generate
    if (DELAY == 0)
    begin : delay_0
      assign out = in;
    end
    else
    begin : delay_non_zero

      logic [DATAWIDTH-1:0] regs[DELAY-1:0];

      assign regs[0] = in;

      always_ff @(posedge clk)
      begin : delays
        for (int i = 1; i < DELAY; i++)
        begin
          regs[i] <= regs[i - 1];
        end
      end

      assign out = regs[DELAY - 1];

    end
  endgenerate



endmodule
