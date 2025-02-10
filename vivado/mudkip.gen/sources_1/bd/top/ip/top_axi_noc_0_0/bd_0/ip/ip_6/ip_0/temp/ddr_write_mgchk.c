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
//       Revision:       $Id: $
//                       $Author: $
//                       $DateTime: $
//                       $Change: $
//       Description:
//           DDRMC: Memory controller calibration code.
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

u8 ddr_write_mgchk(u8 pattern_type)
{
  u8 j;
  u16 left_width[DQS_BITS];
  u16 right_width[DQS_BITS];
  u8 status;
    writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 0xC);
    writeIO_DMC(ral_addr_of_seq_a_dq_1_in_ddrmc_main_ddrmc_main_bank(BLK), 0xD);
    writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), ((0xD << 5) | 0xC));

  DEBUG_PRINTF("INFO: ddr_write_prbs(): Do the sanity check 1\n");
  status = sanity_check_1();
  if(status != 0) {
    DEBUG_PRINTF("ERROR: ddr_write_prbs(): Sanity check 1 Failed\n");
    write_error_XRAM(ERR_CODE_75_WR_PRBS_FAIL_POST_SANITY_CHECK_1);
    return 1;
  } else {
    DEBUG_PRINTF("INFO: ddr_write_prbs(): Sanity check 1 Passed\n");
  }
  if(glbl_phy_rank_switch_en == 1) {
    DEBUG_PRINTF("INFO: ddr_write_prbs(): Dummy write for setting correct odelays\n");
    dummy_write_multi_rank_switch(glbl_margin_rank);

    DEBUG_PRINTF("INFO: ddr_write_prbs(): Disabling dynamic mode for TX\n");
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
    riu_reg_bcast(DIS_DYN_MODE, 0, 0x1);
  }

  for(j=0; j<DQS_BITS; j++) {
    left_width[j] = 0;
    right_width[j] = 0;
  }

  // Find out left and right window widths
  status = get_write_window_margin (glbl_margin_rank, pattern_type, left_width, right_width);

  for(j=0; j<DQS_BITS; j++) {
      // XRAM_DBG: wrmargin_left
      // XRAM_DBG: wrmargin_right
      writeIO_XRAM(XRAM_WRMARGIN_LEFT + j*4, left_width[j]);
      writeIO_XRAM(XRAM_WRMARGIN_RIGHT + j*4, right_width[j]);
  }

  if(status) {
    bitslice_reset_bcast(1, BS_RESET_WT);
    return 1;
    }

  if(glbl_phy_rank_switch_en == 1) {
    DEBUG_PRINTF("INFO: ddr_write_prbs(): Enabling dynamic mode for TX\n");
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
    riu_reg_bcast(DIS_DYN_MODE, 0, 0x0);

    DEBUG_PRINTF("INFO: ddr_write_prbs(): Dummy write for setting correct odelays\n");
    dummy_write_multi_rank_switch(glbl_margin_rank);
  }

  DEBUG_PRINTF("INFO: ddr_write_prbs(): Do the sanity check 1\n");
  status = sanity_check_1();
  if(status != 0) {
    DEBUG_PRINTF("ERROR: ddr_write_prbs(): Sanity check 1 Failed\n");
    write_error_XRAM(ERR_CODE_75_WR_PRBS_FAIL_POST_SANITY_CHECK_1);
    return 1;
  } else {
    DEBUG_PRINTF("INFO: ddr_write_prbs(): Sanity check 1 Passed\n");
  }

  return 0;
}

