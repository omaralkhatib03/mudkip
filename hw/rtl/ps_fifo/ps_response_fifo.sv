`timescale 1 ns/1 ps

module ps_response_fifo #(
  parameter DEPTH = 128,
  parameter READ_LATENCY = 1
) (
  input wire clk,
  input wire rst_n,

  input wire shift_out,
  output logic valid,

  ps_if.master in,
  ps_if.slave out

);

  localparam TOTAL_WDITH = 3 + in.DATA_WIDTH;

  logic [TOTAL_WDITH-1:0] din;
  logic in_valid;

  assign in_valid = in.arvalid || in.wvalid;

  basic_sync_fifo #(
    .DATA_WIDTH(TOTAL_WDITH),
    .DEPTH(DEPTH),
    .READ_LATENCY(READ_LATENCY)
  ) ps_fifo_I (
    .clk        (clk),
    .rst_n      (rst_n),

    .din        ({in.wresp, in.wready, in.rdata, in.rvalid}),
    .shift_in   (in_valid),

    .shift_out  (shift_out),
    .dout       ({out.wresp, out.wready, out.rdata, out.rvalid}),
    .valid      (valid),

    .full       (),
    .empty      (),
    .overflow   (),
    .underflow  () 

  );

endmodule
