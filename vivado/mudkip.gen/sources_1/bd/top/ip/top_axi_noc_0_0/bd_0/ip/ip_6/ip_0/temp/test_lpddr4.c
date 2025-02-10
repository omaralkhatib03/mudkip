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
//       Revision:       $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/test_lpddr4.c#1 $
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

//HW_TEST_CODE_LPDDR4***********************************************************

#define ACT_OPCODE 0x5
#define PRE_OPCODE ACT_OPCODE
#define RD_OPCODE 0x45
#define WR_OPCODE 0x25

void PMC_defaults_lpddr4(void){
   writeIO_DMC(ral_addr_of_seq_cmd_default_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10);
   
   // writeIO_DMC(SEQ_CKE_DEFAULT, 0x1);
   // writeIO_DMC(SEQ_RESET_DEFAULT, 0x1);
   writeIO_DMC(ral_addr_of_seq_cs_bits_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);
   
   //writeIO_DMC(XPI_PHY_RDEN_DLY_0, 0x3);
   //writeIO_DMC(XPI_PHY_RDEN_DLY_1, 0x3);
   
   //writeIO_DMC(XPI_FIFO_RDEN_DLY_0, 0x5);
   //writeIO_DMC(XPI_FIFO_RDEN_DLY_1, 0x5);
   
   return;
}
void sequencer_activate_cmd_lpddr4(void){
  //Requires 4 commands
  writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 1); //Indicates 4 commands to sequencer in LPDDR4 mode
  
  writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), ACT_OPCODE); //Chip Select Assertion
  writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), ((0x03)<<18)|((0x03)<<12)|((0x0)<<6)|(0x01));
   
  writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   
  while (readIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0);
  
  return;
}
void sequencer_single_write_lpddr4(void){
  //Requires 4 commands
  writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 1); //Indicates 4 commands to sequencer in LPDDR4 mode
  
  writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), WR_OPCODE); //Chip Select Assertion
  writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), ((0x00)<<18)|((0x12)<<12)|((0x0)<<6)|(0x04));//
   
  writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   
  while (readIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0);
  return;
}
void sequencer_single_rd_cmd_lpddr4(void){
  //Requires 4 commands
  writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 1); //Indicates 4 commands to sequencer in LPDDR4 mode
  
  writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), RD_OPCODE); //Chip Select Assertion
  writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), ((0x00)<<18)|((0x12)<<12)|((0x0)<<6)|(0x02)); //
   
  writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   
  while (readIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0);
  return;
}
void sequencer_precharge_cmd_lpddr4(void){
  //Requires 2 commands
  writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //Indicates 4 commands to sequencer in LPDDR4 mode
  
  writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), PRE_OPCODE); //Chip Select Assertion
  writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x30); //Precharge All banks
   
  writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   
  while (readIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0);
  return;
}


//HW_TEST_CODE_LPDDR4_SLOW_INIT*************************************************

//the CMD is defined as {extend, delay, opcode}. We typically don't extend the 
//opcode after dealing with RESET/CKE
#define DLY_SHIFT         7
#define ADDR1_SHIFT       6
#define ADDR2_SHIFT       (ADDR1_SHIFT*2)
#define ADDR3_SHIFT       (ADDR1_SHIFT*3)
#define OP7_MASK          0x80
#define OP6_MASK          0x40
#define OP7_SHIFT         2 //Right shift to get to location 5
#define OP6_SHIFT         (OP7_SHIFT-1) //Right shift to get to location 5
#define OP_MASK           0x3F
#define MRS_OPCODE        0x65 //{RESET, CKE, n/a, CS for 4 phases}
#define MRS_P0            0x06 //First part of MRS cmd
#define MRS_P2            0x16 //Second part of MRS cmd

