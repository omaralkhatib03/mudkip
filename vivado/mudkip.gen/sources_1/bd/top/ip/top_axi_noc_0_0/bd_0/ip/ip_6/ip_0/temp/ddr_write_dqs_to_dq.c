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
//       Revision:       $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/ddr_write_dqs_to_dq.c#2 $
//                       $Author: rgaddam $
//                       $DateTime: 2019/02/19 07:08:51 $
//                       $Change: 20624036 $
//       Description:
//           DDRMC : 
//
/////////////////////////////////////////////////////////////////////////////
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

u8 ddr_write_dqs_to_dq ()
{
  u16 dqs_delay[DQS_BITS];
  u16 dq_delay[DQS_BITS*4*NIBBLE_PER_DQS];
  u16 dbi_delay[DQS_BITS];  
  u16 dqs_right_delay[DQS_BITS];
  u16 left_margin[DQS_BITS];
  u8 status;
  u8 i, j, rank;
  u8 nibble, phy_nibble, cal_crse;
  u16 cal_fine, wl_dly_rnk;
  u32 extend_write_data;
  u32 xpi_wrdata_all_nib, xpi_oe_all_nib;
  u32 mr2_1tck_wr_pre;
  u8 bit, byte;
  u32 addr, odly;
  static u16 dqs_right_delay_init[DQS_BITS];

  // Nibble Wise Reset Mask Setting FOR DQS
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK0, glbl_nibble_reset_mask[0]);
  riu_reg_bcast(CFG_BS_RST_MASK, 0, ~(0x7F));

  // Nibble Wise Reset Mask Setting FOR DM
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK0, glbl_nibble_reset_mask[1]);
  riu_reg_bcast(CFG_BS_RST_MASK, 0, ~(0x7D));

  writeIO_RIU( RPI_BCAST_NIBBLE_MASK0, (glbl_nibble_reset_mask[0] | glbl_nibble_reset_mask[1]));

  #ifdef DDR4
  if(glbl_tck >= TCK_TH_WR_CAL) { // SKIP write centering stage
    DEBUG_PRINTF("INFO: ddr_write_dqs_to_dq(): SKIPPING write centering stage for TCK = %d\n", glbl_tck);

  } else { // TCK_TH_WR_CAL

    // Precharge Command
    set_1_seq_command_at_mem(0 /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                             MEM_ROW_PREACHARGE /*u32 seq_a_cmd*/, 0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/,
                             0/*u32 seq_a2a_dly*/);
    if(glbl_wr_dbi_en)
    {
      // MR5 write on the Memory to Disable Write DBI/DM and Read DBI
      set_1_seq_command_at_mem(0 /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, 
                              (glbl_mr5[0] & 0xFFFFE3FF) /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x1 /*u16 seq_a_ba*/, 0 /*u32 seq_a2a_dly*/);

      writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
    }

    if(glbl_wr_pre_2tck_mode) { // 2 tCK write preamble mode
      DEBUG_PRINTF("INFO: ddr_write_dqs_to_dq(): MR4: Setting memory into 1 tCK write preamble mode since write latency is not figured out yet!!\n");
      set_1_seq_command_at_mem(0 /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, 
                              (glbl_mr4 & ~(0x1000)) /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x0 /*u16 seq_a_ba*/, 0 /*u32 seq_a2a_dly*/);

      if(glbl_wr_pre_2tck_cwl_adjust) { // Adjust CWL as needed
        DEBUG_PRINTF("INFO: ddr_write_dqs_to_dq(): Reducing the write latency as per 1 tCK write preamble mode\n");
        mr2_1tck_wr_pre = (glbl_mr2 - 0x8) ; // mr2[5:3]--
        set_1_seq_command_at_mem(0 /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, 
                                 mr2_1tck_wr_pre /*u32 seq_a_addr*/, 0x0 /*u8 seq_a_bg*/, 0x2 /*u16 seq_a_ba*/, 0 /*u32 seq_a2a_dly*/);

        glbl_write_latency = glbl_init_write_latency - 2;

        xpi_oe_all_nib     = readIO_DMC(ral_addr_of_xpi_oe_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK));
        xpi_wrdata_all_nib = readIO_DMC(ral_addr_of_xpi_wrdata_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK)); 

        xpi_oe_all_nib     = (xpi_oe_all_nib & ~(0x7F)) | glbl_write_latency;
        xpi_wrdata_all_nib = (xpi_wrdata_all_nib & ~(0x7F)) | glbl_write_latency;

        writeIO_DMC(ral_addr_of_xpi_oe_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK), xpi_oe_all_nib);
        writeIO_DMC(ral_addr_of_xpi_wrdata_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK), xpi_wrdata_all_nib);
      }
    }

    // Activate Command
    set_1_seq_command_at_mem(0 /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, MEM_ROW_ACTIVE /*u32 seq_a_cmd*/, 
                             0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/, 0 /*u32 seq_a2a_dly*/);

    // Setting for getting Read Data from memory synchronously
    writeIO_DMC(ral_addr_of_seq_data_capture_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

    // Writing Pre, Main and Post Data
    writeIO_DMC(ral_addr_of_seq_a_dq_early_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
    writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
    writeIO_DMC(ral_addr_of_seq_a_dq_late_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

    // Extend Mode Enabled
    extend_write_data = readIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK));
    writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (extend_write_data | 0x00000009));  

    // Initialize all the Delays
    for(i=0; i<DQS_BITS; i++)
    {
      dqs_delay[i] = 0;
      dbi_delay[i] = 0;
      dqs_right_delay[i] = 0;
      left_margin[i] = 0;
 
      for(j=0; j<(4*NIBBLE_PER_DQS); j++)
        dq_delay[(i*4*NIBBLE_PER_DQS)+j] = 0;     
    }

    // Parallel Bitslice Reset
    bitslice_reset_bcast(0, BS_RESET_WT);

    /////////////////////////////////////////////////////
    // Operation Needed in DDR4 Per Bit Deskew //////////
    // ////////////////////////////////////////////////// 
    DEBUG_PRINTF("INFO: ddr_write_dqs_to_dq(): Start Finding the Common DQ valid Window \n");

    // Finding the Valid Window By Moving DQS First and then DQ
    status = get_dq_common_valid_window(dqs_delay, dq_delay);

