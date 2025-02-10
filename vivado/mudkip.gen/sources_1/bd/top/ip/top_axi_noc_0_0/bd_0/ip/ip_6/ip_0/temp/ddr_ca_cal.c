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
//       Revision:       $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/ddr_ca_cal.c#2 $
//                       $Author: rgaddam $
//                       $DateTime: 2019/04/25 11:28:04 $
//                       $Change: 21352856 $
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

#ifdef LPDDR4
u8 ca_cs_center(u8 rank, u8 compare_data_bits, u8 compare_data_type); 
void setup_ca_cal(u8 rank); 
void release_ca_cs_training(u8 rank); 

u8 ddr_ca_cal () {
#ifndef VNC_DIS_CA_CAL
   u8 lb_loop;
   for(lb_loop=0; lb_loop<DQ_BITS; lb_loop++) {
      DEBUG_PRINTF("INFO: set_globals(): Logical DQ[%d] is mapped to Physical DQ[%d] \n", lb_loop, glbl_dq_lbit2pbit_map[lb_loop]);
   }
   u8  status = 0;
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: CALIBRATION_MODULE: ddr_ca_cal(): CA CALIBRATION START \n");
   DEBUG_PRINTF("##################################################################################################################### \n");
 
   u8  rank=RANKS;

   u8  step= 1;
   u8  ck_bit;

   u16 iteration = 0;

   u16 step_size_coarse = FINE_CRS_STEP_SIZE; 
   u16 nos_of_samples = SAMPLE_CNT+2;

   glbl_bits_error_status_31_0;
   glbl_bit_mask_31_0=0;




   if(!glbl_vref_cal){
      setup_ca_cal(0); 
   }
   writeIO_DMC(ral_addr_of_seq_cs_cal_in_ddrmc_main_ddrmc_main_bank(BLK),0x0000);
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: ddr_ca_cal(): Detect left edge while moving CA lines for rank = %d with nibble mask = 0x%08X\n", rank, glbl_nibble_mask);
   DEBUG_PRINTF("##################################################################################################################### \n");
  
   // Addeing delay tCAENT, CKE 1->0 to the Vref update minimum delay
   // Update the CA Vref with default value 
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), T_CAENT);
   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
   writeIO_DMC(ral_addr_of_seq_dqs_default_in_ddrmc_main_ddrmc_main_bank(BLK),3);
   writeIO_DMC(ral_addr_of_seq_dqs_default_in_ddrmc_main_ddrmc_main_bank(BLK),4);
   //writeIO_DMC(DEFAULT_PATTERN,default_pattern);
   //writeIO_DMC(DEFAULT_PATTERN,default_pattern & 0xFFFF00FF);

   // Adding delay tVREFca_Long, the Vref update to the CS assertion minimum delay
   // Start assering the CS periodically
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), T_VREF_CA_LONG);
   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
   writeIO_DMC(ral_addr_of_seq_cs_cal_in_ddrmc_main_ddrmc_main_bank(BLK),0x0180);

   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), nos_of_samples| (nos_of_samples<<16));
   writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK) , T_CACD); // Setup the tCACD, CS to CS assertion minimum delay


