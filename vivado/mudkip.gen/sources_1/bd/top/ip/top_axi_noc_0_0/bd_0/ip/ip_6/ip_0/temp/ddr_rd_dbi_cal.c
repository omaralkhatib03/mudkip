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
//       Revision:       $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/ddr_rd_dbi_cal.c#1 $
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

#include "cal_reg_define.h"
#include "riu_reg_define.h"
#include "xsdb_reg_define.h"
#include "internal_reg_define.h"
#include "function_dec.c"
#include "global_variable_define.h"


/**************** End Used defines **************************/
u8 ddr_rd_dbi_cal(void) {

#ifdef DDR4
   if(!(glbl_mr5[0] & (0x1 << 12))) {
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: DDR4 ddr_rd_dbi_cal(): Read DBI is disabled in the MR5[12] so no need to do the Read DBI Calibration \n");
      return 0;
   }
#else
   if(!(glbl_lp4_mr3 & (0x1 << 6))) {
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: LPDDR4 ddr_rd_dbi_cal(): Read DBI is disabled in the MR3[6] so no need to do the Read DBI Calibration \n");
      return 0;
   }
#endif

#ifdef SKIP_RD_DBI_CAL_STAGE
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: rd_dbi_cal():  READ DBI CALIBRATION SKIPPED \n");
   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: rd_dbi_cal(): Loading precalculated delay values to skip the READ DBI CALIBRATION \n");
   DEBUG_PRINTF("##################################################################################################################### \n");
   writeIO_DMC(ral_addr_of_xpi_read_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_xpi_read_dbi); 
   skip_ddr_rd_dbi_cal();
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: rd_cal(): Loaded precalculated delay values to skip the READ DBI CALIBRATION \n");
   DEBUG_PRINTF("##################################################################################################################### \n");
#else
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_rd_dbi_cal(): READ DBI CALIBRATION START \n");
   DEBUG_PRINTF("##################################################################################################################### \n");

   u8  step_size_coarse       = 10;
   u8  nos_of_samples         = 10;

   u8  nibble;
   u8  bit_cnt;
   u8  rank;
   u8  status;
   u16 iteration;
   u8  read_latency;
   u16 steps;
   int i;

   glbl_rd_dbi_en = 1;

   // Enable read DBI mode in the cal logic.
   DEBUG_PRINTF("INFO: rd_dbi_cal(): Enable read DBI mode in DDRMC\n");
   writeIO_DMC(ral_addr_of_xpi_read_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_xpi_read_dbi); 

   #ifdef LPDDR4
      DEBUG_PRINTF("INFO: rd_dbi_cal(): Enable read DBI mode in DRAM\n");
      for (rank = 0; rank < RANKS; rank++) {
         // Write MPR register to Enable the Read DBI mode into the DRAM.
         lp4_mode_reg_wr(rank, 3, glbl_lp4_mr3);
      }

      // Adjust read latency as per read DBI offset
      u8 temp_nibble;
      for(nibble = 0; nibble<DQ_NIBBLES;nibble++) { //glbl_dqs_first_nibble; nibble <= glbl_dqs_last_nibble ; nibble++) {
         read_latency      = readIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK)+glbl_dq_lnibble2pnibble_map[nibble]*4);
         DEBUG_PRINTF("INFO: rd_dbi_cal() Read latency of nibble number %d without dbi is  = %d\n", glbl_dq_lnibble2pnibble_map[nibble], read_latency);
         writeIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK) + glbl_dq_lnibble2pnibble_map[nibble]*4, read_latency+glbl_mem_read_latency_dbi_offset);
         DEBUG_PRINTF("INFO: rd_dbi_cal() Read latency of nibble number %d with dbi is  = %d\n", glbl_dq_lnibble2pnibble_map[nibble], read_latency+glbl_mem_read_latency_dbi_offset);
      }
      u8 fifo_rd_latency = readIO_DMC(ral_addr_of_fifo_rden_in_ddrmc_main_ddrmc_main_bank(BLK));
      writeIO_DMC(ral_addr_of_fifo_rden_in_ddrmc_main_ddrmc_main_bank(BLK),fifo_rd_latency+glbl_mem_read_latency_dbi_offset/2);
   #endif

   if(glbl_tck >= TCK_TH_RD_CAL) {
      DEBUG_PRINTF("INFO: ddr_rd_dbi_cal(): SKIPPING read DBI centering stage for TCK = %d\n", glbl_tck);
      return 0;
   }

   /*************************************************************************************************/
   //   Unmask all the nibbles which are required for the read dq calibration
   /*************************************************************************************************/
   // Print logical to physical mappling
   glbl_nibble_mask_initial = glbl_dbi_nibble_mask_initial;                 //1 correspond to masked nibble and 0 correspond to unmasked nibble
   glbl_nibble_mask = glbl_nibble_mask_initial; 
   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_rd_dbi_cal(): Read DBI Calibration calibration has started for Ranks = %d, BITS PER RANK = %d \n", RANKS);
   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_rd_dbi_cal(): First nibble = %d, last Nibble = %d, and Nibbles Mask = 0x%8X \n", glbl_first_nibble, glbl_last_nibble, glbl_nibble_mask);
   for(i = 0; i < DBI_BITS; i++) {
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_rd_dbi_cal(): logical bit %d is mapped in physical nibble %d \n", i, glbl_lbit2pnibble_map[i]);
   }

   // Mask all the nibbles which doesn't need to be calibrated
   writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask); 

   for(nibble = 0; nibble < DBI_BITS; nibble++) {
      glbl_stable_valid_win[nibble] = 0;
      DEBUG_PRINTF("INFO: ddr_rd_dbi_cal(): glbl_stable_valid_win[%d] = %d \n", nibble, glbl_stable_valid_win[nibble]);
      writeIO_RIU(glbl_dbi_idly_riu_addr_map[nibble],0x0);
   }

   // Perform Read Data  calibration for each rank sequentially
   //Initialize internal varialbles
   glbl_nibble_mask = glbl_nibble_mask_initial;

   writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask_initial); 
   riu_reg_bcast(INCDEC_CRSE,0,0x1); 
   //riu_reg_bcast(CRSE_DLY,0,0x1); 

   // Enable comparision with data.
   writeIO_DMC(ral_addr_of_seq_data_capture_in_ddrmc_main_ddrmc_main_bank(BLK),0x0); 

   #ifdef DDR4
      // Select write data pattern.
      writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 0);

      for (rank = 0; rank < RANKS; rank += glbl_rank_inc) {
         // Write MPR register to Enable the Read DBI mode into the DRAM.
         set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                                  MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr5[0] | 0x1000 /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x1 /*u16 seq_a_ba*/,
                                  0x30 /*u32 seq_a2a_dly*/);
         // Activate a row for wrtie-read.
         set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                         MEM_ROW_ACTIVE /*u32 seq_a_cmd*/, 0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/,
                         0 /*u32 seq_a2a_dly*/);
 
         // Write a known pattern into the memory
         writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),readIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK))|0x9); //Enable extended write

         set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, MEM_COL_WRITE/*u32 seq_a_cmd*/, 0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0/*u16 seq_a_ba*/, 4/*u32 seq_a2a_dly*/);

         writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),readIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK))&0xFFFFFFF6); //Disable the extended write
      }

      // Setup read command from the written same location
       writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),MEM_COL_READ);

   #else
      // Select write data pattern.
      writeIO_DMC(ral_addr_of_seq_a_dq_early_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
      writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK),0);
      writeIO_DMC(ral_addr_of_seq_a_dq_1_in_ddrmc_main_ddrmc_main_bank(BLK),0);
      writeIO_DMC(ral_addr_of_seq_a_dq_late_in_ddrmc_main_ddrmc_main_bank(BLK),0);

      for (rank = 0; rank < RANKS; rank++) {
         // Activate a row for wrtie-read.
         lp4_bank_row_access(rank /*u8 rank*/, ACTIVATE/*u8 cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/); 

         // Write a known pattern into the memory
         writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),readIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK))|0x9); //Enable extended write
         lp4_bank_col_access(1 /*u32 nos_iteration_of_seqs*/, COL_WR /*u8 cmd_a_type*/, 0 /*u8 cmd_b_type*/, 0 /*u8 bank_addr*/, 0 /*u16 col_addr*/); 
         writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),readIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK))&0xFFFFFFF6); //Disable the extended write
      }

      // Setup read command from the written same location
      lp4_bank_col_access(1 /*u32 nos_iteration_of_seqs*/, COL_RD /*u8 cmd_a_type*/, 0 /*u8 cmd_b_type*/, 0 /*u8 bank_addr*/, 0 /*u16 col_addr*/); 

   #endif
   writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK),6); // tCCD_L = 8
   writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK),0); 
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK),nos_of_samples | (nos_of_samples<<16));


   /**********************************************************************************************************/   
   // Start 1st phase of the read data calibration (Move DBI to valid region with respect to their pdqs and ndqs sampling).
   /**********************************************************************************************************/   
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: rd_dbi_cal(): Start 1st phase of the read dbi calibration (Move DBI to valid region with respect to their pdqs and ndqs sampling) for rank = %d with nibble mask = 0x%08X\n", rank, glbl_nibble_mask);
   DEBUG_PRINTF("##################################################################################################################### \n");
      for(nibble = 0; nibble<DBI_BITS; nibble++) {
         glbl_bits_riu_map[nibble][0] = glbl_dbi_bits_riu_map[nibble][0];
         glbl_bits_riu_map[nibble][1] = glbl_dbi_bits_riu_map[nibble][1];
         glbl_lbit2pnibble_map[nibble] = glbl_dbi_lbit2pnibble_map[nibble];
         glbl_stable_valid_win[nibble] = 0;
      }
      glbl_nibble_mask      = glbl_nibble_mask_initial;
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask); 
      initialize_u16_array(glbl_pnibble2pbs_bcast_map,27,0xFFFF);
      set_nibble2bcast_bits( 1, DBI_BITS);

      glbl_phase_no = 0x1;
      glbl_region_type = PARTIAL;
      glbl_delay_type  = DBI_DELAY;
      glbl_inc_dec_load = INC;
      glbl_step_size = 1;
      glbl_en_qtr_dly_upd_wbs = 0;
      glbl_compare_edge_type = RISE;
      glbl_compare_data_type = BYTES;
      glbl_compare_data_bits = DBI_BITS;
      glbl_dual_edge_region = 0;
      glbl_pattern_type = 0;
      glbl_clear_error = 1;
      status = region_detect(); 
      glbl_en_qtr_dly_upd_wbs = 1;
      if(status) {
         write_error_XRAM(ERR_CODE_48_RD_DBI_FAIL_VALID_WINDOW_DBI_IDELAY);
         return 1;
      }

      u16 dbi_idelay_init_val[DBI_BITS];
      for(bit_cnt= 0; bit_cnt < DBI_BITS; bit_cnt++) { 
         dbi_idelay_init_val[bit_cnt] = readIO_RIU(glbl_dbi_idly_riu_addr_map[bit_cnt])&0x1FF;
         DEBUG_PRINTF("INFO: rd_dbi_cal(): dbi_delay_init_val[%d] = %d \n", bit_cnt, dbi_idelay_init_val[bit_cnt]);
      }
   DEBUG_PRINTF("INFO: 1st phase of the rd_dbi_cal() completed successfully \n");
   /**********************************************************************************************************/   
   // Ends 1st phase of the read data calibration (Moved PDQS and NDQS to valid region of their data bits 
   /**********************************************************************************************************/   

   /**********************************************************************************************************/   
   // Start 2nd phase of the read data calibration, calculate left margin of the read DBI while incrmenting the idelay (with fine taps of step size = 1) of the DBIs.
   /**********************************************************************************************************/   
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: rd_dbi_cal(): Start 2nd phase of the read data calibration, calculate left margin of the read DBI while incrmenting the idelay of the DBIs for rank = %d with nibble mask = 0x%08X\n", rank, glbl_nibble_mask);
   DEBUG_PRINTF("##################################################################################################################### \n");
      glbl_phase_no = 0x2A;
      glbl_region_type = FULL;
      //glbl_delay_type  = DBI_DELAY;
      //glbl_inc_dec_load = INC;
      glbl_step_size = step_size_coarse;
      //glbl_compare_edge_type = RISE;
      //glbl_compare_data_type = BYTES;
      //glbl_compare_data_bits = DBI_BITS;
      //glbl_dual_edge_region = 0;
      //glbl_pattern_type = 0;
      glbl_clear_error = 1;
      status = region_detect(); 
      glbl_en_qtr_dly_upd_wbs = 1;

      glbl_nibble_mask      = glbl_nibble_mask_initial;
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask); 
      status = update_delay_sequential(  DBI_DELAY /*int delay_type*/, DEC /*int inc_dec_load*/); 

      glbl_phase_no = 0x2C;
      //glbl_region_type = FULL;
      //glbl_delay_type  = DBI_DELAY;
      //glbl_inc_dec_load = INC;
      glbl_step_size = 1;
      glbl_en_qtr_dly_upd_wbs = 0;
      //glbl_compare_edge_type = RISE;
      //glbl_compare_data_type = BYTES;
      //glbl_compare_data_bits = DBI_BITS;
      //glbl_dual_edge_region = 0;
      //glbl_pattern_type = 0;
      glbl_clear_error = 1;
      status = region_detect(); 
      glbl_en_qtr_dly_upd_wbs = 1;

   /**********************************************************************************************************/   
   // Ends 3rd phase of the read data calibration, calculated left margin of the read DBI while incrmenting the idelay of the DBIs for rank = %d with nibble mask = 0x%08X\n", rank, glbl_nibble_mask);
   /**********************************************************************************************************/   

   /**********************************************************************************************************/   
   // Start 4th phase of the read data calibration (Record left margin).
   /**********************************************************************************************************/   
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: rd_dbi_cal():Start 4th phase of the read data calibration (Record left margin) for rank = %d with nibble mask = 0x%08X\n", rank, glbl_nibble_mask);
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: 1st phase of the rd_dbi_cal() completed successfully \n");
   u16 rd_delay;
   u16 left_margin[DBI_BITS];
   for(bit_cnt= 0; bit_cnt < DBI_BITS; bit_cnt++) { 
      rd_delay = readIO_RIU(glbl_dbi_idly_riu_addr_map[bit_cnt])&0x1FF;
      left_margin[bit_cnt] = rd_delay-dbi_idelay_init_val[bit_cnt];//+MIN_VLD_CNT;
      DEBUG_PRINTF("INFO: rd_dbi_cal(): Left margin for nibble dbi[%d] = %d \n", bit_cnt, left_margin[bit_cnt]);
   }
   DEBUG_PRINTF("INFO: 4th phase of the rd_dbi_cal() completed successfully \n");
   /**********************************************************************************************************/   
   // Ends 4th phase of the read data calibration (Recorded left margin).
   /**********************************************************************************************************/   

   /**********************************************************************************************************/   
   // Start 5th phase of the read data calibration (Revert back the dbi to start of the detected valid window ).
   /**********************************************************************************************************/   
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: rd_dbi_cal():Start 5th phase of the read data calibration (Revert back the dbi to start of the detected valid window) for rank = %d with nibble mask = 0x%08X\n", rank, glbl_nibble_mask);
   DEBUG_PRINTF("##################################################################################################################### \n");
   for(bit_cnt= 0; bit_cnt < DBI_BITS; bit_cnt++) { 
      writeIO_RIU(glbl_dbi_idly_riu_addr_map[bit_cnt],dbi_idelay_init_val[bit_cnt]);
      //glbl_stable_valid_win[bit_cnt]=0;
   }
   status = update_delay_sequential( DQ_DELAY /*int delay_type*/, DEC /*int inc_dec_load*/); 
   DEBUG_PRINTF("INFO: 5th phase of the rd_dbi_cal() completed successfully \n");
   /**********************************************************************************************************/   
   // Ends 5th phase of the read data calibration (Reverted back the dbi to start of the detected valid window).
   /**********************************************************************************************************/   

   /**********************************************************************************************************/   
   // Start 6th phase of the read data calibration (Move PDQS,NDQS,DQ to detect right margin of the DBI (with fine taps of step size = 1).
   /**********************************************************************************************************/   
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: rd_dbi_cal(): Start 6th phase of the read data calibration (Move PDQS,NDQS,DQ to detect right margin of the DBI (with fine taps of step size = 1) for rank = %d with nibble mask = 0x%08X\n", rank, glbl_nibble_mask);
   DEBUG_PRINTF("##################################################################################################################### \n");
      u16 idelay_init_val[DQ_BITS];
      //u16 idelay_init_val2[DQ_BITS];
      u16 pqtr_init_val[DBI_BITS];
      u16 nqtr_init_val[DBI_BITS];
      for(bit_cnt= 0; bit_cnt < DQ_BITS; bit_cnt++) { 
         idelay_init_val[bit_cnt] = readIO_RIU(glbl_dq_idly_riu_addr_map[bit_cnt])&0x1FF;
         glbl_bits_riu_map[bit_cnt][0] = glbl_dq_bits_riu_map[bit_cnt][0];
         glbl_bits_riu_map[bit_cnt][1] = glbl_dq_bits_riu_map[bit_cnt][1];
         //idelay_init_val2[bit_cnt] = idelay_init_val[bit_cnt];
         DEBUG_PRINTF("INFO: rd_dbi_cal(): delay_init_val[%d] = %d \n", bit_cnt, idelay_init_val[bit_cnt]);
         //DEBUG_PRINTF("INFO: rd_dbi_cal(): delay_init_val[%d] = %d \n", bit_cnt, idelay_init_val2[bit_cnt]);
      }
      for(nibble = 0; nibble<DBI_BITS; nibble++) {
         pqtr_init_val[nibble] = read_riu_nibble_reg(glbl_dbi_lbit2pnibble_map[nibble],PQTR);
         DEBUG_PRINTF("INFO: rd_dbi_cal(): pqtr_init_val[nibble][%d] = %d \n", nibble, pqtr_init_val[nibble]);
         nqtr_init_val[nibble] = read_riu_nibble_reg(glbl_dbi_lbit2pnibble_map[nibble],NQTR);
         DEBUG_PRINTF("INFO: rd_dbi_cal(): nqtr_init_val[nibble][%d] = %d \n", nibble, nqtr_init_val[nibble]);
         glbl_stable_valid_win[nibble] = 0;
      }
      glbl_nibble_mask      = glbl_nibble_mask_initial;
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask); 
      initialize_u16_array(glbl_pnibble2pbs_bcast_map,27,0xFFFF);
      set_nibble2bcast_bits( 1, DQ_BITS);
      for(bit_cnt= 0; bit_cnt < DQ_BITS; bit_cnt++) { 
         DEBUG_PRINTF("INFO: rd_dbi_cal(): delay_init_val[%d] = %d \n", bit_cnt, idelay_init_val[bit_cnt]);
      }

      glbl_phase_no = 0x6A;
      //glbl_region_type = FULL;
      glbl_delay_type  = P_N_DQ_DELAY;
      //glbl_inc_dec_load = INC;
      glbl_step_size = step_size_coarse;
      //glbl_compare_edge_type = RISE;
      //glbl_compare_data_type = BYTES;
      //glbl_compare_data_bits = DBI_BITS;
      //glbl_dual_edge_region = 0;
      //glbl_pattern_type = 0;
      glbl_clear_error = 1;
      status = region_detect(); 
      glbl_en_qtr_dly_upd_wbs = 1;

      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask_initial); 
      status = update_delay_sequential( P_N_DQ_DELAY /*int delay_type*/, DEC /*int inc_dec_load*/); 

      glbl_phase_no = 0x6C;
      //glbl_region_type = FULL;
      //glbl_delay_type  = P_N_DQ_DELAY;
      //glbl_inc_dec_load = INC;
      glbl_step_size = 1;
      glbl_en_qtr_dly_upd_wbs = 0;
      //glbl_compare_edge_type = RISE;
      //glbl_compare_data_type = BYTES;
      //glbl_compare_data_bits = DBI_BITS;
      //glbl_dual_edge_region = 0;
      //glbl_pattern_type = 0;
      glbl_clear_error = 1;
      status = region_detect(); 
      glbl_en_qtr_dly_upd_wbs = 1;

   DEBUG_PRINTF("INFO: 6th phase of the rd_dbi_cal() completed successfully \n");
   /**********************************************************************************************************/   
   // Ends 6th phase of the read data calibration (Moved PDQS,NDQS,DQ to detect right margin of the DBI).
   /**********************************************************************************************************/   

   /**********************************************************************************************************/   
   // Start 7th phase of the read data calibration (Record right margin).
   /**********************************************************************************************************/   
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: rd_dbi_cal():Start 7th phase of the read data calibration (Record right margin) for rank = %d with nibble mask = 0x%08X\n", rank, glbl_nibble_mask);
   DEBUG_PRINTF("##################################################################################################################### \n");
   u16 right_margin[DBI_BITS];
   for(bit_cnt= 0; bit_cnt < DBI_BITS; bit_cnt++) { 
      rd_delay = read_riu_nibble_reg(glbl_dbi_lbit2pnibble_map[bit_cnt],PQTR);
      right_margin[bit_cnt] = rd_delay-pqtr_init_val[bit_cnt];
      DEBUG_PRINTF("INFO: rd_dbi_cal(): rd_delay = %d, pqtr_init_val[%d] = %d, Right margin for nibble dbi[%d] = %d \n", rd_delay, bit_cnt, pqtr_init_val[bit_cnt], bit_cnt, right_margin[bit_cnt]);
   }
   DEBUG_PRINTF("INFO: 7th phase of the rd_dbi_cal() completed successfully \n");
   /**********************************************************************************************************/   
   // Ends 7th phase of the read data calibration (Recorded right margin).
   /**********************************************************************************************************/   

   /**********************************************************************************************************/   
   // Start 8th phase of the read data calibration (Revert back the qtrs to start of the detected valid window ).
   /**********************************************************************************************************/   
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: rd_dbi_cal():Start 8th phase of the read data calibration (Revert back the qtrs to start of the detected valid window) for rank = %d with nibble mask = 0x%08X\n", rank, glbl_nibble_mask);
   DEBUG_PRINTF("##################################################################################################################### \n");
      for(bit_cnt= 0; bit_cnt < DQ_BITS; bit_cnt++) { 
         DEBUG_PRINTF("INFO: rd_dbi_cal(): delay_init_val[%d] = %d \n", bit_cnt, idelay_init_val[bit_cnt]);
         writeIO_RIU(glbl_dq_idly_riu_addr_map[bit_cnt], idelay_init_val[bit_cnt]);
      }
      for(nibble = 0; nibble<DBI_BITS; nibble++) {
         DEBUG_PRINTF("INFO: rd_dbi_cal(): pqtr_init_val[nibble][%d] = %d \n", nibble, pqtr_init_val[nibble]);
         write_riu_nibble_reg(glbl_dbi_lbit2pnibble_map[nibble],PQTR,pqtr_init_val[nibble]);
         DEBUG_PRINTF("INFO: rd_dbi_cal(): nqtr_init_val[nibble][%d] = %d \n", nibble, nqtr_init_val[nibble]);
         write_riu_nibble_reg(glbl_dbi_lbit2pnibble_map[nibble],NQTR,nqtr_init_val[nibble]);
      }
   glbl_nibble_mask_initial = glbl_dq_nibble_mask_initial;                 //1 correspond to masked nibble and 0 correspond to unmasked nibble
   writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask_initial); 
   bitslice_reset_bcast(0, BS_RESET_WT); // BS Reset
   DEBUG_PRINTF("INFO: 8th phase of the rd_dbi_cal() completed successfully \n");
   /**********************************************************************************************************/   
   // Ends 8th phase of the read data calibration (Reverted back the dbi to start of the detected valid window).
   /**********************************************************************************************************/   

   /**********************************************************************************************************/   
   // Start 9th phase of the read data calibration (Calculate center or final taps of pqtr, nqtr, dq, and dbi).
   /**********************************************************************************************************/   
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: rd_dbi_cal(): Start 9th phase of the read data calibration (Calculate center or final taps of pqtr, nqtr, dq, and dbi) for rank = %d with nibble mask = 0x%08X\n", rank, glbl_nibble_mask);
   DEBUG_PRINTF("##################################################################################################################### \n");
   u16 qtr_dly_change[DBI_BITS];
   u16 dbi_dly_change[DBI_BITS];
   for(bit_cnt= 0; bit_cnt < DBI_BITS; bit_cnt++) { 
      if(left_margin[bit_cnt] > right_margin[bit_cnt]) {
         dbi_dly_change[bit_cnt] = (left_margin[bit_cnt] - right_margin[bit_cnt])/2;
         qtr_dly_change[bit_cnt] = 0;
      } else {
         dbi_dly_change[bit_cnt] = 0;;
         qtr_dly_change[bit_cnt] = (right_margin[bit_cnt] - left_margin[bit_cnt])/2;
      }
      DEBUG_PRINTF("INFO: rd_dbi_cal(): dbi_dly_adjust[%d] = %d, qtr_dly_adjust[%d] = %d \n", bit_cnt, dbi_dly_change[bit_cnt], bit_cnt, qtr_dly_change[bit_cnt]);
   }
   DEBUG_PRINTF("INFO: 9th phase of the rd_dbi_cal() completed successfully \n");
   /**********************************************************************************************************/   
   // Ends 9th phase of the read data calibration (Calculated center or final taps of pqtr, nqtr, dq, and dbi).
   /**********************************************************************************************************/   

   /**********************************************************************************************************/   
   // Start 10th phase of the read data calibration (Adjust dbi/qtrs to center align the dbi).
   /**********************************************************************************************************/   
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: rd_dbi_cal():  Start 10th phase of the read data calibration (Adjust dbi/qtrs to center align the dbi) for rank = %d with nibble mask = 0x%08X\n", rank, glbl_nibble_mask);
   DEBUG_PRINTF("##################################################################################################################### \n");
   glbl_bit_mask_31_0 = 0xFFFFFFFF;
   glbl_bit_mask_31_0_last = 0x00000000;
   map_l2p_mask(DBI_BITS, RISE); 
   DEBUG_PRINTF("INFO: physical nibble mask for 17_0 = 0x%08X \n", glbl_nibble_mask);
   iteration = 0;
   do {
      iteration++;
      DEBUG_PRINTF("############################################################################################ \n");
      DEBUG_PRINTF("################################ rd_dbi_cal(): Tenth phase a (Adjusting QTRs) iteration number = %d ############## \n", iteration);
      for(bit_cnt=0; bit_cnt<DBI_BITS; bit_cnt++) {
         if(qtr_dly_change[bit_cnt]>0) {
            qtr_dly_change[bit_cnt]--;
            glbl_bit_mask_31_0 = glbl_bit_mask_31_0 & (~(1<<bit_cnt));
         } else {
            glbl_bit_mask_31_0 = glbl_bit_mask_31_0 | (1<<bit_cnt);
         } 
      }
      DEBUG_PRINTF("INFO: logical nibble mask for 17_0 = 0x%08X, last logical mask for 17_0 = 0x%08X \n", glbl_bit_mask_31_0, glbl_bit_mask_31_0_last);
      if(glbl_bit_mask_31_0 != glbl_bit_mask_31_0_last) {
         map_l2p_mask(DBI_BITS, RISE); 
         DEBUG_PRINTF("INFO: New physical nibble mask for 17_0 = 0x%08X \n", glbl_nibble_mask);
         glbl_bit_mask_31_0_last = glbl_bit_mask_31_0;
         //Mask the nibbles for which the phase has been completed.
         writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask); 
      }
      DEBUG_PRINTF("INFO: physical nibble mask for 17_0 = 0x%08X \n", glbl_nibble_mask);
 
      riu_reg_bcast(IODLY_BCAST_CTRL,0,(0x1<<IDLY_BCAST_EN_POS)|(0x1<<IDLY_INC_POS)); 
      riu_reg_bcast(PQTR,0,(0x1<<QTR_INC_DLY_POS)); 
      riu_reg_bcast(NQTR,0,(0x1<<QTR_INC_DLY_POS)); 
      if(iteration>500)
         return 1;
   } while (glbl_nibble_mask != 0xFFFFFFFF);
   DEBUG_PRINTF("INFO: 10th a phase of the rd_dbi_cal() completed successfully \n");
   /**********************************************************************************************************/   
   // Ends 10th a phase of the read data calibration (Adjusted qtrs to center align the dbi).
   /**********************************************************************************************************/   
   // Set broadcast enable for DBI only
   for(nibble = 0; nibble<DBI_BITS; nibble++) {
      glbl_bits_riu_map[nibble][0] = glbl_dbi_bits_riu_map[nibble][0];
      glbl_bits_riu_map[nibble][1] = glbl_dbi_bits_riu_map[nibble][1];
   }
   glbl_nibble_mask      = glbl_nibble_mask_initial;
   writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask); 
   initialize_u16_array(glbl_pnibble2pbs_bcast_map,27,0xFFFF);
   set_nibble2bcast_bits( 1, DBI_BITS);
   glbl_bit_mask_31_0 = 0xFFFFFFFF;
   glbl_bit_mask_31_0_last = 0x00000000;
   map_l2p_mask(DBI_BITS, RISE); 
   DEBUG_PRINTF("INFO: physical nibble mask for 17_0 = 0x%08X \n", glbl_nibble_mask);
   iteration = 0;
   do {
      iteration++;
      DEBUG_PRINTF("############################################################################################ \n");
      DEBUG_PRINTF("################################ rd_dbi_cal(): Tenth phase b (Adjusting DBIs) iteration number = %d ############## \n", iteration);
      for(bit_cnt=0; bit_cnt<DBI_BITS; bit_cnt++) {
         if(dbi_dly_change[bit_cnt]>0) {
            dbi_dly_change[bit_cnt]--;
            glbl_bit_mask_31_0 = glbl_bit_mask_31_0 & (~(1<<bit_cnt));
         } else {
            glbl_bit_mask_31_0 = glbl_bit_mask_31_0 | (1<<bit_cnt);
         } 
         DEBUG_PRINTF("INFO: dbi_dly_change[%d] = %d nibble mask for 17_0 = 0x%08X \n", bit_cnt, dbi_dly_change[bit_cnt], glbl_bit_mask_31_0);
      }
      DEBUG_PRINTF("INFO: logical nibble mask for 17_0 = 0x%08X, last logical mask for 17_0 = 0x%08X \n", glbl_bit_mask_31_0, glbl_bit_mask_31_0_last);
      if(glbl_bit_mask_31_0 != glbl_bit_mask_31_0_last) {
         map_l2p_mask(DBI_BITS, RISE); 
         DEBUG_PRINTF("INFO: New physical nibble mask for 17_0 = 0x%08X \n", glbl_nibble_mask);
         glbl_bit_mask_31_0_last = glbl_bit_mask_31_0;
         //Mask the nibbles for which the phase has been completed.
         writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask); 
      }
      DEBUG_PRINTF("INFO: physical nibble mask for 17_0 = 0x%08X \n", glbl_nibble_mask);
 
      riu_reg_bcast(IODLY_BCAST_CTRL,0,(0x1<<IDLY_BCAST_EN_POS)|(0x1<<IDLY_INC_POS)); 
      if(iteration>500)
         return 1;
   } while (glbl_nibble_mask != 0xFFFFFFFF);
   DEBUG_PRINTF("INFO: 10th a phase of the rd_dbi_cal() completed successfully \n");
   /**********************************************************************************************************/   
   // Ends 10th b phase of the read data calibration (Adjusted DBIs to center align the dbi).
   /**********************************************************************************************************/   
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: rd_dbi_cal(): READ DATA CALIBRATION END \n");
   DEBUG_PRINTF("##################################################################################################################### \n");

   #ifdef DDR4
      for (rank = 0; rank < RANKS; rank += glbl_rank_inc) {
         // Close any opened ROW
         set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                         MEM_ROW_PREACHARGE/*u32 seq_a_cmd*/, 0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/,
                         0 /*u32 seq_a2a_dly*/);
      }
      //// Write MPR register to Disable the Read DBI mode into the DRAM.
      //set_1_seq_command_at_mem(0 /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
      //                         MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr5[0] & 0xEFFF /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x1 /*u16 seq_a_ba*/,
      //                         0x30 /*u32 seq_a2a_dly*/);
      //// Disable read DBI mode in the cal logic.
      //writeIO_DMC(ral_addr_of_xpi_read_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0); 

      //if(glbl_mr5[0] & (0x1 << 11)) {
      //   // Enable write DBI mode in the cal logic.
      //   writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 0x2); 
      //} else if(glbl_mr5[0] & (0x1 << 10)) {
      //   // Enable write DM mode in the cal logic.
      //   writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1); 
      //}

   #else
      //if(glbl_lp4_mr3 & (0x1 << 7)) {
      //   // Enable write DBI mode in the cal logic.
      //   writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 0x2); 
      //} else if(!(glbl_lp4_mr13 & (0x1 << 5))) { //glbl_lp4_mr13[5]=0:DM Enabled; glbl_lp4_mr13[5]=1: DM Disabled
      //   // Enable write DM mode in the cal logic.
      //   writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1); 
      //}
      for (rank = 0; rank < RANKS; rank++) {
         // Close/Precharge the opened Bank
         lp4_bank_row_access(rank /*u8 rank*/, PRECHARGE /*u8 cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/); 
      }

      //// Write MPR register to Disable the Read DBI mode into the DRAM.
      //lp4_mode_reg_wr(rank, 3, glbl_lp4_mr3 & (~(0x1<<6)));
   
      // // Disable read DBI mode in the cal logic.
      // writeIO_DMC(ral_addr_of_xpi_read_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0); 

   #endif

   #ifdef ENABLE_MICROBLAZE_BFM
      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: rd_dbi_cal(): DUMP the calibration values \n");
      DEBUG_PRINTF("##################################################################################################################### \n");

      RECORD_DLY("// Written from RD_CAL function \n");

      u16 dbi_idelay[DBI_BITS];
      RECORD_DLY("#define DBI_IDELAY_VAL { ");
      for(bit_cnt = 0; bit_cnt < DBI_BITS; bit_cnt++) {
         dbi_idelay[bit_cnt] = (readIO_RIU(glbl_dbi_idly_riu_addr_map[bit_cnt])) & 0x1FF;
         if(bit_cnt < (DBI_BITS-1)) {
            RECORD_DLY(" 0x%04X,", dbi_idelay[bit_cnt]);
         } else {
            RECORD_DLY(" 0x%04X } \n", dbi_idelay[bit_cnt]);
         }
      }   

      u16 idelay[DQ_BITS];
      RECORD_DLY("// Written from RD_CAL function \n");
      RECORD_DLY("#define DBI_DQ_IDELAY_VAL { ");
      for(bit_cnt = 0; bit_cnt < DQ_BITS; bit_cnt++) {
         idelay[bit_cnt] = (readIO_RIU(glbl_dq_idly_riu_addr_map[bit_cnt])) & 0x1FF;
         if(bit_cnt < (DQ_BITS-1)) {
            RECORD_DLY(" 0x%04X,", idelay[bit_cnt]);
         } else {
            RECORD_DLY(" 0x%04X } \n", idelay[bit_cnt]);
         }
      }   
      u8 lbyte;
      u16 qtr_delay[DBI_BITS];
      RECORD_DLY("#define DBI_PQTR_VAL { ");
      for(lbyte = 0; lbyte < DBI_BITS; lbyte++) {
         qtr_delay[lbyte] = read_riu_nibble_reg(glbl_lbit2pnibble_map[lbyte], PQTR) & 0x1FF;
         if(lbyte < (DBI_BITS-1)) {
            RECORD_DLY(" 0x%04X,", qtr_delay[lbyte]);
         } else {
            RECORD_DLY(" 0x%04X } \n", qtr_delay[lbyte]);
         }
      }   
      RECORD_DLY("#define DBI_NQTR_VAL { ");
      for(lbyte = 0; lbyte < DBI_BITS; lbyte++) {
         qtr_delay[lbyte] = read_riu_nibble_reg(glbl_lbit2pnibble_map[lbyte], NQTR) & 0x1FF;
         if(lbyte < (DBI_BITS-1)) {
            RECORD_DLY(" 0x%04X,", qtr_delay[lbyte]);
         } else {
            RECORD_DLY(" 0x%04X } \n", qtr_delay[lbyte]);
         }
      }   
   #endif
#endif
   
   return 0;
}


/*********************************************************************************************************************************************/
// Function definations
/*********************************************************************************************************************************************/