// XRAM_DBG: wrdqdbi_stg3_dq_delay
    for(bit=0; bit<DQ_BITS; bit++) {
      addr = (glbl_dq_bits_riu_map[bit][0] << RIU_NIB_POS) | (glbl_dq_bits_riu_map[bit][1] + ODLY0);
      odly = readIO_RIU(addr) & 0x1FF;
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG3_DQ_ODLY + (bit * 4), odly);
    }

    if(status) {
      write_error_XRAM(ERR_CODE_21_WR_DQ_DBI_FAIL_VALID_WINDOW_DQS_DQ_ODELAY);
      return 1;
    }

    DEBUG_PRINTF("INFO: ddr_write_dqs_to_dq(): Finished Finding the Common DQ valid Window \n");

    // Parallel Bitslice Reset
    bitslice_reset_bcast(0, BS_RESET_WT);
  
    // Moving DQ to Find Per Bit DESKEW
    status = get_dq_per_bit_deskew(dq_delay, FINE_CRS_STEP_SIZE);

// XRAM_DBG: wrdqdbi_deskew_dq_odly_fcrse
    for(bit=0; bit<DQ_BITS; bit++) {
      addr = (glbl_dq_bits_riu_map[bit][0] << RIU_NIB_POS) | (glbl_dq_bits_riu_map[bit][1] + ODLY0);
      odly = readIO_RIU(addr) & 0x1FF;
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_DESKEW_DQ_ODLY_FCRSE + (bit * 4), odly);
    }
 
    if(status) {
      write_error_XRAM(ERR_CODE_22_WR_DQ_DBI_FAIL_NOISE_DQ_ODELAY_HIGHER_TAPS);
      write_error_bit_XRAM();
      return 1;
    }

    status = get_dq_per_bit_deskew(dq_delay, 1);

// XRAM_DBG: wrdqdbi_deskew_dq_odly
    for(bit=0; bit<DQ_BITS; bit++) {
      addr = (glbl_dq_bits_riu_map[bit][0] << RIU_NIB_POS) | (glbl_dq_bits_riu_map[bit][1] + ODLY0);
      odly = readIO_RIU(addr) & 0x1FF;
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_DESKEW_DQ_ODLY + (bit * 4), odly);
    }
 
    if(status) {
      write_error_XRAM(ERR_CODE_23_WR_DQ_DBI_FAIL_NOISE_DQ_ODELAY);
      write_error_bit_XRAM();
      return 1;
    }

    // Parallel Bitslice Reset
    bitslice_reset_bcast(0, BS_RESET_WT);

    if(glbl_wr_dbi_en)
    {
      // DBI Enable in the CAL RTL
      writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 2);

      // Precharge a Bank
      set_1_seq_command_at_mem(0 /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, MEM_ROW_PREACHARGE /*u32 seq_a_cmd*/, 
                               0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/, 0x0 /*u32 seq_a2a_dly*/);

      // Enabling the write DBI into the Memory (Disable DM / READ DBI)
      set_1_seq_command_at_mem(0 /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, 
                               ((glbl_mr5[0] | 0x800) & 0xFFFFEBFF) /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x1 /*u16 seq_a_ba*/, 0x0 /*u32 seq_a2a_dly*/);

      // Activate Command
      set_1_seq_command_at_mem(0 /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, MEM_ROW_ACTIVE /*u32 seq_a_cmd*/, 
                               0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/, 0 /*u32 seq_a2a_dly*/);

      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (extend_write_data | 0x00000009));  

      // Finding The Valid Window By Moving DQS First and then DBI
      status = get_dbi_common_valid_window(dqs_delay, dbi_delay);