//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
   // Start CA Calibration 
   DEBUG_PRINTF("INFO: ddr_ca_cal(): STARTED CA Calibration \n");
   glbl_nibble_mask_initial = glbl_ca_nibble_mask_initial; 
   glbl_nibble_mask = glbl_nibble_mask_initial; 
   DEBUG_PRINTF("rd_cal(): updated the nibble mask for the nibbles = 0x%08X \n", glbl_nibble_mask);
   u8 bit;
   for(bit=0; bit < CA_BITS; bit++) {
      glbl_bits_riu_map[bit][0]  = glbl_ca_bits_riu_map[bit][0];
      glbl_bits_riu_map[bit][1]  = glbl_ca_bits_riu_map[bit][1];
      glbl_lbit2pnibble_map[bit] = glbl_ca_lbit2pnibble_map[bit];
      glbl_odly_riu_addr_map[bit] = glbl_ca_odly_riu_addr_map[bit];
   }
   status = ca_cs_center(0, CA_BITS, BITS);
   if(status==1)
      return 1;
   u16 xpi_map_bitslip = readIO_DMC(ral_addr_of_xpi_map_bitslip_in_ddrmc_main_ddrmc_main_bank(BLK));
   if(status==0)
	   writeIO_DMC(ral_addr_of_xpi_map_bitslip_in_ddrmc_main_ddrmc_main_bank(BLK),xpi_map_bitslip | 0x3);
   DEBUG_PRINTF("INFO: ddr_ca_cal(): PASSED the CA Calibration \n");

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
   if(!glbl_vref_cal) {
      // Start CS Calibration 
      DEBUG_PRINTF("INFO: ddr_ca_cal(): STARTED CS Calibration \n");
      for(u8 rank=0;rank<RANKS;rank++) { 
         if(rank>0) {
            xpi_map_bitslip = readIO_DMC(ral_addr_of_xpi_map_bitslip_in_ddrmc_main_ddrmc_main_bank(BLK));
            writeIO_DMC(ral_addr_of_xpi_map_bitslip_in_ddrmc_main_ddrmc_main_bank(BLK),xpi_map_bitslip & 0x03);
            setup_ca_cal(rank); 
            // Addeing delay tCAENT, CKE 1->0 to the Vref update minimum delay
            // Update the CA Vref with default value 
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
            writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), T_CAENT);
            writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
            while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
            writeIO_DMC(ral_addr_of_seq_dqs_default_in_ddrmc_main_ddrmc_main_bank(BLK),3);
            writeIO_DMC(ral_addr_of_seq_dqs_default_in_ddrmc_main_ddrmc_main_bank(BLK),4);
            // Adding delay tVREFca_Long, the Vref update to the CS assertion minimum delay
            // Start assering the CS periodically
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
            writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), T_VREF_CA_LONG);
            writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
            while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
            writeIO_DMC(ral_addr_of_seq_cs_cal_in_ddrmc_main_ddrmc_main_bank(BLK),0x0180);

            writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), nos_of_samples| (nos_of_samples<<16));
            writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK) , T_CACD); // Setup the tCACD, CS to CS assertion minimum delay
         }
         glbl_cs_nibble_mask_initial = 0;
         for(u8 i=0,bit=rank*LP4_MAX_CS_BITS; i<CS_BITS; i++,bit++) {
            glbl_bits_riu_map[i][0]  = glbl_cs_bits_riu_map[bit][0];
            glbl_bits_riu_map[i][1]  = glbl_cs_bits_riu_map[bit][1];
            glbl_lbit2pnibble_map[i] = glbl_cs_lbit2pnibble_map[bit];
            glbl_odly_riu_addr_map[i] = glbl_cs_odly_riu_addr_map[bit];
            glbl_cs_nibble_mask_initial = glbl_cs_nibble_mask_initial | (1<<glbl_lbit2pnibble_map[i]);
            DEBUG_PRINTF("ca_cal(): glbl_cs_lbit2pnibble_map[%d]=%d, glbl_lbit2pnibble_map[%d] = %d \n", bit,glbl_cs_lbit2pnibble_map[bit], i, glbl_lbit2pnibble_map[i]);
         }
         glbl_cs_nibble_mask_initial = ~glbl_cs_nibble_mask_initial;
         glbl_nibble_mask_initial = glbl_cs_nibble_mask_initial; 
         glbl_nibble_mask = glbl_nibble_mask_initial; 
         DEBUG_PRINTF("rd_cal(): updated the nibble mask for the nibbles = 0x%08X \n", glbl_nibble_mask);
         status = ca_cs_center(rank, CS_BITS, BYTES);
         if(status==1)
            return 1;
         xpi_map_bitslip = readIO_DMC(ral_addr_of_xpi_map_bitslip_in_ddrmc_main_ddrmc_main_bank(BLK));
         if(status == 0)
            writeIO_DMC(ral_addr_of_xpi_map_bitslip_in_ddrmc_main_ddrmc_main_bank(BLK),xpi_map_bitslip | 0x3FFC);
         release_ca_cs_training(rank);
         DEBUG_PRINTF("INFO: ddr_ca_cal(): PASSED the CS Calibration for Rank = %d \n",rank);
      }   
   }
//-----------------------------------------------------------------------------------------------------------------------------------------------------------
   if(glbl_vref_cal) {
      if (status==1)
         return 1;
      else
         return 0;
   }
//-----------------------------------------------------------------------------------------------------------------------------------------------------------
   // Before exiting put back the data bs reset mask setting.
   set_data_bs_rst_mask();
