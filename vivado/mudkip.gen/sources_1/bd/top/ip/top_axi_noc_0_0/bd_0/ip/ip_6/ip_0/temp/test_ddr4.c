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
//       Revision:       $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/test_ddr4.c#1 $
//                       $Author: peerm $
//                       $DateTime: 2019/01/07 22:27:27 $
//                       $Change: 20180851 $
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
#include "cal_reg_define.h"
#include "riu_reg_define.h"
#include "xsdb_reg_define.h"
#include "internal_reg_define.h"
#include "bd_d0e7_MC0_ddrc_0_phy_cal_config_define.h"
#include "bd_d0e7_MC0_ddrc_0_phy_io_config_define.h"
#include "function_dec.c"

//HW_TEST_CODE_DDR4*************************************************************

#define RD_OPCODE 0x14D
#define WR_OPCODE 0x0AC

void PMC_defaults(void){
   writeIO_DMC(ral_addr_of_seq_cmd_default_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1F);
   
   // writeIO_DMC(SEQ_CKE_DEFAULT, 0x1);
   // writeIO_DMC(SEQ_RESET_DEFAULT, 0x1);
   writeIO_DMC(ral_addr_of_seq_cs_bits_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);
   
   //writeIO_DMC(XPI_PHY_RDEN_DLY_0, 0x3);
   //writeIO_DMC(XPI_PHY_RDEN_DLY_1, 0x3);
   
   //writeIO_DMC(XPI_FIFO_RDEN_DLY_0, 0x5);
   //writeIO_DMC(XPI_FIFO_RDEN_DLY_1, 0x5);
   
   return;
}

void init_test(void){
   
   writeIO_DMC(ral_addr_of_seq_init_start_in_ddrmc_main_ddrmc_main_bank(BLK), 0x01);
   
   while ((readIO_DMC(ral_addr_of_seq_init_start_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0));
   
   return;
}

void sequencer_precharge_cmd(void){
   
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0A);
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x400);
   writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1); writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   
   while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0);
   
   return;
}

void sequencer_activate_cmd(void){
   
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0x00);
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x15);
   writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1); writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   
   while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0);
   
   return;
}

void sequencer_single_rd_cmd(void){
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), RD_OPCODE);
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK), 2);
   
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), (1<<16) | 1);
   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   
   while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0);
   
   return;
}

void sequencer_multi_rd_cmd(void){
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), RD_OPCODE);
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   
   writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 10);
   
   // writeIO_DMC(DQ_COMPARE_DATA, 0);
   //writeIO_DMC(DQ_CLEAR_COMPARE, 1);
   //writeIO_DMC(DQ_CLEAR_COMPARE, 0);
   
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), (20<<16) | 20);
   
   while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0);
   
   return;
}

void sequencer_multi_rd_cmd2(void){
   writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0x01);
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), RD_OPCODE);
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   
   writeIO_DMC(ral_addr_of_seq_b_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), RD_OPCODE);
   writeIO_DMC(ral_addr_of_seq_b_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 8);
   writeIO_DMC(ral_addr_of_seq_b_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   writeIO_DMC(ral_addr_of_seq_b_bg_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   
   writeIO_DMC(ral_addr_of_seq_a_b_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 2);
   writeIO_DMC(ral_addr_of_seq_b_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 2);
   
   // writeIO_DMC(DQ_COMPARE_DATA, 0);
   //writeIO_DMC(DQ_CLEAR_COMPARE, 1);
   //writeIO_DMC(DQ_CLEAR_COMPARE, 0);
   
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), (20<<16) | 20);
   
   while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0);
   writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0x00);
   
   return;
}

void sequencer_single_write(void){
   writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0x00);
   
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), WR_OPCODE);
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1); writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   
   while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0);
   
   return;
}

void sequencer_single_write_read(void){
   writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0x01);
   
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), WR_OPCODE);
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 10);
   
   writeIO_DMC(ral_addr_of_seq_b_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), RD_OPCODE);
   writeIO_DMC(ral_addr_of_seq_b_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   
   writeIO_DMC(ral_addr_of_seq_a_b_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 20);
   writeIO_DMC(ral_addr_of_seq_b_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 10);
   
   // writeIO_DMC(DQ_COMPARE_DATA, 10);
   //writeIO_DMC(DQ_CLEAR_COMPARE, 1);
   //writeIO_DMC(DQ_CLEAR_COMPARE, 0);
   
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), (1<<16) | 2);
   
   while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0);
   
   return;
}