// XRAM_DBG: wrdqdbi_stg6_dbi_odly
      for(byte=0; byte<DQS_BITS; byte++) {
        addr = (glbl_dbi_bits_riu_map[byte][0] << RIU_NIB_POS) | (glbl_dbi_bits_riu_map[byte][1] + ODLY0);
        odly = readIO_RIU(addr) & 0x1FF;
        writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG6_DBI_ODLY + (byte * 4), odly);
      }

      if(status) {
        write_error_XRAM(ERR_CODE_24_WR_DQ_DBI_FAIL_VALID_WINDOW_DQS_DBI_ODELAY);
        return 1;
      }

      DEBUG_PRINTF("INFO: ddr_write_dqs_to_dq(): DBI dskew is started with Large Step \n");

      // Moving DBI to Find Per Bit DESKEW 
      status = get_dbi_bit_deskew(dbi_delay, FINE_CRS_STEP_SIZE);

// XRAM_DBG: wrdqdbi_deskew_dbi_odly_fcrse
      for(byte=0; byte<DQS_BITS; byte++) {
        addr = (glbl_dbi_bits_riu_map[byte][0] << RIU_NIB_POS) | (glbl_dbi_bits_riu_map[byte][1] + ODLY0);
        odly = readIO_RIU(addr) & 0x1FF;
        writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_DESKEW_DBI_ODLY_FCRSE + (byte * 4), odly);
      }

      if(status) {
        write_error_XRAM(ERR_CODE_25_WR_DQ_DBI_FAIL_NOISE_DBI_ODELAY_HIGHER_TAPS);
        return 1;
      }

      DEBUG_PRINTF("INFO: ddr_write_dqs_to_dq(): DBI dskew is started with Small Step \n");

      status = get_dbi_bit_deskew(dbi_delay, 1);

// XRAM_DBG: wrdqdbi_deskew_dbi_odly
      for(byte=0; byte<DQS_BITS; byte++) {
        addr = (glbl_dbi_bits_riu_map[byte][0] << RIU_NIB_POS) | (glbl_dbi_bits_riu_map[byte][1] + ODLY0);
        odly = readIO_RIU(addr) & 0x1FF;
        writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_DESKEW_DBI_ODLY + (byte * 4), odly);
      }

      if(status) {
        write_error_XRAM(ERR_CODE_26_WR_DQ_DBI_FAIL_NOISE_DBI_ODELAY);
        return 1;
      }
    }

    // Left Margin Calculation
    status = get_delay_calculation(left_margin, dq_delay, dbi_delay);

// XRAM_DBG: wrdqdbi_left_margin
    for(byte=0; byte<DQS_BITS; byte++) {
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_LEFT_MARGIN + (byte * 4), left_margin[byte]);
    }
// XRAM_DBG: wrdqdbi_left_edge_dq
    for(bit=0; bit<DQ_BITS; bit++) {
      addr = (glbl_dq_bits_riu_map[bit][0] << RIU_NIB_POS) | (glbl_dq_bits_riu_map[bit][1] + ODLY0);
      odly = readIO_RIU(addr) & 0x1FF;
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_LEFT_EDGE_DQ + (bit * 4), odly);
    }
// XRAM_DBG: wrdqdbi_left_edge_dbi
    if(glbl_wr_dbi_en) {
      for(byte=0; byte<DQS_BITS; byte++) {
        addr = (glbl_dbi_bits_riu_map[byte][0] << RIU_NIB_POS) | (glbl_dbi_bits_riu_map[byte][1] + ODLY0);
        odly = readIO_RIU(addr) & 0x1FF;
        writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_LEFT_EDGE_DBI + (byte * 4), odly);
      }
    }

    // Parallel Bitslice Reset
    bitslice_reset_bcast(0, BS_RESET_WT);
  
    for(i=0; i<DQS_BITS; i++) {
      dqs_right_delay_init[i] = dqs_right_delay[i];
    }

    // Moving DQS to Find The Right Margin
    status = get_right_edge(dqs_right_delay, FINE_CRS_STEP_SIZE, SIMPLE);

// XRAM_DBG: wrdqdbi_right_margin_fcrse
    for(byte=0; byte<DQS_BITS; byte++) {
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_RIGHT_MARGIN_FCRSE + (byte * 4), dqs_right_delay[byte] - dqs_right_delay_init[byte]);
    }

    if(status) {
      write_error_XRAM(ERR_CODE_27_WR_DQ_DBI_FAIL_RIGHT_EDGE_DQS_ODELAY_HIGHER_TAPS);
      return 1;
    }

    status = get_right_edge(dqs_right_delay, 1, SIMPLE);

// XRAM_DBG: wrdqdbi_right_margin
    for(byte=0; byte<DQS_BITS; byte++) {
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_RIGHT_MARGIN + (byte * 4), dqs_right_delay[byte] - dqs_right_delay_init[byte]);
    }

