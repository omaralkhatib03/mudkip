`timescale 1ns/1ps

module rf_top  #(
  parameter DATA_WIDTH   = 32,
  parameter ADDR_WIDTH   = 4
) ( 
  input                   clk,
  input                   rst_n,

  // AXI Master PS 
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if AWADDR" *) 
  output [ADDR_WIDTH-1:0] waddr,

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if AWVALID" *) 
  output                  wavalid,

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if AWREADY" *) 
  input                   waready,

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if WDATA" *) 
  output [DATA_WIDTH-1:0] wdata, 

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if WVALID" *) 
  output                  wvalid,

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if WREADY" *) 
  input                   wready,

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if BRESP" *) 
  input                   wresp, 

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if BVALID" *) 
  input                   bvalid, 

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if BREADY" *) 
  output                  bready, 

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if ARADDR" *) 
  output [ADDR_WIDTH-1:0] raddr,

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if ARVALID" *) 
  output                  arvalid,

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if ARREADY" *) 
  input                   arready, 

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if RDATA" *) 
  input  [DATA_WIDTH-1:0] rdata,  

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if RVALID" *) 
  input                   rvalid,

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if RREADY" *) 
  output                  rready 

);

  axi_lite_if #(
    .ADDR_WIDTH(ADDR_WIDTH),
    .DATA_WIDTH(DATA_WIDTH)
  ) axil_ps_if ();

  assign waddr = axil_ps_if.waddr; 
  assign wavalid = axil_ps_if.wavalid;
  assign axil_ps_if.waready = waready;
  assign wdata  = axil_ps_if.wdata;
  assign wvalid  = axil_ps_if.wvalid;
  assign axil_ps_if.wready = wready;
  assign axil_ps_if.wresp  = wresp;
  assign axil_ps_if.bvalid = bvalid;
  assign bready = axil_ps_if.bready;
  assign raddr   = axil_ps_if.raddr;
  assign arvalid = axil_ps_if.arvalid;
  assign axil_ps_if.arready = arready;
  assign axil_ps_if.rdata   = rdata;
  assign axil_ps_if.rvalid  = rvalid;
  assign rready  = axil_ps_if.rready;

  ps_if #(
    .ADDR_WIDTH(axil_ps_if.ADDR_WIDTH),
    .DATA_WIDTH(axil_ps_if.DATA_WIDTH)
  ) ps_m_i ();
  
   axl_ps_adapter  #(
    .FIFO_DEPTH(128)
  ) axl_ps_top_adapter_I (
    .clk        (clk),
    .rst_n      (rst_n),
    .axl_m_i    (axil_ps_if),
    .ps_m_i     (ps_m_i)
  );

  rf_node dut_I (
    .clk    (clk),
    .rst_n  (rst_n),
    .ps_i   (ps_m_i)
  );

endmodule