#define MRS1_CA           0x01
#define MRS2_CA           0x02
#define MRS3_CA           0x03
//#define MRS4_CA           0x04
//#define MRS10_CA          0x0A
#define MRS11_CA          0x0B
#define MRS12_CA          0x0C
#define MRS13_CA          0x0D
#define MRS14_CA          0x0E
#define MRS15_CA          0x0F
#define MRS16_CA          0x10
#define MRS17_CA          0x11
#define MRS20_CA          0x14
#define MRS22_CA          0x16
//#define MRS23_CA          0x17
//#define MRS24_CA          0x18
//#define MRS32_CA          0x20
//#define MRS40_CA          0x28

#define MRS_DLY           0x0A //Generic Delay between MRS commands

//7-bits
//We should define the settings for Both frequencies and program them both during Init
//for right now only basic settings are set for one
#define MRS1_OP           0x04
#define MRS2_OP           0x00
#define MRS3_OP           0x18
//#define MRS4_OP           0x00 //for now exclude MR4
//#define MRS10_OP          0x00 //for now exclude MR10 also (assume no need to reset ZQ)
#define MRS11_OP          0x00 //Depends on SIPI settings recommendations
#define MRS12_OP          0x6C
#define MRS13_OP          0x00
#define MRS14_OP          0x6C
#define MRS15_OP          0x00 //No data bit inversion
#define MRS16_OP          0x00
#define MRS17_OP          0x00
#define MRS20_OP          0x00
#define MRS22_OP          0x00 //Depends on SIPI settings recommendations
//#define MRS23_OP          0x00 //Maybe skip this one since it starts the DQS interval timer
//#define MRS24_OP          0x00 //Skip this for now, just use default
//#define MRS32_OP          0x00 //Skip this, for Cal only
//#define MRS40_OP          0x00 //Skip this, for Cal only

#define MRS1_P0_OP        (((MRS1_OP&OP7_MASK)>>OP7_SHIFT)|MRS_P0)
#define MRS1_P2_OP        (((MRS1_OP&OP6_MASK)>>OP6_SHIFT)|MRS_P2)

#define MRS2_P0_OP        (((MRS2_OP&OP7_MASK)>>OP7_SHIFT)|MRS_P0)
#define MRS2_P2_OP        (((MRS2_OP&OP6_MASK)>>OP6_SHIFT)|MRS_P2)

#define MRS3_P0_OP        (((MRS3_OP&OP7_MASK)>>OP7_SHIFT)|MRS_P0)
#define MRS3_P2_OP        (((MRS3_OP&OP6_MASK)>>OP6_SHIFT)|MRS_P2)

#define MRS11_P0_OP       (((MRS11_OP&OP7_MASK)>>OP7_SHIFT)|MRS_P0)
#define MRS11_P2_OP       (((MRS11_OP&OP6_MASK)>>OP6_SHIFT)|MRS_P2)

#define MRS12_P0_OP       (((MRS12_OP&OP7_MASK)>>OP7_SHIFT)|MRS_P0)
#define MRS12_P2_OP       (((MRS12_OP&OP6_MASK)>>OP6_SHIFT)|MRS_P2)

#define MRS13_P0_OP       (((MRS13_OP&OP7_MASK)>>OP7_SHIFT)|MRS_P0)
#define MRS13_P2_OP       (((MRS13_OP&OP6_MASK)>>OP6_SHIFT)|MRS_P2)

#define MRS14_P0_OP       (((MRS14_OP&OP7_MASK)>>OP7_SHIFT)|MRS_P0)
#define MRS14_P2_OP       (((MRS14_OP&OP6_MASK)>>OP6_SHIFT)|MRS_P2)

#define MRS15_P0_OP       (((MRS15_OP&OP7_MASK)>>OP7_SHIFT)|MRS_P0)
#define MRS15_P2_OP       (((MRS15_OP&OP6_MASK)>>OP6_SHIFT)|MRS_P2)

#define MRS16_P0_OP       (((MRS16_OP&OP7_MASK)>>OP7_SHIFT)|MRS_P0)
#define MRS16_P2_OP       (((MRS16_OP&OP6_MASK)>>OP6_SHIFT)|MRS_P2)