void sequencer_two_writes(void){
   
   writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0x01);
   
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), WR_OPCODE);
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 10);
   
   writeIO_DMC(ral_addr_of_seq_b_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), WR_OPCODE);
   writeIO_DMC(ral_addr_of_seq_b_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   writeIO_DMC(ral_addr_of_seq_b_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 4);
   
   writeIO_DMC(ral_addr_of_seq_a_b_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 8);
   writeIO_DMC(ral_addr_of_seq_b_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 20);
   
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 2);
   
   while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0);
   
   return;
}

void sequencer_single_extend_write(void){
   writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0x00); //1 command only
   
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), WR_OPCODE);
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   
   // writeIO_DMC(SEQ_A_EXTENDED_WRITE, 1);
   writeIO_DMC(ral_addr_of_seq_a_dq_early_in_ddrmc_main_ddrmc_main_bank(BLK), 3);
   writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 10);
   writeIO_DMC(ral_addr_of_seq_a_dq_late_in_ddrmc_main_ddrmc_main_bank(BLK), 7);
   
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1); writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   
   while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0);   
   
   return;
}

void sequencer_single_extend_extra_write(void){
   writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0x00); //1 command only
   
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), WR_OPCODE);
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   
   // writeIO_DMC(SEQ_A_EXTENDED_WRITE, 1);
   writeIO_DMC(ral_addr_of_seq_a_dq_early_in_ddrmc_main_ddrmc_main_bank(BLK), 7);
   writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 10);
   writeIO_DMC(ral_addr_of_seq_a_dq_late_in_ddrmc_main_ddrmc_main_bank(BLK), 3);
   writeIO_DMC(ral_addr_of_seq_a_early_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   writeIO_DMC(ral_addr_of_seq_a_late_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 2);
   
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1); writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   
   while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0);   
   
   return;
}

void sequencer_two_extend_writes_one_read(void){
   writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0x02); //3 commands only
   
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), WR_OPCODE);
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   
   // writeIO_DMC(SEQ_A_EXTENDED_WRITE, 1);
   writeIO_DMC(ral_addr_of_seq_a_dq_early_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 2);
   writeIO_DMC(ral_addr_of_seq_a_dq_late_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   writeIO_DMC(ral_addr_of_seq_a_early_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   writeIO_DMC(ral_addr_of_seq_a_late_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   
   writeIO_DMC(ral_addr_of_seq_b_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), WR_OPCODE);
   writeIO_DMC(ral_addr_of_seq_b_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   
   // writeIO_DMC(SEQ_B_EXTENDED_WRITE, 1);
   writeIO_DMC(ral_addr_of_seq_b_dq_early_in_ddrmc_main_ddrmc_main_bank(BLK), 3);
   writeIO_DMC(ral_addr_of_seq_b_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 4);
   writeIO_DMC(ral_addr_of_seq_b_dq_late_in_ddrmc_main_ddrmc_main_bank(BLK), 5);
   writeIO_DMC(ral_addr_of_seq_b_early_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   writeIO_DMC(ral_addr_of_seq_b_late_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   
   writeIO_DMC(ral_addr_of_seq_c_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), RD_OPCODE);
   writeIO_DMC(ral_addr_of_seq_c_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
   
   // writeIO_DMC(SEQ_C_EXTENDED_WRITE, 0);
   
   writeIO_DMC(ral_addr_of_seq_a_b_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 10);
   writeIO_DMC(ral_addr_of_seq_b_c_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 20);
   writeIO_DMC(ral_addr_of_seq_c_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 6);
   
   // writeIO_DMC(DQ_COMPARE_DATA, 4);
   //writeIO_DMC(DQ_CLEAR_COMPARE, 1);
   //writeIO_DMC(DQ_CLEAR_COMPARE, 0);
   
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), (1<<16) | 3);
   
   while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0);   
   
   // writeIO_DMC(SEQ_A_EXTENDED_WRITE, 0);
   
   return;
}

void sequencer_wrlvl_test(void){
   writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0x00); //1 command
   writeIO_DMC(ral_addr_of_seq_a_dqs_early_in_ddrmc_main_ddrmc_main_bank(BLK), 2); //Default Setting when in WRLVL Mode
   writeIO_DMC(ral_addr_of_seq_cs_bits_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0); //Turn off CS, not needed
   writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 40);
   //writeIO_DMC(SEQ_DATA_CAPTURE_CNT, 30);
   writeIO_DMC(ral_addr_of_seq_dqs_default_in_ddrmc_main_ddrmc_main_bank(BLK), 2);
   
   writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0x01); //WRLVL Mode
   
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), WR_OPCODE);
   
   writeIO_DMC(ral_addr_of_seq_a_dqs_0_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //Actual Command DQS
   
   
   //writeIO_DMC(DQ_CLEAR_COMPARE, 0x01);
   //writeIO_DMC(DQ_CLEAR_COMPARE, 0x00);
   
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), (3<<16) | 3); //send 1, wait for 1 capture
   
   while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0);
   
   //Want an indication the "sample" is taken from HW - TBD
   
   writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0x00); //Turn off WRLVL Mode
   writeIO_DMC(ral_addr_of_seq_cs_bits_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);//Turn back on CS
   
   //writeIO_DMC(DQ_CLEAR_COMPARE, 0x01);
   //writeIO_DMC(DQ_CLEAR_COMPARE, 0x00);
   writeIO_DMC(ral_addr_of_seq_dqs_default_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
}


