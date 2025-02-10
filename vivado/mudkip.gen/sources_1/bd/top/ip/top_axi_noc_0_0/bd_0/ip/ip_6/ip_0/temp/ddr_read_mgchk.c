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
//       Revision:       $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/ddr_rd_cal.c#7 $
//                       $Author: rgaddam $
//                       $DateTime: 2019/04/25 11:32:25 $
//                       $Change: 21352885 $
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
#include "function_dec.c"
#include "global_variable_define.h"


/**************** End Used defines **************************/



u8 ddr_read_mgchk(u8 pattern_type) {

   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_read_mgchk(): READ MARGIN START \n");
   DEBUG_PRINTF("##################################################################################################################### \n");

   u8  step_size_coarse       = FINE_CRS_STEP_SIZE;  
   u8  nos_of_samples         = glbl_margin_check? glbl_margin_pat_len:SAMPLE_CNT;
   u8  nibble;
   u8  bit_cnt;
   u8  rank;
   u32 error_mask;
   u8  status=0;
   int i;
   u32 delay;
   static u16 idelay_ref[DQ_NIBBLES];
   //u8 bits;
   //u16 iteration;
   u16 exptd_pt;

   u32 err_nibbles = 0;
   u32 err_nibbles_pqtr = 0;
   u32 err_nibbles_nqtr = 0;
   u32 total_nibbles = 0;
   
   u16 p_r_edge,p_l_edge,n_r_edge,n_l_edge;  
 
   if (pattern_type == SIMPLE) {
   #ifdef LPDDR4
      //Set MPC read pattern and compare pattern
      exptd_pt = 0x12 | (0x12<<5); // [4:0] - for bl8 [9:5] for bl16//0xC3A5
      //writeIO_DMC(ral_addr_of_seq_dq_pattern0_in_ddrmc_main_ddrmc_main_bank(BLK),0xF00FF0F0); // 0xC3A5);
      writeIO_DMC(ral_addr_of_seq_dq_pattern0_in_ddrmc_main_ddrmc_main_bank(BLK),0x0F0FF0F0); // 0xC3A5);
      //for(rank = 0; rank < RANKS; rank++) {
      //lp4_mode_reg_wr(glbl_margin_rank, 32, 0x9A); // Set mode register with the expected pattern for first half of the bl16
      lp4_mode_reg_wr(glbl_margin_rank, 32, 0x5A); // Set mode register with the expected pattern for first half of the bl16
      lp4_mode_reg_wr(glbl_margin_rank, 40, 0x5A); // Set the mode register with the expected pattern for second half of the bl16
      //lp4_mode_reg_wr(glbl_margin_rank, 40, 0x9A); // Set the mode register with the expected pattern for second half of the bl16
      lp4_mode_reg_wr(glbl_margin_rank, 15, 0x00); // Disable inversion of the data bits pattern of dq[7:0]
      lp4_mode_reg_wr(glbl_margin_rank, 20, 0x00); // Disable inversion of the data bits pattern of dq[15:8] 
      //}

      // Set CAL RTL to consider the commands as read commands.
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0x105);
      //Set MPC Read DQ CAL command
      writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), LP4_MPC_RD_DQ);
   #else

     set_1_seq_command_at_mem(glbl_margin_rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/,MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr3 | MEM_ENABLE_MPR /*u32 seq_a_addr*/, 0x0 /*u8 seq_a_bg*/, 0x3 /*u16 seq_a_ba*/, 0 /*u32 seq_a2a_dly*/);
      
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),MEM_COL_WRITE);
      writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK),0x5A);
      writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK),0);
      writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK),glbl_margin_rank); // Selects xphy rank
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),0x10<<glbl_margin_rank); // Selects DDR Mem rank
      writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK),20);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK))) {}
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),MEM_COL_READ);
      writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK),0);
      writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK),0);
      writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      writeIO_DMC(ral_addr_of_seq_dq_pattern2_in_ddrmc_main_ddrmc_main_bank(BLK),0x0f0ff0f0);
      exptd_pt = 0x14; //0xAAAA
      
   #endif
   } else {
      glbl_cplx_loop_cnt = CPLX_LOOP_CNT_MIN;               // [14:7] Loop count to go through the pattern
      glbl_cplx_row_cnt = CPLX_ROW_CNT_MIN;                // [17:15] Maximum row count value. For fast cal simulation, set it to 0.
      glbl_cplx_burst_array_index = CPLX_BURST_ARRAY_INDEX_MIN;    // [24:20] The max index value when incrementing the burst array sequence. For fast cal simulation, set it to 3.
      // For the complex calibration intialize only PQTR and NQTR taps to 0.
   } 
   writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK),4);
   writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK),exptd_pt); 
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK),nos_of_samples | (nos_of_samples<<16));



   glbl_first_nibble = glbl_dq_first_nibble;
   glbl_last_nibble  = glbl_dq_last_nibble;
   glbl_nibble_mask_initial = glbl_dq_nibble_mask_initial;                 //1 correspond to masked nibble and 0 correspond to unmasked nibble
   glbl_nibble_mask = glbl_nibble_mask_initial;
   writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask_initial); 

   // Clear gate and reset all bit slices
   riu_reg_bcast(BS_RESET_CTRL, 0, 0x1<<CLR_GATE_POS);
   bitslice_reset_bcast(0, BS_RESET_WT); // BS Reset

   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_rd_cal(): Read Calibration calibration has started for Ranks = %d, BITS PER RANK = %d \n", RANKS, DQ_BITS);
   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_rd_cal(): First nibble = %d, last Nibble = %d, and Nibbles Mask = 0x%8X \n", glbl_first_nibble, glbl_last_nibble, glbl_nibble_mask);

   static u16 pqtr_old_center_val[DQ_NIBBLES];
   static u16 nqtr_old_center_val[DQ_NIBBLES];
   for(nibble=0; nibble<DQ_NIBBLES;nibble++) {
         pqtr_old_center_val[nibble] = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble],PQTR);
         DEBUG_PRINTF("INFO: rd_cal(): pqtr_old_center_val[nibble][%d] = %d \n", nibble, pqtr_old_center_val[nibble]);
         nqtr_old_center_val[nibble] = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble],NQTR);
         DEBUG_PRINTF("INFO: rd_cal(): nqtr_old_center_val[nibble][%d] = %d \n", nibble, nqtr_old_center_val[nibble]);
   }

   // Initialize all the pqtrs/nqtrs/idelays to 0
   glbl_bit_mask_31_0  = 0;
   glbl_bit_mask_63_32 = 0;
   glbl_bit_mask_95_64 = 0;
   // Work around for EDT-994592 issue. Issue PRBS write mode sequence before PRBS read calibration.
   writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x1);
   writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
   //---------------------------------------------------------------------------------------------------------------
   DEBUG_PRINTF("INFO: Performing sanity test before starting the PRBS test\n");
   glbl_cal_first_iteration = 1;
   glbl_clear_error = 1;
   // Positive Test - Read Nibble wise comparision status correspond to both edge for pattern 1-0-1-0-1-0-1-0 
   read_compared_status (NIBBLES /*compare_mode*/, DUAL /*compare_edge*/, 1, pattern_type); 
   DEBUG_PRINTF("INFO: Received bits error in first burst for logical nibbles = 0x%08X for positive test \n", glbl_bits_error_status_31_0);

   // After the reset bit slice there is wait time required therefore sending extra traffic burst.
   glbl_cal_first_iteration = 1;
   glbl_clear_error = 1;
   // Positive Test - Read Nibble wise comparision status correspond to both edge for pattern 1-0-1-0-1-0-1-0 
   read_compared_status (NIBBLES /*compare_mode*/, DUAL /*compare_edge*/, 1, pattern_type); 
   err_nibbles = glbl_bits_error_status_31_0;
   DEBUG_PRINTF("INFO: Received bits error in second burst for logical nibbles = 0x%08X for positive test \n", glbl_bits_error_status_31_0);
   for(nibble=0, glbl_bit_pos=1 ,total_nibbles =1; nibble < DQ_NIBBLES; nibble++, glbl_bit_pos=glbl_bit_pos<<1,total_nibbles = total_nibbles | (total_nibbles << 1)) {
      if((glbl_bits_error_status_31_0&glbl_bit_pos)) {
         DEBUG_PRINTF("ERROR: Sanity failed for Nibble for nibble = %d \n", nibble);
         //write_error_XRAM(ERR_CODE_65_RD_PRBS_FAIL_PRE_SANITY_CHECK);
      }
   }
   
   //exiting when all nibble fail   
   if (glbl_bits_error_status_31_0 == total_nibbles ) {
        
        write_error_XRAM(ERR_CODE_65_RD_PRBS_FAIL_PRE_SANITY_CHECK);
	return 1;}
  // set wbs set to 1
   glbl_en_qtr_dly_upd_wbs = 1; //added by aswin make it as 1 during margin
   status = update_delay_sequential(P_N_DELAY /*int delay_type*/, LOAD /*int inc_dec_load*/);
   //after this bs reset 
   if(status) {
      //return 1;
   }
   // Initialize the pqtr/nqtr/idelay/glbl_stable_valid_win counter to 0
   for(nibble = 0; nibble < DQ_NIBBLES; nibble++) {
      for(bit_cnt = 0; bit_cnt < 4; bit_cnt++) {
         glbl_stable_valid_win[bit_cnt + nibble*4] = 0;
         glbl_stable_valid_win_nqtr[bit_cnt + nibble*4] = 0;
      }
   }
   for(bit_cnt=0; bit_cnt < DQ_BITS; bit_cnt++) {
      glbl_bits_riu_map[bit_cnt][0] = glbl_dq_bits_riu_map[bit_cnt][0];
      glbl_bits_riu_map[bit_cnt][1] = glbl_dq_bits_riu_map[bit_cnt][1];
      glbl_lbit2pnibble_map[bit_cnt] = glbl_dq_lbit2pnibble_map[bit_cnt];
   }
   /**********************************************************************************************************/   
   for(bit_cnt=0; bit_cnt < DQ_NIBBLES; bit_cnt++) {
      glbl_lbit2pnibble_map[bit_cnt] = glbl_dq_lnibble2pnibble_map[bit_cnt];
   }


   /**********************************************************************************************************/   
   // Start 1st phase of the read data calibration (Move PDQS and NDQS to find Valid window correspond their associated bits)
   /**********************************************************************************************************/   
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: rd_cal(): Start 1st phase of the read data calibration (Move PDQS and NDQS to find Valid window correspond their associated bits) for rank = %d with nibble mask = 0x%08X\n", RANKS, glbl_nibble_mask);
   DEBUG_PRINTF("##################################################################################################################### \n");
   for(nibble=0; nibble < DQ_NIBBLES; nibble++) {
      glbl_stable_valid_win[nibble] = 0;
      glbl_stable_valid_win_nqtr[nibble] = 0;
   }
   DEBUG_PRINTF("######################################################################################################################################## \n");
   DEBUG_PRINTF("################################ rd_cal(): Fifth phase a: use Fine taps to go deepr into valid region #################################### \n");
   glbl_min_vld_cnt = MIN_VLD_CNT;
   glbl_bit_mask_31_0  = 0;
   glbl_bit_mask_31_0_last  = 0;
   glbl_bit_mask_nqtr_17_0 = 0;
   glbl_bit_mask_nqtr_17_0_last = 0;
   glbl_bits_error_status_31_0  = 0;
   glbl_bits_error_status_nqtr_17_0 = 0;
   glbl_clear_error = 1;

   glbl_phase_no = 0x5A;
   glbl_region_type = PARTIAL;
   glbl_delay_type  = P_DELAY;
   glbl_inc_dec_load = INC;
   glbl_step_size = 1;
   //glbl_en_qtr_dly_upd_wbs = 0;
   glbl_compare_edge_type = RISE;
   glbl_compare_data_type = NIBBLES;
   glbl_compare_data_bits = DQ_NIBBLES;
   glbl_dual_edge_region = 1;
   glbl_pattern_type = pattern_type;
   status = region_detect();   
   //err_nibbles = err_nibbles | glbl_bits_error_status_31_0; 
   //glbl_en_qtr_dly_upd_wbs = 1;

   static u16 pqtr_init_val[DQ_NIBBLES];
   static u16 nqtr_init_val[DQ_NIBBLES];
   for(nibble=0; nibble<DQ_NIBBLES;nibble++) {
      pqtr_init_val[nibble] = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble],PQTR);
      DEBUG_PRINTF("INFO: ddr_rd_cal(): pqtr_init_val[nibble][%d] = %d \n", nibble, pqtr_init_val[nibble]);
