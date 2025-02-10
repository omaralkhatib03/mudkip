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
//       Revision:       $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/xsdb_functions.c#1 $
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
#include "bd_d0e7_MC0_ddrc_0_phy_io_config_define.h"

#include "xsdb_reg_define.h"
#include "cal_reg_define.h"
#include "function_dec.c"
#include "global_variable_define.h"

void update_xsdb_cal_status (u8 code, u8 rank) {
// FIXME: Add the status reporting into Microblaze BRAM space

  return;
}
 
void xsdb_dbg(u8 index, u16 address, u16 value){
// FIXME: Add the status reporting into Microblaze BRAM space

  return;
}

void write_err_reg(u8 code, u8 nibble, u8 bit) {
// FIXME: Add the status reporting into Microblaze BRAM space

  return;
}

void write_warning_reg(u16 code, u8 nibble) {
// FIXME: Add the status reporting into Microblaze BRAM space

  return;
}

void write_error_XRAM(u16 error_code) {
   if(glbl_first_error_code == 0) { // Report only the first error
      writeIO_XRAM(XRAM_CAL_ERROR, error_code);
      glbl_first_error_code = error_code;
      if (glbl_margin_check == 1) {
      glbl_first_error_code = 0;
      }
      if(glbl_cal_done) { // Report all the post calibration errors as vt_track_err
         glbl_gpo4 = glbl_gpo4 | (0x1 << UB2PMC_VT_TRACK_ERR);
         writeIO_IOM(IO_GPO4, glbl_gpo4);
      }
   }

   return;
}

void write_error_nibble_XRAM(u8 phy_nibble, u32 error_nibble) {
   u32 error_phy_nibble;

   if(phy_nibble) {
      error_phy_nibble = error_nibble;
   } else {
      error_phy_nibble = l2p_nibble_status(error_nibble);
   }

   writeIO_XRAM(XRAM_CAL_ERROR_PHY_NIBBLE_8_0, (error_phy_nibble & 0x1FF));
   writeIO_XRAM(XRAM_CAL_ERROR_PHY_NIBBLE_17_9, ((error_phy_nibble >> 9) & 0x1FF));
   writeIO_XRAM(XRAM_CAL_ERROR_PHY_NIBBLE_26_18, ((error_phy_nibble >> 18) & 0x1FF));
   return;
}

void write_error_bit_XRAM() {
   u8 bit_cnt;
   u8 phy_nibble;
   u8 phy_bit;
   u32 error_bits;
   u8 error_bit_phy_nibble[27];

   for(phy_nibble=0; phy_nibble < 27; phy_nibble++) {
      error_bit_phy_nibble[phy_nibble] = 0;
   }

   for(bit_cnt=0; bit_cnt < DQ_BITS; bit_cnt++) {
      if(bit_cnt == 0) {
         error_bits = glbl_bits_error_status_31_0;
      } else if (bit_cnt == 32) {
         error_bits = glbl_bits_error_status_63_32;
      } else if (bit_cnt == 64) {
         error_bits = glbl_bits_error_status_95_64;
      }
      if(error_bits & (0x1 << (bit_cnt%32))) {
         phy_nibble = glbl_dq_bits_riu_map[bit_cnt][0];
         phy_bit = glbl_dq_bits_riu_map[bit_cnt][1];
         error_bit_phy_nibble[phy_nibble] |= (0x1 << phy_bit);
      }
   }

   for(phy_nibble=0; phy_nibble < 27; phy_nibble++) {
      writeIO_XRAM(XRAM_CAL_ERROR_BIT_PHY_NIBBLE + (phy_nibble * 4), error_bit_phy_nibble[phy_nibble]);
   }

   return;
}
