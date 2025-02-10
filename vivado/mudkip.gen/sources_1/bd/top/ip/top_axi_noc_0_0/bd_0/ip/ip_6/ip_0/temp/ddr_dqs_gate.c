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
//       Revision:       $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/ddr_dqs_gate.c#6 $
//                       $Author: rgaddam $
//                       $DateTime: 2019/04/24 20:01:22 $
//                       $Change: 21332573 $
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
void collect_gt_sample (  u32 *dqs_received_pattern); 
void update_rnk_delay_sequential(u8 rank_offset, u8 coares_fine_n, int inc_dec_load, u8 step_size, u8 read_latency, u8 coarse_delay, u16 fine_delay); 
int edge_detect_phase(u8 rank,   u8 phase_num) ; 


u8 ddr_dqs_gate(u8 restore_cal) {
#ifdef SKIP_DQS_CAL_STAGE
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: dqs_gate(): DQS GATE CALIBRATION SKIPPED \n");
   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: dqs_gate(): Loading precalculated delay values to skip the DQS GATE CALIBRATION \n");
   DEBUG_PRINTF("##################################################################################################################### \n");
   skip_ddr_dqs_gate();
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: dqs_gate(): Loaded precalculated delay values to skip the DQS GATE CALIBRATION \n");
   DEBUG_PRINTF("##################################################################################################################### \n");
#else
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: dqs_gate(): DQS GATE CALIBRATION START \n");
   DEBUG_PRINTF("##################################################################################################################### \n");

   u16 fine_delay_array[RANKS][DQS_BITS] ;
   u8  coarse_delay_array[RANKS][DQS_BITS];
   u8  read_latency_array[RANKS][DQS_BITS];
   u8  nibble;
   u8  rank, rank_rcd;
   u8  stop_read_burst = 0;
   u8  loop_cnt = 0, LOOP_LIMIT = 8; // Maximum sweep of read latency range = LOOP_LIMIT * 3
   u8  command2compare_delay = glbl_mem_read_latency+RD_CMD2GT_SAMPLE_DELAY; 
   int status;
   u8  phy_nibble;
   u16 addr;
   u8  read_latency;
   u16 coarse_fine_delay;
   u16 fine_delay;
   u8  coarse_delay;

   #ifdef DDR4
      u8 dqs_mode  = (glbl_mr4>>11) & 0x1;
      if(dqs_mode == 0) // 1tCK mode, toggle preamble 
         dqs_mode = 1;
      else              // 2tck mode, toggle preamble
         dqs_mode = 2;
   #else
      u8 dqs_mode  = (glbl_lp4_mr1>>3) & 0x1;
      if(dqs_mode == 0) // 2tCK mode, static preamble 
         dqs_mode = 4;
      else              // 2tck mode, toggle preamble
         dqs_mode = 3;
   #endif
      writeIO_XRAM(0xB0,MIN_VLD_CNT);
      writeIO_XRAM(0xB4,SAMPLE_CNT);
   u32 nibble_ctrl2 = (read_riu_nibble_reg(glbl_dqs_lnibble2pnibble_map[0],DUP_NIBBLE_CTRL2) & 0xC7FF) | (dqs_mode<<11);

   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: dqs_gate(): Phase 1: Initialize internal parameters and Put the Memory in Preamble traing mode                           \n");
   DEBUG_PRINTF("##################################################################################################################### \n");

   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: dqs_gate(): DQS calibration has started for Ranks = %d, DQS PER RANK = %d with read latency of %d, and initial latency of %d \n", RANKS, DQS_BITS, glbl_mem_read_latency, glbl_initial_read_latency);
   // Perform DQS Gate calibration for each rank sequentially
   for(rank = 0; rank < RANKS; rank += glbl_rank_inc) {  
      rank_rcd = (rank / (RANKS/SLOTS)) * (RANKS/SLOTS);
      status = banks_refresh(rank, 0x10 | rank);
      if(status) {
         return 1;
      }

      //Set Commaand to GT status compare delay
      writeIO_DMC(ral_addr_of_seq_data_capture_in_ddrmc_main_ddrmc_main_bank(BLK),(command2compare_delay<<1)|0x1); // - sel - rw - uint - [0] 0x0 - 0 - cal_seq - When set to 0, regular data is captured based on the data valid selection. 
                                                                  // When set to 1, data is captured based on the counter programmed in bits [6:1]. 
                                                                  // When seq_mode is set to 1 (WRLVL mode) it automatically turns on this mode also.
                                                                  // - delay_cnt - rw - uint - [6:1] 0x0 - 0 - cal_seq - Sets a delay in controller clock cycles after 
                                                                  // a command goes out that a flag gets generated to capture the data (either DQ for WRLVL mode, or GT_STATUS if selected). 
                                                                  // This allows capture of certain data at a programmable time.



      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: dqs_gate(): DQS calibration has started for Rank = %d, DQS PER RANK = %d with read latency of %d, and initial latency of %d \n", rank, DQS_BITS, glbl_mem_read_latency, glbl_initial_read_latency);

      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: dqs_gate(): First nibble = %d, last Nibble = %d, and Nibbles Mask = 0x%8X \n", glbl_dqs_first_nibble, glbl_dqs_last_nibble, glbl_dqs_nibble_mask_initial);
      /*************************************************************************************************/
      /*************************************************************************************************/
      // Initialize the internal variables
      for(nibble = 0; nibble < DQS_BITS; nibble++) {
         fine_delay_array[rank][nibble] = 0;
         coarse_delay_array[rank][nibble] = 0;
         read_latency_array[rank][nibble] = glbl_initial_read_latency;
      }

   #define XPHY_BASED_TRAIN
   #ifdef XPHY_BASED_TRAIN
      u8  gt_train;
      u8  gt_done;
      u8  iter;
      u16 coarse_over_under_flow;
      u32 coarse_overflow;
      u8  restore_read_latency[27];
      u16 rl_dly_rnk;
      u16 rl_dly_rnk_crse;
      #ifdef DDR4
         u32 rd_gap = (20000 / glbl_tck) + 4 + 10; // FIXME: Minimum of 4 CTRL_CLOCKs to XPHY (200MHz - 5ns) + BL8 + Phase alignment
      #else
         u32 rd_gap = (20000 / glbl_tck) + 8 + 10; // Minimum of 4 CTRL_CLOCKs to XPHY (200MHz - 5ns) + BL16 + Phase alignment
      #endif
      glbl_nibble_mask = glbl_dqs_nibble_mask_initial;
      // Setup the number of read commands. 
      //writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), GT_SAMPLES<<16 | GT_SAMPLES);
      //writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), (1<<16) | 1);

      // Initialize the delays
      if(restore_cal == 0) {
         for(phy_nibble = glbl_dqs_first_nibble; phy_nibble <= glbl_dqs_last_nibble; phy_nibble++) {
            if(!((glbl_nibble_mask>>phy_nibble)&0x1)) {
               writeIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK)+phy_nibble*4,glbl_initial_read_latency);
               write_riu_nibble_reg(phy_nibble /*u8 nibble*/, RLDLYRNK0 + rank /*u8 reg_addr*/, 0x8000);
            }
         }
      } else { // Reduce by 4 coarse delays or 1 read latency
         for(phy_nibble = glbl_dqs_first_nibble; phy_nibble <= glbl_dqs_last_nibble; phy_nibble++) {
            if(!((glbl_nibble_mask>>phy_nibble)&0x1)) {
               if(rank == 0) { // Capture for Rank 0 only to avoid cumulative affect
                  restore_read_latency[phy_nibble] = readIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK)+phy_nibble*4);
               }
               rl_dly_rnk = read_riu_nibble_reg(phy_nibble, RLDLYRNK0 + rank);
	       rl_dly_rnk_crse = (rl_dly_rnk >> 9) & 0xF;
	       if(rl_dly_rnk_crse < 4) {
	          read_latency = restore_read_latency[phy_nibble] - 1;
		  rl_dly_rnk = (rl_dly_rnk_crse << 9);
	       } else {
	          read_latency = restore_read_latency[phy_nibble];
	          rl_dly_rnk_crse = (rl_dly_rnk_crse - 4);
		  rl_dly_rnk = (rl_dly_rnk_crse << 9);
	       }
               writeIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK)+phy_nibble*4, read_latency);
               write_riu_nibble_reg(phy_nibble /*u8 nibble*/, RLDLYRNK0 + rank /*u8 reg_addr*/, (rl_dly_rnk | 0x8000));
            }
         }
      }

      status = banks_refresh(rank, 0x14 | rank);
      if(status) {
         return 1;
      }
      /*************************************************************************************************/
      //   Unmask all the nibbles which are required for the dqs gate calibration
      /*************************************************************************************************/
      #ifdef LPDDR4
         //Put the memory in preamble training mode
         DEBUG_PRINTF("INFO: dqs_gate(): LPDDR4: Putting the memory in Read preamble training mode \n");
         lp4_mode_reg_wr(rank, 13, glbl_lp4_mr13|0x2);

         //Set MPC Read DQ CAL command
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0x105);
         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), LP4_MPC_RD_DQ);

      #else
         if(glbl_lrdimm_en) {
            ddr_mrs7_write (rank_rcd, 0x1100 | DB_F0BC1X_PACKAGE_RANK_TIMING_ALIGNMENT_ENABLE | 0x10); // F0BC1X Register: Enter Read Preamble Training Mode
         }

         // Before putting the memory in the read preamble training mode put the memory in MPR Mode
         DEBUG_PRINTF("INFO: dqs_gate(): Putting the memory in read preamble training mode \n");
         set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                               MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr3 | MEM_ENABLE_MPR /*u32 seq_a_addr*/, 0x0 /*u8 seq_a_bg*/, 0x3 /*u16 seq_a_ba*/,
                               0 /*u32 seq_a2a_dly*/);

         // Put memory in the DQS Training Mode
         DEBUG_PRINTF("INFO: dqs_gate(): Putting the memory in DQS training mode \n");
         set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_(seqs*/, 
                                MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr4 | MEM_ENABLE_PMB_TRAIN /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x0 /*u16 seq_a_ba*/,
                               0 /*u32 seq_a2a_dly*/);
         
         //Set Column read command
         writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),MEM_COL_READ);
      #endif

      // Load the read count and gap
      if(restore_cal == 0) {
         writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 80); // Setup the command gaps to 6 which is more than the xphy requirment of 4 clocks
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), (32<<16) | 32);
      } else {
         writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), rd_gap); // Setup the command gaps to xphy requirment of 4 control clocks
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), (64<<16) | 64);
      }

      loop_cnt = 0;
      do {
         gt_done = 0;
         // Enable the DQS gate training and disable the DQS gate tracking in the XPHY
         DEBUG_PRINTF("INFO: dqs_gate(): Enabled the DQS gate training \n");
         writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask); 
         riu_reg_bcast(BS_RESET_CTRL, 0, 0x1<<CLR_GATE_POS);
         bitslice_reset_bcast(0, BS_RESET_WT);
         riu_reg_bcast(DUP_NIBBLE_CTRL2,0,(0x3<<6 | nibble_ctrl2) & 0xC7FF); // Making dqs_mode to 0 and enable gt_train
         glbl_gt_train_en = 1;

         if(restore_cal == 0) {
            iter = 0;
            while(gt_done == 0) {
               // Enable the read commands generation
               DEBUG_PRINTF("INFO: dqs_gate(): Start the read commands generation \n");
               writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

               // Poll for the command geneation done
               while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

               // Poll gate training mode bit for 0. 0-training done; 1-training in progress
               DEBUG_PRINTF("INFO: dqs_gate(): Poll for the gate training mode bit\n");
               gt_done = 1;
               for(phy_nibble = glbl_dqs_first_nibble; phy_nibble <= glbl_dqs_last_nibble; phy_nibble++) {
                  if(!((glbl_nibble_mask>>phy_nibble)&0x1)) {
                     gt_train = read_riu_nibble_reg(phy_nibble /*u8 nibble*/, DUP_NIBBLE_CTRL2 /*u8 reg_addr*/) & 0x40;
                     if(gt_train != 0) {
                        gt_done = 0;
                     }
                  }
               }
               if(iter > 64) { // Low frequencies need higher iterations
                  write_error_XRAM(ERR_CODE_6_DQS_GATE_TRAIN_TIMEOUT);
                  write_error_nibble_XRAM(1, ~glbl_nibble_mask);
                  writeIO_XRAM(XRAM_CAL_RANK, (1<<rank | 1 << 4));
                  return 1;
               }
               iter++;
            }
         } else { // Restore calibration
            stop_read_burst = 0;
            iter = 0;
            while(gt_done == 0) {
               if(readIO_DMC(ral_addr_of_seq_status_in_ddrmc_main_ddrmc_main_bank(BLK)) == 1) { // Sequencer is IDLE
                  if(stop_read_burst == 0) {
                     // Enable the read commands generation
                     DEBUG_PRINTF("INFO: dqs_gate(): Start the read commands generation \n");
                     writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
                     stop_read_burst = (glbl_tck >= TCK_TH_DQS_GATE) ? 1 : 0; // Stop at the end of first read burst for lower frequencies
                  } else {
                     DEBUG_PRINTF("INFO: dqs_gate(): Stop at the end of first read burst for lower frequencies\n");
                     riu_reg_bcast(DUP_NIBBLE_CTRL2,0,(0x2<<6 | nibble_ctrl2) & 0xC7FF); // Making dqs_mode to 0 and disable gt_train
                     break;
                  }
               }

               // Poll gate training mode bit for 0. 0-training done; 1-training in progress
               DEBUG_PRINTF("INFO: dqs_gate(): Poll for the gate training mode bit\n");
               gt_done = 1;
               for(phy_nibble = glbl_dqs_first_nibble; phy_nibble <= glbl_dqs_last_nibble; phy_nibble++) {
                  if(!((glbl_nibble_mask>>phy_nibble)&0x1)) {
                     gt_train = read_riu_nibble_reg(phy_nibble /*u8 nibble*/, DUP_NIBBLE_CTRL2 /*u8 reg_addr*/) & 0x40;
                     if(gt_train != 0) {
                        gt_done = 0;
                        break;
                     }
                  }
               }

               if(gt_done == 1) {
                  DEBUG_PRINTF("INFO: dqs_gate(): Gate training is done for all the nibbles. Waiting for the sequencer to complete\n");
                  while(readIO_DMC(ral_addr_of_seq_status_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x1);
                  // DEBUG_PRINTF("INFO: dqs_gate(): Gate training is done for all the nibbles. Clear the sequencer count\n");
                  // writeIO_DMC(ral_addr_of_seq_clr_in_ddrmc_main_ddrmc_main_bank(BLK),1);
                  // writeIO_DMC(ral_addr_of_seq_clr_in_ddrmc_main_ddrmc_main_bank(BLK),0);
               }
               if(iter > 64) { // Low frequencies need higher iterations
                  write_error_XRAM(ERR_CODE_90_SELF_REF_EXIT_DQS_GATE_TIMOUT);
                  write_error_nibble_XRAM(1, ~glbl_nibble_mask);
                  writeIO_XRAM(XRAM_CAL_RANK, (1<<rank | 1 << 4));
                  return 1;
               }
               iter++;
            }
         }
         glbl_gt_train_en = 0;

         // Check coarse overflow status
         //if overflow status == 1 then adjust the read latency by 3 tCK and restart the training with coarse = 0
         DEBUG_PRINTF("INFO: dqs_gate(): The gate training has stopped now \n");
         coarse_overflow = 0x0;
         for(nibble=0; nibble<DQS_BITS; nibble++) {
            phy_nibble = glbl_dqs_lnibble2pnibble_map[nibble];
            if(!((glbl_nibble_mask>>phy_nibble)&0x1)) {
               coarse_over_under_flow = read_riu_nibble_reg(phy_nibble /*u8 nibble*/, BS_RESET_CTRL/*u8 reg_addr*/) & 0xC000;
               coarse_overflow |= ((coarse_over_under_flow >> COARSE_OVERFLOW_POS) & 0x1) << nibble;

               if(coarse_over_under_flow==0) {
                  glbl_nibble_mask = glbl_nibble_mask | (1<<phy_nibble);
               } else {
                  read_latency   = readIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK)+phy_nibble*4);
                  writeIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK)+phy_nibble*4,read_latency+3);
                  write_riu_nibble_reg(phy_nibble /*u8 nibble*/, RLDLYRNK0 + rank /*u8 reg_addr*/, 0x8000);
               }
            }
         }