#endif // VNC_DIS_CA_CAL
   return 0;
}
   u8 ca_cs_center(u8 rank, u8 compare_data_bits, u8 compare_data_type) {
      u8 bit;
      u8 delay_type=CA_DELAY;
      u8 status;
      u8  step= 1;
      for(bit=0; bit < compare_data_bits; bit++) {
         glbl_stable_valid_win[bit] = 0;
      }
      glbl_exptd_pt = 7; //0-10101010 3-00000000 4-01010101  7-11111111
      writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK),glbl_exptd_pt); 
      if(glbl_vref_cal) {
         writeIO_DMC(ral_addr_of_xpi_map_bitslip_in_ddrmc_main_ddrmc_main_bank(BLK),0x3FFC);
         DEBUG_PRINTF("INFO: ddr_ca_cal(): Detect Valid region on right side of the noise window \n");
         glbl_phase_no = 0x2C;
         glbl_region_type = PARTIAL;
         glbl_delay_type  = delay_type;
         glbl_inc_dec_load = DEC;
         glbl_step_size = 10;
         glbl_compare_edge_type = RISE;
         glbl_compare_data_type = compare_data_type;
         glbl_compare_data_bits = compare_data_bits;
         glbl_dual_edge_region = 0;
         glbl_pattern_type = SIMPLE;
         status = region_detect(); 
         if(status == 1) {
            DEBUG_PRINTF("INFO: ddr_ca_cal(): Revertback the ca delay by the same coarse taps \n");
            writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask_initial); 
            initialize_u16_array(glbl_pnibble2pbs_bcast_map,27,0xFFFF);
            set_nibble2bcast_bits( 1, compare_data_bits);
            for(step=0; step<glbl_step_size ; step++) {
               riu_reg_bcast(IODLY_BCAST_CTRL,0,(0x1<<ODLY_BCAST_EN_POS)|(0x1<<ODLY_INC_POS)); 
            }
         }
      }
      if(!glbl_vref_cal) {

         DEBUG_PRINTF("INFO: ddr_ca_cal(): Detect Right edge of the noise window using big jumps of the fine taps \n");

         glbl_bit_mask_31_0 = 0;
         glbl_delay_stage = FINE;
            glbl_clear_error = 0;

            glbl_phase_no = 0x1A;
            glbl_region_type = FULL;
            glbl_delay_type  = delay_type;
            glbl_inc_dec_load = INC;
            glbl_step_size = 10;
            glbl_compare_edge_type = RISE;
            glbl_compare_data_type = compare_data_type;
            glbl_compare_data_bits = compare_data_bits;
            glbl_dual_edge_region = 0;
            glbl_pattern_type = SIMPLE;
            status = region_detect();
            if(status) {
               DEBUG_PRINTF("ERROR: ddr_ca_cal(): Delay taps got saturated \n");
// XRAM_ERROR: ca_odelay_crse_noise_right - noise detection - per channel ca bit - rank 0 only
// XRAM_ERROR: cs_odelay_crse_noise_right - noise detection - per channel/rank cs
               write_error_nibble_XRAM(0, glbl_bits_error_status_31_0);
               if(glbl_compare_data_type == BITS) {
                  write_error_XRAM(ERR_CODE_116_CA_CAL_FAIL_NOISE_RIGHT_CA_ODELAY_HIGHER_TAPS);
               } else {
                  write_error_XRAM(ERR_CODE_119_CA_CAL_FAIL_NOISE_RIGHT_CS_ODELAY_HIGHER_TAPS);
               }
               return 1;
            }

            DEBUG_PRINTF("INFO: ddr_ca_cal(): Revertback to last coarse step  \n");
            writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask_initial);
            initialize_u16_array(glbl_pnibble2pbs_bcast_map,27,0xFFFF);
            set_nibble2bcast_bits( 1, compare_data_bits);
            for(step=0; step<(glbl_step_size) ; step++) {
               riu_reg_bcast(IODLY_BCAST_CTRL,0,(0x1<<ODLY_BCAST_EN_POS));
            }
      }
   
      DEBUG_PRINTF("INFO: ddr_ca_cal(): Detect Right edge of the noise window using the fine taps \n");
         glbl_phase_no = 0x1C;
         glbl_region_type = FULL;
         glbl_delay_type  = delay_type;
         glbl_inc_dec_load = INC;
         glbl_step_size = 1;
         glbl_compare_edge_type = RISE;
         glbl_compare_data_type = compare_data_type;
         glbl_compare_data_bits = compare_data_bits;
         glbl_dual_edge_region = 0;
         glbl_pattern_type = SIMPLE;
         status = region_detect();
         if(status) {
            DEBUG_PRINTF("ERROR: ddr_ca_cal(): Delay taps got saturated \n");
// XRAM_ERROR: ca_odelay_fine_noise_right - noise detection - per channel ca bit - rank 0 only
// XRAM_ERROR: cs_odelay_fine_noise_right - noise detection - per channel/rank cs
            write_error_nibble_XRAM(0, glbl_bits_error_status_31_0);
            if(glbl_compare_data_type == BITS) {
               write_error_XRAM(ERR_CODE_117_CA_CAL_FAIL_NOISE_RIGHT_CA_ODELAY);
            } else {
               write_error_XRAM(ERR_CODE_120_CA_CAL_FAIL_NOISE_RIGHT_CS_ODELAY);
            }
            return 1;
         }
   
      DEBUG_PRINTF("INFO: ddr_ca_cal(): Record the initial fine delay of ca lines, right edge of the noise window \n");
         static u16 delay_init[CA_BITS];
         for(bit=0; bit<compare_data_bits; bit++) {
            delay_init[bit] = readIO_RIU(glbl_odly_riu_addr_map[bit]);
            DEBUG_PRINTF("INFO: ddr_ca_cal(): delay_init[%d] = %d \n", bit, delay_init[bit]);
         }
   
      DEBUG_PRINTF("INFO: ddr_ca_cal(): Detect Left edge of the noise window using big jumps of the fine taps \n");
         glbl_exptd_pt = 3; //0-10101010 3-00000000 4-01010101  7-11111111
         writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK),glbl_exptd_pt); 
         glbl_clear_error = 1;
         glbl_delay_stage = FINE;

         glbl_phase_no = 0x2A;
         glbl_region_type = PARTIAL;
         glbl_delay_type  = delay_type;
         glbl_inc_dec_load = INC;
         glbl_step_size = 10;
         glbl_compare_edge_type = RISE;
         glbl_compare_data_type = compare_data_type;
         glbl_compare_data_bits = compare_data_bits;
         glbl_dual_edge_region = 0;
         glbl_pattern_type = SIMPLE;
         status = region_detect(); 
         if(status == 0) {
            DEBUG_PRINTF("INFO: ddr_ca_cal(): Revertback to last coarse step  \n");
            writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask_initial); 
            initialize_u16_array(glbl_pnibble2pbs_bcast_map,27,0xFFFF);
            set_nibble2bcast_bits( 1, compare_data_bits);
            for(step=0; step<(glbl_step_size) ; step++) {
               riu_reg_bcast(IODLY_BCAST_CTRL,0,(0x1<<ODLY_BCAST_EN_POS));
            }
            for(bit=0; bit < compare_data_bits; bit++) {
               glbl_stable_valid_win[bit] = 0;
            }
   
            DEBUG_PRINTF("INFO: ddr_ca_cal(): Detect Left edge of the noise window using the fine taps \n");
            //glbl_exptd_pt = 3; //0-10101010 3-00000000 4-01010101  7-11111111
            //writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK),glbl_exptd_pt); 
            //glbl_clear_error = 1;

            glbl_phase_no = 0x2C;
            glbl_region_type = PARTIAL;
            glbl_delay_type  = delay_type;
            glbl_inc_dec_load = INC;
            glbl_step_size = 1;
            glbl_compare_edge_type = RISE;
            glbl_compare_data_type = compare_data_type;
            glbl_compare_data_bits = compare_data_bits;
            glbl_dual_edge_region = 0;
            glbl_pattern_type = SIMPLE;
            status = region_detect(); 
            if(status == 0) {
               DEBUG_PRINTF("INFO: ddr_ca_cal(): Revertback the ca delay by the MIN_VALID counts  \n");
               writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask_initial); 
               riu_reg_bcast(INCDEC_CRSE,0,MIN_VLD_CNT-1); 
               initialize_u16_array(glbl_pnibble2pbs_bcast_map,27,0xFFFF);
               set_nibble2bcast_bits( 1, compare_data_bits);
               for(step=0; step<(MIN_VLD_CNT-1) ; step++) {
                  riu_reg_bcast(IODLY_BCAST_CTRL,0,(0x1<<ODLY_BCAST_EN_POS)); 
               }
            }
