`timescale 1ns/1ps

// TODO: Add PARALLELISM
module fl_vadd # (
  parameter PARALLELISM = 1,
  parameter DATA_WIDTH = 32 
) ( 
  input wire                          clk,
  input wire                          rst_n,

  input wire [DATA_WIDTH-1:0]         in_x_data, 
  input wire                          in_x_valid, 
  output logic                        x_ready,
  input wire                         x_end,

  input wire [DATA_WIDTH-1:0]         in_y_data, 
  input wire                          in_y_valid, 
  output logic                        y_ready,
  input wire                         y_end,
  
  output logic [DATA_WIDTH-1:0]       out_data, 
  output logic                        out_valid, 
  input logic                         out_ready,
  output logic                        out_end
);
  
  logic [DATA_WIDTH-1:0] x_data;
  logic x_eop;
  logic x_valid;

  logic [DATA_WIDTH-1:0] y_data;

  logic y_eop;
  logic y_valid;

  logic out_fifo_full;
  logic empty;

  logic x_empty;
  logic y_empty;
  logic shift_out;

  logic [DATA_WIDTH-1:0] fifo_in;

  logic shift_in;
  logic s_axis_y_tready;
  logic s_axis_x_tready;
  logic out_eop;
  
  assign shift_out  = !x_empty && !y_empty && s_axis_x_tready && s_axis_y_tready;
  
  ctrl_data_fifo #(
    .DATA_WIDTH     (DATA_WIDTH),
    .CTRL_WIDTH     (1),
    .DEPTH          (256),
    .READ_LATENCY   (1)
  ) x_fifo (
    .clk            (clk),
    .rst_n          (rst_n),
    .din_data       (in_x_data),
    .data_valid     (in_x_valid),
    .data_ready     (x_ready),

    .ctrl_data      (x_end),
    .ctrl_valid     (x_valid),
    .ctrl_ready     (),

    .dout           ({x_eop, x_data}),
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
    .CTRL_WIDTH     (1),
    .DEPTH          (256),
    .READ_LATENCY   (1)
  ) y_fifo (
    .clk            (clk),
    .rst_n          (rst_n),
    .din_data       (in_y_data),
    .data_valid     (in_y_valid),
    .data_ready     (y_ready),

    .ctrl_data      (y_end),
    .ctrl_valid     (y_valid),
    .ctrl_ready     (),

    .dout           ({y_eop, y_data}),
    .valid          (y_valid),
    .ready          (out_fifo_full),

    .shift_out      (shift_out),
    .empty          (y_empty),

    .ctrl_overflow  (),
    .ctrl_underflow (),

    .data_overflow  (),
    .data_underflow ()
  );
 
  fp_add_s fp_add_s_I (
    .aclk(clk),                                  // input wire aclk
    .s_axis_a_tvalid      (x_valid),            // input wire s_axis_a_tvalid
    .s_axis_a_tready      (s_axis_x_tready),            // output wire s_axis_a_tready
    .s_axis_a_tdata       (x_data),              // input wire [31 : 0] s_axis_a_tdata
    .s_axis_a_tlast       (x_eop),              // input wire s_axis_a_tlast

    .s_axis_b_tvalid      (y_valid),            // input wire s_axis_b_tvalid
    .s_axis_b_tready      (s_axis_y_tready),            // output wire s_axis_b_tready
    .s_axis_b_tdata       (y_data),              // input wire [31 : 0] s_axis_b_tdata
    .s_axis_b_tlast       (y_eop),              // input wire s_axis_b_tlast

    .m_axis_result_tvalid (shift_in),  // output wire m_axis_result_tvalid
    .m_axis_result_tready (!out_fifo_full),  // input wire m_axis_result_tready
    .m_axis_result_tdata  (out_data),    // output wire [31 : 0] m_axis_result_tdata
    .m_axis_result_tlast  (out_eop)    // output wire m_axis_result_tlast
  ); 

  basic_sync_fifo #(
    .DATA_WIDTH   (DATA_WIDTH + 1),
    .DEPTH        (256),
    .READ_LATENCY (1)
  ) out_fifo_I (
    .clk          (clk),
    .rst_n        (rst_n),

    .din          ({out_eop, fifo_in}),
    .shift_in     (shift_in),

    .shift_out    (!empty && out_ready),
    .valid        (out_valid),
    .dout         ({out_end, out_data}),
    .empty        (empty),
    .full         (out_fifo_full),

    .underflow    (),
    .overflow     ()
  );

endmodule
