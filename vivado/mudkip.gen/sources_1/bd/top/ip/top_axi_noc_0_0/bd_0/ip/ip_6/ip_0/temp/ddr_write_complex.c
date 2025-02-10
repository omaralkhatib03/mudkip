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

u8 ddr_write_complex()
{
  u16 left_margin[DQS_BITS];
  u16 right_margin[DQS_BITS];
  u8 status;
  u8 i;
  u8 rank=0;
  u16 odly;

  if(glbl_tck >= TCK_TH_WR_CMPLX) {
    DEBUG_PRINTF("INFO: ddr_write_complex(): SKIPPING write centering stage for TCK = %d\n", glbl_tck);
    return 0;
  }

  // Nibble Wise Reset Mask Setting For DQS
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK0, glbl_nibble_reset_mask[0]);
  riu_reg_bcast(CFG_BS_RST_MASK, 0, ~(0x7F));

  // Nibble Wise Reset Mask Setting For DM
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK0, glbl_nibble_reset_mask[1]);
  riu_reg_bcast(CFG_BS_RST_MASK, 0, ~(0x7D));

  writeIO_RIU( RPI_BCAST_NIBBLE_MASK0, (glbl_nibble_reset_mask[0] | glbl_nibble_reset_mask[1]));

  #ifdef DDR4
    for(i=0; i<DQS_BITS; i++) {
      left_margin[i]  = 0;
      right_margin[i] = 0;
    }

    if(glbl_phy_rank_switch_en == 1) {
      DEBUG_PRINTF("INFO: ddr_write_complex(): Dummy write for setting correct odelays\n");
      dummy_write_multi_rank_switch(rank);

      DEBUG_PRINTF("INFO: ddr_write_complex(): Disabling dynamic mode for TX\n");
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
      riu_reg_bcast(DIS_DYN_MODE, 0, 0x1);
    }

    // Parallel Bitslice Reset
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
    bitslice_reset_bcast(0, BS_RESET_WT);

    // Precharge Command
    set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                            MEM_ROW_PREACHARGE /*u32 seq_a_cmd*/, 0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/,
                            0x0 /*u32 seq_a2a_dly*/);
    if(glbl_wr_dbi_en)
    {
      // DBI Enable in the CAL RTL
      writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 2);

      // Enabling the write DBI into the Memory
      set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
            		       MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, ((glbl_mr5[0] | 0x800) & 0xFFFFFBFF) /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x1 /*u16 seq_a_ba*/,
          		       0x0 /*u32 seq_a2a_dly*/);
    }

    status = get_noise_right_edge(rank, FINE_CRS_STEP_SIZE, COMPLEX, left_margin); 

// XRAM_DBG: wrcmplx_left_margin_fcrse
    for(i=0; i<DQS_BITS; i++) {
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRCMPLX_LEFT_MARGIN_FCRSE + i*4, left_margin[i]);
    }

    if(status != 0) {
      DEBUG_PRINTF("ERROR: ddr_write_complex(): while finding right edge of the noise with coarse steps\n");
      write_error_XRAM(ERR_CODE_55_WR_COMPLEX_FAIL_LEFT_EDGE_DQ_ODELAY_HIGHER_TAPS);
      writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
      return 1;
    }

    status = get_noise_right_edge(rank, 1, COMPLEX, left_margin);

// XRAM_DBG: wrcmplx_left_margin
    for(i=0; i<DQS_BITS; i++) {
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRCMPLX_LEFT_MARGIN + i*4, left_margin[i]);
    }

    if(status != 0) {
      DEBUG_PRINTF("ERROR: ddr_write_complex(): while finding right edge of the noise with single step\n");
      write_error_XRAM(ERR_CODE_56_WR_COMPLEX_FAIL_LEFT_EDGE_DQ_ODELAY);
      writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
      return 1; 
    }

    status = get_dq_left_shift(left_margin); 

    if(status != 0) {
      DEBUG_PRINTF("ERROR: ddr_write_complex(): while finding left margin\n");
      return 1;
    }

    // Parallel Bitslice Reset
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
    bitslice_reset_bcast(0, BS_RESET_WT);

    status = get_noise_left_edge(rank, FINE_CRS_STEP_SIZE, right_margin);
  