// XRAM_DBG: dqsgate_stg1_overflow
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_DQSGATE_STG1_OVERFLOW + (((rank * LOOP_LIMIT) + loop_cnt) * 2 * 4), coarse_overflow & 0x1FF);
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_DQSGATE_STG1_OVERFLOW + (((rank * LOOP_LIMIT) + loop_cnt) * 2 * 4) + 4, (coarse_overflow >> 9) & 0x1FF);
         DEBUG_PRINTF("INFO: dqs_gate(): Completed the dqs gate training for nibbles with mask = 0x%08X, loop_cnt = %d\n", glbl_nibble_mask, loop_cnt);
         loop_cnt++;
      } while((glbl_nibble_mask != 0xFFFFFFFF) && (loop_cnt <= LOOP_LIMIT));

      //if overflow status == 0 then training done
      if(glbl_nibble_mask == 0xFFFFFFFF) { // Completed successfully
         DEBUG_PRINTF("INFO: dqs_gate(): Gate training completed successfully\n");
      } else { // Reached limit, Failed
         DEBUG_PRINTF("ERROR: dqs_gate(): Gate training failed: Reached maximum read latency limit\n");
         if(restore_cal == 0) {
            write_error_XRAM(ERR_CODE_7_DQS_GATE_MAX_READ_LATENCY_LIMIT);
         } else {
            write_error_XRAM(ERR_CODE_91_SELF_REF_EXIT_DQS_GATE_MAX_READ_LATENCY_LIMIT);
         }
         write_error_nibble_XRAM(1, ~glbl_nibble_mask);
         writeIO_XRAM(XRAM_CAL_RANK, (1<<rank | 1 << 4));
         return 1;
      }

    #else // MCS_DQS_GATE // Perform the dqs gate training under control of the MCS
 
      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: dqs_gate(): Phase 2: Starts 3rd edge detection phase for rank = %d with nibble mask = 0x%08X using the coarse delay taps only \n", rank, glbl_dqs_nibble_mask_initial);
      DEBUG_PRINTF("##################################################################################################################### \n");

      writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK),command2compare_delay); // After the 3rd edge detection the read commands can be sent back to back.
      status = edge_detect_phase(rank, 2 /*u8 phase_num*/) ; 
      if(status)
         return 1;

      status = banks_refresh(rank, 0x10);
      if(status) {
         return 1;
      }

      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: dqs_gate(): Phase 2: Ends 3rd edge detection phase \n");
      DEBUG_PRINTF("##################################################################################################################### \n");

      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: dqs_gate(): Phase 3: Start finding Stable 0 before the 3rd edge for rank = %d with nibble mask = 0x%08X\n", rank, glbl_dqs_nibble_mask_initial);
      DEBUG_PRINTF("##################################################################################################################### \n");
      
   #ifdef LPDDR4
      writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK),6); // After the 3rd edge detection the read commands can be sent back to back. tCCD = 8
   #else
      writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK),2); // After the 3rd edge detection the read commands can be sent back to back.
   #endif
      status = edge_detect_phase(rank,  3 /*u8 phase_num*/) ; 
      if(status)
         return 1;
      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: dqs_gate(): Phase 3: Ends finding Stable 0 before the 3rd edge \n");
      DEBUG_PRINTF("##################################################################################################################### \n");
      
         
      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: dqs_gate(): Phase 4: Starts finding left edge of the noise of the 3rd edge using fine taps step size = %d jumps for rank = %d with nibble mask = 0x%08X\n", FINE_CRS_STEP_SIZE, rank, glbl_dqs_nibble_mask_initial);
      DEBUG_PRINTF("##################################################################################################################### \n");
      status = edge_detect_phase(rank,  4 /*u8 phase_num*/) ; 
      if(status)
         return 1;

      status = banks_refresh(rank, 0x10);
      if(status) {
         return 1;
      }

      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: dqs_gate(): Phase 4: Ends finding left edge of the noise of the 3rd edge \n");
      DEBUG_PRINTF("##################################################################################################################### \n");

      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: dqs_gate(): Phase 5: Starts finding noise width of the 3rd edge with step size  = 1 for rank = %d with nibble mask = 0x%08X\n", rank, glbl_dqs_nibble_mask_initial);
      DEBUG_PRINTF("##################################################################################################################### \n");
      status = edge_detect_phase(rank,  5 /*u8 phase_num*/) ; 
      if(status)
         return 1;
      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: dqs_gate(): Phase 5: Ends finding noise width of the 3rd edge \n");
      DEBUG_PRINTF("##################################################################################################################### \n");


      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: dqs_gate(): Phase 7: Starts adding extra read latency = %d, for for rank = %d with nibble mask = 0x%08X \n", 1,rank, glbl_dqs_nibble_mask_initial);
      DEBUG_PRINTF("##################################################################################################################### \n");
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dqs_nibble_mask_initial); 
      glbl_nibble_mask = glbl_dqs_nibble_mask_initial;
      //Send nos_of_gt_samples read commands for the opened bank at RD_CMD2GT_SAMPLE_DELAY interval
      riu_reg_bcast(BS_RESET_CTRL, 0, 0x1<<CLR_GATE_POS);
      bitslice_reset_bcast(0, BS_RESET_WT);
      writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), GT_SAMPLES<<16 | GT_SAMPLES);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      //writeIO_DMC(ral_addr_of_seq_rd_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), GT_SAMPLES);
      //while(readIO_DMC(ral_addr_of_seq_rd_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

      update_rnk_delay_sequential(rank /*u8 rank_offset*/,  COARSE /*u8 coares_fine_n*/, DEC /*int inc_dec_load*/, 4 /*u8 step_size*/, 0 /*u8 read_latency*/, 0/*u8 coarse_delay*/, 0/*u16 fine_delay*/); 
      riu_reg_bcast(BS_RESET_CTRL, 0, 0x1<<CLR_GATE_POS);
      bitslice_reset_bcast(0, BS_RESET_WT);
      //Send nos_of_gt_samples read commands for the opened bank at RD_CMD2GT_SAMPLE_DELAY interval
      writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), GT_SAMPLES<<16 | GT_SAMPLES);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      //writeIO_DMC(ral_addr_of_seq_rd_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), GT_SAMPLES);
      //while(readIO_DMC(ral_addr_of_seq_rd_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

      status = banks_refresh(rank, 0x10);
      if(status) {
         return 1;
      }

   #endif // Ends MCS_DQS_GATE 

