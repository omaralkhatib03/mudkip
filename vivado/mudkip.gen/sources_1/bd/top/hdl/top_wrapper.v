//Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
//Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2024.2.1 (lin64) Build 5266912 Sun Dec 15 09:03:31 MST 2024
//Date        : Mon Feb 10 13:42:43 2025
//Host        : omar-Dell-G15-5515 running 64-bit Ubuntu 24.04.1 LTS
//Command     : generate_target top_wrapper.bd
//Design      : top_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module top_wrapper
   (ddr4_dimm1_0_act_n,
    ddr4_dimm1_0_adr,
    ddr4_dimm1_0_ba,
    ddr4_dimm1_0_bg,
    ddr4_dimm1_0_ck_c,
    ddr4_dimm1_0_ck_t,
    ddr4_dimm1_0_cke,
    ddr4_dimm1_0_cs_n,
    ddr4_dimm1_0_dm_n,
    ddr4_dimm1_0_dq,
    ddr4_dimm1_0_dqs_c,
    ddr4_dimm1_0_dqs_t,
    ddr4_dimm1_0_odt,
    ddr4_dimm1_0_reset_n,
    ddr4_dimm1_sma_clk_clk_n,
    ddr4_dimm1_sma_clk_clk_p,
    reset_rtl);
  output ddr4_dimm1_0_act_n;
  output [16:0]ddr4_dimm1_0_adr;
  output [1:0]ddr4_dimm1_0_ba;
  output [1:0]ddr4_dimm1_0_bg;
  output ddr4_dimm1_0_ck_c;
  output ddr4_dimm1_0_ck_t;
  output ddr4_dimm1_0_cke;
  output ddr4_dimm1_0_cs_n;
  inout [7:0]ddr4_dimm1_0_dm_n;
  inout [63:0]ddr4_dimm1_0_dq;
  inout [7:0]ddr4_dimm1_0_dqs_c;
  inout [7:0]ddr4_dimm1_0_dqs_t;
  output ddr4_dimm1_0_odt;
  output ddr4_dimm1_0_reset_n;
  input ddr4_dimm1_sma_clk_clk_n;
  input ddr4_dimm1_sma_clk_clk_p;
  input reset_rtl;

  wire ddr4_dimm1_0_act_n;
  wire [16:0]ddr4_dimm1_0_adr;
  wire [1:0]ddr4_dimm1_0_ba;
  wire [1:0]ddr4_dimm1_0_bg;
  wire ddr4_dimm1_0_ck_c;
  wire ddr4_dimm1_0_ck_t;
  wire ddr4_dimm1_0_cke;
  wire ddr4_dimm1_0_cs_n;
  wire [7:0]ddr4_dimm1_0_dm_n;
  wire [63:0]ddr4_dimm1_0_dq;
  wire [7:0]ddr4_dimm1_0_dqs_c;
  wire [7:0]ddr4_dimm1_0_dqs_t;
  wire ddr4_dimm1_0_odt;
  wire ddr4_dimm1_0_reset_n;
  wire ddr4_dimm1_sma_clk_clk_n;
  wire ddr4_dimm1_sma_clk_clk_p;
  wire reset_rtl;

  top top_i
       (.ddr4_dimm1_0_act_n(ddr4_dimm1_0_act_n),
        .ddr4_dimm1_0_adr(ddr4_dimm1_0_adr),
        .ddr4_dimm1_0_ba(ddr4_dimm1_0_ba),
        .ddr4_dimm1_0_bg(ddr4_dimm1_0_bg),
        .ddr4_dimm1_0_ck_c(ddr4_dimm1_0_ck_c),
        .ddr4_dimm1_0_ck_t(ddr4_dimm1_0_ck_t),
        .ddr4_dimm1_0_cke(ddr4_dimm1_0_cke),
        .ddr4_dimm1_0_cs_n(ddr4_dimm1_0_cs_n),
        .ddr4_dimm1_0_dm_n(ddr4_dimm1_0_dm_n),
        .ddr4_dimm1_0_dq(ddr4_dimm1_0_dq),
        .ddr4_dimm1_0_dqs_c(ddr4_dimm1_0_dqs_c),
        .ddr4_dimm1_0_dqs_t(ddr4_dimm1_0_dqs_t),
        .ddr4_dimm1_0_odt(ddr4_dimm1_0_odt),
        .ddr4_dimm1_0_reset_n(ddr4_dimm1_0_reset_n),
        .ddr4_dimm1_sma_clk_clk_n(ddr4_dimm1_sma_clk_clk_n),
        .ddr4_dimm1_sma_clk_clk_p(ddr4_dimm1_sma_clk_clk_p),
        .reset_rtl(reset_rtl));
endmodule
