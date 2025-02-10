/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//185f439ad35e2293624be383394296229a3745323be6fe7e51ba3245d50dcbae
// Proprietary Note:
// XILINX CONFIDENTIAL
//
// Copyright 2019 Xilinx, Inc. All rights reserved.
// This file contains confidential and proprietary information of Xilinx, Inc.
// and is protected under U.S. and international copyright and other
// intellectual property laws.
//
// DISCLAIMER
// This disclaimer is not a license and does not grant any
// rights to the materials distributed herewith. Except as
// otherwise provided in a valid license issued to you by
// Xilinx, and to the maximum extent permitted by applicable
// law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
// WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
// AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
// BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
// INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
// (2) Xilinx shall not be liable (whether in contract or tort,
// including negligence, or under any other theory of
// liability) for any loss or damage of any kind or nature
// related to, arising under or in connection with these
// materials, including for any direct, or any indirect,
// special, incidental, or consequential loss or damage
// (including loss of data, profits, goodwill, or any type of
// loss or damage suffered as a result of any action brought
// by a third party) even if such damage or loss was
// reasonably foreseeable or Xilinx had been advised of the
// possibility of the same.
//
// CRITICAL APPLICATIONS
// Xilinx products are not designed or intended to be fail-
// safe, or for use in any application requiring fail-safe
// performance, such as life-support or safety devices or
// systems, Class III medical devices, nuclear facilities,
// applications related to the deployment of airbags, or any
// other applications that could lead to death, personal
// injury, or severe property or environmental damage
// (individually and collectively, "Critical
// Applications"). Customer assumes the sole risk and
// liability of any use of Xilinx products in Critical
// Applications, subject only to applicable laws and
// regulations governing limitations on product liability.
//
// THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
// PART OF THIS FILE AT ALL TIMES.
//
//
//
//       Owner:
//       Revision:       $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/ddr_write_latency_cal.c#4 $
//                       $Author: rgaddam $
//                       $DateTime: 2019/04/15 11:06:56 $
//                       $Change: 21202855 $
//       Description:
//           DDRMC : 
//
///////////////////////////////////////////////////////////////////////////////
#ifndef ENABLE_MICROBLAZE_BFM
   #include "xparameters.h"
   #include "xiomodule.h"
#else
   #include "ddr_standalone.h"
#endif

#include "bd_d0e7_MC0_ddrc_0_phy_cal_config_define.h"
#include "bd_d0e7_MC0_ddrc_0_phy_io_config_define.h"
#include "cal_reg_define.h"
#include "riu_reg_define.h"
#include "xsdb_reg_define.h"
#include "internal_reg_define.h"
#include "mc_reg_define.h"
#include "function_dec.c"
#include "global_variable_define.h"

