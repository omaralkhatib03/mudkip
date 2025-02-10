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
//       Revision:       $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/skip_ddr_dqs_gate.c#2 $
//                       $Author: rgaddam $
//                       $DateTime: 2019/02/19 07:08:51 $
//                       $Change: 20624036 $
//       Description:
//           DDRMC : 
//
///////////////////////////////////////////////////////////////////////////////
#ifdef SKIP_DQS_CAL_STAGE
   #include "skip_cal_data.h"
   
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
   void skip_ddr_dqs_gate() 
   {
      u16 rldly_rnk_val[DQ_NIBBLES][4]=RL_DLY_RNK_VAL;
      u16 phy_rden_val[DQ_NIBBLES]=PHY_RDEN_VAL;
      u32 nibble_mask_initial, first_nibble, last_nibble;
      u32 dqs_riu_map[DQS_BITS][2] = DQS_RIU_MAP;
      //map_dqs_nibbles(&nibble_mask_initial/*u32 *nibble_mask*/, &first_nibble /*u8 *first_nibble*/, &last_nibble /*u8 *last_nibble*/); 
      u8 rank;
      //u8 phy_nibble;
      u8 log_nibble;

      RECORD_DLY("// Written from SKIP_DQS_GATE_CAL function \n");
      RECORD_DLY("#define RL_DLY_RNK_VAL { ");
      log_nibble = 0;
      //for(phy_nibble = first_nibble; phy_nibble <= last_nibble; phy_nibble++) {
      for(log_nibble=0; log_nibble < DQS_BITS; log_nibble++) {
         //if(!((nibble_mask_initial>>phy_nibble)&0x1)) {
            RECORD_DLY("{");
            for(rank= 0; rank < 4; rank++) {
               //write_riu_nibble_reg(phy_nibble, RLDLYRNK0 + 4*rank, rldly_rnk_val[log_nibble][rank]);
               write_riu_nibble_reg(dqs_riu_map[log_nibble][0], RLDLYRNK0 + rank, rldly_rnk_val[log_nibble][rank]);
               if(rank<(4-1)) {
                  RECORD_DLY("0x%04X,",rldly_rnk_val[log_nibble][rank]);
               } else {
                  RECORD_DLY("0x%04X",rldly_rnk_val[log_nibble][rank]);
               }
            }
            if(log_nibble<(DQS_BITS-1)) {
               RECORD_DLY("},");
            } else { 
               RECORD_DLY("} }\n");
            }
      //      log_nibble++;
        // }
      }
      RECORD_DLY("#define PHY_RDEN_VAL {");
      //log_nibble = 0;
      //for(phy_nibble = first_nibble; phy_nibble <= last_nibble; phy_nibble++) {
      for(log_nibble= 0; log_nibble < DQS_BITS; log_nibble++) {
         //if(!((nibble_mask_initial>>phy_nibble)&0x1)) {
            writeIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK)+dqs_riu_map[log_nibble][0]*4,phy_rden_val[log_nibble]);
            //if(phy_nibble<last_nibble) {
            if(log_nibble<(DQS_BITS-1)) {
               RECORD_DLY("%d,",phy_rden_val[log_nibble]);
            } else { 
               RECORD_DLY("%d} \n",phy_rden_val[log_nibble]);
            }
       //     log_nibble++;
       //  }
      }
      writeIO_DMC(ral_addr_of_fifo_rden_in_ddrmc_main_ddrmc_main_bank(BLK), MAX_LATENCY);
      RECORD_DLY("#define MAX_LATENCY %d \n", MAX_LATENCY);

      RECORD_DLY("//Recorded the delays\n");

      //writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~nibble_mask_initial); 
      for(log_nibble= 0; log_nibble < DQS_BITS; log_nibble++) {
            write_riu_nibble_reg(dqs_riu_map[log_nibble][0], BS_RESET_CTRL,0x1<<CLR_GATE_POS);
            write_riu_nibble_reg(dqs_riu_map[log_nibble][0], BS_RESET_CTRL,0x1<<BS_RESET_CTRL);
            write_riu_nibble_reg(dqs_riu_map[log_nibble][0], BS_RESET_CTRL,0x0);
      }
      //riu_reg_bcast(BS_RESET_CTRL,0,0x1<<CLR_GATE_POS);  // Enable Clear DQS Gate.
      //riu_reg_bcast(BS_RESET_CTRL,0,0x1<<BS_RESET_POS);  // Enable BS Reset.
      //riu_reg_bcast(BS_RESET_CTRL,0,0x0);  //- Disable Clear DQS Gate.
   }
#endif