// XRAM_DBG: wrdqdbi_right_edge_dqs
    for(byte=0; byte<DQS_BITS; byte++) {
      addr = (glbl_dqs_bits_riu_map[byte][0] << RIU_NIB_POS) | (glbl_dqs_bits_riu_map[byte][1] + ODLY0);
      odly = readIO_RIU(addr) & 0x1FF;
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_RIGHT_EDGE_DQS + (byte * 4), odly);
    }

    // Precharge Command
    DEBUG_PRINTF("INFO: ddr_write_dqs_to_dq(): Precharge the bank. It is possible that memory would have gone into error state due to tDQSS violation while moving DQS\n");
    set_1_seq_command_at_mem(0 /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, MEM_ROW_PREACHARGE /*u32 seq_a_cmd*/, 
                             0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/, 0/*u32 seq_a2a_dly*/);

    if(status) {
      write_error_XRAM(ERR_CODE_28_WR_DQ_DBI_FAIL_RIGHT_EDGE_DQS_ODELAY);
      return 1;
    }

    // Parallel Bitslice Reset
    bitslice_reset_bcast(0, BS_RESET_WT);
  
    // Centering Operation
    status = get_centre(dqs_delay, dq_delay, dbi_delay, left_margin, dqs_right_delay);

// XRAM_DBG: wrdqdbi_odly_dqs_final
    for(byte=0; byte<DQS_BITS; byte++) {
      addr = (glbl_dqs_bits_riu_map[byte][0] << RIU_NIB_POS) | (glbl_dqs_bits_riu_map[byte][1] + ODLY0);
      odly = readIO_RIU(addr) & 0x1FF;
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_ODLY_DQS_FINAL + (byte * 4), odly);
    }
// XRAM_DBG: wrdqdbi_odly_dq_final
    for(bit=0; bit<DQ_BITS; bit++) {
      addr = (glbl_dq_bits_riu_map[bit][0] << RIU_NIB_POS) | (glbl_dq_bits_riu_map[bit][1] + ODLY0);
      odly = readIO_RIU(addr) & 0x1FF;
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_ODLY_DQ_FINAL + (bit * 4), odly);
    }
