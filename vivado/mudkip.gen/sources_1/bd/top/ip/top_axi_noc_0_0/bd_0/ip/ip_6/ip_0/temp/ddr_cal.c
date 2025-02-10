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
//       Revision:       $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/ddr_cal.c#6 $
//                       $Author: rgaddam $
//                       $DateTime: 2019/04/28 12:58:54 $
//                       $Change: 21407526 $
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
#include "cal_reg_define.h"
#include "riu_reg_define.h"
#include "xsdb_reg_define.h"
#include "internal_reg_define.h"
#include "bd_d0e7_MC0_ddrc_0_phy_cal_config_define.h"
#include "bd_d0e7_MC0_ddrc_0_phy_io_config_define.h"
#include "function_dec.c"
#include "global_variable_define.h"
//#define HW_TEST_CODE_DDR4
//#define HW_TEST_CODE_LPDDR4
//#define HW_TEST_CODE_DDR4_AC_CAL
//#define HW_TEST_CODE_LPDDR4_SLOW_INIT
//#define HW_TEST_RPI
//#define HW_TEST_SEQ_COVERAGE
//#define HW_TEST_APB_FABRIC_PORT

#ifdef HW_TEST_CODE_DDR4
void PMC_defaults(void);
void init_test(void);
void sequencer_precharge_cmd(void);
void sequencer_activate_cmd(void);
void sequencer_single_rd_cmd(void);
void sequencer_multi_rd_cmd(void);
void sequencer_multi_rd_cmd2(void);
void sequencer_single_write(void);
void sequencer_single_write_read(void);
void sequencer_two_writes(void);
void sequencer_single_extend_write(void);
void sequencer_single_extend_extra_write(void);
void sequencer_two_extend_writes_one_read(void);
void sequencer_wrlvl_test(void);
#endif

#ifdef HW_TEST_CODE_LPDDR4
void PMC_defaults_lpddr4(void);
void sequencer_precharge_cmd_lpddr4(void);
void sequencer_activate_cmd_lpddr4(void);
void sequencer_single_write_lpddr4(void);
void sequencer_single_rd_cmd_lpddr4(void);
#endif

#ifdef HW_TEST_CODE_DDR4_AC_CAL
void sequencer_ac_cal_single(void);
#endif

#ifdef HW_TEST_CODE_LPDDR4_SLOW_INIT
void sequencer_setup_slow_ck(u8 on);
void lpddr4_init_test(void);
#endif

#ifdef HW_TEST_RPI
void rpi_rst_test(void);
void rpi_bcast_nibble_mask_test(void);
void rpi_adjustments_test(void);
#endif

#ifdef HW_TEST_SEQ_COVERAGE
void seq_coverage(void);
#endif
void hw_test_code();

#ifdef HW_TEST_APB_FABRIC_PORT
u8 apb_fabric_test(void);
#endif

