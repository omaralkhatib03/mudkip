// (c) Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// (c) Copyright 2022-2025 Advanced Micro Devices, Inc. All rights reserved.
// 
// This file contains confidential and proprietary information
// of AMD and is protected under U.S. and international copyright
// and other intellectual property laws.
// 
// DISCLAIMER
// This disclaimer is not a license and does not grant any
// rights to the materials distributed herewith. Except as
// otherwise provided in a valid license issued to you by
// AMD, and to the maximum extent permitted by applicable
// law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
// WITH ALL FAULTS, AND AMD HEREBY DISCLAIMS ALL WARRANTIES
// AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
// BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
// INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
// (2) AMD shall not be liable (whether in contract or tort,
// including negligence, or under any other theory of
// liability) for any loss or damage of any kind or nature
// related to, arising under or in connection with these
// materials, including for any direct, or any indirect,
// special, incidental, or consequential loss or damage
// (including loss of data, profits, goodwill, or any type of
// loss or damage suffered as a result of any action brought
// by a third party) even if such damage or loss was
// reasonably foreseeable or AMD had been advised of the
// possibility of the same.
// 
// CRITICAL APPLICATIONS
// AMD products are not designed or intended to be fail-
// safe, or for use in any application requiring fail-safe
// performance, such as life-support or safety devices or
// systems, Class III medical devices, nuclear facilities,
// applications related to the deployment of airbags, or any
// other applications that could lead to death, personal
// injury, or severe property or environmental damage
// (individually and collectively, "Critical
// Applications"). Customer assumes the sole risk and
// liability of any use of AMD products in Critical
// Applications, subject only to applicable laws and
// regulations governing limitations on product liability.
// 
// THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
// PART OF THIS FILE AT ALL TIMES.
// 
// DO NOT MODIFY THIS FILE.


// IP VLNV: xilinx.com:module_ref:fl_vadd_top:1.0
// IP Revision: 1