// XRAM_DBG: dqsgate_stg1_read_lat
// XRAM_DBG: dqsgate_stg1_rldlyrnk_crse
// XRAM_DBG: dqsgate_stg1_rldlyrnk_fine
      for(nibble=0; nibble<DQS_BITS; nibble++) {
         phy_nibble = glbl_dqs_lnibble2pnibble_map[nibble];
         addr = ((rank * MAX_DQS_BITS) + nibble) * 4;
         read_latency                          = readIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK)+phy_nibble*4);
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_DQSGATE_STG1_READ_LAT + addr, read_latency);

         coarse_fine_delay                     = read_riu_nibble_reg(phy_nibble /*u8 nibble*/, RLDLYRNK0 + rank /*u8 reg_addr*/);
         coarse_delay                          = 0xF   & (coarse_fine_delay>>9);
         fine_delay                            = 0x1FF & (coarse_fine_delay);
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_DQSGATE_STG1_RLDLYRNK_CRSE + addr, coarse_delay);
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_DQSGATE_STG1_RLDLYRNK_FINE + addr, fine_delay);
      }

      writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), GT_SAMPLES<<16 | GT_SAMPLES);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

      nibble = 0;
      DEBUG_PRINTF("INFO: dqs_gate(): dqs_mode = %d\n", dqs_mode);
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dqs_nibble_mask_initial); 
      riu_reg_bcast(DUP_NIBBLE_CTRL2, 0, nibble_ctrl2); // Keeping dqs_mode back

      for(phy_nibble = glbl_dqs_first_nibble; phy_nibble <= glbl_dqs_last_nibble; phy_nibble++) {
         if(!((glbl_dqs_nibble_mask_initial>>phy_nibble)&0x1)) {
            coarse_fine_delay                     = read_riu_nibble_reg(phy_nibble /*u8 nibble*/, RLDLYRNK0 + rank /*u8 reg_addr*/);
            coarse_delay                          = 0xF   & (coarse_fine_delay>>9);
            fine_delay                            = 0x1FF & (coarse_fine_delay);
            read_latency                          = readIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK)+phy_nibble*4);

            if(dqs_mode == 3) { // -3/4 tCK: LP4 with toggle preamble
               if(coarse_delay >= 3) {
                  coarse_delay -= 3; // -3/4 tCK
	       } else {
                  coarse_delay += 1; // +1/4 tCK
                  read_latency -= 1; // -1 tCK
	       }
	    } else if ((dqs_mode == 2) || (dqs_mode == 4)) { // -1/2 tCK: LP4 with no-toggle preamble or DDR4 with 2 tCK preamble
               if(coarse_delay >= 2) {
                  coarse_delay -= 2; // -2/4 tCK
	       } else {
                  coarse_delay += 2; // +2/4 tCK
                  read_latency -= 1; // -1 tCK
	       }
	    }

            if(coarse_delay > 11) {
               coarse_delay = coarse_delay - 12;
               read_latency = read_latency+3;
            } else if (coarse_delay > 7) {
               coarse_delay = coarse_delay - 8;
               read_latency = read_latency+2;
            } else if (coarse_delay > 3) {
               coarse_delay = coarse_delay - 4;
               read_latency = read_latency+1;
            }
            coarse_fine_delay                     = (fine_delay & 0x01FF) | (coarse_delay<<9);
            write_riu_nibble_reg(phy_nibble /*u8 nibble*/, RLDLYRNK0 + rank /*u8 reg_addr*/, coarse_fine_delay|0x8000);
            writeIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK) + phy_nibble*4, read_latency);

            fine_delay_array[rank][nibble]        = fine_delay;
            coarse_delay_array[rank][nibble]      = coarse_delay;
            read_latency_array[rank][nibble]      = read_latency;
            DEBUG_PRINTF("INFO: dqs_gate(): Phase 7: For Rank = %d, Nibble = %d, Read latency = %d, Coarse taps = %d, Fine taps = %d \n", rank, nibble, read_latency_array[rank][nibble], coarse_delay_array[rank][nibble], fine_delay_array[rank][nibble] );
            nibble++;
         }
      }