u8 ddr_cal(void) {

   u8 status = 0;
   u32 addr, val;
   u32 cal_time;
   u32 pit1_cnt;
   u32 cal_stage_time;
   u32 prbs_cal_time;
   u32 cal_index_freq_switch=0;
   u32 cal_stage_iter=0;
   u32 cal_iter=0;
   u32 CAL_ITER_LIMIT = 62; // Calibration iterations limit
   u32 xpi_map_bitslip;
   u16 pqtr_val[DQ_NIBBLES], nqtr_val[DQ_NIBBLES];
   u16 dq_idelay_val[DQ_BITS], dbi_idelay_val[DBI_BITS];
   u8  bit, nibble;
   u8  fifo_rd_latency;
   u8  read_latency;
   u8  temp_nibble;
   u8  rank;
   int i;
   // Test code for hardware access
   hw_test_code();
   #ifdef HW_TEST_APB_FABRIC_PORT
   apb_fabric_test();
   #endif

   writeIO_XRAM(XRAM_CAL_PRE_STATUS_17_9, 0x1FF);
   cal_stage_time = get_cal_stage_time();

   // Run the calibration sequence
   for (glbl_cal_index=0; glbl_cal_stage[glbl_cal_index]!=CAL_DONE; glbl_cal_index++) {
      glbl_cal_code = glbl_cal_stage[glbl_cal_index];
      glbl_heartbeat = (glbl_heartbeat == 0) ? 1 : 0;
      glbl_gpo3 = glbl_heartbeat ? (glbl_gpo3 | (1 << GPO3_HEARTBEAT_POS)) : (glbl_gpo3 & ~(1 << GPO3_HEARTBEAT_POS));
      if(glbl_cal_stage[glbl_cal_index] & CAL_SKIP) { // CAL_SKIP
         DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): SKIPPED the calibration stage. Stage code = %0x Stage Name = %s \n", glbl_cal_stage[glbl_cal_index] & 0x3F, glbl_stage_code_string[glbl_cal_code]);
         glbl_cal_status = (CAL_SKIPPED | (glbl_cal_stage[glbl_cal_index] & 0x3F));
         writeIO_XRAM((XRAM_CAL_SEQ_STATUS + 4*glbl_cal_index), glbl_cal_status);
         writeIO_XRAM(XRAM_CAL_POINTER, glbl_cal_status);
         glbl_gpo3 = (glbl_gpo3 & GPO3_CAL_STATUS_CLR) | (glbl_cal_status << GPO3_CAL_STATUS_POS) | GPO3_CAL_BUSY;
         writeIO_IOM(IO_GPO3, glbl_gpo3);

      } else { // CAL_EN
         glbl_cal_status = (CAL_IN_PROG | (glbl_cal_stage[glbl_cal_index] & 0x3F));
         writeIO_XRAM((XRAM_CAL_SEQ_STATUS + 4*glbl_cal_index), glbl_cal_status);
         writeIO_XRAM(XRAM_CAL_POINTER, glbl_cal_status);
         glbl_gpo3 = (glbl_gpo3 & GPO3_CAL_STATUS_CLR) | (glbl_cal_status << GPO3_CAL_STATUS_POS) | GPO3_CAL_BUSY;
         writeIO_IOM(IO_GPO3, glbl_gpo3);

         DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): STARTED the calibration stage. Stage code = %0x Stage Name = %s \n", glbl_cal_stage[glbl_cal_index] & 0x3F, glbl_stage_code_string[glbl_cal_code]);

         switch (glbl_cal_stage[glbl_cal_index] & 0x3F) {
            case PHY_BISC:
                  status = ddr_self_calibrate(1);
               break;

            case MEM_INIT:
                  status = ddr_init_f0();
               break;

            #ifdef LPDDR4
               case F0_LPDDR4_CS_CA_TRAIN:
                  if(F0_TCK<834) {
                     status = ddr_ca_cal();
                  } else {
                     DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): Skipped the CA CS calibration because the TCK is greater than 833ps \n");
                  }
               break;
               case F1_LPDDR4_CS_CA_TRAIN:
                  if(F1_TCK<834) {
                     status = ddr_ca_cal();
                  } else {
                     DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): Skipped the CA CS calibration because the TCK is greater than 833ps \n");
                  }
               break;

#ifndef VNC_DIS_VREF_CAL
               case F0_LPDDR4_CA_VREF_TRAIN:
                  if(F0_TCK<834) {
                     status = ddr_ca_vref_cal(SIMPLE);
                  } else {
                     DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): Skipped the CA CS Vref calibration because the TCK is greater than 833ps \n");
                  }
                  break;
               case F1_LPDDR4_CA_VREF_TRAIN:
                  if(F1_TCK<834) {
                     status = ddr_ca_vref_cal(SIMPLE);
                  } else {
                     DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): Skipped the CA CS Vref calibration because the TCK is greater than 833ps \n");
                  }
                  break;
            #endif
#endif // VNC_DIS_VREF_CAL