u8 get_write_window_margin (u8 rank, u8 pattern_type, u16 *left_width, u16 *right_width)
{
  u8 i, j;
  u8 status;

  if(pattern_type == SIMPLE) {
    writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 0xC);
    writeIO_DMC(ral_addr_of_seq_a_dq_1_in_ddrmc_main_ddrmc_main_bank(BLK), 0xD);
    writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), ((0xD << 5) | 0xC));
  }

  // Finding Left Margin
  DEBUG_PRINTF("INFO: get_write_window(): Noise Right Edge With Large Step \n"); 
  status = get_noise_right_edge (rank, FINE_CRS_STEP_SIZE, pattern_type, left_width);

  if(status) {
      write_error_XRAM(ERR_CODE_69_WR_PRBS_FAIL_LEFT_MARGIN_DQ_ODELAY_HIGHER_TAPS);
    return 1;
  }

  DEBUG_PRINTF("INFO: get_write_window(): Noise Right Edge With Small Step \n"); 
  status = get_noise_right_edge (rank, 1, pattern_type, left_width);

  if(status) {
      write_error_XRAM(ERR_CODE_70_WR_PRBS_FAIL_LEFT_MARGIN_DQ_ODELAY);
    return 1;
  }

  DEBUG_PRINTF("INFO: get_write_window(): Left window is computed\n");
  DEBUG_PRINTF("INFO: get_write_window(): Moving the DQ/DBI to the initial position\n");

  // Setting the INC DEC Register With left_width Value
  for(j=0; j<DQS_BITS; j++)
  {
    write_riu_nibble_reg(glbl_map_riu[(j*NIBBLE_PER_DQS)], INCDEC_CRSE, left_width[j]);

    if(NIBBLE_PER_DQS == 2)
      write_riu_nibble_reg(glbl_map_riu[(j*NIBBLE_PER_DQS) + 1], INCDEC_CRSE, left_width[j]);
  }

  // Delaying the DQ to the initial position
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(IODLY_BCAST_MASK, 1, 0x000000FF); // 0000_1111_1111 
  riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000028); // 0000_0010_1000 // Decrement By coarse

  if(glbl_wr_dbi_en)
  {
    // Broadcast Increment the DBI
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dbi_nibble_mask_initial);
    riu_reg_bcast(ODLY0, 1, 0x6000); // Step Wise Decrement of ODLY
  }
  bitslice_reset_bcast(1, BS_RESET_WT);

  // Finding Right Margin
  DEBUG_PRINTF("INFO: get_write_window(): Noise Left Edge With Large Step \n"); 
#ifdef DDR4
  status = get_right_edge(right_width, FINE_CRS_STEP_SIZE, pattern_type);
#else
  status = get_lp4_noise_left_edge(rank, FINE_CRS_STEP_SIZE, pattern_type, right_width);
#endif

  if(status) {
    if(pattern_type == PRBS) {
      #ifdef DDR4
         write_error_XRAM(ERR_CODE_71_WR_PRBS_FAIL_RIGHT_MARGIN_DQS_ODELAY_HIGHER_TAPS);
      #else
         write_error_XRAM(ERR_CODE_73_WR_PRBS_FAIL_RIGHT_MARGIN_DQ_ODELAY_HIGHER_TAPS);
      #endif
    }
    return 1; 
  }

  DEBUG_PRINTF("INFO: get_write_window(): Noise Left Edge With Small Step \n"); 
#ifdef DDR4
  status = get_right_edge(right_width, 1, pattern_type);
#else
  status = get_lp4_noise_left_edge(rank, 1, pattern_type, right_width);
#endif

  if(status) {
    if(pattern_type == PRBS) {
      #ifdef DDR4
         write_error_XRAM(ERR_CODE_72_WR_PRBS_FAIL_RIGHT_MARGIN_DQS_ODELAY);
      #else
         write_error_XRAM(ERR_CODE_74_WR_PRBS_FAIL_RIGHT_MARGIN_DQ_ODELAY);
      #endif
    }
    return 1; 
  }

  DEBUG_PRINTF("INFO: get_write_window(): Right window is computed\n");
  DEBUG_PRINTF("INFO: get_write_window(): Moving the DQS to the initial position\n");

  // Setting the INC DEC Register With right_width Value
  for(j=0; j<DQS_BITS; j++)
  {
    write_riu_nibble_reg(glbl_map_riu[(j*NIBBLE_PER_DQS)], INCDEC_CRSE, right_width[j]);

    if(NIBBLE_PER_DQS == 2)
      write_riu_nibble_reg(glbl_map_riu[(j*NIBBLE_PER_DQS) + 1], INCDEC_CRSE, right_width[j]);
  }

#ifdef DDR4
  // Delaying the DQS & Tristate to the initial position
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dqs_nibble_mask_initial);
  riu_reg_bcast(ODLY0, 1, 0x6000); // In this DQS ODLY Coarse Decrement

  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(TRISTATE_ODLY, 1, 0x6000); //Tristate Inc Dec Crse ... 15 14 13

#else
  // Delaying the DQ and DBI to the initial position
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(IODLY_BCAST_MASK, 1, 0x000000FF); // 0000_1111_1111 
  riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000038); // 0000_0010_1000 // Increment By coarse

  if(glbl_wr_dbi_en)
  {
    // Broadcast Increment the DBI
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dbi_nibble_mask_initial);
    riu_reg_bcast(ODLY0, 1, 0xA000); // Crse Wise Increment of ODLY
  }
#endif

  // Apply bit-slice reset to all the DQ nibbles
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
  bitslice_reset_bcast(0, BS_RESET_WT);
 
  return 0;
}