//HW_TEST_CODE_DDR4_AC_CAL *****************************************************
void sequencer_ac_cal_single(void){
  writeIO_DMC(ral_addr_of_seq_cmd_default_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1F);
  writeIO_DMC(ral_addr_of_seq_cke_cal_in_ddrmc_main_ddrmc_main_bank(BLK), 0xFFFF); //Should make this default in ODS
  writeIO_DMC(ral_addr_of_seq_we_cal_in_ddrmc_main_ddrmc_main_bank(BLK), 0xFFF9); //Just an example for DDR4
  writeIO_DMC(ral_addr_of_seq_cas_cal_in_ddrmc_main_ddrmc_main_bank(BLK), 0xF00F); //Just an example for DDR4
  writeIO_DMC(ral_addr_of_seq_ras_cal_in_ddrmc_main_ddrmc_main_bank(BLK), 0xFF0F); //Just an example for DDR4
	
  writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0x00); //1 commands
  writeIO_DMC(ral_addr_of_seq_ck_cal_in_ddrmc_main_ddrmc_main_bank(BLK), 0xCCCC); //Divided by 2 clk pattern
  writeIO_DMC(ral_addr_of_seq_cke_cal_in_ddrmc_main_ddrmc_main_bank(BLK), 0xFFFF); //Should make this default
  writeIO_DMC(ral_addr_of_seq_cs_cal_in_ddrmc_main_ddrmc_main_bank(BLK), 0xFF9F);
  writeIO_DMC(ral_addr_of_seq_addr_cal0_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0060);
  writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0x02); //AC Cal Mode
  
  writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1); writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   
  while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0);
  
  writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0x00); //AC Cal Mode
  
  writeIO_DMC(ral_addr_of_seq_we_cal_in_ddrmc_main_ddrmc_main_bank(BLK), 0xFFFF);
  writeIO_DMC(ral_addr_of_seq_cas_cal_in_ddrmc_main_ddrmc_main_bank(BLK), 0xFFFF);
  writeIO_DMC(ral_addr_of_seq_ras_cal_in_ddrmc_main_ddrmc_main_bank(BLK), 0xFFFF);
  
  writeIO_DMC(ral_addr_of_seq_ck_cal_in_ddrmc_main_ddrmc_main_bank(BLK), 0xF0F0); //Divided by 4 clk pattern
  writeIO_DMC(ral_addr_of_seq_cs_cal_in_ddrmc_main_ddrmc_main_bank(BLK), 0xFF00);
  writeIO_DMC(ral_addr_of_seq_addr_cal0_in_ddrmc_main_ddrmc_main_bank(BLK), 0x00FF); //SDR
  writeIO_DMC(ral_addr_of_seq_addr_cal1_in_ddrmc_main_ddrmc_main_bank(BLK), 0xFF00); //SDR 2nd edge
  writeIO_DMC(ral_addr_of_seq_addr_cal2_in_ddrmc_main_ddrmc_main_bank(BLK), 0x003C); //less than SDR timing
  writeIO_DMC(ral_addr_of_seq_addr_cal3_in_ddrmc_main_ddrmc_main_bank(BLK), 0x3C00); //less than SDR timing
  writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0x02); //AC Cal Mode
  writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1); writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   
  while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0);
  
  writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0x00); //AC Cal Mode
  
  return;
}