// XRAM_DBG: wrdqdbi_odly_dbi_final
    if(glbl_wr_dbi_en) {
      for(byte=0; byte<DQS_BITS; byte++) {
        addr = (glbl_dbi_bits_riu_map[byte][0] << RIU_NIB_POS) | (glbl_dbi_bits_riu_map[byte][1] + ODLY0);
        odly = readIO_RIU(addr) & 0x1FF;
        writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_ODLY_DBI_FINAL + (byte * 4), odly);
      }
    }

    // Parallel Bitslice Reset
    bitslice_reset_bcast(0, BS_RESET_WT);

    // Activate Command
    set_1_seq_command_at_mem(0 /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, MEM_ROW_ACTIVE /*u32 seq_a_cmd*/, 
                             0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/, 0 /*u32 seq_a2a_dly*/);

    // Sanity Check Stage
    status = get_wr_center_sanity(0, 0); 

    if(status) {
      write_error_XRAM(ERR_CODE_29_WR_DQ_DBI_FAIL_POST_SANITY_CHECK);
      return 1;
    }

    // Precharge Command
    set_1_seq_command_at_mem(0 /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, MEM_ROW_PREACHARGE /*u32 seq_a_cmd*/, 
                             0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/, 0/*u32 seq_a2a_dly*/);

  } // TCK_TH_WR_CAL: SKIP write centering stage

    // Disable The Write DBI if DM is enabled
    if((glbl_mr5[0] & 0x400) != 0)
    {
      set_1_seq_command_at_mem(0 /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, 
                               (glbl_mr5[0] & 0xFFFFE7FF) /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x1 /*u16 seq_a_ba*/, 0 /*u32 seq_a2a_dly*/);
    
      // Write DM Enable in the CAL RTL
      writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
    }

  #else // Operation Needed In LPDDR4 Operation

    u8 dq_coarse[DQS_BITS], dbi_coarse[DQS_BITS];
    u8 dq_fine[DQS_BITS], dbi_fine[DQS_BITS];
    u16 valid_width[DQS_BITS];

  for(rank=0; rank < RANKS; rank++) {

    if(glbl_tck >= TCK_TH_WR_CAL) { // SKIP write centering stage
      DEBUG_PRINTF("INFO: ddr_write_dqs_to_dq(): SKIPPING write centering stage for Rank = %d, for TCK = %d\n", rank, glbl_tck);

      status = lp4_dqs_osc_read(rank, 0);
      if(status == 1) {
        DEBUG_PRINTF("ERROR: ddr_write_dqs_to_dq(): while getting the new DQS oscillator value for Rank = %d\n", rank);
        write_error_XRAM(ERR_CODE_35_WR_DQ_DBI_FAIL_DQS_OSC_VALUE_ZERO);
	writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
        return 1;
      } else if(status == 2) {
        DEBUG_PRINTF("ERROR: ddr_write_dqs_to_dq(): while getting the new DQS oscillator value for Rank = %d\n", rank);
        write_error_XRAM(ERR_CODE_36_WR_DQ_DBI_FAIL_DQS_OSC_VALUE_OUT_OF_RANGE);
	writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
        return 1;
      }

      for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
        phy_nibble = glbl_map_riu[nibble];
        DEBUG_PRINTF("INFO: ddr_write_dqs_to_dq(): Rank = %d, Nibble = %d, TCK = %d, tDQS2DQ = %d, rl_dly_qtr = %d\n", rank, nibble, glbl_tck, glbl_cur_tdqs2dq[rank][nibble/4], glbl_rl_dly_qtr[phy_nibble]);

        if(glbl_cur_tdqs2dq[rank][nibble/4] >= (glbl_tck / 4)) { // tDQS2DQ >= TCK/4
          cal_crse = 0;
          cal_fine = ((glbl_cur_tdqs2dq[rank][nibble/4] - (glbl_tck / 4)) * glbl_rl_dly_qtr[phy_nibble]) / (glbl_tck / 4);
        } else { // tDQS2DQ < TCK/4
          cal_crse = 3; // 270 degrees (-90)
          cal_fine = (glbl_cur_tdqs2dq[rank][nibble/4] * glbl_rl_dly_qtr[phy_nibble]) / (glbl_tck / 4);
        }
        wl_dly_rnk = (cal_crse << 9) | (cal_fine & 0x1FF);
        DEBUG_PRINTF("INFO: ddr_write_dqs_to_dq(): Rank = %d, Nibble = %d, wl_dly_rnk = %x, coarse = %d, fine = %d\n", rank, nibble, wl_dly_rnk, cal_crse, cal_fine);
        write_riu_nibble_reg(phy_nibble, WLDLYRNK0+rank, wl_dly_rnk);

        if(rank == 0) { // Load for Rank 0 only to avoid rank switching
          write_riu_nibble_reg(phy_nibble, ODLY2, cal_fine);
          write_riu_nibble_reg(phy_nibble, ODLY3, cal_fine);
          write_riu_nibble_reg(phy_nibble, ODLY4, cal_fine);
          write_riu_nibble_reg(phy_nibble, ODLY5, cal_fine);

          if(nibble%2 == 1) { // DM/DBI Nibble
            write_riu_nibble_reg(phy_nibble, ODLY0, cal_fine); // DM/DBI pin
          }
        }
      }

      // Parallel Bitslice Reset
      bitslice_reset_bcast(0, BS_RESET_WT);

      if(rank == 0) { // Dummy write for Rank 0, since ODLYs are loaded
        if(glbl_phy_rank_switch_en == 1) {
          DEBUG_PRINTF("INFO: ddr_write_dqs_to_dq(): Dummy write for setting correct odelays\n");
          dummy_write_multi_rank_switch(0);
        }
      }

    } else { // TCK_TH_WR_CAL
    DEBUG_PRINTF("INFO: ddr_write_dqs_to_dq(): Starting calibration for Rank = %d\n", rank);

    // Initializing all the coarse delay values
    for(i=0; i<DQS_BITS; i++)
    {
      dq_coarse[i] = 0;
      dq_fine[i] = 0;
      valid_width[i] = 0;
    }

    if(glbl_wr_dbi_en)
    {
      // Mode Register Write For Write DBI/READ DBI Disable
      lp4_mode_reg_wr(rank, 3, (glbl_lp4_mr3 & 0x3F));

      // Mode Register Write For Write DM disable
      lp4_mode_reg_wr(rank, 0xd, (glbl_lp4_mr13 | 0x20));

      writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
    }

    // Activate Command
    lp4_bank_row_access(rank /*u8 rank*/, ACTIVATE /*cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);

    // Setting for getting Read Data from memory synchronously
    writeIO_DMC(ral_addr_of_seq_data_capture_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

    // Writing Pre, Main and Post Data
    writeIO_DMC(ral_addr_of_seq_a_dq_early_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
    writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
    writeIO_DMC(ral_addr_of_seq_a_dq_1_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
    writeIO_DMC(ral_addr_of_seq_a_dq_late_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

    // Extend Mode Enabled
    extend_write_data = readIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK));
    writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (extend_write_data | 0x00000009));  

    // Clear the DQ ODELAYs before starting the calibration. DQS ODELAY remains at write leveling value.
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, (~glbl_dq_nibble_mask_initial));
    for(i=2; i<6; i++) {
       riu_reg_bcast(ODLY0 + i, 1, 0x0); 
    }

    // Clear the DBI ODELAYs
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, (~glbl_dbi_nibble_mask_initial));
    riu_reg_bcast(ODLY0 + 0, 1, 0x0); // DBI is on bit-slice 0

    // Parallel Bitslice Reset
    bitslice_reset_bcast(0, BS_RESET_WT);

    status = get_lp4_dq_dbi_region(BITS /*u8 data_type*/, DEEP_NOISE /*u8 region_type*/, rank /*u8 rank*/, dq_coarse /*u8 *coarse*/, dq_fine /*u8 *fine*/, 1 /*u8 step_size*/, INC /*u8 inc_dec*/);

// XRAM_DBG: wrdqdbi_stg1c_wldlyrnk_crse: per_dqs, stage 1 final value
    for(i=0; i<DQS_BITS; i++) {
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG1C_WLDLYRNK_CRSE + (i * 4), dq_coarse[i]);
    }

    if(status == 1) {
      write_error_XRAM(ERR_CODE_122_WR_DQ_DBI_FAIL_RIGHT_EDGE_DEEP_NOISE_INC_DQ_ODELAY);
      write_error_bit_XRAM();
      writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
      return 1;
    }

    // Parallel Bitslice Reset
    bitslice_reset_bcast(0, BS_RESET_WT);

    if(glbl_wr_dbi_en)
    {
        // Remove the Extended Mode
        writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (extend_write_data & 0xFFFFFFF6));  

        // DBI Enable in the CAL RTL
        writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 2);

        // PRECHARGE COMMAND
        lp4_bank_row_access(rank /*u8 rank*/, PRECHARGE /*u8 cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);

        // Mode Register Write For Write DBI Enable/ Read DBI Disable
        lp4_mode_reg_wr(rank, 3, ((glbl_lp4_mr3 | 0x80) & 0xBF));

        // Activate Command
        lp4_bank_row_access(rank /*u8 rank*/, ACTIVATE /*cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);

        writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (extend_write_data | 0x00000009));  

    	//status = get_lp4_dbi_noise_region(rank, dq_coarse, dq_fine, glbl_rl_dly_qtr_max/2, 1 /*inc_dec*/);
    	status = get_lp4_dq_dbi_region(NIBBLES /*u8 data_type*/, DEEP_NOISE /*u8 region_type*/, rank /*u8 rank*/, dq_coarse /*u8 *coarse*/, dq_fine /*u8 *fine*/, 1 /*u8 step_size*/, INC /*u8 inc_dec*/);
    	if(status) {
          write_error_XRAM(ERR_CODE_123_WR_DQ_DBI_FAIL_RIGHT_EDGE_DEEP_NOISE_INC_DBI_ODELAY);
	  writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
          return 1;
        }

    	status = get_lp4_dq_dbi_region(NIBBLES /*u8 data_type*/, PARTIAL /*u8 region_type*/, rank /*u8 rank*/, dq_coarse /*u8 *coarse*/, dq_fine /*u8 *fine*/, 1 /*u8 step_size*/, INC /*u8 inc_dec*/);
    	if(status) {
          write_error_XRAM(ERR_CODE_124_WR_DQ_DBI_FAIL_RIGHT_EDGE_VALID_WINDOW_INC_DBI_ODELAY);
	  writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
          return 1;
        }

    	status = get_lp4_dq_dbi_region(NIBBLES /*u8 data_type*/, LOW_NOISE /*u8 region_type*/, rank /*u8 rank*/, dq_coarse /*u8 *coarse*/, dq_fine /*u8 *fine*/, 1 /*u8 step_size*/, DEC /*u8 inc_dec*/);
        if(status) {
          write_error_XRAM(ERR_CODE_125_WR_DQ_DBI_FAIL_RIGHT_EDGE_LOW_NOISE_DEC_DBI_ODELAY);
	  writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
          return 1;
        }

