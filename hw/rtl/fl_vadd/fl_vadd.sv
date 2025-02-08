`timescale 1ns/1ps

// TODO: Add PARALLELISM
module fl_vadd # (
  parameter PARALLELISM = 1,
  parameter DATA_WIDTH = 32 
) ( 
  input wire                          clk,
  input wire                          rst_n,

  input wire                          x_start,
  input wire [DATA_WIDTH-1:0]         in_x_data, 
  input wire                          in_x_valid, 
  output logic                        x_ready,
  output wire                         x_end,

  input wire                          y_start,
  input wire [DATA_WIDTH-1:0]         in_y_data, 
  input wire                          in_y_valid, 
  output logic                        y_ready,
  output wire                         y_end,
  
  output logic                        out_start,
  output logic [DATA_WIDTH-1:0]       out_data, 
  output logic                        out_valid, 
  input logic                         out_ready,
  output logic                        out_end
);
  
  logic [DATA_WIDTH-1:0] x_data;
  logic x_sop;
  logic x_eop;
  logic x_valid;

  logic [DATA_WIDTH-1:0] y_data;

  logic y_sop;
  logic y_eop;
  logic y_valid;

  logic out_fifo_full;
  logic out_sop;
  logic out_eop;
  logic empty;

  logic x_empty;
  logic y_empty;
  logic shift_out;

  logic [DATA_WIDTH-1:0] fifo_in;
  
  assign shift_out  = !x_empty && !y_empty && !out_fifo_full;
  assign out_eop    = x_end && y_end;
  assign out_sop    = x_sop && y_eop;
  
  ctrl_data_fifo #(
    .DATA_WIDTH     (DATA_WIDTH),
    .CTRL_WIDTH     (2),
    .DEPTH          (256),
    .READ_LATENCY   (1)
  ) x_fifo (
    .clk            (clk),
    .rst_n          (rst_n),
    .din_data       (in_x_data),
    .data_valid     (in_x_valid),
    .data_ready     (x_ready),

    .ctrl_data      ({x_start, x_end}),
    .ctrl_valid     (x_valid),
    .ctrl_ready     (),

    .dout           ({x_sop, x_eop, x_data}),
    .valid          (x_valid),
    .ready          (out_fifo_full),

    .shift_out      (shift_out),
    .empty          (x_empty),

    .ctrl_overflow  (),
    .ctrl_underflow (),

    .data_overflow  (),
    .data_underflow ()
  );

  ctrl_data_fifo #(
    .DATA_WIDTH     (DATA_WIDTH),
    .CTRL_WIDTH     (2),
    .DEPTH          (256),
    .READ_LATENCY   (1)
  ) y_fifo (
    .clk            (clk),
    .rst_n          (rst_n),
    .din_data       (in_y_data),
    .data_valid     (in_y_valid),
    .data_ready     (y_ready),

    .ctrl_data      ({y_start, y_end}),
    .ctrl_valid     (y_valid),
    .ctrl_ready     (),

    .dout           ({y_sop, y_eop, x_data}),
    .valid          (y_valid),
    .ready          (out_fifo_full),

    .shift_out      (shift_out),
    .empty          (y_empty),

    .ctrl_overflow  (),
    .ctrl_underflow (),

    .data_overflow  (),
    .data_underflow ()
  );
 

  fadd fadd_I ( 
    .din1(x_data),
    .din2(y_data),
    .dout(fifo_in)
  );
  
  basic_sync_fifo #(
    .DATA_WIDTH   (DATA_WIDTH + 2),
    .DEPTH        (256),
    .READ_LATENCY (1)
  ) out_fifo_I (
    .clk          (clk),
    .rst_n        (rst_n),

    .din          ({out_sop, out_eop, fifo_in}),
    .shift_in     (shift_out),

    .shift_out    (!empty && out_ready),
    .valid        (out_valid),
    .dout         ({out_start, out_end, out_data}),
    .empty        (empty),
    .full         (out_fifo_full),

    .underflow    (),
    .overflow     ()
  );

  assign x_ready = '1;
  assign y_ready = '1;

endmodule
