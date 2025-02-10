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
#include "bd_d0e7_MC0_ddrc_0_phy_io_config_define.h"

#include "cal_reg_define.h"
#include "riu_reg_define.h"
#include "xsdb_reg_define.h"
#include "internal_reg_define.h"
#include "function_dec.c"
#include "global_variable_define.h"

#ifdef LPDDR4
void lpddr4_init_seq () {
	//glbl_term_vref++;
	int wait = 0;
	//************* Setup Mask for write and read data bytes *****************************//
	//   u8  dq_bit;
	//   u32 write_nibbles_mask = 0;
	//   u32 read_nibbles_mask = 0;
	//   bitslice_reset_bcast(0, BS_RESET_WT);
	//   writeIO_DMC(ral_addr_of_xpi_lp4_write_nib_mask_in_ddrmc_main_ddrmc_main_bank(BLK),write_nibbles_mask);
	//   writeIO_DMC(ral_addr_of_xpi_lp4_read_nib_mask_in_ddrmc_main_ddrmc_main_bank(BLK),read_nibbles_mask);
	//   //************************************************************************************//

	//   lp4_mode_reg_wr(rank, 12, 0x40);
	//   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 100);
	//   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

	// Keep default addr to 1s
	writeIO_DMC(ral_addr_of_seq_addr_default_in_ddrmc_main_ddrmc_main_bank(BLK),0xFFFFFFFF);

	// Assert Reset_n and De-assert CKE
	// WE_LOC=0;  CAS_LOC=1;  RAS_LOC=2;  ACT_LOC=3;  CS_LOC=4; CKE_LOC=5;RESET_LOC=6;
	writeIO_DMC(ral_addr_of_seq_cmd_default_in_ddrmc_main_ddrmc_main_bank(BLK),0x0<<5 | 0x100);
	wait = 100000;
	while(wait--);

	// De-assert Reset_n and Keep CKE asserted
	// WE_LOC=0;  CAS_LOC=1;  RAS_LOC=2;  ACT_LOC=3;  CS_LOC=4; CKE_LOC=5;RESET_LOC=6;
	writeIO_DMC(ral_addr_of_seq_cmd_default_in_ddrmc_main_ddrmc_main_bank(BLK),0x2<<5 | 0x100);
	wait = 100000;
	while(wait--);

	// De-assert Reset_n and de-assert CKE
	// WE_LOC=0;  CAS_LOC=1;  RAS_LOC=2;  ACT_LOC=3;  CS_LOC=4; CKE_LOC=5;RESET_LOC=6;
	writeIO_DMC(ral_addr_of_seq_cmd_default_in_ddrmc_main_ddrmc_main_bank(BLK),0x3<<5 | 0x100);
	wait = 100000;
	while(wait--);

	lp4_mode_reg_wr(0, 2, F0_MR2);
	wait = 100;
	while(wait--);
	lp4_mode_reg_wr(1, 2, F0_MR2);
	wait = 100;
	while(wait--);

	//lp4_mode_reg_wr(0, 12, 0x14);
	//	lp4_mode_reg_wr(0, 12, 0x72);
	//lp4_mode_reg_wr(0, 12, glbl_term_vref&0x7F);
	//lp4_mode_reg_wr(0, 12, 0x1B);
	lp4_mode_reg_wr(0, 12, F0_MR12);
	wait = 1000;
	while(wait--);
	lp4_mode_reg_wr(1, 12, F0_MR12);
	wait = 1000;
	while(wait--);

	//	lp4_mode_reg_wr(0, 14, 0x14);
	//	lp4_mode_reg_wr(0, 14, 0x4D);
	//lp4_mode_reg_wr(0, 14, 0x1B);
	lp4_mode_reg_wr(0, 14, F0_MR14);
	wait = 1000;
	while(wait--);
	lp4_mode_reg_wr(1, 14, F0_MR14);
	wait = 1000;
	while(wait--);

	//lp4_mode_reg_wr(0, 22, 0x6);
	lp4_mode_reg_wr(0, 22, F0_MR22);
	//	lp4_mode_reg_wr(0, 22, 0x26);
	//	lp4_mode_reg_wr(0, 22, F0_MR22);
	wait = 100;
	while(wait--);
	lp4_mode_reg_wr(1, 22, F0_MR22);
	//	lp4_mode_reg_wr(0, 22, 0x26);
	//	lp4_mode_reg_wr(0, 22, F0_MR22);
	wait = 100;
	while(wait--);

	//lp4_mode_reg_wr(0, 11, 0x44);
	lp4_mode_reg_wr(0, 11, F0_MR11);
	//	lp4_mode_reg_wr(0, 11, (glbl_term_vref>>7)&0x7F);
	//	lp4_mode_reg_wr(0, 11, F0_MR11);
	wait = 100;
	while(wait--);
	lp4_mode_reg_wr(1, 11, F0_MR11);
	//	lp4_mode_reg_wr(0, 11, (glbl_term_vref>>7)&0x7F);
	//	lp4_mode_reg_wr(0, 11, F0_MR11);
	wait = 100;
	while(wait--);

	// ZQ Start
	lp4_mpc_wr(0, LP4_MPC_START_ZQCAL, 0, 0);
	wait = 100000;
	while(wait--);

	// ZQ Latch
	lp4_mpc_wr(0, LP4_MPC_LATCH_ZQCAL, 0, 0);
	wait = 10000;
	while(wait--);

	// ZQ Start
	lp4_mpc_wr(1, LP4_MPC_START_ZQCAL, 0, 0);
	wait = 100000;
	while(wait--);

	// ZQ Latch
	lp4_mpc_wr(1, LP4_MPC_LATCH_ZQCAL, 0, 0);
	wait = 10000;
	while(wait--);

	//   writeIO_DMC(ral_addr_of_seq_ck_cal_in_ddrmc_main_ddrmc_main_bank(BLK),0xAAAA);
	//   writeIO_DMC(ral_addr_of_seq_cs_cal_in_ddrmc_main_ddrmc_main_bank(BLK),0x0000);
	//   writeIO_DMC(ral_addr_of_seq_addr_cal0_in_ddrmc_main_ddrmc_main_bank(BLK),0x99999999);
	//   writeIO_DMC(ral_addr_of_seq_addr_cal1_in_ddrmc_main_ddrmc_main_bank(BLK),0x99999999);
	//   writeIO_DMC(ral_addr_of_seq_addr_cal2_in_ddrmc_main_ddrmc_main_bank(BLK),0x99999999);
	//   writeIO_DMC(ral_addr_of_seq_addr_cal3_in_ddrmc_main_ddrmc_main_bank(BLK),0x99999999);
	//   writeIO_DMC(ral_addr_of_seq_addr_cal4_in_ddrmc_main_ddrmc_main_bank(BLK),0x99999999);
	//   writeIO_DMC(ral_addr_of_seq_addr_cal5_in_ddrmc_main_ddrmc_main_bank(BLK),0x99999999);
	//
	//   //u32 default_pattern = readIO_DMC(DEFAULT_PATTERN);
	//   //writeIO_DMC(DEFAULT_PATTERN,default_pattern & 0xFFFF00FF);
	//
	//
	//   writeIO_DMC(ral_addr_of_seq_cmd_default_in_ddrmc_main_ddrmc_main_bank(BLK),0x3<<5 | 0x100); // Make default value of the CKE as 1 and the the reset a 1;
	//   writeIO_DMC(ral_addr_of_seq_cke_cal_in_ddrmc_main_ddrmc_main_bank(BLK), 0xFFFF);
	//   writeIO_DMC(ral_addr_of_seq_cs_bits_in_ddrmc_main_ddrmc_main_bank(BLK),0x1<<rank); // Rank select
	//   writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK) , T_CACD); // Setup the tCACD, CS to CS assertion minimum delay
	//   DEBUG_PRINTF("INFO: ddr_ca_cal(): Enable CA calibration mode in the calibration RTL \n");
	//   writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK),3); // Enable CA/CS calibration in the Sequencer.
	//                              // Drives out a clock/address/controls as defined in the "seq_*_cal* registers.
	//                              // This allows the phase adjustment of signals relative to each other and the clock.
	//                              // Full DDR control of each signal is given.
	//                              //
	//
	//   DEBUG_PRINTF("INFO: ddr_ca_cal(): Toggle CKE to 0 to change operating FSP to 1 \n");
	//   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), T_DQSCKE);
	//   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
	//   while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
	//   writeIO_DMC(ral_addr_of_seq_cke_cal_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0000);
	//   writeIO_DMC(ral_addr_of_seq_cmd_default_in_ddrmc_main_ddrmc_main_bank(BLK),0x2<<5 | 0x100); // Make default value of the CKE as 0 and the the reset a 1;
	//   //writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0x1<<8); //Assert write command to drive the dq/dqs pattern out


}
#endif