// XRAM_DBG: rddq_pqtr_left: -glbl_min_vld_cnt
// XRAM_DBG: rdcmplx_pqtr_left_short: -glbl_min_vld_cnt
      delay = pqtr_init_val[nibble] - glbl_min_vld_cnt;

      nqtr_init_val[nibble] = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble],NQTR);
      DEBUG_PRINTF("INFO: ddr_rd_cal(): nqtr_init_val[nibble][%d] = %d \n", nibble, nqtr_init_val[nibble]);
// XRAM_DBG: rddq_nqtr_left: -glbl_min_vld_cnt
// XRAM_DBG: rdcmplx_nqtr_left_short: -glbl_min_vld_cnt
      delay = nqtr_init_val[nibble] - glbl_min_vld_cnt;
   }

   if(status) {
      write_error_XRAM(ERR_CODE_66_RD_PRBS_FAIL_LEFT_EDGE_PQTR_NQTR);
   }


   DEBUG_PRINTF("######################################################################################################################################## \n");
   DEBUG_PRINTF("################################ rd_cal(): Fifth phase b: use coarse (multiple fine) taps to get right edge of the valid window quickly## \n");
   // Decrease the burst length to fast scan the valid region
   glbl_cplx_loop_cnt = CPLX_LOOP_CNT_MIN;               // [14:7] Loop count to go through the pattern
   glbl_cplx_row_cnt = CPLX_ROW_CNT_MIN;                // [17:15] Maximum row count value. For fast cal simulation, set it to 0.
   glbl_cplx_burst_array_index = CPLX_BURST_ARRAY_INDEX_MIN;    // [24:20] The max index value when incrementing the burst array sequence. For fast cal simulation, set it to 3.

   glbl_bit_mask_31_0  = 0;
   glbl_bit_mask_31_0_last  = 0;
   glbl_bit_mask_nqtr_17_0 = 0;
   glbl_bit_mask_nqtr_17_0_last = 0;
   glbl_bits_error_status_31_0  = 0;
   glbl_bits_error_status_nqtr_17_0 = 0;
   glbl_clear_error = 1;// Fix Me: It should be 0 but in case of LDDR4 once it samples x it remains x until it gets cleared. And appers as no error.
   writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x1);
   writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);

   glbl_phase_no = 0x5B;
   glbl_region_type = FULL;
   glbl_delay_type  = P_DELAY;
   glbl_inc_dec_load = INC;
   glbl_step_size = step_size_coarse;
   glbl_compare_edge_type = RISE;
   glbl_compare_data_type = NIBBLES;
   glbl_compare_data_bits = DQ_NIBBLES;
   glbl_dual_edge_region = 1;
   glbl_pattern_type = pattern_type;
   status = region_detect(); 
   for(nibble=0; nibble<DQ_NIBBLES; nibble++) {
      delay = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble], PQTR);

      delay = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble], NQTR);
   }

   DEBUG_PRINTF("######################################################################################################################################## \n");
   DEBUG_PRINTF("################################ rd_cal(): Fifth phase d: use fine taps to get the right edge of the valid window ###################### \n");
   glbl_min_vld_cnt = MIN_VLD_CNT;
   for(nibble=0; nibble < DQ_NIBBLES; nibble++) {
      glbl_stable_valid_win[nibble] = 0;
      glbl_stable_valid_win_nqtr[nibble] = 0;
   }
   glbl_bit_mask_31_0  = 0;
   glbl_bit_mask_31_0_last  = 0;
   glbl_bit_mask_nqtr_17_0 = 0;
   glbl_bit_mask_nqtr_17_0_last = 0;
   glbl_bits_error_status_31_0  = 0;
   glbl_bits_error_status_nqtr_17_0 = 0;
   glbl_clear_error = 1;

   glbl_phase_no = 0x5D;
   glbl_region_type = PARTIAL;
   glbl_delay_type  = P_DELAY;
   glbl_inc_dec_load = DEC;
   glbl_step_size = 1;
   //glbl_en_qtr_dly_upd_wbs = 0;
   glbl_compare_edge_type = RISE;
   glbl_compare_data_type = NIBBLES;
   glbl_compare_data_bits = DQ_NIBBLES;
   glbl_dual_edge_region = 1;
   glbl_pattern_type = pattern_type;
   status = region_detect(); 
   //glbl_en_qtr_dly_upd_wbs = 1;
   //err_nibbles = err_nibbles | glbl_bits_error_status_31_0; 
   for(nibble=0; nibble<DQ_NIBBLES; nibble++) {
      delay = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble], PQTR);
      delay += glbl_min_vld_cnt;

      delay = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble], NQTR);
      delay += glbl_min_vld_cnt;
   }

   if(status) {
         write_error_XRAM(ERR_CODE_67_RD_PRBS_FAIL_RIGHT_EDGE_PQTR_NQTR);
   }

   DEBUG_PRINTF("INFO: 5th phase of the rd_cal() completed successfully \n");

   /**********************************************************************************************************/   
   // Start 6th phase of the read data calibration (Move PDQS and NDQS to center of the Valid window correspond their associated bits)
   /**********************************************************************************************************/   
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: rd_cal(): Start 6th phase of the read data calibration (Move PDQS and NDQS to center of the Valid window correspond their associated bits) for rank = %d with nibble mask = 0x%08X\n", RANKS, glbl_nibble_mask);
   DEBUG_PRINTF("##################################################################################################################### \n");
   u8 busy =  readIO_DMC(ral_addr_of_cplx_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & 0x1;
   while(busy == 1) {
      busy =  readIO_DMC(ral_addr_of_cplx_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & 0x1;
   }

   static u16 pqtr_end_val[DQ_NIBBLES];
   static u16 nqtr_end_val[DQ_NIBBLES];
   static u16 pqtr_left_margin[DQ_NIBBLES];
   static u16 nqtr_left_margin[DQ_NIBBLES];
   static u16 pqtr_right_margin[DQ_NIBBLES];
   static u16 nqtr_right_margin[DQ_NIBBLES];
   for(nibble=0; nibble<DQ_NIBBLES;nibble++) {
         pqtr_end_val[nibble] = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble],PQTR);
         DEBUG_PRINTF("INFO: ddr_rd_cal(): pqtr_end_val[nibble][%d] = %d \n", nibble, pqtr_end_val[nibble]);
         nqtr_end_val[nibble] = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble],NQTR);
         DEBUG_PRINTF("INFO: ddr_rd_cal(): nqtr_end_val[nibble][%d] = %d \n", nibble, nqtr_end_val[nibble]);
   }

   glbl_min_vld_cnt = (pattern_type == COMPLEX) ? MIN_VLD_CNT_CPLX : MIN_VLD_CNT;

   for(nibble=0; nibble < DQ_NIBBLES; nibble++) {
      glbl_stable_valid_win[nibble]=(pqtr_end_val[nibble]-pqtr_init_val[nibble])+2*glbl_min_vld_cnt;
// XRAM_DBG: rddq_pqtr_final
// XRAM_DBG: rdcmplx_pqtr_final
      delay = pqtr_old_center_val[nibble]; //TEST
      //delay = (pqtr_init_val[nibble] + pqtr_end_val[nibble]) / 2;
      //delay = pqtr_init_val[nibble] + ((pqtr_end_val[nibble] - pqtr_init_val[nibble]) / 2);
      pqtr_left_margin[nibble]  = delay - pqtr_init_val[nibble] + glbl_min_vld_cnt; 
      pqtr_right_margin[nibble] = pqtr_end_val[nibble] - delay  + glbl_min_vld_cnt; 
      p_l_edge = pqtr_init_val[nibble]; 
      p_r_edge = pqtr_end_val[nibble]; 
      
     if (p_l_edge > (delay + glbl_min_vld_cnt)) {
       err_nibbles_pqtr = err_nibbles_pqtr | (1 << nibble);
      }
      if ((p_r_edge + glbl_min_vld_cnt)  < delay ) {
       err_nibbles_pqtr = err_nibbles_pqtr |  (1 << nibble);
      }
      write_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble] /*u8 nibble*/, PQTR /*u8 reg_addr*/, delay);
      //write_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble] /*u8 nibble*/, PQTR /*u8 reg_addr*/, pqtr_old_center_val[nibble]);
      DEBUG_PRINTF("INFO: rd_cal(): loading PQTR of Logical Nibble %d or Physical Nibble %d with %d taps , left margin = %d, right margin = %d \n", nibble, glbl_dq_lnibble2pnibble_map[nibble], delay, pqtr_left_margin[nibble], pqtr_right_margin[nibble]) 
      //writeIO_XRAM(XRAM_RDMARGIN_PQTR_LEFT + nibble*4, pqtr_left_margin[nibble]);
      //writeIO_XRAM(XRAM_RDMARGIN_PQTR_RIGHT + nibble*4, pqtr_right_margin[nibble]);

      glbl_stable_valid_win_nqtr[nibble]=(nqtr_end_val[nibble]-nqtr_init_val[nibble])+2*glbl_min_vld_cnt;
