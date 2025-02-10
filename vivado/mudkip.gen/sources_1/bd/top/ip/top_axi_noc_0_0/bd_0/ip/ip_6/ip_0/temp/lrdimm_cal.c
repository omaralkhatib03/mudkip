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
//       Revision:       $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/lrdimm_cal.c#3 $
//                       $Author: rgaddam $
//                       $DateTime: 2019/04/23 03:27:17 $
//                       $Change: 21310834 $
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

#include  "cal_reg_define.h"
#include  "internal_reg_define.h"
#include  "riu_reg_define.h"
#include "function_dec.c"
#include "global_variable_define.h"

#ifdef DDR4
// void switch_ranks(u8 rank, u8 rank_phy){
//   // Setup memory chip select for the given rank
//   writeIO_DMC(SEQ_A_RANK,1<<rank);
//   writeIO_DMC(SEQ_B_RANK,1<<rank);
//   writeIO_DMC(SEQ_C_RANK,1<<rank);
// 
//   // Setup XPHY rank chip select
//   writeIO_DMC(ral_addr_of_seq_cs_bits_in_ddrmc_main_ddrmc_main_bank(BLK), 1<<rank_phy);
// }

void ddr_mrs_write (u8 rank, u32 mrx, u32 value) {

   u8 mirror_en = (glbl_xpi_mirror_en & (rank%2 == 1)) ? 1 : 0;

   writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (0x10<<rank) | (mirror_en << 1)); // Selects DDR Mem rank
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_MODE_REG_WRITE);
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), value);
   writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), ((mrx >> 2) & 0x3));
   writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), (mrx & 0x3));
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   // while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

   if(glbl_rdimm_en & (mrx != 0x7)) { // Write to Side-B only for MRS commands: only for MRS0-6. Exclude MRS7.
      // Enable the inversion 
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (0x10<<rank) | (mirror_en << 1) | 0x4 ); 

      // Writing for Side B
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      // while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

      // Disable the inversion & mirror
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (0x10<<rank)); 
   } 

   if(glbl_clamshell_cfg == 1) { // Clamshell configuration, mode register write
      // Enable the mirror
      mirror_en = glbl_xpi_mirror_en;
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (0x10<<rank) | (mirror_en << 1)); // Selects DDR Mem rank

      // Writing to Mirrored components of clamshell
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

      // Disable the mirror
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (0x10<<rank)); 
   }
  return;
}

void ddr_mrs7_write (u8 rank_rcd, u32 value) {

   // No need of mirror & inversion settings for MRS7
   writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10<<rank_rcd); // Selects DDR Mem rank
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_MODE_REG_WRITE);
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), value);
   writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);
   writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0x3);
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   // while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

  return;
}

void ddr_mrs7_write_opt (u32 value) {

   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), value);
   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

  return;
}

void dq_refclk_en (u32 rank_phy) {
  u8 nibble;
  u8 phy_riu;

  for(nibble = 0; nibble < DQS_BITS; nibble++) { 
    phy_riu = glbl_dqs_bits_riu_map[nibble][0];
    // Disable RX Gating
    read_mod_write_riu_nibble_reg(phy_riu /*u8 nibble*/, CFG_NIBBLE_CTRL0 /*u8 reg_addr*/, 0x00 /*u16 data*/, 0x5 /*u8 mask_pos*/, 0x1 /*u16 mask*/); 

    // Turn on the reference clock into the datapath to sample the data on DQ bus.
    read_mod_write_riu_nibble_reg(phy_riu /*u8 nibble*/, CFG_CALIB_CTRL /*u8 reg_addr*/, 0x3F /*u16 data*/, 0x3 /*u8 mask_pos*/, 0x3F /*u16 mask*/); 
  }

  // Keep XPHY in write leveling mode
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dqs_nibble_mask_initial);
  riu_reg_bcast(WL_TRAIN, 0, WL_TRAIN_ON);  

  return;
}

void dq_refclk_dis (u32 rank_phy) {
  u8 nibble;
  u8 phy_riu;

  // Keep XPHY in write leveling mode
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dqs_nibble_mask_initial);
  riu_reg_bcast(WL_TRAIN, 0, WL_TRAIN_OFF);  

  for(nibble = 0; nibble < DQS_BITS; nibble++) { 
    phy_riu = glbl_dqs_bits_riu_map[nibble][0];

    // Turn off the reference clock into the datapath to sample the data on DQ bus.
    read_mod_write_riu_nibble_reg(phy_riu /*u8 nibble*/, CFG_CALIB_CTRL /*u8 reg_addr*/, 0x0 /*u16 data*/, 0x3 /*u8 mask_pos*/, 0x3F /*u16 mask*/); 

    // Enable RX Gating
    read_mod_write_riu_nibble_reg(phy_riu /*u8 nibble*/, CFG_NIBBLE_CTRL0 /*u8 reg_addr*/, 0x01 /*u16 data*/, 0x5 /*u8 mask_pos*/, 0x1 /*u16 mask*/); 
  }

  return;
}

u8 db_delay_code (u8 tap) {
  u8 code;
  code = (tap < 0xF) ? (0x1F - tap) : (tap - 0xF); // From 0x1F down to 0x11 followed by 0x0 to 0xF
  return code;
}