// XRAM_DBG: dqsgate_stg2_read_lat
// XRAM_DBG: dqsgate_stg2_rldlyrnk_crse
      for(nibble=0; nibble<DQS_BITS; nibble++) {
         phy_nibble = glbl_dqs_lnibble2pnibble_map[nibble];
         addr = ((rank * MAX_DQS_BITS) + nibble) * 4;
         read_latency                          = readIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK)+phy_nibble*4);
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_DQSGATE_STG2_READ_LAT + addr, read_latency);

         coarse_fine_delay                     = read_riu_nibble_reg(phy_nibble /*u8 nibble*/, RLDLYRNK0 + rank /*u8 reg_addr*/);
         coarse_delay                          = 0xF   & (coarse_fine_delay>>9);
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_DQSGATE_STG2_RLDLYRNK_CRSE + addr, coarse_delay);
      }

      writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), GT_SAMPLES<<16 | GT_SAMPLES);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: dqs_gate(): Phase 7: Ends adding extra read latency = %d \n", 1);
      DEBUG_PRINTF("##################################################################################################################### \n");

   #ifdef LPDDR4
         //Relase the memory preamble training mode
         DEBUG_PRINTF("INFO: dqs_gate(): LPDDR4: Release the memory from Read preamble training mode \n");
         lp4_mode_reg_wr(rank, 13, glbl_lp4_mr13);

   #else
      if(glbl_lrdimm_en) {
         ddr_mrs7_write (rank_rcd, 0x1100 | DB_F0BC1X_PACKAGE_RANK_TIMING_ALIGNMENT_ENABLE | 0x0); // F0BC1X Register: Normal mode
      }

      // Release the memory from Preamble Training Mode
      DEBUG_PRINTF("INFO: dqs_gate(): Release the memory from Preamble Training Mode \n");
      set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                            MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr4 & MEM_DISABLE_PMB_TRAIN /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x0 /*u16 seq_a_ba*/,
                            0 /*u32 seq_a2a_dly*/);
      // Release the memory from MPR mode
      DEBUG_PRINTF("INFO: dqs_gate(): Release the memory from MPR Mode \n");
      set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                            MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr3 & MEM_DISABLE_MPR /*u32 seq_a_addr*/, 0x0 /*u8 seq_a_bg*/, 0x3 /*u16 seq_a_ba*/,
                            0 /*u32 seq_a2a_dly*/);

      //Close/Precharge the opened Bank
      DEBUG_PRINTF("INFO: dqs_gate(): Closing the opened row \n");
      set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                       MEM_ROW_PREACHARGE /*u32 seq_a_cmd*/, 0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/,
                       0 /*u32 seq_a2a_dly*/);
      
   #endif

      //Reset Command to GT status compare delay to default
      writeIO_DMC(ral_addr_of_seq_data_capture_in_ddrmc_main_ddrmc_main_bank(BLK),0); // - sel - rw - uint - [0] 0x0 - 0 - cal_seq - When set to 0, regular data is captured based on the data valid selection. 
                                                                  // When set to 1, data is captured based on the counter programmed in bits [6:1]. 
                                                                  // When seq_mode is set to 1 (WRLVL mode) it automatically turns on this mode also.
                                                                  // - delay_cnt - rw - uint - [6:1] 0x0 - 0 - cal_seq - Sets a delay in controller clock cycles after 
                                                                  // a command goes out that a flag gets generated to capture the data (either DQ for WRLVL mode, or GT_STATUS if selected). 
                                                                  // This allows capture of certain data at a programmable time.
   } // RANKS

   status = banks_refresh(0, 0x18);
   if(status) {
      return 1;
   }

   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: dqs_gate(): Phase 8: Starts adjusting read latency and the coarse taps delay of all the ranks \n");
   DEBUG_PRINTF("##################################################################################################################### \n");
   u8 max_latency = 0;
   for(nibble = 0; nibble < DQS_BITS; nibble++) {
      for(rank= 0; rank < RANKS; rank += glbl_rank_inc) {
         if(read_latency_array[rank][nibble] > max_latency) {
            max_latency = read_latency_array[rank][nibble]; 
         }
      }
   }

// XRAM_DBG: dqsgate_max_read_lat
   writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_DQSGATE_MAX_READ_LAT, max_latency);

   DEBUG_PRINTF("INFO: dqs_gate(): Final Phase: Maximum Read Latency = %d \n", max_latency);
   max_latency = (max_latency / 2) + 5; // Convert from tCK to mc_clk cycles and add some offset to consider internal timing
   writeIO_DMC(ral_addr_of_fifo_rden_in_ddrmc_main_ddrmc_main_bank(BLK),max_latency);
   DEBUG_PRINTF("INFO: dqs_gate(): Final Phase: FIFO read enable latency = %d \n", max_latency);

   #ifdef DDR4
   u8 min_latency;
   u8 center_coarse_update;
   glbl_error_nibble = 0;
   for(nibble = 0; nibble < DQS_BITS; nibble++) {
      min_latency = 255;
      for(rank= 0; rank < RANKS; rank += glbl_rank_inc) {
         if(read_latency_array[rank][nibble] < min_latency) {
            min_latency = read_latency_array[rank][nibble]; 
         }
      }
      center_coarse_update = 0;
      for(rank= 0; rank < RANKS; rank += glbl_rank_inc) {
         coarse_delay_array[rank][nibble] = (read_latency_array[rank][nibble] - min_latency)*4 + coarse_delay_array[rank][nibble];
         //read_latency_array[rank][nibble] = min_latency;
         // Earlier coarse values are limited to 0 to 3. Now limit coarses from 2 to 9 considering all the ranks of DDR4.
         // DDR4 rank to rank maximum skew is 4 (one tCK). lowest value can end up between 2 to 5 based on the phase.
         if(coarse_delay_array[rank][nibble] < 2) {
            center_coarse_update = 1;
         }
      }
      for(rank= 0; rank < RANKS; rank += glbl_rank_inc) {
         coarse_delay_array[rank][nibble] = coarse_delay_array[rank][nibble]+center_coarse_update*4;
         if(coarse_delay_array[rank][nibble]>=15) {
            DEBUG_PRINTF("ERROR: dqs_gate(): Final Phase: For Rank = %d, Logical Nibble = %d, Coarse taps = 0x%03X coarse delay overflow because of rank to rank skew is more than 2.5 tCK \n", rank, nibble, coarse_delay_array[rank][nibble]);
            glbl_error_nibble |= (0x1 << nibble);
         } else if(coarse_delay_array[rank][nibble]>=12) {
            DEBUG_PRINTF("WARNING: dqs_gate(): Final Phase: For Rank = %d, Logical Nibble = %d, Coarse taps = 0x%03X marginal coarse delay margin lesser than 1 tCK \n", rank, nibble, coarse_delay_array[rank][nibble]);
         }
      }
      read_latency_array[0][nibble] = min_latency-center_coarse_update;
   }
   if(glbl_error_nibble != 0) {
      write_error_nibble_XRAM(0, glbl_error_nibble);
      if(restore_cal == 0) {
         write_error_XRAM(ERR_CODE_8_DQS_GATE_HIGHER_RANK_2_RANK_SKEW);
      } else {
         write_error_XRAM(ERR_CODE_92_SELF_REF_EXIT_DQS_GATE_HIGHER_RANK_2_RANK_SKEW);
      }
      writeIO_XRAM(XRAM_CAL_RANK, (1<<rank | 1 << 4));
      return 1;
   }

   #else // LPDDR4
      #if RANKS == 2
         u8 rank_coarse_skew;
         glbl_error_nibble = 0;
         for(nibble = 0; nibble < DQS_BITS; nibble++) {
            // Earlier coarse values are limited to 0 to 3. Adjust them to have common read latency.
            DEBUG_PRINTF("INFO: dqs_gate(): Pre-Final Phase: Nibble = %d, rank 0 read latency = %d, coarse = %d, fine = %d, rank 1 read latency = %d, coarse = %d, fine = %d\n", nibble, read_latency_array[0][nibble], coarse_delay_array[0][nibble], fine_delay_array[0][nibble], read_latency_array[1][nibble], coarse_delay_array[1][nibble], fine_delay_array[1][nibble]);
            if(read_latency_array[1][nibble] > read_latency_array[0][nibble]) {
               coarse_delay_array[1][nibble] += ((read_latency_array[1][nibble] - read_latency_array[0][nibble]) * 4);
               read_latency_array[1][nibble] = read_latency_array[0][nibble];
            } else {
               coarse_delay_array[0][nibble] += ((read_latency_array[0][nibble] - read_latency_array[1][nibble]) * 4);
               read_latency_array[0][nibble] = read_latency_array[1][nibble];
            }

            if(coarse_delay_array[1][nibble] > coarse_delay_array[0][nibble]) {
               rank_coarse_skew = coarse_delay_array[1][nibble] - coarse_delay_array[0][nibble];
            } else {
               rank_coarse_skew = coarse_delay_array[0][nibble] - coarse_delay_array[1][nibble];
            }

            // Rank skew more than 9 coarse is not expected.
            if(rank_coarse_skew > 9) {
               DEBUG_PRINTF("ERROR: dqs_gate(): Final Phase: Nibble = %d, rank to rank skew is more than expected. rank 0 coarse = %d, rank 1 coarse = %d\n", nibble, coarse_delay_array[0][nibble], coarse_delay_array[1][nibble]);
               glbl_error_nibble |= (0x1 << nibble);
            }

            // Now coarse values need to be placed in the center of the coarse window (to accomodate for drift), 0 to 15.
            // LPDDR4 rank to rank maximum skew is 8 (1ns, two tCK at 4.267G). Let the sum of both rank coarse values be more than or equal to 12, to place them optimally.
            while((coarse_delay_array[0][nibble] + coarse_delay_array[1][nibble]) < 12) { // If sum is less than 12, add 4 coarses and reduce 1 read latency
               coarse_delay_array[0][nibble] += 4;
               coarse_delay_array[1][nibble] += 4;
               read_latency_array[0][nibble] -= 1;
               read_latency_array[1][nibble] -= 1;
            }
            DEBUG_PRINTF("INFO: dqs_gate(): Final Phase: Nibble = %d, rank 0 read latency = %d, coarse = %d, fine = %d, rank 1 read latency = %d, coarse = %d, fine = %d\n", nibble, read_latency_array[0][nibble], coarse_delay_array[0][nibble], fine_delay_array[0][nibble], read_latency_array[1][nibble], coarse_delay_array[1][nibble], fine_delay_array[1][nibble]);
         }
         if(glbl_error_nibble != 0) {
            write_error_nibble_XRAM(0, glbl_error_nibble);
            if(restore_cal == 0) {
               write_error_XRAM(ERR_CODE_8_DQS_GATE_HIGHER_RANK_2_RANK_SKEW);
            } else {
               write_error_XRAM(ERR_CODE_92_SELF_REF_EXIT_DQS_GATE_HIGHER_RANK_2_RANK_SKEW);
            }
            writeIO_XRAM(XRAM_CAL_RANK, (1<<rank | 1 << 4));
            return 1;
         }
      #else // RANKS == 1
         for(nibble = 0; nibble < DQS_BITS; nibble++) {
            // Earlier coarse values are limited to 0 to 3.
            DEBUG_PRINTF("INFO: dqs_gate(): Pre-Final Phase: Nibble = %d, rank 0 read latency = %d, coarse = %d, fine = %d\n", nibble, read_latency_array[0][nibble], coarse_delay_array[0][nibble], fine_delay_array[0][nibble]);
            // Now coarse values need to be placed in the center of the coarse window (to accomodate for drift), 0 to 15.
            while(coarse_delay_array[0][nibble] < 6) { // If less than 6, add 4 coarses and reduce 1 read latency
               coarse_delay_array[0][nibble] += 4;
               read_latency_array[0][nibble] -= 1;
            }
            DEBUG_PRINTF("INFO: dqs_gate(): Final Phase: Nibble = %d, rank 0 read latency = %d, coarse = %d, fine = %d\n", nibble, read_latency_array[0][nibble], coarse_delay_array[0][nibble], fine_delay_array[0][nibble]);
         }
      #endif
   #endif

   status = banks_refresh(0, 0x19);
   if(status) {
      return 1;
   }

