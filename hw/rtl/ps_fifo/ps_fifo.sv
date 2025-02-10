`timescale 1 ns/1 ps

module ps_fifo #(
  parameter DEPTH = 128,
  parameter READ_LATENCY = 1
) (
  input wire clk,
  input wire rst_n,

  input wire shift_out,
  output logic valid,

  ps_if.slave in,
  ps_if.master out

);

  localparam TOTAL_WDITH = 2 * (in.ADDR_WIDTH) + in.DATA_WIDTH + 34;

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

    .din        ({in.node_addr, in.waddr, in.wdata, in.wvalid, in.raddr, in.arvalid}),
    .shift_in   (in_valid),

    .shift_out  (shift_out),
    .dout       ({out.node_addr, out.waddr, out.wdata, out.wvalid, out.raddr, out.arvalid}),
    .valid      (valid),

    .full       (),
    .empty      (),
    .overflow   (),
    .underflow  () 

  );

  assign in.wready      = out.wready; // Ready to Write 
  assign in.wresp       = out.wresp;  // done writing (optional)


  assign in.rdata       = out.rdata;    // data read
  assign in.rvalid      = out.rvalid;   // valid data out
  
endmodule