u8 ddr_write_latency_cal ()
{
  u8 i, j;
  u32 rd_data, bl16_rd_data, wl_dly_rnk, crse;
  u32 xpi_oe_all_nib, xpi_wrdata_all_nib;
  u8 adjust_oe_all_nib;
  u8 adjust_wrdata_all_nib;
  u32 mr2_1tck_wr_pre;
  u8 min_required_lat = 255;
  u8 ecc_encode_en;
  u8 xpi_min_lat;
  u8 min_ddr_write_latency[DQS_BITS];
  u8 current_rank;
  u8 ddr_write_latency[RANKS][DQS_BITS];
  u32 extend_write_data;
  u32 dq_compare_result;
  u8 phy_oe_dly;
  u8 phy_oe_dly_upd[RANKS][DQS_BITS];
  u8 phy_oe_dly_min, phy_oe_dly_max, phy_oe_all_dly_final;
  u32 data_dly;
  u8 first_iteration = 1;
  u8 coarse[RANKS];
  u16 fine[RANKS];
  u16 rl_dly_qtr;
  u8 diff;
  u8 odelay;
  u8 status;
  u32 reg_safe_config5;
  u8 add_cmd_delay;
  u8 add_cmd_delay_en;
  u8 phy_nibble;
  u8 phy_rden;

  // FIX ME
  u32 cplx_config2;
  u8 cas_gap_timer, act_gap_timer, burst_gap_timer, cas2pre_timer, prea2act_timer;
  u16 lp4_dqs_coarse, lp4_dqs_fine, lp4_dq_coarse, lp4_dq_fine;
  u16 t_dqs_to_dq_latency;
  u32 dqs_delay, dq_delay;
  u16 xpi_oe_pre_post;

  if(glbl_phy_rank_switch_en == 1) {
    DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Enabling dynamic mode for TX\n");
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
    riu_reg_bcast(DIS_DYN_MODE, 0, 0x0);

    DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Dummy write for setting correct odelays\n");
    dummy_write_multi_rank_switch(0);
  }

  // SEQUENCE CAPTURE MUX SELECTION
  writeIO_DMC(ral_addr_of_seq_data_capture_in_ddrmc_main_ddrmc_main_bank(BLK),0x0); 

  // Bcast the Nibble Wise Reset Mask Setting For DQS
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK0, glbl_nibble_reset_mask[0]);
  riu_reg_bcast(CFG_BS_RST_MASK, 0, ~(0x7F));

  // Bcast the Nibble Wise Reset Mask Setting For DM
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK0, glbl_nibble_reset_mask[1]);
  riu_reg_bcast(CFG_BS_RST_MASK, 0, ~(0x7D));

  writeIO_RIU( RPI_BCAST_NIBBLE_MASK0, (glbl_nibble_reset_mask[0] | glbl_nibble_reset_mask[1]));

  bitslice_reset_bcast(0, BS_RESET_WT);

  xpi_wrdata_all_nib      = readIO_DMC(ral_addr_of_xpi_wrdata_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK)); 
  xpi_oe_all_nib          = readIO_DMC(ral_addr_of_xpi_oe_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK));

  for(current_rank = 0; current_rank < RANKS; current_rank += glbl_rank_inc)
  {
    bitslice_reset_bcast(0, BS_RESET_WT);

    // Programming the Data Delay Value into the Registers
    for(i=0; i<DQS_BITS; i++)
    {
      writeIO_DMC(ral_addr_of_phy_data_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*i*NIBBLE_PER_DQS), 0x0);
     
      if(NIBBLE_PER_DQS == 2)
        writeIO_DMC(ral_addr_of_phy_data_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*i*NIBBLE_PER_DQS) + 4, 0x0);
    }

    // Programming the Ouput EnableDelay Value into the Registers
    for(i=0; i<DQS_BITS; i++)
    {
      for(j=0; j<NIBBLE_PER_DQS; j++)
      {
        writeIO_DMC(ral_addr_of_phy_oe_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*glbl_map_riu[(i*NIBBLE_PER_DQS)+j]) , 0x0);   
      }
    }

    writeIO_DMC(ral_addr_of_seq_cs_bits_in_ddrmc_main_ddrmc_main_bank(BLK), 1<<current_rank);

    #ifdef DDR4
      // PRECHARGE COMMAND
      set_1_seq_command_at_mem(current_rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                            MEM_ROW_PREACHARGE /*u32 seq_a_cmd*/, 0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/,
                            0/*u32 seq_a2a_dly*/);

      if(glbl_wr_pre_2tck_mode) { // 2 tCK write preamble mode
        DEBUG_PRINTF("INFO: ddr_write_latency_cal(): MR4: Setting memory into 1 tCK write preamble mode since write latency is not figured out yet!!\n");
        set_1_seq_command_at_mem(current_rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, 
                                (glbl_mr4 & ~(0x1000)) /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x0 /*u16 seq_a_ba*/, 0 /*u32 seq_a2a_dly*/);

        if(glbl_wr_pre_2tck_cwl_adjust) { // Adjust CWL as needed
          DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Reducing the write latency as per 1 tCK write preamble mode\n");
          mr2_1tck_wr_pre = (glbl_mr2 - 0x8) ; // mr2[5:3]--
          set_1_seq_command_at_mem(current_rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, 
                                   mr2_1tck_wr_pre /*u32 seq_a_addr*/, 0x0 /*u8 seq_a_bg*/, 0x2 /*u16 seq_a_ba*/, 0 /*u32 seq_a2a_dly*/);

          if(current_rank == 0) { // Only once
            glbl_write_latency = glbl_init_write_latency - 2;

            xpi_oe_all_nib     = readIO_DMC(ral_addr_of_xpi_oe_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK));
            xpi_wrdata_all_nib = readIO_DMC(ral_addr_of_xpi_wrdata_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK)); 

            xpi_oe_all_nib     = (xpi_oe_all_nib & ~(0x7F)) | glbl_write_latency;
            xpi_wrdata_all_nib = (xpi_wrdata_all_nib & ~(0x7F)) | glbl_write_latency;

            writeIO_DMC(ral_addr_of_xpi_oe_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK), xpi_oe_all_nib);
            writeIO_DMC(ral_addr_of_xpi_wrdata_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK), xpi_wrdata_all_nib);
          }
        }
      }

      // ACTIVATE COMMAND
      set_1_seq_command_at_mem(current_rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                            MEM_ROW_ACTIVE /*u32 seq_a_cmd*/, 0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/,
                            0 /*u32 seq_a2a_dly*/);
    #else
      // PRECHARGE COMMAND
        lp4_bank_row_access(current_rank /*u8 rank*/, PRECHARGE /*u8 cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);
     
      // ACTIVATE COMMAND
        lp4_bank_row_access(current_rank /*u8 rank*/, ACTIVATE /*cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);
    
    #endif

// XRAM_DBG: wrlat_init_latency
    writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRLAT_INIT_LATENCY, glbl_write_latency);

    // Writing PRE MAIN and POST DATA
    writeIO_DMC(ral_addr_of_seq_a_dq_early_in_ddrmc_main_ddrmc_main_bank(BLK), 0x3);
    writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK),0xA);

    #ifdef LPDDR4
      writeIO_DMC(ral_addr_of_seq_a_dq_1_in_ddrmc_main_ddrmc_main_bank(BLK),0xC);
    #endif

    writeIO_DMC(ral_addr_of_seq_a_dq_late_in_ddrmc_main_ddrmc_main_bank(BLK),0x7);

    extend_write_data = readIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK));
    writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (extend_write_data | 0x00000009));

    #ifdef DDR4
      set_2_seq_command_at_mem(current_rank /* rank */, 2 /* no of iteration */, first_iteration /* full write */, MEM_COL_WRITE /* seq_a_cmd */,
                            0 /* seq_a_addr */, 0 /* seq_a_bg */, 0 /* seq_ba */, MEM_COL_READ /* seq_b_cmd */, 
                            0 /*seq_b_addr*/, 0 /*seq_b_bg*/,0 /*seq_b_ba*/, 40 /*seq_a2b_dly*/, 40/*seq_b2a_dly*/);
    #else
      lp4_bank_col_access(2, COL_WR, COL_RD, 0, 0);  

    #endif

    writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (extend_write_data & 0xFFFFFFF6));

    // Precharge the ROW
    #ifdef DDR4
      set_1_seq_command_at_mem(current_rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                           MEM_ROW_PREACHARGE /*u32 seq_a_cmd*/, 0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/,
                           0/*u32 seq_a2a_dly*/);
    #else
        lp4_bank_row_access(current_rank /*u8 rank*/, PRECHARGE /*u8 cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);
     
    #endif

    dq_compare_result = 0;
    for(i=0; i<DQS_BITS; i++)
    {
      rd_data = (readIO_DMC(ral_addr_of_bl8_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*i*NIBBLE_PER_DQS)));
      DEBUG_PRINTF("INFO: ddr_write_latency_cal(): rd_data[%d] == %x \n",i, rd_data);

      #ifdef DDR4
        if(rd_data == WL_PATTERN_12)
          ddr_write_latency[current_rank][i] = glbl_write_latency + 3; 
        else if(rd_data == WL_PATTERN_8)
          ddr_write_latency[current_rank][i] = glbl_write_latency + 2; 
        else if(rd_data == WL_PATTERN_4)
          ddr_write_latency[current_rank][i] = glbl_write_latency + 1; 
        else if(rd_data == WL_PATTERN_0)
          ddr_write_latency[current_rank][i] = glbl_write_latency + 0; 
        else if(rd_data == WL_PATTERN_M_4)
          ddr_write_latency[current_rank][i] = glbl_write_latency - 1; 
        else {
          DEBUG_PRINTF("ERROR: ddr_write_latency_cal(): WRITE LATENCY FAILED DUE TO NO PATTERN MATCH \n")
          ddr_write_latency[current_rank][i] = 0xFF;
          dq_compare_result |= (0x1 << (i*NIBBLE_PER_DQS));
        }

      #else
        bl16_rd_data = (readIO_DMC(ral_addr_of_bl16_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*i*NIBBLE_PER_DQS)));
        DEBUG_PRINTF("INFO: ddr_write_latency_cal(): bl16_rd_data[%d] == %x \n",i, bl16_rd_data);

        if((rd_data == LP4_WL_PATTERN_12_0) && (bl16_rd_data == LP4_WL_PATTERN_12_1))
          ddr_write_latency[current_rank][i] = glbl_write_latency + 3; 
        else if((rd_data == LP4_WL_PATTERN_8_0) && (bl16_rd_data == LP4_WL_PATTERN_8_1))
          ddr_write_latency[current_rank][i] = glbl_write_latency + 2; 
        else if((rd_data == LP4_WL_PATTERN_4_0) && (bl16_rd_data == LP4_WL_PATTERN_4_1))
          ddr_write_latency[current_rank][i] = glbl_write_latency + 1; 
        else if((rd_data == LP4_WL_PATTERN_0_0) && (bl16_rd_data == LP4_WL_PATTERN_0_1))
          ddr_write_latency[current_rank][i] = glbl_write_latency + 0; 
        else if((rd_data == LP4_WL_PATTERN_M_4_0) && (bl16_rd_data == LP4_WL_PATTERN_M_4_1))
          ddr_write_latency[current_rank][i] = glbl_write_latency - 1; 
        else {
          DEBUG_PRINTF("ERROR: ddr_write_latency_cal(): WRITE LATENCY FAILED DUE TO NO PATTERN MATCH \n")
          ddr_write_latency[current_rank][i] = 0xFF;
          dq_compare_result |= (0x1 << (i*NIBBLE_PER_DQS));
        }
      #endif

      DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Rank = %d, Nibble = %d, Write Latency == %d\n", current_rank, i, ddr_write_latency[current_rank][i]);
// XRAM_DBG: wrlat_match
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRLAT_MATCH + ((current_rank * MAX_DQS_BITS + i) * 4), ddr_write_latency[current_rank][i]);

      // Finding The Minimum Value of Write Latency    
      if(min_required_lat > ddr_write_latency[current_rank][i])
        min_required_lat = ddr_write_latency[current_rank][i];
    }
    if(dq_compare_result) {
      write_error_nibble_XRAM(0, dq_compare_result);
      write_error_XRAM(ERR_CODE_37_WRLAT_PATTERN_MISMATCH);
      return 1;
    }

    bitslice_reset_bcast(0, BS_RESET_WT);
  } //RANKS

  // Check for minimum supported XPI write latency
  ecc_encode_en = (readIO_DMC(ral_addr_of_reg_pinout_in_ddrmc_main_ddrmc_main_bank(BLK)) >> 5) & 0x1; // [5] - ecc_encode_en
  xpi_min_lat = ecc_encode_en ? 11 : 9; // 9 without ECC and 11 with ECC
  DEBUG_PRINTF("INFO: ddr_write_latency_cal(): ecc_encode_en = %d, minimum supported XPI write latency = %d\n", ecc_encode_en, xpi_min_lat);

  if(min_required_lat < xpi_min_lat) {
    DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Minimum required latency is not supported in XPI. min_required_lat = %d\n", min_required_lat);

    reg_safe_config5 = readIO_DMC(ral_addr_of_reg_safe_config5_in_ddrmc_main_ddrmc_main_bank(BLK));
    add_cmd_delay_en = (reg_safe_config5 >> 31) & 0x1; // [31]
    add_cmd_delay = ((reg_safe_config5 >> 28) & 0x7); // [30:28]

    DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Default CMD latency: reg_safe_config5 = %x, add_cmd_delay_en = %d, add_cmd_delay = %d\n", reg_safe_config5, add_cmd_delay_en, add_cmd_delay);

    // Add 2 DRAM clocks delay
    if(add_cmd_delay_en) { // Check if add_cmd_delay is already enabled or not
      add_cmd_delay = add_cmd_delay + 1; // [30:28]
    } else {
      add_cmd_delay = 0; // [30:28]
    }

    add_cmd_delay_en = 1; // [31]
    reg_safe_config5 = ((reg_safe_config5 & ~(0xF << 28)) | (add_cmd_delay << 28) | (add_cmd_delay_en << 31));
    
    DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Updated CMD latency: reg_safe_config5 = %x, add_cmd_delay_en = %d, add_cmd_delay = %d\n", reg_safe_config5, add_cmd_delay_en, add_cmd_delay);
    writeIO_DMC(ral_addr_of_reg_safe_config5_in_ddrmc_main_ddrmc_main_bank(BLK), reg_safe_config5);

    min_required_lat = min_required_lat + 2;

    for(i=0; i<DQS_BITS; i++) {
      for(current_rank = 0; current_rank < RANKS; current_rank += glbl_rank_inc) {
        ddr_write_latency[current_rank][i] = ddr_write_latency[current_rank][i] + 2; 
      }

      // Adjust DQS read latency
      phy_nibble = glbl_dqs_bits_riu_map[i][0];
      phy_rden = readIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK) + (phy_nibble * 4));
      phy_rden = phy_rden + 2;
      DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Adjusting DQS phy read enable. Phy nibble = %d, Updated phy_rden = %d\n", phy_nibble, phy_rden);
      writeIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK) + (phy_nibble * 4), phy_rden);
    }
  }

  DEBUG_PRINTF("INFO: ddr_write_latency_cal(): min_required_lat = %d\n", min_required_lat);
  writeIO_DMC(ral_addr_of_xpi_wrdata_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK), min_required_lat | (xpi_wrdata_all_nib & 0x780));
  writeIO_DMC(ral_addr_of_xpi_oe_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK), min_required_lat | (xpi_oe_all_nib & 0x780));

// XRAM_DBG: wrlat_min_latency
  writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRLAT_MIN_LATENCY, min_required_lat);

// XRAM_DBG: wrlat_xpi_oe_all_final
// XRAM_DBG: wrlat_xpi_wrdata_all_final
  writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRLAT_XPI_OE_ALL_FINAL, min_required_lat);
  writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRLAT_XPI_WRDATA_ALL_FINAL, min_required_lat);

  // Updating the Value of Write Latency
  for(current_rank = 0; current_rank < RANKS; current_rank += glbl_rank_inc)
  {
    for(i=0; i<DQS_BITS; i++)
    {
      ddr_write_latency[current_rank][i] = ddr_write_latency[current_rank][i] - min_required_lat;

      // Add the XPHY coarse latencies in calculation
      wl_dly_rnk = read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], WLDLYRNK0+current_rank);
      crse = (wl_dly_rnk & RIU_WL_DELAY_CRSE_MASK) >> RIU_WL_DELAY_CRSE_OFFSET;

      ddr_write_latency[current_rank][i] = (ddr_write_latency[current_rank][i] << 2) + crse;

      crse = crse % 4; // Keep it less than tCK, to avoid double counting across XPI and XPHY settings.
      wl_dly_rnk = (wl_dly_rnk & ~RIU_WL_DELAY_CRSE_MASK) | (crse << RIU_WL_DELAY_CRSE_OFFSET);

      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], WLDLYRNK0+current_rank, wl_dly_rnk);
      if(NIBBLE_PER_DQS == 2) {
    	wl_dly_rnk = (wl_dly_rnk & 0x1E00) | (0x1FF&read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)+ 1], WLDLYRNK0+current_rank));
        write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], WLDLYRNK0+current_rank, wl_dly_rnk);
      }

      DEBUG_PRINTF("INFO: ddr_write_latency_cal(): ddr_write_latency[%d][%d] = %d, wl_dly_rnk coarse = %d\n", current_rank, i, ddr_write_latency[current_rank][i], crse);
    }
  }

  // Latency Correction Stage
  for(i=0; i<DQS_BITS; i++)
  {
    min_ddr_write_latency[i] = 255;
    for(current_rank = 0; current_rank < RANKS; current_rank += glbl_rank_inc) {
      if(min_ddr_write_latency[i] > ddr_write_latency[current_rank][i])
        min_ddr_write_latency[i] = ddr_write_latency[current_rank][i];
    }

    DEBUG_PRINTF("INFO: ddr_write_latency_cal(): min_ddr_write_latency[%d] = %d\n", i, min_ddr_write_latency[i]);
  }

  for(i=0; i<DQS_BITS; i++)
  {
    if((min_ddr_write_latency[i]/4) != 0) // Take out coarse values
    {
      // Programming the Data Delay Value into the Registers
      data_dly = 0x7 & readIO_DMC(ral_addr_of_phy_data_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*i*NIBBLE_PER_DQS));
      data_dly += min_ddr_write_latency[i]/4;

      DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Nibble = %d, final data_nib delay = %d\n", (i*NIBBLE_PER_DQS), data_dly);
      writeIO_DMC(ral_addr_of_phy_data_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*i*NIBBLE_PER_DQS), data_dly);
   
      if(NIBBLE_PER_DQS == 2) {
        DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Nibble = %d, final data_nib delay = %d\n", (i*NIBBLE_PER_DQS)+1, data_dly);
        writeIO_DMC(ral_addr_of_phy_data_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*i*NIBBLE_PER_DQS) + 4, data_dly);
      }

      // Programming the Ouput EnableDelay Value into the Registers
      for(j=0; j<NIBBLE_PER_DQS; j++)
      {
        phy_oe_dly = 0x07 & readIO_DMC(ral_addr_of_phy_oe_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*glbl_map_riu[(i*NIBBLE_PER_DQS)+j]));
        phy_oe_dly += min_ddr_write_latency[i]/4;

        DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Nibble = %d, final data_nib delay = %d\n", (i*NIBBLE_PER_DQS)+j, phy_oe_dly);
        writeIO_DMC(ral_addr_of_phy_oe_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*glbl_map_riu[(i*NIBBLE_PER_DQS)+j]), phy_oe_dly);   
      }
    }
  }

  // Updating the Rank Delay Value into the Registers
  glbl_error_phy_nibble = 0;
  for(i=0; i<DQS_BITS; i++)
  {
    for(current_rank = 0; current_rank < RANKS; current_rank += glbl_rank_inc)
    {
      diff = (ddr_write_latency[current_rank][i] - min_ddr_write_latency[i]);

      DEBUG_PRINTF("INFO: ddr_write_latency_cal(): diff = %d, write latency = %d, min write latency = %d\n", diff, ddr_write_latency[current_rank][i], min_ddr_write_latency[i]);
  
      wl_dly_rnk = read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], WLDLYRNK0+current_rank);
      DEBUG_PRINTF("INFO: ddr_write_latency_cal(): wl_dly_rnk[%d] = %x \n", i, wl_dly_rnk);

      coarse[current_rank] = (wl_dly_rnk & RIU_WL_DELAY_CRSE_MASK) >> RIU_WL_DELAY_CRSE_OFFSET ;   
      fine[current_rank]   = (wl_dly_rnk & RIU_WL_DELAY_FINE_MASK) >> RIU_WL_DELAY_FINE_OFFSET ;
      DEBUG_PRINTF("INFO: ddr_write_latency_cal(): coarse = %d, fine = %x\n", coarse[current_rank], fine[current_rank]);

      if(diff <= 8) // Check for Rank to Rank spacing of more than 2 tCK
      {
        if((ddr_write_latency[current_rank][i]/4) != (min_ddr_write_latency[i]/4)) {
          coarse[current_rank] += ((ddr_write_latency[current_rank][i]/4) - (min_ddr_write_latency[i]/4)) * 4;
          DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Rank to rank adjustment: coarse = %d, fine = %x\n", coarse[current_rank], fine[current_rank]);

          if(coarse[current_rank] > 7) { // If target coarse is more than 7. Adjust in Fine taps
            dummy_write_multi_rank_switch((current_rank == 0) ? 1 : 0); // Switch to the other rank, while updating the current rank coarse & fine delays
            // rl_dly_qtr = read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], RL_DLY_QTR); // Use the same RL_DLY_QTR across both the nibble to keep it simple
            write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], RD_DBG_RW_INDEX, 0xC); // Select HALFT_DQSM register in the Debug bus
            rl_dly_qtr = (read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], RD_DBG_RD_STATUS) >> 1); // QTR = HALF/2

            fine[current_rank] = fine[current_rank] + (coarse[current_rank] - 7) * rl_dly_qtr;
            coarse[current_rank] = 7;
            DEBUG_PRINTF("INFO: ddr_write_latency_cal(): WL_DLY_RNK Coarse/Fine adjustment: coarse = %d, fine = %x\n", coarse[current_rank], fine[current_rank]);

            if(fine[current_rank] > 350) { // Maximum taps cannot go beyond 350 taps or so, to allow tDQS2DQ variations in LP4 configuration
              DEBUG_PRINTF("ERROR: ddr_write_latency_cal(): write_latency calibration failed due to coarse/find difference across ranks is more\n");
            }
          }

          write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], WLDLYRNK0+current_rank, (coarse[current_rank]<<9) + fine[current_rank]);

          if(NIBBLE_PER_DQS == 2) {
            fine[current_rank] = (0x1FF&read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], WLDLYRNK0+current_rank));
            write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], WLDLYRNK0+current_rank, (coarse[current_rank]<<9) + fine[current_rank]);
          }
          }
      }
      else
      {
        DEBUG_PRINTF("ERROR: ddr_write_latency_cal(): write_latency calibration failed due to coarse difference across ranks is more than 8 value\n");
        glbl_error_phy_nibble |= (0x1 << glbl_map_riu[(i*NIBBLE_PER_DQS)]);
      }
// XRAM_DBG: wrlat_wldlyrnk_crse_final
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRLAT_WLDLYRNK_CRSE_FINAL + ((current_rank * MAX_DQS_BITS + i) * 4), coarse[current_rank]);
    } 
  }
  if(glbl_error_phy_nibble) {
    write_error_nibble_XRAM(1, glbl_error_phy_nibble);
    write_error_XRAM(ERR_CODE_38_WRLAT_HIGHER_RANK_2_RANK_SKEW);
    return 1;
  }

  bitslice_reset_bcast(0, BS_RESET_WT);

  //////////////////////////////////////////
  // Sanity Check Logic
  //////////////////////////////////////////
  writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK),0xB);
  writeIO_DMC(ral_addr_of_seq_a_dq_1_in_ddrmc_main_ddrmc_main_bank(BLK),0x11);
  writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), (0x11 << 5) | 0xB);
  
  #ifdef LPDDR4
    for(current_rank=0; current_rank < RANKS; current_rank++) {
      status = lp4_dqs_osc_read(current_rank, 1);

      if(status == 1) {
        DEBUG_PRINTF("ERROR: ddr_write_latency_cal(): while getting the new DQS oscillator value for Rank = %d\n", current_rank);
        write_error_XRAM(ERR_CODE_46_WRLAT_FAIL_DQS_OSC_VALUE_ZERO);
	writeIO_XRAM(XRAM_CAL_RANK,(1 << current_rank | 1 << 4));
        return 1;
      } else if(status == 2) {
        DEBUG_PRINTF("ERROR: ddr_write_latency_cal(): while getting the new DQS oscillator value for Rank = %d\n", current_rank);
        write_error_XRAM(ERR_CODE_47_WRLAT_FAIL_DQS_OSC_VALUE_OUT_OF_RANGE);
	writeIO_XRAM(XRAM_CAL_RANK,(1 << current_rank | 1 << 4));
        return 1;
      }
    }  

    phy_oe_dly_min = 10;
    phy_oe_dly_max = 0;
    for(i=0; i<DQS_BITS; i++) {
      // FIXME: Check for multi-rank. What if tDQS2DQ values are different across the ranks
      for(current_rank=0; current_rank < RANKS; current_rank++) {
        // rl_dly_qtr = read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], RL_DLY_QTR);
        write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], RD_DBG_RW_INDEX, 0xC); // Select HALFT_DQSM register in the Debug bus
        rl_dly_qtr = (read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], RD_DBG_RD_STATUS) >> 1); // QTR = HALF/2

        lp4_dqs_coarse = read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], LP4_WL_DQS); 
        lp4_dqs_fine = read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], ODLY1) & 0x1FF;

        wl_dly_rnk  = read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], WLDLYRNK0+current_rank);
        lp4_dq_coarse  = (wl_dly_rnk >> 9) & 0xF;
        lp4_dq_fine = wl_dly_rnk & 0x1FF;

        phy_oe_dly = 0x07 & readIO_DMC(ral_addr_of_phy_oe_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*glbl_map_riu[(i*NIBBLE_PER_DQS)]));

	// Compare (DQS coarse + DQS fine + tDQS2DQ) with (DQ coarse + DQ fine + 90 degree) and adjust DQS latency accordingly.
        // t_dqs_to_dq_latency = ((glbl_cur_tdqs2dq[current_rank][i/2] + ((lp4_dqs_coarse - lp4_dq_coarse) * (glbl_tck/4))) / glbl_tck);
	dqs_delay = ((lp4_dqs_coarse * glbl_tck) / 4) + glbl_cur_tdqs2dq[current_rank][i/2] + ((lp4_dqs_fine * glbl_tck) / (4 * rl_dly_qtr));
	dq_delay = ((lp4_dq_coarse * glbl_tck) / 4) + (glbl_tck / 4) + ((lp4_dq_fine * glbl_tck) / (4 * rl_dly_qtr));

        // Add TCK/2 to get the rounding value
	// Add 2 TCK offset to get positive values
	if(dqs_delay > dq_delay) {
	   t_dqs_to_dq_latency = (dqs_delay - dq_delay + (glbl_tck / 2)) / glbl_tck;
	   phy_oe_dly_upd[current_rank][i] = phy_oe_dly + 2 - t_dqs_to_dq_latency;
	} else {
	   t_dqs_to_dq_latency = (dq_delay - dqs_delay + (glbl_tck / 2)) / glbl_tck;
	   phy_oe_dly_upd[current_rank][i] = phy_oe_dly + 2 + t_dqs_to_dq_latency;
	}

	if(phy_oe_dly_upd[current_rank][i] < phy_oe_dly_min) {
	  phy_oe_dly_min = phy_oe_dly_upd[current_rank][i];
        }

	if(phy_oe_dly_upd[current_rank][i] > phy_oe_dly_max) {
	  phy_oe_dly_max = phy_oe_dly_upd[current_rank][i];
        }

        DEBUG_PRINTF("INFO: ddr_write_latency_cal(): lp4_dqs_coarse = %d, lp4_dqs_fine = %d, lp4_dq_coarse = %d, lp4_dq_fine = %d, tdqs2dq = %d, glbl_tck = %d\n", lp4_dqs_coarse, lp4_dqs_fine, lp4_dq_coarse, lp4_dq_fine, glbl_cur_tdqs2dq[current_rank][i/2], glbl_tck);
        DEBUG_PRINTF("INFO: ddr_write_latency_cal(): dqs_delay = %d, dq_delay = %d, t_dqs_to_dq_latency = %d, phy_oe_dly = %d, phy_oe_dly_upd = %d\n", dqs_delay, dq_delay, t_dqs_to_dq_latency, phy_oe_dly, phy_oe_dly_upd[current_rank][i]);
      }
    }
 
    // Adjust the OE ALL delay based on minimum delay. Remove the 2 TCK offset.
    phy_oe_all_dly_final = (min_required_lat + phy_oe_dly_min - 2);
    DEBUG_PRINTF("INFO: ddr_write_latency_cal(): min_required_lat = %d, phy_oe_dly_min = %d, phy_oe_dly_max = %d, final oe_all delay = %d\n", min_required_lat, phy_oe_dly_min, phy_oe_dly_max, phy_oe_all_dly_final);
    writeIO_DMC(ral_addr_of_xpi_oe_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK), phy_oe_all_dly_final | (xpi_oe_all_nib & 0x780));
// XRAM_DBG: wrlat_xpi_oe_all_final
  writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRLAT_XPI_OE_ALL_FINAL, phy_oe_all_dly_final);

    for(i=0; i<DQS_BITS; i++) {
      for(current_rank=0; current_rank < RANKS; current_rank++) {
        phy_oe_dly_upd[current_rank][i] -= phy_oe_dly_min;
      }
      phy_oe_dly = phy_oe_dly_upd[0][i];

      DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Nibble = %d, final phy_oe_nib delay = %d\n", i*NIBBLE_PER_DQS, phy_oe_dly);
      writeIO_DMC(ral_addr_of_phy_oe_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*glbl_map_riu[(i*NIBBLE_PER_DQS)]) , phy_oe_dly);

      // Add extra cycle delay on non-DQS nibble to compensate for LP4 crse in DQS nibble
      lp4_dqs_coarse = read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], LP4_WL_DQS); 
      phy_oe_dly += (lp4_dqs_coarse != 0);

      DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Nibble = %d, final phy_oe_nib delay = %d\n", (i*NIBBLE_PER_DQS)+1, phy_oe_dly);
      writeIO_DMC(ral_addr_of_phy_oe_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*glbl_map_riu[(i*NIBBLE_PER_DQS)+1]) , phy_oe_dly);
    }

    xpi_oe_pre_post = glbl_xpi_oe_val;

    if(((xpi_oe_pre_post >> 4) & 0xF) < 2) { // Make sure minimum postamble is 2 tCK, to cover the tri-state for tDQS2DQ
      xpi_oe_pre_post = (xpi_oe_pre_post & ~(0xF0)) | 0x20;
    }
 
    writeIO_DMC(ral_addr_of_xpi_oe_in_ddrmc_main_ddrmc_main_bank(BLK), xpi_oe_pre_post);
  #endif

    xpi_oe_all_nib     = readIO_DMC(ral_addr_of_xpi_oe_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK));
    xpi_wrdata_all_nib = readIO_DMC(ral_addr_of_xpi_wrdata_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK)); 

#ifdef S80_DEVICE
  #if SI_REVISION == es1           // EDT-995671 issue
    adjust_oe_all_nib = ((xpi_oe_all_nib & 0x7F) > (xpi_wrdata_all_nib & 0x7F)) ? ((xpi_oe_all_nib & 0x7F) - (xpi_wrdata_all_nib & 0x7F)) : 0;
    adjust_wrdata_all_nib = ((xpi_wrdata_all_nib & 0x7F) > (xpi_oe_all_nib & 0x7F)) ? ((xpi_wrdata_all_nib & 0x7F) - (xpi_oe_all_nib & 0x7F)) : 0;
  #else
    adjust_oe_all_nib = 0;
    adjust_wrdata_all_nib = 0;
  #endif
#else
  adjust_oe_all_nib = 0;
  adjust_wrdata_all_nib = 0;
#endif

    if(adjust_oe_all_nib != 0) {
      xpi_oe_all_nib     -= adjust_oe_all_nib;
      writeIO_DMC(ral_addr_of_xpi_oe_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK), xpi_oe_all_nib);
// XRAM_DBG: wrlat_xpi_oe_all_final
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRLAT_XPI_OE_ALL_FINAL, xpi_oe_all_nib & 0x7F);

      for(i=0; i<DQS_BITS*NIBBLE_PER_DQS; i++) {
        phy_oe_dly = readIO_DMC(ral_addr_of_phy_oe_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*glbl_map_riu[i]));   
	phy_oe_dly += adjust_oe_all_nib;
        writeIO_DMC(ral_addr_of_phy_oe_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*glbl_map_riu[i]), phy_oe_dly);   
      }
    }

    if(adjust_wrdata_all_nib != 0) {
      xpi_wrdata_all_nib -= adjust_wrdata_all_nib;
      writeIO_DMC(ral_addr_of_xpi_wrdata_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK), xpi_wrdata_all_nib);
// XRAM_DBG: wrlat_xpi_wrdata_all_final
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRLAT_XPI_WRDATA_ALL_FINAL, xpi_wrdata_all_nib & (0x7F));

      for(i=0; i<DQS_BITS*NIBBLE_PER_DQS; i++) {
        data_dly = readIO_DMC(ral_addr_of_phy_data_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*i));
        data_dly += adjust_wrdata_all_nib;
        writeIO_DMC(ral_addr_of_phy_data_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*i), data_dly);
      }
    }

  for(current_rank = 0; current_rank < RANKS; current_rank += glbl_rank_inc)
  { 
    // Clearing the Compare Status
    writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);
    writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

    bitslice_reset_bcast(0, BS_RESET_WT);

    #ifdef DDR4
    // ACTIVATE COMMAND
      set_1_seq_command_at_mem(current_rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                           MEM_ROW_ACTIVE /*u32 seq_a_cmd*/, 0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/,
                           0 /*u32 seq_a2a_dly*/);

    // Single Read And Write Command
      set_2_seq_command_at_mem(current_rank /* rank */, 2 /* no of iteration */, first_iteration /* full write */, MEM_COL_WRITE /* seq_a_cmd */,
                            0 /* seq_a_addr */, 0 /* seq_a_bg */, 0 /* seq_ba */, MEM_COL_READ /* seq_b_cmd */, 
                            0 /*seq_b_addr*/, 0 /*seq_b_bg*/,0 /*seq_b_ba*/, 40 /*seq_a2b_dly*/, 40/*seq_b2a_dly*/);

    // PRECHARGE COMMAND
      set_1_seq_command_at_mem(current_rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                          MEM_ROW_PREACHARGE /*u32 seq_a_cmd*/, 0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/,
                          0/*u32 seq_a2a_dly*/);
    #else
      lp4_bank_row_access(current_rank /*u8 rank*/, ACTIVATE /*cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);
      lp4_bank_col_access(2, COL_WR, COL_RD, 0, 0);  
      lp4_bank_row_access(current_rank /*u8 rank*/, PRECHARGE /*u8 cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);
    #endif

    dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_nibble_compare_17_0;
    DEBUG_PRINTF("INFO: ddr_write_latency_cal(): dq_compare_result = %x \n", dq_compare_result);
    if(dq_compare_result) { // ERROR case
      for(i=0; i<DQS_BITS; i++) {
        rd_data = (readIO_DMC(ral_addr_of_bl8_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*i*NIBBLE_PER_DQS)));
  
      #ifdef DDR4
        if(rd_data != WR_LATENCY_SANITY_TEST_PATTERN) {
          DEBUG_PRINTF("ERROR: ddr_write_latency_cal(): write_latency calibration failed in sanity check. rd_data = %x \n", rd_data);
        }
      #else
        bl16_rd_data = (readIO_DMC(ral_addr_of_bl16_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*i*NIBBLE_PER_DQS)));

        if((rd_data != LP4_WR_LATENCY_SANITY_TEST_PATTERN_0) || (bl16_rd_data != LP4_WR_LATENCY_SANITY_TEST_PATTERN_1)) {
          DEBUG_PRINTF("ERROR: ddr_write_latency_cal(): write_latency calibration failed in sanity check. rd_data = {%x, %x} \n", rd_data, bl16_rd_data);
        }
      #endif
      }

      write_error_nibble_XRAM(0, dq_compare_result);
      write_error_XRAM(ERR_CODE_39_WRLAT_FAIL_POST_SANITY_CHECK);
      writeIO_XRAM(XRAM_CAL_RANK,(1 << current_rank | 1 << 4));
      return 1;
    }
  }

  bitslice_reset_bcast(0, BS_RESET_WT);
 
  #ifdef DDR4
  if(glbl_wr_pre_2tck_mode) { // 2 tCK write preamble mode
    for(current_rank = 0; current_rank < RANKS; current_rank += glbl_rank_inc) {
      DEBUG_PRINTF("INFO: ddr_write_latency_cal(): MR4: Setting memory back into 2 tCK write preamble mode\n");
      set_1_seq_command_at_mem(current_rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, 
                               glbl_mr4 /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x0 /*u16 seq_a_ba*/, 0 /*u32 seq_a2a_dly*/);

      if(glbl_wr_pre_2tck_cwl_adjust) { // Adjust CWL as needed
        DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Adding the write latency as per 2 tCK write preamble mode\n");
        set_1_seq_command_at_mem(current_rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, 
                                 glbl_mr2 /*u32 seq_a_addr*/, 0x0 /*u8 seq_a_bg*/, 0x2 /*u16 seq_a_ba*/, 0 /*u32 seq_a2a_dly*/);

        if(current_rank == 0) { // Only once
          glbl_write_latency = glbl_init_write_latency;

          xpi_oe_all_nib     = readIO_DMC(ral_addr_of_xpi_oe_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK));
          xpi_wrdata_all_nib = readIO_DMC(ral_addr_of_xpi_wrdata_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK)); 

          xpi_oe_all_nib     += 2; // Adjust the calibrated values
          xpi_wrdata_all_nib += 2;

          writeIO_DMC(ral_addr_of_xpi_oe_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK), xpi_oe_all_nib);
          writeIO_DMC(ral_addr_of_xpi_wrdata_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK), xpi_wrdata_all_nib);