// XRAM_DBG: rddq_nqtr_final
// XRAM_DBG: rdcmplx_nqtr_final
      delay = nqtr_old_center_val[nibble]; //TEST
      //delay = (nqtr_end_val[nibble] + nqtr_init_val[nibble])/2;
      //delay = nqtr_init_val[nibble] + (glbl_stable_valid_win_nqtr[nibble]/2);
      //delay = nqtr_init_val[nibble] + ((nqtr_end_val[nibble] - nqtr_init_val[nibble]) / 2);
      //delay = (glbl_stable_valid_win_nqtr[nibble]/2);
      nqtr_left_margin[nibble]  = delay - nqtr_init_val[nibble] + glbl_min_vld_cnt; 
      nqtr_right_margin[nibble] = nqtr_end_val[nibble] - delay  + glbl_min_vld_cnt;
      n_l_edge = nqtr_init_val[nibble]; 
      n_r_edge = nqtr_end_val[nibble]; 
      if (n_l_edge > (delay  + glbl_min_vld_cnt) ) {
       err_nibbles_nqtr = err_nibbles_nqtr | ( 1 << nibble);
      }
      if ((n_r_edge + glbl_min_vld_cnt) < delay ) {
       err_nibbles_nqtr = err_nibbles_nqtr | ( 1 << nibble);
      }
      write_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble] /*u8 nibble*/, NQTR /*u8 reg_addr*/, delay);
      //write_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble] /*u8 nibble*/, NQTR /*u8 reg_addr*/, nqtr_old_center_val[nibble]);
      DEBUG_PRINTF("INFO: rd_cal(): loading NQTR of Logical Nibble %d or Physical Nibble %d with %d taps , left margin = %d, right margin = %d \n", nibble, glbl_dq_lnibble2pnibble_map[nibble], delay, nqtr_left_margin[nibble], nqtr_right_margin[nibble]) 

      if (err_nibbles & (1<<nibble)) {
	nqtr_left_margin[nibble] = 0;
	nqtr_right_margin[nibble] = 0;
	pqtr_left_margin[nibble] = 0;
	pqtr_right_margin[nibble] = 0;
      }
      if (err_nibbles_pqtr & (1<<nibble)) {
	pqtr_left_margin[nibble] = 0;
	pqtr_right_margin[nibble] = 0;
      }
      if (err_nibbles_nqtr & (1<<nibble)) {
	nqtr_left_margin[nibble] = 0;
	nqtr_right_margin[nibble] = 0;
      }
      writeIO_XRAM(XRAM_RDMARGIN_NQTR_LEFT + nibble*4, nqtr_left_margin[nibble]);
      writeIO_XRAM(XRAM_RDMARGIN_NQTR_RIGHT + nibble*4, nqtr_right_margin[nibble]);
      writeIO_XRAM(XRAM_RDMARGIN_PQTR_LEFT + nibble*4, pqtr_left_margin[nibble]);
      writeIO_XRAM(XRAM_RDMARGIN_PQTR_RIGHT + nibble*4, pqtr_right_margin[nibble]);
   }

   //Reset Bit slice
   writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask_initial); 
   bitslice_reset_bcast(0, BS_RESET_WT);

   DEBUG_PRINTF("INFO: 6th phase of the rd_cal() completed successfully \n");
   /**********************************************************************************************************/   
   // Ends 6th phase of the read data calibration (Moved PDQS and NDQS to center of the Valid window correspond their associated bits)
   /**********************************************************************************************************/   


   /**********************************************************************************************************/   
   // Start 7th phase of the read data calibration (Check Sanity of the calibration)
   /**********************************************************************************************************/   
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: rd_cal(): Start 7th phase of the read data calibration (Check Sanity of the calibration) for rank = %d, and nibble mask = 0x%08X \n", RANKS, glbl_nibble_mask);
   DEBUG_PRINTF("##################################################################################################################### \n");

   DEBUG_PRINTF("############################################################################################ \n");
   DEBUG_PRINTF("################################ rd_cal(): Seventh phase ##################################### \n");
   if(pattern_type == SIMPLE) {

      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask_initial); 

      // Negative Test - Read Nibble wise comparision status correspond to both edge for ith pattern
      writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK),4); 
      read_compared_status (NIBBLES /*compare_mode*/, DUAL /*compare_edge*/,  1, pattern_type); 
      DEBUG_PRINTF("INFO: Received bits error for logical nibbles = 0x%08X for negative test \n", glbl_bits_error_status_31_0);

      for(nibble=0,glbl_bit_pos=1; nibble < DQ_NIBBLES; nibble++,glbl_bit_pos=glbl_bit_pos<<1) {
         if(!((glbl_bits_error_status_31_0 & glbl_bit_pos))) {
            DEBUG_PRINTF("ERROR: Sanity failed for Nibble for nibble = %d \n", nibble);
            write_error_XRAM(ERR_CODE_19_RD_DQ_FAIL_NEGATIVE_SANITY_CHECK);
            //return 1;
         } 
      }

      glbl_clear_error = 1;
      writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK),exptd_pt);
      // Positive Test - Read Nibble wise comparision status correspond to both edge for pattern 1-0-1-0-1-0-1-0 
      read_compared_status (NIBBLES /*compare_mode*/, DUAL /*compare_edge*/, 1, pattern_type); 
      DEBUG_PRINTF("INFO: Received bits error for logical nibbles = 0x%08X for positive test \n", glbl_bits_error_status_31_0);

      for(nibble=0,glbl_bit_pos=1; nibble < DQ_NIBBLES; nibble++,glbl_bit_pos=glbl_bit_pos<<1) {
         if((glbl_bits_error_status_31_0&glbl_bit_pos)) {
            DEBUG_PRINTF("ERROR: Sanity failed for Nibble for nibble = %d \n", nibble);
            write_error_XRAM(ERR_CODE_20_RD_DQ_FAIL_POSITIVE_SANITY_CHECK);
            //return 1;
         }
      }
   } else {  
      glbl_cal_first_iteration = 1;
      glbl_clear_error = 1;
      // Positive Test - Read Nibble wise comparision status correspond to both edge for pattern 1-0-1-0-1-0-1-0 
      read_compared_status (NIBBLES /*compare_mode*/, DUAL /*compare_edge*/, 1, pattern_type); 
      DEBUG_PRINTF("INFO: Received bits error in first burst for logical nibbles = 0x%08X for positive test \n", glbl_bits_error_status_31_0);

      // After the reset bit slice there is wait time required therefore sending extra traffic burst.
      glbl_cal_first_iteration = 1;
      glbl_clear_error = 1;
      // Positive Test - Read Nibble wise comparision status correspond to both edge for pattern 1-0-1-0-1-0-1-0 
      read_compared_status (NIBBLES /*compare_mode*/, DUAL /*compare_edge*/, 1, pattern_type); 
      DEBUG_PRINTF("INFO: Received bits error in second burst for logical nibbles = 0x%08X for positive test \n", glbl_bits_error_status_31_0);

      for(nibble=0, glbl_bit_pos=1; nibble < DQ_NIBBLES; nibble++, glbl_bit_pos=glbl_bit_pos<<1) {
         if((glbl_bits_error_status_31_0&glbl_bit_pos)) {
            DEBUG_PRINTF("ERROR: Sanity failed for Nibble for nibble = %d \n", nibble);
            if(pattern_type == COMPLEX) {
               write_error_XRAM(ERR_CODE_54_RD_COMPLEX_FAIL_POSITIVE_POST_SANITY_CHECK);
            } else if(pattern_type == PRBS) {
               write_error_XRAM(ERR_CODE_68_RD_PRBS_FAIL_POST_SANITY_CHECK);
            }
            //return 1;
         }
      }
   }
   DEBUG_PRINTF("INFO: 7th phase of the rd_cal() completed successfully \n");
   /**********************************************************************************************************/   
   // Ends 7th phase of the read data calibration (Sanity of the calibration Passed)
   /**********************************************************************************************************/   
   /**********************************************************************************************************/   
   // Program default center values
   /**********************************************************************************************************/   
   for(nibble=0; nibble < DQ_NIBBLES; nibble++) {

      write_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble] /*u8 nibble*/, PQTR /*u8 reg_addr*/, pqtr_old_center_val[nibble]);
      write_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble] /*u8 nibble*/, NQTR /*u8 reg_addr*/, nqtr_old_center_val[nibble]);

   }
   writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask_initial); 
   bitslice_reset_bcast(0, BS_RESET_WT);
   /**********************************************************************************************************/   
   // End of programming default center values
   /**********************************************************************************************************/   
   //Turn off the MPR Mode to put the memory in normal mode
   #ifdef DDR4
      for(rank = 0; rank < RANKS; rank += glbl_rank_inc) {
         set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                               MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr3 & MEM_DISABLE_MPR /*u32 seq_a_addr*/, 0x0 /*u8 seq_a_bg*/, 0x3 /*u16 seq_a_ba*/,
                               0 /*u32 seq_a2a_dly*/);
      }
   #else
      //DEBUG_PRINTF("INFO: ddr_rd_cal(): LPDDR4: Releasing the memory from the Read preamble training mode \n");
      //for(rank = 0; rank < RANKS; rank++) {
      //   lp4_mode_reg_wr(rank, 13, glbl_lp4_mr13&0xFD);
      //}

   #endif
   #ifdef ENABLE_MICROBLAZE_BFM
      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: rd_cal(): DUMP the calibration values \n");
      DEBUG_PRINTF("##################################################################################################################### \n");
      static u16 idelay[DQ_BITS];
      RECORD_DLY("// Written from RD_CAL function \n");
      if (pattern_type == SIMPLE) {
         RECORD_DLY("#define IDELAY_VAL { ");
      } else { 
         RECORD_DLY("#define CPLX_IDELAY_VAL { ");
      }
      for(bit_cnt = 0; bit_cnt < DQ_BITS; bit_cnt++) {
         idelay[bit_cnt] = (readIO_RIU(glbl_dq_idly_riu_addr_map[bit_cnt])) & 0x1FF;
         if(bit_cnt < (DQ_BITS-1)) {
            RECORD_DLY(" 0x%04X,", idelay[bit_cnt]);
         } else {
            RECORD_DLY(" 0x%04X } \n", idelay[bit_cnt]);
         }
      }   
      u8 lnibble;
      static u16 qtr_delay[DQ_BITS/4];
      if (pattern_type == SIMPLE) {
         RECORD_DLY("#define PQTR_VAL { ");
      } else {
         RECORD_DLY("#define CPLX_PQTR_VAL { ");
      }
      for(lnibble = 0; lnibble < (DQ_BITS/4); lnibble++) {
         qtr_delay[lnibble] = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[lnibble], PQTR) & 0x1FF;
         if(lnibble < ((DQ_BITS/4)-1)) {
            RECORD_DLY(" 0x%04X,", qtr_delay[lnibble]);
         } else {
            RECORD_DLY(" 0x%04X } \n", qtr_delay[lnibble]);
         }
      }   

      if (pattern_type == SIMPLE) {
         RECORD_DLY("#define NQTR_VAL { ");
      } else {
         RECORD_DLY("#define CPLX_NQTR_VAL { ");
      }
      for(lnibble = 0; lnibble < (DQ_BITS/4); lnibble++) {
         qtr_delay[lnibble] = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[lnibble], NQTR) & 0x1FF;
         if(lnibble < ((DQ_BITS/4)-1)) {
            RECORD_DLY(" 0x%04X,", qtr_delay[lnibble]);
         } else {
            RECORD_DLY(" 0x%04X } \n", qtr_delay[lnibble]);
         }
      }   

      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: rd_cal(): READ DATA CALIBRATION END \n");
      DEBUG_PRINTF("##################################################################################################################### \n");
   #endif                            
   
   return 0;
}


/*********************************************************************************************************************************************/
// Function definations
/*********************************************************************************************************************************************/