// XRAM_DBG: dqsgate_read_lat_final
// XRAM_DBG: dqsgate_rldlyrnk_crse_final
   for(nibble=0; nibble<DQS_BITS; nibble++) {
      phy_nibble = glbl_dqs_lnibble2pnibble_map[nibble];
      read_latency                          = readIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK)+phy_nibble*4);
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_DQSGATE_READ_LAT_FINAL + nibble * 4, read_latency);

      for(rank = 0; rank < RANKS; rank += glbl_rank_inc) {  
         coarse_fine_delay                     = read_riu_nibble_reg(phy_nibble /*u8 nibble*/, RLDLYRNK0 + rank /*u8 reg_addr*/);
         coarse_delay                          = 0xF   & (coarse_fine_delay>>9);
         addr = ((rank * MAX_DQS_BITS) + nibble) * 4;
         writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_DQSGATE_RLDLYRNK_CRSE_FINAL + addr, coarse_delay);
      }
   }

   nibble = 0;
   RECORD_DLY("// Written from DQS_GATE_CAL function \n");
   RECORD_DLY("#define MAX_LATENCY %d \n", max_latency);
   RECORD_DLY("#define RL_DLY_RNK_VAL { ");
   for(phy_nibble = glbl_dqs_first_nibble; phy_nibble <= glbl_dqs_last_nibble; phy_nibble++) {
      if(!((glbl_dqs_nibble_mask_initial>>phy_nibble)&0x1)) {
         RECORD_DLY("{");
         for(rank= 0; rank < 4; rank++) {
            if(glbl_lrdimm_en) {
               rank_rcd = (SLOTS <= 1) ? 0 : ((rank / (RANKS/SLOTS)) * (RANKS/SLOTS));
               coarse_fine_delay                     = (fine_delay_array[rank_rcd][nibble] & 0x1FF) | ((coarse_delay_array[rank_rcd][nibble]<<9) & 0xFE00);
            } else
               coarse_fine_delay                     = (fine_delay_array[rank%RANKS][nibble] & 0x1FF) | ((coarse_delay_array[rank%RANKS][nibble]<<9) & 0xFE00);

	    if(rank == 3) // Set update for the last rank which results in updating all the ranks
               write_riu_nibble_reg(phy_nibble /*u8 nibble*/, RLDLYRNK0 + rank /*u8 reg_addr*/, coarse_fine_delay|0x8000);
	    else
               write_riu_nibble_reg(phy_nibble /*u8 nibble*/, RLDLYRNK0 + rank /*u8 reg_addr*/, coarse_fine_delay|0x0000);
            DEBUG_PRINTF("INFO: dqs_gate(): Final Phase: For Rank = %d, Logical Nibble = %d, Physical Nibble =%d, Coarse taps = 0x%03X, Fine taps = 0x%03X, coarse_fine_delay = 0x%03X\n", rank, nibble, phy_nibble, (coarse_fine_delay >> 9) & 0xF, coarse_fine_delay & 0x1FF, coarse_fine_delay);
            if(rank<3) {
               RECORD_DLY(" 0x%04X,",coarse_fine_delay|0x8000);
            } else {
               RECORD_DLY(" 0x%04X",coarse_fine_delay|0x8000);
            }
         }
         if(phy_nibble<glbl_dqs_last_nibble) {
            RECORD_DLY("},");
         } else { 
            RECORD_DLY("} }\n");
         }
         nibble++;
      }
   }

   nibble = 0;
   for(phy_nibble = glbl_dq_first_nibble; phy_nibble <= glbl_dq_last_nibble; phy_nibble++) {
      if(!((glbl_dq_nibble_mask_initial>>phy_nibble)&0x1)) {
         if(NIBBLE_PER_DQS == 2)
            writeIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK) + phy_nibble*4, read_latency_array[0][nibble/2]);
         else
            writeIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK) + phy_nibble*4, read_latency_array[0][nibble]);
         nibble++;
      }
   }

   nibble = 0;
   RECORD_DLY("#define PHY_RDEN_VAL {");
   for(phy_nibble = glbl_dqs_first_nibble; phy_nibble <= glbl_dqs_last_nibble; phy_nibble++) {
      if(!((glbl_dqs_nibble_mask_initial>>phy_nibble)&0x1)) {
         // writeIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK) + phy_nibble*4, read_latency_array[0][nibble]);
         if(phy_nibble<glbl_dqs_last_nibble) {
            RECORD_DLY("%d,",read_latency_array[0][nibble]);
         } else { 
            RECORD_DLY("%d}",read_latency_array[0][nibble]);
         }
         DEBUG_PRINTF("INFO: dqs_gate(): Final Phase: For Rank = %d, Nibble = %d, Read latency = %d \n", rank, nibble, read_latency_array[0][nibble]);
         nibble++;
      }
   }
   RECORD_DLY("\n //Recorded the delays\n");
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: dqs_gate(): Phase 8: Ends adjusting read latency and the coarse taps delay of all the ranks \n");
   DEBUG_PRINTF("##################################################################################################################### \n");

   writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial); 
   riu_reg_bcast(BS_RESET_CTRL, 0, 0x1<<CLR_GATE_POS);
   bitslice_reset_bcast(0, BS_RESET_WT);

   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: dqs_gate(): DQS GATE CALIBRATION END \n");
   DEBUG_PRINTF("##################################################################################################################### \n");

