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



u8 ddr_rd_cal(u8 pattern_type) {

#ifdef SKIP_RD_CAL_STAGE
   if (pattern_type == SIMPLE) {
      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: rd_cal():  READ DATA CALIBRATION SKIPPED \n");
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: rd_cal(): Loading precalculated delay values to skip the READ DATA CALIBRATION \n");
      DEBUG_PRINTF("##################################################################################################################### \n");
      skip_ddr_rd_cal(SIMPLE);
      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: rd_cal(): Loaded precalculated delay values to skip the READ DATA CALIBRATION \n");
      DEBUG_PRINTF("##################################################################################################################### \n");
      return 0;
   }
#endif
#ifdef SKIP_RD_COMPLEX_CAL_STAGE
   if (pattern_type == COMPLEX) {
      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: rd_cal():  READ DATA COMPLEX CALIBRATION SKIPPED \n");
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: rd_cal(): Loading precalculated delay values to skip the READ DATA COMPLEX CALIBRATION \n");
      DEBUG_PRINTF("##################################################################################################################### \n");
      skip_ddr_rd_cal(COMPLEX);
      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: rd_cal(): Loaded precalculated delay values to skip the READ DATA COMPLEX CALIBRATION \n");
      DEBUG_PRINTF("##################################################################################################################### \n");
      return 0;
   }
#endif
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_rd_cal(): READ DATA CALIBRATION START \n");
   DEBUG_PRINTF("##################################################################################################################### \n");

   u8  step_size_coarse       = FINE_CRS_STEP_SIZE;  
   u8  nos_of_samples         = SAMPLE_CNT;

   u8  nibble;
   u8  bit_cnt;
   u8  rank;

   u8  status=0;
   int i;
   u32 delay;
   static u16 idelay_ref[DQ_NIBBLES];
   //u8 bits;
   //u16 iteration;
   u16 exptd_pt;

   /*************************************************************************************************/
   //   Unmask all the nibbles which are required for the read dq calibration
   /*************************************************************************************************/

   if (pattern_type == SIMPLE) {
      if(glbl_tck >= TCK_TH_RD_CAL) {
         DEBUG_PRINTF("INFO: ddr_rd_cal(): SKIPPING read DQ centering simple stage for TCK = %d\n", glbl_tck);
         return 0;
      }
   } else if(pattern_type == COMPLEX) {
      if(glbl_tck >= TCK_TH_RD_CMPLX) {
         DEBUG_PRINTF("INFO: ddr_rd_cal(): SKIPPING read centering complex stage for TCK = %d\n", glbl_tck);
         return 0;
      }
   } else if(pattern_type == PRBS) {
      if(glbl_tck >= TCK_TH_RD_PRBS) {
         DEBUG_PRINTF("INFO: ddr_rd_cal(): SKIPPING read PRBS stage for TCK = %d\n", glbl_tck);
         return 0;
      }
   }

   if (pattern_type == SIMPLE) {
   #ifdef LPDDR4
      //Set MPC read pattern and compare pattern
      exptd_pt = 0x12 | (0x12<<5); // [4:0] - for bl8 [9:5] for bl16//0xC3A5
      writeIO_DMC(ral_addr_of_seq_dq_pattern0_in_ddrmc_main_ddrmc_main_bank(BLK),0xF00FF0F0); // 0xC3A5);
      for(rank = 0; rank < RANKS; rank++) {
         lp4_mode_reg_wr(rank, 32, 0x9A); // Set mode register with the expected pattern for first half of the bl16
         lp4_mode_reg_wr(rank, 40, 0x9A); // Set the mode register with the expected pattern for second half of the bl16
         lp4_mode_reg_wr(rank, 15, 0x00); // Disable inversion of the data bits pattern of dq[7:0]
         lp4_mode_reg_wr(rank, 20, 0x00); // Disable inversion of the data bits pattern of dq[15:8] 

         ////Put the memory preamble training mode
         //DEBUG_PRINTF("INFO: ddr_rd_cal(): LPDDR4: Putting the memory in Read preamble training mode \n");
         //lp4_mode_reg_wr(rank, 13, glbl_lp4_mr13|0x2);
      }

      // Set CAL RTL to consider the commands as read commands.
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0x105);
      //Set MPC Read DQ CAL command
      writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), LP4_MPC_RD_DQ);
   #else
      for(rank = 0; rank < RANKS; rank += glbl_rank_inc) {
         // Before starting the Read calibration put the memory in the MPR Mode to read the data pattern from MPR0
         set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                               MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr3 | MEM_ENABLE_MPR /*u32 seq_a_addr*/, 0x0 /*u8 seq_a_bg*/, 0x3 /*u16 seq_a_ba*/,
                               0 /*u32 seq_a2a_dly*/);
      }
      // Set MPR read command
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),MEM_COL_READ);
      writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK),0);
      writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK),0);
      writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK),0);
      exptd_pt = 0; //0xAAAA
      writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK),0);
   #endif
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
   if(pattern_type == PRBS) {
      for(nibble=0; nibble<DQ_NIBBLES;nibble++) {
            pqtr_old_center_val[nibble] = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble],PQTR);
            DEBUG_PRINTF("INFO: rd_cal(): pqtr_old_center_val[nibble][%d] = %d \n", nibble, pqtr_old_center_val[nibble]);
            nqtr_old_center_val[nibble] = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble],NQTR);
            DEBUG_PRINTF("INFO: rd_cal(): nqtr_old_center_val[nibble][%d] = %d \n", nibble, nqtr_old_center_val[nibble]);
      }
   }

   // Initialize all the pqtrs/nqtrs/idelays to 0
   glbl_bit_mask_31_0  = 0;
   glbl_bit_mask_63_32 = 0;
   glbl_bit_mask_95_64 = 0;
   if((pattern_type == COMPLEX) || (glbl_vref_cal == 1)) {
      glbl_cplx_loop_cnt = CPLX_LOOP_CNT_MIN;               // [14:7] Loop count to go through the pattern
      glbl_cplx_row_cnt = CPLX_ROW_CNT_MIN;                // [17:15] Maximum row count value. For fast cal simulation, set it to 0.
      glbl_cplx_burst_array_index = CPLX_BURST_ARRAY_INDEX_MIN;    // [24:20] The max index value when incrementing the burst array sequence. For fast cal simulation, set it to 3.
      // For the complex calibration intialize only PQTR and NQTR taps to 0.
      status = update_delay_sequential(P_N_DELAY /*int delay_type*/, LOAD /*int inc_dec_load*/); 
   } else if(pattern_type == PRBS) {
      // Work around for EDT-994592 issue. Issue PRBS write mode sequence before PRBS read calibration.
      writeIO_DMC(ral_addr_of_prbs_config_in_ddrmc_main_ddrmc_main_bank(BLK), (0x0<<0) | (0x0<<1) | (0x0<<2) | (0x8<<3) | (0x0<<7) | (0x1<<9) | (0x4<<13) | (0x0<<17)); // Write mode and Refresh disabled.
      writeIO_DMC(ral_addr_of_prbs_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 0x4 | (0x1<<16)); // Switch from write to read after 10 writes; And repeate the loop 1 times.
      writeIO_DMC(ral_addr_of_prbs_start_in_ddrmc_main_ddrmc_main_bank(BLK),0x1);
      writeIO_DMC(ral_addr_of_prbs_start_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
      while(readIO_DMC(ral_addr_of_prbs_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & 0x1);
      writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x1);
      writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);

      writeIO_DMC(ral_addr_of_prbs_config_in_ddrmc_main_ddrmc_main_bank(BLK), (0x1<<0) | (0x0<<1) | (0x1<<2) | (0x8<<3) | (0x0<<7) | (0x1<<9) | (0x4<<13) | (0x0<<17)); 
// [0] : 1: wr/rd/rd/rd . . . Sequence.Write once read forever"
// [1] : 0:  cal_prbs    "0: Normal operation
// [2] : 0:  cal_prbs    "0: Refreshes disabled
// [6:3] 10: selects how many bits are used for CAS addressing, max 10
// [8:7] 0:  cal_prbs    "0: PRBS23
// [12:9] 1: Enabled only Rank 0;  0x0  0  cal_prbs    A 1 in a given bit position means that given rank is enabled in the test pattern
// [16:13]0:  0x0  0  cal_prbs    A 1 in a given bit position means that given logical rank (for 3ds) is enabled in the test pattern
// [20:17]10:  0x0  0  cal_prbs    Delay in clock cycles between data bursts when switching ranks

      writeIO_DMC(ral_addr_of_prbs_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 0x4 | (0x2<<16)); // Switch from write to read after 10 writes; And repeate the loop 2 times.
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
      DEBUG_PRINTF("INFO: Received bits error in second burst for logical nibbles = 0x%08X for positive test \n", glbl_bits_error_status_31_0);

      for(nibble=0, glbl_bit_pos=1; nibble < DQ_NIBBLES; nibble++, glbl_bit_pos=glbl_bit_pos<<1) {
         if((glbl_bits_error_status_31_0&glbl_bit_pos)) {
            DEBUG_PRINTF("ERROR: Sanity failed for Nibble for nibble = %d \n", nibble);
            write_error_XRAM(ERR_CODE_65_RD_PRBS_FAIL_PRE_SANITY_CHECK);
            return 1;
         }
      }
      status = update_delay_sequential(P_N_DELAY /*int delay_type*/, LOAD /*int inc_dec_load*/); 
   } else {
      status = update_delay_sequential(P_N_DQ_DELAY /*int delay_type*/, LOAD /*int inc_dec_load*/); 
   }
   if(status) {
      return 1;
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
   // For the calibration with simple pattern perfrom data deskew (Phase 1,2, and 3). For the calibration with complex pattern these stages are skipped.  
   if((pattern_type == SIMPLE) && (glbl_vref_cal == 0)) {
      /**********************************************************************************************************/   
      // Start 1st phase of the read data calibration (Move PDQS and NDQS to valid region of their data bits 
      /**********************************************************************************************************/   
      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: rd_cal(): Start 1st phase of the read data calibration (Move PDQS and NDQS to valid region of their data bits rank = %d with nibble mask = 0x%08X\n", RANKS, glbl_nibble_mask);
      DEBUG_PRINTF("##################################################################################################################### \n");
         glbl_min_vld_cnt = MIN_VLD_CNT;
         glbl_nibble_mask = 0xFFFFFFFF;
	 glbl_nibble_mask_nqtr = 0xFFFFFFFF;
	 glbl_bit_mask_31_0  = 0;
         glbl_bit_mask_63_32 = 0;
         glbl_bit_mask_95_64 = 0;
         glbl_bit_mask_31_0_last  = 0;
         glbl_bit_mask_63_32_last = 0;
         glbl_bit_mask_95_64_last = 0;
         glbl_bits_error_status_31_0  = 0;
         glbl_bits_error_status_63_32 = 0;
         glbl_bits_error_status_95_64 = 0;
	 glbl_clear_error = 1;
         //glbl_check_sequential = 0;
         glbl_phase_no = 0x1;
         glbl_region_type = PARTIAL;
         glbl_delay_type  = P_N_DELAY;
         glbl_inc_dec_load = INC;
         glbl_step_size = 1;
         glbl_en_qtr_dly_upd_wbs = 0;
         glbl_compare_edge_type = RISE;
         glbl_compare_data_type = BITS;
         glbl_compare_data_bits = DQ_BITS;
         glbl_dual_edge_region = 0;
         glbl_pattern_type = pattern_type;
         glbl_iteration_limit = glbl_max_limit_90_degree; //glbl_rl_dly_qtr_max; 
         status = region_detect(); 
         glbl_en_qtr_dly_upd_wbs = 1;
         if(status) {
          //  write_error_XRAM(ERR_CODE_12_RD_DQ_FAIL_RISE_EDGE_VALID_PQTR);
          //  return 1;
        	 // The nibbles which have not been able to detect the common valid using the qtr delay taps in those nibble the sampling
        	 // clock could be right side of the valid window. So revert back the qtr delays to 0 and try with idelay increments.
        	 status = update_delay_sequential(P_N_DELAY /*int delay_type*/, LOAD /*int inc_dec_load*/);
         }
         //writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask_initial); 
         //riu_reg_bcast(INCDEC_CRSE,0,glbl_min_vld_cnt); 
         //status = update_delay_sequential( P_N_DELAY/*int delay_type*/, DEC /*int inc_dec_load*/,0); 
// XRAM_DBG: rddq_qtr_deskew
      for(nibble=0; nibble<DQ_NIBBLES; nibble++) {
        delay = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble], PQTR);
        delay -= glbl_min_vld_cnt;
        writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDDQ_QTR_DESKEW + nibble*4, delay);
      }
      DEBUG_PRINTF("INFO: 1st phase of the rd_cal() completed successfully \n");
      /**********************************************************************************************************/   
      // Ends 1st phase of the read data calibration (Moved PDQS and NDQS to valid region of their data bits 
      /**********************************************************************************************************/   


      /**********************************************************************************************************/   
      // Start 2nd phase of the read data calibration (Move DQ lines to Valid region sampling of their PDQS and NDQS
      /**********************************************************************************************************/   
      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: rd_cal(): Start 2nd phase of the read data calibration (Move DQ lines to Valid region sampling of their PDQS and NDQS for rank = %d with nibble mask = 0x%08X\n", RANKS, glbl_nibble_mask);
      DEBUG_PRINTF("##################################################################################################################### \n");
         glbl_min_vld_cnt = MIN_VLD_CNT;
         glbl_nibble_mask = 0xFFFFFFFF;
	 glbl_nibble_mask_nqtr = 0xFFFFFFFF;
	 glbl_bit_mask_31_0  = 0;
         glbl_bit_mask_63_32 = 0;
         glbl_bit_mask_95_64 = 0;
         glbl_bit_mask_31_0_last  = 0;
         glbl_bit_mask_63_32_last = 0;
         glbl_bit_mask_95_64_last = 0;
         glbl_clear_error = 1;
         //glbl_check_sequential = 0;
         glbl_phase_no = 0x2;
         glbl_region_type = PARTIAL;
         glbl_delay_type  = DQ_DELAY;
         glbl_inc_dec_load = INC;
         glbl_step_size = 1;
         glbl_en_qtr_dly_upd_wbs = 0;
         glbl_compare_edge_type = RISE;
         glbl_compare_data_type = BITS;
         glbl_compare_data_bits = DQ_BITS;
         glbl_dual_edge_region = 0;
         glbl_pattern_type = pattern_type;
         status = region_detect(); 
         glbl_en_qtr_dly_upd_wbs = 1;
         if(status) {
            write_error_XRAM(ERR_CODE_13_RD_DQ_FAIL_RISE_EDGE_VALID_DQ_IDELAY);
            write_error_bit_XRAM();
            return 1;
         }
         DEBUG_PRINTF("INFO: Revert the qtr delay by min valid counts \n");
         writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask_initial); 
         // Commented out the belwo pqtr/nqtr taps decrement because there could be possibility in the
         // phase 1 that common valid wouln't be found usin pqtr/nqtr and it has been found using idelays.
        // riu_reg_bcast(INCDEC_CRSE,0,MIN_VLD_CNT-glbl_step_size); // Revert by the minimum valid count less the step size
         status = update_delay_sequential( P_N_DELAY/*int delay_type*/, DEC /*int inc_dec_load*/); 
         if(status) 
            return 1;
      DEBUG_PRINTF("INFO: 2nd phase of the rd_cal() completed successfully \n");
      /**********************************************************************************************************/   
      // Ends 2nd phase of the read data calibration (Moved DQ lines to Valid region sample of their PDQS and NDQS
      /**********************************************************************************************************/   

      /**********************************************************************************************************/   
      // Start 3rd phase of the read data calibration (Move DQ lines to Noise region sampling of their PDQS
      /**********************************************************************************************************/   
      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: rd_cal(): Start 3rd phase of the read data calibration (Move DQ lines to Noise region sampling of their PDQS for rank = %d with nibble mask = 0x%08X\n", RANKS, glbl_nibble_mask);
      DEBUG_PRINTF("##################################################################################################################### \n");
         DEBUG_PRINTF("######################################################################################################################################## \n");
         DEBUG_PRINTF("################################ rd_cal(): Third phase a: Move DQ lines to Noise region using fine taps of step size = %d ####################### \n", step_size_coarse);
            glbl_nibble_mask = 0xFFFFFFFF;
	    glbl_nibble_mask_nqtr = 0xFFFFFFFF;
	    glbl_bit_mask_31_0  = 0;
            glbl_bit_mask_63_32 = 0;
            glbl_bit_mask_95_64 = 0;
            glbl_bit_mask_31_0_last  = 0;
            glbl_bit_mask_63_32_last = 0;
            glbl_bit_mask_95_64_last = 0;
            glbl_bits_error_status_31_0  = 0;
            glbl_bits_error_status_63_32 = 0;
            glbl_bits_error_status_95_64 = 0;
            glbl_clear_error = 1;// Fix Me: It should be 0 but in case of LDDR4 once it samples x it remains x until it gets cleared. And appers as no error.
            writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x1);
            writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);

            glbl_phase_no = 0x3A;
            glbl_region_type = FULL;
            glbl_delay_type  = DQ_DELAY;
            glbl_inc_dec_load = INC;
            glbl_step_size = step_size_coarse;
            glbl_compare_edge_type = RISE;
            glbl_compare_data_type = BITS;
            glbl_compare_data_bits = DQ_BITS;
            glbl_dual_edge_region = 0;
            glbl_pattern_type = pattern_type;

            // Before starting the noise detection phase add the delay on the data lines first.
            // Because if in the first iteration it see the noise then it increase the idelay and
            // after the end of the noise detection phase reverting all the idelay by the step size
            // may result in underflow.
            riu_reg_bcast(INCDEC_CRSE,0,glbl_step_size); 
            writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask_initial); 
            riu_reg_bcast(IODLY_BCAST_MASK,0,0x3); 
            status = update_delay_sequential(DQ_DELAY /*int delay_type*/, INC /*int inc_dec_load*/); 
            bitslice_reset_bcast(0, BS_RESET_WT);

            status = region_detect(); 
            if(status) {
               write_error_XRAM(ERR_CODE_14_RD_DQ_FAIL_RISE_EDGE_NOISE_DQ_IDELAY_HIGHER_TAPS);
               write_error_bit_XRAM();
               return 1;
            }

         DEBUG_PRINTF("######################################################################################################################################## \n");
         DEBUG_PRINTF("################################ rd_cal(): Third phase b: revert back to last coarse tap which gave valid sample ####################### \n");
            // Initialize loggic nibble mask and stable valid window to 0
            writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask_initial); 
            riu_reg_bcast(IODLY_BCAST_MASK,0,0x3); 
            status = update_delay_sequential(DQ_DELAY /*int delay_type*/, DEC /*int inc_dec_load*/); 
            if(status) 
               return 1;

         DEBUG_PRINTF("######################################################################################################################################## \n");
         DEBUG_PRINTF("################################ rd_cal(): Third phase c: Move DQ lines to Noise region using fine taps of step size = %d ####################### \n", 1);
            glbl_nibble_mask = 0xFFFFFFFF;
	    glbl_nibble_mask_nqtr = 0xFFFFFFFF;
	    glbl_bit_mask_31_0  = 0;
            glbl_bit_mask_63_32 = 0;
            glbl_bit_mask_95_64 = 0;
            glbl_bit_mask_31_0_last  = 0;
            glbl_bit_mask_63_32_last = 0;
            glbl_bit_mask_95_64_last = 0;
            glbl_bits_error_status_31_0  = 0;
            glbl_bits_error_status_63_32 = 0;
            glbl_bits_error_status_95_64 = 0;
            glbl_clear_error = 1;// Fix Me: It should be 0 but in case of LDDR4 once it samples x it remains x until it gets cleared. And appers as no error.
            writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x1);
            writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);

            glbl_phase_no = 0x3C;
            glbl_region_type = FULL;
            glbl_delay_type  = DQ_DELAY;
            glbl_inc_dec_load = INC;
            glbl_step_size = 1;
            glbl_en_qtr_dly_upd_wbs = 0;
            glbl_compare_edge_type = RISE;
            glbl_compare_data_type = BITS;
            glbl_compare_data_bits = DQ_BITS;
            glbl_dual_edge_region = 0;
            glbl_pattern_type = pattern_type;
            status = region_detect(); 
            glbl_en_qtr_dly_upd_wbs = 1;