// XRAM_DBG: wrdqdbi_stg2c_wldlyrnk_crse: per_dqs, stage 2 final value
      for(i=0; i<DQS_BITS; i++) {
        writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG2C_WLDLYRNK_CRSE + (i * 4), dbi_coarse[i]);
      }

      // Parallel Bitslice Reset
      bitslice_reset_bcast(0, BS_RESET_WT);
        writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (extend_write_data & 0xFFFFFFF6));

        // Mode Register Write For Write DBI/READ DBI Disable
        lp4_mode_reg_wr(rank, 3, (glbl_lp4_mr3 & 0x3F));

        // Mode Register Write For Write DM disable
        lp4_mode_reg_wr(rank, 0xd, (glbl_lp4_mr13 | 0x20));

        writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 0);

        writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (extend_write_data | 0x00000009));
    } else {
        status = get_lp4_dq_dbi_region(BITS /*u8 data_type*/, PARTIAL /*u8 region_type*/, rank /*u8 rank*/, dq_coarse /*u8 *coarse*/, dq_fine /*u8 *fine*/, 1 /*u8 step_size*/, INC /*u8 inc_dec*/);
        if(status) {
          write_error_XRAM(ERR_CODE_126_WR_DQ_DBI_FAIL_RIGHT_EDGE_VALID_WINDOW_INC_DQ_ODELAY);
          write_error_bit_XRAM();
	  writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
          return 1;
        }
    }
    status = get_lp4_dq_dbi_region(BITS /*u8 data_type*/, LOW_NOISE /*u8 region_type*/, rank /*u8 rank*/, dq_coarse /*u8 *coarse*/, dq_fine /*u8 *fine*/, 1/*u8 step_size*/, DEC /*u8 inc_dec*/);


