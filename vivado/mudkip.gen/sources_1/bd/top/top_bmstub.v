// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// Copyright 2022-2025 Advanced Micro Devices, Inc. All Rights Reserved.
// -------------------------------------------------------------------------------

`timescale 1 ps / 1 ps

(* BLOCK_STUB = "true" *)
module top (
  reset_rtl,
  ddr4_dimm1_0_dq,
  ddr4_dimm1_0_dqs_t,
  ddr4_dimm1_0_dqs_c,
  ddr4_dimm1_0_adr,
  ddr4_dimm1_0_ba,
  ddr4_dimm1_0_bg,
  ddr4_dimm1_0_act_n,
  ddr4_dimm1_0_reset_n,
  ddr4_dimm1_0_ck_t,
  ddr4_dimm1_0_ck_c,
  ddr4_dimm1_0_cke,
  ddr4_dimm1_0_cs_n,
  ddr4_dimm1_0_dm_n,
  ddr4_dimm1_0_odt,
  ddr4_dimm1_sma_clk_clk_p,
  ddr4_dimm1_sma_clk_clk_n
);

  (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 RST.RESET_RTL RST" *)
  (* X_INTERFACE_MODE = "slave RST.RESET_RTL" *)
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME RST.RESET_RTL, POLARITY ACTIVE_LOW, INSERT_VIP 0" *)
  input reset_rtl;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 ddr4_dimm1_0 DQ" *)
  (* X_INTERFACE_MODE = "master ddr4_dimm1_0" *)
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME ddr4_dimm1_0, CAN_DEBUG false, TIMEPERIOD_PS 1250, MEMORY_TYPE COMPONENTS, DATA_WIDTH 8, CS_ENABLED true, DATA_MASK_ENABLED true, SLOT Single, MEM_ADDR_MAP ROW_COLUMN_BANK, BURST_LENGTH 8, AXI_ARBITRATION_SCHEME TDM, CAS_LATENCY 11, CAS_WRITE_LATENCY 11" *)
  inout [63:0]ddr4_dimm1_0_dq;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 ddr4_dimm1_0 DQS_T" *)
  inout [7:0]ddr4_dimm1_0_dqs_t;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 ddr4_dimm1_0 DQS_C" *)
  inout [7:0]ddr4_dimm1_0_dqs_c;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 ddr4_dimm1_0 ADR" *)
  output [16:0]ddr4_dimm1_0_adr;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 ddr4_dimm1_0 BA" *)
  output [1:0]ddr4_dimm1_0_ba;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 ddr4_dimm1_0 BG" *)
  output [1:0]ddr4_dimm1_0_bg;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 ddr4_dimm1_0 ACT_N" *)
  output ddr4_dimm1_0_act_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 ddr4_dimm1_0 RESET_N" *)
  output ddr4_dimm1_0_reset_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 ddr4_dimm1_0 CK_T" *)
  output ddr4_dimm1_0_ck_t;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 ddr4_dimm1_0 CK_C" *)
  output ddr4_dimm1_0_ck_c;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 ddr4_dimm1_0 CKE" *)
  output ddr4_dimm1_0_cke;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 ddr4_dimm1_0 CS_N" *)
  output ddr4_dimm1_0_cs_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 ddr4_dimm1_0 DM_N" *)
  inout [7:0]ddr4_dimm1_0_dm_n;
  (* X_INTERFACE_INFO = "xilinx.com:interface:ddr4:1.0 ddr4_dimm1_0 ODT" *)
  output ddr4_dimm1_0_odt;
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 ddr4_dimm1_sma_clk CLK_P" *)
  (* X_INTERFACE_MODE = "slave ddr4_dimm1_sma_clk" *)
  (* X_INTERFACE_PARAMETER = "XIL_INTERFACENAME ddr4_dimm1_sma_clk, CAN_DEBUG false, FREQ_HZ 200000000" *)
  input ddr4_dimm1_sma_clk_clk_p;
  (* X_INTERFACE_INFO = "xilinx.com:interface:diff_clock:1.0 ddr4_dimm1_sma_clk CLK_N" *)
  input ddr4_dimm1_sma_clk_clk_n;

  // stub module has no contents

endmodule