#define MRS17_P0_OP       (((MRS17_OP&OP7_MASK)>>OP7_SHIFT)|MRS_P0)
#define MRS17_P2_OP       (((MRS17_OP&OP6_MASK)>>OP6_SHIFT)|MRS_P2)

#define MRS20_P0_OP       (((MRS20_OP&OP7_MASK)>>OP7_SHIFT)|MRS_P0)
#define MRS20_P2_OP       (((MRS20_OP&OP6_MASK)>>OP6_SHIFT)|MRS_P2)

#define MRS22_P0_OP       (((MRS22_OP&OP7_MASK)>>OP7_SHIFT)|MRS_P0)
#define MRS22_P2_OP       (((MRS22_OP&OP6_MASK)>>OP6_SHIFT)|MRS_P2)

#define LPDDR4_MRS_CMD   ((MRS_DLY<<DLY_SHIFT)|(MRS_OPCODE))
#define LPDDR4_MRS1_ADDR (((MRS1_OP&OP_MASK)<<ADDR3_SHIFT)|\
                           (MRS1_P2_OP      <<ADDR2_SHIFT)|\
						   (MRS1_CA         <<ADDR1_SHIFT)|\
						   (MRS1_P0_OP))
						   
#define LPDDR4_MRS2_ADDR (((MRS2_OP&OP_MASK)<<ADDR3_SHIFT)|\
                           (MRS2_P2_OP      <<ADDR2_SHIFT)|\
						   (MRS2_CA         <<ADDR1_SHIFT)|\
						   (MRS2_P0_OP))
						   
#define LPDDR4_MRS3_ADDR (((MRS3_OP&OP_MASK)<<ADDR3_SHIFT)|\
                           (MRS3_P2_OP      <<ADDR2_SHIFT)|\
						   (MRS3_CA         <<ADDR1_SHIFT)|\
						   (MRS3_P0_OP))
						   
#define LPDDR4_MRS11_ADDR (((MRS11_OP&OP_MASK)<<ADDR3_SHIFT)|\
                            (MRS11_P2_OP      <<ADDR2_SHIFT)|\
						    (MRS11_CA         <<ADDR1_SHIFT)|\
						    (MRS11_P0_OP))
							
#define LPDDR4_MRS12_ADDR (((MRS12_OP&OP_MASK)<<ADDR3_SHIFT)|\
                            (MRS12_P2_OP      <<ADDR2_SHIFT)|\
						    (MRS12_CA         <<ADDR1_SHIFT)|\
						    (MRS12_P0_OP))
							
#define LPDDR4_MRS13_ADDR (((MRS13_OP&OP_MASK)<<ADDR3_SHIFT)|\
                            (MRS13_P2_OP      <<ADDR2_SHIFT)|\
						    (MRS13_CA         <<ADDR1_SHIFT)|\
						    (MRS13_P0_OP))
							
#define LPDDR4_MRS14_ADDR (((MRS14_OP&OP_MASK)<<ADDR3_SHIFT)|\
                            (MRS14_P2_OP      <<ADDR2_SHIFT)|\
						    (MRS14_CA         <<ADDR1_SHIFT)|\
						    (MRS14_P0_OP))
							
#define LPDDR4_MRS15_ADDR (((MRS15_OP&OP_MASK)<<ADDR3_SHIFT)|\
                            (MRS15_P2_OP      <<ADDR2_SHIFT)|\
						    (MRS15_CA         <<ADDR1_SHIFT)|\
						    (MRS15_P0_OP))
							
#define LPDDR4_MRS16_ADDR (((MRS16_OP&OP_MASK)<<ADDR3_SHIFT)|\
                            (MRS16_P2_OP      <<ADDR2_SHIFT)|\
						    (MRS16_CA         <<ADDR1_SHIFT)|\
						    (MRS16_P0_OP))
							
