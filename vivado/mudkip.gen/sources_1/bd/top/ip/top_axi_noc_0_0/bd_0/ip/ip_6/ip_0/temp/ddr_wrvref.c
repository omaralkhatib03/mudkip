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
//       Revision:       $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/ddr_wrvref.c#1 $
//                       $Author: peerm $
//                       $DateTime: 2019/01/07 22:27:27 $
//                       $Change: 20180851 $
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
#include "cal_reg_define.h"
#include "riu_reg_define.h"
#include "xsdb_reg_define.h"
#include "internal_reg_define.h"
#include "function_dec.c"
#include "global_variable_define.h"

u8 ddr_write_vref()
{
  u8 i,j,k;
  u8 status;
  u8 xpi_write_dm_dbi;
  u32 initial_flag = 0;
  u32 dq_odelay_per_byte_flag, vref_per_byte_flag;
  u32 nibble_mask; 
  u8 vref_value_cur;
  u8 vref_value[100];
  u8 default_vref_value = glbl_wrvref_default_value;
  u16 valid_width[100][DQS_BITS];
  static u16 vref_left_width[100][DQS_BITS];
  static u16 vref_right_width[100][DQS_BITS];
  u16 valid_width_cur;
  u16 valid_width_perc;
  u16 left_width[DQS_BITS];
  u16 right_width[DQS_BITS];
  u16 valid_width_threshold[DQS_BITS];
  u16 max_valid_width = 0;
  u16 max_valid_width_perc = 0;
  u8 lower_vref_value, upper_vref_value, final_vref_value[DQS_BITS];
  u8 found_lower, found_upper;

  if(glbl_tck >= TCK_TH_WR_VREF) {
    DEBUG_PRINTF("INFO: ddr_write_vref(): SKIPPING write vref stage for TCK = %d\n", glbl_tck);
    return 0;
  }

  if(glbl_phy_rank_switch_en == 1) {
    DEBUG_PRINTF("INFO: ddr_write_vref(): Dummy write for setting correct odelays\n");
    dummy_write_multi_rank_switch(0);

    DEBUG_PRINTF("INFO: ddr_write_vref(): Disabling dynamic mode for TX\n");
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
    riu_reg_bcast(DIS_DYN_MODE, 0, 0x1);
  }

    if(default_vref_value < VREF_LOWER_LIMIT) {
      DEBUG_PRINTF("WARNING: ddr_write_vref(): default_vref = %x is much lower value\n", default_vref_value);
      default_vref_value = VREF_LOWER_LIMIT;
      DEBUG_PRINTF("WARNING: ddr_write_vref(): Corrected default_vref = %x\n", default_vref_value);
    } else if(default_vref_value > (VREF_UPPER_LIMIT - VREF_CRSE_JMP)) {
      DEBUG_PRINTF("WARNING: ddr_write_vref(): default_vref = %x is much higher value\n", default_vref_value);
      default_vref_value = (VREF_UPPER_LIMIT - VREF_CRSE_JMP);
      DEBUG_PRINTF("WARNING: ddr_write_vref(): Corrected default_vref = %x\n", default_vref_value);
    } else {
      DEBUG_PRINTF("INFO: ddr_write_vref(): default_vref = %x \n", default_vref_value);
    }

    for(i=0; i<DQS_BITS; i++)
      initial_flag |= (1<<i);

    initial_flag = ~initial_flag;

    for(i=0; i<100; i++) {
      vref_value[i] = default_vref_value;
      for(j=0; j<DQS_BITS; j++) {
        valid_width[i][j] = 0;
        vref_left_width[i][j] = 0;
        vref_right_width[i][j] = 0;
      }
    }

    for(k=0; k < 2; k++) // This Loop is for Lower and upper VREF Values (51-99 upper vref, 0-50 lower vref)
    {
      vref_per_byte_flag         = initial_flag;
      dq_odelay_per_byte_flag    = initial_flag; 
      i = (k==0) ? 0 : 1; // i starts from 0 for lower VREF values and starts from 1 for upper VREF values
      vref_value_cur = (k==0) ? default_vref_value : (default_vref_value + VREF_CRSE_JMP);

      while(vref_per_byte_flag != 0xFFFFFFFF)
      {
        if(k == 0) { // Lower direction
          DEBUG_PRINTF("INFO: ddr_write_vref(): Lower direction: iteration = %d, vref_value = %x\n", i, vref_value_cur);
        } else {
          DEBUG_PRINTF("INFO: ddr_write_vref(): Upper direction: iteration = %d, vref_value = %x\n", i, vref_value_cur);
        }

        // Writing the Vref Value
        set_write_vref(vref_value_cur, 0);
       
      #ifdef DDR4
        // Activate The Bank
        set_1_seq_command_at_mem(0 /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                                 MEM_ROW_ACTIVE /*u32 seq_a_cmd*/, 0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/,
                                 0 /*u32 seq_a2a_dly*/);
      #else
        // Activate the Bank
        lp4_bank_row_access(0 /*u8 rank*/, ACTIVATE /*cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);
      #endif

        for(j=0; j<DQS_BITS; j++) {
          left_width[j] = 0;
          right_width[j] = 0;
        }

        // Find out left and right window widths
        status = get_write_window (0, SIMPLE, left_width, right_width);

        if(status)
          return 1;

        for(j=0; j<DQS_BITS; j++)
        {
          if(((vref_per_byte_flag >> j) & 0x1) == 0)
          {
            valid_width_cur  = left_width[j] + right_width[j];
            valid_width_perc = (valid_width_cur * VREF_PERC_LIMIT) / 100;
            DEBUG_PRINTF("INFO: ddr_write_vref(): Nibble = %d, vref_value = %x, valid_width = %d, vref_left_width = %d, vref_right_width = %d, valid_window threshold = %d\n", j, vref_value_cur, valid_width_cur, left_width[j], right_width[j], valid_width_perc);

            if(i == 0) // First sample
              valid_width_threshold[j] = valid_width_perc;
            else if(valid_width_threshold[j] < valid_width_perc) // Everytime there is new higher threshold
              valid_width_threshold[j] = valid_width_perc;
            else if(valid_width_threshold[j] > valid_width_cur) {
              vref_per_byte_flag |= (1<<j);
              DEBUG_PRINTF("INFO: ddr_write_vref(): Found boundary for Nibble = %d, valid_window threshold = %d, current valid window = %d\n", j, valid_width_perc, valid_width_cur);
            }

            if(k==0) {
               vref_left_width[50 - i][j]  = left_width[j];
               vref_right_width[50 - i][j] = right_width[j];
               valid_width[50 - i][j]      = left_width[j] + right_width[j];
	       vref_value[50 - i]          = vref_value_cur;
            } else {
               vref_left_width[50 + i][j]  = left_width[j];
               vref_right_width[50 + i][j] = right_width[j];
               valid_width[50 + i][j]      = left_width[j] + right_width[j];
	       vref_value[50 + i]          = vref_value_cur;
            }
          }
        }

        if(k == 0) { // Lower direction
	  if(((vref_value_cur - VREF_CRSE_JMP) < VREF_LOWER_LIMIT) && (vref_value_cur < VREF_CRSE_JMP)) {
            vref_per_byte_flag = 0xFFFFFFFF;
            DEBUG_PRINTF("WARNING: ddr_write_vref(): Reached lower limit with out finding the VREF valid window percentage boundary\n");
          } else {
            vref_value_cur -= VREF_CRSE_JMP;
          }
        } else { // Upper direction
          if((vref_value_cur + VREF_CRSE_JMP) > VREF_UPPER_LIMIT) {
            vref_per_byte_flag = 0xFFFFFFFF;
            DEBUG_PRINTF("WARNING: ddr_write_vref(): Reached upper limit with out finding the VREF valid window percentage boundary\n");
          } else { 
            vref_value_cur += VREF_CRSE_JMP;
          }
        }

        i++;

      #ifdef DDR4
        // PRECHARGE COMMAND
        set_1_seq_command_at_mem(0 /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                                 MEM_ROW_PREACHARGE /*u32 seq_a_cmd*/, 0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/,
                                 0/*u32 seq_a2a_dly*/);
      #else
        // Precharge Bank
        lp4_bank_row_access(0 /*u8 rank*/, PRECHARGE /*u8 cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);
      #endif
      }
    }

    // Calculating the Final Vref and left right Value
    for(j=0; j<DQS_BITS; j++)
    {
      // Find Out the Maximum Valid Width
      found_lower = 0;
      found_upper = 0;
      for(i=0; i<100; i++)
      {
        if(i == 0)
          max_valid_width = valid_width[i][j];
        else if(valid_width[i][j] > max_valid_width)
          max_valid_width = valid_width[i][j];
      }  

      max_valid_width_perc = (max_valid_width * VREF_PERC_LIMIT) / 100;

      // Finding the lower most Vref value that is more than threshold
      for(i=0; i<100; i++)
      {
        if((found_lower == 0) && (valid_width[i][j] >= max_valid_width_perc))
        {
          found_lower = 1;
          lower_vref_value = vref_value[i];
          break;
        }
      }

      // Finding the upper most Vref value that is more than threshold
      for(i=0; i<100; i++)
      {
        if((found_upper == 0) && (valid_width[99-i][j] >= max_valid_width_perc))
        {
          found_upper = 1;
          upper_vref_value = vref_value[99-i];
          break;
        }
      }

      final_vref_value[j] = (lower_vref_value + upper_vref_value)/2;
      glbl_wrvref_value[j] = final_vref_value[j];
      DEBUG_PRINTF("INFO: ddr_write_vref(): Found final vref value: Nibble = %d, Default vref = %d, Final vref = %d, Lower vref = %d, Upper vref = %d\n", j, default_vref_value, final_vref_value[j], lower_vref_value, upper_vref_value);
    }
    glbl_wrvref_done = 1;

  // Writing the Final Value corresponding to all the Component
  #ifdef DDR4
    // Disable DM/DBI while enabling PDA mode
    if(glbl_wr_dbi_en) {
      xpi_write_dm_dbi = readIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK));
      writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
    }

    glbl_wrlvl = 1;

    // PDA/PBA mode enable
    DEBUG_PRINTF("INFO: ddr_write_vref(): Enabling the PDA mode \n");
    if(glbl_lrdimm_en) {
      ddr_mrs7_write (0, 0x1100 | DB_F0BC1X_PACKAGE_RANK_TIMING_ALIGNMENT_ENABLE | 0x1); // F0BC1X Register: Enter PBA Mode
    } else {
      set_1_seq_command_at_mem(0 /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                               MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, (glbl_mr3 | 0x00000010) /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 3 /*u16 seq_a_ba*/,
                               0/*u32 seq_a2a_dly*/);
    }

    DEBUG_PRINTF("INFO: ddr_write_vref(): Enabling the Tristate \n");

    // Enable the Tristate
    writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 1);                  

    // NIBBLE BASED Pattern Transfer
    writeIO_DMC(ral_addr_of_seq_dq_nibble_sel_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

    // Accessing all the bytes sequentially
    for(j=0; j<DQS_BITS*NIBBLE_PER_DQS; j++)
      writeIO_DMC(ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (j * 4), 0xFFFFFFFF);    

    for(j=0; j<DQS_BITS; j++)
    {
      writeIO_DMC(ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + ((j * NIBBLE_PER_DQS) * 4), 0x00000000);    
      if(NIBBLE_PER_DQS == 2) {
        writeIO_DMC(ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + ((j * NIBBLE_PER_DQS) * 4) + 4, 0x00000000);    
      }

      DEBUG_PRINTF("INFO: ddr_write_vref(): Writing the Vref Value \n");

      set_write_vref(final_vref_value[j], 0);

      writeIO_DMC(ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + ((j * NIBBLE_PER_DQS) * 4), 0xFFFFFFFF);    
      if(NIBBLE_PER_DQS == 2) {
        writeIO_DMC(ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + ((j * NIBBLE_PER_DQS) * 4) + 4, 0xFFFFFFFF);    
      }
    }

    for(j=0; j<DQS_BITS*NIBBLE_PER_DQS; j++)
      writeIO_DMC(ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (j * 4), 0x00000000);    

    // PDA/PBA mode disabled
    if(glbl_lrdimm_en) {
      ddr_mrs7_write (0, 0x1100 | DB_F0BC1X_PACKAGE_RANK_TIMING_ALIGNMENT_ENABLE | 0x0); // F0BC1X Register: Exit PBA Mode
    } else {
      set_1_seq_command_at_mem(0 /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                               MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, (glbl_mr3 & 0xFFFFFFEF) /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 3 /*u16 seq_a_ba*/,
                               0/*u32 seq_a2a_dly*/);
    }

    glbl_wrlvl = 0;

    writeIO_DMC(ral_addr_of_seq_dq_nibble_sel_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
    writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0);

    // Keep the DM/DBI back
    if(glbl_wr_dbi_en) {
      writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), xpi_write_dm_dbi);
    }

  #else
    // Writing Vref on Channel 0
    writeIO_DMC(ral_addr_of_cal_cs_ch_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);
    lp4_mode_reg_wr(0, 14, final_vref_value[0] | (glbl_lp4_mr14&0x40));

    // Writing Vref on Channel 1
    writeIO_DMC(ral_addr_of_cal_cs_ch_in_ddrmc_main_ddrmc_main_bank(BLK), 0x3);
    lp4_mode_reg_wr(0, 14, final_vref_value[DQS_BITS-1] | (glbl_lp4_mr14&0x40));

    writeIO_DMC(ral_addr_of_cal_cs_ch_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
  #endif

  if(glbl_phy_rank_switch_en == 1) {
    DEBUG_PRINTF("INFO: ddr_write_vref(): Enabling dynamic mode for TX\n");
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
    riu_reg_bcast(DIS_DYN_MODE, 0, 0x0);

    DEBUG_PRINTF("INFO: ddr_write_vref(): Dummy write for setting correct odelays\n");
    dummy_write_multi_rank_switch(0);
  }

  DEBUG_PRINTF("INFO: ddr_write_vref(): Do the sanity check 1\n");
  status = sanity_check_1();
  if(status != 0) {
    DEBUG_PRINTF("ERROR: ddr_write_vref(): Sanity check 1 Failed\n");
    return 1;
  } else {
    DEBUG_PRINTF("INFO: ddr_write_vref(): Sanity check 1 Passed\n");
  }

  return 0;
}