u8 lrdimm_db_mrep () {
  u8 rank;
  u8 rank_phy;
  u8 rank_db;
  u8 rank_rcd;
  u8 ranks_per_slot;

  u8 nibble;
  u8 phy_riu;
  u8 tap;
  u32 stable_0_8;
  u32 stable_9_17;
  u8 st0_cnt;
  u8 tap_1_n1;
  u8 found_1_n1;
  u8 st1_cnt;
  u8 tap_n0_0;
  u8 found_n0_0;
  u8 noise_window;

  u8 mrep_dq0_tap[18][64];

  for(rank=0; rank<RANKS; rank++) {
    ranks_per_slot = RANKS/SLOTS;
    rank_db = rank % ranks_per_slot;
    rank_phy = rank / ranks_per_slot;
    rank_rcd = rank_phy * ranks_per_slot;

    DEBUG_PRINTF("INFO: lrdimm_db_mrep(): Rank MEM = %d, Rank PHY = %d, Rank RCD = %d, Rank DB = %d\n", rank, rank_phy, rank_rcd, rank_db);
    // Setup XPHY rank chip select
    writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK), rank_phy); // Selects xphy rank
    writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10<<rank); // Selects DDR Mem rank

    // Program DRAM into MPR Read mode
    ddr_mrs_write (rank, 0x3,  (glbl_mr3 | MEM_ENABLE_MPR));

    // Program Data Buffer into MREP mode
    ddr_mrs7_write (rank_rcd, 0x10C1); // F0BC0C Register: MREP Mode

    // Turn on reference clock to sample DQ0
    dq_refclk_en(rank_phy);

    // Select Function space for programming F[3:0]BC2X & F[3:0]BC3X registers
    ddr_mrs7_write_opt (0x1700 | rank_db); // F[7:0]BC7x Register: Function space as per Rank number

    for(tap=0;tap<64;tap++) {
      // Program TAP value
      ddr_mrs7_write (rank_rcd, 0x1200 | tap); // F[3:0]BC2X Register
      ddr_mrs7_write_opt (0x1300 | tap); // F[3:0]BC3X Register

      writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),1);

      // Issue READ commands
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10<<rank); // Selects DDR Mem rank
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_READ);
      writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
      writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 0x2);
      writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), (200 << 16) | 200);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

      while ((readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) & 0xFFFF) >= 190);
      writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0);

      while ((readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) & 0xFFFF) >= 100); // FIXME: Assuming 100 Fabric cycles is good enough to read stable values

      // Check for Stable 0 & 1
      stable_0_8 = readIO_DMC(ral_addr_of_compare_data_stable_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK));
      stable_9_17 = readIO_DMC(ral_addr_of_compare_data_stable_nibble1_in_ddrmc_main_ddrmc_main_bank(BLK));

      while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0);

      DEBUG_PRINTF("INFO: lrdimm_db_mrep(): Tap = %x, Stable 0-8 = %x, Stable 9-17 = %x\n", tap, stable_0_8, stable_9_17);

      for(nibble = 0; nibble < DQS_BITS; nibble++) { 
        if(nibble < 9) // Nibbles 0 to 8
          mrep_dq0_tap[nibble][tap] = (stable_0_8 >> (nibble * 2)) & 0x3;
        else // Nibbles 9 to 17
          mrep_dq0_tap[nibble][tap] = (stable_9_17 >> ((nibble-9) * 2)) & 0x3;
      }
    }

    // Select Function space back to 0
    ddr_mrs7_write (rank_rcd, 0x1700); // F[7:0]BC7x Register: Function space 0

    // Find the 1 to 0 transition
    for(nibble = 0; nibble < DQS_BITS; nibble++) { 
      st1_cnt = 0;
      found_1_n1 = 0;
      tap_1_n1 = 0;
      st0_cnt = 0;
      found_n0_0 = 0;
      tap_n0_0 = 0;

      for(tap=0;tap<128;tap++) {
        if(!found_1_n1) {
          if(mrep_dq0_tap[nibble][tap%64] == DB_DQ0_STABLE_ONE) {
            st1_cnt++;
          } else {
            if(st1_cnt >= DB_STABLE_FILTER_WIDTH) {
              found_1_n1 = 1;
              tap_1_n1 = tap - 1;
            }
            st1_cnt = 0;
          }
        }

        if(found_1_n1) {
          if(mrep_dq0_tap[nibble][tap%64] == DB_DQ0_STABLE_ZERO) {
            if(st0_cnt == 0)
              tap_n0_0 = tap;

            st0_cnt++;
            if(st0_cnt >= DB_STABLE_FILTER_WIDTH) {
              found_n0_0 = 1;
              break;
            }
          } else {
            st0_cnt = 0;
          }
        }
      }

      // Average the TAP between 1 to non-1 transition and non-0 to 0 transition
      glbl_mrep_tap[rank][nibble] = ((tap_1_n1 + tap_n0_0) >> 1) % 64;
      noise_window = (tap_n0_0 - tap_1_n1) - 1;

      DEBUG_PRINTF("INFO: lrdimm_db_mrep(): Rank = %d, Nibble = %d, Final tap (1 to 0) = %x, Tap 1 to n_1 = %x, Tap n_0 to 0 = %x, Noise window = %d\n", rank, nibble, glbl_mrep_tap[rank][nibble], tap_1_n1, tap_n0_0, noise_window);

      xsdb_dbg(DB_MREP_LEFT,  (rank*DQS_BITS)+nibble, tap_1_n1);
      xsdb_dbg(DB_MREP_RIGHT, (rank*DQS_BITS)+nibble, tap_n0_0);
      xsdb_dbg(DB_MREP_EDGE,  (rank*DQS_BITS)+nibble, glbl_mrep_tap[rank][nibble]);

      if(!found_1_n1 | !found_n0_0) {
        DEBUG_PRINTF("CAL_ERROR: CALIBRATION_MODULE: lrdimm_db_mrep(): DB-DRAM MREP Training error for rank = %d, nibble = %d\n", rank, nibble);
        write_err_reg(0x1, nibble, 0);
        return 1;
      } else if(noise_window > DB_NOISE_EDGE_MAX) {
        if(rank == 0)
          write_warning_reg(DDR_WARN_DB_RANK0_MREP_WIDER_NOISE_WINDOW, nibble);
        else if(rank == 1)
          write_warning_reg(DDR_WARN_DB_RANK1_MREP_WIDER_NOISE_WINDOW, nibble);
        else if(rank == 2)
          write_warning_reg(DDR_WARN_DB_RANK2_MREP_WIDER_NOISE_WINDOW, nibble);
        else // if(rank == 3)
          write_warning_reg(DDR_WARN_DB_RANK3_MREP_WIDER_NOISE_WINDOW, nibble);
      }
    }

    // Turn off reference clock to sample DQ0
    dq_refclk_dis(rank_phy);

    // Program DRAM back to Normal mode
    ddr_mrs_write (rank, 0x3,  (glbl_mr3 & MEM_DISABLE_MPR));

    // Program Data Buffer back to Normal mode
    ddr_mrs7_write (rank_rcd, 0x10C0); // F0BC0C Register: Normal mode

    // Enter PBA mode to program the set delays
    ddr_mrs7_write_opt (0x1100 | DB_F0BC1X_PACKAGE_RANK_TIMING_ALIGNMENT_ENABLE | 0x1); // F0BC1X Register: Enter PBA Mode

    // Enable DQ nibble select
    writeIO_DMC(ral_addr_of_seq_dq_nibble_sel_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);

    // Enable CAL write leveling mode to drive out DQS/DQ // FIXME
    writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

//    // Disable TX gating for data nibbles
//    for(nibble = 0; nibble < DQS_BITS; nibble++) { 
//      phy_riu = glbl_dqs_bits_riu_map[nibble][0];
//      read_mod_write_riu_nibble_reg(phy_riu /*u8 nibble*/, CFG_NIBBLE_CTRL0 /*u8 reg_addr*/, 0x00 /*u16 data*/, 0x4 /*u8 mask_pos*/, 0x1 /*u16 mask*/); 
//    }

    // Select function space as per the targeted rank
    ddr_mrs7_write_opt (0x1700 | rank_db); // F[7:0]BC7x Register: Function space as per Rank number

    // Reset all the nibbles
    for(nibble = 0; nibble < DQS_BITS; nibble=nibble+2) { 
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0xFFFFFFFF);
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0xFFFFFFFF);
    }

    for(nibble = 0; nibble < DQS_BITS; nibble=nibble+2) { 
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0x00000000); // Set targeted nibble
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0x00000000); // Set targeted nibble

      ddr_mrs7_write_opt (0x1200 | glbl_mrep_tap[rank][nibble]); // F[3:0]BC2X Register
      ddr_mrs7_write_opt (0x1300 | glbl_mrep_tap[rank][nibble+1]); // F[3:0]BC3X Register

      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0xFFFFFFFF); // Reset targeted nibble
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0xFFFFFFFF); // Reset targeted nibble
    }

    // Set all the nibbles
    for(nibble = 0; nibble < DQS_BITS; nibble=nibble+2) { 
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0x00000000);
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0x00000000);
    }

    // Program function space back to 0
    ddr_mrs7_write_opt (0x1700); // F[7:0]BC7x Register: Function space 0

    // Exit PBA mode
    ddr_mrs7_write_opt (0x1100 | DB_F0BC1X_PACKAGE_RANK_TIMING_ALIGNMENT_ENABLE | 0x0); // F0BC1X Register: Exit PBA Mode

    writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0);

//    // Enable TX gating for data nibbles
//    for(nibble = 0; nibble < DQS_BITS; nibble++) { 
//      phy_riu = glbl_dqs_bits_riu_map[nibble][0];
//      read_mod_write_riu_nibble_reg(phy_riu /*u8 nibble*/, CFG_NIBBLE_CTRL0 /*u8 reg_addr*/, 0x01 /*u16 data*/, 0x4 /*u8 mask_pos*/, 0x1 /*u16 mask*/); 
//    }

    // Disable DQ nibble select
    writeIO_DMC(ral_addr_of_seq_dq_nibble_sel_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

  } // RANKS

  return 0;
}