// XRAM_DBG: wrcmplx_right_margin_fcrse
    for(i=0; i<DQS_BITS; i++) {
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRCMPLX_RIGHT_MARGIN_FCRSE + i*4, right_margin[i]);
    }

    if(status != 0) {
      DEBUG_PRINTF("ERROR: ddr_write_complex(): while finding left edge of the noise with coarse steps\n");
      write_error_XRAM(ERR_CODE_57_WR_COMPLEX_FAIL_RIGHT_EDGE_DQS_ODELAY_HIGHER_TAPS);
      writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
      return 1;
    }

    status = get_noise_left_edge(rank, 1, right_margin);

// XRAM_DBG: wrcmplx_right_margin
    for(i=0; i<DQS_BITS; i++) {
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRCMPLX_RIGHT_MARGIN + i*4, right_margin[i]);
    }
  
    if(status != 0) {
      DEBUG_PRINTF("ERROR: ddr_write_complex(): while finding left edge of the noise with single step\n");
      write_error_XRAM(ERR_CODE_58_WR_COMPLEX_FAIL_RIGHT_EDGE_DQS_ODELAY);
      writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
      return 1;
    }

    // Parallel Bitslice Reset
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
    bitslice_reset_bcast(0, BS_RESET_WT);

    status = get_cplx_centre(left_margin, right_margin);

// XRAM_DBG: wrcmplx_odly_dqs_final
    for(i=0; i<DQS_BITS; i++) {
      odly = read_riu_nibble_reg(glbl_dqs_bits_riu_map[i][0], ODLY0);
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRCMPLX_ODLY_DQS_FINAL + i*4, odly);
    }

// XRAM_DBG: wrcmplx_odly_dq_final
    for(i=0; i<DQ_BITS; i++) {
      odly = read_riu_nibble_reg(glbl_dq_bits_riu_map[i][0], glbl_dq_bits_riu_map[i][1] + ODLY0);
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRCMPLX_ODLY_DQ_FINAL + i*4, odly);
    }