// XRAM_DBG: rddq_idelay_final
            for(nibble=0; nibble<DQ_NIBBLES; nibble++) {
               delay = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble], IDLY2);
               writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDDQ_IDELAY_FINAL + nibble*4*4, delay);
               delay = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble], IDLY3);
               writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDDQ_IDELAY_FINAL + nibble*4*4 + 4, delay);
               delay = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble], IDLY4);
               writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDDQ_IDELAY_FINAL + nibble*4*4 + 8, delay);
               delay = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble], IDLY5);
               writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDDQ_IDELAY_FINAL + nibble*4*4 + 12, delay);
            }
            if(status) {
               write_error_XRAM(ERR_CODE_15_RD_DQ_FAIL_RISE_EDGE_NOISE_DQ_IDELAY);
               write_error_bit_XRAM();
               return 1;
            }

         //DEBUG_PRINTF("######################################################################################################################################## \n");
         //DEBUG_PRINTF("################################ rd_cal(): Third phase d: Find minimum taps nibbles wise and adust all the idelay and the qtrs ####################### \n");
         //   u16 rd_delay_min;
         //   u16 rd_delay;
         //   for(nibble=0; nibble < DQ_NIBBLES; nibble++) {
         //      rd_delay = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble] /*nibble*/, PQTR /*reg_address*/);
         //      DEBUG_PRINTF("INFO: 3rd d phase of the rd_cal() rd_dly of pqtr = %d, of lnibble = %d, pnibble = %d\n", rd_delay, nibble, glbl_dq_lnibble2pnibble_map[nibble]);
         //      rd_delay_min = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble] /*nibble*/, NQTR /*reg_address*/);
         //      DEBUG_PRINTF("INFO: 3rd d phase of the rd_cal() rd_dly of nqtr = %d, of lnibble = %d, pnibble = %d\n", rd_delay_min, nibble, glbl_dq_lnibble2pnibble_map[nibble]);
         //      if(rd_delay_min > rd_delay)
         //         rd_delay_min = rd_delay;
         //      for(bit_cnt=0; bit_cnt<4; bit_cnt++) {
         //         rd_delay = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble] /*nibble*/, IDLY0+2+bit_cnt/*reg_address*/);
         //         DEBUG_PRINTF("INFO: 3rd d phase of the rd_cal() rd_dly of idelay[%d] = %d, of lnibble = %d, pnibble = %d\n", bit_cnt+4*nibble, rd_delay, nibble, glbl_dq_lnibble2pnibble_map[nibble]);
         //         if(rd_delay_min > rd_delay)
         //            rd_delay_min = rd_delay;
         //      }
         //      DEBUG_PRINTF("INFO: 3rd d phase of the rd_cal() rd_dly_min = %d, of lnibble = %d, pnibble = %d\n", rd_delay_min, nibble, glbl_dq_lnibble2pnibble_map[nibble]);
         //      write_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble] /*nibble*/, CRSE_DLY /*reg_address*/, rd_delay_min);
         //      write_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble] /*nibble*/, INCDEC_CRSE /*reg_address*/, rd_delay_min);
         //   }
         //   writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask_initial); 
         //   initialize_u16_array(glbl_pnibble2pbs_bcast_map,27,0xFFFF);
         //   set_nibble2bcast_bits( 1, DQ_BITS);
         //   status = update_delay_sequential(P_N_DQ_DELAY /*int delay_type*/, DEC /*int inc_dec_load*/,0); 

      DEBUG_PRINTF("INFO: 3rd phase of the rd_cal() completed successfully \n");
      /**********************************************************************************************************/   
      // Ends 3rd phase of the read data calibration (Moved DQ lines to Noise region sampling of their PDQS 
      /**********************************************************************************************************/   
   } 
   /**********************************************************************************************************/   
   // Start 4th phase of the read data calibration (Move all DQ together lines to Noise region sampling of their NDQS
   /**********************************************************************************************************/   
   for(bit_cnt=0; bit_cnt < DQ_NIBBLES; bit_cnt++) {
      glbl_lbit2pnibble_map[bit_cnt] = glbl_dq_lnibble2pnibble_map[bit_cnt];
   }
   if(pattern_type != PRBS) {
      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: rd_cal(): Start 4th phase of the read data calibration (Move all DQ together lines to Noise region sampling of their NDQS for rank = %d with nibble mask = 0x%08X\n", RANKS, glbl_nibble_mask);
      DEBUG_PRINTF("##################################################################################################################### \n");
      u8 delay_type;
      if (pattern_type == SIMPLE) {
         delay_type = DQ_DELAY;
      } else {
         delay_type = DQ_DBI_DELAY;
      }
      glbl_nibble_mask = 0xFFFFFFFF;
      glbl_nibble_mask_nqtr = 0xFFFFFFFF;
      glbl_bit_mask_31_0  = 0;
      glbl_bit_mask_31_0_last  = 0;
      glbl_bits_error_status_31_0  = 0;
      glbl_clear_error = 1;// Fix Me: It should be 0 but in case of LDDR4 once it samples x it remains x until it gets cleared. And appers as no error.
      writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x1);
      writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);

      glbl_phase_no = 4;
      glbl_region_type = FULL;
      glbl_delay_type  = delay_type;
      glbl_inc_dec_load = INC;
      glbl_step_size = 1;
      glbl_en_qtr_dly_upd_wbs = 0;
      glbl_compare_edge_type = FALL;
      glbl_compare_data_type = NIBBLES;
      glbl_compare_data_bits = DQ_BITS;
      glbl_dual_edge_region = 0;
      glbl_pattern_type = pattern_type;