u8 lrdimm_db_mrd_cycle () {
  u8 rank;
  u8 rank_phy;
  u8 rank_db;
  u8 rank_rcd;
  u8 ranks_per_slot;

  u8 nibble;
  u8 lat;
  u8 lat_val;
  u8 pol;
  u8 sts;
  u8 mirror_en;
  u32 stable_0_8;
  u32 stable_9_17;

  u8 mrd_dq0_lat[18][5];
  u8 mrd_dq0_lat_neg[18][5];

  for(rank=0; rank<RANKS; rank++) {
    ranks_per_slot = RANKS/SLOTS;
    rank_db = rank % ranks_per_slot;
    rank_phy = rank / ranks_per_slot;
    rank_rcd = rank_phy * ranks_per_slot;

    mirror_en = (glbl_xpi_mirror_en & (rank%2 == 1)) ? 1 : 0;

    DEBUG_PRINTF("INFO: lrdimm_db_mrd_cycle(): Rank MEM = %d, Rank PHY = %d, Rank RCD = %d, Rank DB = %d\n", rank, rank_phy, rank_rcd, rank_db);
    // Setup XPHY rank chip select
    writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK), rank_phy); // Selects xphy rank
    writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (0x10<<rank) | (mirror_en << 1)); // Selects DDR Mem rank

    // Program DRAM into MPR Read mode
    ddr_mrs_write (rank, 0x3,  (glbl_mr3 | MEM_ENABLE_MPR));

    // Turn on reference clock to sample DQ0
    dq_refclk_en(rank_phy);

    // Load MPR Data into Data Buffer
    ddr_mrs7_write (rank_rcd, 0x1705); // F[7:0]BC7x Register: Function space 5

    ddr_mrs7_write_opt (0x1000); // F5BC0x Register: MPR0 - UI 0 // RGAD: FIXME
    ddr_mrs7_write_opt (0x1100); // F5BC1x Register: MPR1 - UI 1
    ddr_mrs7_write_opt (0x1200); // F5BC2x Register: MPR2 - UI 2
    ddr_mrs7_write_opt (0x1300); // F5BC3x Register: MPR3 - UI 3

    ddr_mrs7_write_opt (0x1706); // F[7:0]BC7x Register: Function space 6

    ddr_mrs7_write_opt (0x10FF); // F6BC0x Register: MPR4 - UI 4
    ddr_mrs7_write_opt (0x11FF); // F6BC1x Register: MPR5 - UI 5
    ddr_mrs7_write_opt (0x12FF); // F6BC2x Register: MPR6 - UI 6
    ddr_mrs7_write_opt (0x13FF); // F6BC3x Register: MPR7 - UI 7

    ddr_mrs7_write_opt (0x1400 | DB_F6BC4X); // F6BC4x Register: Status per nibble

    ddr_mrs7_write_opt (0x1700); // F[7:0]BC7x Register: Function space 0

    for(lat=0;lat<5;lat++) {
      // Program Latency value
      if(lat == 0)
        lat_val = 0x66; // -2 nCK read latency
      else if(lat == 1)
        lat_val = 0x55; // -1 nCK read latency
      else if(lat == 2)
        lat_val = 0x00; //  0 nCK read latency
      else if(lat == 3)
        lat_val = 0x11; // +1 nCK read latency
      else // if(lat == 4)
        lat_val = 0x22; // +2 nCK read latency

      if((rank_db == 0) | (rank_db == 2))
        ddr_mrs7_write (rank_rcd, 0x1700 | 0x0); // F[7:0]BC7x Register: Function space as per Rank number
      else
        ddr_mrs7_write (rank_rcd, 0x1700 | 0x1); // F[7:0]BC7x Register: Function space as per Rank number

      if((rank_db == 0) | (rank_db == 1))
        ddr_mrs7_write_opt (0x1C00 | lat_val); // F0BCCx/F1BCCx Register
      else // rank_db = 2 or 3
        ddr_mrs7_write_opt (0x1E00 | lat_val); // F0BCEx/F1BCEx Register

      ddr_mrs7_write_opt (0x1700); // F[7:0]BC7x Register: Function space 0

      for(pol=0;pol<2;pol++) {
        // Load the pattern based on Positive or Negative testing
        ddr_mrs7_write (rank_rcd, 0x1706); // F[7:0]BC7x Register: Function space 6

        if(pol==0) // Positive testing
          ddr_mrs7_write_opt (0x11FF); // F6BC1x Register: MPR5 - UI 5
        else // Negative testing
          ddr_mrs7_write_opt (0x1100); // F6BC1x Register: MPR5 - UI 5

        ddr_mrs7_write_opt (0x1700); // F[7:0]BC7x Register: Function space 0

        // Clear the Error status before issuing the commands
        ddr_mrs7_write_opt (0x1063); // F0BC06 Register: Clear Error Status

        // Program Data Buffer into MRD mode
        ddr_mrs7_write_opt (0x10C6); // F0BC0C Register: MRD Mode

        // Disable Output Inversion in RC00, so as to read the same MPR data from both sides: A & B // FIXME:
        ddr_mrs7_write_opt (0x0001); // RC00 Register: Output Inversion Disable

        // Issue READ commands
        writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (0x10<<rank) | (mirror_en << 1)); // Selects DDR Mem rank
        writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_READ);
        writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
        writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
        writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0x2);
        writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 8);
        writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 4);
        writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
        while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0);

        // Compare enable
        writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),1);
        writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0);

        // Issue READ commands
        writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 20);
        writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
        while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0);

        // Check for Stable 0 & 1
        stable_0_8 = readIO_DMC(ral_addr_of_compare_data_stable_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK));
        stable_9_17 = readIO_DMC(ral_addr_of_compare_data_stable_nibble1_in_ddrmc_main_ddrmc_main_bank(BLK));

        DEBUG_PRINTF("INFO: lrdimm_db_mrd_cycle(): Latency = %d, Polarity = %d, Stable 0-8 = %x, Stable 9-17 = %x\n", lat, pol, stable_0_8, stable_9_17);

        // Enable Output Inversion in RC00
        ddr_mrs7_write (rank_rcd, 0x0000); // RC00 Register: Output Inversion Enable

        // Program Data Buffer back to Normal mode
        ddr_mrs7_write_opt (0x10C0); // F0BC0C Register: Normal mode

        for(nibble = 0; nibble < DQS_BITS; nibble++) { 
          if(nibble < 9) // Nibbles 0 to 8
            sts = (stable_0_8 >> (nibble * 2)) & 0x3;
          else // Nibbles 9 to 17
            sts = (stable_9_17 >> ((nibble-9) * 2)) & 0x3;

          if(pol==0)
            mrd_dq0_lat[nibble][lat] = sts;
          else
            mrd_dq0_lat_neg[nibble][lat] = sts;
        }
      }
    }

    // Find the PASS Latency
    for(nibble = 0; nibble < DQS_BITS; nibble++) { 
      if((mrd_dq0_lat[nibble][2] == DB_DQ0_STABLE_ONE) & (mrd_dq0_lat_neg[nibble][2] == DB_DQ0_STABLE_ZERO)) {
        glbl_mrd_lat[rank][nibble] = 0;
      } else if((mrd_dq0_lat[nibble][1] == DB_DQ0_STABLE_ONE) & (mrd_dq0_lat_neg[nibble][1] == DB_DQ0_STABLE_ZERO)) {
        glbl_mrd_lat[rank][nibble] = 5;
      } else if((mrd_dq0_lat[nibble][3] == DB_DQ0_STABLE_ONE) & (mrd_dq0_lat_neg[nibble][3] == DB_DQ0_STABLE_ZERO)) {
        glbl_mrd_lat[rank][nibble] = 1;
      } else if((mrd_dq0_lat[nibble][0] == DB_DQ0_STABLE_ONE) & (mrd_dq0_lat_neg[nibble][0] == DB_DQ0_STABLE_ZERO)) {
        glbl_mrd_lat[rank][nibble] = 6;
      } else if((mrd_dq0_lat[nibble][4] == DB_DQ0_STABLE_ONE) & (mrd_dq0_lat_neg[nibble][4] == DB_DQ0_STABLE_ZERO)) {
        glbl_mrd_lat[rank][nibble] = 2;
      } else {
        DEBUG_PRINTF("CAL_ERROR: CALIBRATION_MODULE: lrdimm_db_mrd_cycle(): DB-DRAM MRD Cycle Training error for rank = %d, nibble = %d\n", rank, nibble);
        write_err_reg(0x1, nibble, 0);
        return 1;
      }
      DEBUG_PRINTF("INFO: lrdimm_db_mrd_cycle(): Rank = %d, Nibble = %d, Final Read Latency = %d\n", rank, nibble, glbl_mrd_lat[rank][nibble]);
      xsdb_dbg(DB_MRD_LATENCY, (rank*DQS_BITS)+nibble, glbl_mrd_lat[rank][nibble]);
    }

    // Turn off reference clock to sample DQ0
    dq_refclk_dis(rank_phy);

    // Program DRAM back to Normal mode
    ddr_mrs_write (rank, 0x3,  (glbl_mr3 & MEM_DISABLE_MPR));

    // Enter PBA mode to program the set delays
    ddr_mrs7_write (rank_rcd, 0x1100 | DB_F0BC1X_PACKAGE_RANK_TIMING_ALIGNMENT_ENABLE | 0x1); // F0BC1X Register: Enter PBA Mode

    // Enable DQ nibble select
    writeIO_DMC(ral_addr_of_seq_dq_nibble_sel_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);

    // Enable CAL write leveling mode to drive out DQS/DQ
    writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

    // Select function space as per the targeted rank
    if((rank_db == 0) | (rank_db == 2))
      ddr_mrs7_write_opt (0x1700 | 0x0); // F[7:0]BC7x Register: Function space as per Rank number
    else
      ddr_mrs7_write_opt (0x1700 | 0x1); // F[7:0]BC7x Register: Function space as per Rank number

    // Reset all the nibbles
    for(nibble = 0; nibble < DQS_BITS; nibble=nibble+2) { 
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0xFFFFFFFF);
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0xFFFFFFFF);
    }

    for(nibble = 0; nibble < DQS_BITS; nibble=nibble+2) { 
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0x00000000); // Set targeted nibble
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0x00000000); // Set targeted nibble

      if((rank_db == 0) | (rank_db == 1))
        ddr_mrs7_write_opt (0x1C00 | (glbl_mrd_lat[rank][nibble+1] << 4) | (glbl_mrd_lat[rank][nibble])); // F0BCCx/F1BCCx Register: nCK read latency
      else // rank_db = 2 or 3
        ddr_mrs7_write_opt (0x1E00 | (glbl_mrd_lat[rank][nibble+1] << 4) | (glbl_mrd_lat[rank][nibble])); // F0BCEx/F1BCEx Register: nCK read latency

      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0xFFFFFFFF); // Reset targeted nibble
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0xFFFFFFFF); // Reset targeted nibble
    }

    // Set all the nibbles
    for(nibble = 0; nibble < DQS_BITS; nibble=nibble+2) { 
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0x00000000);
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0x00000000);
    }

    // Program function space back to 0
    ddr_mrs7_write_opt (0x1700); // F[7:0]BC7x Register: Function space 0

    // Exit PBA mode
    ddr_mrs7_write_opt (0x1100 | DB_F0BC1X_PACKAGE_RANK_TIMING_ALIGNMENT_ENABLE | 0x0); // F0BC1X Register: Exit PBA Mode

    writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0);

    // Disable DQ nibble select
    writeIO_DMC(ral_addr_of_seq_dq_nibble_sel_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
  } // RANKS

  return 0;
}

