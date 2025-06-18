`timescale 1ns/1ps

module ctrl_data_fifo #(
  parameter DATA_WIDTH        = 32,
  parameter CTRL_WIDTH        = 32,
  parameter DEPTH             = 16,
  parameter READ_LATENCY      = 1
) (
  input wire                                    clk,
  input wire                                    rst_n,

  input wire [DATA_WIDTH-1:0]                   din_data,
  input wire                                    data_valid,
  output                                        data_ready,

  input wire [CTRL_WIDTH-1:0]                   ctrl_data,
  input wire                                    ctrl_valid,
  output wire                                   ctrl_ready,

  output wire [DATA_WIDTH+CTRL_WIDTH-1:0]       dout,
  output wire                                   valid,
  input wire                                    shift_out,

  output wire                                  empty,

  output wire                                  ctrl_overflow,
  output wire                                  ctrl_underflow,

  output wire                                  data_overflow,
  output wire                                  data_underflow

);

  logic ctrl_empty;
  logic data_empty;

  logic data_full;
  logic ctrl_full;

  logic data_fifo_valid;
  logic ctrl_fifo_valid;

  logic [CTRL_WIDTH-1:0] ctrl_out;
  logic [DATA_WIDTH-1:0] data_out;

  basic_sync_fifo #(
    .DATA_WIDTH(DATA_WIDTH),
    .DEPTH(DEPTH),
    .READ_LATENCY(READ_LATENCY)
  ) data_fifo_i (
    .clk(clk),
    .rst_n(rst_n),

    .din(din_data),
    .shift_in(data_valid),

    .shift_out(shift_out),
    .dout(data_out),
    .valid(data_fifo_valid),

    .full(data_full),
    .empty(data_empty),
    .overflow(data_overflow),
    .underflow(data_underflow)

  );

  basic_sync_fifo #(
    .DATA_WIDTH(CTRL_WIDTH),
    .DEPTH(DEPTH),
    .READ_LATENCY(READ_LATENCY)
  ) ctrl_fifo_i (
    .clk(clk),
    .rst_n(rst_n),

    .din(ctrl_data),
    .shift_in(ctrl_valid),

    .shift_out(shift_out),
    .dout(ctrl_out),
    .valid(ctrl_fifo_valid),

    .full(ctrl_full),
    .empty(ctrl_empty),
    .overflow(ctrl_overflow),
    .underflow(ctrl_underflow)
  );

  assign valid      = data_fifo_valid && ctrl_fifo_valid;
  assign dout       = {ctrl_out, data_out};
  assign data_ready = !data_full;
  assign ctrl_ready = !ctrl_full;
  assign empty      = ctrl_empty && data_empty;

endmodule
