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
//       Revision:       $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/calibration.c#3 $
//                       $Author: rgaddam $
//                       $DateTime: 2019/04/25 11:28:04 $
//                       $Change: 21352856 $
//       Description:
//           DDRMC : Calibration top level file, which defines main function of the calibration software code.
//
///////////////////////////////////////////////////////////////////////////////


#ifndef ENABLE_MICROBLAZE_BFM
   #include "xparameters.h"
   #include "xiomodule.h"
   #include "xiomodule_l.h"
#else
   #include "ddr_standalone.h"
#endif
#define DEFINE_GLBL_VAR
#include "mc_reg_define.h"
#include "cal_reg_define.h"
#include "riu_reg_define.h"
#include "xsdb_reg_define.h"
#include "internal_reg_define.h"
#include "bd_d0e7_MC0_ddrc_0_phy_cal_config_define.h"
#include "bd_d0e7_MC0_ddrc_0_phy_io_config_define.h"
#include "function_dec.c"
#include "global_variable_define.h"
#include "register_dump.h"

#ifdef ENABLE_MICROBLAZE_BFM
int fd_request;
int fd_response;
int fd_response_time;
#endif

#ifndef CALIB_PATH
   #define  CALIB_PATH "skip_cal_data.h" 
#endif
 
int main() {
#ifdef ENABLE_MICROBLAZE_BFM
   printf("INFO: main()\n");
   #ifdef MICROBLAZE_BFM_0
     DebugLogFile = fopen("DEBUG_CalibrationModule_0.log","w");  // NOTE: Need to open logfile _before_ calling initialize_calibration_module()
   #elif MICROBLAZE_BFM_1
     DebugLogFile = fopen("DEBUG_CalibrationModule_1.log","w");  // NOTE: Need to open logfile _before_ calling initialize_calibration_module()
   #elif MICROBLAZE_BFM_2
     DebugLogFile = fopen("DEBUG_CalibrationModule_2.log","w");  // NOTE: Need to open logfile _before_ calling initialize_calibration_module()
   #elif MICROBLAZE_BFM_3
     DebugLogFile = fopen("DEBUG_CalibrationModule_3.log","w");  // NOTE: Need to open logfile _before_ calling initialize_calibration_module()
   #else
     DebugLogFile = fopen("DEBUG_CalibrationModule_0.log","w");  // NOTE: Need to open logfile _before_ calling initialize_calibration_module()
   #endif
   calib_stg_dly_log= fopen(CALIB_PATH,"w"); 
   initialize_calibration_module();                          // setup communication channels
   fprintf(DebugLogFile, "INFO: Hello World!\n");
   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: main(): $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/calibration.c#3 $\n");
   fprintf(calib_stg_dly_log, "//INFO: Hello World!\n");
   RECORD_DLY("//Recorded the delays\n");
#endif

   volatile XStatus status = 0;
   int vmm_ral_status;

   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: main(): MICROBLAZE_START\n");
   glbl_gpo3 = (0x1 << GPO3_UB_AWAKE_POS);
   writeIO_IOM(IO_GPO3, glbl_gpo3);

   // Set the global variables
   set_globals();
#ifdef COMPARE_REGVAL
   register_dump();
#endif
   // Disable locked_deskew_2nd - Workaround for CR-1030808
   u8 xpll_nibble = BANK0_PLL0_RPI_NIBBLE + 2; // Bank-1, XPLL-0
   write_riu_nibble_reg(xpll_nibble, 0x22, 0x7); // 0x88

   // Set the PLL clkoutphy enable
   status = pll_clkoutphy_en(1);

   if(status == 0) { // Check if XPLLs locked
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: main(): XPLLs are locked\n");
      glbl_gpo3 = (glbl_gpo3 | (0x1 << GPO3_XPLL_LOCK));
      writeIO_IOM(IO_GPO3, glbl_gpo3);

      if(glbl_cal_mode_bypass == 1) { // Calibration BYPASS
         status = ddr_self_calibrate(1);
         if (status == 0) {
            DEBUG_PRINTF("INFO: CALIBRATION_MODULE: main(): Self Calibration has completed\n");
            status = ddr_init_f0();
            if (status == 0) {
               DEBUG_PRINTF("INFO: CALIBRATION_MODULE: main(): Memory initialization has completed\n");
               writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
               bitslice_reset_bcast(0, BS_RESET_WT); // Needed for XPHY loopback test
            } else {
               DEBUG_PRINTF("ERROR: CALIBRATION_MODULE: main(): Memory initialization has FAILED\n");
            }
         } else {
            DEBUG_PRINTF("ERROR: CALIBRATION_MODULE: main(): Self Calibration has FAILED\n");
         }

      } else if(glbl_restore_cal_en == 1) { // Restore Enable
         DEBUG_PRINTF("INFO: CALIBRATION_MODULE: main(): started restore: calling cal_restore()\n");
         status = cal_restore();

         if (status == 0){ // Restore PASS
            DEBUG_PRINTF("INFO: CALIBRATION_MODULE: main(): Restore PASSED\n");
         } else { // Restore FAIL
            glbl_post_cal_error = 1;
            glbl_gpo3 |= ((1 << (POST_CAL_STATUS_POWER_DOWN_RESTORE_ERROR + GPO3_POST_CAL_SELF_REF_POS)) | GPO3_POST_CAL_ERROR);
            writeIO_IOM(IO_GPO3, glbl_gpo3);
            DEBUG_PRINTF("ERROR: CALIBRATION_MODULE: main(): Restore FAILED\n");
            return 1;
         } 

      } else { // Full Calibration Enable

         DEBUG_PRINTF("INFO: CALIBRATION_MODULE: main(): started calibration: calling ddr_cal() \n");
         status = ddr_cal();

         if (status == 0){ // Calibration PASS
            DEBUG_PRINTF("INFO: CALIBRATION_MODULE: main(): Calibration PASSED\n");
         } else { // Calibration FAIL
            DEBUG_PRINTF("ERROR: CALIBRATION_MODULE: main(): Calibration FAILED\n");
            glbl_cal_status = (CAL_FAILED | (glbl_cal_stage[glbl_cal_index] & 0x3F));
            writeIO_XRAM((XRAM_CAL_SEQ_STATUS + 4*glbl_cal_index), glbl_cal_status);
            writeIO_XRAM(XRAM_CAL_POINTER, glbl_cal_status);
            glbl_gpo3 = (glbl_gpo3 & GPO3_CAL_STATUS_CLR) | (glbl_cal_status << GPO3_CAL_STATUS_POS) | GPO3_CAL_ERROR;
            writeIO_IOM(IO_GPO3, glbl_gpo3);
            return 1;
         } 
      }

      if((glbl_restore_cal_en == 0) && (status == 0)) { // Power up calibration only
         // Set the calibration done in DDRMC
         writeIO_DMC(ral_addr_of_cal_status_in_ddrmc_main_ddrmc_main_bank(BLK), 0x3);

         if(glbl_cal_mode_bypass == 1) { // Calibration BYPASS
            // Disable the ALERT interrupts and DQS Oscillator interrupts while cal_done = 1
            writeIO_IOM(IO_IRQ_ENABLE, (IRQ_EN & ~IRQ_EN_ALERT & ~IRQ_EN_DQS_OSC));
         } else { // Full Calibration Enable
            // Disable the ALERT interrupts while cal_done = 1
            writeIO_IOM(IO_IRQ_ENABLE, (IRQ_EN & ~IRQ_EN_ALERT));
         }

      #ifdef FIX_EDT_999807
         status = en_mission_mode_tracking();
      #endif
         if(status) {
            DEBUG_PRINTF("ERROR: CALIBRATION_MODULE: main(): Enabling mission mode tracking FAILED\n");
            glbl_cal_status = (CAL_FAILED | (glbl_cal_stage[glbl_cal_index] & 0x3F));
            writeIO_XRAM((XRAM_CAL_SEQ_STATUS + 4*glbl_cal_index), glbl_cal_status);
            writeIO_XRAM(XRAM_CAL_POINTER, glbl_cal_status);
            glbl_gpo3 = (glbl_gpo3 & GPO3_CAL_STATUS_CLR) | (glbl_cal_status << GPO3_CAL_STATUS_POS) | GPO3_CAL_ERROR;
            writeIO_IOM(IO_GPO3, glbl_gpo3);
            return 1;
         } else {
            glbl_cal_done = 1;
            glbl_cal_status = (CAL_PASSED | (glbl_cal_stage[glbl_cal_index] & 0x3F));
            writeIO_XRAM((XRAM_CAL_SEQ_STATUS + 4*glbl_cal_index), glbl_cal_status);
            writeIO_XRAM(XRAM_CAL_POINTER, glbl_cal_status);
            glbl_gpo3 = (glbl_gpo3 & GPO3_CAL_STATUS_CLR) | (glbl_cal_status << GPO3_CAL_STATUS_POS) | GPO3_CAL_DONE;
            writeIO_IOM(IO_GPO3, glbl_gpo3);
	    read_def_vref();
         }
      }

   } else {
      DEBUG_PRINTF("ERROR: CALIBRATION_MODULE: main(): XPLLs didn't lock\n");
      DEBUG_PRINTF("ERROR: CALIBRATION_MODULE: main(): Calibration FAILED\n");
      glbl_gpo3 = (glbl_gpo3 | GPO3_CAL_ERROR);
      writeIO_IOM(IO_GPO3, glbl_gpo3);
      return 1;
   }

//#ifdef ENABLE_MICROBLAZE_BFM 
//   DEBUG_PRINTF( "INFO: CALIBRATION_MODULE: main(): sending DONE indication to MicroBlaze BFM\n", status);
//   send_done(status);
//
//#else
   // This is an indefinite loop that runs on interrupts and it exits only in case of an ERROR
   if(status == 0) { // Calibration passed
#ifdef COMPARE_REGVAL
   register_compare();
#endif
      status = post_cal();
   }

   // There is an ERROR in post calibration, i.e. in mission mode
   while (1) {
      glbl_heartbeat = (glbl_heartbeat == 0) ? 1 : 0;
      glbl_gpo3 = glbl_heartbeat ? (glbl_gpo3 | (1 << GPO3_HEARTBEAT_POS)) : (glbl_gpo3 & ~(1 << GPO3_HEARTBEAT_POS));
      writeIO_IOM(IO_GPO3, glbl_gpo3);
      wait_idle_heartbeat();
   }
//#endif

   return status;
}