// XRAM_DBG: rdcmplx_idelay_offset
      if (pattern_type == COMPLEX) {
         for(nibble=0; nibble<DQ_NIBBLES; nibble++) {
            idelay_ref[nibble] = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble], IDLY2);
         }
      }

      status = region_detect(); 
      glbl_en_qtr_dly_upd_wbs = 1;

// XRAM_DBG: rdcmplx_idelay_offset
      if (pattern_type == COMPLEX) {
         for(nibble=0; nibble<DQ_NIBBLES; nibble++) {
            delay = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble], IDLY2);
            delay -= idelay_ref[nibble];
            writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDCMPLX_IDELAY_OFFSET + nibble*4, delay);
         }
      }
      if(status) {
         if(pattern_type == COMPLEX) {
            write_error_XRAM(ERR_CODE_49_RD_COMPLEX_FAIL_NOISE_DQ_IDELAY);
         } else if(pattern_type == SIMPLE) {
            write_error_XRAM(ERR_CODE_16_RD_DQ_FAIL_NOISE_DQ_IDELAY);
            write_error_bit_XRAM();
         }
         return 1;
      }
      /**********************************************************************************************************/   
      // Ends 4th phase of the read data calibration (Moved all DQ together lines to Noise region sampling of their NDQS
      /**********************************************************************************************************/   
   }


   /**********************************************************************************************************/   
   // Start 5th phase of the read data calibration (Move PDQS and NDQS to find Valid window correspond their associated bits)
   /**********************************************************************************************************/   
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: rd_cal(): Start 5th phase of the read data calibration (Move PDQS and NDQS to find Valid window correspond their associated bits) for rank = %d with nibble mask = 0x%08X\n", RANKS, glbl_nibble_mask);
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
   glbl_en_qtr_dly_upd_wbs = 0;
   glbl_compare_edge_type = RISE;
   glbl_compare_data_type = NIBBLES;
   glbl_compare_data_bits = DQ_NIBBLES;
   glbl_dual_edge_region = 1;
   glbl_pattern_type = pattern_type;
   status = region_detect();    
   glbl_en_qtr_dly_upd_wbs = 1;

   static u16 pqtr_init_val[DQ_NIBBLES];
   static u16 nqtr_init_val[DQ_NIBBLES];
   for(nibble=0; nibble<DQ_NIBBLES;nibble++) {
      pqtr_init_val[nibble] = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble],PQTR);
      DEBUG_PRINTF("INFO: ddr_rd_cal(): pqtr_init_val[nibble][%d] = %d \n", nibble, pqtr_init_val[nibble]);