// XRAM_WARNING: ca_odelay_fine_noise_left - noise detection - per channel ca bit - rank 0 only
// XRAM_WARNING: cs_odelay_fine_noise_left - noise detection - per channel/rank cs
         }
// XRAM_WARNING: ca_odelay_crse_noise_left - noise detection - per channel ca bit - rank 0 only
// XRAM_WARNING: cs_odelay_crse_noise_left - noise detection - per channel/rank cs
   
      DEBUG_PRINTF("INFO: ddr_ca_cal(): Record the end fine delay of ca lines, left edge of the noise window \n");
         static u16 delay_end[CA_BITS];
         for(bit=0; bit<compare_data_bits; bit++) {
            delay_end[bit] = readIO_RIU(glbl_odly_riu_addr_map[bit]);
            DEBUG_PRINTF("INFO: ddr_ca_cal(): delay_end[%d] = %d \n", bit, delay_end[bit]);
         }
   
      DEBUG_PRINTF("INFO: ddr_ca_cal(): Record the final fine delay of ca lines, center of the noise window \n");
         static u16 delay_final[CA_BITS];
         static u16 delay_change[CA_BITS];
         u8 bit_slip_en=1;
         for(bit=0; bit<compare_data_bits; bit++) {
            delay_final[bit] = (delay_init[bit] + delay_end[bit])/2;
            glbl_noise_win[bit] = delay_end[bit]-delay_init[bit];
         }
         for(bit=0; bit<compare_data_bits; bit++) {
            if(compare_data_type == BITS) {
               //delay_change[bit/6]=glbl_noise_win[bit]/2;
               if(bit_slip_en)
                  delay_change[bit]=glbl_noise_win[bit]/2;
               else
            	  delay_change[bit]=glbl_noise_win[bit]/2+glbl_rl_dly_qtr_max*2;
            } else {
            	if(bit_slip_en)
            		delay_change[bit]=glbl_noise_win[bit]/2;
            	else
            		delay_change[bit]=glbl_noise_win[bit]/2+glbl_rl_dly_qtr_max*2;
            }
            //writeIO_RIU(glbl_odly_riu_addr_map[bit], delay_final[bit]);
            DEBUG_PRINTF("INFO: ddr_ca_cal(): delay_final[%d] = %d noise_win[%d] = %d \n", bit, delay_final[bit], bit, glbl_noise_win[bit]);
         }

         writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask_initial); 
         //riu_reg_bcast(CRSE_DLY,0,1); 
         riu_reg_bcast(INCDEC_CRSE,0,1); 
         initialize_u16_array(glbl_pnibble2pbs_bcast_map,27,0xFFFF);
         set_nibble2bcast_bits( 1, compare_data_bits);
         glbl_bit_mask_31_0 = 0xFFFFFFFF;
         glbl_bit_mask_31_0_last = 0x00000000;
         u16 iteration = 0;
         u8 bits;
