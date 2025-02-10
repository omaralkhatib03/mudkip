`timescale 1ns/1ps

module fl_vadd_top  #(
  parameter AXI_VECTOR_WIDTH    = 32,
  parameter AXI_PS_DATAWIDTH    = 32,
  parameter AXI_DDR_DATAWIDTH   = 32,
  parameter AXI_DDR_ADDR_WIDTH  = 4,
  parameter AXI_PS_ADDR_WIDTH   = 4
) ( 
  input                          clk,
  input                          rst_n,

  // DMA Write In Vector X
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axi_mm_x AWADDR" *) 
  input  [AXI_PS_ADDR_WIDTH-1:0] waddr_y,    
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axi_mm_x AWVALID" *) 
  input                   wavalid_y,  
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axi_mm_x AWREADY" *) 
  output                  waready_y,  

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axi_mm_x WDATA" *) 
  input  [AXI_PS_DATAWIDTH-1:0] wdata_y,   
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axi_mm_x WVALID" *) 
  input                   wvalid_y,   
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axi_mm_x WREADY" *) 
  output                  wready_y,  

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axi_mm_x BRESP" *) 
  output                  wresp_y,  
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axi_mm_x BVALID" *) 
  output                  bvalid_y,  
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axi_mm_x BREADY" *) 
  input                  bready_y,  

  // DMA Write In Vector X
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axi_mm_y  AWADDR" *) 
  input  [AXI_PS_ADDR_WIDTH-1:0] waddr_x,                       
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axi_mm_y  AWVALID" *) 
  input                   wavalid_x,                            
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axi_mm_y  AWREADY" *) 
  output                  waready_x,                            
                                                                
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axi_mm_y  WDATA" *) 
  input  [AXI_PS_DATAWIDTH-1:0] wdata_x,                        
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axi_mm_y  WVALID" *) 
  input                   wvalid_x,                             
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axi_mm_y  WREADY" *) 
  output                  wready_x,                             
                                                                
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axi_mm_y  BRESP" *) 
  output                  wresp_x,                              
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axi_mm_y  BVALID" *) 
  output                  bvalid_x,                             
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axi_mm_y  BREADY" *) 
  input                  bready_x,  

  // PS Slave Axi Interface mm2s_x
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

  // PS Slave Axi Interface mm2s_y
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if_x  AWADDR" *) 
  input  [AXI_PS_ADDR_WIDTH-1:0] waddr_mm2s_x,    
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if_x  AWVALID" *) 
  input                   wavalid_mm2s_x,  
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if_x  AWREADY" *) 
  output                  waready_mm2s_x,  

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if_x WDATA" *) 
  input  [AXI_PS_DATAWIDTH-1:0] wdata_mm2s_x,   
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if_x  WVALID" *) 
  input                   wvalid_mm2s_x,   
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if_x  WREADY" *) 
  output                  wready_mm2s_x,  

  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if_x  BRESP" *) 
  output                  wresp_mm2s_x,  
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if_x  BVALID" *) 
  output                  bvalid_mm2s_x,  
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if_x  BREADY" *) 
  input                   bready_mm2s_x,  

  // PS Slave Axi Interface 
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if_y  AWADDR" *) 
  input  [AXI_PS_ADDR_WIDTH-1:0] waddr_mm2s_y,                     
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if_y  AWVALID" *) 
  input                   wavalid_mm2s_y,                          
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if_y  AWREADY" *) 
  output                  waready_mm2s_y,                          
                                                                  
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if_y  WDATA" *) 
  input  [AXI_PS_DATAWIDTH-1:0] wdata_mm2s_y,                      
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if_y  WVALID" *) 
  input                   wvalid_mm2s_y,                           
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if_y  WREADY" *) 
  output                  wready_mm2s_y,                           
                                                                   
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if_y  BRESP" *) 
  output                  wresp_mm2s_y,                            
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if_y  BVALID" *) 
  output                  bvalid_mm2s_y,                           
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if_y  BREADY" *) 
  input                  bready_mm2s_y,  

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

  axi_stream_if #(.DATA_WIDTH(AXI_VECTOR_WIDTH)) vector_out();

  axi_lite_if #(
    .ADDR_WIDTH(AXI_PS_ADDR_WIDTH),
    .DATA_WIDTH(AXI_PS_DATAWIDTH)
  ) axil_ps_if ();

  axi_lite_if #(
    .ADDR_WIDTH(AXI_PS_ADDR_WIDTH),
    .DATA_WIDTH(AXI_PS_DATAWIDTH)
  ) axil_ps_if_x ();

  axi_lite_if #(
    .ADDR_WIDTH(AXI_PS_ADDR_WIDTH),
    .DATA_WIDTH(AXI_PS_DATAWIDTH)
  ) axil_ps_if_y ();

  axi_lite_if #(
    .ADDR_WIDTH(AXI_DDR_ADDR_WIDTH),
    .DATA_WIDTH(AXI_DDR_DATAWIDTH)
  ) axil_ram_if ();

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

  dma_read dma_read_x_I (
    .clk    (clk),
    .rst_n  (rst_n)

  );

  ps_if #(
    .ADDR_WIDTH(axil_ps_if.ADDR_WIDTH),
    .DATA_WIDTH(axil_ps_if.DATA_WIDTH)
  ) ps_m_i ();
  
   axl_ps_adapter  #(
    .FIFO_DEPTH(128)
  ) axl_ps_top_adapter_I (
    .clk(clk),
    .rst_n(rst_n),
    .axl_m_i(axil_ps_if),
    .ps_m_i(ps_m_i)
  );

  fl_vadd # (
    .DATA_WIDTH(32) 
  ) fl_v_add_I ( 
    .clk(clk),
    .rst_n(rst_n),

    .in_x_data    (in_x_data), 
    .in_x_valid   (in_x_valid), 
    .x_ready      (in_x_ready),
    .x_end        (in_x_tlast),

    .in_y_data    (in_y_data), 
    .in_y_valid   (in_y_valid), 
    .y_ready      (in_y_ready),
    .y_end        (in_y_tlast),

    .out_data     (vector_out.data), 
    .out_valid    (vector_out.valid), 
    .out_ready    (vector_out.ready),
    .out_end      (vector_out.last)
  );

  s2mm #(
    .DATA_WIDTH(AXI_VECTOR_WIDTH) 
  ) s2mm_ddr_I (

    .clk(clk),
    .rst_n(rst_n),
    .ps_i(ps_m_i),
    .din_i(vector_out),
    .dout_i(axil_ram_if)
  );

endmodule
