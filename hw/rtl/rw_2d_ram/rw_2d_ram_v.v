`timescale 1 ns/1 ps

module rw_2d_ram_v
#(
  parameter DEPTH = 32,
  parameter WIDTH = 32
) (
  input                     clk, 
  input                     rst_n,

  // read intf 
  input                     s_axis_arvalid,
  input [$clog2(DEPTH)-1:0] s_axis_raddr,
  output                    s_axis_arready,

  output [WIDTH-1:0]        s_axis_rdata,
  output                    s_axis_rvalid,
  input                     s_axis_rready,
  
  // write intf
  input                     s_axis_awvalid,
  input                     s_axis_wvalid,
  input [$clog2(DEPTH)-1:0] s_axis_waddr,
  input [WIDTH-1:0]         s_axis_wdata
);

  wire [WIDTH-1:0] r_data;
  wire valid_inter;
  wire empty;
  wire shift_out;

  assign shift_out = !empty && !s_axis_rready; 

  rw_2d_ram
  #(
    .DEPTH(DEPTH),
    .WIDTH(WIDTH)
  ) sv_mod (
      .clk(clk),
      .rst_n(rst_n),
      .valid_addr_ps(s_axis_arvalid),
      .r_addr(s_axis_raddr),
      .r_data(r_data),
      .valid_data(valid_inter),
      .valid_w(s_axis_awvalid && s_axis_wvalid),
      .w_addr(s_axis_waddr),
      .w_data(s_axis_wdata),
      .ready_pl()
  );

  basic_sync_fifo #(
    .DATA_WIDTH(32),        
    .DEPTH(32),
    .READ_LATENCY(0)
  ) r_fifo_out (
    .clk(clk),
    .rst_n(rst_n),

    .din(r_data),
    .shift_in(valid_inter),
    .shift_out(shift_out),
    .dout(s_axis_rdata),
    .valid(s_axis_rvalid),

    .full(s_axis_arready),
    .empty(empty),
    .overflow(),
    .underflow()
  );

endmodule