//RPI Test code
#define RIU_RST 0x0000800B

void rpi_rst_test(void){
  
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_RPI, RPI reset test\n");
  writeIO_RIU(RIU_RST, 0x01); //assert the reset
  writeIO_RIU(RIU_RST, 0x00); //deassert the reset
  
  return;
}

void rpi_bcast_nibble_mask_test(void){
    
  u32 read_mask = 0;
  
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_RPI, Broadcast nibble mask test\n");
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, 0x0000DEAD);
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, 0x0000BEEF);
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK2, 0x07FFFFFF);
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK3, 0x07777777);
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK4, 0x01111111);
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK5, 0x05555555);
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK6, 0x03BBBBBB);
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK7, 0x01999999);
  
  read_mask = readIO_RIU(RPI_BCAST_NIBBLE_MASK0);
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_RPI, RPI_BCAST_NIBBLE_MASK0 %08x\n", read_mask);
  read_mask = readIO_RIU(RPI_BCAST_NIBBLE_MASK1);
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_RPI, RPI_BCAST_NIBBLE_MASK1 %08x\n", read_mask);
  read_mask = readIO_RIU(RPI_BCAST_NIBBLE_MASK2);
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_RPI, RPI_BCAST_NIBBLE_MASK2 %08x\n", read_mask);
  read_mask = readIO_RIU(RPI_BCAST_NIBBLE_MASK3);
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_RPI, RPI_BCAST_NIBBLE_MASK0 %08x\n", read_mask);
  read_mask = readIO_RIU(RPI_BCAST_NIBBLE_MASK4);
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_RPI, RPI_BCAST_NIBBLE_MASK4 %08x\n", read_mask);
  read_mask = readIO_RIU(RPI_BCAST_NIBBLE_MASK5);
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_RPI, RPI_BCAST_NIBBLE_MASK5 %08x\n", read_mask);
  read_mask = readIO_RIU(RPI_BCAST_NIBBLE_MASK6);
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_RPI, RPI_BCAST_NIBBLE_MASK6 %08x\n", read_mask);
  read_mask = readIO_RIU(RPI_BCAST_NIBBLE_MASK7);
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_RPI, RPI_BCAST_NIBBLE_MASK7 %08x\n", read_mask);

  //Try to use the BCAST mask
  writeIO_RIU(0x00024000 | (0 << 8) | 0x07, 0x00); //Set PQTR for a test
  writeIO_RIU(0x00024000 | (1 << 8) | 0x07, 0x01);
  writeIO_RIU(0x00024000 | (2 << 8) | 0x07, 0x02);
  writeIO_RIU(0x00024000 | (3 << 8) | 0x07, 0x03);
  writeIO_RIU(0x00024000 | (4 << 8) | 0x07, 0x04);
  writeIO_RIU(0x00024000 | (5 << 8) | 0x07, 0x05);
  writeIO_RIU(0x00024000 | (6 << 8) | 0x07, 0x06);
  writeIO_RIU(0x00024000 | (7 << 8) | 0x07, 0x07);
  
  writeIO_RIU(0x00024000 | (2 << 8) | 0x07, 0x00); //Clear
  
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, 0x00000000);
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, 0x00000000);
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK2, 0x00000000);
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK3, 0x00000000);
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK4, 0x00000000);
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK5, 0x00000000);
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK6, 0x00000000);
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK7, 0x00000000);
  
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_RPI, Broadcast nibble mask test END\n");
  return;
}