u8 lrdimm_db_mrd_center () {
  u8 rank;
  u8 rank_phy;
  u8 rank_db;
  u8 rank_rcd;
  u8 ranks_per_slot;

  u8 nibble;
  u8 tap;
  u8 left;
  u8 right;
  u8 center;
  u8 st1_cnt;
  u8 found_1;
  u8 mirror_en;
  u32 stable_0_8;
  u32 stable_9_17;
  u8 valid_window;

  u8 mrd_dq0_tap[18][32];

  for(rank=0; rank<RANKS; rank++) {
    ranks_per_slot = RANKS/SLOTS;
    rank_db = rank % ranks_per_slot;
    rank_phy = rank / ranks_per_slot;
    rank_rcd = rank_phy * ranks_per_slot;

    mirror_en = (glbl_xpi_mirror_en & (rank%2 == 1)) ? 1 : 0;

    DEBUG_PRINTF("INFO: lrdimm_db_mrd_center(): Rank MEM = %d, Rank PHY = %d, Rank RCD = %d, Rank DB = %d\n", rank, rank_phy, rank_rcd, rank_db);
    // Setup XPHY rank chip select
    writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK), rank_phy); // Selects xphy rank
    writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (0x10<<rank) | (mirror_en << 1)); // Selects DDR Mem rank

    // Program DRAM into MPR Read mode
    ddr_mrs_write (rank, 0x3,  (glbl_mr3 | MEM_ENABLE_MPR));

    // Turn on reference clock to sample DQ0
    dq_refclk_en(rank_phy);

    // Load MPR Data into Data Buffer
    ddr_mrs7_write (rank_rcd, 0x1705); // F[7:0]BC7x Register: Function space 5

    ddr_mrs7_write_opt (0x1000); // F5BC0x Register: MPR0 - UI 0 // RGAD: FIXME
    ddr_mrs7_write_opt (0x11FF); // F5BC1x Register: MPR1 - UI 1
    ddr_mrs7_write_opt (0x1200); // F5BC2x Register: MPR2 - UI 2
    ddr_mrs7_write_opt (0x13FF); // F5BC3x Register: MPR3 - UI 3

    ddr_mrs7_write_opt (0x1706); // F[7:0]BC7x Register: Function space 6

    ddr_mrs7_write_opt (0x1000); // F6BC0x Register: MPR4 - UI 4
    ddr_mrs7_write_opt (0x11FF); // F6BC1x Register: MPR5 - UI 5
    ddr_mrs7_write_opt (0x1200); // F6BC2x Register: MPR6 - UI 6
    ddr_mrs7_write_opt (0x13FF); // F6BC3x Register: MPR7 - UI 7

    ddr_mrs7_write_opt (0x1400 | DB_F6BC4X); // F6BC4x Register: Status per nibble

    ddr_mrs7_write_opt (0x1700); // F[7:0]BC7x Register: Function space 0

    for(tap=0;tap<31;tap++) {
      // Program Tap value
      ddr_mrs7_write (rank_rcd, 0x1700 | rank_db); // F[7:0]BC7x Register: Function space as per Rank number
      ddr_mrs7_write_opt (0x1400 | db_delay_code(tap)); // F[3:0]BC4x Register: tCK/4 + (n/64)*tCK read delay tap for Lower nibble
      ddr_mrs7_write_opt (0x1500 | db_delay_code(tap)); // F[3:0]BC5x Register: tCK/4 + (n/64)*tCK read delay tap for Upper nibble
      ddr_mrs7_write_opt (0x1700); // F[7:0]BC7x Register: Function space 0

      // Clear the Error status before issuing the commands
      ddr_mrs7_write_opt (0x1063); // F0BC06 Register: Clear Error Status

      // Program Data Buffer into MRD mode
      ddr_mrs7_write_opt (0x10C6); // F0BC0C Register: MRD Mode

      // Disable Output Inversion in RC00, so as to read the same MPR data from both sides: A & B
      ddr_mrs7_write_opt (0x0001); // RC00 Register: Output Inversion Disable

      // Issue READ commands
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (0x10<<rank) | (mirror_en << 1)); // Selects DDR Mem rank
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_READ);
      writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
      writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
      writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
      writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 8);
      writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 4);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0);

      // Compare enable
      writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0);

      // Issue READ commands
      writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 20);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0);

      // Check for Stable 0 & 1
      stable_0_8 = readIO_DMC(ral_addr_of_compare_data_stable_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK));
      stable_9_17 = readIO_DMC(ral_addr_of_compare_data_stable_nibble1_in_ddrmc_main_ddrmc_main_bank(BLK));

      DEBUG_PRINTF("INFO: lrdimm_db_mrd_center(): Tap = %x, Stable 0-8 = %x, Stable 9-17 = %x\n", tap, stable_0_8, stable_9_17);

      // Enable Output Inversion in RC00
      ddr_mrs7_write (rank_rcd, 0x0000); // RC00 Register: Output Inversion Enable

      // Program Data Buffer back to Normal mode
      ddr_mrs7_write_opt (0x10C0); // F0BC0C Register: Normal mode

      for(nibble = 0; nibble < DQS_BITS; nibble++) { 
        if(nibble < 9) // Nibbles 0 to 8
          mrd_dq0_tap[nibble][tap] = (stable_0_8 >> (nibble * 2)) & 0x3;
        else // Nibbles 9 to 17
          mrd_dq0_tap[nibble][tap] = (stable_9_17 >> ((nibble-9) * 2)) & 0x3;
      }
    }

    // Find the MRD Read Delay center tap
    for(nibble = 0; nibble < DQS_BITS; nibble++) { 
      found_1 = 0;
      st1_cnt = 0;
      left = 0;
      right = 0;
      center = 0;

      for(tap=0;tap<31;tap++) {
        if(!found_1) {
          if(mrd_dq0_tap[nibble][tap] == DB_DQ0_STABLE_ONE) {
            if(st1_cnt == 0)
              left = tap;

            st1_cnt++;
            if(st1_cnt >= DB_STABLE_WINDOW_MIN)
              found_1 = 1;
          } else {
            st1_cnt = 0;
          }
        }

        if(found_1) {
          if(mrd_dq0_tap[nibble][tap] == DB_DQ0_STABLE_ONE) {
            right = tap;
          } else {
            break;
          }
        }
      }

      center = (left + right) / 2;
      valid_window = (right - left);
      glbl_mrd_tap[rank][nibble] = db_delay_code(center);

      xsdb_dbg(DB_MRD_LEFT,   (rank*DQS_BITS)+nibble, left);
      xsdb_dbg(DB_MRD_RIGHT,  (rank*DQS_BITS)+nibble, right);
      xsdb_dbg(DB_MRD_CENTER, (rank*DQS_BITS)+nibble, glbl_mrd_tap[rank][nibble]);

      if(!found_1) {
        DEBUG_PRINTF("CAL_ERROR: CALIBRATION_MODULE: lrdimm_db_mrd_center(): DB-DRAM MRD Center Training error for rank = %d, nibble = %d\n", rank, nibble);
        write_err_reg(0x1, nibble, 0);
        return 1;
      } else if(valid_window < DB_STABLE_WINDOW_WARN) {
        if(rank == 0)
          write_warning_reg(DDR_WARN_DB_RANK0_MRD_CENTER_NARROW_VALID_WINDOW, nibble);
        else if(rank == 1)
          write_warning_reg(DDR_WARN_DB_RANK1_MRD_CENTER_NARROW_VALID_WINDOW, nibble);
        else if(rank == 2)
          write_warning_reg(DDR_WARN_DB_RANK2_MRD_CENTER_NARROW_VALID_WINDOW, nibble);
        else // if(rank == 3)
          write_warning_reg(DDR_WARN_DB_RANK3_MRD_CENTER_NARROW_VALID_WINDOW, nibble);
      }

      DEBUG_PRINTF("INFO: lrdimm_db_mrd_center(): Found DB-DRAM MRD window edges for rank = %d, nibble = %d, left = %d, right = %d\n", rank, nibble, left, right);
    }

    // Turn off reference clock to sample DQ0
    dq_refclk_dis(rank_phy);

    // Program DRAM back to Normal mode
    ddr_mrs_write (rank, 0x3,  (glbl_mr3 & MEM_DISABLE_MPR));

    // Enter PBA mode to program the set delays
    ddr_mrs7_write (rank_rcd, 0x1100 | DB_F0BC1X_PACKAGE_RANK_TIMING_ALIGNMENT_ENABLE | 0x1); // F0BC1X Register: Enter PBA Mode

    // Enable DQ nibble select
    writeIO_DMC(ral_addr_of_seq_dq_nibble_sel_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);

    // Enable CAL write leveling mode to drive out DQS/DQ
    writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

    // Select function space as per the targeted rank
    ddr_mrs7_write_opt (0x1700 | rank_db); // F[7:0]BC7x Register: Function space as per Rank number

    // Reset all the nibbles
    for(nibble = 0; nibble < DQS_BITS; nibble=nibble+2) { 
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0xFFFFFFFF);
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0xFFFFFFFF);
    }

    // Load the DB Delays
    for(nibble = 0; nibble < DQS_BITS; nibble=nibble+2) { 
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0x00000000); // Set targeted nibble
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0x00000000); // Set targeted nibble

      ddr_mrs7_write_opt (0x1400 | glbl_mrd_tap[rank][nibble]); // F[3:0]BC4x Register: tCK/4 + (n/64)*tCK read delay tap for Lower nibble
      ddr_mrs7_write_opt (0x1500 | glbl_mrd_tap[rank][nibble+1]); // F[3:0]BC5x Register: tCK/4 + (n/64)*tCK read delay tap for Upper nibble

      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0xFFFFFFFF); // Reset targeted nibble
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0xFFFFFFFF); // Reset targeted nibble
    }

    // Set all the nibbles
    for(nibble = 0; nibble < DQS_BITS; nibble=nibble+2) { 
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0x00000000);
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0x00000000);
    }

    // Program function space back to 0
    ddr_mrs7_write_opt (0x1700); // F[7:0]BC7x Register: Function space 0

    // Exit PBA mode
    ddr_mrs7_write_opt (0x1100 | DB_F0BC1X_PACKAGE_RANK_TIMING_ALIGNMENT_ENABLE | 0x0); // F0BC1X Register: Exit PBA Mode

    writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0);

    // Disable DQ nibble select
    writeIO_DMC(ral_addr_of_seq_dq_nibble_sel_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
  } // RANKS

  return 0;
}