#ifndef VNC_DIS_LRDIMM	  
            #ifdef DDR4
               case F0_LRDIMM_DB_MREP: 
               case F1_LRDIMM_DB_MREP: 
                  status = lrdimm_db_mrep();
                  break;

               case F0_LRDIMM_DB_MRD_CYCLE: 
               case F1_LRDIMM_DB_MRD_CYCLE: 
                  status = lrdimm_db_mrd_cycle();
                  break;

               case F0_LRDIMM_DB_MRD_CENTER: 
               case F1_LRDIMM_DB_MRD_CENTER: 
                  status = lrdimm_db_mrd_center();
                  break;

               case F0_LRDIMM_DB_DWL: 
               case F1_LRDIMM_DB_DWL: 
                  status = lrdimm_db_dwl();
                  break;

               case F0_LRDIMM_DB_MWD_CYCLE: 
               case F1_LRDIMM_DB_MWD_CYCLE: 
                  status = lrdimm_db_mwd_cycle();
                  break;

               case F0_LRDIMM_DB_MWD_CENTER: 
               case F1_LRDIMM_DB_MWD_CENTER: 
                  status = lrdimm_db_mwd_center();
                  break;
            #endif
#endif // VNC_DIS_LRDIMM

            case F0_DQS_GATE_CAL:
            case F1_DQS_GATE_CAL:
               status = ddr_dqs_gate(0);
               break;

            case F0_WRITE_LEVELING: 
            case F1_WRITE_LEVELING: 
               status = ddr_wrlvl();
               break;
            
            case F0_READ_DQ_CAL: 
            case F1_READ_DQ_CAL: 
               status = ddr_rd_cal(SIMPLE);
               break;

            case F0_WRITE_DQ_DBI_CAL: 
            case F1_WRITE_DQ_DBI_CAL: 
               status = ddr_write_dqs_to_dq();
               break;
            
            case F0_WRITE_LATENCY_CAL: 
            case F1_WRITE_LATENCY_CAL: 
               status = ddr_write_latency_cal();
               break;

            case F0_READ_DBI_CAL: 
            case F1_READ_DBI_CAL: 
               status = ddr_rd_dbi_cal();
//-----------------------------------Disable the Read DBI-------------------------------------
                DEBUG_PRINTF("INFO: rd_dbi_cal(): Disable read DBI mode in DRAM\n");
                writeIO_DMC(ral_addr_of_xpi_read_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_xpi_read_dbi&0xFE);
                #ifdef LPDDR4
                   for (rank = 0; rank < RANKS; rank++) {
                      lp4_mode_reg_wr(rank, 3, glbl_lp4_mr3 & (~(0x1<<6)));
                   }
        	       temp_nibble;
        	       for(nibble = 0; nibble<DQ_NIBBLES;nibble++) { //glbl_dqs_first_nibble; nibble <= glbl_dqs_last_nibble ; nibble++) {
        	          read_latency      = readIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK)+glbl_dq_lnibble2pnibble_map[nibble]*4);
        	          writeIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK) + glbl_dq_lnibble2pnibble_map[nibble]*4, read_latency-glbl_mem_read_latency_dbi_offset);
        	       }
        	       fifo_rd_latency = readIO_DMC(ral_addr_of_fifo_rden_in_ddrmc_main_ddrmc_main_bank(BLK));
        	       writeIO_DMC(ral_addr_of_fifo_rden_in_ddrmc_main_ddrmc_main_bank(BLK),fifo_rd_latency-glbl_mem_read_latency_dbi_offset/2);
        	    #else
        	       for (rank = 0; rank < RANKS; rank += glbl_rank_inc) {
        	       // Write MPR register to Enable the Read DBI mode into the DRAM.
        	              set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/,
        	                                       MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr5[0]&0xEFFF /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x1 /*u16 seq_a_ba*/,
        	                                       0x30 /*u32 seq_a2a_dly*/);
        	       }
        	    #endif
//--------------------------------------------------------------------------------------------
        	break;

#ifndef VNC_DIS_VREF_CAL
            case F0_READ_DQ_VREF_CAL:
            case F1_READ_DQ_VREF_CAL:
               status = ddr_rd_vref_cal(SIMPLE);
               break;
#endif // VNC_DIS_VREF_CAL

            case F0_READ_DQ_DBI_CAL_COMPLEX:
            case F1_READ_DQ_DBI_CAL_COMPLEX:
               status = ddr_rd_cal(COMPLEX);