#endif

#ifdef TEST_MPR
   volatile u32 rd_data[DQS_BITS*NIBBLE_PER_DQS];
   volatile u32 rd_data_1[DQS_BITS*NIBBLE_PER_DQS];
   volatile u32 rd_data_2[DQS_BITS*NIBBLE_PER_DQS];
   volatile u32 rd_data_3[DQS_BITS*NIBBLE_PER_DQS];
   u8 read_mpr;
   read_mpr = 0;

    // Before starting the Read calibration put the memory in the MPR Mode to read the data pattern from MPR0
       set_1_seq_command_at_mem(0 /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/,
                             MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr3 | MEM_ENABLE_MPR /*u32 seq_a_addr*/, 0x0 /*u8 seq_a_bg*/, 0x3 /*u16 seq_a_ba*/,
                             0 /*u32 seq_a2a_dly*/);

while(read_mpr == 0) {
    // Clearing the Compare Status
    writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);
    writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

    writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
    riu_reg_bcast(BS_RESET_CTRL, 0, 0x1<<CLR_GATE_POS);
    bitslice_reset_bcast(0, BS_RESET_WT);

    // Set MPR read command
    rank = 0;
    writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),MEM_COL_READ);
    writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK),0);
    writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK),0);
    writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK),0);
    writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK),4);
    writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK),rank); // Selects xphy rank
    writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),0x10<<rank); // Selects DDR Mem rank

    // MPR0
    flush_xphy_fifo();
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
    riu_reg_bcast(BS_RESET_CTRL, 0, 0x1<<CLR_GATE_POS);
    bitslice_reset_bcast(0, BS_RESET_WT);

    writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1 | (1<<16));
    writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK),0);
    writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
    while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

    for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
       rd_data[nibble] = readIO_DMC(ral_addr_of_bl8_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (nibble * 4));
       writeIO_XRAM(0x6000 + nibble*4, rd_data[nibble]);
       writeIO_MBDR(RAM_DATA_CAL_OFFSET + 0x600 + (nibble*4), rd_data[nibble]);
    }

    // MPR1
    flush_xphy_fifo();
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
    riu_reg_bcast(BS_RESET_CTRL, 0, 0x1<<CLR_GATE_POS);
    bitslice_reset_bcast(0, BS_RESET_WT);

    writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1 | (1<<16));
    writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK),1);
    writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
    while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

    for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
       rd_data_1[nibble] = readIO_DMC(ral_addr_of_bl8_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (nibble * 4));
       writeIO_XRAM(0x6100 + nibble*4, rd_data_1[nibble]);
       writeIO_MBDR(RAM_DATA_CAL_OFFSET + 0x700 + (nibble*4), rd_data_1[nibble]);
    }

    // MPR2
    flush_xphy_fifo();
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
    riu_reg_bcast(BS_RESET_CTRL, 0, 0x1<<CLR_GATE_POS);
    bitslice_reset_bcast(0, BS_RESET_WT);

    writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1 | (1<<16));
    writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK),2);
    writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
    while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

    for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
       rd_data_2[nibble] = readIO_DMC(ral_addr_of_bl8_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (nibble * 4));
       writeIO_XRAM(0x6200 + nibble*4, rd_data_2[nibble]);
       writeIO_MBDR(RAM_DATA_CAL_OFFSET + 0x800 + (nibble*4), rd_data_2[nibble]);
    }

    // MPR3
    flush_xphy_fifo();
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
    riu_reg_bcast(BS_RESET_CTRL, 0, 0x1<<CLR_GATE_POS);
    bitslice_reset_bcast(0, BS_RESET_WT);

    writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1 | (1<<16));
    writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK),3);
    writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
    while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

    for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
       rd_data_3[nibble] = readIO_DMC(ral_addr_of_bl8_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (nibble * 4));
       writeIO_XRAM(0x6300 + nibble*4, rd_data_3[nibble]);
       writeIO_MBDR(RAM_DATA_CAL_OFFSET + 0x900 + (nibble*4), rd_data_3[nibble]);
    }

    read_mpr = readIO_MBDR(RAM_DATA_CAL_OFFSET);
}

    // Disable MPR mode
    set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/,
                          MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr3 & MEM_DISABLE_MPR /*u32 seq_a_addr*/, 0x0 /*u8 seq_a_bg*/, 0x3 /*u16 seq_a_ba*/,
                          0 /*u32 seq_a2a_dly*/);
#endif

   status = banks_refresh(0, 0x1A);
   if(status) {
      return 1;
   }

   return 0;
}

void flush_xphy_fifo() {
    writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 16 | (16<<16));
    writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK),3);
    writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
    while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
}