//         if(compare_data_type == BITS) {
//            bits = compare_data_bits/6;
//         } else {
            bits = compare_data_bits;
//         }
         u8 exit_loop=1;
         do {
            iteration++;
            DEBUG_PRINTF("############################################################################################ \n");
            DEBUG_PRINTF("################################ ca_cs_cal(): centering phase iteration number = %d ############## \n", iteration);
            exit_loop = 1;
            for(bit=0; bit<bits; bit++) {
               DEBUG_PRINTF("INFO: delay_change[%d] = %d \n", bit, delay_change[bit]);
               if(delay_change[bit]>0) {
                  exit_loop=0;
                  delay_change[bit]--;
                  if(compare_data_type == BITS) {
                     glbl_bit_mask_31_0 = glbl_bit_mask_31_0 & (~(0x3F<<(bit*6)));
                  } else {
                     glbl_bit_mask_31_0 = glbl_bit_mask_31_0 & (~(0x1<<bit));
                  }
               } else {
                  if(compare_data_type == BITS) {
                     glbl_bit_mask_31_0 = glbl_bit_mask_31_0 | (0x3F<<(bit*6));
                  } else {
                     glbl_bit_mask_31_0 = glbl_bit_mask_31_0 | (0x1<<bit);
                  }
               } 
            }
            DEBUG_PRINTF("INFO: logical nibble mask for 17_0 = 0x%08X, last logical mask for 17_0 = 0x%08X \n", glbl_bit_mask_31_0, glbl_bit_mask_31_0_last);
            if(glbl_bit_mask_31_0 != glbl_bit_mask_31_0_last) {
               set_nibble2bcast_bits(0, compare_data_bits);
               glbl_bit_mask_31_0_last = glbl_bit_mask_31_0;
               //Mask the nibbles for which the phase has been completed.
            }
            riu_reg_bcast(IODLY_BCAST_CTRL,0,(0x1<<ODLY_BCAST_EN_POS)); 
            if(iteration>500) {
// XRAM_ERROR: ca_odelay_centering_error - per channel ca bit - rank 0 only
// XRAM_ERROR: cs_odelay_centering_error - per channel/rank cs
               write_error_nibble_XRAM(0, glbl_bits_error_status_31_0);
               if(glbl_compare_data_type == BITS) {
                  write_error_XRAM(ERR_CODE_118_CA_CAL_FAIL_CENTERING_CA_ODELAY);
               } else {
                  write_error_XRAM(ERR_CODE_121_CA_CAL_FAIL_CENTERING_CS_ODELAY);
               }
               return 1;
            }
         } while (!exit_loop);

      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
   
      DEBUG_PRINTF("INFO: ddr_ca_cal(): Remove 180 degree shift from the CA line \n");
      //u8 xpi_map_bitslip = readIO_DMC(ral_addr_of_xpi_map_bitslip_in_ddrmc_main_ddrmc_main_bank(BLK));
      //if(compare_data_type == BYTES) {
      //   writeIO_DMC(ral_addr_of_xpi_map_bitslip_in_ddrmc_main_ddrmc_main_bank(BLK),xpi_map_bitslip | 0xF0);
      //} else {
      //   writeIO_DMC(ral_addr_of_xpi_map_bitslip_in_ddrmc_main_ddrmc_main_bank(BLK),xpi_map_bitslip | 0xF);
      //}
   
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
      if(bit_slip_en)
         return 0;
      else
// XRAM_WARNING: ca_odelay_centering_error - per channel ca bit - rank 0 only
// XRAM_WARNING: cs_odelay_centering_error - per channel/rank cs
    	 return 2;
   
   }

