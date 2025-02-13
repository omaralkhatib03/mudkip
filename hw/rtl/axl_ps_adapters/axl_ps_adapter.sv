
`timescale 1 ns/1 ps

// Ensure that the input and output have the same data width and depth (address bits) targets.

module axl_ps_adapter  #(
  parameter FIFO_DEPTH    = 16,
  parameter READ_LATENCY  = 1 
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

  logic   w_empty;
  logic   r_empty;
  logic   b_empty;
  logic   res_empty;

  logic ax_full;
  logic ps_full;

  /* verilator lint_off PINMISSING */ // (Over/Under) Pins
  ctrl_data_fifo #(
    .DATA_WIDTH(axl_m_i.DATA_WIDTH),
    .CTRL_WIDTH(ADDR_WIDTH),
    .DEPTH(FIFO_DEPTH),
    .READ_LATENCY(READ_LATENCY)      
  ) w_adapter_fifo_I (
    .clk                  (clk),
    .rst_n                (rst_n),

    .din_data             (axl_m_i.wdata),
    .data_valid           (axl_m_i.wvalid),
    .data_ready           (axl_m_i.wready),

    .ctrl_data            (axl_m_i.waddr),
    .ctrl_valid           (axl_m_i.wavalid),
    .ctrl_ready           (axl_m_i.waready),

    .dout                 ({ps_m_i.waddr, ps_m_i.wdata}),
    .valid                (ps_m_i.wvalid),

    .shift_out            (shift_out_w),

    .empty                (w_empty)
  );
  /* verilator lint_off PINMISSING */ // (Over/Under) Pins

  /* verilator lint_off PINMISSING */ // (Over/Under) Pins
  basic_sync_fifo #(
    .DEPTH          (FIFO_DEPTH),
    .DATA_WIDTH     (axl_m_i.ADDR_WIDTH),
    .READ_LATENCY   (READ_LATENCY)      
  ) r_adapter_fifo_I (
    .clk            (clk),
    .rst_n          (rst_n),

    .din            (axl_m_i.raddr),
    .shift_in       (axl_m_i.arvalid),

    .full           (ax_full),
    .dout           (ps_m_i.raddr),
    .valid          (ps_m_i.arvalid),
    .shift_out      (shift_out_r),
    .empty          (r_empty)
  );
  /* verilator lint_off PINMISSING */ // (Over/Under) Pins

  assign shift_out_w      = !w_empty && ps_m_i.wready;
  assign shift_out_r      = !r_empty && ps_m_i.aready;

  assign axl_m_i.arready  = !ax_full; 
  assign ps_m_i.rready    = !ps_full; 

  /* verilator lint_off PINMISSING */ // (Over/Under) Pins
  basic_sync_fifo #(
    .DEPTH          (FIFO_DEPTH),
    .DATA_WIDTH     (axl_m_i.DATA_WIDTH),
    .READ_LATENCY   (READ_LATENCY)      
  ) res_adapter_fifo_I (
    .clk            (clk),
    .rst_n          (rst_n),

    .din            (ps_m_i.rdata),
    .shift_in       (ps_m_i.rvalid),

    .full           (ps_full),

    .dout           (axl_m_i.rdata),
    .valid          (axl_m_i.rvalid),
    .shift_out      (axl_m_i.rready && !res_empty),
    .empty          (res_empty)
  );
  /* verilator lint_off PINMISSING */ // (Over/Under) Pins


  /* verilator lint_off PINMISSING */ // (Over/Under) Pins
  basic_sync_fifo #(
    .DEPTH          (FIFO_DEPTH),
    .DATA_WIDTH     (axl_m_i.DATA_WIDTH),
    .READ_LATENCY   (READ_LATENCY)      
  ) b_adapter_fifo_I (
    .clk            (clk),
    .rst_n          (rst_n),

    .din            (ps_m_i.bdata),
    .shift_in       (ps_m_i.bvalid),

    .full           (ps_m_i.bready),

    .dout           (axl_m_i.bdata),
    .valid          (axl_m_i.bvalid),
    .shift_out      (axl_m_i.bready && !b_empty),
    .empty          (b_empty)
  );
  /* verilator lint_off PINMISSING */ // (Over/Under) Pins



endmodule