/*********************************************************************************************************************************************/
// Function definations
/*********************************************************************************************************************************************/

     void collect_gt_sample (u32 *dqs_received_pattern) {
        //u8 nibble;
        u8 nibble;
        //u8 sample_cnt;
        //u16 read_data;
        u32 dqs_0_12_gt_status = 0;
        u32 dqs_13_26_gt_status = 0; 
        //u32 dqs_15_0_gt_status = 0; 
        //u32 dqs_17_16_gt_status = 0; 

        riu_reg_bcast(BS_RESET_CTRL, 0, 0x1<<CLR_GATE_POS);
        bitslice_reset_bcast(0, BS_RESET_WT);
        writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),1);  //- 32 - STATUS - Assert clear comparison during calibration
        writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0);  //- 32 - STATUS - Release clear comparison during calibration

        //Send nos_of_gt_samples read commands for the opened bank at RD_CMD2GT_SAMPLE_DELAY interval
        writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), GT_SAMPLES<<16 | GT_SAMPLES);
        writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
        //writeIO_DMC(ral_addr_of_seq_rd_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), GT_SAMPLES);
        //while(readIO_DMC(ral_addr_of_seq_rd_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
        while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

        // Read GT samples of lower 8 data nibbles
        dqs_0_12_gt_status = readIO_DMC(ral_addr_of_compare_gt_status0_stable_in_ddrmc_main_ddrmc_main_bank(BLK));
                                                                         // - val - rw - uint - [17:0] 0x0 - 0 - cal_cmpr - TRUE - 2 bits per nibble. 
                                                                         // Nibbles 0 - 8. 2'b00: RESET Value 2'b01: Stable 1 2'b10: Stable 0 2'b11: Unstable
        // Read GT samples of upper 8 data nibbles
        dqs_13_26_gt_status = readIO_DMC(ral_addr_of_compare_gt_status1_stable_in_ddrmc_main_ddrmc_main_bank(BLK)); //- 32 - STATUS - Comparison of error per bit in a nibble
                                                                         // - val - rw - uint - [17:0] 0x0 - 0 - cal_cmpr - TRUE - 2 bits per nibble. 
                                                                         // Nibbles 9 - 17. 2'b00: RESET Value 2'b01: Stable 1 2'b10: Stable 0 2'b11: Unstable
        //dqs_15_0_gt_status = (dqs_9_17_gt_status<<18) | dqs_0_8_gt_status;
        //dqs_17_16_gt_status = dqs_9_17_gt_status>>14; 
        //for(sample_cnt = 0; sample_cnt < GT_SAMPLES; sample_cnt++) {
        //   //Send one read command
        //   set_1_seq_command_at_mem(0 /*u8 rank*/, 0x10001 /*u32 nos_iteration_of_seqs*/, 
        //                    MEM_COL_READ /*u32 seq_a_cmd*/, 0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/,
        //                    0/*u32 seq_a2a_dly*/);
        //   //Wait until all read commnds are executed
        //   while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

        //   // Read GT Status
        //   dqs_index = 0;
        //   for(nibble = glbl_dqs_first_nibble; nibble <= glbl_dqs_last_nibble; nibble++) {
        //      if(!((glbl_dqs_nibble_mask_initial>>nibble) & 0x1)) {
        //         if(!((glbl_nibble_mask >> nibble) & 0x1)) {
        //            read_data = read_riu_nibble_reg(nibble, BS_RESET_CTRL);
        //            read_data = (read_data & 0x100);
        //            if(read_data)
        //               dqs_15_0_gt_status = dqs_15_0_gt_status | (1<<(2*dqs_index));
        //            else
        //               dqs_15_0_gt_status = dqs_15_0_gt_status | (2<<(2*dqs_index));
        //            //write_riu_nibble_reg(nibble, BS_RESET_CTRL,0x1); // CLR_GATE ENABLE
        //            //write_riu_nibble_reg(nibble, BS_RESET_CTRL,0x0); // CLR_GATE DISABLE
        //         };
        //         dqs_index++;
        //     }
        //   }
        //}

        DEBUG_PRINTF("INFO: CALIBRATION_MODULE: collect_gt_sample(): gt_samples of nibbles [8:0] = 0x%08X and of nibbles [17:9] = 0x%08X \n",dqs_0_12_gt_status, dqs_13_26_gt_status);  

        //for(phy_nibble = glbl_dqs_first_nibble, nibble=0 ; phy_nibble <= glbl_dqs_last_nibble; phy_nibble++) {
        for(nibble=0; nibble < 27; nibble++) {
           if(nibble < 13) {
              dqs_received_pattern[nibble] = ( dqs_received_pattern[nibble] << 2 ) | ( (dqs_0_12_gt_status  >> nibble*2) & 0x3 );
           } else {
              dqs_received_pattern[nibble] = ( dqs_received_pattern[nibble] << 2 ) | ( (dqs_13_26_gt_status >> (nibble-13)*2) & 0x3 );
           }
        } 
     }

     void update_rnk_delay_sequential(u8 rank_offset, u8 coares_fine_n, int inc_dec_load, u8 step_size, u8 read_latency, u8 coarse_delay, u16 fine_delay) {
        u16 combined_delay;
        u16 coarse_fine_delay;
        u8  nibble;
        //u8  nibble;
        //u32 read_data;
        //nibble = 0;
        for(nibble = glbl_dqs_first_nibble; nibble <= glbl_dqs_last_nibble ; nibble++) {
           if(!(   (glbl_nibble_mask>>nibble)  &    0x1   )) {
              if(inc_dec_load != LOAD) {
                 coarse_fine_delay = read_riu_nibble_reg(nibble /*u8 nibble*/, RLDLYRNK0 + rank_offset /*u8 reg_addr*/);
                 coarse_delay      = 0xF   & (coarse_fine_delay>>9);
                 fine_delay        = 0x1FF & coarse_fine_delay;
                 read_latency      = readIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK)+nibble*4);
                 combined_delay    = coarse_delay + (read_latency << 2);
                 if(coares_fine_n == COARSE)
                    combined_delay    = combined_delay + (inc_dec_load*step_size);
                 else if (coares_fine_n == FINE)
                    fine_delay        = fine_delay + (inc_dec_load*step_size);
                 else {
                    combined_delay    = combined_delay + (inc_dec_load*step_size);
                    fine_delay        = fine_delay + (inc_dec_load*step_size);
                 }
                 coarse_delay      = combined_delay & 0x3;
                 read_latency      = combined_delay>>2;
              } 
              coarse_fine_delay = (coarse_delay << 9) | (fine_delay & 0x1FF);
              DEBUG_PRINTF("INFO: CALIBRATION_MODULE: update_rnk_delay_sequential(): Physical Nibble = %d, Read Latency = %d, Coarse = %d, Fine = %d \n",nibble, read_latency, coarse_delay, fine_delay);
              write_riu_nibble_reg(nibble /*u8 nibble*/, RLDLYRNK0+ rank_offset /*u8 reg_addr*/, coarse_fine_delay|0x8000);
              writeIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4, read_latency);
           }
        }
     }

      int edge_detect_phase(u8 rank,  u8 phase_num) { 
         u8 nibble;
         u32 dqs_received_pattern[27];
         u8  noise_width[27];
         u32 dqs_exp_pattern;
         u32 dqs_exp_pattern_mask;
         initialize_u32_array (dqs_received_pattern /*u32 *array_start_addr*/, 27 /*u8 array_size*/, 0 /*u32 init_value*/);
         initialize_u8_array (noise_width /*u8 *array_start_addr*/, 27 /*u8 array_size*/, 0 /*u8 init_value*/);
         u32 noise_width_inc_en = 0;
         //u32 noise_width_inc_mask = 0xFFFFFFFF;
         u32 phase_done = 0;
         u16 fine_delay = 0;
         u16 coarse_delay = 0;
         u16 rl_rnk_dly = 0;
         u16 read_latency = glbl_initial_read_latency;
         u16 rd_latency_crs_taps_combo = (glbl_initial_read_latency << 2);
         glbl_nibble_mask = glbl_dqs_nibble_mask_initial;
         u8  quater_taps            = 20; 
         u8  read_latency_limit     = glbl_initial_read_latency + 40;
         u8  tap_count;
         u8  phase3_iteration=0;  
         u16 fine_tap_iteration=0;
         u16 iteration =0;

         if(phase_num == 2) { // Detect 3rd edge
            dqs_exp_pattern      = GT_ACCUMULATED_PAT_SAMPLE_3RD_EDGE;
            dqs_exp_pattern_mask = GT_ACCUMULATED_PAT_SAMPLE_3RD_EDGE_MASK;
         } else if(phase_num == 3) { // Confirm Stable 0 before the 3rd edge
            dqs_exp_pattern      = GT_STATUS_B2B_STABLE_ZERO;
            dqs_exp_pattern_mask = GT_STABLE_0_PAT_BEFORE_3RD_EDGE_MASK;
         } else if(phase_num == 4) { // Find stable 0 to Noise trasaction using coarse fine taps
            dqs_exp_pattern = GT_STATUS_STABLE_ONE;
            dqs_exp_pattern_mask = GT_NOISE_PAT_BEFORE_3RD_EDGE_MASK;
         } else if(phase_num == 5) { // Find Noise width
            dqs_exp_pattern      = GT_STATUS_B2B_STABLE_ONE;
            dqs_exp_pattern_mask = GT_STABLE_1_PAT_AFTER_3RD_EDGE_MASK;
         }
        if(phase_num == 5) { // Detect noise window width using fine taps of step size equal to 1.
            // Before starting the phase 5 revert all the delay taps by the bigger step size of the phase 4.
            // Mask all the nibbles which doesn't need to be calibrated
            writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask); 
            riu_reg_bcast( RLDLYUPDATE,0,(0x1<<RLDLY_DEC_POS)|(0x1<<RLDLY_CRSE_EN_POS)|(RLDLY_RNK_MASK_MASK & (~(0x1<<(RLDLY_RNK_MASK_POS+rank)))) ); 
         }
         do {
            DEBUG_PRINTF("################ Phase %d iteration no %d ################## \n", phase_num, iteration);
            DEBUG_PRINTF("INFO: CALIBRATION_MODULE: dqs_gate.edge_detect_phase(): expected pattern wmask = 0x%08X, expected pattern mask = 0x%08X \n", dqs_exp_pattern, dqs_exp_pattern_mask);
            iteration++;
            //Mask all the nibbles which doesn't need to be calibrated
            writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask); 
            if(phase_num == 2) { // Detect 3rd edge using coarse taps
               //Broadcast coarse and fine delay to all the nibbles, which are getting calibrated.
               riu_reg_bcast(RLDLYRNK0+rank,0,rl_rnk_dly|0x8000); 
               // Broadcast sequentially read latency register of the nibbles which are getting calibrated.
               for(nibble = glbl_dqs_first_nibble; nibble <= glbl_dqs_last_nibble; nibble++) {
                  if(!((glbl_nibble_mask>>nibble)&0x1)) {
                     writeIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4, read_latency);
                  }
               }
               collect_gt_sample ( dqs_received_pattern /*u16 *dqs_received_pattern*/); 
            } else if(phase_num == 3) { // Confirm stable 0 before the 3rd edge
               // Revert coarse delay by 1 step
               update_rnk_delay_sequential(rank /*u8 rank_offset*/,  COARSE /*u8 coares_fine_n*/, DEC /*int inc_dec_load*/, 1 /*u8 step_size*/, read_latency /*u8 read_latency*/, coarse_delay /*u8 coarse_delay*/, fine_delay /*u16 fine_delay*/); 
               // Incrment fine taps step by step till GT_STATUS_B2B_STABLE_ONE. And collect gt status of each sample. 
               // Set coarse step size of the fine delays
               riu_reg_bcast(INCDEC_CRSE,0,0x1); 
               for(tap_count = 0; tap_count < GT_STATUS_B2B_STABLE_ZERO_COUNT; tap_count++) {
                  // Broadcast incrment of fine delay to all the nibbles, which are getting calibrated.
                  riu_reg_bcast( RLDLYUPDATE,0,(0x1<<RLDLY_INC_POS)|(0x1<<RLDLY_CRSE_EN_POS)|(RLDLY_RNK_MASK_MASK & (~(0x1<<(RLDLY_RNK_MASK_POS+rank)))) ); 
                  // Collect the status of the GT samples
                  collect_gt_sample (    dqs_received_pattern /*u16 *dqs_received_pattern*/); 
               }
               phase3_iteration++;
               if(phase3_iteration == 3) 
                  return 1;
            } else if(phase_num == 4) { // Detect Stable 0 to Noise transaction using bigger jumps of the fine taps
               // Set coarse step size of the fine delays
               riu_reg_bcast(INCDEC_CRSE,0, FINE_CRS_STEP_SIZE);
               // Broadcast incrment of fine delay of bigger step size to all the nibbles, which are getting calibrated.
               riu_reg_bcast( RLDLYUPDATE,0,(0x1<<RLDLY_INC_POS)|(0x1<<RLDLY_CRSE_EN_POS)|(RLDLY_RNK_MASK_MASK & (~(0x1<<(RLDLY_RNK_MASK_POS+rank)))) ); 
               fine_tap_iteration=fine_tap_iteration+FINE_CRS_STEP_SIZE;
               if(fine_tap_iteration > 500)
                  return 1;
               collect_gt_sample (    dqs_received_pattern /*u16 *dqs_received_pattern*/); 
            } else if(phase_num == 5) { // Detect noise window width using fine taps of step size equal to 1.
               // Set coarse step size of the fine delays
               riu_reg_bcast(INCDEC_CRSE,0,0x1); 
               // Broadcast decrment of fine delay to all the nibbles, which are getting calibrated.
               riu_reg_bcast( RLDLYUPDATE,0,(0x1<<RLDLY_INC_POS)|(0x1<<RLDLY_CRSE_EN_POS)|(RLDLY_RNK_MASK_MASK & (~(0x1<<(RLDLY_RNK_MASK_POS+rank)))) ); 
               fine_tap_iteration=fine_tap_iteration+1;
               if(fine_tap_iteration > 500)
                  return 1;
               collect_gt_sample (    dqs_received_pattern /*u16 *dqs_received_pattern*/); 
            } 
            DEBUG_PRINTF("INFO: CALIBRATION_MODULE: dqs_gate.edge_detect_phase(): nibble_mask = 0x%08X, phase_done = 0x%08X \n", glbl_nibble_mask, phase_done);
            for(nibble = glbl_dqs_first_nibble; nibble <= glbl_dqs_last_nibble; nibble++) {
                  if(!((glbl_nibble_mask >> nibble) & 0x1)) { 
                     DEBUG_PRINTF("INFO: CALIBRATION_MODULE: dqs_gate.edge_detect_phase(): dqs_received_pattern = 0x%08X , dqs_received_pattern wmask = 0x%08X for Physical Nibble = %d \n",dqs_received_pattern[nibble], dqs_received_pattern[nibble] & dqs_exp_pattern_mask, nibble);
                     if((dqs_received_pattern[nibble] & dqs_exp_pattern_mask) == dqs_exp_pattern) {
                        DEBUG_PRINTF("INFO: CALIBRATION_MODULE: dqs_gate.edge_detect_phase(): Detected pattern for Physical Nibble = %d, Read Latency = %d, Coarse = %d, Fine = %d \n",nibble, read_latency, coarse_delay, fine_delay);
                        phase_done = (phase_done | 1<<nibble);
                     }
                  } 
            }
            
            /*************************************************************************************************/   
            // Mask the nibbles whose phase has been completed 
            /*************************************************************************************************/   
            glbl_nibble_mask = glbl_nibble_mask | phase_done;
            /*************************************************************************************************/   
            /*************************************************************************************************/   

     
            if(phase_num == 2) {// Detect 3rd edge using coarse taps
               /*************************************************************************************************/   
               // Incrment coarse delay
               /*************************************************************************************************/   
               rd_latency_crs_taps_combo++; 
               // Repeate the loop with new fine taps offset if it is not able to find the pattern with the read latency limit
               if(read_latency == read_latency_limit) {
                  DEBUG_PRINTF("##################################################################################################################### \n");
                  DEBUG_PRINTF("INFO: CALIBRATION_MODULE: dqs_gate.edge_detect_phase(): Reached Read latency limit = %d and re-iterating the 3rd edge detect phase with Fine taps offset = %d for rank = %d, with nibbles mask = 0x%08X\n", read_latency, fine_delay, rank, glbl_nibble_mask);
                  DEBUG_PRINTF("##################################################################################################################### \n");
                  rd_latency_crs_taps_combo = (glbl_initial_read_latency << 2);
                  fine_delay   = quater_taps;
                  quater_taps  = quater_taps/2;
                  initialize_u32_array (dqs_received_pattern /*u32 *array_start_addr*/, 27 /*u8 array_size*/, 0 /*u32 init_value*/);
                  // Return error if it is not able to find the expected pattern with the offset
                  if(quater_taps == 1) {
                     DEBUG_PRINTF("##################################################################################################################### \n");
                     DEBUG_PRINTF("INFO: CALIBRATION_MODULE: dqs_gate.edge_detect_phase(): FAILED DQS GATE 3rd EDGE DETECT Phase: Reached fine delay offset limit = %d rank = %d, with nibbles mask = 0x%08X\n", fine_delay, rank, glbl_nibble_mask);
                     DEBUG_PRINTF("##################################################################################################################### \n");
                     return 1;
                  }
               }
               coarse_delay = rd_latency_crs_taps_combo & 0x3;
               read_latency = rd_latency_crs_taps_combo>>2;
               rl_rnk_dly = (coarse_delay << 9) | (fine_delay&0x1FF);
               DEBUG_PRINTF("INFO: CALIBRATION_MODULE: dqs_gate.edge_detect_phase(): Max: read latency = % d, coarse delay = %d, fine delay = %d \n", read_latency, coarse_delay, fine_delay);
               /*************************************************************************************************/   
               /*************************************************************************************************/   
            }   
            if(phase_num == 3) { // Confirm stable 0 before the 3rd edge
               // Set coarse step size of the fine delays
               riu_reg_bcast(INCDEC_CRSE,0,GT_STATUS_B2B_STABLE_ZERO_COUNT);
               // Revert fine taps for the nibbles whose stable 0 has not been found
               //Mask all the nibbles which doesn't need to be calibrated
               writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask); 
               //Broadcast decrment of fine delay to all the nibbles, which are getting calibrated.
               riu_reg_bcast( RLDLYUPDATE,0,(0x1<<RLDLY_DEC_POS)|(0x1<<RLDLY_CRSE_EN_POS)|(RLDLY_RNK_MASK_MASK & (~(0x1<<(RLDLY_RNK_MASK_POS+rank)))) ); 
            } else if (phase_num == 5) { // Detect noise window width using fine taps of step size equal to 1.
               for(nibble = glbl_dqs_first_nibble; nibble <= glbl_dqs_last_nibble; nibble++) {
                     if(!((glbl_nibble_mask >> nibble) & 0x1)) { 
                        if ((dqs_received_pattern[nibble] & 0x3) != GT_STATUS_STABLE_ZERO) {
                           noise_width_inc_en = noise_width_inc_en | (1<<nibble);
                           DEBUG_PRINTF("INFO: dqs_gate.edge_detect_phase(): Phase 5: Incrment noise width for physical nibble %d \n", nibble);
                        }
                     } else {
                        noise_width_inc_en = noise_width_inc_en & (~(1<<nibble));
                        DEBUG_PRINTF("INFO: dqs_gate.edge_detect_phase(): Phase 5: Masked incrment noise width for physical nibble %d \n", nibble);
                     } 
               }
               // Increase noise width counter for the nibbles whose inc is unmasked 
               inc_u8_array (noise_width /*u8 *array_start_addr*/, 27 /*u8 array_size*/, ~noise_width_inc_en/*u32 mask*/, 1 /* u8 step_size*/); 
            }
         } while (glbl_nibble_mask != 0xFFFFFFFF);
         if(phase_num == 5) {
            DEBUG_PRINTF("##################################################################################################################### \n");
            DEBUG_PRINTF("INFO: dqs_gate.edge_detect_phase(): Phase 6: Start Centering in the noise region of the 3rd edge for rank = %d with nibble mask = 0x%08X\n", rank, glbl_dqs_nibble_mask_initial);
            DEBUG_PRINTF("##################################################################################################################### \n");
            center_align (glbl_dqs_first_nibble /*u8 first_element*/, glbl_dqs_last_nibble /*u8 last_element*/, glbl_dqs_nibble_mask_initial /*u32 elements_mask*/, RLDLYRNK0/*u32 delay_offset*/, rank /*u8 rank*/, noise_width /*u8 *window_width*/);  
            DEBUG_PRINTF("##################################################################################################################### \n");
            DEBUG_PRINTF("INFO: dqs_gate.edge_detect_phase(): Phase 6: Ends Centering in the noise region of the 3rd edge \n");
            DEBUG_PRINTF("##################################################################################################################### \n");
        }
        return 0;
      }