// XRAM_DBG: wrdqdbi_wldlyrnk_crse_final
    for(i=0; i<DQS_BITS; i++) {
      phy_nibble = glbl_dqs_bits_riu_map[i][0];
      wl_dly_rnk = read_riu_nibble_reg(phy_nibble, WLDLYRNK0 + rank);
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_WLDLYRNK_CRSE_FINAL + (i * 4), (wl_dly_rnk >> 9) & 0xF);
    }

    if(status) {
      write_error_XRAM(ERR_CODE_127_WR_DQ_DBI_FAIL_RIGHT_EDGE_LOW_NOISE_DEC_DQ_ODELAY);
      write_error_bit_XRAM();
      writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
      return 1;
    }

    // Parallel Bitslice Reset
    bitslice_reset_bcast(0, BS_RESET_WT);


    status = get_lp4_dq_per_bit_deskew(FINE_CRS_STEP_SIZE);

// XRAM_DBG: wrdqdbi_stg4a_deskew_dq_odly_fcrse: valid_detection_per_dq_odly: with coarse steps
    for(bit=0; bit<DQ_BITS; bit++) {
      addr = (glbl_dq_bits_riu_map[bit][0] << RIU_NIB_POS) | (glbl_dq_bits_riu_map[bit][1] + ODLY0);
      odly = readIO_RIU(addr) & 0x1FF;
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG4A_DESKEW_DQ_ODLY_FCRSE + (bit * 4), odly);
    }

    if(status) {
      write_error_XRAM(ERR_CODE_22_WR_DQ_DBI_FAIL_NOISE_DQ_ODELAY_HIGHER_TAPS);
      write_error_bit_XRAM();
      writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
      return 1;
    }

    // Parallel Bitslice Reset
    bitslice_reset_bcast(0, BS_RESET_WT);

    status = get_lp4_dq_per_bit_deskew(1);