// XRAM_DBG: wrlat_xpi_oe_all_final
// XRAM_DBG: wrlat_xpi_wrdata_all_final
          writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRLAT_XPI_OE_ALL_FINAL, xpi_oe_all_nib & 0x7F);
          writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRLAT_XPI_WRDATA_ALL_FINAL, xpi_wrdata_all_nib & (0x7F));
        }
      }
    } // RANKS
  }
  #endif

  DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Do the sanity check 1\n");
  status = sanity_check_1();
  if(status != 0) {
    DEBUG_PRINTF("ERROR: ddr_write_latency_cal(): Sanity check 1 Failed\n");
    write_error_XRAM(ERR_CODE_40_WRLAT_FAIL_SANITY_CHECK_1);
    return 1;
  } else {
    DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Sanity check 1 Passed\n");
  }

  for(i=0; i<DQS_BITS; i++) {
// XRAM_DBG: wrlat_phy_oe_nib_final
    phy_oe_dly = 0x07 & readIO_DMC(ral_addr_of_phy_oe_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*glbl_map_riu[(i*NIBBLE_PER_DQS)+j]));
    writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRLAT_PHY_OE_NIB_FINAL + i*4, phy_oe_dly);

// XRAM_DBG: wrlat_phy_data_nib_final
    data_dly = 0x7 & readIO_DMC(ral_addr_of_phy_data_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*i*NIBBLE_PER_DQS));
    writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRLAT_PHY_DATA_NIB_FINAL + i*4, data_dly);
  }

  DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Do the sanity check 2\n");
  status = sanity_check_2();
  if(status != 0) {
    DEBUG_PRINTF("ERROR: ddr_write_latency_cal(): Sanity check 2 Failed\n");
    write_error_XRAM(ERR_CODE_41_WRLAT_FAIL_SANITY_CHECK_2);
    return 1;
  } else {
    DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Sanity check 2 Passed\n");
  }

  u32 loop_cnt = 1;

  status = sanity_check_complex_read(loop_cnt);
  if(status != 0) {
    DEBUG_PRINTF("ERROR: ddr_write_latency_cal(): Sanity check: Complex Read Failed\n");
    write_error_XRAM(ERR_CODE_42_WRLAT_FAIL_RD_COMPLEX_SANITY_CHECK);
    return 1;
  } else {
    DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Sanity check: Complex Read Passed\n");
  }

  status = sanity_check_complex_write(loop_cnt);
  if(status != 0) {
    DEBUG_PRINTF("ERROR: ddr_write_latency_cal(): Sanity check: Complex Write Failed\n");
    write_error_XRAM(ERR_CODE_43_WRLAT_FAIL_WR_COMPLEX_SANITY_CHECK);
    return 1;
  } else {
    DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Sanity check: Complex Write Passed\n");
  }

// Removing PRBS sanity checks as they may not pass at higher write latencies: EDT-1001168
//  status = sanity_check_prbs_read(loop_cnt);
//  if(status != 0) {
//    DEBUG_PRINTF("ERROR: ddr_write_latency_cal(): Sanity check: PRBS Read Failed\n");
//    write_error_XRAM(ERR_CODE_44_WRLAT_FAIL_RD_PRBS_SANITY_CHECK);
//    return 1;
//  } else {
//    DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Sanity check: PRBS Read Passed\n");
//  }
//
//  status = sanity_check_prbs_write(loop_cnt);
//  if(status != 0) {
//    DEBUG_PRINTF("ERROR: ddr_write_latency_cal(): Sanity check: PRBS Write Failed\n");
//    write_error_XRAM(ERR_CODE_45_WRLAT_FAIL_WR_PRBS_SANITY_CHECK);
//    return 1;
//  } else {
//    DEBUG_PRINTF("INFO: ddr_write_latency_cal(): Sanity check: PRBS Write Passed\n");
//  }

  return 0;
}