void setup_ca_cal(u8 rank) {
   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: ddr_ca_cal(): Start CA training for ranks = %d with nibble mask = 0x%08X\n", RANKS, glbl_nibble_mask);
   DEBUG_PRINTF("##################################################################################################################### \n");

   // Disable the oe postamble and preamble for the ca traing.
   glbl_xpi_oe_val = readIO_DMC(ral_addr_of_xpi_oe_in_ddrmc_main_ddrmc_main_bank(BLK));
   writeIO_DMC(ral_addr_of_xpi_oe_in_ddrmc_main_ddrmc_main_bank(BLK),glbl_xpi_oe_val & 0xFFFFFF00);
   glbl_xpi_dqs_val = readIO_DMC(ral_addr_of_xpi_dqs_in_ddrmc_main_ddrmc_main_bank(BLK));
   writeIO_DMC(ral_addr_of_xpi_dqs_in_ddrmc_main_ddrmc_main_bank(BLK),glbl_xpi_dqs_val & 0xFFFFFF0F);
   u16 static lcl_f0_mr22[RANKS] = F0_MR22;
   // Setup Command to response read delay timing
   u8  command2compare_delay = T_ADR; 
   writeIO_DMC(ral_addr_of_seq_data_capture_in_ddrmc_main_ddrmc_main_bank(BLK),(command2compare_delay<<1)|0x1); // - sel - rw - uint - [0] 0x0 - 0 - cal_seq - When set to 0, regular data is captured based on the data valid selection. 
                                                                  // When set to 1, data is captured based on the counter programmed in bits [6:1]. 
                                                                  // When seq_mode is set to 1 (WRLVL mode) it automatically turns on this mode also.
                                                                  // - delay_cnt - rw - uint - [6:1] 0x0 - 0 - cal_seq - Sets a delay in controller clock cycles after 
                                                                  // a command goes out that a flag gets generated to capture the data (either DQ for WRLVL mode, or GT_STATUS if selected). 
                                                                  // This allows capture of certain data at a programmable time.

   // Use dq bits for comparision
   writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),1); // Clears comparision status.
   writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0); // Relase the clear signal. 

   //************* Setup Mask for write and read data bytes *****************************//
   u8  dq_bit;
   u32 write_nibbles_mask = 0;
   u32 read_nibbles_mask = 0;
   u8  nibble;
   for(dq_bit=0;dq_bit<DQ_BITS; dq_bit++) {
      nibble = glbl_dq_bits_iob_map[dq_bit][0];
      if((dq_bit%16) < 8) {
         read_nibbles_mask   = read_nibbles_mask   | (0x1<<nibble);
      } else {
         write_nibbles_mask  = write_nibbles_mask  | (0x1<<nibble);
         write_riu_nibble_reg(nibble,CFG_NIBBLE_CTRL0, read_riu_nibble_reg(nibble,CFG_NIBBLE_CTRL0) & 0xFFDF);
         write_riu_nibble_reg(nibble,CFG_CALIB_CTRL, read_riu_nibble_reg(nibble,CFG_CALIB_CTRL) | RIU_REF_CLK_EN_MASK);
         write_riu_nibble_reg(nibble,WL_TRAIN, read_riu_nibble_reg(nibble,WL_TRAIN) | 0x1);
      }
   }
   writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial); 
   bitslice_reset_bcast(0, BS_RESET_WT);
   writeIO_DMC(ral_addr_of_xpi_lp4_write_nib_mask_in_ddrmc_main_ddrmc_main_bank(BLK),write_nibbles_mask);
   writeIO_DMC(ral_addr_of_xpi_lp4_read_nib_mask_in_ddrmc_main_ddrmc_main_bank(BLK),read_nibbles_mask);
   //************************************************************************************//

   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: ddr_ca_cal(): Putting the Memory in CA traing mode \n");
   DEBUG_PRINTF("##################################################################################################################### \n");

   DEBUG_PRINTF("INFO: ddr_ca_cal(): Write Mode Register MR13[6] to 1 to change the FSP WR to 1 to allow writting the MRs corresponds to the targated frequency \n");
   if(glbl_lp4_mr13&0x80) {
      // FSP OP is 1

      // Change the FSP WR to 0 
      lp4_mode_reg_wr(rank, 13, glbl_lp4_mr13&(~0x40)); 

      lp4_mode_reg_wr(rank, 12, F0_MR12);
      writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 100);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

      lp4_mode_reg_wr(rank, 14, F0_MR14);
       writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 100);
       writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

      //lp4_mode_reg_wr(rank, 22, F0_MR22);
      lp4_mode_reg_wr(rank, 22, lcl_f0_mr22[rank]);
      writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 100);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

      lp4_mode_reg_wr(rank, 11, F0_MR11);
      writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 100);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

      // Change the FSP OP to 0 but keep the FSP WR to 1 
      lp4_mode_reg_wr(rank, 13, glbl_lp4_mr13&(~0x80)); 

      // Enable CA training in FSP 1
      lp4_mode_reg_wr(rank, 13, ((glbl_lp4_mr13|0x01)|(0x40))&(~0x80));
      writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
      writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 100);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   } else {
      // FSP OP is 0
	   lp4_mode_reg_wr(rank, 12, F0_MR12);
	   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 100);
	   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

	   lp4_mode_reg_wr(rank, 14, F0_MR14);
	   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 100);
	   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

	   //lp4_mode_reg_wr(rank, 22, F0_MR22);
           lp4_mode_reg_wr(rank, 22, lcl_f0_mr22[rank]);
	   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 100);
	   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

	   lp4_mode_reg_wr(rank, 11, F0_MR11);
	   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 100);
	   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

	   // Change the FSP WR to 1
	   lp4_mode_reg_wr(rank, 13, glbl_lp4_mr13|0x40);

	   lp4_mode_reg_wr(rank, 12, F0_MR12);
	   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 100);
	   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

	   lp4_mode_reg_wr(rank, 14, F0_MR14);
	   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 100);
	   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

	   //lp4_mode_reg_wr(rank, 22, F0_MR22);
           lp4_mode_reg_wr(rank, 22, lcl_f0_mr22[rank]);
	   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 100);
	   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

	   lp4_mode_reg_wr(rank, 11, F0_MR11);
	   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 100);
	   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

	   // Change the FSP OP to 1 but keep the FSP WR to 0
	   lp4_mode_reg_wr(rank, 13, glbl_lp4_mr13|0x80);

      // Enable CA training in FSP 0
      lp4_mode_reg_wr(rank, 13, ((glbl_lp4_mr13|0x01)&(~0x40))|0x80);
      writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
      writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 100);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   }