// XRAM_DBG: rddq_pqtr_left: -glbl_min_vld_cnt
// XRAM_DBG: rdcmplx_pqtr_left_short: -glbl_min_vld_cnt
      delay = pqtr_init_val[nibble] - glbl_min_vld_cnt;
      if(pattern_type == COMPLEX) {
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDCMPLX_PQTR_LEFT_SHORT + nibble*4, delay);
      } else if(pattern_type == SIMPLE) {
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDDQ_PQTR_LEFT + nibble*4, delay);
      }

      nqtr_init_val[nibble] = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble],NQTR);
      DEBUG_PRINTF("INFO: ddr_rd_cal(): nqtr_init_val[nibble][%d] = %d \n", nibble, nqtr_init_val[nibble]);
// XRAM_DBG: rddq_nqtr_left: -glbl_min_vld_cnt
// XRAM_DBG: rdcmplx_nqtr_left_short: -glbl_min_vld_cnt
      delay = nqtr_init_val[nibble] - glbl_min_vld_cnt;
      if(pattern_type == COMPLEX) {
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDCMPLX_NQTR_LEFT_SHORT + nibble*4, delay);
      } else if(pattern_type == SIMPLE) {
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDDQ_NQTR_LEFT + nibble*4, delay);
      }
   }

   if(status) {
      if(pattern_type == COMPLEX) {
         write_error_XRAM(ERR_CODE_50_RD_COMPLEX_FAIL_LEFT_EDGE_PQTR_NQTR_SHORT_BURST);
      } else if(pattern_type == SIMPLE) {
         write_error_XRAM(ERR_CODE_17_RD_DQ_FAIL_LEFT_EDGE_PQTR_NQTR);
      } else if(pattern_type == PRBS) {
         write_error_XRAM(ERR_CODE_66_RD_PRBS_FAIL_LEFT_EDGE_PQTR_NQTR);
      }
      return 1;
   }

   if(pattern_type == COMPLEX && glbl_vref_cal == 0) {
      DEBUG_PRINTF("######################################################################################################################################## \n");
      DEBUG_PRINTF("################################ rd_cal(): Fifth phase aa: Revert back by MIN Valid count #################################### \n");
      glbl_nibble_mask      = glbl_nibble_mask_initial;
      glbl_nibble_mask_nqtr = glbl_nibble_mask_initial;
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask); 
      riu_reg_bcast(INCDEC_CRSE,0,glbl_min_vld_cnt - 1); //EDT-1112869 
      //riu_reg_bcast(CRSE_DLY,0,glbl_min_vld_cnt); 
      status = update_delay_sequential( P_N_DELAY /*int delay_type*/, DEC /*int inc_dec_load*/); 
      // Increase the burst length
      glbl_cplx_loop_cnt = CPLX_LOOP_CNT;               // [14:7] Loop count to go through the pattern
      glbl_cplx_row_cnt = CPLX_ROW_CNT;                // [17:15] Maximum row count value. For fast cal simulation, set it to 0.
      glbl_cplx_burst_array_index = CPLX_BURST_ARRAY_INDEX;   // [24:20] The max index value when incrementing the burst array sequence. For fast cal simulation, set it to 3.

      DEBUG_PRINTF("######################################################################################################################################## \n");
      DEBUG_PRINTF("################################ rd_cal(): Fifth phase ab: use Fine taps to go deepr into valid region with the big burst #################################### \n");
      glbl_min_vld_cnt = MIN_VLD_CNT_CPLX;
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

      glbl_phase_no = 0x5A;
      glbl_region_type = PARTIAL;
      glbl_delay_type  = P_DELAY;
      glbl_inc_dec_load = INC;
      glbl_step_size = 1;
      glbl_en_qtr_dly_upd_wbs = 0;
      glbl_compare_edge_type = RISE;
      glbl_compare_data_type = NIBBLES;
      glbl_compare_data_bits = DQ_NIBBLES;
      glbl_dual_edge_region = 1;
      glbl_pattern_type = pattern_type;
      status = region_detect(); 
      glbl_en_qtr_dly_upd_wbs = 1;

      for(nibble=0; nibble<DQ_NIBBLES;nibble++) {
         pqtr_init_val[nibble] = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble],PQTR);
         DEBUG_PRINTF("INFO: ddr_rd_cal(): pqtr_init_val[nibble][%d] = %d for complex cal \n", nibble, pqtr_init_val[nibble]);