#define LPDDR4_MRS17_ADDR (((MRS17_OP&OP_MASK)<<ADDR3_SHIFT)|\
                            (MRS17_P2_OP      <<ADDR2_SHIFT)|\
						    (MRS17_CA         <<ADDR1_SHIFT)|\
						    (MRS17_P0_OP))
							
#define LPDDR4_MRS20_ADDR (((MRS20_OP&OP_MASK)<<ADDR3_SHIFT)|\
                            (MRS20_P2_OP      <<ADDR2_SHIFT)|\
						    (MRS20_CA         <<ADDR1_SHIFT)|\
						    (MRS20_P0_OP))
							
#define LPDDR4_MRS22_ADDR (((MRS22_OP&OP_MASK)<<ADDR3_SHIFT)|\
                            (MRS22_P2_OP      <<ADDR2_SHIFT)|\
						    (MRS22_CA         <<ADDR1_SHIFT)|\
						    (MRS22_P0_OP))

#define ZQ_OPCODE          0x61 //{RESET, CKE, n/a, CS for 4 phases}
                                //Cmd for only 2 clocks, with 2 NOPS after
#define ZQ_START_OP        0x79
#define ZQ_LATCH_OP        0x45

#define ZQ_START_DLY       0x60//1us MIN (use arbitrary small value for now)
#define ZQ_LATCH_DLY       0x20//30ns MIN (or 8 tck whichever is bigger)

#define ZQ_START_P0_OP     ((ZQ_START_OP&OP6_MASK)>>OP6_SHIFT) //Regular address is all 0
#define ZQ_START_P1_OP     (ZQ_START_OP&OP_MASK)

#define ZQ_LATCH_P0_OP     ((ZQ_LATCH_OP&OP6_MASK)>>OP6_SHIFT) //Regular address is all 0
#define ZQ_LATCH_P1_OP     (ZQ_LATCH_OP&OP_MASK)

#define LPDDR4_ZQ_START_CMD  ((ZQ_START_DLY<<DLY_SHIFT)|(ZQ_OPCODE))
#define LPDDR4_ZQ_START_ADDR ((ZQ_START_P1_OP <<ADDR1_SHIFT)|\
						      (ZQ_START_P0_OP))

#define LPDDR4_ZQ_LATCH_CMD  ((ZQ_LATCH_DLY<<DLY_SHIFT)|(ZQ_OPCODE))							  
#define LPDDR4_ZQ_LATCH_ADDR ((ZQ_LATCH_P1_OP <<ADDR1_SHIFT)|\
						      (ZQ_LATCH_P0_OP))
							  
void sequencer_setup_slow_ck(u8 on){
  if (on == 1){
    writeIO_DMC(ral_addr_of_seq_ck_cal_in_ddrmc_main_ddrmc_main_bank(BLK), 0xF0F0); //Divided by 4 clk pattern
	writeIO_DMC(ral_addr_of_seq_cmd_default_in_ddrmc_main_ddrmc_main_bank(BLK), 0x00);
	// writeIO_DMC(SEQ_CKE_DEFAULT, 0x1);
	// writeIO_DMC(SEQ_RESET_DEFAULT, 0x1);
	writeIO_DMC(ral_addr_of_seq_cs_bits_in_ddrmc_main_ddrmc_main_bank(BLK),0x1); // Rank select
	
	writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0x03); //AC Cal Mode
  } else {
    writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0x00); //Regular mode
  }
  return;
}