//   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0x1<<7); //Assert write command to drive the dq/dqs pattern out


 //  writeIO_DMC(ral_addr_of_seq_ck_cal_in_ddrmc_main_ddrmc_main_bank(BLK),0xAAAA);
   writeIO_DMC(ral_addr_of_seq_cs_cal_in_ddrmc_main_ddrmc_main_bank(BLK),0x0000);
   writeIO_DMC(ral_addr_of_seq_addr_cal0_in_ddrmc_main_ddrmc_main_bank(BLK),0x99999999); 
   writeIO_DMC(ral_addr_of_seq_addr_cal1_in_ddrmc_main_ddrmc_main_bank(BLK),0x99999999);
   writeIO_DMC(ral_addr_of_seq_addr_cal2_in_ddrmc_main_ddrmc_main_bank(BLK),0x99999999);
   writeIO_DMC(ral_addr_of_seq_addr_cal3_in_ddrmc_main_ddrmc_main_bank(BLK),0x99999999);
   writeIO_DMC(ral_addr_of_seq_addr_cal4_in_ddrmc_main_ddrmc_main_bank(BLK),0x99999999);
   writeIO_DMC(ral_addr_of_seq_addr_cal5_in_ddrmc_main_ddrmc_main_bank(BLK),0x99999999);
   if(!glbl_vref_cal) { // In vref enabled case it the DQ output will be set by the ca_cal_vref function.
       u8 dq_7_0 = glbl_lp4_mr12 & 0xFF;
       for(u8 ca_group_idx=0;ca_group_idx<DQ_BITS/16;ca_group_idx++) {
          set_ca_vref(ca_group_idx,dq_7_0);
       }
   }
   writeIO_DMC(ral_addr_of_seq_dqs_default_in_ddrmc_main_ddrmc_main_bank(BLK),4);
   writeIO_DMC(ral_addr_of_seq_dqs_pattern0_in_ddrmc_main_ddrmc_main_bank(BLK),0x55);
   writeIO_DMC(ral_addr_of_seq_dqs_pattern1_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
   //u32 default_pattern = readIO_DMC(DEFAULT_PATTERN);
   //writeIO_DMC(DEFAULT_PATTERN,default_pattern & 0xFFFF00FF);
   
   writeIO_DMC(ral_addr_of_seq_dq_nibble_sel_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   writeIO_DMC(ral_addr_of_seq_cmd_default_in_ddrmc_main_ddrmc_main_bank(BLK),0x3<<5 | 0x100); // Make default value of the CKE as 1 and the the reset a 1;
   writeIO_DMC(ral_addr_of_seq_cke_cal_in_ddrmc_main_ddrmc_main_bank(BLK), 0xFFFF);
   writeIO_DMC(ral_addr_of_seq_cs_bits_in_ddrmc_main_ddrmc_main_bank(BLK),0x1<<rank); // Rank select
   writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK) , T_CACD); // Setup the tCACD, CS to CS assertion minimum delay
   DEBUG_PRINTF("INFO: ddr_ca_cal(): Enable CA calibration mode in the calibration RTL \n");
   writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK),3); // Enable CA/CS calibration in the Sequencer. 
                              // Drives out a clock/address/controls as defined in the "seq_*_cal* registers. 
                              // This allows the phase adjustment of signals relative to each other and the clock. 
                              // Full DDR control of each signal is given.
                              //

   DEBUG_PRINTF("INFO: ddr_ca_cal(): Toggle CKE to 0 to change operating FSP to 1 \n");
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), T_DQSCKE);
   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
   writeIO_DMC(ral_addr_of_seq_cke_cal_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0000);
   writeIO_DMC(ral_addr_of_seq_cmd_default_in_ddrmc_main_ddrmc_main_bank(BLK),0x2<<5 | 0x100); // Make default value of the CKE as 0 and the the reset a 1;
   //writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0x1<<8); //Assert write command to drive the dq/dqs pattern out
   
   

   DEBUG_PRINTF("INFO: ddr_ca_cal(): Change the operating clock frequency to targeted one, high frequency clock \n");
   // writeIO_DMC(ral_addr_of_seq_ck_cal_in_ddrmc_main_ddrmc_main_bank(BLK),0x5555);
    writeIO_DMC(ral_addr_of_seq_ck_cal_in_ddrmc_main_ddrmc_main_bank(BLK),0xAAAA);

}