// XRAM_DBG: wrdqdbi_stg4b_deskew_dq_odly: valid_detection_per_dq_odly: with fine steps
    for(bit=0; bit<DQ_BITS; bit++) {
      addr = (glbl_dq_bits_riu_map[bit][0] << RIU_NIB_POS) | (glbl_dq_bits_riu_map[bit][1] + ODLY0);
      odly = readIO_RIU(addr) & 0x1FF;
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG4B_DESKEW_DQ_ODLY + (bit * 4), odly);
    }

    if(status) {
      write_error_XRAM(ERR_CODE_23_WR_DQ_DBI_FAIL_NOISE_DQ_ODELAY);
      write_error_bit_XRAM();
      writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
      return 1;
    }

    // Parallel Bitslice Reset
    bitslice_reset_bcast(0, BS_RESET_WT);

    // per Bit Deskew operation for DBI
    if(glbl_wr_dbi_en)
    {
      // Remove the Extended Mode
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (extend_write_data & 0xFFFFFFF6));  

      // DBI Enable in the CAL RTL
      writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 2);

      // PRECHARGE COMMAND
      lp4_bank_row_access(rank /*u8 rank*/, PRECHARGE /*u8 cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);

      // Mode Register Write For Write DBI Enable/ Read DBI Disable
      lp4_mode_reg_wr(rank, 3, ((glbl_lp4_mr3 | 0x80) & 0xBF));

      // Activate Command
      lp4_bank_row_access(rank /*u8 rank*/, ACTIVATE /*cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);

      // Extended Mode Enabled
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (extend_write_data | 0x00000009));  

      status = get_lp4_dbi_bit_deskew(FINE_CRS_STEP_SIZE);

// XRAM_DBG: wrdqdbi_stg5a_valid_detection_per_dbi_odly: with coarse steps
      for(byte=0; byte<DQS_BITS; byte++) {
        addr = (glbl_dbi_bits_riu_map[byte][0] << RIU_NIB_POS) | (glbl_dbi_bits_riu_map[byte][1] + ODLY0);
        odly = readIO_RIU(addr) & 0x1FF;
        writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG5A_DESKEW_DBI_ODLY_FCRSE + (byte * 4), odly);
      }

      if(status) {
        write_error_XRAM(ERR_CODE_24_WR_DQ_DBI_FAIL_VALID_WINDOW_DQS_DBI_ODELAY);
	writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
        return 1;
      }

      // Parallel Bitslice Reset
      bitslice_reset_bcast(0, BS_RESET_WT);

      status = get_lp4_dbi_bit_deskew(1);

// XRAM_DBG: wrdqdbi_stg5b_valid_detection_per_dbi_odly: with fine steps
      for(byte=0; byte<DQS_BITS; byte++) {
        addr = (glbl_dbi_bits_riu_map[byte][0] << RIU_NIB_POS) | (glbl_dbi_bits_riu_map[byte][1] + ODLY0);
        odly = readIO_RIU(addr) & 0x1FF;
        writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG5B_DESKEW_DBI_ODLY + (byte * 4), odly);
      }

      if(status) {
        write_error_XRAM(ERR_CODE_25_WR_DQ_DBI_FAIL_NOISE_DBI_ODELAY_HIGHER_TAPS);
	writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
        return 1;
      }

      // Parallel Bitslice Reset
      bitslice_reset_bcast(0, BS_RESET_WT);
    }

    status = get_noise_right_edge(rank, FINE_CRS_STEP_SIZE, SIMPLE, valid_width);

// XRAM_DBG: wrdqdbi_stg6a_valid_window: per_dqs: local valid_width: with coarse steps
    for(i=0; i<DQS_BITS; i++) {
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG6A_VALID_WINDOW_FCRSE + (i * 4), valid_width[i]);
    }

    if(status) {
      write_error_XRAM(ERR_CODE_33_WR_DQ_DBI_FAIL_LEFT_EDGE_DQ_ODELAY_HIGHER_TAPS);
      writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
      return 1;
    }

    status = get_noise_right_edge (rank, 1, SIMPLE, valid_width);

// XRAM_DBG: wrdqdbi_window_margin: per_dqs: local valid_width: with fine steps
// XRAM_DBG: wrdqdbi_left_margin // Added to ease the debug software
// XRAM_DBG: wrdqdbi_right_margin
    for(i=0; i<DQS_BITS; i++) {
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_WINDOW_MARGIN + (i * 4), valid_width[i]);
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_LEFT_MARGIN + (i * 4), valid_width[i]/2);
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_RIGHT_MARGIN + (i * 4), valid_width[i]/2);
    }

    if(status) {
      write_error_XRAM(ERR_CODE_34_WR_DQ_DBI_FAIL_LEFT_EDGE_DQ_ODELAY);
      writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
      return 1;
    }

    // Parallel Bitslice Reset
    bitslice_reset_bcast(0, BS_RESET_WT);

    status = get_lp4_centering(rank, valid_width);

// XRAM_DBG: wrdqdbi_odly_dqs_final
    for(byte=0; byte<DQS_BITS; byte++) {
      addr = (glbl_dqs_bits_riu_map[byte][0] << RIU_NIB_POS) | (glbl_dqs_bits_riu_map[byte][1] + ODLY0);
      odly = readIO_RIU(addr) & 0x1FF;
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_ODLY_DQS_FINAL + (byte * 4), odly);
    }
// XRAM_DBG: wrdqdbi_odly_dq_final
    for(bit=0; bit<DQ_BITS; bit++) {
      addr = (glbl_dq_bits_riu_map[bit][0] << RIU_NIB_POS) | (glbl_dq_bits_riu_map[bit][1] + ODLY0);
      odly = readIO_RIU(addr) & 0x1FF;
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_ODLY_DQ_FINAL + (bit * 4), odly);
    }
// XRAM_DBG: wrdqdbi_odly_dbi_final
    if(glbl_wr_dbi_en) {
      for(byte=0; byte<DQS_BITS; byte++) {
        addr = (glbl_dbi_bits_riu_map[byte][0] << RIU_NIB_POS) | (glbl_dbi_bits_riu_map[byte][1] + ODLY0);
        odly = readIO_RIU(addr) & 0x1FF;
        writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_ODLY_DBI_FINAL + (byte * 4), odly);
      }
    }

    // Sanity Check Stage
    status = get_wr_center_sanity(rank, 0); 

    if(status) {
      write_error_XRAM(ERR_CODE_29_WR_DQ_DBI_FAIL_POST_SANITY_CHECK);
      writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
      return 1;
    }

    // Extended Mode Disabled
    writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (extend_write_data & 0xFFFFFFF6));  

    // PRECHARGE COMMAND
    lp4_bank_row_access(rank /*u8 rank*/, PRECHARGE /*u8 cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);

    } // TCK_TH_WR_CAL: SKIP write centering stage

    // Disable The Write DBI if DM is enabled 
    if((glbl_lp4_mr13 & 0x20) == 0) 
    {
      // Mode Register Write For Write DBI Disable
      lp4_mode_reg_wr(rank, 3, (glbl_lp4_mr3 & 0x3F));

      // Mode Register Write For Write DM disable
      lp4_mode_reg_wr(rank, 0xd, (glbl_lp4_mr13 | 0x20));

      // Write DM Enable in the CAL RTL
      writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
    }
  } // RANKS
  #endif  

  if(glbl_phy_rank_switch_en == 1) {
    DEBUG_PRINTF("INFO: ddr_write_dqs_to_dq(): Enabling dynamic mode for TX\n");
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
    riu_reg_bcast(DIS_DYN_MODE, 0, 0x0);

    DEBUG_PRINTF("INFO: ddr_write_dqs_to_dq(): Dummy write for setting correct odelays\n");
    dummy_write_multi_rank_switch(0);
  }

  return 0;
}