void lpddr4_init_test(void){
  //Setup values for init
  //Init_cmd is {extend, dly, opcode}
  writeIO_DMC(ral_addr_of_seq_init_cmd0_in_ddrmc_main_ddrmc_main_bank(BLK), ((0x1)<<29)|((0x0A)<<7)|(0x00)); //Drop RST low (extend)
  writeIO_DMC(ral_addr_of_seq_init_cmd1_in_ddrmc_main_ddrmc_main_bank(BLK), ((0x0)<<29)|((0x3F)<<7)|(0x40)); //Raise RST back high (extend not needed since default)
                                                                //Guessing on count value, must be 2ms min before CKE!
																//For now use a smaller value for testing
  writeIO_DMC(ral_addr_of_seq_init_cmd2_in_ddrmc_main_ddrmc_main_bank(BLK), ((0x0)<<29)|((0x7F)<<7)|(0x60)); //Raise CKE to high, wait 2us. Which is a ton of time!
                                                                //Default is 1, no need to extend cmd.
  //Some of these might be able to be skipped depending on the values
  //Most are just set to default for now  
  writeIO_DMC(ral_addr_of_seq_init_cmd3_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS_CMD);
  writeIO_DMC(ral_addr_of_seq_init_addr3_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS1_ADDR); //MRS1
  
  writeIO_DMC(ral_addr_of_seq_init_cmd4_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS_CMD);
  writeIO_DMC(ral_addr_of_seq_init_addr4_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS2_ADDR); //MRS2
  
  writeIO_DMC(ral_addr_of_seq_init_cmd5_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS_CMD);
  writeIO_DMC(ral_addr_of_seq_init_addr5_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS3_ADDR); //MRS3
  
  writeIO_DMC(ral_addr_of_seq_init_cmd6_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS_CMD);
  writeIO_DMC(ral_addr_of_seq_init_addr6_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS11_ADDR); //MRS11
  
  writeIO_DMC(ral_addr_of_seq_init_cmd7_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS_CMD);
  writeIO_DMC(ral_addr_of_seq_init_addr7_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS12_ADDR); //MRS12
  
  writeIO_DMC(ral_addr_of_seq_init_cmd8_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS_CMD);
  writeIO_DMC(ral_addr_of_seq_init_addr8_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS13_ADDR); //MRS13
  
  writeIO_DMC(ral_addr_of_seq_init_cmd9_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS_CMD);
  writeIO_DMC(ral_addr_of_seq_init_addr9_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS14_ADDR); //MRS14
  
  writeIO_DMC(ral_addr_of_seq_init_cmd10_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS_CMD);
  writeIO_DMC(ral_addr_of_seq_init_addr10_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS15_ADDR); //MRS15
  
  writeIO_DMC(ral_addr_of_seq_init_cmd11_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS_CMD);
  writeIO_DMC(ral_addr_of_seq_init_addr11_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS16_ADDR); //MRS16
  
  writeIO_DMC(ral_addr_of_seq_init_cmd12_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS_CMD);
  writeIO_DMC(ral_addr_of_seq_init_addr12_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS17_ADDR); //MRS17
  
  writeIO_DMC(ral_addr_of_seq_init_cmd13_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS_CMD);
  writeIO_DMC(ral_addr_of_seq_init_addr13_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS20_ADDR); //MRS20
  
  writeIO_DMC(ral_addr_of_seq_init_cmd14_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS_CMD);
  writeIO_DMC(ral_addr_of_seq_init_addr14_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_MRS22_ADDR); //MRS22
  
  //ZQ
  writeIO_DMC(ral_addr_of_seq_init_cmd15_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_ZQ_START_CMD);
  writeIO_DMC(ral_addr_of_seq_init_addr15_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_ZQ_START_ADDR); //ZQ Start
  
  writeIO_DMC(ral_addr_of_seq_init_cmd16_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_ZQ_LATCH_CMD);
  writeIO_DMC(ral_addr_of_seq_init_addr16_in_ddrmc_main_ddrmc_main_bank(BLK), LPDDR4_ZQ_LATCH_ADDR); //ZQ Latch
  
  
  // writeIO_DMC(SEQ_INIT_LOOP, 17);
  writeIO_DMC(ral_addr_of_seq_init_start_in_ddrmc_main_ddrmc_main_bank(BLK), 0x01);
   
  while ((readIO_DMC(ral_addr_of_seq_init_start_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x0));
   
  return;
}
