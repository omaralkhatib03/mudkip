`timescale 1ns/1ps

module fl_vadd_top  #(
  parameter AXI_VECTOR_WIDTH  = 32,
  parameter AXI_PS_DEPTH      = 32,
  parameter AXI_PS_DATAWIDTH  = 32,
  parameter AXI_DDR_DATAWIDTH = 32,
  parameter AXI_DDR_DEPTH     = 32,
  parameter AXI_DDR_ADDR_WIDTH = $clog2(AXI_DDR_DEPTH),
  parameter AXI_PS_ADDR_WIDTH = $clog2(AXI_PS_DEPTH)
) ( 
  input                          clk,
  input                          rst_n,

  // DMA Write In Vector X
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 vector_x tdata" *)
  input  [AXI_VECTOR_WIDTH-1:0]  in_x_data, 
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 vector_x tvalid" *)
  input                          in_x_valid, 
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 vector_x tready" *)
  output                         x_ready,
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 vector_x tlast" *)
  input                          in_x_tlast,

  // DMA Write In Vector X
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 vector_y tdata" *)
  input  [AXI_VECTOR_WIDTH-1:0] in_y_data, 
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 vector_y tvalid" *)
  input                          in_y_valid, 
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 vector_y tready" *)
  output                         y_ready,
  (* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 vector_y tlast" *)
  input                          in_y_tlast,
  
  // PS Slave Axi Interface 
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if  AWADDR" *) 
  input  [AXI_PS_ADDR_WIDTH-1:0] waddr,    
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if  AWVALID" *) 
  input                   wavalid,  
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if  AWREADY" *) 
  output                  waready,  

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if  WDATA" *) 
  input  [AXI_PS_DATAWIDTH-1:0] wdata,   
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if  WVALID" *) 
  input                   wvalid,   
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if  WREADY" *) 
  output                  wready,  

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if  BRESP" *) 
  output                  wresp,  
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if  BVALID" *) 
  output                  bvalid,  
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if  BREADY" *) 
  input                  bready,  

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if  ARADDR" *) 
  input  [AXI_PS_ADDR_WIDTH-1:0] raddr, 
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if  ARVALID" *) 
  input                   arvalid,
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if  ARREADY" *) 
  output                  arready,

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if  RDATA" *) 
  output [AXI_PS_DATAWIDTH-1:0] rdata, 
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if  RVALID" *) 
  output                  rvalid,   
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if  RREADY" *) 
  input                   rready,

  // AXI Master Output DDR 
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if AWADDR" *) 
  output [AXI_DDR_ADDR_WIDTH-1:0] out_vector_waddr,
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if AWVALID" *) 
  output                  out_vector_wavalid,
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if AWREADY" *) 
  input                   out_vector_waready,

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if WDATA" *) 
  output [AXI_DDR_DATAWIDTH-1:0] out_vector_wdata, 
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if WVALID" *) 
  output                  out_vector_wvalid,
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if WREADY" *) 
  input                   out_vector_wready,

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if BRESP" *) 
  input                   out_vector_wresp, 
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if BVALID" *) 
  input                   out_vector_bvalid, 
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if BREADY" *) 
  output                  out_vector_bready, 

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if ARADDR" *) 
  output [AXI_DDR_ADDR_WIDTH-1:0] out_vector_raddr,
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if ARVALID" *) 
  output                  out_vector_arvalid,
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if ARREADY" *) 
  input                   out_vector_arready, 

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if RDATA" *) 
  input  [AXI_DDR_DATAWIDTH-1:0] out_vector_rdata,  
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if RVALID" *) 
  input                   out_vector_rvalid,
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if RREADY" *) 
  output                  out_vector_rready 

);

  axi_stream_if #(.DATA_WIDTH(AXI_VECTOR_WIDTH)) vector_x();
  axi_stream_if #(.DATA_WIDTH(AXI_VECTOR_WIDTH)) vector_y();

  axi_lite_if #(
    .DEPTH(AXI_PS_DEPTH),
    .DATA_WIDTH(AXI_PS_DATAWIDTH)
  ) axil_ps_if ();

  axi_lite_if #(
    .DEPTH(AXI_DDR_DEPTH),
    .DATA_WIDTH(AXI_DDR_DATAWIDTH)
  ) axil_ram_if ();

  assign axil_ps_if.waddr = waddr;
  assign axil_ps_if.wavalid = wavalid;
  assign waready = axil_ps_if.waready;
  assign axil_ps_if.wdata = wdata;
  assign axil_ps_if.wvalid = wvalid;
  assign wready = axil_ps_if.wready;
  
  assign wresp = axil_ps_if.wresp;
  assign bvalid = axil_ps_if.wresp;
  assign axil_ps_if.bready = bready;

  assign axil_ps_if.raddr = raddr;
  assign axil_ps_if.arvalid = arvalid;
  assign arready = axil_ps_if.arready;
  assign rdata = axil_ps_if.rdata;
  assign rvalid = axil_ps_if.rvalid;
  assign axil_ps_if.rready = rready;

  assign out_vector_waddr = axil_ram_if.waddr; 
  assign out_vector_wavalid = axil_ram_if.wavalid;
  assign axil_ram_if.waready = out_vector_waready;
  assign out_vector_wdata  = axil_ram_if.wdata;
  assign out_vector_wvalid  = axil_ram_if.wvalid;
  assign axil_ram_if.wready = out_vector_wready;
  assign axil_ram_if.wresp  = out_vector_wresp;
  assign axil_ram_if.bvalid = out_vector_bvalid;
  assign out_vector_bready = axil_ram_if.bready;
  assign out_vector_raddr   = axil_ram_if.raddr;
  assign out_vector_arvalid = axil_ram_if.arvalid;
  assign axil_ram_if.arready = out_vector_arready;
  assign axil_ram_if.rdata   = out_vector_rdata;
  assign axil_ram_if.rvalid  = out_vector_rvalid;
  assign out_vector_rready  = axil_ram_if.rready;

  assign 

endmodule