u8 lrdimm_db_dwl () {
  u8 rank;
  u8 rank_phy;
  u8 rank_db;
  u8 rank_rcd;
  u8 ranks_per_slot;

  u8 nibble;
  u8 phy_riu;
  u8 tap;
  u32 stable_0_8;
  u32 stable_9_17;
  u8 st0_cnt;
  u8 tap_0_n0;
  u8 found_0_n0;
  u8 st1_cnt;
  u8 tap_n1_1;
  u8 found_n1_1;
  u8 noise_window;

  u8 dwl_dq0_tap[18][64];

  for(rank=0; rank<RANKS; rank++) {
    ranks_per_slot = RANKS/SLOTS;
    rank_db = rank % ranks_per_slot;
    rank_phy = rank / ranks_per_slot;
    rank_rcd = rank_phy * ranks_per_slot;

    DEBUG_PRINTF("INFO: lrdimm_db_dwl(): Rank MEM = %d, Rank PHY = %d, Rank RCD = %d, Rank DB = %d\n", rank, rank_phy, rank_rcd, rank_db);
    // Setup XPHY rank chip select
    writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK), rank_phy); // Selects xphy rank
    writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10<<rank); // Selects DDR Mem rank

    //Turn on write leveling mode in DRAM
    ddr_mrs_write (rank, 0x1,  (glbl_mr1[rank] | 0x0080));

    // Select the Rank number
    ddr_mrs7_write (rank_rcd, 0x1080 | rank_db); // F0BC08 Register: Rank Number Selection control word

    // Program Data Buffer into DWL mode
    ddr_mrs7_write_opt (0x10C4); // F0BC0C Register: DWL Mode

    // Turn on reference clock to sample DQ0
    dq_refclk_en(rank_phy);

    // Select Function space for programming F[3:0]BC2X & F[3:0]BC3X registers
    ddr_mrs7_write_opt (0x1700 | rank_db); // F[7:0]BC7x Register: Function space as per Rank number

    for(tap=0;tap<64;tap++) {
      // Program TAP value
      ddr_mrs7_write (rank_rcd, 0x1A00 | tap); // F[3:0]BCAX Register
      ddr_mrs7_write_opt (0x1B00 | tap); // F[3:0]BCBX Register

      writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),1);

      // Issue NOP commands
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10<<rank); // Selects DDR Mem rank
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), (0x100 | MEM_NOP)); // Consider it as a read
      writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
      writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 0x2);
      writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), (200 << 16) | 200);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

      while ((readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) & 0xFFFF) >= 190);
      writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0);

      while ((readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) & 0xFFFF) >= 100);

      // Check for Stable 0 & 1
      stable_0_8 = readIO_DMC(ral_addr_of_compare_data_stable_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK));
      stable_9_17 = readIO_DMC(ral_addr_of_compare_data_stable_nibble1_in_ddrmc_main_ddrmc_main_bank(BLK));

      while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0);

      DEBUG_PRINTF("INFO: lrdimm_db_dwl(): Tap = %x, Stable 0-8 = %x, Stable 9-17 = %x\n", tap, stable_0_8, stable_9_17);

      for(nibble = 0; nibble < DQS_BITS; nibble++) { 
        if(nibble < 9) // Nibbles 0 to 8
          dwl_dq0_tap[nibble][tap] = (stable_0_8 >> (nibble * 2)) & 0x3;
        else // Nibbles 9 to 17
          dwl_dq0_tap[nibble][tap] = (stable_9_17 >> ((nibble-9) * 2)) & 0x3;
      }
    }

    // Select Function space back to 0
    ddr_mrs7_write (rank_rcd, 0x1700); // F[7:0]BC7x Register: Function space 0

    // Find the 0 to 1 transition
    for(nibble = 0; nibble < DQS_BITS; nibble++) { 
      st0_cnt = 0;
      found_0_n0 = 0;
      tap_0_n0 = 0;
      st1_cnt = 0;
      found_n1_1 = 0;
      tap_n1_1 = 0;

      for(tap=0;tap<128;tap++) {
        if(!found_0_n0) {
          if(dwl_dq0_tap[nibble][tap%64] == DB_DQ0_STABLE_ZERO) {
            st0_cnt++;
          } else {
            if(st0_cnt >= DB_STABLE_FILTER_WIDTH) {
              found_0_n0 = 1;
              tap_0_n0 = tap - 1;
            }
            st0_cnt = 0;
          }
        }

        if(found_0_n0) {
          if(dwl_dq0_tap[nibble][tap%64] == DB_DQ0_STABLE_ONE) {
            if(st1_cnt == 0)
              tap_n1_1 = tap;

            st1_cnt++;
            if(st1_cnt >= DB_STABLE_FILTER_WIDTH) {
              found_n1_1 = 1;
              break;
            }
          } else {
            st1_cnt = 0;
          }
        }
      }

      // Average the TAP between 0 to non-0 transition and non-1 to 1 transition
      glbl_dwl_tap[rank][nibble] = ((tap_0_n0 + tap_n1_1) >> 1) % 64;
      noise_window = (tap_n1_1 - tap_0_n0) - 1;

      DEBUG_PRINTF("INFO: lrdimm_db_dwl(): Rank = %d, Nibble = %d, Final tap (0 to 1) = %x, Tap 0 to n_0 = %x, Tap n_1 to 1 = %x, Noise window = %d\n", rank, nibble, glbl_dwl_tap[rank][nibble], tap_0_n0, tap_n1_1, noise_window);

      xsdb_dbg(DB_DWL_LEFT,  (rank*DQS_BITS)+nibble, tap_0_n0);
      xsdb_dbg(DB_DWL_RIGHT, (rank*DQS_BITS)+nibble, tap_n1_1);
      xsdb_dbg(DB_DWL_EDGE,  (rank*DQS_BITS)+nibble, glbl_dwl_tap[rank][nibble]);

      if(!found_0_n0 | !found_n1_1) {
        DEBUG_PRINTF("CAL_ERROR: CALIBRATION_MODULE: lrdimm_db_dwl(): DB-DRAM DWL Training error for rank = %d, nibble = %d\n", rank, nibble);
        write_err_reg(0x1, nibble, 0);
        return 1;
      } else if(noise_window > DB_NOISE_EDGE_MAX) {
        if(rank == 0)
          write_warning_reg(DDR_WARN_DB_RANK0_DWL_WIDER_NOISE_WINDOW, nibble);
        else if(rank == 1)
          write_warning_reg(DDR_WARN_DB_RANK1_DWL_WIDER_NOISE_WINDOW, nibble);
        else if(rank == 2)
          write_warning_reg(DDR_WARN_DB_RANK2_DWL_WIDER_NOISE_WINDOW, nibble);
        else // if(rank == 3)
          write_warning_reg(DDR_WARN_DB_RANK3_DWL_WIDER_NOISE_WINDOW, nibble);
      }
    }

    // Turn off reference clock to sample DQ0
    dq_refclk_dis(rank_phy);

    // Program Data Buffer back to Normal mode
    ddr_mrs7_write (rank_rcd, 0x10C0); // F0BC0C Register: Normal mode

    // Keeping it back to default value
    ddr_mrs7_write_opt (0x1080); // F0BC08 Register: Rank Number Selection control word

    // Turn off write leveling mode in the DRAM
    ddr_mrs_write (rank, 0x1,  (glbl_mr1[rank] | 0x0000));

    // Enter PBA mode to program the set delays
    ddr_mrs7_write (rank_rcd, 0x1100 | DB_F0BC1X_PACKAGE_RANK_TIMING_ALIGNMENT_ENABLE | 0x1); // F0BC1X Register: Enter PBA Mode

    // Enable DQ nibble select
    writeIO_DMC(ral_addr_of_seq_dq_nibble_sel_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);

    // Enable CAL write leveling mode to drive out DQS/DQ
    writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

    // Select function space as per the targeted rank
    ddr_mrs7_write_opt (0x1700 | rank_db); // F[7:0]BC7x Register: Function space as per Rank number

    // Reset all the nibbles
    for(nibble = 0; nibble < DQS_BITS; nibble=nibble+2) { 
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0xFFFFFFFF);
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0xFFFFFFFF);
    }

    for(nibble = 0; nibble < DQS_BITS; nibble=nibble+2) { 
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0x00000000); // Set for the selected Data Buffer only
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0x00000000); // Set for the selected Data Buffer only

      ddr_mrs7_write_opt (0x1A00 | glbl_dwl_tap[rank][nibble]); // F[3:0]BCAX Register
      ddr_mrs7_write_opt (0x1B00 | glbl_dwl_tap[rank][nibble+1]); // F[3:0]BCBX Register

      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0xFFFFFFFF); // Reset targeted nibble
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0xFFFFFFFF); // Reset targeted nibble
    }

    // Set all the nibbles
    for(nibble = 0; nibble < DQS_BITS; nibble=nibble+2) { 
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0x00000000);
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0x00000000);
    }

    // Program function space back to 0
    ddr_mrs7_write_opt (0x1700); // F[7:0]BC7x Register: Function space 0

    // Exit PBA mode
    ddr_mrs7_write_opt (0x1100 | DB_F0BC1X_PACKAGE_RANK_TIMING_ALIGNMENT_ENABLE | 0x0); // F0BC1X Register: Exit PBA Mode

    writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0);

    // Disable DQ nibble select
    writeIO_DMC(ral_addr_of_seq_dq_nibble_sel_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

  } // RANKS

  return 0;
}

