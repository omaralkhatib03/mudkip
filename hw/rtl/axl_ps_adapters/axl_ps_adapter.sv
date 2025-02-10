`timescale 1 ns/1 ps

// Ensure that the input and output have the same data width and depth (address bits) targets.

module axl_ps_adapter  #(
  parameter FIFO_DEPTH = 256
) (
  input wire clk,
  input wire rst_n,
  
  axi_lite_if.slave axl_m_i,
  ps_if.master ps_m_i

);

  `define ASSERT_PARAM_EQUAL(param1, param2) \
      initial begin \
          if (param1 !== param2) begin \
              $error("Assertion failed: Parameters %s (%0d) and %s (%0d) are not equal", `"param1`", param1, `"param2`", param2); \
          end \
      end

  `ASSERT_PARAM_EQUAL(axl_m_i.DATA_WIDTH, ps_m_i.DATA_WIDTH)
  `ASSERT_PARAM_EQUAL(axl_m_i.ADDR_WIDTH, ps_m_i.ADDR_WIDTH)

  localparam ADDR_WIDTH = axl_m_i.ADDR_WIDTH;

  logic   shift_out_w;
  logic   shift_out_r;

  logic   r_empty;
  logic   w_empty;

  ctrl_data_fifo #(
    .DATA_WIDTH(axl_m_i.DATA_WIDTH),
    .CTRL_WIDTH(ADDR_WIDTH),
    .DEPTH(FIFO_DEPTH),
    .READ_LATENCY(1)      
  ) w_adapter_fifo_I (
    .clk(clk),
    .rst_n(rst_n),

    .din_data(axl_m_i.wdata),
    .data_valid(axl_m_i.wvalid),
    .data_ready(axl_m_i.wready),

    .ctrl_data(axl_m_i.waddr),
    .ctrl_valid(axl_m_i.wavalid),
    .ctrl_ready(axl_m_i.waready),

    .dout({ps_m_i.waddr, ps_m_i.wdata}),
    .valid(ps_m_i.wvalid),
    .ready(ps_m_i.wready),

    .shift_out(shift_out_w),

    .empty(w_empty),

    .data_overflow(), 
    .data_underflow(), 

    .ctrl_overflow(),
    .ctrl_underflow() 
  );

  assign shift_out_w    = !w_empty && ps_m_i.wready;
  assign shift_out_r    = !r_empty && ps_m_i.rready;

  assign axl_m_i.wresp  = ps_m_i.wresp;  

  assign ps_m_i.raddr   = axl_m_i.raddr;
  assign ps_m_i.arvalid = axl_m_i.arvalid;

  assign axl_m_i.rdata  = ps_m_i.rdata;

  assign axl_m_i.rvalid = ps_m_i.rvalid;
  assign ps_m_i.rready  = axl_m_i.rready;
  
endmodule