// XRAM_DBG: rdcmplx_pqtr_left: -glbl_min_vld_cnt
         delay = pqtr_init_val[nibble] - glbl_min_vld_cnt;
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDCMPLX_PQTR_LEFT + nibble*4, delay);

         nqtr_init_val[nibble] = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble],NQTR);
         DEBUG_PRINTF("INFO: ddr_rd_cal(): nqtr_init_val[nibble][%d] = %d for complex cal \n", nibble, nqtr_init_val[nibble]);
// XRAM_DBG: rdcmplx_nqtr_left: -glbl_min_vld_cnt
         delay = nqtr_init_val[nibble] - glbl_min_vld_cnt;
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDCMPLX_NQTR_LEFT + nibble*4, delay);
      }
      if(status) {
         write_error_XRAM(ERR_CODE_51_RD_COMPLEX_FAIL_LEFT_EDGE_PQTR_NQTR);
         return 1;
      }
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
   //if(status) { 
   //   status = update_delay_sequential( glbl_delay_type /*int delay_type*/, DEC /*int inc_dec_load*/); 
   //}
   for(nibble=0; nibble<DQ_NIBBLES; nibble++) {
// XRAM_DBG: rddq_pqtr_right_crse
// XRAM_DBG: rdcmplx_pqtr_right_short_crse
      delay = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble], PQTR);
      if(pattern_type == COMPLEX) {
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDCMPLX_PQTR_RIGHT_SHORT_FCRSE + nibble*4, delay);
      } else if(pattern_type == SIMPLE) {
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDDQ_PQTR_RIGHT_FCRSE + nibble*4, delay);
      }