u8 lrdimm_db_mwd_cycle () {
  u8 rank;
  u8 rank_phy;
  u8 rank_db;
  u8 rank_rcd;
  u8 ranks_per_slot;

  u8 nibble;
  u8 lat;
  u8 lat_val;
  u8 xor_data;
  u32 stable_0_8;
  u32 stable_9_17;

  u8 mwd_dq0_lat[18][5];

  for(rank=0; rank<RANKS; rank++) {
    ranks_per_slot = RANKS/SLOTS;
    rank_db = rank % ranks_per_slot;
    rank_phy = rank / ranks_per_slot;
    rank_rcd = rank_phy * ranks_per_slot;

    DEBUG_PRINTF("INFO: lrdimm_db_mwd_cycle(): Rank MEM = %d, Rank PHY = %d, Rank RCD = %d, Rank DB = %d\n", rank, rank_phy, rank_rcd, rank_db);
    // Setup XPHY rank chip select
    writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK), rank_phy); // Selects xphy rank
    writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10<<rank); // Selects DDR Mem rank

    // Turn on reference clock to sample DQ0
    dq_refclk_en(rank_phy);

    for(lat=0;lat<5;lat++) {
      // Program Latency value
      if(lat == 0) {
        lat_val = 0x66; // -2 nCK write latency
        xor_data = 0x00;
      } else if(lat == 1) {
        lat_val = 0x55; // -1 nCK write latency
        xor_data = 0x37;
      } else if(lat == 2) {
        lat_val = 0x00; //  0 nCK write latency
        xor_data = 0xE5;
      } else if(lat == 3) {
        lat_val = 0x11; // +1 nCK write latency
        xor_data = 0x4B;
      } else { // if(lat == 4)
        lat_val = 0x22; // +2 nCK write latency
        xor_data = 0xA6;
      }

      if((rank_db == 0) | (rank_db == 2))
        ddr_mrs7_write (rank_rcd, 0x1700 | 0x0); // F[7:0]BC7x Register: Function space as per Rank number
      else
        ddr_mrs7_write (rank_rcd, 0x1700 | 0x1); // F[7:0]BC7x Register: Function space as per Rank number

      if((rank_db == 0) | (rank_db == 1))
        ddr_mrs7_write_opt (0x1D00 | lat_val); // F0BCDx/F1BCDx Register
      else // rank_db = 2 or 3
        ddr_mrs7_write_opt (0x1F00 | lat_val); // F0BCFx/F1BCFx Register

      ddr_mrs7_write_opt (0x1700); // F[7:0]BC7x Register: Function space 0

      // Load MPR Data into Data Buffer
      ddr_mrs7_write_opt (0x1705); // F[7:0]BC7x Register: Function space 5

      ddr_mrs7_write_opt (0x10AB ^ xor_data); // F5BC0x Register: MPR0 - UI 0 // RGAD: FIXME
      ddr_mrs7_write_opt (0x11D5 ^ xor_data); // F5BC1x Register: MPR1 - UI 1
      ddr_mrs7_write_opt (0x1263 ^ xor_data); // F5BC2x Register: MPR2 - UI 2
      ddr_mrs7_write_opt (0x1397 ^ xor_data); // F5BC3x Register: MPR3 - UI 3

      ddr_mrs7_write_opt (0x1706); // F[7:0]BC7x Register: Function space 6

      ddr_mrs7_write_opt (0x108B ^ xor_data); // F6BC0x Register: MPR4 - UI 4
      ddr_mrs7_write_opt (0x11C2 ^ xor_data); // F6BC1x Register: MPR5 - UI 5
      ddr_mrs7_write_opt (0x12F4 ^ xor_data); // F6BC2x Register: MPR6 - UI 6
      ddr_mrs7_write_opt (0x136E ^ xor_data); // F6BC3x Register: MPR7 - UI 7

      ddr_mrs7_write_opt (0x1400 | DB_F6BC4X); // F6BC4x Register: Status per nibble
//      ddr_mrs7_write_opt (0x1400 | DB_F6BC4X | 0x51); // F6BC4x Register: Status per nibble

      ddr_mrs7_write_opt (0x1700); // F[7:0]BC7x Register: Function space 0

      // Clear the Error status before issuing the commands
      ddr_mrs7_write_opt (0x1063); // F0BC06 Register: Clear Error Status

      // Program Data Buffer into MWD mode
      ddr_mrs7_write_opt (0x10C7); // F0BC0C Register: MWD Mode

      // Activate the row in the DRAM
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10<<rank); // Selects DDR Mem rank
      writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), (0x00000000 | (lat << 4)));
      writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
      writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_ROW_ACTIVE);
      writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 2);
      writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

      // Issue WRITE commands, without driving DQS/DQ
      writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), (0x00000100 | (lat << 4)));
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), (MEM_COL_WRITE & 0x7F)); // seq_a_cmd_wr_rd = 0
      writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 8);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

      // Issue READ commands
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_READ);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

      // Compare enable
      writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0);

      // Issue READ commands
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

      // Check for Stable 0 & 1
      stable_0_8 = readIO_DMC(ral_addr_of_compare_data_stable_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK));
      stable_9_17 = readIO_DMC(ral_addr_of_compare_data_stable_nibble1_in_ddrmc_main_ddrmc_main_bank(BLK));

      DEBUG_PRINTF("INFO: lrdimm_db_mwd_cycle(): Latency = %d, Stable 0-8 = %x, Stable 9-17 = %x\n", lat, stable_0_8, stable_9_17);

      // Precharge the row in the DRAM
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_ROW_PREACHARGE);
      writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x400);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

      // Program Data Buffer back to Normal mode
      ddr_mrs7_write (rank_rcd, 0x10C0); // F0BC0C Register: Normal mode

      for(nibble = 0; nibble < DQS_BITS; nibble++) { 
        if(nibble < 9) // Nibbles 0 to 8
          mwd_dq0_lat[nibble][lat] = (stable_0_8 >> (nibble * 2)) & 0x3;
        else // Nibbles 9 to 17
          mwd_dq0_lat[nibble][lat] = (stable_9_17 >> ((nibble-9) * 2)) & 0x3;
      }
    }

    // Find the PASS Latency
    for(nibble = 0; nibble < DQS_BITS; nibble++) { 
      if(mwd_dq0_lat[nibble][2] == DB_DQ0_STABLE_ONE) {
        glbl_mwd_lat[rank][nibble] = 0;
      } else if(mwd_dq0_lat[nibble][1] == DB_DQ0_STABLE_ONE) {
        glbl_mwd_lat[rank][nibble] = 5;
      } else if(mwd_dq0_lat[nibble][3] == DB_DQ0_STABLE_ONE) {
        glbl_mwd_lat[rank][nibble] = 1;
      } else if(mwd_dq0_lat[nibble][0] == DB_DQ0_STABLE_ONE) {
        glbl_mwd_lat[rank][nibble] = 6;
      } else if(mwd_dq0_lat[nibble][4] == DB_DQ0_STABLE_ONE) {
        glbl_mwd_lat[rank][nibble] = 2;
      } else {
        DEBUG_PRINTF("CAL_ERROR: CALIBRATION_MODULE: lrdimm_db_mwd_cycle(): DB-DRAM MWD Cycle Training error for rank = %d, nibble = %d\n", rank, nibble);
        write_err_reg(0x1, nibble, 0);
        return 1;
      }
      DEBUG_PRINTF("INFO: lrdimm_db_mwd_cycle(): Rank = %d, Nibble = %d, Final Write Latency = %d\n", rank, nibble, glbl_mwd_lat[rank][nibble]);
      xsdb_dbg(DB_MWD_LATENCY, (rank*DQS_BITS)+nibble, glbl_mwd_lat[rank][nibble]);
    }

    // Turn off reference clock to sample DQ0
    dq_refclk_dis(rank_phy);

    // Enter PBA mode to program the set delays
    ddr_mrs7_write (rank_rcd, 0x1100 | DB_F0BC1X_PACKAGE_RANK_TIMING_ALIGNMENT_ENABLE | 0x1); // F0BC1X Register: Enter PBA Mode

    // Enable DQ nibble select
    writeIO_DMC(ral_addr_of_seq_dq_nibble_sel_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);

    // Enable CAL write leveling mode to drive out DQS/DQ
    writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

    // Select function space as per the targeted rank
    if((rank_db == 0) | (rank_db == 2))
      ddr_mrs7_write_opt (0x1700 | 0x0); // F[7:0]BC7x Register: Function space as per Rank number
    else
      ddr_mrs7_write_opt (0x1700 | 0x1); // F[7:0]BC7x Register: Function space as per Rank number

    // Reset all the nibbles
    for(nibble = 0; nibble < DQS_BITS; nibble=nibble+2) { 
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0xFFFFFFFF);
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0xFFFFFFFF);
    }

    for(nibble = 0; nibble < DQS_BITS; nibble=nibble+2) { 
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0x00000000); // Set targeted nibble
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0x00000000); // Set targeted nibble

      if((rank_db == 0) | (rank_db == 1))
        ddr_mrs7_write_opt (0x1D00 | (glbl_mwd_lat[rank][nibble+1] << 4) | (glbl_mwd_lat[rank][nibble])); // F0BCDx/F1BCDx Register: nCK write latency
      else // rank_db = 2 or 3
        ddr_mrs7_write_opt (0x1F00 | (glbl_mwd_lat[rank][nibble+1] << 4) | (glbl_mwd_lat[rank][nibble])); // F0BCFx/F1BCFx Register: nCK write latency

      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0xFFFFFFFF); // Reset targeted nibble
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0xFFFFFFFF); // Reset targeted nibble
    }

    // Set all the nibbles
    for(nibble = 0; nibble < DQS_BITS; nibble=nibble+2) { 
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0x00000000);
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0x00000000);
    }

    // Program function space back to 0
    ddr_mrs7_write_opt (0x1700); // F[7:0]BC7x Register: Function space 0

    // Exit PBA mode
    ddr_mrs7_write_opt (0x1100 | DB_F0BC1X_PACKAGE_RANK_TIMING_ALIGNMENT_ENABLE | 0x0); // F0BC1X Register: Exit PBA Mode

    writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0);

    // Disable DQ nibble select
    writeIO_DMC(ral_addr_of_seq_dq_nibble_sel_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
  } // RANKS

  return 0;
}

