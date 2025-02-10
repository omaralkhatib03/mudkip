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
//       Revision:       $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/skip_ddr_rd_cal.c#2 $
//                       $Author: rgaddam $
//                       $DateTime: 2019/02/19 07:08:51 $
//                       $Change: 20624036 $
//       Description:
//           DDRMC : 
//
///////////////////////////////////////////////////////////////////////////////
#if defined(SKIP_RD_CAL_STAGE) || defined(SKIP_RD_CPLX_CAL_STAGE)
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
   void skip_ddr_rd_cal(u8 pattern_type) 
   {
      u8 bit_cnt;
      #ifdef SKIP_RD_CAL_STAGE
         u16 idelay_val[DQ_BITS] = IDELAY_VAL;
         u16 pqtr_val[DQ_BITS] = PQTR_VAL;
         u16 nqtr_val[DQ_BITS] = NQTR_VAL;
      #else
         u16 idelay_val[DQ_BITS];
         u16 pqtr_val[DQ_BITS];
         u16 nqtr_val[DQ_BITS];
      #endif
      #ifdef SKIP_RD_CPLX_CAL_STAGE
         u16 idelay_cplx_val[DQ_BITS] = CPLX_IDELAY_VAL;
         u16 pqtr_cplx_val[DQ_BITS] = CPLX_PQTR_VAL;
         u16 nqtr_cplx_val[DQ_BITS] = CPLX_NQTR__VAL;
      #else
         u16 idelay_cplx_val[DQ_BITS] ;
         u16 pqtr_cplx_val[DQ_BITS] ;
         u16 nqtr_cplx_val[DQ_BITS] ;
      #endif
      
      if(pattern_type == COMPLEX) {
         for(bit_cnt = 0; bit_cnt<DQ_BITS; bit_cnt++) {
            idelay_val[bit_cnt] = idelay_cplx_val[bit_cnt];
            pqtr_val[bit_cnt]   = pqtr_cplx_val[bit_cnt];
            nqtr_val[bit_cnt]   = nqtr_cplx_val[bit_cnt];
         }
      }

      if(pattern_type == SIMPLE) {
         RECORD_DLY("// Written from SKIP RD_CAL function \n");
         RECORD_DLY("#define IDELAY_VAL { ");
      } else {
         RECORD_DLY("// Written from SKIP CPLX RD_CAL function \n");
         RECORD_DLY("#define IDELAY_CPLX_VAL { ");
      }
      for(bit_cnt = 0; bit_cnt < DQ_BITS; bit_cnt++) {
         writeIO_RIU( glbl_dq_idly_riu_addr_map[bit_cnt], idelay_val[bit_cnt]);  
         if(bit_cnt < (DQ_BITS-1)) {
            RECORD_DLY(" 0x%04X,", idelay_val[bit_cnt]);
         } else {
            RECORD_DLY(" 0x%04X } \n", idelay_val[bit_cnt]);
         }
      }   

      u8 lnibble;
      if(pattern_type == SIMPLE) {
         RECORD_DLY("#define PQTR_VAL { ");
      } else {
         RECORD_DLY("#define PQTR_CPLX_VAL { ");
      }
      for(lnibble = 0; lnibble < (DQ_BITS/4); lnibble++) {
         write_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[lnibble], PQTR, pqtr_val[lnibble]); 
         if(lnibble < ((DQ_BITS/4)-1)) {
            RECORD_DLY(" 0x%04X,", pqtr_val[lnibble]);
         } else {
            RECORD_DLY(" 0x%04X } \n", pqtr_val[lnibble]);
         }
      }   
      if(pattern_type == SIMPLE) {
         RECORD_DLY("#define NQTR_VAL { ");
      } else {
         RECORD_DLY("#define NQTR_CPLX_VAL { ");
      }
      for(lnibble = 0; lnibble < (DQ_BITS/4); lnibble++) {
         write_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[lnibble], NQTR, nqtr_val[lnibble]); 
         if(lnibble < ((DQ_BITS/4)-1)) {
            RECORD_DLY(" 0x%04X,", nqtr_val[lnibble]);
         } else {
            RECORD_DLY(" 0x%04X } \n", nqtr_val[lnibble]);
         }
      }   


      for(lnibble = 0; lnibble < (DQ_BITS/4); lnibble++) {
         write_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[lnibble], BS_RESET_CTRL,0x1<<CLR_GATE_POS);
         write_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[lnibble], BS_RESET_CTRL,0x1<<BS_RESET_CTRL);
         write_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[lnibble], BS_RESET_CTRL,0x0);
      }
   }
#endif