// XRAM_DBG: rddq_nqtr_right_crse
// XRAM_DBG: rdcmplx_nqtr_right_short_crse
      delay = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble], NQTR);
      if(pattern_type == COMPLEX) {
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDCMPLX_NQTR_RIGHT_SHORT_FCRSE + nibble*4, delay);
      } else if(pattern_type == SIMPLE) {
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDDQ_NQTR_RIGHT_FCRSE + nibble*4, delay);
      }
   }

   //DEBUG_PRINTF("######################################################################################################################################## \n");
   //DEBUG_PRINTF("################################ rd_cal(): Fifth phase c: revert back to last coarse tap which gave valid sample ####################### \n");
   //// Initialize loggic nibble mask and stable valid window to 0
   //glbl_nibble_mask      = glbl_nibble_mask_initial;
   //glbl_nibble_mask_nqtr = glbl_nibble_mask_initial;
   //writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask); 

   //glbl_step_size = step_size_coarse;
   //status = update_delay_sequential( P_N_DELAY /*int delay_type*/, DEC /*int inc_dec_load*/); 
   ////status = status | update_delay_sequential( N_DELAY /*int delay_type*/, DEC /*int inc_dec_load*/,step_size_coarse); 
   ////if(status) 
   ////   return 1;

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
   glbl_en_qtr_dly_upd_wbs = 0;
   glbl_compare_edge_type = RISE;
   glbl_compare_data_type = NIBBLES;
   glbl_compare_data_bits = DQ_NIBBLES;
   glbl_dual_edge_region = 1;
   glbl_pattern_type = pattern_type;
   status = region_detect(); 
   glbl_en_qtr_dly_upd_wbs = 1;

   for(nibble=0; nibble<DQ_NIBBLES; nibble++) {
// XRAM_DBG: rddq_pqtr_right: +glbl_min_vld_cnt
// XRAM_DBG: rdcmplx_pqtr_right_short: +glbl_min_vld_cnt
      delay = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble], PQTR);
      delay += glbl_min_vld_cnt;
      if(pattern_type == COMPLEX) {
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDCMPLX_PQTR_RIGHT_SHORT + nibble*4, delay);
      } else if(pattern_type == SIMPLE) {
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDDQ_PQTR_RIGHT + nibble*4, delay);
      }

