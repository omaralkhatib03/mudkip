//`timescale 1ns/1ps

// module fl_vadd_v ( 
//   input                          clk,
//   input                          rst_n,
// 
//   (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_x tdata" *)
//   input  [31:0]                  in_x_data, 
//   (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_x tvalid" *)
//   input                          in_x_valid, 
//   (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_x tready" *)
//   output                         x_ready,
// 
//   (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_y tdata" *)
//   input  [31:0]                  in_y_data, 
//   (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_y tvalid" *)
//   input                          in_y_valid, 
//   (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_y tready" *)
//   output                         y_ready,
//   
//   (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_out tdata" *)
//   output  [31:0]                 out_data, 
//   (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_out tvalid" *)
//   output                         out_valid, 
//   (* x_interface_info = "xilinx.com:interface:axis:1.0 vector_out tready" *)
//   input                          out_ready
// 
// );
// 
//  fl_vadd fl_vadd_i ( 
//   .clk        (clk),
//   .rst_n      (rst_n),
//   .in_x_data  (in_x_data), 
//   .in_x_valid (in_x_valid), 
//   .x_ready    (x_ready),
//   .in_y_data  (in_y_data), 
//   .in_y_valid (in_y_valid), 
//   .y_ready    (y_ready),
//   .out_data   (out_data), 
//   .out_valid  (out_valid), 
//   .out_ready  (out_ready)
// );
// 
// 
// endmodule