u8 lrdimm_db_mwd_center () {
  u8 rank;
  u8 rank_phy;
  u8 rank_db;
  u8 rank_rcd;
  u8 ranks_per_slot;

  u8 nibble;
  u8 tap;
  u8 left;
  u8 right;
  u8 center;
  u8 st1_cnt;
  u8 found_1;
  u32 stable_0_8;
  u32 stable_9_17;
  u8 valid_window;

  u8 mwd_dq0_tap[18][32];

  for(rank=0; rank<RANKS; rank++) {
    ranks_per_slot = RANKS/SLOTS;
    rank_db = rank % ranks_per_slot;
    rank_phy = rank / ranks_per_slot;
    rank_rcd = rank_phy * ranks_per_slot;

    DEBUG_PRINTF("INFO: lrdimm_db_mwd_center(): Rank MEM = %d, Rank PHY = %d, Rank RCD = %d, Rank DB = %d\n", rank, rank_phy, rank_rcd, rank_db);
    // Setup XPHY rank chip select
    writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK), rank_phy); // Selects xphy rank
    writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10<<rank); // Selects DDR Mem rank

    // Turn on reference clock to sample DQ0
    dq_refclk_en(rank_phy);

    // Load MPR Data into Data Buffer
    ddr_mrs7_write (rank_rcd, 0x1705); // F[7:0]BC7x Register: Function space 5

    ddr_mrs7_write_opt (0x1011); // F5BC0x Register: MPR0 - UI 0 // RGAD: FIXME
    ddr_mrs7_write_opt (0x1122); // F5BC1x Register: MPR1 - UI 1
    ddr_mrs7_write_opt (0x1233); // F5BC2x Register: MPR2 - UI 2
    ddr_mrs7_write_opt (0x1344); // F5BC3x Register: MPR3 - UI 3

    ddr_mrs7_write_opt (0x1706); // F[7:0]BC7x Register: Function space 6

    ddr_mrs7_write_opt (0x1055); // F6BC0x Register: MPR4 - UI 4
    ddr_mrs7_write_opt (0x1166); // F6BC1x Register: MPR5 - UI 5
    ddr_mrs7_write_opt (0x1277); // F6BC2x Register: MPR6 - UI 6
    ddr_mrs7_write_opt (0x1388); // F6BC3x Register: MPR7 - UI 7

    ddr_mrs7_write_opt (0x1400 | DB_F6BC4X); // F6BC4x Register: Status per nibble