//-----------------------------------Enable the Read DBI-------------------------------------
            if((F0_MODE_READ_DBI_CAL==CAL_EN) || (F1_MODE_READ_DBI_CAL==CAL_EN))
            {
               DEBUG_PRINTF("INFO: rd_dbi_cal(): Enable read DBI mode in DRAM\n");
               writeIO_DMC(ral_addr_of_xpi_read_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_xpi_read_dbi);
               #ifdef LPDDR4
                  for (rank = 0; rank < RANKS; rank++) {
                     lp4_mode_reg_wr(rank, 3, glbl_lp4_mr3);
                  }
                  temp_nibble;
                  for(nibble = 0; nibble<DQ_NIBBLES;nibble++) { //glbl_dqs_first_nibble; nibble <= glbl_dqs_last_nibble ; nibble++) {
                     read_latency      = readIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK)+glbl_dq_lnibble2pnibble_map[nibble]*4);
                     writeIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK) + glbl_dq_lnibble2pnibble_map[nibble]*4, read_latency+glbl_mem_read_latency_dbi_offset);
                  }
                  fifo_rd_latency = readIO_DMC(ral_addr_of_fifo_rden_in_ddrmc_main_ddrmc_main_bank(BLK));
                  writeIO_DMC(ral_addr_of_fifo_rden_in_ddrmc_main_ddrmc_main_bank(BLK),fifo_rd_latency+glbl_mem_read_latency_dbi_offset/2);
               #else
                  for (rank = 0; rank < RANKS; rank += glbl_rank_inc) {
                  // Write MPR register to Enable the Read DBI mode into the DRAM.
                  set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/,
                                   MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr5[0] | 0x1000 /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x1 /*u16 seq_a_ba*/,
                                   0x30 /*u32 seq_a2a_dly*/);
                  }
               #endif
            }
//--------------------------------------------------------------------------------------------
               break;
            
#ifndef VNC_DIS_VREF_CAL
            case F0_WRITE_DQ_VREF_CAL: 
            case F1_WRITE_DQ_VREF_CAL: 
               status = ddr_write_vref();
               break;
#endif // VNC_DIS_VREF_CAL

            case F0_WRITE_DQ_DBI_CAL_COMPLEX: 
            case F1_WRITE_DQ_DBI_CAL_COMPLEX: 
               status = ddr_write_complex();
               break;

            case F0_PRBS_READ:
            case F1_PRBS_READ:
               status = ddr_rd_cal(PRBS);
               break;

            case F0_PRBS_WRITE:
            case F1_PRBS_WRITE:
               status = ddr_write_prbs();
               break;

            case FREQ_SWITCH: 
               status = cal_freq_switch();
               break;

            case EN_VT_TRACK: 
               status = ddr_en_vtc();
               break;

            case READ_DQS_TRACK: 
               status = ddr_read_dqs_track();
               break;

            #ifdef LPDDR4
               case LPDDR4_WR_DQS_TRACK: 
                  status = lp4_wr_dqs_track(4);
                  break;
            #endif

            default:
               DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): Invalid calibration stage: 0x%0X, Stage Name = %s \n", glbl_cal_stage[glbl_cal_index], glbl_stage_code_string[glbl_cal_code]);
               DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): Exiting ddr_cal()\n");
               status = 1;
         } // Switch glbl_cal_stage

         DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): END the calibration stage. Stage code = %0x Stage Name = %s \n", glbl_cal_stage[glbl_cal_index] & 0x3F, glbl_stage_code_string[glbl_cal_code]);
         if (status!=0){
            glbl_cal_status = (CAL_FAILED | (glbl_cal_stage[glbl_cal_index] & 0x3F));
            writeIO_XRAM((XRAM_CAL_SEQ_STATUS + 4*glbl_cal_index), glbl_cal_status);
            writeIO_XRAM(XRAM_CAL_POINTER, glbl_cal_status);
            glbl_gpo3 = (glbl_gpo3 & GPO3_CAL_STATUS_CLR) | (glbl_cal_status << GPO3_CAL_STATUS_POS) | GPO3_CAL_ERROR;
            writeIO_IOM(IO_GPO3, glbl_gpo3);
            DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): FAILED the calibration stage. Stage code = %0x Stage Name = %s \n", glbl_cal_stage[glbl_cal_index] & 0x3F, glbl_stage_code_string[glbl_cal_code]);
            return 1;
         } else {
            glbl_cal_status = (CAL_PASSED | (glbl_cal_stage[glbl_cal_index] & 0x3F));
            writeIO_XRAM((XRAM_CAL_SEQ_STATUS + 4*glbl_cal_index), glbl_cal_status);
            writeIO_XRAM(XRAM_CAL_POINTER, glbl_cal_status);
            glbl_gpo3 = (glbl_gpo3 & GPO3_CAL_STATUS_CLR) | (glbl_cal_status << GPO3_CAL_STATUS_POS) | GPO3_CAL_BUSY;
            writeIO_IOM(IO_GPO3, glbl_gpo3);
            DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): PASSED the calibration stage. Stage code = %0x Stage Name = %s \n", glbl_cal_stage[glbl_cal_index] & 0x3F, glbl_stage_code_string[glbl_cal_code]);
         }
      } // CAL_EN

      cal_stage_time = get_cal_stage_time();
      writeIO_XRAM(XRAM_CAL_STAGE_TIME + (glbl_cal_code * 4), cal_stage_time);