void release_ca_cs_training(u8 rank) {
   writeIO_DMC(ral_addr_of_seq_cs_cal_in_ddrmc_main_ddrmc_main_bank(BLK),0x0000);

   DEBUG_PRINTF("##################################################################################################################### \n");
   DEBUG_PRINTF("INFO: ddr_ca_cal(): Post CA calibration for RANKS = %d with nibble mask = 0x%08X\n", RANKS, glbl_nibble_mask);
   DEBUG_PRINTF("##################################################################################################################### \n");
   u32 write_nibbles_mask  = 0;
   u32 read_nibbles_mask   = 0;
   u8 dq_bit;
   u8 nibble;
   for(dq_bit=0;dq_bit<DQ_BITS; dq_bit++) {
      nibble = glbl_dq_bits_iob_map[dq_bit][0];
      write_riu_nibble_reg(nibble,CFG_NIBBLE_CTRL0, read_riu_nibble_reg(nibble,CFG_NIBBLE_CTRL0) | 0x0020);
      write_riu_nibble_reg(nibble,CFG_CALIB_CTRL, read_riu_nibble_reg(nibble,CFG_CALIB_CTRL) & (~RIU_REF_CLK_EN_MASK));
      write_riu_nibble_reg(nibble,WL_TRAIN, read_riu_nibble_reg(nibble,WL_TRAIN) & 0xFFFE);
   }

   DEBUG_PRINTF("INFO: ddr_ca_cal(): Change the operating clock frequency to the boot frequency clock \n");
   writeIO_DMC(ral_addr_of_seq_ck_cal_in_ddrmc_main_ddrmc_main_bank(BLK),0xF0F0);

   DEBUG_PRINTF("INFO: ddr_ca_cal(): Toggle CKE to 1 to change operating FSP to 1 \n");
   writeIO_DMC(ral_addr_of_seq_cke_cal_in_ddrmc_main_ddrmc_main_bank(BLK), 0xFFFF);
   writeIO_DMC(ral_addr_of_seq_cmd_default_in_ddrmc_main_ddrmc_main_bank(BLK),0x3<<5); // Make default value of the CKE as 1 and the the reset a 1;

   writeIO_DMC(ral_addr_of_seq_addr_cal0_in_ddrmc_main_ddrmc_main_bank(BLK),0x0); 
   writeIO_DMC(ral_addr_of_seq_addr_cal1_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
   writeIO_DMC(ral_addr_of_seq_addr_cal2_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
   writeIO_DMC(ral_addr_of_seq_addr_cal3_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
   writeIO_DMC(ral_addr_of_seq_addr_cal4_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
   writeIO_DMC(ral_addr_of_seq_addr_cal5_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);

        
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), T_CKEHDQS);
   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

   writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK),0); // Disable CA/CS calibration in the Sequencer.

   writeIO_DMC(ral_addr_of_xpi_lp4_write_nib_mask_in_ddrmc_main_ddrmc_main_bank(BLK),write_nibbles_mask);
   writeIO_DMC(ral_addr_of_xpi_lp4_read_nib_mask_in_ddrmc_main_ddrmc_main_bank(BLK),read_nibbles_mask);
   writeIO_DMC(ral_addr_of_seq_dqs_default_in_ddrmc_main_ddrmc_main_bank(BLK),0);
   //writeIO_DMC(DEFAULT_PATTERN,default_pattern);
   DEBUG_PRINTF("INFO: ddr_ca_cal(): Write the mode register to exit the CA traing mode \n");
   DEBUG_PRINTF("INFO: ddr_ca_cal(): Change the FSP_OP to the targated FSP\n");
   if(glbl_lp4_mr13&0x80) {
      // FSP OP is 1
      lp4_mode_reg_wr(rank, 13, glbl_lp4_mr13 & (~0x80));
      lp4_mode_reg_wr(rank, 13, glbl_lp4_mr13);
   } else {
      // FSP OP is 0
      lp4_mode_reg_wr(rank, 13, glbl_lp4_mr13 | 0x80);
      lp4_mode_reg_wr(rank, 13, glbl_lp4_mr13);
   }
   writeIO_DMC(ral_addr_of_seq_dq_nibble_sel_in_ddrmc_main_ddrmc_main_bank(BLK),0);
   DEBUG_PRINTF("INFO: ddr_ca_cal(): Change the operating clock frequency to targeted one, high frequency clock \n");
   writeIO_DMC(ral_addr_of_seq_ck_cal_in_ddrmc_main_ddrmc_main_bank(BLK),0xAAAA);
   // Restore the glbl_xpi_oe_val
   writeIO_DMC(ral_addr_of_xpi_oe_in_ddrmc_main_ddrmc_main_bank(BLK),glbl_xpi_oe_val);
   writeIO_DMC(ral_addr_of_xpi_dqs_in_ddrmc_main_ddrmc_main_bank(BLK),glbl_xpi_dqs_val);
}
#endif
