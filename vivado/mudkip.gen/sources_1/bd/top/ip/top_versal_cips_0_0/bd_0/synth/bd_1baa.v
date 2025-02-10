//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Command: generate_target bd_1baa.bd
//Design : bd_1baa
//Purpose: IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

(* CORE_GENERATION_INFO = "bd_1baa,IP_Integrator,{x_ipVendor=xilinx.com,x_ipLibrary=BlockDiagram,x_ipName=bd_1baa,x_ipVersion=1.00.a,x_ipLanguage=VERILOG,numBlks=1,numReposBlks=1,numNonXlnxBlks=0,numHierBlks=0,maxHierDepth=0,numSysgenBlks=0,numHlsBlks=0,numHdlrefBlks=0,numPkgbdBlks=0,bdsource=SBD,synth_mode=None}" *) (* HW_HANDOFF = "top_versal_cips_0_0.hwdef" *) 
module bd_1baa
   (FPD_CCI_NOC_0_araddr,
    FPD_CCI_NOC_0_arburst,
    FPD_CCI_NOC_0_arcache,
    FPD_CCI_NOC_0_arid,
    FPD_CCI_NOC_0_arlen,
    FPD_CCI_NOC_0_arlock,
    FPD_CCI_NOC_0_arprot,
    FPD_CCI_NOC_0_arqos,
    FPD_CCI_NOC_0_arready,
    FPD_CCI_NOC_0_arsize,
    FPD_CCI_NOC_0_aruser,
    FPD_CCI_NOC_0_arvalid,
    FPD_CCI_NOC_0_awaddr,
    FPD_CCI_NOC_0_awburst,
    FPD_CCI_NOC_0_awcache,
    FPD_CCI_NOC_0_awid,
    FPD_CCI_NOC_0_awlen,
    FPD_CCI_NOC_0_awlock,
    FPD_CCI_NOC_0_awprot,
    FPD_CCI_NOC_0_awqos,
    FPD_CCI_NOC_0_awready,
    FPD_CCI_NOC_0_awsize,
    FPD_CCI_NOC_0_awuser,
    FPD_CCI_NOC_0_awvalid,
    FPD_CCI_NOC_0_bid,
    FPD_CCI_NOC_0_bready,
    FPD_CCI_NOC_0_bresp,
    FPD_CCI_NOC_0_bvalid,
    FPD_CCI_NOC_0_rdata,
    FPD_CCI_NOC_0_rid,
    FPD_CCI_NOC_0_rlast,
    FPD_CCI_NOC_0_rready,
    FPD_CCI_NOC_0_rresp,
    FPD_CCI_NOC_0_rvalid,
    FPD_CCI_NOC_0_wdata,
    FPD_CCI_NOC_0_wlast,
    FPD_CCI_NOC_0_wready,
    FPD_CCI_NOC_0_wstrb,
    FPD_CCI_NOC_0_wuser,
    FPD_CCI_NOC_0_wvalid,
    FPD_CCI_NOC_1_araddr,
    FPD_CCI_NOC_1_arburst,
    FPD_CCI_NOC_1_arcache,
    FPD_CCI_NOC_1_arid,
    FPD_CCI_NOC_1_arlen,
    FPD_CCI_NOC_1_arlock,
    FPD_CCI_NOC_1_arprot,
    FPD_CCI_NOC_1_arqos,
    FPD_CCI_NOC_1_arready,
    FPD_CCI_NOC_1_arsize,
    FPD_CCI_NOC_1_aruser,
    FPD_CCI_NOC_1_arvalid,
    FPD_CCI_NOC_1_awaddr,
    FPD_CCI_NOC_1_awburst,
    FPD_CCI_NOC_1_awcache,
    FPD_CCI_NOC_1_awid,
    FPD_CCI_NOC_1_awlen,
    FPD_CCI_NOC_1_awlock,
    FPD_CCI_NOC_1_awprot,
    FPD_CCI_NOC_1_awqos,
    FPD_CCI_NOC_1_awready,
    FPD_CCI_NOC_1_awsize,
    FPD_CCI_NOC_1_awuser,
    FPD_CCI_NOC_1_awvalid,
    FPD_CCI_NOC_1_bid,
    FPD_CCI_NOC_1_bready,
    FPD_CCI_NOC_1_bresp,
    FPD_CCI_NOC_1_bvalid,
    FPD_CCI_NOC_1_rdata,
    FPD_CCI_NOC_1_rid,
    FPD_CCI_NOC_1_rlast,
    FPD_CCI_NOC_1_rready,
    FPD_CCI_NOC_1_rresp,
    FPD_CCI_NOC_1_rvalid,
    FPD_CCI_NOC_1_wdata,
    FPD_CCI_NOC_1_wlast,
    FPD_CCI_NOC_1_wready,
    FPD_CCI_NOC_1_wstrb,
    FPD_CCI_NOC_1_wuser,
    FPD_CCI_NOC_1_wvalid,
    FPD_CCI_NOC_2_araddr,
    FPD_CCI_NOC_2_arburst,
    FPD_CCI_NOC_2_arcache,
    FPD_CCI_NOC_2_arid,
    FPD_CCI_NOC_2_arlen,
    FPD_CCI_NOC_2_arlock,
    FPD_CCI_NOC_2_arprot,
    FPD_CCI_NOC_2_arqos,
    FPD_CCI_NOC_2_arready,
    FPD_CCI_NOC_2_arsize,
    FPD_CCI_NOC_2_aruser,
    FPD_CCI_NOC_2_arvalid,
    FPD_CCI_NOC_2_awaddr,
    FPD_CCI_NOC_2_awburst,
    FPD_CCI_NOC_2_awcache,
    FPD_CCI_NOC_2_awid,
    FPD_CCI_NOC_2_awlen,
    FPD_CCI_NOC_2_awlock,
    FPD_CCI_NOC_2_awprot,
    FPD_CCI_NOC_2_awqos,
    FPD_CCI_NOC_2_awready,
    FPD_CCI_NOC_2_awsize,
    FPD_CCI_NOC_2_awuser,
    FPD_CCI_NOC_2_awvalid,
    FPD_CCI_NOC_2_bid,
    FPD_CCI_NOC_2_bready,
    FPD_CCI_NOC_2_bresp,
    FPD_CCI_NOC_2_bvalid,
    FPD_CCI_NOC_2_rdata,
    FPD_CCI_NOC_2_rid,
    FPD_CCI_NOC_2_rlast,
    FPD_CCI_NOC_2_rready,
    FPD_CCI_NOC_2_rresp,
    FPD_CCI_NOC_2_rvalid,
    FPD_CCI_NOC_2_wdata,
    FPD_CCI_NOC_2_wlast,
    FPD_CCI_NOC_2_wready,
    FPD_CCI_NOC_2_wstrb,
    FPD_CCI_NOC_2_wuser,
    FPD_CCI_NOC_2_wvalid,
    FPD_CCI_NOC_3_araddr,
    FPD_CCI_NOC_3_arburst,
    FPD_CCI_NOC_3_arcache,
    FPD_CCI_NOC_3_arid,
    FPD_CCI_NOC_3_arlen,
    FPD_CCI_NOC_3_arlock,
    FPD_CCI_NOC_3_arprot,
    FPD_CCI_NOC_3_arqos,
    FPD_CCI_NOC_3_arready,
    FPD_CCI_NOC_3_arsize,
    FPD_CCI_NOC_3_aruser,
    FPD_CCI_NOC_3_arvalid,
    FPD_CCI_NOC_3_awaddr,
    FPD_CCI_NOC_3_awburst,
    FPD_CCI_NOC_3_awcache,
    FPD_CCI_NOC_3_awid,
    FPD_CCI_NOC_3_awlen,
    FPD_CCI_NOC_3_awlock,
    FPD_CCI_NOC_3_awprot,
    FPD_CCI_NOC_3_awqos,
    FPD_CCI_NOC_3_awready,
    FPD_CCI_NOC_3_awsize,
    FPD_CCI_NOC_3_awuser,
    FPD_CCI_NOC_3_awvalid,
    FPD_CCI_NOC_3_bid,
    FPD_CCI_NOC_3_bready,
    FPD_CCI_NOC_3_bresp,
    FPD_CCI_NOC_3_bvalid,
    FPD_CCI_NOC_3_rdata,
    FPD_CCI_NOC_3_rid,
    FPD_CCI_NOC_3_rlast,
    FPD_CCI_NOC_3_rready,
    FPD_CCI_NOC_3_rresp,
    FPD_CCI_NOC_3_rvalid,
    FPD_CCI_NOC_3_wdata,
    FPD_CCI_NOC_3_wlast,
    FPD_CCI_NOC_3_wready,
    FPD_CCI_NOC_3_wstrb,
    FPD_CCI_NOC_3_wuser,
    FPD_CCI_NOC_3_wvalid,
    fpd_cci_noc_axi0_clk,
    fpd_cci_noc_axi1_clk,
    fpd_cci_noc_axi2_clk,
    fpd_cci_noc_axi3_clk,
    pl0_ref_clk);
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 ARADDR" *) (* X_INTERFACE_MODE = "Master" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME FPD_CCI_NOC_0, ADDR_WIDTH 64, ARUSER_WIDTH 18, AWUSER_WIDTH 18, BUSER_WIDTH 0, CATEGORY noc, CLK_DOMAIN bd_1baa_pspmc_0_0_fpd_cci_noc_axi0_clk, DATA_WIDTH 128, FREQ_HZ 824991760, HAS_BRESP 1, HAS_BURST 1, HAS_CACHE 1, HAS_LOCK 1, HAS_PROT 1, HAS_QOS 1, HAS_REGION 0, HAS_RRESP 1, HAS_WSTRB 1, ID_WIDTH 16, INDEX 0, INSERT_VIP 0, MAX_BURST_LENGTH 256, MY_CATEGORY ps_cci, NUM_READ_OUTSTANDING 64, NUM_READ_THREADS 1, NUM_WRITE_OUTSTANDING 64, NUM_WRITE_THREADS 1, PHASE 0.0, PHYSICAL_CHANNEL PS_CCI_TO_NOC_NMU, PROTOCOL AXI4, READ_WRITE_MODE READ_WRITE, RUSER_BITS_PER_BYTE 0, RUSER_WIDTH 0, SUPPORTS_NARROW_BURST 1, WUSER_BITS_PER_BYTE 0, WUSER_WIDTH 17" *) output [63:0]FPD_CCI_NOC_0_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 ARBURST" *) output [1:0]FPD_CCI_NOC_0_arburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 ARCACHE" *) output [3:0]FPD_CCI_NOC_0_arcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 ARID" *) output [15:0]FPD_CCI_NOC_0_arid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 ARLEN" *) output [7:0]FPD_CCI_NOC_0_arlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 ARLOCK" *) output FPD_CCI_NOC_0_arlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 ARPROT" *) output [2:0]FPD_CCI_NOC_0_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 ARQOS" *) output [3:0]FPD_CCI_NOC_0_arqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 ARREADY" *) input FPD_CCI_NOC_0_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 ARSIZE" *) output [2:0]FPD_CCI_NOC_0_arsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 ARUSER" *) output [17:0]FPD_CCI_NOC_0_aruser;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 ARVALID" *) output FPD_CCI_NOC_0_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 AWADDR" *) output [63:0]FPD_CCI_NOC_0_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 AWBURST" *) output [1:0]FPD_CCI_NOC_0_awburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 AWCACHE" *) output [3:0]FPD_CCI_NOC_0_awcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 AWID" *) output [15:0]FPD_CCI_NOC_0_awid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 AWLEN" *) output [7:0]FPD_CCI_NOC_0_awlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 AWLOCK" *) output FPD_CCI_NOC_0_awlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 AWPROT" *) output [2:0]FPD_CCI_NOC_0_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 AWQOS" *) output [3:0]FPD_CCI_NOC_0_awqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 AWREADY" *) input FPD_CCI_NOC_0_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 AWSIZE" *) output [2:0]FPD_CCI_NOC_0_awsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 AWUSER" *) output [17:0]FPD_CCI_NOC_0_awuser;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 AWVALID" *) output FPD_CCI_NOC_0_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 BID" *) input [15:0]FPD_CCI_NOC_0_bid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 BREADY" *) output FPD_CCI_NOC_0_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 BRESP" *) input [1:0]FPD_CCI_NOC_0_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 BVALID" *) input FPD_CCI_NOC_0_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 RDATA" *) input [127:0]FPD_CCI_NOC_0_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 RID" *) input [15:0]FPD_CCI_NOC_0_rid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 RLAST" *) input FPD_CCI_NOC_0_rlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 RREADY" *) output FPD_CCI_NOC_0_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 RRESP" *) input [1:0]FPD_CCI_NOC_0_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 RVALID" *) input FPD_CCI_NOC_0_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 WDATA" *) output [127:0]FPD_CCI_NOC_0_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 WLAST" *) output FPD_CCI_NOC_0_wlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 WREADY" *) input FPD_CCI_NOC_0_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 WSTRB" *) output [15:0]FPD_CCI_NOC_0_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 WUSER" *) output [16:0]FPD_CCI_NOC_0_wuser;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_0 WVALID" *) output FPD_CCI_NOC_0_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 ARADDR" *) (* X_INTERFACE_MODE = "Master" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME FPD_CCI_NOC_1, ADDR_WIDTH 64, ARUSER_WIDTH 18, AWUSER_WIDTH 18, BUSER_WIDTH 0, CATEGORY noc, CLK_DOMAIN bd_1baa_pspmc_0_0_fpd_cci_noc_axi1_clk, DATA_WIDTH 128, FREQ_HZ 824991760, HAS_BRESP 1, HAS_BURST 1, HAS_CACHE 1, HAS_LOCK 1, HAS_PROT 1, HAS_QOS 1, HAS_REGION 0, HAS_RRESP 1, HAS_WSTRB 1, ID_WIDTH 16, INDEX 1, INSERT_VIP 0, MAX_BURST_LENGTH 256, MY_CATEGORY ps_cci, NUM_READ_OUTSTANDING 64, NUM_READ_THREADS 1, NUM_WRITE_OUTSTANDING 64, NUM_WRITE_THREADS 1, PHASE 0.0, PHYSICAL_CHANNEL PS_CCI_TO_NOC_NMU, PROTOCOL AXI4, READ_WRITE_MODE READ_WRITE, RUSER_BITS_PER_BYTE 0, RUSER_WIDTH 0, SUPPORTS_NARROW_BURST 1, WUSER_BITS_PER_BYTE 0, WUSER_WIDTH 17" *) output [63:0]FPD_CCI_NOC_1_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 ARBURST" *) output [1:0]FPD_CCI_NOC_1_arburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 ARCACHE" *) output [3:0]FPD_CCI_NOC_1_arcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 ARID" *) output [15:0]FPD_CCI_NOC_1_arid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 ARLEN" *) output [7:0]FPD_CCI_NOC_1_arlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 ARLOCK" *) output FPD_CCI_NOC_1_arlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 ARPROT" *) output [2:0]FPD_CCI_NOC_1_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 ARQOS" *) output [3:0]FPD_CCI_NOC_1_arqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 ARREADY" *) input FPD_CCI_NOC_1_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 ARSIZE" *) output [2:0]FPD_CCI_NOC_1_arsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 ARUSER" *) output [17:0]FPD_CCI_NOC_1_aruser;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 ARVALID" *) output FPD_CCI_NOC_1_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 AWADDR" *) output [63:0]FPD_CCI_NOC_1_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 AWBURST" *) output [1:0]FPD_CCI_NOC_1_awburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 AWCACHE" *) output [3:0]FPD_CCI_NOC_1_awcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 AWID" *) output [15:0]FPD_CCI_NOC_1_awid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 AWLEN" *) output [7:0]FPD_CCI_NOC_1_awlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 AWLOCK" *) output FPD_CCI_NOC_1_awlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 AWPROT" *) output [2:0]FPD_CCI_NOC_1_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 AWQOS" *) output [3:0]FPD_CCI_NOC_1_awqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 AWREADY" *) input FPD_CCI_NOC_1_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 AWSIZE" *) output [2:0]FPD_CCI_NOC_1_awsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 AWUSER" *) output [17:0]FPD_CCI_NOC_1_awuser;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 AWVALID" *) output FPD_CCI_NOC_1_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 BID" *) input [15:0]FPD_CCI_NOC_1_bid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 BREADY" *) output FPD_CCI_NOC_1_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 BRESP" *) input [1:0]FPD_CCI_NOC_1_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 BVALID" *) input FPD_CCI_NOC_1_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 RDATA" *) input [127:0]FPD_CCI_NOC_1_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 RID" *) input [15:0]FPD_CCI_NOC_1_rid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 RLAST" *) input FPD_CCI_NOC_1_rlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 RREADY" *) output FPD_CCI_NOC_1_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 RRESP" *) input [1:0]FPD_CCI_NOC_1_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 RVALID" *) input FPD_CCI_NOC_1_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 WDATA" *) output [127:0]FPD_CCI_NOC_1_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 WLAST" *) output FPD_CCI_NOC_1_wlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 WREADY" *) input FPD_CCI_NOC_1_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 WSTRB" *) output [15:0]FPD_CCI_NOC_1_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 WUSER" *) output [16:0]FPD_CCI_NOC_1_wuser;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_1 WVALID" *) output FPD_CCI_NOC_1_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 ARADDR" *) (* X_INTERFACE_MODE = "Master" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME FPD_CCI_NOC_2, ADDR_WIDTH 64, ARUSER_WIDTH 18, AWUSER_WIDTH 18, BUSER_WIDTH 0, CATEGORY noc, CLK_DOMAIN bd_1baa_pspmc_0_0_fpd_cci_noc_axi2_clk, DATA_WIDTH 128, FREQ_HZ 824991760, HAS_BRESP 1, HAS_BURST 1, HAS_CACHE 1, HAS_LOCK 1, HAS_PROT 1, HAS_QOS 1, HAS_REGION 0, HAS_RRESP 1, HAS_WSTRB 1, ID_WIDTH 16, INDEX 2, INSERT_VIP 0, MAX_BURST_LENGTH 256, MY_CATEGORY ps_cci, NUM_READ_OUTSTANDING 64, NUM_READ_THREADS 1, NUM_WRITE_OUTSTANDING 64, NUM_WRITE_THREADS 1, PHASE 0.0, PHYSICAL_CHANNEL PS_CCI_TO_NOC_NMU, PROTOCOL AXI4, READ_WRITE_MODE READ_WRITE, RUSER_BITS_PER_BYTE 0, RUSER_WIDTH 0, SUPPORTS_NARROW_BURST 1, WUSER_BITS_PER_BYTE 0, WUSER_WIDTH 17" *) output [63:0]FPD_CCI_NOC_2_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 ARBURST" *) output [1:0]FPD_CCI_NOC_2_arburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 ARCACHE" *) output [3:0]FPD_CCI_NOC_2_arcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 ARID" *) output [15:0]FPD_CCI_NOC_2_arid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 ARLEN" *) output [7:0]FPD_CCI_NOC_2_arlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 ARLOCK" *) output FPD_CCI_NOC_2_arlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 ARPROT" *) output [2:0]FPD_CCI_NOC_2_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 ARQOS" *) output [3:0]FPD_CCI_NOC_2_arqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 ARREADY" *) input FPD_CCI_NOC_2_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 ARSIZE" *) output [2:0]FPD_CCI_NOC_2_arsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 ARUSER" *) output [17:0]FPD_CCI_NOC_2_aruser;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 ARVALID" *) output FPD_CCI_NOC_2_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 AWADDR" *) output [63:0]FPD_CCI_NOC_2_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 AWBURST" *) output [1:0]FPD_CCI_NOC_2_awburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 AWCACHE" *) output [3:0]FPD_CCI_NOC_2_awcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 AWID" *) output [15:0]FPD_CCI_NOC_2_awid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 AWLEN" *) output [7:0]FPD_CCI_NOC_2_awlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 AWLOCK" *) output FPD_CCI_NOC_2_awlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 AWPROT" *) output [2:0]FPD_CCI_NOC_2_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 AWQOS" *) output [3:0]FPD_CCI_NOC_2_awqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 AWREADY" *) input FPD_CCI_NOC_2_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 AWSIZE" *) output [2:0]FPD_CCI_NOC_2_awsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 AWUSER" *) output [17:0]FPD_CCI_NOC_2_awuser;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 AWVALID" *) output FPD_CCI_NOC_2_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 BID" *) input [15:0]FPD_CCI_NOC_2_bid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 BREADY" *) output FPD_CCI_NOC_2_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 BRESP" *) input [1:0]FPD_CCI_NOC_2_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 BVALID" *) input FPD_CCI_NOC_2_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 RDATA" *) input [127:0]FPD_CCI_NOC_2_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 RID" *) input [15:0]FPD_CCI_NOC_2_rid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 RLAST" *) input FPD_CCI_NOC_2_rlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 RREADY" *) output FPD_CCI_NOC_2_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 RRESP" *) input [1:0]FPD_CCI_NOC_2_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 RVALID" *) input FPD_CCI_NOC_2_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 WDATA" *) output [127:0]FPD_CCI_NOC_2_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 WLAST" *) output FPD_CCI_NOC_2_wlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 WREADY" *) input FPD_CCI_NOC_2_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 WSTRB" *) output [15:0]FPD_CCI_NOC_2_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 WUSER" *) output [16:0]FPD_CCI_NOC_2_wuser;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_2 WVALID" *) output FPD_CCI_NOC_2_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 ARADDR" *) (* X_INTERFACE_MODE = "Master" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME FPD_CCI_NOC_3, ADDR_WIDTH 64, ARUSER_WIDTH 18, AWUSER_WIDTH 18, BUSER_WIDTH 0, CATEGORY noc, CLK_DOMAIN bd_1baa_pspmc_0_0_fpd_cci_noc_axi3_clk, DATA_WIDTH 128, FREQ_HZ 824991760, HAS_BRESP 1, HAS_BURST 1, HAS_CACHE 1, HAS_LOCK 1, HAS_PROT 1, HAS_QOS 1, HAS_REGION 0, HAS_RRESP 1, HAS_WSTRB 1, ID_WIDTH 16, INDEX 3, INSERT_VIP 0, MAX_BURST_LENGTH 256, MY_CATEGORY ps_cci, NUM_READ_OUTSTANDING 64, NUM_READ_THREADS 1, NUM_WRITE_OUTSTANDING 64, NUM_WRITE_THREADS 1, PHASE 0.0, PHYSICAL_CHANNEL PS_CCI_TO_NOC_NMU, PROTOCOL AXI4, READ_WRITE_MODE READ_WRITE, RUSER_BITS_PER_BYTE 0, RUSER_WIDTH 0, SUPPORTS_NARROW_BURST 1, WUSER_BITS_PER_BYTE 0, WUSER_WIDTH 17" *) output [63:0]FPD_CCI_NOC_3_araddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 ARBURST" *) output [1:0]FPD_CCI_NOC_3_arburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 ARCACHE" *) output [3:0]FPD_CCI_NOC_3_arcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 ARID" *) output [15:0]FPD_CCI_NOC_3_arid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 ARLEN" *) output [7:0]FPD_CCI_NOC_3_arlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 ARLOCK" *) output FPD_CCI_NOC_3_arlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 ARPROT" *) output [2:0]FPD_CCI_NOC_3_arprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 ARQOS" *) output [3:0]FPD_CCI_NOC_3_arqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 ARREADY" *) input FPD_CCI_NOC_3_arready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 ARSIZE" *) output [2:0]FPD_CCI_NOC_3_arsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 ARUSER" *) output [17:0]FPD_CCI_NOC_3_aruser;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 ARVALID" *) output FPD_CCI_NOC_3_arvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 AWADDR" *) output [63:0]FPD_CCI_NOC_3_awaddr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 AWBURST" *) output [1:0]FPD_CCI_NOC_3_awburst;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 AWCACHE" *) output [3:0]FPD_CCI_NOC_3_awcache;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 AWID" *) output [15:0]FPD_CCI_NOC_3_awid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 AWLEN" *) output [7:0]FPD_CCI_NOC_3_awlen;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 AWLOCK" *) output FPD_CCI_NOC_3_awlock;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 AWPROT" *) output [2:0]FPD_CCI_NOC_3_awprot;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 AWQOS" *) output [3:0]FPD_CCI_NOC_3_awqos;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 AWREADY" *) input FPD_CCI_NOC_3_awready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 AWSIZE" *) output [2:0]FPD_CCI_NOC_3_awsize;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 AWUSER" *) output [17:0]FPD_CCI_NOC_3_awuser;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 AWVALID" *) output FPD_CCI_NOC_3_awvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 BID" *) input [15:0]FPD_CCI_NOC_3_bid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 BREADY" *) output FPD_CCI_NOC_3_bready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 BRESP" *) input [1:0]FPD_CCI_NOC_3_bresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 BVALID" *) input FPD_CCI_NOC_3_bvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 RDATA" *) input [127:0]FPD_CCI_NOC_3_rdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 RID" *) input [15:0]FPD_CCI_NOC_3_rid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 RLAST" *) input FPD_CCI_NOC_3_rlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 RREADY" *) output FPD_CCI_NOC_3_rready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 RRESP" *) input [1:0]FPD_CCI_NOC_3_rresp;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 RVALID" *) input FPD_CCI_NOC_3_rvalid;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 WDATA" *) output [127:0]FPD_CCI_NOC_3_wdata;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 WLAST" *) output FPD_CCI_NOC_3_wlast;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 WREADY" *) input FPD_CCI_NOC_3_wready;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 WSTRB" *) output [15:0]FPD_CCI_NOC_3_wstrb;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 WUSER" *) output [16:0]FPD_CCI_NOC_3_wuser;
  (* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 FPD_CCI_NOC_3 WVALID" *) output FPD_CCI_NOC_3_wvalid;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.FPD_CCI_NOC_AXI0_CLK CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.FPD_CCI_NOC_AXI0_CLK, ASSOCIATED_BUSIF FPD_CCI_NOC_0, CLK_DOMAIN bd_1baa_pspmc_0_0_fpd_cci_noc_axi0_clk, FREQ_HZ 824991760, FREQ_TOLERANCE_HZ 0, INSERT_VIP 0, PHASE 0.0, PHYSICAL_CHANNEL PS_CCI_TO_NOC_NMU" *) output fpd_cci_noc_axi0_clk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.FPD_CCI_NOC_AXI1_CLK CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.FPD_CCI_NOC_AXI1_CLK, ASSOCIATED_BUSIF FPD_CCI_NOC_1, CLK_DOMAIN bd_1baa_pspmc_0_0_fpd_cci_noc_axi1_clk, FREQ_HZ 824991760, FREQ_TOLERANCE_HZ 0, INSERT_VIP 0, PHASE 0.0, PHYSICAL_CHANNEL PS_CCI_TO_NOC_NMU" *) output fpd_cci_noc_axi1_clk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.FPD_CCI_NOC_AXI2_CLK CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.FPD_CCI_NOC_AXI2_CLK, ASSOCIATED_BUSIF FPD_CCI_NOC_2, CLK_DOMAIN bd_1baa_pspmc_0_0_fpd_cci_noc_axi2_clk, FREQ_HZ 824991760, FREQ_TOLERANCE_HZ 0, INSERT_VIP 0, PHASE 0.0, PHYSICAL_CHANNEL PS_CCI_TO_NOC_NMU" *) output fpd_cci_noc_axi2_clk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.FPD_CCI_NOC_AXI3_CLK CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.FPD_CCI_NOC_AXI3_CLK, ASSOCIATED_BUSIF FPD_CCI_NOC_3, CLK_DOMAIN bd_1baa_pspmc_0_0_fpd_cci_noc_axi3_clk, FREQ_HZ 824991760, FREQ_TOLERANCE_HZ 0, INSERT_VIP 0, PHASE 0.0, PHYSICAL_CHANNEL PS_CCI_TO_NOC_NMU" *) output fpd_cci_noc_axi3_clk;
  (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 CLK.PL0_REF_CLK CLK" *) (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME CLK.PL0_REF_CLK, CLK_DOMAIN bd_1baa_pspmc_0_0_pl0_ref_clk, FREQ_HZ 333329987, FREQ_TOLERANCE_HZ 0, INSERT_VIP 0, PHASE 0.0" *) output pl0_ref_clk;

  (* HARD_CONN = "true" *) wire [63:0]FPD_CCI_NOC_0_araddr;
  (* HARD_CONN = "true" *) wire [1:0]FPD_CCI_NOC_0_arburst;
  (* HARD_CONN = "true" *) wire [3:0]FPD_CCI_NOC_0_arcache;
  (* HARD_CONN = "true" *) wire [15:0]FPD_CCI_NOC_0_arid;
  (* HARD_CONN = "true" *) wire [7:0]FPD_CCI_NOC_0_arlen;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_0_arlock;
  (* HARD_CONN = "true" *) wire [2:0]FPD_CCI_NOC_0_arprot;
  (* HARD_CONN = "true" *) wire [3:0]FPD_CCI_NOC_0_arqos;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_0_arready;
  (* HARD_CONN = "true" *) wire [2:0]FPD_CCI_NOC_0_arsize;
  (* HARD_CONN = "true" *) wire [17:0]FPD_CCI_NOC_0_aruser;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_0_arvalid;
  (* HARD_CONN = "true" *) wire [63:0]FPD_CCI_NOC_0_awaddr;
  (* HARD_CONN = "true" *) wire [1:0]FPD_CCI_NOC_0_awburst;
  (* HARD_CONN = "true" *) wire [3:0]FPD_CCI_NOC_0_awcache;
  (* HARD_CONN = "true" *) wire [15:0]FPD_CCI_NOC_0_awid;
  (* HARD_CONN = "true" *) wire [7:0]FPD_CCI_NOC_0_awlen;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_0_awlock;
  (* HARD_CONN = "true" *) wire [2:0]FPD_CCI_NOC_0_awprot;
  (* HARD_CONN = "true" *) wire [3:0]FPD_CCI_NOC_0_awqos;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_0_awready;
  (* HARD_CONN = "true" *) wire [2:0]FPD_CCI_NOC_0_awsize;
  (* HARD_CONN = "true" *) wire [17:0]FPD_CCI_NOC_0_awuser;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_0_awvalid;
  (* HARD_CONN = "true" *) wire [15:0]FPD_CCI_NOC_0_bid;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_0_bready;
  (* HARD_CONN = "true" *) wire [1:0]FPD_CCI_NOC_0_bresp;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_0_bvalid;
  (* HARD_CONN = "true" *) wire [127:0]FPD_CCI_NOC_0_rdata;
  (* HARD_CONN = "true" *) wire [15:0]FPD_CCI_NOC_0_rid;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_0_rlast;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_0_rready;
  (* HARD_CONN = "true" *) wire [1:0]FPD_CCI_NOC_0_rresp;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_0_rvalid;
  (* HARD_CONN = "true" *) wire [127:0]FPD_CCI_NOC_0_wdata;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_0_wlast;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_0_wready;
  (* HARD_CONN = "true" *) wire [15:0]FPD_CCI_NOC_0_wstrb;
  (* HARD_CONN = "true" *) wire [16:0]FPD_CCI_NOC_0_wuser;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_0_wvalid;
  (* HARD_CONN = "true" *) wire [63:0]FPD_CCI_NOC_1_araddr;
  (* HARD_CONN = "true" *) wire [1:0]FPD_CCI_NOC_1_arburst;
  (* HARD_CONN = "true" *) wire [3:0]FPD_CCI_NOC_1_arcache;
  (* HARD_CONN = "true" *) wire [15:0]FPD_CCI_NOC_1_arid;
  (* HARD_CONN = "true" *) wire [7:0]FPD_CCI_NOC_1_arlen;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_1_arlock;
  (* HARD_CONN = "true" *) wire [2:0]FPD_CCI_NOC_1_arprot;
  (* HARD_CONN = "true" *) wire [3:0]FPD_CCI_NOC_1_arqos;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_1_arready;
  (* HARD_CONN = "true" *) wire [2:0]FPD_CCI_NOC_1_arsize;
  (* HARD_CONN = "true" *) wire [17:0]FPD_CCI_NOC_1_aruser;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_1_arvalid;
  (* HARD_CONN = "true" *) wire [63:0]FPD_CCI_NOC_1_awaddr;
  (* HARD_CONN = "true" *) wire [1:0]FPD_CCI_NOC_1_awburst;
  (* HARD_CONN = "true" *) wire [3:0]FPD_CCI_NOC_1_awcache;
  (* HARD_CONN = "true" *) wire [15:0]FPD_CCI_NOC_1_awid;
  (* HARD_CONN = "true" *) wire [7:0]FPD_CCI_NOC_1_awlen;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_1_awlock;
  (* HARD_CONN = "true" *) wire [2:0]FPD_CCI_NOC_1_awprot;
  (* HARD_CONN = "true" *) wire [3:0]FPD_CCI_NOC_1_awqos;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_1_awready;
  (* HARD_CONN = "true" *) wire [2:0]FPD_CCI_NOC_1_awsize;
  (* HARD_CONN = "true" *) wire [17:0]FPD_CCI_NOC_1_awuser;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_1_awvalid;
  (* HARD_CONN = "true" *) wire [15:0]FPD_CCI_NOC_1_bid;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_1_bready;
  (* HARD_CONN = "true" *) wire [1:0]FPD_CCI_NOC_1_bresp;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_1_bvalid;
  (* HARD_CONN = "true" *) wire [127:0]FPD_CCI_NOC_1_rdata;
  (* HARD_CONN = "true" *) wire [15:0]FPD_CCI_NOC_1_rid;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_1_rlast;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_1_rready;
  (* HARD_CONN = "true" *) wire [1:0]FPD_CCI_NOC_1_rresp;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_1_rvalid;
  (* HARD_CONN = "true" *) wire [127:0]FPD_CCI_NOC_1_wdata;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_1_wlast;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_1_wready;
  (* HARD_CONN = "true" *) wire [15:0]FPD_CCI_NOC_1_wstrb;
  (* HARD_CONN = "true" *) wire [16:0]FPD_CCI_NOC_1_wuser;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_1_wvalid;
  (* HARD_CONN = "true" *) wire [63:0]FPD_CCI_NOC_2_araddr;
  (* HARD_CONN = "true" *) wire [1:0]FPD_CCI_NOC_2_arburst;
  (* HARD_CONN = "true" *) wire [3:0]FPD_CCI_NOC_2_arcache;
  (* HARD_CONN = "true" *) wire [15:0]FPD_CCI_NOC_2_arid;
  (* HARD_CONN = "true" *) wire [7:0]FPD_CCI_NOC_2_arlen;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_2_arlock;
  (* HARD_CONN = "true" *) wire [2:0]FPD_CCI_NOC_2_arprot;
  (* HARD_CONN = "true" *) wire [3:0]FPD_CCI_NOC_2_arqos;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_2_arready;
  (* HARD_CONN = "true" *) wire [2:0]FPD_CCI_NOC_2_arsize;
  (* HARD_CONN = "true" *) wire [17:0]FPD_CCI_NOC_2_aruser;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_2_arvalid;
  (* HARD_CONN = "true" *) wire [63:0]FPD_CCI_NOC_2_awaddr;
  (* HARD_CONN = "true" *) wire [1:0]FPD_CCI_NOC_2_awburst;
  (* HARD_CONN = "true" *) wire [3:0]FPD_CCI_NOC_2_awcache;
  (* HARD_CONN = "true" *) wire [15:0]FPD_CCI_NOC_2_awid;
  (* HARD_CONN = "true" *) wire [7:0]FPD_CCI_NOC_2_awlen;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_2_awlock;
  (* HARD_CONN = "true" *) wire [2:0]FPD_CCI_NOC_2_awprot;
  (* HARD_CONN = "true" *) wire [3:0]FPD_CCI_NOC_2_awqos;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_2_awready;
  (* HARD_CONN = "true" *) wire [2:0]FPD_CCI_NOC_2_awsize;
  (* HARD_CONN = "true" *) wire [17:0]FPD_CCI_NOC_2_awuser;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_2_awvalid;
  (* HARD_CONN = "true" *) wire [15:0]FPD_CCI_NOC_2_bid;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_2_bready;
  (* HARD_CONN = "true" *) wire [1:0]FPD_CCI_NOC_2_bresp;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_2_bvalid;
  (* HARD_CONN = "true" *) wire [127:0]FPD_CCI_NOC_2_rdata;
  (* HARD_CONN = "true" *) wire [15:0]FPD_CCI_NOC_2_rid;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_2_rlast;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_2_rready;
  (* HARD_CONN = "true" *) wire [1:0]FPD_CCI_NOC_2_rresp;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_2_rvalid;
  (* HARD_CONN = "true" *) wire [127:0]FPD_CCI_NOC_2_wdata;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_2_wlast;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_2_wready;
  (* HARD_CONN = "true" *) wire [15:0]FPD_CCI_NOC_2_wstrb;
  (* HARD_CONN = "true" *) wire [16:0]FPD_CCI_NOC_2_wuser;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_2_wvalid;
  (* HARD_CONN = "true" *) wire [63:0]FPD_CCI_NOC_3_araddr;
  (* HARD_CONN = "true" *) wire [1:0]FPD_CCI_NOC_3_arburst;
  (* HARD_CONN = "true" *) wire [3:0]FPD_CCI_NOC_3_arcache;
  (* HARD_CONN = "true" *) wire [15:0]FPD_CCI_NOC_3_arid;
  (* HARD_CONN = "true" *) wire [7:0]FPD_CCI_NOC_3_arlen;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_3_arlock;
  (* HARD_CONN = "true" *) wire [2:0]FPD_CCI_NOC_3_arprot;
  (* HARD_CONN = "true" *) wire [3:0]FPD_CCI_NOC_3_arqos;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_3_arready;
  (* HARD_CONN = "true" *) wire [2:0]FPD_CCI_NOC_3_arsize;
  (* HARD_CONN = "true" *) wire [17:0]FPD_CCI_NOC_3_aruser;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_3_arvalid;
  (* HARD_CONN = "true" *) wire [63:0]FPD_CCI_NOC_3_awaddr;
  (* HARD_CONN = "true" *) wire [1:0]FPD_CCI_NOC_3_awburst;
  (* HARD_CONN = "true" *) wire [3:0]FPD_CCI_NOC_3_awcache;
  (* HARD_CONN = "true" *) wire [15:0]FPD_CCI_NOC_3_awid;
  (* HARD_CONN = "true" *) wire [7:0]FPD_CCI_NOC_3_awlen;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_3_awlock;
  (* HARD_CONN = "true" *) wire [2:0]FPD_CCI_NOC_3_awprot;
  (* HARD_CONN = "true" *) wire [3:0]FPD_CCI_NOC_3_awqos;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_3_awready;
  (* HARD_CONN = "true" *) wire [2:0]FPD_CCI_NOC_3_awsize;
  (* HARD_CONN = "true" *) wire [17:0]FPD_CCI_NOC_3_awuser;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_3_awvalid;
  (* HARD_CONN = "true" *) wire [15:0]FPD_CCI_NOC_3_bid;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_3_bready;
  (* HARD_CONN = "true" *) wire [1:0]FPD_CCI_NOC_3_bresp;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_3_bvalid;
  (* HARD_CONN = "true" *) wire [127:0]FPD_CCI_NOC_3_rdata;
  (* HARD_CONN = "true" *) wire [15:0]FPD_CCI_NOC_3_rid;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_3_rlast;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_3_rready;
  (* HARD_CONN = "true" *) wire [1:0]FPD_CCI_NOC_3_rresp;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_3_rvalid;
  (* HARD_CONN = "true" *) wire [127:0]FPD_CCI_NOC_3_wdata;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_3_wlast;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_3_wready;
  (* HARD_CONN = "true" *) wire [15:0]FPD_CCI_NOC_3_wstrb;
  (* HARD_CONN = "true" *) wire [16:0]FPD_CCI_NOC_3_wuser;
  (* HARD_CONN = "true" *) wire FPD_CCI_NOC_3_wvalid;
  (* HARD_CONN = "true" *) wire fpd_cci_noc_axi0_clk;
  (* HARD_CONN = "true" *) wire fpd_cci_noc_axi1_clk;
  (* HARD_CONN = "true" *) wire fpd_cci_noc_axi2_clk;
  (* HARD_CONN = "true" *) wire fpd_cci_noc_axi3_clk;
  wire pl0_ref_clk;

  bd_1baa_pspmc_0_0 pspmc_0
       (.cpmps_corr_irq(1'b0),
        .cpmps_misc_irq(1'b0),
        .cpmps_uncorr_irq(1'b0),
        .fpd_cci_noc_axi0_araddr(FPD_CCI_NOC_0_araddr),
        .fpd_cci_noc_axi0_arburst(FPD_CCI_NOC_0_arburst),
        .fpd_cci_noc_axi0_arcache(FPD_CCI_NOC_0_arcache),
        .fpd_cci_noc_axi0_arid(FPD_CCI_NOC_0_arid),
        .fpd_cci_noc_axi0_arlen(FPD_CCI_NOC_0_arlen),
        .fpd_cci_noc_axi0_arlock(FPD_CCI_NOC_0_arlock),
        .fpd_cci_noc_axi0_arprot(FPD_CCI_NOC_0_arprot),
        .fpd_cci_noc_axi0_arqos(FPD_CCI_NOC_0_arqos),
        .fpd_cci_noc_axi0_arready(FPD_CCI_NOC_0_arready),
        .fpd_cci_noc_axi0_arsize(FPD_CCI_NOC_0_arsize),
        .fpd_cci_noc_axi0_aruser(FPD_CCI_NOC_0_aruser),
        .fpd_cci_noc_axi0_arvalid(FPD_CCI_NOC_0_arvalid),
        .fpd_cci_noc_axi0_awaddr(FPD_CCI_NOC_0_awaddr),
        .fpd_cci_noc_axi0_awburst(FPD_CCI_NOC_0_awburst),
        .fpd_cci_noc_axi0_awcache(FPD_CCI_NOC_0_awcache),
        .fpd_cci_noc_axi0_awid(FPD_CCI_NOC_0_awid),
        .fpd_cci_noc_axi0_awlen(FPD_CCI_NOC_0_awlen),
        .fpd_cci_noc_axi0_awlock(FPD_CCI_NOC_0_awlock),
        .fpd_cci_noc_axi0_awprot(FPD_CCI_NOC_0_awprot),
        .fpd_cci_noc_axi0_awqos(FPD_CCI_NOC_0_awqos),
        .fpd_cci_noc_axi0_awready(FPD_CCI_NOC_0_awready),
        .fpd_cci_noc_axi0_awsize(FPD_CCI_NOC_0_awsize),
        .fpd_cci_noc_axi0_awuser(FPD_CCI_NOC_0_awuser),
        .fpd_cci_noc_axi0_awvalid(FPD_CCI_NOC_0_awvalid),
        .fpd_cci_noc_axi0_bid(FPD_CCI_NOC_0_bid),
        .fpd_cci_noc_axi0_bready(FPD_CCI_NOC_0_bready),
        .fpd_cci_noc_axi0_bresp(FPD_CCI_NOC_0_bresp),
        .fpd_cci_noc_axi0_bvalid(FPD_CCI_NOC_0_bvalid),
        .fpd_cci_noc_axi0_clk(fpd_cci_noc_axi0_clk),
        .fpd_cci_noc_axi0_rdata(FPD_CCI_NOC_0_rdata),
        .fpd_cci_noc_axi0_rid(FPD_CCI_NOC_0_rid),
        .fpd_cci_noc_axi0_rlast(FPD_CCI_NOC_0_rlast),
        .fpd_cci_noc_axi0_rready(FPD_CCI_NOC_0_rready),
        .fpd_cci_noc_axi0_rresp(FPD_CCI_NOC_0_rresp),
        .fpd_cci_noc_axi0_rvalid(FPD_CCI_NOC_0_rvalid),
        .fpd_cci_noc_axi0_wdata(FPD_CCI_NOC_0_wdata),
        .fpd_cci_noc_axi0_wlast(FPD_CCI_NOC_0_wlast),
        .fpd_cci_noc_axi0_wready(FPD_CCI_NOC_0_wready),
        .fpd_cci_noc_axi0_wstrb(FPD_CCI_NOC_0_wstrb),
        .fpd_cci_noc_axi0_wuser(FPD_CCI_NOC_0_wuser),
        .fpd_cci_noc_axi0_wvalid(FPD_CCI_NOC_0_wvalid),
        .fpd_cci_noc_axi1_araddr(FPD_CCI_NOC_1_araddr),
        .fpd_cci_noc_axi1_arburst(FPD_CCI_NOC_1_arburst),
        .fpd_cci_noc_axi1_arcache(FPD_CCI_NOC_1_arcache),
        .fpd_cci_noc_axi1_arid(FPD_CCI_NOC_1_arid),
        .fpd_cci_noc_axi1_arlen(FPD_CCI_NOC_1_arlen),
        .fpd_cci_noc_axi1_arlock(FPD_CCI_NOC_1_arlock),
        .fpd_cci_noc_axi1_arprot(FPD_CCI_NOC_1_arprot),
        .fpd_cci_noc_axi1_arqos(FPD_CCI_NOC_1_arqos),
        .fpd_cci_noc_axi1_arready(FPD_CCI_NOC_1_arready),
        .fpd_cci_noc_axi1_arsize(FPD_CCI_NOC_1_arsize),
        .fpd_cci_noc_axi1_aruser(FPD_CCI_NOC_1_aruser),
        .fpd_cci_noc_axi1_arvalid(FPD_CCI_NOC_1_arvalid),
        .fpd_cci_noc_axi1_awaddr(FPD_CCI_NOC_1_awaddr),
        .fpd_cci_noc_axi1_awburst(FPD_CCI_NOC_1_awburst),
        .fpd_cci_noc_axi1_awcache(FPD_CCI_NOC_1_awcache),
        .fpd_cci_noc_axi1_awid(FPD_CCI_NOC_1_awid),
        .fpd_cci_noc_axi1_awlen(FPD_CCI_NOC_1_awlen),
        .fpd_cci_noc_axi1_awlock(FPD_CCI_NOC_1_awlock),
        .fpd_cci_noc_axi1_awprot(FPD_CCI_NOC_1_awprot),
        .fpd_cci_noc_axi1_awqos(FPD_CCI_NOC_1_awqos),
        .fpd_cci_noc_axi1_awready(FPD_CCI_NOC_1_awready),
        .fpd_cci_noc_axi1_awsize(FPD_CCI_NOC_1_awsize),
        .fpd_cci_noc_axi1_awuser(FPD_CCI_NOC_1_awuser),
        .fpd_cci_noc_axi1_awvalid(FPD_CCI_NOC_1_awvalid),
        .fpd_cci_noc_axi1_bid(FPD_CCI_NOC_1_bid),
        .fpd_cci_noc_axi1_bready(FPD_CCI_NOC_1_bready),
        .fpd_cci_noc_axi1_bresp(FPD_CCI_NOC_1_bresp),
        .fpd_cci_noc_axi1_bvalid(FPD_CCI_NOC_1_bvalid),
        .fpd_cci_noc_axi1_clk(fpd_cci_noc_axi1_clk),
        .fpd_cci_noc_axi1_rdata(FPD_CCI_NOC_1_rdata),
        .fpd_cci_noc_axi1_rid(FPD_CCI_NOC_1_rid),
        .fpd_cci_noc_axi1_rlast(FPD_CCI_NOC_1_rlast),
        .fpd_cci_noc_axi1_rready(FPD_CCI_NOC_1_rready),
        .fpd_cci_noc_axi1_rresp(FPD_CCI_NOC_1_rresp),
        .fpd_cci_noc_axi1_rvalid(FPD_CCI_NOC_1_rvalid),
        .fpd_cci_noc_axi1_wdata(FPD_CCI_NOC_1_wdata),
        .fpd_cci_noc_axi1_wlast(FPD_CCI_NOC_1_wlast),
        .fpd_cci_noc_axi1_wready(FPD_CCI_NOC_1_wready),
        .fpd_cci_noc_axi1_wstrb(FPD_CCI_NOC_1_wstrb),
        .fpd_cci_noc_axi1_wuser(FPD_CCI_NOC_1_wuser),
        .fpd_cci_noc_axi1_wvalid(FPD_CCI_NOC_1_wvalid),
        .fpd_cci_noc_axi2_araddr(FPD_CCI_NOC_2_araddr),
        .fpd_cci_noc_axi2_arburst(FPD_CCI_NOC_2_arburst),
        .fpd_cci_noc_axi2_arcache(FPD_CCI_NOC_2_arcache),
        .fpd_cci_noc_axi2_arid(FPD_CCI_NOC_2_arid),
        .fpd_cci_noc_axi2_arlen(FPD_CCI_NOC_2_arlen),
        .fpd_cci_noc_axi2_arlock(FPD_CCI_NOC_2_arlock),
        .fpd_cci_noc_axi2_arprot(FPD_CCI_NOC_2_arprot),
        .fpd_cci_noc_axi2_arqos(FPD_CCI_NOC_2_arqos),
        .fpd_cci_noc_axi2_arready(FPD_CCI_NOC_2_arready),
        .fpd_cci_noc_axi2_arsize(FPD_CCI_NOC_2_arsize),
        .fpd_cci_noc_axi2_aruser(FPD_CCI_NOC_2_aruser),
        .fpd_cci_noc_axi2_arvalid(FPD_CCI_NOC_2_arvalid),
        .fpd_cci_noc_axi2_awaddr(FPD_CCI_NOC_2_awaddr),
        .fpd_cci_noc_axi2_awburst(FPD_CCI_NOC_2_awburst),
        .fpd_cci_noc_axi2_awcache(FPD_CCI_NOC_2_awcache),
        .fpd_cci_noc_axi2_awid(FPD_CCI_NOC_2_awid),
        .fpd_cci_noc_axi2_awlen(FPD_CCI_NOC_2_awlen),
        .fpd_cci_noc_axi2_awlock(FPD_CCI_NOC_2_awlock),
        .fpd_cci_noc_axi2_awprot(FPD_CCI_NOC_2_awprot),
        .fpd_cci_noc_axi2_awqos(FPD_CCI_NOC_2_awqos),
        .fpd_cci_noc_axi2_awready(FPD_CCI_NOC_2_awready),
        .fpd_cci_noc_axi2_awsize(FPD_CCI_NOC_2_awsize),
        .fpd_cci_noc_axi2_awuser(FPD_CCI_NOC_2_awuser),
        .fpd_cci_noc_axi2_awvalid(FPD_CCI_NOC_2_awvalid),
        .fpd_cci_noc_axi2_bid(FPD_CCI_NOC_2_bid),
        .fpd_cci_noc_axi2_bready(FPD_CCI_NOC_2_bready),
        .fpd_cci_noc_axi2_bresp(FPD_CCI_NOC_2_bresp),
        .fpd_cci_noc_axi2_bvalid(FPD_CCI_NOC_2_bvalid),
        .fpd_cci_noc_axi2_clk(fpd_cci_noc_axi2_clk),
        .fpd_cci_noc_axi2_rdata(FPD_CCI_NOC_2_rdata),
        .fpd_cci_noc_axi2_rid(FPD_CCI_NOC_2_rid),
        .fpd_cci_noc_axi2_rlast(FPD_CCI_NOC_2_rlast),
        .fpd_cci_noc_axi2_rready(FPD_CCI_NOC_2_rready),
        .fpd_cci_noc_axi2_rresp(FPD_CCI_NOC_2_rresp),
        .fpd_cci_noc_axi2_rvalid(FPD_CCI_NOC_2_rvalid),
        .fpd_cci_noc_axi2_wdata(FPD_CCI_NOC_2_wdata),
        .fpd_cci_noc_axi2_wlast(FPD_CCI_NOC_2_wlast),
        .fpd_cci_noc_axi2_wready(FPD_CCI_NOC_2_wready),
        .fpd_cci_noc_axi2_wstrb(FPD_CCI_NOC_2_wstrb),
        .fpd_cci_noc_axi2_wuser(FPD_CCI_NOC_2_wuser),
        .fpd_cci_noc_axi2_wvalid(FPD_CCI_NOC_2_wvalid),
        .fpd_cci_noc_axi3_araddr(FPD_CCI_NOC_3_araddr),
        .fpd_cci_noc_axi3_arburst(FPD_CCI_NOC_3_arburst),
        .fpd_cci_noc_axi3_arcache(FPD_CCI_NOC_3_arcache),
        .fpd_cci_noc_axi3_arid(FPD_CCI_NOC_3_arid),
        .fpd_cci_noc_axi3_arlen(FPD_CCI_NOC_3_arlen),
        .fpd_cci_noc_axi3_arlock(FPD_CCI_NOC_3_arlock),
        .fpd_cci_noc_axi3_arprot(FPD_CCI_NOC_3_arprot),
        .fpd_cci_noc_axi3_arqos(FPD_CCI_NOC_3_arqos),
        .fpd_cci_noc_axi3_arready(FPD_CCI_NOC_3_arready),
        .fpd_cci_noc_axi3_arsize(FPD_CCI_NOC_3_arsize),
        .fpd_cci_noc_axi3_aruser(FPD_CCI_NOC_3_aruser),
        .fpd_cci_noc_axi3_arvalid(FPD_CCI_NOC_3_arvalid),
        .fpd_cci_noc_axi3_awaddr(FPD_CCI_NOC_3_awaddr),
        .fpd_cci_noc_axi3_awburst(FPD_CCI_NOC_3_awburst),
        .fpd_cci_noc_axi3_awcache(FPD_CCI_NOC_3_awcache),
        .fpd_cci_noc_axi3_awid(FPD_CCI_NOC_3_awid),
        .fpd_cci_noc_axi3_awlen(FPD_CCI_NOC_3_awlen),
        .fpd_cci_noc_axi3_awlock(FPD_CCI_NOC_3_awlock),
        .fpd_cci_noc_axi3_awprot(FPD_CCI_NOC_3_awprot),
        .fpd_cci_noc_axi3_awqos(FPD_CCI_NOC_3_awqos),
        .fpd_cci_noc_axi3_awready(FPD_CCI_NOC_3_awready),
        .fpd_cci_noc_axi3_awsize(FPD_CCI_NOC_3_awsize),
        .fpd_cci_noc_axi3_awuser(FPD_CCI_NOC_3_awuser),
        .fpd_cci_noc_axi3_awvalid(FPD_CCI_NOC_3_awvalid),
        .fpd_cci_noc_axi3_bid(FPD_CCI_NOC_3_bid),
        .fpd_cci_noc_axi3_bready(FPD_CCI_NOC_3_bready),
        .fpd_cci_noc_axi3_bresp(FPD_CCI_NOC_3_bresp),
        .fpd_cci_noc_axi3_bvalid(FPD_CCI_NOC_3_bvalid),
        .fpd_cci_noc_axi3_clk(fpd_cci_noc_axi3_clk),
        .fpd_cci_noc_axi3_rdata(FPD_CCI_NOC_3_rdata),
        .fpd_cci_noc_axi3_rid(FPD_CCI_NOC_3_rid),
        .fpd_cci_noc_axi3_rlast(FPD_CCI_NOC_3_rlast),
        .fpd_cci_noc_axi3_rready(FPD_CCI_NOC_3_rready),
        .fpd_cci_noc_axi3_rresp(FPD_CCI_NOC_3_rresp),
        .fpd_cci_noc_axi3_rvalid(FPD_CCI_NOC_3_rvalid),
        .fpd_cci_noc_axi3_wdata(FPD_CCI_NOC_3_wdata),
        .fpd_cci_noc_axi3_wlast(FPD_CCI_NOC_3_wlast),
        .fpd_cci_noc_axi3_wready(FPD_CCI_NOC_3_wready),
        .fpd_cci_noc_axi3_wstrb(FPD_CCI_NOC_3_wstrb),
        .fpd_cci_noc_axi3_wuser(FPD_CCI_NOC_3_wuser),
        .fpd_cci_noc_axi3_wvalid(FPD_CCI_NOC_3_wvalid),
        .hsdp_gt_rxoutclk(1'b0),
        .hsdp_xpipe0_rx_datavalid(1'b0),
        .hsdp_xpipe0_rx_header({1'b0,1'b0}),
        .hsdp_xpipe0_rx_headervalid(1'b0),
        .hsdp_xpipe0_rx_resetdone(1'b0),
        .hsdp_xpipe0_tx_resetdone(1'b0),
        .hsdp_xpipe1_rx_datavalid(1'b0),
        .hsdp_xpipe1_rx_header({1'b0,1'b0}),
        .hsdp_xpipe1_rx_headervalid(1'b0),
        .hsdp_xpipe1_rx_resetdone(1'b0),
        .hsdp_xpipe1_tx_resetdone(1'b0),
        .hsdp_xpipe2_rx_datavalid(1'b0),
        .hsdp_xpipe2_rx_header({1'b0,1'b0}),
        .hsdp_xpipe2_rx_headervalid(1'b0),
        .hsdp_xpipe2_rx_resetdone(1'b0),
        .hsdp_xpipe2_tx_resetdone(1'b0),
        .link0_xpipe_bufgtce(1'b0),
        .link0_xpipe_bufgtcemask({1'b0,1'b0,1'b0,1'b0}),
        .link0_xpipe_bufgtdiv({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .link0_xpipe_bufgtrst(1'b0),
        .link0_xpipe_bufgtrstmask({1'b0,1'b0,1'b0,1'b0}),
        .link0_xpipe_gtoutclk(1'b0),
        .link0_xpipe_phyready(1'b0),
        .link1_xpipe_bufgtce(1'b0),
        .link1_xpipe_bufgtcemask({1'b0,1'b0,1'b0,1'b0}),
        .link1_xpipe_bufgtdiv({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .link1_xpipe_bufgtrst(1'b0),
        .link1_xpipe_bufgtrstmask({1'b0,1'b0,1'b0,1'b0}),
        .link1_xpipe_gtoutclk(1'b0),
        .link1_xpipe_phyready(1'b0),
        .m_axi0_ps_araddr({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi0_ps_arburst({1'b0,1'b0}),
        .m_axi0_ps_arcache({1'b0,1'b0,1'b0,1'b0}),
        .m_axi0_ps_arid({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi0_ps_arlen({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi0_ps_arlock(1'b0),
        .m_axi0_ps_arprot({1'b0,1'b0,1'b0}),
        .m_axi0_ps_arqos({1'b0,1'b0,1'b0,1'b0}),
        .m_axi0_ps_arregion({1'b0,1'b0,1'b0,1'b0}),
        .m_axi0_ps_arsize({1'b0,1'b0,1'b0}),
        .m_axi0_ps_aruser({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi0_ps_arvalid(1'b0),
        .m_axi0_ps_awaddr({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi0_ps_awburst({1'b0,1'b0}),
        .m_axi0_ps_awcache({1'b0,1'b0,1'b0,1'b0}),
        .m_axi0_ps_awid({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi0_ps_awlen({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi0_ps_awlock(1'b0),
        .m_axi0_ps_awprot({1'b0,1'b0,1'b0}),
        .m_axi0_ps_awqos({1'b0,1'b0,1'b0,1'b0}),
        .m_axi0_ps_awregion({1'b0,1'b0,1'b0,1'b0}),
        .m_axi0_ps_awsize({1'b0,1'b0,1'b0}),
        .m_axi0_ps_awuser({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi0_ps_awvalid(1'b0),
        .m_axi0_ps_bready(1'b0),
        .m_axi0_ps_rready(1'b0),
        .m_axi0_ps_wdata({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi0_ps_wid({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi0_ps_wlast(1'b0),
        .m_axi0_ps_wstrb({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi0_ps_wuser({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi0_ps_wvalid(1'b0),
        .m_axi1_ps_araddr({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi1_ps_arburst({1'b0,1'b0}),
        .m_axi1_ps_arcache({1'b0,1'b0,1'b0,1'b0}),
        .m_axi1_ps_arid({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi1_ps_arlen({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi1_ps_arlock(1'b0),
        .m_axi1_ps_arprot({1'b0,1'b0,1'b0}),
        .m_axi1_ps_arqos({1'b0,1'b0,1'b0,1'b0}),
        .m_axi1_ps_arregion({1'b0,1'b0,1'b0,1'b0}),
        .m_axi1_ps_arsize({1'b0,1'b0,1'b0}),
        .m_axi1_ps_aruser({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi1_ps_arvalid(1'b0),
        .m_axi1_ps_awaddr({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi1_ps_awburst({1'b0,1'b0}),
        .m_axi1_ps_awcache({1'b0,1'b0,1'b0,1'b0}),
        .m_axi1_ps_awid({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi1_ps_awlen({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi1_ps_awlock(1'b0),
        .m_axi1_ps_awprot({1'b0,1'b0,1'b0}),
        .m_axi1_ps_awqos({1'b0,1'b0,1'b0,1'b0}),
        .m_axi1_ps_awregion({1'b0,1'b0,1'b0,1'b0}),
        .m_axi1_ps_awsize({1'b0,1'b0,1'b0}),
        .m_axi1_ps_awuser({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi1_ps_awvalid(1'b0),
        .m_axi1_ps_bready(1'b0),
        .m_axi1_ps_rready(1'b0),
        .m_axi1_ps_wdata({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi1_ps_wid({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi1_ps_wlast(1'b0),
        .m_axi1_ps_wstrb({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi1_ps_wuser({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .m_axi1_ps_wvalid(1'b0),
        .phy_ready_tobot(1'b0),
        .pl0_ref_clk(pl0_ref_clk),
        .s_axi_cfg_arready(1'b0),
        .s_axi_cfg_awready(1'b0),
        .s_axi_cfg_bid({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_cfg_bresp({1'b0,1'b0}),
        .s_axi_cfg_buser(1'b0),
        .s_axi_cfg_bvalid(1'b0),
        .s_axi_cfg_rdata({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_cfg_rid({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_cfg_rlast(1'b0),
        .s_axi_cfg_rresp({1'b0,1'b0}),
        .s_axi_cfg_ruser({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_cfg_rvalid(1'b0),
        .s_axi_cfg_wready(1'b0),
        .s_axi_pcie_arready(1'b0),
        .s_axi_pcie_awready(1'b0),
        .s_axi_pcie_bid({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_pcie_bresp({1'b0,1'b0}),
        .s_axi_pcie_buser({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_pcie_bvalid(1'b0),
        .s_axi_pcie_rdata({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_pcie_rid({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_pcie_rlast(1'b0),
        .s_axi_pcie_rresp({1'b0,1'b0}),
        .s_axi_pcie_ruser({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .s_axi_pcie_rvalid(1'b0),
        .s_axi_pcie_wready(1'b0),
        .xpipe0_rx_charisk({1'b0,1'b0}),
        .xpipe0_rx_data({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .xpipe0_rx_datavalid(1'b0),
        .xpipe0_rx_elecidle(1'b0),
        .xpipe0_rx_startblock(1'b0),
        .xpipe0_rx_status({1'b0,1'b0,1'b0}),
        .xpipe0_rx_syncheader({1'b0,1'b0}),
        .xpipe0_rx_valid(1'b0),
        .xpipe10_phystatus(1'b0),
        .xpipe10_rx_charisk({1'b0,1'b0}),
        .xpipe10_rx_data({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .xpipe10_rx_datavalid(1'b0),
        .xpipe10_rx_elecidle(1'b0),
        .xpipe10_rx_startblock(1'b0),
        .xpipe10_rx_status({1'b0,1'b0,1'b0}),
        .xpipe10_rx_syncheader({1'b0,1'b0}),
        .xpipe10_rx_valid(1'b0),
        .xpipe11_phystatus(1'b0),
        .xpipe11_rx_charisk({1'b0,1'b0}),
        .xpipe11_rx_data({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .xpipe11_rx_datavalid(1'b0),
        .xpipe11_rx_elecidle(1'b0),
        .xpipe11_rx_startblock(1'b0),
        .xpipe11_rx_status({1'b0,1'b0,1'b0}),
        .xpipe11_rx_syncheader({1'b0,1'b0}),
        .xpipe11_rx_valid(1'b0),
        .xpipe12_phystatus(1'b0),
        .xpipe12_rx_charisk({1'b0,1'b0}),
        .xpipe12_rx_data({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .xpipe12_rx_datavalid(1'b0),
        .xpipe12_rx_elecidle(1'b0),
        .xpipe12_rx_startblock(1'b0),
        .xpipe12_rx_status({1'b0,1'b0,1'b0}),
        .xpipe12_rx_syncheader({1'b0,1'b0}),
        .xpipe12_rx_valid(1'b0),
        .xpipe13_phystatus(1'b0),
        .xpipe13_rx_charisk({1'b0,1'b0}),
        .xpipe13_rx_data({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .xpipe13_rx_datavalid(1'b0),
        .xpipe13_rx_elecidle(1'b0),
        .xpipe13_rx_startblock(1'b0),
        .xpipe13_rx_status({1'b0,1'b0,1'b0}),
        .xpipe13_rx_syncheader({1'b0,1'b0}),
        .xpipe13_rx_valid(1'b0),
        .xpipe14_phystatus(1'b0),
        .xpipe14_rx_charisk({1'b0,1'b0}),
        .xpipe14_rx_data({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .xpipe14_rx_datavalid(1'b0),
        .xpipe14_rx_elecidle(1'b0),
        .xpipe14_rx_startblock(1'b0),
        .xpipe14_rx_status({1'b0,1'b0,1'b0}),
        .xpipe14_rx_syncheader({1'b0,1'b0}),
        .xpipe14_rx_valid(1'b0),
        .xpipe15_phystatus(1'b0),
        .xpipe15_rx_charisk({1'b0,1'b0}),
        .xpipe15_rx_data({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .xpipe15_rx_datavalid(1'b0),
        .xpipe15_rx_elecidle(1'b0),
        .xpipe15_rx_startblock(1'b0),
        .xpipe15_rx_status({1'b0,1'b0,1'b0}),
        .xpipe15_rx_syncheader({1'b0,1'b0}),
        .xpipe15_rx_valid(1'b0),
        .xpipe1_phystatus(1'b0),
        .xpipe1_rx_charisk({1'b0,1'b0}),
        .xpipe1_rx_data({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .xpipe1_rx_datavalid(1'b0),
        .xpipe1_rx_elecidle(1'b0),
        .xpipe1_rx_startblock(1'b0),
        .xpipe1_rx_status({1'b0,1'b0,1'b0}),
        .xpipe1_rx_syncheader({1'b0,1'b0}),
        .xpipe1_rx_valid(1'b0),
        .xpipe2_phystatus(1'b0),
        .xpipe2_rx_charisk({1'b0,1'b0}),
        .xpipe2_rx_data({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .xpipe2_rx_datavalid(1'b0),
        .xpipe2_rx_elecidle(1'b0),
        .xpipe2_rx_startblock(1'b0),
        .xpipe2_rx_status({1'b0,1'b0,1'b0}),
        .xpipe2_rx_syncheader({1'b0,1'b0}),
        .xpipe2_rx_valid(1'b0),
        .xpipe3_phystatus(1'b0),
        .xpipe3_rx_charisk({1'b0,1'b0}),
        .xpipe3_rx_data({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .xpipe3_rx_datavalid(1'b0),
        .xpipe3_rx_elecidle(1'b0),
        .xpipe3_rx_startblock(1'b0),
        .xpipe3_rx_status({1'b0,1'b0,1'b0}),
        .xpipe3_rx_syncheader({1'b0,1'b0}),
        .xpipe3_rx_valid(1'b0),
        .xpipe4_phystatus(1'b0),
        .xpipe4_rx_charisk({1'b0,1'b0}),
        .xpipe4_rx_data({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .xpipe4_rx_datavalid(1'b0),
        .xpipe4_rx_elecidle(1'b0),
        .xpipe4_rx_startblock(1'b0),
        .xpipe4_rx_status({1'b0,1'b0,1'b0}),
        .xpipe4_rx_syncheader({1'b0,1'b0}),
        .xpipe4_rx_valid(1'b0),
        .xpipe5_phystatus(1'b0),
        .xpipe5_rx_charisk({1'b0,1'b0}),
        .xpipe5_rx_data({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .xpipe5_rx_datavalid(1'b0),
        .xpipe5_rx_elecidle(1'b0),
        .xpipe5_rx_startblock(1'b0),
        .xpipe5_rx_status({1'b0,1'b0,1'b0}),
        .xpipe5_rx_syncheader({1'b0,1'b0}),
        .xpipe5_rx_valid(1'b0),
        .xpipe6_phystatus(1'b0),
        .xpipe6_rx_charisk({1'b0,1'b0}),
        .xpipe6_rx_data({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .xpipe6_rx_datavalid(1'b0),
        .xpipe6_rx_elecidle(1'b0),
        .xpipe6_rx_startblock(1'b0),
        .xpipe6_rx_status({1'b0,1'b0,1'b0}),
        .xpipe6_rx_syncheader({1'b0,1'b0}),
        .xpipe6_rx_valid(1'b0),
        .xpipe7_phystatus(1'b0),
        .xpipe7_rx_charisk({1'b0,1'b0}),
        .xpipe7_rx_data({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .xpipe7_rx_datavalid(1'b0),
        .xpipe7_rx_elecidle(1'b0),
        .xpipe7_rx_startblock(1'b0),
        .xpipe7_rx_status({1'b0,1'b0,1'b0}),
        .xpipe7_rx_syncheader({1'b0,1'b0}),
        .xpipe7_rx_valid(1'b0),
        .xpipe8_phystatus(1'b0),
        .xpipe8_rx_charisk({1'b0,1'b0}),
        .xpipe8_rx_data({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .xpipe8_rx_datavalid(1'b0),
        .xpipe8_rx_elecidle(1'b0),
        .xpipe8_rx_startblock(1'b0),
        .xpipe8_rx_status({1'b0,1'b0,1'b0}),
        .xpipe8_rx_syncheader({1'b0,1'b0}),
        .xpipe8_rx_valid(1'b0),
        .xpipe9_phystatus(1'b0),
        .xpipe9_rx_charisk({1'b0,1'b0}),
        .xpipe9_rx_data({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .xpipe9_rx_datavalid(1'b0),
        .xpipe9_rx_elecidle(1'b0),
        .xpipe9_rx_startblock(1'b0),
        .xpipe9_rx_status({1'b0,1'b0,1'b0}),
        .xpipe9_rx_syncheader({1'b0,1'b0}),
        .xpipe9_rx_valid(1'b0),
        .xpipe_q0_rxmargin_req_ack(1'b0),
        .xpipe_q0_rxmargin_res_cmd({1'b0,1'b0,1'b0,1'b0}),
        .xpipe_q0_rxmargin_res_lanenum({1'b0,1'b0}),
        .xpipe_q0_rxmargin_res_payload({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .xpipe_q0_rxmargin_res_req(1'b0),
        .xpipe_q1_rxmargin_req_ack(1'b0),
        .xpipe_q1_rxmargin_res_cmd({1'b0,1'b0,1'b0,1'b0}),
        .xpipe_q1_rxmargin_res_lanenum({1'b0,1'b0}),
        .xpipe_q1_rxmargin_res_payload({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .xpipe_q1_rxmargin_res_req(1'b0),
        .xpipe_q2_rxmargin_req_ack(1'b0),
        .xpipe_q2_rxmargin_res_cmd({1'b0,1'b0,1'b0,1'b0}),
        .xpipe_q2_rxmargin_res_lanenum({1'b0,1'b0}),
        .xpipe_q2_rxmargin_res_payload({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .xpipe_q2_rxmargin_res_req(1'b0),
        .xpipe_q3_rxmargin_req_ack(1'b0),
        .xpipe_q3_rxmargin_res_cmd({1'b0,1'b0,1'b0,1'b0}),
        .xpipe_q3_rxmargin_res_lanenum({1'b0,1'b0}),
        .xpipe_q3_rxmargin_res_payload({1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0,1'b0}),
        .xpipe_q3_rxmargin_res_req(1'b0));
endmodule