//      ddr_mrs7_write_opt (0x1400 | DB_F6BC4X | 0x51); // F6BC4x Register: Status per nibble

    ddr_mrs7_write_opt (0x1700); // F[7:0]BC7x Register: Function space 0

    for(tap=0;tap<31;tap++) {
      // Program Tap value
      ddr_mrs7_write (rank_rcd, 0x1700 | rank_db); // F[7:0]BC7x Register: Function space as per Rank number
      ddr_mrs7_write_opt (0x1800 | db_delay_code(tap)); // F[3:0]BC8x Register: tCK/4 + (n/64)*tCK write delay tap for Lower nibble
      ddr_mrs7_write_opt (0x1900 | db_delay_code(tap)); // F[3:0]BC9x Register: tCK/4 + (n/64)*tCK write delay tap for Upper nibble
      ddr_mrs7_write_opt (0x1700); // F[7:0]BC7x Register: Function space 0

      // Clear the Error status before issuing the commands
      ddr_mrs7_write_opt (0x1063); // F0BC06 Register: Clear Error Status

      // Program Data Buffer into MWD mode
      ddr_mrs7_write_opt (0x10C7); // F0BC0C Register: MWD Mode

      // Activate the row in the DRAM
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10<<rank); // Selects DDR Mem rank
      writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
      writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
      writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_ROW_ACTIVE);
      writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 2);
      writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

      // Issue WRITE commands, without driving DQS/DQ
      writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), (MEM_COL_WRITE & 0x7F)); // seq_a_cmd_wr_rd = 0
      writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 8);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

      // Issue READ commands
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_READ);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

      // Compare enable
      writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0);

      // Issue READ commands
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0);

      // Check for Stable 0 & 1
      stable_0_8 = readIO_DMC(ral_addr_of_compare_data_stable_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK));
      stable_9_17 = readIO_DMC(ral_addr_of_compare_data_stable_nibble1_in_ddrmc_main_ddrmc_main_bank(BLK));

      DEBUG_PRINTF("INFO: lrdimm_db_mwd_center(): Tap = %x, Stable 0-8 = %x, Stable 9-17 = %x\n", tap, stable_0_8, stable_9_17);

      // Precharge the row in the DRAM
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_ROW_PREACHARGE);
      writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x400);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      while (readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

      // Program Data Buffer back to Normal mode
      ddr_mrs7_write (rank_rcd, 0x10C0); // F0BC0C Register: Normal mode

      for(nibble = 0; nibble < DQS_BITS; nibble++) { 
        if(nibble < 9) // Nibbles 0 to 8
          mwd_dq0_tap[nibble][tap] = (stable_0_8 >> (nibble * 2)) & 0x3;
        else // Nibbles 9 to 17
          mwd_dq0_tap[nibble][tap] = (stable_9_17 >> ((nibble-9) * 2)) & 0x3;
      }
    }

    // Find the MWD Read Delay center tap
    for(nibble = 0; nibble < DQS_BITS; nibble++) { 
      found_1 = 0;
      st1_cnt = 0;
      left = 0;
      right = 0;
      center = 0;

      for(tap=0;tap<31;tap++) {
        if(!found_1) {
          if(mwd_dq0_tap[nibble][tap] == DB_DQ0_STABLE_ONE) {
            if(st1_cnt == 0)
              left = tap;

            st1_cnt++;
            if(st1_cnt >= DB_STABLE_WINDOW_MIN)
              found_1 = 1;
          } else {
            st1_cnt = 0;
          }
        }

        if(found_1) {
          if(mwd_dq0_tap[nibble][tap] == DB_DQ0_STABLE_ONE) {
            right = tap;
          } else {
            break;
          }
        }
      }

      center = (left + right) / 2;
      valid_window = (right - left);
      glbl_mwd_tap[rank][nibble] = db_delay_code(center);

      xsdb_dbg(DB_MWD_LEFT,   (rank*DQS_BITS)+nibble, left);
      xsdb_dbg(DB_MWD_RIGHT,  (rank*DQS_BITS)+nibble, right);
      xsdb_dbg(DB_MWD_CENTER, (rank*DQS_BITS)+nibble, glbl_mwd_tap[rank][nibble]);

      if(!found_1) {
        DEBUG_PRINTF("CAL_ERROR: CALIBRATION_MODULE: lrdimm_db_mwd_center(): DB-DRAM MWD Center Training error for rank = %d, nibble = %d\n", rank, nibble);
        write_err_reg(0x1, nibble, 0);
        return 1;
      } else if(valid_window < DB_STABLE_WINDOW_WARN) {
        if(rank == 0)
          write_warning_reg(DDR_WARN_DB_RANK0_MWD_CENTER_NARROW_VALID_WINDOW, nibble);
        else if(rank == 1)
          write_warning_reg(DDR_WARN_DB_RANK1_MWD_CENTER_NARROW_VALID_WINDOW, nibble);
        else if(rank == 2)
          write_warning_reg(DDR_WARN_DB_RANK2_MWD_CENTER_NARROW_VALID_WINDOW, nibble);
        else // if(rank == 3)
          write_warning_reg(DDR_WARN_DB_RANK3_MWD_CENTER_NARROW_VALID_WINDOW, nibble);
      }

      DEBUG_PRINTF("INFO: lrdimm_db_mwd_center(): Found DB-DRAM MWD window edges for rank = %d, nibble = %d, left = %d, right = %d\n", rank, nibble, left, right);
    }

    // Turn off reference clock to sample DQ0
    dq_refclk_dis(rank_phy);

    // Enter PBA mode to program the set delays
    ddr_mrs7_write (rank_rcd, 0x1100 | DB_F0BC1X_PACKAGE_RANK_TIMING_ALIGNMENT_ENABLE | 0x1); // F0BC1X Register: Enter PBA Mode

    // Enable DQ nibble select
    writeIO_DMC(ral_addr_of_seq_dq_nibble_sel_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);

    // Enable CAL write leveling mode to drive out DQS/DQ
    writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

    // Select function space as per the targeted rank
    ddr_mrs7_write_opt (0x1700 | rank_db); // F[7:0]BC7x Register: Function space as per Rank number

    // Reset all the nibbles
    for(nibble = 0; nibble < DQS_BITS; nibble=nibble+2) { 
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0xFFFFFFFF);
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0xFFFFFFFF);
    }

    for(nibble = 0; nibble < DQS_BITS; nibble=nibble+2) { 
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0x00000000); // Set targeted nibble
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0x00000000); // Set targeted nibble

      ddr_mrs7_write_opt (0x1800 | glbl_mwd_tap[rank][nibble]); // F[3:0]BC8x Register: tCK/4 + (n/64)*tCK write delay tap for Lower nibble
      ddr_mrs7_write_opt (0x1900 | glbl_mwd_tap[rank][nibble+1]); // F[3:0]BC9x Register: tCK/4 + (n/64)*tCK write delay tap for Upper nibble

      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0xFFFFFFFF); // Reset targeted nibble
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0xFFFFFFFF); // Reset targeted nibble
    }

    // Set all the nibbles
    for(nibble = 0; nibble < DQS_BITS; nibble=nibble+2) { 
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4), 0x00000000);
      writeIO_DMC((ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + nibble*4 + 4), 0x00000000);
    }

    // Program function space back to 0
    ddr_mrs7_write_opt (0x1700); // F[7:0]BC7x Register: Function space 0

    // Exit PBA mode
    ddr_mrs7_write_opt (0x1100 | DB_F0BC1X_PACKAGE_RANK_TIMING_ALIGNMENT_ENABLE | 0x0); // F0BC1X Register: Exit PBA Mode

    writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0);

    // Disable DQ nibble select
    writeIO_DMC(ral_addr_of_seq_dq_nibble_sel_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
  } // RANKS

  return 0;
}
#endif