void rpi_adjustments_test(void){
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_RPI, Adjustments test\n");
  
  u32 read_data = 0;
  u8 loop_cnt   = 0;
  
  for (loop_cnt = 0; loop_cnt < 8 ; loop_cnt++){
    DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_RPI, Loop Count %08x\n", loop_cnt);
    writeIO_RIU(RIU_NIBBLE_SEL_ADJ, loop_cnt);
    writeIO_RIU(RIU_R_VALID_ADJ, loop_cnt);
    
    writeIO_RIU(0x08, 0x01);
    
    read_data = readIO_RIU(0x08);
    DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_RPI, Read Data %08x\n", read_data);
    read_data = readIO_RIU(RIU_READ_DATA_DBG);
    DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_RPI, Read Data Debug%08x\n", read_data);
  }
  
  read_data = readIO_RIU((0x0000001C<<8));
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_RPI, Read Data PLL (address 28)%08x\n", read_data);
  
  read_data = readIO_RIU((0x0000001E<<8));
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_RPI, Read Data PLL (address 30)%08x\n", read_data);
  
  read_data = readIO_RIU((0x00000020<<8));
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_RPI, Read Data PLL (address 32)%08x\n", read_data);
  
  writeIO_RIU(0x08, 0x00);
  writeIO_RIU(RIU_NIBBLE_SEL_ADJ, 0);
  writeIO_RIU(RIU_R_VALID_ADJ, 0);
  
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_RPI, Adjustments test END\n");
  return;
}