#ifdef TEST_VNC
      if((glbl_cal_stage[glbl_cal_index] == PHY_BISC) || (glbl_cal_stage[glbl_cal_index] == FREQ_SWITCH)) {
         writeIO_MBDR(RAM_DATA_CAL_OFFSET + 0xD00 + (cal_iter%64) * 4, (glbl_rl_dly_qtr[0] << 16) | cal_iter); // Total Calibration time
         cal_iter++;
      }

      if(glbl_cal_stage[glbl_cal_index] == FREQ_SWITCH) {
         cal_index_freq_switch = glbl_cal_index;
      }   

      if(!(glbl_cal_stage[glbl_cal_index] & CAL_SKIP)) {
         writeIO_MBDR(RAM_DATA_CAL_OFFSET + 0xE00 + (cal_stage_iter%64) * 4, (glbl_rl_dly_qtr[0] << 24) | (cal_stage_iter << 16) | (glbl_cal_index << 8) | (glbl_cal_stage[glbl_cal_index] & 0x3F)); // Total Calibration time
         cal_stage_iter++;
      }

      if((cal_iter < CAL_ITER_LIMIT) && (glbl_cal_stage[glbl_cal_index+1] == EN_VT_TRACK)) {
         glbl_cal_index = cal_index_freq_switch-1; // It is incremented in the for loop
      }