// XRAM_DBG: rddq_nqtr_right: +glbl_min_vld_cnt
// XRAM_DBG: rdcmplx_nqtr_right_short: +glbl_min_vld_cnt
      delay = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble], NQTR);
      delay += glbl_min_vld_cnt;
      if(pattern_type == COMPLEX) {
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDCMPLX_NQTR_RIGHT_SHORT + nibble*4, delay);
      } else if(pattern_type == SIMPLE) {
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDDQ_NQTR_RIGHT + nibble*4, delay);
      }
   }

   if(status) {
      if(pattern_type == COMPLEX) {
         write_error_XRAM(ERR_CODE_52_RD_COMPLEX_FAIL_RIGHT_EDGE_PQTR_NQTR_SHORT_BURST);
      } else if(pattern_type == SIMPLE) {
         write_error_XRAM(ERR_CODE_18_RD_DQ_FAIL_RIGHT_EDGE_PQTR_NQTR);
      } else if(pattern_type == PRBS) {
         write_error_XRAM(ERR_CODE_67_RD_PRBS_FAIL_RIGHT_EDGE_PQTR_NQTR);
      }
      return 1;
   }

   if(pattern_type == COMPLEX && glbl_vref_cal == 0) {
      DEBUG_PRINTF("######################################################################################################################################## \n");
      DEBUG_PRINTF("################################ rd_cal(): Fifth phase da: Revert back by MIN Valid count #################################### \n");
      glbl_nibble_mask      = glbl_nibble_mask_initial;
      glbl_nibble_mask_nqtr = glbl_nibble_mask_initial;
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask); 
      riu_reg_bcast(INCDEC_CRSE,0,glbl_min_vld_cnt-1); //EDT-1112869 
      //riu_reg_bcast(CRSE_DLY,0,glbl_min_vld_cnt); 
      status = update_delay_sequential( P_N_DELAY /*int delay_type*/, INC /*int inc_dec_load*/); 
      // Increase the burst length
      glbl_cplx_loop_cnt = CPLX_LOOP_CNT;               // [14:7] Loop count to go through the pattern
      glbl_cplx_row_cnt = CPLX_ROW_CNT;                // [17:15] Maximum row count value. For fast cal simulation, set it to 0.
      glbl_cplx_burst_array_index = CPLX_BURST_ARRAY_INDEX;   // [24:20] The max index value when incrementing the burst array sequence. For fast cal simulation, set it to 3.

      DEBUG_PRINTF("######################################################################################################################################## \n");
      DEBUG_PRINTF("################################ rd_cal(): Fifth phase db: use Fine taps to go deepr into valid region with the big burst #################################### \n");
      glbl_min_vld_cnt=MIN_VLD_CNT_CPLX;
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
      glbl_en_qtr_dly_upd_wbs = 0;
      glbl_compare_edge_type = RISE;
      glbl_compare_data_type = NIBBLES;
      glbl_compare_data_bits = DQ_NIBBLES;
      glbl_dual_edge_region = 1;
      glbl_pattern_type = pattern_type;
      status = region_detect(); 
      glbl_en_qtr_dly_upd_wbs = 1;

      for(nibble=0; nibble<DQ_NIBBLES; nibble++) {
// XRAM_DBG: rdcmplx_pqtr_right: +glbl_min_vld_cnt
         delay = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble], PQTR);
         delay += glbl_min_vld_cnt;
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDCMPLX_PQTR_RIGHT + nibble*4, delay);
   
