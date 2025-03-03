`timescale 1ns/1ps

module fl_vadd # (
  parameter PARALLELISM = 1,
  parameter DATA_WIDTH = 32 
) ( 
  input wire                          clk,
  input wire                          rst_n,

  (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_x TDATA" *)
  input wire [DATA_WIDTH-1:0]         in_x_data, 

  (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_x TVALID" *)
  input wire                          in_x_valid, 

  (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_x TREADY" *)
  output wire                        x_ready,

  (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_x TLAST" *)
  input wire                          x_end,

  (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_y TDATA" *)
  input wire [DATA_WIDTH-1:0]         in_y_data, 

  (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_y TVALID" *)
  input wire                          in_y_valid, 

  (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_y TREADY" *)
  output wire                        y_ready,

  (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_y TLAST" *)
  input wire                          y_end,
  
  (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_out TDATA" *)
  output wire [DATA_WIDTH-1:0]       out_data, 

  (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_out TVALID" *)
  output wire                        out_valid, 

  (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_out TREADY" *)
  input wire                         out_ready,

  (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_out TLAST" *)
  output wire                        out_end
);
  
  wire [DATA_WIDTH-1:0] x_data;
  wire x_eop;
  wire x_valid;

  wire [DATA_WIDTH-1:0] y_data;

  wire y_eop;
  wire y_valid;

  wire out_fifo_full;
  wire empty;

  wire x_empty;
  wire y_empty;
  wire shift_out;

  wire [DATA_WIDTH-1:0] fifo_in;

  wire shift_in;
  wire s_axis_y_tready;
  wire s_axis_x_tready;
  wire out_eop;
  
  assign shift_out  = !x_empty && !y_empty && s_axis_x_tready && s_axis_y_tready;
  
  /* verilator lint_off PINMISSING */
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

    .dout           ({x_eop, x_data}),
    .valid          (x_valid),
    .shift_out      (shift_out),
    .empty          (x_empty)

  );
  /* verilator lint_on PINMISSING */

  /* verilator lint_off PINMISSING */
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

    .dout           ({y_eop, y_data}),
    .valid          (y_valid),

    .shift_out      (shift_out),
    .empty          (y_empty)

  );
  /* verilator lint_on PINMISSING */

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
    .m_axis_result_tdata  (fifo_in),    // output wire [31 : 0] m_axis_result_tdata
    .m_axis_result_tlast  (out_eop)    // output wire m_axis_result_tlast
  ); 

  /* verilator lint_off PINMISSING */
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
    .full         (out_fifo_full)

  );
  /* verilator lint_on PINMISSING */

endmodule