#endif

   } // For glbl_cal_index

   // Set the calibration done in XRAM
   glbl_cal_status = (CAL_IN_PROG | (glbl_cal_stage[glbl_cal_index] & 0x3F));
   writeIO_XRAM((XRAM_CAL_SEQ_STATUS + 4*glbl_cal_index), glbl_cal_status);
   writeIO_XRAM(XRAM_CAL_POINTER, glbl_cal_status);

   // Program the default values back into the controller
   writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_xpi_write_dm_dbi);
   #ifdef LPDDR4
     for(u8 rank=0; rank < RANKS; rank++) {
       lp4_mode_reg_wr(rank, 3, glbl_lp4_mr3);
       lp4_mode_reg_wr(rank, 13, glbl_lp4_mr13);
     }
   #endif

   status = temp_control_ref_rate();
   if(status != 0) {
      return 1;
   }

   DEBUG_PRINTF("INFO: ddr_cal(): Calibration completed. Dumping all the XRAM values\n");
   for(addr=0; addr<8192*4; addr+=4) {
      val = readIO_XRAM(addr);
      DEBUG_PRINTF("INFO: ddr_cal(): XRAM: addr = %x, data = %x\n", addr, val);
   }

   // Compute total calibration time
   pit1_cnt = readIO_IOM(IO_PIT1_COUNTER);
   cal_time = ((glbl_pit1_preload - pit1_cnt) * 2) / (10 * NOC_CLK_PERIOD); // Convert in terms of 10us
   writeIO_MBDR(RAM_DATA_CAL_OFFSET + 0xFFC, cal_time); // Total Calibration time

   // Compute calibration time excluding PRBS stages
   prbs_cal_time = readIO_XRAM(XRAM_CAL_STAGE_TIME + ((F0_PRBS_READ & 0x3F) * 4));
   cal_time -= prbs_cal_time;
   prbs_cal_time = readIO_XRAM(XRAM_CAL_STAGE_TIME + ((F0_PRBS_WRITE & 0x3F) * 4));
   cal_time -= prbs_cal_time;
   prbs_cal_time = readIO_XRAM(XRAM_CAL_STAGE_TIME + ((F1_PRBS_READ & 0x3F) * 4));
   cal_time -= prbs_cal_time;
   prbs_cal_time = readIO_XRAM(XRAM_CAL_STAGE_TIME + ((F1_PRBS_WRITE & 0x3F) * 4));
   cal_time -= prbs_cal_time;

   writeIO_XRAM(XRAM_CAL_TIME_8_0, (cal_time & 0x1FF));
   writeIO_XRAM(XRAM_CAL_TIME_17_9, ((cal_time >> 9) & 0x1FF));

   return glbl_first_error_code;
}

void hw_test_code () {
   #ifdef HW_TEST_CODE_DDR4
      PMC_defaults();
      //init_test();
      sequencer_activate_cmd();
      sequencer_single_write();
      sequencer_single_rd_cmd();
      sequencer_multi_rd_cmd();
      sequencer_multi_rd_cmd2();
      sequencer_single_write_read();
      sequencer_two_writes();
      sequencer_single_extend_write();
      sequencer_single_extend_extra_write();
      sequencer_two_extend_writes_one_read();
      sequencer_wrlvl_test();
      sequencer_precharge_cmd();
   #endif
   
   #ifdef HW_TEST_CODE_LPDDR4
     PMC_defaults_lpddr4();
     sequencer_activate_cmd_lpddr4();
     sequencer_single_write_lpddr4();
     sequencer_single_rd_cmd_lpddr4();
     sequencer_precharge_cmd_lpddr4();
   #endif
   
   #ifdef HW_TEST_CODE_LPDDR4
     PMC_defaults_lpddr4();
     sequencer_activate_cmd_lpddr4();
     sequencer_single_write_lpddr4();
     sequencer_single_rd_cmd_lpddr4();
     sequencer_precharge_cmd_lpddr4();
   #endif
   
   #ifdef HW_TEST_CODE_DDR4_AC_CAL
     sequencer_ac_cal_single();
   #endif
   
   #ifdef HW_TEST_CODE_LPDDR4_SLOW_INIT
   sequencer_setup_slow_ck(1); //Turn on
   lpddr4_init_test();
   sequencer_setup_slow_ck(0); //Turn off
   #endif
   
   #ifdef HW_TEST_RPI
   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_cal(): HW_TEST_RPI\n");
   rpi_rst_test();
   rpi_bcast_nibble_mask_test();
   rpi_adjustments_test();
   #endif

   #ifdef HW_TEST_SEQ_COVERAGE
   seq_coverage();
   #endif
}

void test_write_read () {
    // ACTIVATE Command
    set_1_seq_command_at_mem(0 /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, MEM_ROW_ACTIVE /*u32 seq_a_cmd*/,
                             0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/, 0 /*u32 seq_a2a_dly*/);

    writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 0xA);
    writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), (0xA << 3));

    writeIO_DMC(ral_addr_of_seq_dqs_default_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

    // WRITE Command
    writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_WRITE);

    writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
    while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

    // READ Command
    writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_READ);

    writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
    while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

    // PRECHARGE all banks
    writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x400);
    writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_ROW_PREACHARGE);
    writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
    while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

    return;
}
