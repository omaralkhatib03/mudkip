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
//       Revision:       $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/ddr_rd_vref_cal.c#1 $
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

#include "cal_reg_define.h"
#include "riu_reg_define.h"
#include "xsdb_reg_define.h"
#include "internal_reg_define.h"
#include "function_dec.c"
#include "global_variable_define.h"


/**************** End Used defines **************************/



u8 ddr_rd_vref_cal(u8 pattern_type) {

#ifdef SKIP_RD_VREF_CAL_STAGE
   if (pattern_type == SIMPLE) {
      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: rd_vref_cal():  READ DATA VREF CALIBRATION SKIPPED \n");
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: rd_vref_cal(): Loading precalculated delay values to skip the READ DATA VREF CALIBRATION \n");
      DEBUG_PRINTF("##################################################################################################################### \n");
      skip_ddr_rd_vref_cal(SIMPLE);
      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: rd_cal(): Loaded precalculated delay values to skip the READ DATA VREF CALIBRATION \n");
      DEBUG_PRINTF("##################################################################################################################### \n");
      return 0;
   }
#endif
#ifdef SKIP_RD_VREF_COMPLEX_CAL_STAGE
   if (pattern_type == COMPLEX) {
      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: rd_vref_cal():  READ DATA COMPLEX VREF CALIBRATION SKIPPED \n");
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: rd_vref_cal(): Loading precalculated delay values to skip the READ DATA COMPLEX VREF CALIBRATION \n");
      DEBUG_PRINTF("##################################################################################################################### \n");
      skip_ddr_rd_vref_cal(COMPLEX);
      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: rd_vref_cal(): Loaded precalculated delay values to skip the READ DATA COMPLEX VREF CALIBRATION \n");
      DEBUG_PRINTF("##################################################################################################################### \n");
      return 0;
   }
#endif

   if(glbl_tck >= TCK_TH_RD_VREF) {
      DEBUG_PRINTF("INFO: ddr_rd_vref_cal(): SKIPPING read vref stage for TCK = %d\n", glbl_tck);
      return 0;
   }

   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_rd_vref_cal(): READ DATA VREF CALIBRATION START \n");
   DEBUG_PRINTF("##################################################################################################################### \n");

   u8  status=0;

   /*************************************************************************************************/
   //   Unmask all the nibbles which are required for the read dq calibration
   /*************************************************************************************************/

   u8  nibble;
   u8  vref_step;
   u8  vref_step_size;
   u16 vref_initial; 
   u32 nibble_mask;
   u32 log_nibble_mask;
   u16 write_vref;
   u16 min_valid_win_new;
   u16 min_pqtr_valid_win[DQ_NIBBLES];
   u16 min_nqtr_valid_win[DQ_NIBBLES];
   u16 vref_min[DQ_NIBBLES];
   u16 vref_max[DQ_NIBBLES];
   u16 vref_center[DQ_NIBBLES];

   glbl_vref_cal = 1;
   nibble_mask      = glbl_dq_nibble_mask_initial;

   // Assuming that initial vref across all the data nibbles is same. So read the vref from any of the data nibble.
   vref_initial = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[0],VREF_CTRL) & 0x3FF; 
 
   // Initialize the minimum valid window thrishold
   for(nibble=0; nibble<DQ_NIBBLES; nibble++) {
      min_pqtr_valid_win[nibble] = 20; 
      min_nqtr_valid_win[nibble] = 20;
   }

   vref_step_size = 5;

   // Find Vref max
   nibble_mask      = glbl_dq_nibble_mask_initial;
   log_nibble_mask  = 0;
   writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~nibble_mask); 
   vref_step = 0;
   DEBUG_PRINTF("Started finiding upper limit of the Vref with nibble mask = 0x%08X, vref_step_size = %d, initial vref = 0x%04X \n",nibble_mask, vref_step_size, vref_initial); 
   do {
      DEBUG_PRINTF("Refresh all the banks \n"); 
      //banks_refresh();
      write_vref = vref_initial + vref_step*vref_step_size;
      if(write_vref >= (XPIO_VREF_MAX_LIMIT-vref_step_size)) {
         DEBUG_PRINTF("Reached the Vref upper limit \n");
         for(nibble=0, glbl_bit_pos = 1; nibble<DQ_NIBBLES; nibble++, glbl_bit_pos = glbl_bit_pos << 1) {
            if( !( (log_nibble_mask & glbl_bit_pos))) {
               nibble_mask = nibble_mask | (1<<glbl_dq_lnibble2pnibble_map[nibble]);
               log_nibble_mask = log_nibble_mask | (1<<nibble);
               vref_max[nibble] = write_vref-vref_step_size;
               DEBUG_PRINTF("Found vref_max[%d] = %d \n",nibble, vref_max[nibble]); 
            }
         }
         break;
      } 
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~nibble_mask); 
      riu_reg_bcast(VREF_CTRL,0,write_vref|0x400);
      status = ddr_rd_cal(COMPLEX);
      if(status) return 1;
      DEBUG_PRINTF("Read Vref = %d \n", write_vref); 
      for(nibble=0; nibble<DQ_NIBBLES; nibble++) {
            DEBUG_PRINTF("glbl_stable_valid_win[%d]=%d \n",nibble, glbl_stable_valid_win[nibble]); 
            DEBUG_PRINTF("glbl_stable_valid_win_nqtr[%d]=%d \n\n",nibble, glbl_stable_valid_win_nqtr[nibble]); 
      }
      for(nibble=0, glbl_bit_pos = 1; nibble<DQ_NIBBLES; nibble++, glbl_bit_pos = glbl_bit_pos << 1) {
         if(!(log_nibble_mask & glbl_bit_pos)) {

            // PQTR Vref calculation
            // valid_win_vref_pqtr_nibble[vref_step][nibble] = glbl_stable_valid_win[nibble];
            min_valid_win_new = 7*glbl_stable_valid_win[nibble]/10;
            if(min_valid_win_new > min_pqtr_valid_win[nibble]) {
               min_pqtr_valid_win[nibble] = min_valid_win_new; 
               DEBUG_PRINTF("Changed the min_pqtr_valid_win[%d] = %d \n",nibble, min_valid_win_new); 
            } 
            if(glbl_stable_valid_win[nibble] < min_pqtr_valid_win[nibble]) {
               nibble_mask      = nibble_mask          | (1<<glbl_dq_lnibble2pnibble_map[nibble]);
               log_nibble_mask  = log_nibble_mask      | (1<<nibble);
               vref_max[nibble] = write_vref-vref_step_size;
               DEBUG_PRINTF("Found vref_max[%d] = %d \n",nibble, vref_max[nibble]); 
            }

            // NQTR Vref calculation
            //valid_win_vref_nqtr_nibble[vref_step][nibble] = glbl_stable_valid_win_nqtr[nibble];
            min_valid_win_new = 7*glbl_stable_valid_win_nqtr[nibble]/10;
            if(min_valid_win_new > min_nqtr_valid_win[nibble]) {
               min_nqtr_valid_win[nibble] = min_valid_win_new; 
               DEBUG_PRINTF("Changed the min_nqtr_valid_win[%d] = %d \n",nibble, min_valid_win_new); 
            } 
            if(glbl_stable_valid_win_nqtr[nibble] < min_nqtr_valid_win[nibble]) {
               nibble_mask      = nibble_mask     | (1<<glbl_dq_lnibble2pnibble_map[nibble]);
               log_nibble_mask  = log_nibble_mask | (1<<nibble);
               vref_max[nibble] = write_vref-vref_step_size;
               DEBUG_PRINTF("Found vref_max[%d] = %d \n",nibble, vref_max[nibble]); 
            }
         }
      }

      vref_step++;

   } while(nibble_mask != 0xFFFFFFFF);
 
   // Find Vref min
   nibble_mask      = glbl_dq_nibble_mask_initial;
   log_nibble_mask  = 0;
   writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~nibble_mask); 
   vref_step = 1;
   DEBUG_PRINTF("Started finiding lower limit of the Vref with nibble mask = 0x%08X, vref_step_size = %d, initial vref = 0x%04X \n",nibble_mask, vref_step_size, vref_initial); 
   do {
      DEBUG_PRINTF("Refresh all the banks \n"); 
      //banks_refresh();
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~nibble_mask); 
      write_vref = vref_initial - vref_step*vref_step_size;
      if(write_vref <= (XPIO_VREF_MIN_LIMIT+vref_step_size)) {
         DEBUG_PRINTF("Reached the Vref lower limit \n");
         for(nibble=0, glbl_bit_pos = 1; nibble<DQ_NIBBLES; nibble++, glbl_bit_pos = glbl_bit_pos << 1) {
            if( !( (log_nibble_mask & glbl_bit_pos))) {
               nibble_mask = nibble_mask | (1<<glbl_dq_lnibble2pnibble_map[nibble]);
               log_nibble_mask = log_nibble_mask | (1<<nibble);
               vref_min[nibble] = write_vref+vref_step_size;
               DEBUG_PRINTF("Found vref_min[%d] = %d \n",nibble, vref_min[nibble]); 
            }
         }
         break;
      } 
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~nibble_mask); 
      riu_reg_bcast(VREF_CTRL,0,write_vref|0x400);
      status = ddr_rd_cal(COMPLEX);
      if(status) return 1;
      DEBUG_PRINTF("Read Vref = %d \n", write_vref); 
      for(nibble=0; nibble<DQ_NIBBLES; nibble++) {
            DEBUG_PRINTF("glbl_stable_valid_win[%d]=%d \n",nibble, glbl_stable_valid_win[nibble]); 
            DEBUG_PRINTF("glbl_stable_valid_win_nqtr[%d]=%d \n\n",nibble, glbl_stable_valid_win_nqtr[nibble]); 
      }
      for(nibble=0, glbl_bit_pos = 1; nibble<DQ_NIBBLES; nibble++, glbl_bit_pos = glbl_bit_pos << 1) {
         if(!(log_nibble_mask & glbl_bit_pos)) {
   
            // PQTR Vref calculation
            // valid_win_vref_pqtr_nibble[vref_step][nibble] = glbl_stable_valid_win[nibble];
            min_valid_win_new = 7*glbl_stable_valid_win[nibble]/10;
            if(min_valid_win_new > min_pqtr_valid_win[nibble]) {
               min_pqtr_valid_win[nibble] = min_valid_win_new; 
               DEBUG_PRINTF("Changed the min_pqtr_valid_win[%d] = %d \n",nibble, min_valid_win_new); 
            } 
            if(glbl_stable_valid_win[nibble] < min_pqtr_valid_win[nibble]) {
               nibble_mask      = nibble_mask | (1<<glbl_dq_lnibble2pnibble_map[nibble]);
               log_nibble_mask  = log_nibble_mask      | (1<<nibble);
               vref_min[nibble] = write_vref+vref_step_size;
               DEBUG_PRINTF("Found vref_min[%d] = %d \n",nibble, vref_min[nibble]); 
            }
   
            // NQTR Vref calculation
            // valid_win_vref_nqtr_nibble[vref_step][nibble] = glbl_stable_valid_win_nqtr[nibble];
            min_valid_win_new = 7*glbl_stable_valid_win_nqtr[nibble]/10;
            if(min_valid_win_new > min_nqtr_valid_win[nibble]) {
               min_nqtr_valid_win[nibble] = min_valid_win_new; 
               DEBUG_PRINTF("Changed the min_nqtr_valid_win[%d] = %d \n",nibble, min_valid_win_new); 
            } 
            if(glbl_stable_valid_win_nqtr[nibble] < min_nqtr_valid_win[nibble]) {
               nibble_mask      = nibble_mask | (1<<glbl_dq_lnibble2pnibble_map[nibble]);
               log_nibble_mask  = log_nibble_mask | (1<<nibble);
               vref_min[nibble] = write_vref+vref_step_size;
               DEBUG_PRINTF("Found vref_min[%d] = %d \n",nibble, vref_min[nibble]); 
            }
         }
      }

      vref_step++;


   } while(nibble_mask != 0xFFFFFFFF);

   // Final Vref loading
   DEBUG_PRINTF("Vref cal centering stage started \n"); 
   for(nibble=0; nibble<DQ_NIBBLES; nibble++) {
      vref_center[nibble] = (vref_max[nibble] + vref_min[nibble])/2;
      write_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble],VREF_CTRL,vref_center[nibble]|0x400);
      DEBUG_PRINTF("vref_min[%d] = %d , vref_max[%d]=%d, vref_center[%d]=%d \n",nibble, vref_min[nibble], nibble, vref_max[nibble], nibble, vref_center[nibble]); 
   }

   // Final read centering at the Final Vref Point.
   DEBUG_PRINTF("Horizontal centering started at final Vref point \n"); 
   status = ddr_rd_cal(COMPLEX);
   if(status) return 1;
   for(nibble=0; nibble<DQ_NIBBLES; nibble++) {
         DEBUG_PRINTF("Read Vref[%d] = %d \n", nibble, vref_center[nibble]); 
         DEBUG_PRINTF("glbl_stable_valid_win[%d]=%d \n",nibble, glbl_stable_valid_win[nibble]); 
         DEBUG_PRINTF("glbl_stable_valid_win_nqtr[%d]=%d \n\n",nibble, glbl_stable_valid_win_nqtr[nibble]); 
   }
   
   glbl_vref_cal = 0;

   DEBUG_PRINTF("Vref Calibration Passed \n"); 

   return 0;
}


/*********************************************************************************************************************************************/
// Function definations
/*********************************************************************************************************************************************/










