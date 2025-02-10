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
//       Revision:       $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/ddr_ca_vref_cal.c#1 $
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



u8 ddr_ca_vref_cal(u8 pattern_type) {

u8 status = 0;
#ifdef LPDDR4
#ifdef SKIP_CA_VREF_CAL_STAGE
   if (pattern_type == SIMPLE) {
      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ca_vref_cal():  CA VREF CALIBRATION SKIPPED \n");
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ca_vref_cal(): Loading precalculated delay values to skip the CA VREF CALIBRATION \n");
      DEBUG_PRINTF("##################################################################################################################### \n");
      skip_ddr_ca_vref_cal(SIMPLE);
      DEBUG_PRINTF("##################################################################################################################### \n");
      DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ca_vref_cal(): Loaded precalculated delay values to skip the CA VREF CALIBRATION \n");
      DEBUG_PRINTF("##################################################################################################################### \n");
      return 0;
   }
#endif
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_ca_vref_cal(): CA VREF CALIBRATION START \n");
   DEBUG_PRINTF("##################################################################################################################### \n");


   /*************************************************************************************************/
   //   Unmask all the bytes which are required for the read dq calibration
   /*************************************************************************************************/

   u8  byte;
   u8  vref_step;
   u8  vref_step_size;
   u16 vref_initial=glbl_lp4_mr12; 
   u32 byte_mask;
   u16 write_vref;
   u16 max_noise_win_new;
   u16 max_noise_win[CA_BITS/6];
   u16 vref_max[CA_BITS/6];
   u16 vref_min[CA_BITS/6];
   u16 vref_center[CA_BITS/6];
   u32 default_pattern;  
   u8 dq_3_0, dq_7_4;
   u32 dq_3_0_o=0, dq_7_4_o=0;
   u8 i;
   u32 wait;
   u8 all_bytes_done;

   glbl_vref_cal = 1;
   byte_mask      = 0;
   setup_ca_cal(0); 

   // Assuming that initial vref across all ca groups is same. So read the vref from any of the ca groups.
 
   // Initialize the minimum valid window thrishold
   for(byte=0; byte<(CA_BITS/6); byte++) {
      max_noise_win[byte] = 500; 
   }

   vref_step_size = 10;

   // Find Vref max
   vref_step = 0;
   DEBUG_PRINTF("Started finiding upper limit of the Vref with byte mask = 0x%08X, vref_step_size = %d, initial vref = 0x%04X \n",byte_mask, vref_step_size, vref_initial); 
   do {
      all_bytes_done=1;
      write_vref = vref_initial + vref_step*vref_step_size;
      for(u8 ca_group_idx=0;ca_group_idx<DQ_BITS/16;ca_group_idx++) {
         set_ca_vref(ca_group_idx,write_vref);
      }
      //dq_3_0 = write_vref & 0x0F;
      //dq_7_4 = (write_vref & 0xF0)>>4;
      //for(i=0;i<8;i++) {
      //   dq_3_0_o = dq_3_0 | (dq_3_0_o << 4);
      //   dq_7_4_o = dq_7_4 | (dq_7_4_o << 4);
      //}
      //DEBUG_PRINTF("dq_3_0 = 0x%08X dq_7_4 = 0x%08X \n",dq_3_0, dq_7_4); 
      //DEBUG_PRINTF("dq_3_0_o = 0x%08X dq_7_4_o = 0x%08X \n",dq_3_0_o, dq_7_4_o); 
      //writeIO_DMC(ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK),dq_3_0_o);
      //writeIO_DMC(ral_addr_of_seq_dq_nibble1_in_ddrmc_main_ddrmc_main_bank(BLK),dq_7_4_o);
      //writeIO_DMC(ral_addr_of_seq_dq_nibble4_in_ddrmc_main_ddrmc_main_bank(BLK),dq_3_0_o);
      //writeIO_DMC(ral_addr_of_seq_dq_nibble5_in_ddrmc_main_ddrmc_main_bank(BLK),dq_7_4_o);
      //writeIO_DMC(ral_addr_of_seq_dq_nibble8_in_ddrmc_main_ddrmc_main_bank(BLK),dq_3_0_o);
      //writeIO_DMC(ral_addr_of_seq_dq_nibble9_in_ddrmc_main_ddrmc_main_bank(BLK),dq_7_4_o);
      //writeIO_DMC(ral_addr_of_seq_dq_nibble12_in_ddrmc_main_ddrmc_main_bank(BLK),dq_3_0_o);
      //writeIO_DMC(ral_addr_of_seq_dq_nibble13_in_ddrmc_main_ddrmc_main_bank(BLK),dq_7_4_o);
      //writeIO_DMC(ral_addr_of_seq_dq_nibble16_in_ddrmc_main_ddrmc_main_bank(BLK),dq_3_0_o);
      //writeIO_DMC(ral_addr_of_seq_dq_nibble17_in_ddrmc_main_ddrmc_main_bank(BLK),dq_7_4_o);
      //default_pattern = readIO_DMC(DEFAULT_PATTERN);
      //writeIO_DMC(DEFAULT_PATTERN,default_pattern & 0xFFFF00FF);
      //writeIO_DMC(ral_addr_of_seq_dqs_default_in_ddrmc_main_ddrmc_main_bank(BLK),3);
      //writeIO_DMC(ral_addr_of_seq_dqs_default_in_ddrmc_main_ddrmc_main_bank(BLK),4);
      //wait = T_VREF_CA_LONG;
      //while(wait--);

      DEBUG_PRINTF("write_vref = %d \n",write_vref);
      if((write_vref & 0x3F) >= (CA_VREF_MAX_LIMIT-vref_step_size)) {
         DEBUG_PRINTF("Reached the Vref upper limit \n");
         for(byte=0; byte<(CA_BITS/6); byte++) {
            if( ! ( (byte_mask>>byte) & 0x1 ) ) {
               byte_mask = byte_mask | (1<<byte);
               vref_max[byte] = write_vref-vref_step_size;
               DEBUG_PRINTF("Found vref_max[%d] = %d \n",byte, vref_max[byte]); 
            }
         }
         break;
      } 
      status = ddr_ca_cal();
      for(byte=0; byte<(CA_BITS/6); byte++) {

         max_noise_win_new = 12*glbl_noise_win[byte]/10;
         DEBUG_PRINTF("glbl_noise_win[%d] = %d \n", byte, glbl_noise_win[byte]);
         if(max_noise_win_new < max_noise_win[byte]) {
            max_noise_win[byte] = max_noise_win_new; 
            DEBUG_PRINTF("Changed the max_noise_win[%d] = %d \n",byte, max_noise_win_new); 
         } 
         if( ! ( (byte_mask>>byte) & 0x1 ) ) {
            all_bytes_done=0;
            if(glbl_noise_win[byte] > max_noise_win[byte]) {
               byte_mask = byte_mask | (1<<byte);
               vref_max[byte] = write_vref-vref_step_size;
               DEBUG_PRINTF("Found vref_max[%d] = %d \n",byte, vref_max[byte]); 
            }
         }
      }

      vref_step++;

   } while(!all_bytes_done);
 
   // Find Vref min
   byte_mask      = 0;
   writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~byte_mask); 
   vref_step = 0;
   DEBUG_PRINTF("Started finiding lower limit of the Vref with byte mask = 0x%08X, vref_step_size = %d, initial vref = 0x%04X \n",byte_mask, vref_step_size, vref_initial); 
   do {
      all_bytes_done = 1;
      write_vref = vref_initial - vref_step*vref_step_size;
      for(u8 ca_group_idx=0;ca_group_idx<DQ_BITS/16;ca_group_idx++) {
         set_ca_vref(ca_group_idx,write_vref);
      }
      //dq_3_0 = write_vref & 0x0F;
      //dq_7_4 = (write_vref & 0xF0)>>4;
      //for(i=0;i<8;i++) {
      //   dq_3_0_o = dq_3_0 | (dq_3_0_o << 4);
      //   dq_7_4_o = dq_7_4 | (dq_7_4_o << 4);
      //}
      //writeIO_DMC(ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK),dq_3_0_o);
      //writeIO_DMC(ral_addr_of_seq_dq_nibble1_in_ddrmc_main_ddrmc_main_bank(BLK),dq_7_4_o);
      //writeIO_DMC(ral_addr_of_seq_dq_nibble4_in_ddrmc_main_ddrmc_main_bank(BLK),dq_3_0_o);
      //writeIO_DMC(ral_addr_of_seq_dq_nibble5_in_ddrmc_main_ddrmc_main_bank(BLK),dq_7_4_o);
      //writeIO_DMC(ral_addr_of_seq_dq_nibble8_in_ddrmc_main_ddrmc_main_bank(BLK),dq_3_0_o);
      //writeIO_DMC(ral_addr_of_seq_dq_nibble9_in_ddrmc_main_ddrmc_main_bank(BLK),dq_7_4_o);
      //writeIO_DMC(ral_addr_of_seq_dq_nibble12_in_ddrmc_main_ddrmc_main_bank(BLK),dq_3_0_o);
      //writeIO_DMC(ral_addr_of_seq_dq_nibble13_in_ddrmc_main_ddrmc_main_bank(BLK),dq_7_4_o);
      //writeIO_DMC(ral_addr_of_seq_dq_nibble16_in_ddrmc_main_ddrmc_main_bank(BLK),dq_3_0_o);
      //writeIO_DMC(ral_addr_of_seq_dq_nibble17_in_ddrmc_main_ddrmc_main_bank(BLK),dq_7_4_o);
      //default_pattern = readIO_DMC(DEFAULT_PATTERN);
      //writeIO_DMC(DEFAULT_PATTERN,default_pattern & 0xFFFF00FF);
      //writeIO_DMC(ral_addr_of_seq_dqs_default_in_ddrmc_main_ddrmc_main_bank(BLK),3);
      //writeIO_DMC(ral_addr_of_seq_dqs_default_in_ddrmc_main_ddrmc_main_bank(BLK),4);
      //wait = T_VREF_CA_LONG;
      //while(wait--);
      DEBUG_PRINTF("write_vref = %d \n",write_vref);
      if((write_vref & 0x3F) <= (CA_VREF_MIN_LIMIT+vref_step_size)) {
         DEBUG_PRINTF("Reached the Vref lower limit \n");
         for(byte=0; byte<(CA_BITS/6); byte++) {
            if( !( (byte_mask>>byte) & 0x1 ) ) {
               byte_mask = byte_mask | (1<<byte);
               vref_min[byte] = write_vref+vref_step_size;
               DEBUG_PRINTF("Found vref_min[%d] = %d \n",byte, vref_min[byte]); 
            }
         }
         break;
      } 
      status = ddr_ca_cal();
      for(byte=0; byte<(CA_BITS/6); byte++) {

         max_noise_win_new = 12*glbl_noise_win[byte]/10;
         DEBUG_PRINTF("glbl_noise_win[%d] = %d \n", byte, glbl_noise_win[byte]);
         if(max_noise_win_new < max_noise_win[byte]) {
            max_noise_win[byte] = max_noise_win_new; 
            DEBUG_PRINTF("Changed the max_noise_win[%d] = %d \n",byte, max_noise_win_new); 
         } 
         if( !( (byte_mask>>byte) & 0x1 ) ) {
            all_bytes_done = 0;
            if(glbl_noise_win[byte] > max_noise_win[byte]) {
               byte_mask = byte_mask | (1<<byte);
               vref_min[byte] = write_vref+vref_step_size;
               DEBUG_PRINTF("Found vref_min[%d] = %d \n",byte, vref_min[byte]); 
            }
         }
      }

      vref_step++;
   } while(!all_bytes_done);

   // Final Vref loading
   DEBUG_PRINTF("Vref cal centering stage started \n"); 
   for(byte=0; byte<(CA_BITS/6); byte++) {
      vref_center[byte] = (vref_max[byte] + vref_min[byte])/2;
      DEBUG_PRINTF("vref_min[%d] = %d , vref_max[%d] = %d, vref_center[%d] = %d \n",byte, vref_min[byte], byte, vref_max[byte], byte, vref_center[byte]); 
      if(PINOUT_SELECT==23) {
         set_ca_vref(byte*2,vref_center[byte]);
         set_ca_vref(byte*2+1,vref_center[byte]);
      } else {
         set_ca_vref(byte,vref_center[byte]);
      }
   }

   //default_pattern = readIO_DMC(DEFAULT_PATTERN);
   //writeIO_DMC(DEFAULT_PATTERN,default_pattern & 0xFFFF00FF);
   writeIO_DMC(ral_addr_of_seq_dqs_default_in_ddrmc_main_ddrmc_main_bank(BLK),3);
   writeIO_DMC(ral_addr_of_seq_dqs_default_in_ddrmc_main_ddrmc_main_bank(BLK),4);
   wait = T_VREF_CA_LONG;
   while(wait--);

   // Final read centering at the Final Vref Point.
   DEBUG_PRINTF("Horizontal centering started at final Vref point \n"); 
   status = ddr_ca_cal();
   release_ca_cs_training(0);
   

   DEBUG_PRINTF("CA Vref Calibration Passed \n"); 
#endif


   return 0;
}


/*********************************************************************************************************************************************/
// Function definations
/*********************************************************************************************************************************************/