// XRAM_DBG: rdcmplx_nqtr_right: +glbl_min_vld_cnt
         delay = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble], NQTR);
         delay += glbl_min_vld_cnt;
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDCMPLX_NQTR_RIGHT + nibble*4, delay);
      }
      glbl_min_vld_cnt = MIN_VLD_CNT;

      if(status) { 
         write_error_XRAM(ERR_CODE_53_RD_COMPLEX_FAIL_RIGHT_EDGE_PQTR_NQTR);
         return 1;
      }
   }
   DEBUG_PRINTF("INFO: 5th phase of the rd_cal() completed successfully \n");
   /**********************************************************************************************************/   
   // Ends 5th phase of the read data calibration (Moved PDQS and NDQS to find Valid window correspond their associated bits)
   /**********************************************************************************************************/   


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
      delay = (pqtr_end_val[nibble] + pqtr_init_val[nibble])/2;
      if(pattern_type == COMPLEX) {
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDCMPLX_PQTR_FINAL + nibble*4, delay);
      } else if(pattern_type == SIMPLE) {
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDDQ_PQTR_FINAL + nibble*4, delay);
      }
      if(pattern_type == PRBS) {
         delay = pqtr_old_center_val[nibble];
      }
      pqtr_left_margin[nibble]  = delay - pqtr_init_val[nibble] + glbl_min_vld_cnt; 
      pqtr_right_margin[nibble] = pqtr_end_val[nibble] - delay  + glbl_min_vld_cnt; 
      write_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble] /*u8 nibble*/, PQTR /*u8 reg_addr*/, delay);
      DEBUG_PRINTF("INFO: rd_cal(): loading PQTR of Logical Nibble %d or Physical Nibble %d with %d taps , left margin = %d, right margin = %d \n", nibble, glbl_dq_lnibble2pnibble_map[nibble], delay, pqtr_left_margin[nibble], pqtr_right_margin[nibble]) 

      if(pattern_type == PRBS) {
         if(glbl_cal_done == 0) { // Calibration margins
// XRAM_DBG: rdprbs_pqtr_left_margin
// XRAM_DBG: rdprbs_pqtr_right_margin
            writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDPRBS_PQTR_LEFT_MARGIN + nibble*4, pqtr_left_margin[nibble]);
            writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDPRBS_PQTR_RIGHT_MARGIN + nibble*4, pqtr_right_margin[nibble]);
         } else { // Mission mode margins
// XRAM_DBG: rdmargin_pqtr_left
// XRAM_DBG: rdmargin_pqtr_right
            writeIO_XRAM(XRAM_RDMARGIN_PQTR_LEFT + nibble*4, pqtr_left_margin[nibble]);
            writeIO_XRAM(XRAM_RDMARGIN_PQTR_RIGHT + nibble*4, pqtr_right_margin[nibble]);
         }
      }

      glbl_stable_valid_win_nqtr[nibble]=(nqtr_end_val[nibble]-nqtr_init_val[nibble])+2*glbl_min_vld_cnt;
// XRAM_DBG: rddq_nqtr_final
// XRAM_DBG: rdcmplx_nqtr_final
      delay = (nqtr_end_val[nibble] + nqtr_init_val[nibble])/2;
      if(pattern_type == COMPLEX) {
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDCMPLX_NQTR_FINAL + nibble*4, delay);
      } else if(pattern_type == SIMPLE) {
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDDQ_NQTR_FINAL + nibble*4, delay);
      }
      if(pattern_type == PRBS) {
         delay = nqtr_old_center_val[nibble];
      }
      nqtr_left_margin[nibble]  = delay - nqtr_init_val[nibble] + glbl_min_vld_cnt; 
      nqtr_right_margin[nibble] = nqtr_end_val[nibble] - delay  + glbl_min_vld_cnt; 
      write_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble] /*u8 nibble*/, NQTR /*u8 reg_addr*/, delay);
      DEBUG_PRINTF("INFO: rd_cal(): loading NQTR of Logical Nibble %d or Physical Nibble %d with %d taps , left margin = %d, right margin = %d \n", nibble, glbl_dq_lnibble2pnibble_map[nibble], delay, nqtr_left_margin[nibble], nqtr_right_margin[nibble]) 

      if(pattern_type == PRBS) {
         if(glbl_cal_done == 0) { // Calibration margins
// XRAM_DBG: rdprbs_nqtr_left_margin
// XRAM_DBG: rdprbs_nqtr_right_margin
            writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDPRBS_NQTR_LEFT_MARGIN + nibble*4, nqtr_left_margin[nibble]);
            writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_RDPRBS_NQTR_RIGHT_MARGIN + nibble*4, nqtr_right_margin[nibble]);
         } else { // Mission mode margins
// XRAM_DBG: rdmargin_nqtr_left
// XRAM_DBG: rdmargin_nqtr_right
            writeIO_XRAM(XRAM_RDMARGIN_NQTR_LEFT + nibble*4, nqtr_left_margin[nibble]);
            writeIO_XRAM(XRAM_RDMARGIN_NQTR_RIGHT + nibble*4, nqtr_right_margin[nibble]);
         }
      }
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
            return 1;
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
            return 1;
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
            return 1;
         }
      }
   }
   DEBUG_PRINTF("INFO: 7th phase of the rd_cal() completed successfully \n");
   /**********************************************************************************************************/   
   // Ends 7th phase of the read data calibration (Sanity of the calibration Passed)
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