`timescale 1ns/1ps

(* IP_DEFINITION_SOURCE = "module_ref" *)
(* DowngradeIPIdentifiedWarnings = "yes" *)
module top_fl_vadd_top_0_1 (
  clk,
  rst_n,
  in_x_data,
  in_x_valid,
  in_x_ready,
  in_x_tlast,
  in_y_data,
  in_y_valid,
  in_y_ready,
  in_y_tlast,
  waddr,
  wavalid,
  waready,
  wdata,
  wvalid,
  wready,
  wresp,
  bvalid,
  bready,
  raddr,
  arvalid,
  arready,
  rdata,
  rvalid,
  rready,
  out_vector_waddr,
  out_vector_wavalid,
  out_vector_waready,
  out_vector_wdata,
  out_vector_wvalid,
  out_vector_wready,
  out_vector_wresp,
  out_vector_bvalid,
  out_vector_bready,
  out_vector_raddr,
  out_vector_arvalid,
  out_vector_arready,
  out_vector_rdata,
  out_vector_rvalid,
  out_vector_rready
);

(* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 clk CLK" *)
(* X_INTERFACE_MODE = "slave" *)
(* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME clk, ASSOCIATED_BUSIF vector_x:vector_y:axil_ps_if:out_vector_if, FREQ_HZ 333329987, FREQ_TOLERANCE_HZ 0, PHASE 0.0, CLK_DOMAIN bd_1baa_pspmc_0_0_pl0_ref_clk, INSERT_VIP 0" *)
input wire clk;
(* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 rst_n RST" *)
(* X_INTERFACE_MODE = "slave" *)
(* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME rst_n, POLARITY ACTIVE_LOW, INSERT_VIP 0" *)
input wire rst_n;
(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 vector_x TDATA" *)
(* X_INTERFACE_MODE = "slave" *)
(* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME vector_x, TDATA_NUM_BYTES 4, TDEST_WIDTH 0, TID_WIDTH 0, TUSER_WIDTH 0, HAS_TREADY 1, HAS_TSTRB 0, HAS_TKEEP 0, HAS_TLAST 1, FREQ_HZ 333329987, PHASE 0.0, CLK_DOMAIN bd_1baa_pspmc_0_0_pl0_ref_clk, LAYERED_METADATA undef, INSERT_VIP 0" *)
input wire [31 : 0] in_x_data;
(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 vector_x TVALID" *)
input wire in_x_valid;
(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 vector_x TREADY" *)
output wire in_x_ready;
(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 vector_x TLAST" *)
input wire in_x_tlast;
(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 vector_y TDATA" *)
(* X_INTERFACE_MODE = "slave" *)
(* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME vector_y, TDATA_NUM_BYTES 4, TDEST_WIDTH 0, TID_WIDTH 0, TUSER_WIDTH 0, HAS_TREADY 1, HAS_TSTRB 0, HAS_TKEEP 0, HAS_TLAST 1, FREQ_HZ 333329987, PHASE 0.0, CLK_DOMAIN bd_1baa_pspmc_0_0_pl0_ref_clk, LAYERED_METADATA undef, INSERT_VIP 0" *)
input wire [31 : 0] in_y_data;
(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 vector_y TVALID" *)
input wire in_y_valid;
(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 vector_y TREADY" *)
output wire in_y_ready;
(* X_INTERFACE_INFO = "xilinx.com:interface:axis:1.0 vector_y TLAST" *)
input wire in_y_tlast;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if AWADDR" *)
(* X_INTERFACE_MODE = "slave" *)
(* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME axil_ps_if, DATA_WIDTH 32, PROTOCOL AXI4LITE, FREQ_HZ 333329987, ID_WIDTH 0, ADDR_WIDTH 2, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 0, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 0, HAS_BRESP 1, HAS_RRESP 0, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 1, NUM_WRITE_OUTSTANDING 1, MAX_BURST_LENGTH 1, PHASE 0.0, CLK_DOMAIN bd_1baa_pspmc_0_0_pl0_ref_clk, NUM_READ_THREADS 1, NUM_W\
RITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *)
input wire [1 : 0] waddr;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if AWVALID" *)
input wire wavalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if AWREADY" *)
output wire waready;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if WDATA" *)
input wire [31 : 0] wdata;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if WVALID" *)
input wire wvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if WREADY" *)
output wire wready;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if BRESP" *)
output wire wresp;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if BVALID" *)
output wire bvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if BREADY" *)
input wire bready;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if ARADDR" *)
input wire [1 : 0] raddr;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if ARVALID" *)
input wire arvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if ARREADY" *)
output wire arready;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if RDATA" *)
output wire [31 : 0] rdata;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if RVALID" *)
output wire rvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 axil_ps_if RREADY" *)
input wire rready;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if AWADDR" *)
(* X_INTERFACE_MODE = "master" *)
(* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME out_vector_if, DATA_WIDTH 32, PROTOCOL AXI4LITE, FREQ_HZ 333329987, ID_WIDTH 0, ADDR_WIDTH 5, AWUSER_WIDTH 0, ARUSER_WIDTH 0, WUSER_WIDTH 0, RUSER_WIDTH 0, BUSER_WIDTH 0, READ_WRITE_MODE READ_WRITE, HAS_BURST 0, HAS_LOCK 0, HAS_PROT 0, HAS_CACHE 0, HAS_QOS 0, HAS_REGION 0, HAS_WSTRB 0, HAS_BRESP 1, HAS_RRESP 0, SUPPORTS_NARROW_BURST 0, NUM_READ_OUTSTANDING 1, NUM_WRITE_OUTSTANDING 1, MAX_BURST_LENGTH 1, PHASE 0.0, CLK_DOMAIN bd_1baa_pspmc_0_0_pl0_ref_clk, NUM_READ_THREADS 1, NU\
M_WRITE_THREADS 1, RUSER_BITS_PER_BYTE 0, WUSER_BITS_PER_BYTE 0, INSERT_VIP 0" *)
output wire [4 : 0] out_vector_waddr;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if AWVALID" *)
output wire out_vector_wavalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if AWREADY" *)
input wire out_vector_waready;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if WDATA" *)
output wire [31 : 0] out_vector_wdata;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if WVALID" *)
output wire out_vector_wvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if WREADY" *)
input wire out_vector_wready;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if BRESP" *)
input wire out_vector_wresp;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if BVALID" *)
input wire out_vector_bvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if BREADY" *)
output wire out_vector_bready;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if ARADDR" *)
output wire [4 : 0] out_vector_raddr;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if ARVALID" *)
output wire out_vector_arvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if ARREADY" *)
input wire out_vector_arready;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if RDATA" *)
input wire [31 : 0] out_vector_rdata;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if RVALID" *)
input wire out_vector_rvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 out_vector_if RREADY" *)
output wire out_vector_rready;

  fl_vadd_top #(
    .AXI_VECTOR_WIDTH(32),
    .AXI_PS_DEPTH(4),
    .AXI_PS_DATAWIDTH(32),
    .AXI_DDR_DATAWIDTH(32),
    .AXI_DDR_DEPTH(32),
    .AXI_DDR_ADDR_WIDTH(5),
    .AXI_PS_ADDR_WIDTH(2)
  ) inst (
    .clk(clk),
    .rst_n(rst_n),
    .in_x_data(in_x_data),
    .in_x_valid(in_x_valid),
    .in_x_ready(in_x_ready),
    .in_x_tlast(in_x_tlast),
    .in_y_data(in_y_data),
    .in_y_valid(in_y_valid),
    .in_y_ready(in_y_ready),
    .in_y_tlast(in_y_tlast),
    .waddr(waddr),
    .wavalid(wavalid),
    .waready(waready),
    .wdata(wdata),
    .wvalid(wvalid),
    .wready(wready),
    .wresp(wresp),
    .bvalid(bvalid),
    .bready(bready),
    .raddr(raddr),
    .arvalid(arvalid),
    .arready(arready),
    .rdata(rdata),
    .rvalid(rvalid),
    .rready(rready),
    .out_vector_waddr(out_vector_waddr),
    .out_vector_wavalid(out_vector_wavalid),
    .out_vector_waready(out_vector_waready),
    .out_vector_wdata(out_vector_wdata),
    .out_vector_wvalid(out_vector_wvalid),
    .out_vector_wready(out_vector_wready),
    .out_vector_wresp(out_vector_wresp),
    .out_vector_bvalid(out_vector_bvalid),
    .out_vector_bready(out_vector_bready),
    .out_vector_raddr(out_vector_raddr),
    .out_vector_arvalid(out_vector_arvalid),
    .out_vector_arready(out_vector_arready),
    .out_vector_rdata(out_vector_rdata),
    .out_vector_rvalid(out_vector_rvalid),
    .out_vector_rready(out_vector_rready)
  );
endmodule