void seq_coverage(void){
  #ifdef DDR4

  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_SEQ_COVERAGE, Start \n");
    
  //re-run memory init with slot set to 1
  writeIO_DMC(ral_addr_of_seq_init_cmd_set_in_ddrmc_main_ddrmc_main_bank(BLK), 0x17F); //Set slot to 1 for a test, assume this is DDR4 so use NOP
  /*writeIO_DMC(ral_addr_of_seq_init_start_in_ddrmc_main_ddrmc_main_bank(BLK), 0x01); //Start
  //Wait for that to finish
  while ((readIO_DMC(ral_addr_of_seq_init_status_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x1));
   
  writeIO_DMC(ral_addr_of_seq_init_cmd_set_in_ddrmc_main_ddrmc_main_bank(BLK), 0x07F); //Set back to 0
   
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_SEQ_COVERAGE, Slot 1 init test done \n");*/
  
  //Run some regular commands with different options, don't care about data.
  writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0x02); //3 cmds
  writeIO_DMC(ral_addr_of_seq_a_b_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 10); //arbitrary delays
  writeIO_DMC(ral_addr_of_seq_b_c_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 20);
  writeIO_DMC(ral_addr_of_seq_c_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 6);
  
  writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 3);
  
  //1. Write (slot 1), write (slot 0), write (slot 1) --------------------------
  writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), WR_OPCODE);
  writeIO_DMC(ral_addr_of_seq_b_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), WR_OPCODE);
  writeIO_DMC(ral_addr_of_seq_c_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), WR_OPCODE);
  
  writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 8);
  writeIO_DMC(ral_addr_of_seq_b_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 9);
  writeIO_DMC(ral_addr_of_seq_c_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 10);
  
  writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x200); //no CS, slot 1 set
  writeIO_DMC(ral_addr_of_seq_b_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x000); //no CS, slot 0 set
  writeIO_DMC(ral_addr_of_seq_c_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x200); //no CS, slot 1 set
  
  writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1); //START
  while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0); //Wait for DONE
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_SEQ_COVERAGE, TEST 1 done \n");
  
  //2. Write (slot 0), write (slot 1), write (slot 0) --------------------------
  writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 11);
  writeIO_DMC(ral_addr_of_seq_b_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 12);
  writeIO_DMC(ral_addr_of_seq_c_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 13);
  
  writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x000); //no CS, slot 0 set
  writeIO_DMC(ral_addr_of_seq_b_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x200); //no CS, slot 1 set
  writeIO_DMC(ral_addr_of_seq_c_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x000); //no CS, slot 0 set
  
  writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1); //START
  while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0); //Wait for DONE
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_SEQ_COVERAGE, TEST 2 done \n");
  
  //3. Extended writes for all, Write (slot 0), Write (slot 1), Write (slot 0) --------------------------
  writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x009); //no CS, slot 0 set
  writeIO_DMC(ral_addr_of_seq_b_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x209); //no CS, slot 1 set
  writeIO_DMC(ral_addr_of_seq_c_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x009); //no CS, slot 0 set
  
  writeIO_DMC(ral_addr_of_seq_a_early_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 3);
  writeIO_DMC(ral_addr_of_seq_b_early_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 2);
  writeIO_DMC(ral_addr_of_seq_c_early_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
  
  writeIO_DMC(ral_addr_of_seq_a_late_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 3);
  writeIO_DMC(ral_addr_of_seq_b_late_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 2);
  writeIO_DMC(ral_addr_of_seq_c_late_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
  
  writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1); //START
  while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0); //Wait for DONE
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_SEQ_COVERAGE, TEST 3 done \n");
  
  //Loop through various write patterns to be hit
  u8 loop_cnt = 0;
  
  writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0x00); //1 cmd mode
  writeIO_DMC(ral_addr_of_seq_a_b_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 5); //arbitrary delays
  writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
  writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), WR_OPCODE);
  
  for (loop_cnt = 0; loop_cnt < 28; loop_cnt++){
    //select different pattern for early/late
    writeIO_DMC(ral_addr_of_seq_a_dq_early_in_ddrmc_main_ddrmc_main_bank(BLK), ((loop_cnt+1)%28));
    writeIO_DMC(ral_addr_of_seq_a_dqs_early_in_ddrmc_main_ddrmc_main_bank(BLK), ((loop_cnt+1)%5));
    writeIO_DMC(ral_addr_of_seq_a_dm_early_in_ddrmc_main_ddrmc_main_bank(BLK), ((loop_cnt+1)%6));
      
    writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), loop_cnt);
    writeIO_DMC(ral_addr_of_seq_a_dq_1_in_ddrmc_main_ddrmc_main_bank(BLK), loop_cnt);
    writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), ((loop_cnt<<5) | loop_cnt));
    writeIO_DMC(ral_addr_of_seq_a_dqs_0_in_ddrmc_main_ddrmc_main_bank(BLK), (loop_cnt%5));
    writeIO_DMC(ral_addr_of_seq_a_dqs_1_in_ddrmc_main_ddrmc_main_bank(BLK), (loop_cnt%5));
    writeIO_DMC(ral_addr_of_seq_a_dm_0_in_ddrmc_main_ddrmc_main_bank(BLK), (loop_cnt%6));
    writeIO_DMC(ral_addr_of_seq_a_dm_1_in_ddrmc_main_ddrmc_main_bank(BLK), (loop_cnt%6));
    
    writeIO_DMC(ral_addr_of_seq_a_dq_late_in_ddrmc_main_ddrmc_main_bank(BLK), ((loop_cnt+2)%28));
    writeIO_DMC(ral_addr_of_seq_a_dqs_late_in_ddrmc_main_ddrmc_main_bank(BLK), ((loop_cnt+2)%5));
    writeIO_DMC(ral_addr_of_seq_a_dm_late_in_ddrmc_main_ddrmc_main_bank(BLK), ((loop_cnt+2)%6));
    
    writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1); //START
    while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0); //Wait for DONE
  }
  
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_SEQ_COVERAGE, pattern loop done \n");
  
  //Test out driving RESET AND CKE values
  writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x100); //no CS, slot 0 set
  writeIO_DMC(ral_addr_of_seq_b_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x100); //no CS, slot 0 set
  writeIO_DMC(ral_addr_of_seq_c_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x100); //no CS, slot 0 set

  writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0x07F); //Drive RESET and CKE high (same value as it is)
  
  writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1); //START
  while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0); //Wait for DONE
  
  //Test out using all the side command locations ------------------------------
  /*writeIO_DMC(ral_addr_of_seq_side_cmd_set_in_ddrmc_main_ddrmc_main_bank(BLK), 0x17F); //Set slot to 1 for a test, assume this is DDR4 so use NOP
  writeIO_DMC(ral_addr_of_seq_side_cmd_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 16); //test all locations
  
  writeIO_DMC(ral_addr_of_seq_side_cmd0_in_ddrmc_main_ddrmc_main_bank(BLK), (4<<9)); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd1_in_ddrmc_main_ddrmc_main_bank(BLK), (5<<9)); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd2_in_ddrmc_main_ddrmc_main_bank(BLK), (6<<9)); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd3_in_ddrmc_main_ddrmc_main_bank(BLK), (7<<9)); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd4_in_ddrmc_main_ddrmc_main_bank(BLK), (8<<9)); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd5_in_ddrmc_main_ddrmc_main_bank(BLK), (9<<9)); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd6_in_ddrmc_main_ddrmc_main_bank(BLK), (10<<9)); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd7_in_ddrmc_main_ddrmc_main_bank(BLK), (11<<9)); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd8_in_ddrmc_main_ddrmc_main_bank(BLK), (12<<9)); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd9_in_ddrmc_main_ddrmc_main_bank(BLK), (13<<9)); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd10_in_ddrmc_main_ddrmc_main_bank(BLK), (14<<9)); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd11_in_ddrmc_main_ddrmc_main_bank(BLK), (15<<9)); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd12_in_ddrmc_main_ddrmc_main_bank(BLK), (16<<9)); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd13_in_ddrmc_main_ddrmc_main_bank(BLK), (17<<9)); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd14_in_ddrmc_main_ddrmc_main_bank(BLK), (18<<9)); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd15_in_ddrmc_main_ddrmc_main_bank(BLK), (19<<9)); //delays between commands
  
  writeIO_DMC(ral_addr_of_seq_side_cmd_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1); //START
  while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0); //Wait for DONE
  
  writeIO_DMC(ral_addr_of_seq_side_cmd_set_in_ddrmc_main_ddrmc_main_bank(BLK), 0x07F); //Set back to 0
  writeIO_DMC(ral_addr_of_seq_side_cmd_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  writeIO_DMC(ral_addr_of_seq_side_cmd0_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd1_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd2_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd3_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd4_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd5_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd6_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd7_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd8_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd9_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd10_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd11_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd12_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd13_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd14_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //delays between commands
  writeIO_DMC(ral_addr_of_seq_side_cmd15_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //delays between commands*/
  //end of side cmd testing ----------------------------------------------------
  
  writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x000); //no CS, slot 0 set
  writeIO_DMC(ral_addr_of_seq_b_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x000); //no CS, slot 0 set
  writeIO_DMC(ral_addr_of_seq_c_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x000); //no CS, slot 0 set
  
  writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  writeIO_DMC(ral_addr_of_seq_b_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  writeIO_DMC(ral_addr_of_seq_c_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  writeIO_DMC(ral_addr_of_seq_a_early_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  writeIO_DMC(ral_addr_of_seq_b_early_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  writeIO_DMC(ral_addr_of_seq_c_early_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  writeIO_DMC(ral_addr_of_seq_a_late_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  writeIO_DMC(ral_addr_of_seq_b_late_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  writeIO_DMC(ral_addr_of_seq_c_late_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  
  writeIO_DMC(ral_addr_of_seq_a_dq_early_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  writeIO_DMC(ral_addr_of_seq_a_dqs_early_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  writeIO_DMC(ral_addr_of_seq_a_dm_early_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
      
  writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  writeIO_DMC(ral_addr_of_seq_a_dq_1_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  writeIO_DMC(ral_addr_of_seq_a_dqs_0_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  writeIO_DMC(ral_addr_of_seq_a_dqs_1_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  writeIO_DMC(ral_addr_of_seq_a_dm_0_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  writeIO_DMC(ral_addr_of_seq_a_dm_1_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
    
  writeIO_DMC(ral_addr_of_seq_a_dq_late_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  writeIO_DMC(ral_addr_of_seq_a_dqs_late_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  writeIO_DMC(ral_addr_of_seq_a_dm_late_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  
  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_SEQ_COVERAGE, Done\n");
  
  #endif
  
  return;
}