// XRAM_DBG: wrcmplx_odly_dbi_final
    if(glbl_wr_dbi_en) {
      for(i=0; i<DQS_BITS; i++) {
        odly = read_riu_nibble_reg(glbl_dbi_bits_riu_map[i][0], ODLY0);
        writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRCMPLX_ODLY_DBI_FINAL + i*4, odly);
      }
    }

    if(status != 0) {
      DEBUG_PRINTF("ERROR: ddr_write_complex(): while finding the center of the window\n");
      return 1;
    }

    // Parallel Bitslice Reset
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
    bitslice_reset_bcast(0, BS_RESET_WT);

    status = get_wr_center_sanity(rank, 1);
  
    if(status != 0) {
      DEBUG_PRINTF("ERROR: ddr_write_complex(): sanity check failed\n");
      write_error_XRAM(ERR_CODE_59_WR_COMPLEX_FAIL_POST_SANITY_CHECK);
      writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
      return 1;
    }

    // Precharge Command
    set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                             MEM_ROW_PREACHARGE /*u32 seq_a_cmd*/, 0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/,
                             0/*u32 seq_a2a_dly*/);
   
    // Disable The Write DBI if DM is enabled
    if((glbl_mr5[0] & 0x400) != 0)
    {
      set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                               MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr5[0] /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x1 /*u16 seq_a_ba*/,
                               0 /*u32 seq_a2a_dly*/);

      // Write DM Enable in the CAL RTL
      writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
    }

  #else
    for(rank=0; rank < RANKS; rank++) {
      DEBUG_PRINTF("INFO: ddr_write_complex(): Starting write complex calibration for Rank = %d\n", rank);

      for(i=0; i<DQS_BITS; i++) {
        left_margin[i]  = 0;
        right_margin[i] = 0;
      }

      if(glbl_phy_rank_switch_en == 1) {
        DEBUG_PRINTF("INFO: ddr_write_complex(): Enabling dynamic mode for TX\n");
        writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
        riu_reg_bcast(DIS_DYN_MODE, 0, 0x0);

        DEBUG_PRINTF("INFO: ddr_write_complex(): Dummy write for setting correct odelays for Rank = %d\n", rank);
        dummy_write_multi_rank_switch(rank);

        DEBUG_PRINTF("INFO: ddr_write_complex(): Disabling dynamic mode for TX\n");
        writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
        riu_reg_bcast(DIS_DYN_MODE, 0, 0x1);
      }

      // Parallel Bitslice Reset
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
      bitslice_reset_bcast(0, BS_RESET_WT);

      // PRECHARGE COMMAND
      lp4_bank_row_access(rank /*u8 rank*/, PRECHARGE /*u8 cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);

      // Mode Register Write For Write DBI Enable
      if(glbl_wr_dbi_en) {
        lp4_mode_reg_wr(rank, 3, (glbl_lp4_mr3 | 0x80));
      }

      // Finding the Left Margin
      status = get_noise_right_edge(rank, FINE_CRS_STEP_SIZE, COMPLEX, left_margin);
     
// XRAM_DBG: wrcmplx_left_margin_fcrse // FIXME, Need to add rank wise
      for(i=0; i<DQS_BITS; i++) {
        writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRCMPLX_LEFT_MARGIN_FCRSE + i*4, left_margin[i]);
      }

      if(status != 0) {
        DEBUG_PRINTF("ERROR: ddr_write_complex(): while finding right edge of the noise with coarse steps\n");
        write_error_XRAM(ERR_CODE_60_WR_COMPLEX_FAIL_RIGHT_EDGE_DQ_ODELAY_HIGHER_TAPS);
      writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
        return 1;
      }

      status = get_noise_right_edge(rank, 1, COMPLEX, left_margin);

// XRAM_DBG: wrcmplx_left_margin
      for(i=0; i<DQS_BITS; i++) {
        writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRCMPLX_LEFT_MARGIN + i*4, left_margin[i]);
      }

      if(status != 0) {
        DEBUG_PRINTF("ERROR: ddr_write_complex(): while finding right edge of the noise with single step\n");
        write_error_XRAM(ERR_CODE_61_WR_COMPLEX_FAIL_RIGHT_EDGE_DQ_ODELAY);
      writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
        return 1; 
      }

      status = get_dq_left_shift(left_margin); 

      if(status != 0) {
        DEBUG_PRINTF("ERROR: ddr_write_complex(): while finding left margin\n");
        return 1;
      }
 
      // Finding the Right Margin
      status = get_lp4_noise_left_edge(rank, FINE_CRS_STEP_SIZE, COMPLEX, right_margin);

// XRAM_DBG: wrcmplx_right_margin_fcrse
      for(i=0; i<DQS_BITS; i++) {
        writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRCMPLX_RIGHT_MARGIN_FCRSE + i*4, right_margin[i]);
      }

      if(status != 0) {
        DEBUG_PRINTF("ERROR: ddr_write_complex(): while finding left edge of the noise with coarse steps\n");
        write_error_XRAM(ERR_CODE_55_WR_COMPLEX_FAIL_LEFT_EDGE_DQ_ODELAY_HIGHER_TAPS);
      writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
        return 1; 
      }

      status = get_lp4_noise_left_edge(rank, 1, COMPLEX, right_margin);

// XRAM_DBG: wrcmplx_right_margin
      for(i=0; i<DQS_BITS; i++) {
        writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRCMPLX_RIGHT_MARGIN + i*4, right_margin[i]);
      }
  
      if(status != 0) {
        DEBUG_PRINTF("ERROR: ddr_write_complex(): while finding left edge of the noise with single step\n");
        write_error_XRAM(ERR_CODE_56_WR_COMPLEX_FAIL_LEFT_EDGE_DQ_ODELAY);
      writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
        return 1; 
      }

      // Parallel Bitslice Reset
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
      bitslice_reset_bcast(0, BS_RESET_WT);

      status = get_lp4_cplx_centre(rank, left_margin, right_margin);

// XRAM_DBG: wrcmplx_odly_dqs_final
      for(i=0; i<DQS_BITS; i++) {
        odly = read_riu_nibble_reg(glbl_dqs_bits_riu_map[i][0], ODLY1); // LP4 DQS is on Bit-slice-1
        writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRCMPLX_ODLY_DQS_FINAL + i*4, odly);
      }

// XRAM_DBG: wrcmplx_odly_dq_final
      for(i=0; i<DQ_BITS; i++) {
        odly = read_riu_nibble_reg(glbl_dq_bits_riu_map[i][0], glbl_dq_bits_riu_map[i][1] + ODLY0);
        writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRCMPLX_ODLY_DQ_FINAL + i*4, odly);
      }

// XRAM_DBG: wrcmplx_odly_dbi_final
      if(glbl_wr_dbi_en) {
        for(i=0; i<DQS_BITS; i++) {
          odly = read_riu_nibble_reg(glbl_dbi_bits_riu_map[i][0], ODLY0);
          writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRCMPLX_ODLY_DBI_FINAL + i*4, odly);
        }
      }

      if(status != 0) {
        DEBUG_PRINTF("ERROR: ddr_write_complex(): while finding the center of the window\n");
        return 1;
      }

      // Parallel Bitslice Reset
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
      bitslice_reset_bcast(0, BS_RESET_WT);

      // Read DQS Oscillator count and store the values
      status = lp4_dqs_osc_read(rank, 1);

      if(status == 1) {
        DEBUG_PRINTF("ERROR: ddr_write_complex(): while getting the new DQS oscillator value\n");
        write_error_XRAM(ERR_CODE_63_WR_COMPLEX_FAIL_DQS_OSC_VALUE_ZERO);
      writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
        return 1;
      } else if(status == 2) {
        DEBUG_PRINTF("ERROR: ddr_write_complex(): while getting the new DQS oscillator value\n");
        write_error_XRAM(ERR_CODE_64_WR_COMPLEX_FAIL_DQS_OSC_VALUE_OUT_OF_RANGE);
      writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
        return 1;
      }

      status = get_wr_center_sanity(rank, 1);

      if(status != 0) {
        DEBUG_PRINTF("ERROR: ddr_write_complex(): sanity check failed\n");
        write_error_XRAM(ERR_CODE_59_WR_COMPLEX_FAIL_POST_SANITY_CHECK);
      writeIO_XRAM(XRAM_CAL_RANK,(1 << rank | 1 << 4));
        return 1;
      }

      // PRECHARGE COMMAND
      lp4_bank_row_access(rank /*u8 rank*/, PRECHARGE /*u8 cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);

      // Disable The Write DBI if DM is enabled 
      if((glbl_lp4_mr13 & 0x20) == 0) 
      {
        // Mode Register Write For Write DBI Disable
        lp4_mode_reg_wr(rank, 3, glbl_lp4_mr3);

        // Mode Register Write For Write DM disable
        lp4_mode_reg_wr(rank, 0xd, glbl_lp4_mr13);

        // Write DM Enable in the CAL RTL
        writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
      }
    } // RANKS

  #endif

  if(glbl_phy_rank_switch_en == 1) {
    DEBUG_PRINTF("INFO: ddr_write_complex(): Enabling dynamic mode for TX\n");
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
    riu_reg_bcast(DIS_DYN_MODE, 0, 0x0);

    DEBUG_PRINTF("INFO: ddr_write_complex(): Dummy write for setting correct odelays\n");
    dummy_write_multi_rank_switch(0);
  }

  DEBUG_PRINTF("INFO: ddr_write_complex(): Do the sanity check 1\n");
  status = sanity_check_1();
  if(status != 0) {
    DEBUG_PRINTF("ERROR: ddr_write_complex(): Sanity check 1 Failed\n");
    write_error_XRAM(ERR_CODE_62_WR_COMPLEX_FAIL_POST_SANITY_CHECK_1);
    return 1;
  } else {
    DEBUG_PRINTF("INFO: ddr_write_complex(): Sanity check 1 Passed\n");
  }

  return 0;
}
