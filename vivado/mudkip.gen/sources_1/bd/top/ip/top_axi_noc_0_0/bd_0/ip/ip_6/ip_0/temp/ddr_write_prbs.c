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

u8 ddr_write_prbs()
{
  u8 j;
  u16 left_width[DQS_BITS];
  u16 right_width[DQS_BITS];
  u8 status;

  if(glbl_tck >= TCK_TH_WR_PRBS) {
    DEBUG_PRINTF("INFO: ddr_write_prbs(): SKIPPING write PRBS stage for TCK = %d\n", glbl_tck);
    return 0;
  }

  if(glbl_phy_rank_switch_en == 1) {
    DEBUG_PRINTF("INFO: ddr_write_prbs(): Dummy write for setting correct odelays\n");
    dummy_write_multi_rank_switch(0);

    DEBUG_PRINTF("INFO: ddr_write_prbs(): Disabling dynamic mode for TX\n");
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
    riu_reg_bcast(DIS_DYN_MODE, 0, 0x1);
  }

  for(j=0; j<DQS_BITS; j++) {
    left_width[j] = 0;
    right_width[j] = 0;
  }

  // Find out left and right window widths
  status = get_write_window (0, PRBS, left_width, right_width);

  for(j=0; j<DQS_BITS; j++) {
    if(glbl_cal_done == 0) { // Calibration margins
      // XRAM_DBG: wrprbs_left_margin
      // XRAM_DBG: wrprbs_right_margin
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRPRBS_LEFT_MARGIN + j*4, left_width[j]);
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRPRBS_RIGHT_MARGIN + j*4, right_width[j]);
    } else { // Mission mode margins
      // XRAM_DBG: wrmargin_left
      // XRAM_DBG: wrmargin_right
      writeIO_XRAM(XRAM_WRMARGIN_LEFT + j*4, left_width[j]);
      writeIO_XRAM(XRAM_WRMARGIN_RIGHT + j*4, right_width[j]);
    }
  }

  if(status)
    return 1;

  if(glbl_phy_rank_switch_en == 1) {
    DEBUG_PRINTF("INFO: ddr_write_prbs(): Enabling dynamic mode for TX\n");
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
    riu_reg_bcast(DIS_DYN_MODE, 0, 0x0);

    DEBUG_PRINTF("INFO: ddr_write_prbs(): Dummy write for setting correct odelays\n");
    dummy_write_multi_rank_switch(0);
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
