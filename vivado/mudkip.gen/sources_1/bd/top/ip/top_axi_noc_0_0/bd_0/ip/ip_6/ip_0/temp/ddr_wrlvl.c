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
//       Revision:       $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/ddr_wrlvl.c#4 $
//                       $Author: rgaddam $
//                       $DateTime: 2019/04/15 11:06:56 $
//                       $Change: 21202855 $
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

void setup_xiphy_wrlvl(u8 wl_train);
void mult_sample_dqs(u8 *all_sample_status);
int stable_0_confirmation (u8 rank, u8 *dqs_coarse);
int noise_width_calculation (u8 rank, u8 *dqs_coarse);
void get_90degree_phase_remove();

#define TRI_ODLY_OFFSET 0x0

u8 ddr_wrlvl(void)
{
  u8 i,j;
  u8 coarse		  	= 0;
  u32 dqs_coarse_status   	= 0;
  u8 status 			= 0;
  u8 dqs_coarse_status_pat[DQS_BITS], dqs_coarse[DQS_BITS], all_sample_status[DQS_BITS];
  u8 current_rank, rank, rank_rcd;
  u16 odelay_offset;
  u32 calib_cntrl[DQS_BITS*NIBBLE_PER_DQS];
  u32 extend_write_data;
  u32 nibble_mask;
  u32 dqs_idle; 

  #ifdef LPDDR4
  if(glbl_tck >= TCK_TH_WR_LVL) {
    DEBUG_PRINTF("INFO: ddr_wrlvl(): SKIPPING write leveling stage for TCK = %d\n", glbl_tck);
    return 0;
  }
  #endif

  // Set write preamble & postamble to 0 during write leveling to avoid unwanted dqs toggeling
  writeIO_DMC(ral_addr_of_xpi_oe_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_xpi_oe_val & ~(0xFF)));

  // DQS default pattern to be '0'
  dqs_idle = readIO_DMC(ral_addr_of_xpi_dqs_in_ddrmc_main_ddrmc_main_bank(BLK));
  writeIO_DMC(ral_addr_of_xpi_dqs_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

  writeIO_DMC(ral_addr_of_seq_dqs_default_in_ddrmc_main_ddrmc_main_bank(BLK), 0x2);

  // Sample Setting For Write Leveling
  writeIO_DMC(ral_addr_of_seq_a_dqs_early_in_ddrmc_main_ddrmc_main_bank(BLK), 0x2);
  writeIO_DMC(ral_addr_of_seq_a_dqs_0_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
  #ifdef LPDDR4
    writeIO_DMC(ral_addr_of_seq_a_dqs_1_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
  #endif
  writeIO_DMC(ral_addr_of_seq_a_dqs_late_in_ddrmc_main_ddrmc_main_bank(BLK), 0x2);
  writeIO_DMC(ral_addr_of_seq_data_capture_in_ddrmc_main_ddrmc_main_bank(BLK), (0x30 << 1) | 0x1);

  // Bcast the Nibble Wise Reset Mask Setting For DQS 
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK0, glbl_nibble_reset_mask[0]);
  riu_reg_bcast(CFG_BS_RST_MASK, 0, ~(0x7F));

  // Bcast the Nibble Wise Reset Mask Setting For DM 
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK0, glbl_nibble_reset_mask[1]);
  riu_reg_bcast(CFG_BS_RST_MASK, 0, ~(0x7D));

  #ifdef LPDDR4
    get_90degree_phase_remove();
  #endif

  glbl_wrlvl = 1;

  for(rank = 0; rank < RANKS; rank++)
  {
    current_rank = (RANKS - 1) - rank;

    if((current_rank % glbl_rank_inc) != 0)
      continue;

    rank_rcd = (current_rank / (RANKS/SLOTS)) * (RANKS/SLOTS);

    for(i=0; i<DQS_BITS; i++)
      dqs_coarse_status	 |= (1 <<i);     

    dqs_coarse_status = ~dqs_coarse_status;

    DEBUG_PRINTF("INFO: ddr_wrlvl(): dqs_coarse_status = %x \n", dqs_coarse_status);

    // MASK 0 Setting 
    update_nibble_dly_mask(&dqs_coarse_status, &nibble_mask);
    writeIO_RIU( RPI_BCAST_NIBBLE_MASK0, nibble_mask);

    // Start From zero ODELAY_DQS
    for(i=0; i<DQS_BITS; i++)
      writeIO_RIU( glbl_dqs_odly_riu_addr_map[i], 0x0);

    // Start From zero Tristate Delay
    update_dly_mask(&dqs_coarse_status, &nibble_mask);
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, nibble_mask);
    riu_reg_bcast(TRISTATE_ODLY, 1, 0x0000); //Tristate Inc Dec Crse ... 15 14 13

    #ifdef DDR4
      // Put the Memory in the MPR mode
      set_1_seq_command_at_mem(current_rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                             MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr1[current_rank] | 0x080 /*u32 seq_a_addr*/, 0x0 /*u8 seq_a_bg*/, 0x1 /*u16 seq_a_ba*/,
                             0x0 /*u32 seq_a2a_dly*/);
      if(glbl_lrdimm_en) {
        ddr_mrs7_write (rank_rcd, 0x1080 | (current_rank % (RANKS/SLOTS))); // F0BC08 Register: Rank Number Selection control word
        ddr_mrs7_write (rank_rcd, 0x10C5); // F0BC0C Register: HWL Mode
      }
    #else
      writeIO_DMC(ral_addr_of_seq_cs_bits_in_ddrmc_main_ddrmc_main_bank(BLK),1<<current_rank); //Rank select           
      lp4_mode_reg_wr(current_rank, 2, glbl_lp4_mr2|0x80);

    // SEQ Mode Setting For Write Leveling
      writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 1);                  
    #endif

    DEBUG_PRINTF("INFO: ddr_wrlvl(): Turned On Write Leveling Mode in MEMORY \n");

    // Setting the Seq Count Value
    writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 0x40);
    writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), ((SAMPLE_CNT<<16) | SAMPLE_CNT));

    DEBUG_PRINTF("INFO: ddr_wrlvl(): CS is off in the write leveling \n");

    // NO CS during write leveling
    extend_write_data = readIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK));
    writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (extend_write_data & 0xFFFFFF0F));

    DEBUG_PRINTF("INFO: ddr_wrlvl(): wl_train is on in the RIU Register \n");

    // SEND the WRITE Command
    #ifdef DDR4
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_WRITE); 
    #else
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0xA5); 
      writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), LP4_COL_WRITE);
      writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
    #endif

    DEBUG_PRINTF("INFO: ddr_wrlvl(): dqs_per_rank = %d, nibble_cnt = %d \n", DQS_BITS, NIBBLE_PER_DQS);

    // Calculate the DELAY OFFSET (It will be of 90 degree offset)
    odelay_offset = glbl_rl_dly_qtr_max; 

    // Parallel Bitslice, Tristate and Clear gate 
    riu_reg_bcast(BS_RESET_CTRL, 0, 0x1<<CLR_GATE_POS);
    bitslice_reset_bcast(0, 0x1<<BS_RESET_POS);
    DEBUG_PRINTF("INFO: ddr_wrlvl(): RX GATE IS CLEAR AND BIT SLICE LOGIC IS RESETTED \n");

    // Turn On Write Leveling Mode in the Xiphy
    setup_xiphy_wrlvl(WL_TRAIN_ON);

    DEBUG_PRINTF("INFO: ddr_wrlvl(): WRLVL MODE IS SET IN THE XIPHY \n");

    // Turn On reference Clock in XPHY
    for(i = 0; i < DQS_BITS; i++)
    { 
      for(j = 0; j < NIBBLE_PER_DQS; j++)
      {
        calib_cntrl[(i*NIBBLE_PER_DQS) + j] = read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + j], CFG_CALIB_CTRL);
        write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + j], CFG_CALIB_CTRL, (calib_cntrl[(i*NIBBLE_PER_DQS) + j] | RIU_REF_CLK_EN_MASK));
      }
    }

    DEBUG_PRINTF("INFO: ddr_wrlvl(): REF_CLK is ON FOR ALL THE BYTES \n");

    // RISE EDGE DETECTION LOGIC
    DEBUG_PRINTF("INFO: ddr_wrlvl(): RISE EDGE DETECTION USING COARSE TAPS STARTS \n");    

    do
    {
      coarse = 0;

      for(i=0; i<DQS_BITS; i++)
        dqs_coarse_status_pat[i] 	= 0;

      do 
      {
        // Incrementing the Coarse value for Unmask Bytes
        update_dly_mask(&dqs_coarse_status, &nibble_mask);
        DEBUG_PRINTF("INFO: ddr_wrlvl(): nibble_mask = %x \n", nibble_mask);        

  	writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);

        #ifdef DDR4 
          riu_reg_bcast(WLDLYRNK0 + current_rank, 1, coarse<<9);
        #else
          riu_reg_bcast(LP4_WL_DQS, 1, coarse);
        #endif

        bitslice_reset_bcast(0, 0x1<<BS_RESET_POS); // Tri state and bs_reset control set high

        mult_sample_dqs(all_sample_status);

        // Sequential Analysis the result
        for(i=0; i<DQS_BITS; i++)
        {
          if((dqs_coarse_status & (1<<i)) == 0)
          {
            dqs_coarse_status_pat[i] = (dqs_coarse_status_pat[i] << 2) | (all_sample_status[i] & 0x3);

            DEBUG_PRINTF("INFO: ddr_wrlvl(): dqs_coarse_status_pat[%d] = %x \n", i, dqs_coarse_status_pat[i]);

            if((dqs_coarse_status_pat[i] & DQS_COARSE_PAT_MASK) == DQS_COARSE_PAT)
            {
              dqs_coarse_status |= (1<<i);

              if((dqs_coarse_status_pat[i] & DQS_COARSE_STABLE_0_PAT_MASK) == DQS_COARSE_STABLE_0_PAT)
              {
                dqs_coarse[i] = coarse - 1;
                
                #ifdef DDR4
                  write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], WLDLYRNK0 + current_rank, (coarse-1)<<9);
                #else
                  write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], LP4_WL_DQS, (coarse-1));
                #endif
              }
              else
              {
                dqs_coarse[i] = coarse - 2;

                #ifdef DDR4
                  write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], WLDLYRNK0 + current_rank, (coarse-2)<<9);
                #else
                  write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], LP4_WL_DQS, (coarse-2));
                #endif
              }
// XRAM_DBG: wrlvl_crse_stg1
// XRAM_DBG: wrlvl_offset
              writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRLVL_CRSE_STG1 + ((current_rank * MAX_DQS_BITS + i) * 4), dqs_coarse[i]);
              writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRLVL_OFFSET + ((current_rank * MAX_DQS_BITS + i) * 4), odelay_offset);
            }
          }
        }

        DEBUG_PRINTF("INFO: ddr_wrlvl(): dqs_coarse_status = %x, coarse = %d\n", dqs_coarse_status, coarse);

        coarse++;

      } while ((coarse <= WL_DLY_CRSE_SEARCH_LIMIT) && (dqs_coarse_status != 0xFFFFFFFF));

      DEBUG_PRINTF("INFO: ddr_wrlvl(): EXITING FROM RISE EDGE DETECTION LOOP WITH ALL COARSE COMPLETED \n");

      bitslice_reset_bcast(0, 0x1<<BS_RESET_POS);

      if(dqs_coarse_status != 0xFFFFFFFF)
      {
        if((odelay_offset == 1) || (odelay_offset == 0))
        {
          odelay_offset = 0;
          
          DEBUG_PRINTF("ERROR: ddr_wrlvl(): RISE EDGE DETECTION FAILED due to ODELAY_OFFSET = 0 \n")
          write_error_XRAM(ERR_CODE_9_WRLVL_FAIL_RISE_EDGE_DETECTION);
          write_error_nibble_XRAM(0, ~dqs_coarse_status);
	  writeIO_XRAM(XRAM_CAL_RANK,(1 << current_rank | 1 << 4));
          return 1;
        }
        else
        {
          odelay_offset = odelay_offset >> 1;
          DEBUG_PRINTF("INFO: ddr_wrlvl(): RISE EDGE DETECTION WILL START AGAIN WITH ALL COARSE \n"); 
          DEBUG_PRINTF("INFO: ddr_wrlvl(): odelay_offset value for current rise edge detection = %x \n", odelay_offset); 
        }
      }

      update_dly_mask(&dqs_coarse_status, &nibble_mask);
      writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
     
      #ifdef DDR4
        riu_reg_bcast(ODLY0, 1, odelay_offset);
      #else
        riu_reg_bcast(ODLY1, 1, odelay_offset);
      #endif

    } while (dqs_coarse_status != 0xFFFFFFFF);

    DEBUG_PRINTF("INFO: ddr_wrlvl(): RISE EDGE DETECTION PASSED \n");

    // Uppdating the Coarse Value based on the result we got in RISE EDGE DETECTION STAGE
    DEBUG_PRINTF("INFO: ddr_wrlvl(): Calling Stable_0_confirmation Function \n");
  
    status = stable_0_confirmation(current_rank, dqs_coarse);
// XRAM_DBG: wrlvl_crse_final
    for(i=0; i<DQS_BITS; i++)
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRLVL_CRSE_FINAL + ((current_rank * MAX_DQS_BITS + i) * 4), dqs_coarse[i]);

    DEBUG_PRINTF("INFO: ddr_wrlvl(): Final Exit From stable_0_confirmation \n");

    if(status)
    {
      DEBUG_PRINTF("ERROR: ddr_wrlvl(): STABLE_0_CONFIRMATION STAGE FAILED \n")
      write_error_XRAM(ERR_CODE_10_WRLVL_FAIL_STABLE_0_CONFIRMATION);
      writeIO_XRAM(XRAM_CAL_RANK,(1 << current_rank | 1 << 4));
      return 1;
    }

    DEBUG_PRINTF("INFO: ddr_wrlvl(): STABLE_0_CONFIRMATION STAGE PASSED \n")

    DEBUG_PRINTF("INFO: ddr_wrlvl(): Calling Noise_width_calculation Function \n");
   
    status = noise_width_calculation(current_rank, dqs_coarse);

    if(status)
    {
      DEBUG_PRINTF("ERROR: ddr_wrlvl(): NOISE_WIDTH_CALCULATION STAGE FAILED \n");
      write_error_XRAM(ERR_CODE_11_WRLVL_FAIL_HIGHER_NOISE_WIDTH);
      writeIO_XRAM(XRAM_CAL_RANK,(1 << current_rank | 1 << 4));
      return 1;
    }

    DEBUG_PRINTF("INFO: ddr_wrlvl(): NOISE_WIDTH_CALCULATION STAGE PASSED \n");

    // Turn Off the Ref clock
    for(i=0; i<DQS_BITS; i++)
    {
      for(j=0; j<NIBBLE_PER_DQS; j++)
      {
        // TURN OFF THE REF CLOCK
        write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)+j], CFG_CALIB_CTRL, calib_cntrl[(i*NIBBLE_PER_DQS)+j] & (~RIU_REF_CLK_EN_MASK));
      }
    }

    DEBUG_PRINTF("INFO: ddr_wrlvl(): Turned Off the Reference Clock \n");

    // Disable Write leveling in XIPHY
    setup_xiphy_wrlvl(WL_TRAIN_OFF);

    DEBUG_PRINTF("INFO: ddr_wrlvl(): Disable the write leveling in the XPHY \n");

    // Parallel Bitslice and Tristate Reset
    riu_reg_bcast(BS_RESET_CTRL, 0, 0x1<<CLR_GATE_POS);
    bitslice_reset_bcast(0, 0x1<<BS_RESET_POS);

    DEBUG_PRINTF("INFO: ddr_wrlvl(): RX GATE IS CLEAR AND BIT SLICE LOGIC IS RESETTED \n");

    extend_write_data = readIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK));
    writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (extend_write_data | 0x000000F0));

    DEBUG_PRINTF("INFO: ddr_wrlvl(): WL_TRAIN is diabled in the RIU register \n");

    #ifdef DDR4
      set_1_seq_command_at_mem(current_rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                             MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr1[current_rank] /*u32 seq_a_addr*/, 0x0 /*u8 seq_a_bg*/, 0x1 /*u16 seq_a_ba*/,
                             0 /*u32 seq_a2a_dly*/);
      if(glbl_lrdimm_en) {
        ddr_mrs7_write (rank_rcd, 0x10C0); // F0BC0C Register: Normal Mode
        ddr_mrs7_write (rank_rcd, 0x1080); // F0BC08 Register: Rank Number Selection control word
      }
    #else
      writeIO_DMC(ral_addr_of_seq_cs_bits_in_ddrmc_main_ddrmc_main_bank(BLK),1<<current_rank); //Rank select           
      lp4_mode_reg_wr(current_rank, 2, glbl_lp4_mr2);
    #endif
       
    DEBUG_PRINTF("INFO: ddr_wrlvl(): Turned OFF Write leveling Mode in MEMORY \n");

    dqs_coarse_status = 0;

    #ifdef LPDDR4
      writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0);                  
    #endif
  } //RANKS

  writeIO_DMC(ral_addr_of_seq_a_dqs_early_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
  writeIO_DMC(ral_addr_of_seq_a_dqs_late_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
  writeIO_DMC(ral_addr_of_seq_dqs_default_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
  writeIO_DMC(ral_addr_of_seq_data_capture_in_ddrmc_main_ddrmc_main_bank(BLK), (0x30 << 1));

  glbl_wrlvl = 0;

  // Clearing the Compare Status
  writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);
  writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

  // Revert Back the Default value of DQS
  writeIO_DMC(ral_addr_of_xpi_dqs_in_ddrmc_main_ddrmc_main_bank(BLK), dqs_idle);
  
  // Reverting write preamble & postamble
  writeIO_DMC(ral_addr_of_xpi_oe_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_xpi_oe_val);

  return 0;
}


int noise_width_calculation (u8 rank, u8 *dqs_coarse)
{
  u8 all_sample_status[DQS_BITS];
  u8 i, j; 
  u8 stable_1_detect_count[DQS_BITS];
  u32 unstable_detect_done = 0;
  u16 ft = 0;
  u16 left_edge_tap[DQS_BITS];
  u16 odelay;
  u16 right_edge_tap[DQS_BITS];
  u16 center_tap[DQS_BITS];
  u32 nibble_mask;
  u32 noise_detect_done = 0;
  u32 stable_1_detect_done = 0;
  u32 init_status;

  for(i=0; i<DQS_BITS; i++)
  {
    stable_1_detect_count[i]	= 0;
    stable_1_detect_done        |= (1<<i);
  }

  stable_1_detect_done 	= ~stable_1_detect_done;
  noise_detect_done    	= stable_1_detect_done;
  init_status 		= stable_1_detect_done;
  
  //////////////////////////////////////////////
  ////////////////// PHASE 1 ///////////////////
  //////////////////////////////////////////////

  // INCDEC_CRSE value is programmed to Min Valid Count
  update_dly_mask(&noise_detect_done, &nibble_mask); 
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK2, nibble_mask);
  riu_reg_bcast(INCDEC_CRSE, 2, FINE_CRS_STEP_SIZE);
  
  do
  {
    if(ft == FT_LIMIT_MAX)
    {
      DEBUG_PRINTF("ERROR: ddr_wrlvl(): NOISE_WIDTH_CALCULATION STAGE FAILED DUE TO MAX TAP REACHED \n");       
      write_error_nibble_XRAM(0, ~noise_detect_done);
      return 1;
    }

    // BroadCast Increment Odelay With Larger Value
    update_dly_mask(&noise_detect_done, &nibble_mask);
    writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);

    #ifdef DDR4
      riu_reg_bcast(ODLY0, 1, 0xa000); // ODLY Inc Dec Crse ... 15 14 13
    #else
      riu_reg_bcast(ODLY1, 1, 0xa000); // ODLY Inc Dec Crse ... 15 14 13
    #endif

    // Bit slice Reset
    riu_reg_bcast(BS_RESET_CTRL, 0, 0x1<<CLR_GATE_POS);
    bitslice_reset_bcast(0, 0x1<<BS_RESET_POS);

    mult_sample_dqs(all_sample_status);

    for(i=0; i<DQS_BITS; i++)
    {
       if((noise_detect_done & (1<<i)) ==  0)
       {
         if(all_sample_status[i] != 2) {
           noise_detect_done |= (1 << i);
// XRAM_DBG: wrlvl_fine_noise_crse
         #ifdef DDR4
           odelay = read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], ODLY0);
         #else
           odelay = read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], ODLY1);
         #endif
           writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRLVL_NOISE_FCRSE + ((rank * MAX_DQS_BITS + i) * 4), odelay);
           DEBUG_PRINTF("INFO: ddr_wrlvl(): rank = %d, nibble = %d, left_edge_tap = %x \n", rank, i, odelay);
         }
       }
    } 

    ft = ft + FINE_CRS_STEP_SIZE;

    DEBUG_PRINTF("INFO: ddr_wrlvl(): noise_detect_done = %x, fine taps = %d\n", noise_detect_done, ft);

  } while (noise_detect_done != 0xFFFFFFFF);

  //////////////////////////////////////////////
  ////////////////// PHASE 2 ///////////////////
  //////////////////////////////////////////////

  // BroadCast One Large Step Decrement Odelay
  update_dly_mask(&init_status, &nibble_mask);
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
  
  #ifdef DDR4
    riu_reg_bcast(ODLY0, 1, 0x6000); // ODLY Inc Dec Crse ... 15 14 13
  #else
    riu_reg_bcast(ODLY1, 1, 0x6000); // ODLY Inc Dec Crse ... 15 14 13
  #endif

  // Bit slice Reset
  riu_reg_bcast(BS_RESET_CTRL, 0, 0x1<<CLR_GATE_POS);
  bitslice_reset_bcast(0, 0x1<<BS_RESET_POS);

  DEBUG_PRINTF("INFO: ddr_wrlvl(): Phase 2 finished \n");
  
  //////////////////////////////////////////////
  ////////////////// PHASE 3 ///////////////////
  //////////////////////////////////////////////
  ft = 0;

  do
  {
    if(ft == FT_LIMIT_MAX)
    {
      DEBUG_PRINTF("ERROR: ddr_wrlvl(): NOISE_WIDTH_CALCULATION STAGE FAILED DUE TO MAX TAP REACHED \n");       
      write_error_nibble_XRAM(0, ~stable_1_detect_done);
      return 1;
    }
   
    // BroadCast Increment Odelay Value
    update_dly_mask(&stable_1_detect_done, &nibble_mask);
    writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);

    #ifdef DDR4
      riu_reg_bcast(ODLY0, 1, 0x8000); // ODLY Inc Dec Crse ... 15 14 13
    #else
      riu_reg_bcast(ODLY1, 1, 0x8000); // ODLY Inc Dec Crse ... 15 14 13
    #endif

    mult_sample_dqs(all_sample_status);

    // Calculate the left and right edge and update the odelay value 
    for(i=0; i<DQS_BITS; i++)
    {
      if((stable_1_detect_done & (1<<i)) == 0)
      {
        if((unstable_detect_done & (1<<i)) == 0)
        {
          if(all_sample_status[i] != 2)
          {
            unstable_detect_done |= (1 << i);
          #ifdef DDR4
            left_edge_tap[i] = read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], ODLY0);
          #else
            left_edge_tap[i] = read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], ODLY1);
          #endif
// XRAM_DBG: wrlvl_fine_left
            writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRLVL_FINE_LEFT + ((rank * MAX_DQS_BITS + i) * 4), left_edge_tap[i]);
            DEBUG_PRINTF("INFO: ddr_wrlvl(): rank = %d, nibble = %d, left_edge_tap = %x \n", rank, i, left_edge_tap[i]);

            if(all_sample_status[i] == 1)
              stable_1_detect_count[i] += 1; 
          }
        }
        else
        {
          if(all_sample_status[i] == 1)
            stable_1_detect_count[i] += 1;
          else 
            stable_1_detect_count[i] = 0;

          if(stable_1_detect_count[i] == MIN_VLD_CNT)
          {
            stable_1_detect_done |= (1<<i);
          #ifdef DDR4
            right_edge_tap[i] = read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], ODLY0);
          #else
            right_edge_tap[i] = read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], ODLY1);
          #endif
            right_edge_tap[i] -= (MIN_VLD_CNT-1);
// XRAM_DBG: wrlvl_fine_right
            writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRLVL_FINE_RIGHT + ((rank * MAX_DQS_BITS + i) * 4), right_edge_tap[i]);
            DEBUG_PRINTF("INFO: ddr_wrlvl(): rank = %d, nibble = %d, right_edge_tap = %x \n", rank, i, right_edge_tap[i]);
          }
        }
      }
    }

    DEBUG_PRINTF("INFO: ddr_wrlvl(): unstable_detect_done = %x, stable_1_detect_done = %x, fine taps = %d\n", unstable_detect_done, stable_1_detect_done, ft);

    ft = ft + 1;

  } while (stable_1_detect_done != 0xFFFFFFFF);

  // Update Stage For all The Coarse and Fine Delay
  u8 phy_nibble;
  for(i=0; i<DQS_BITS; i++)
  {
    center_tap[i] = (right_edge_tap[i] + left_edge_tap[i]) >> 1; 
    phy_nibble = glbl_dqs_bits_riu_map[i][0];

    if(dqs_coarse[i] > 3)
      dqs_coarse[i] -= 4;

    while(center_tap[i] < TRI_ODLY_OFFSET) {
      center_tap[i]    += glbl_rl_dly_qtr[phy_nibble];
      dqs_coarse[i] = (dqs_coarse[i] == 0) ? 3 : (dqs_coarse[i] - 1);
    }

// XRAM_DBG: wrlvl_fine_final
    writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRLVL_FINE_FINAL + ((rank * MAX_DQS_BITS + i) * 4), center_tap[i]);

    #ifdef DDR4
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], WLDLYRNK0 + rank, (dqs_coarse[i]<<9)+center_tap[i]);

      if(NIBBLE_PER_DQS == 2)
        write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], WLDLYRNK0 + rank, (dqs_coarse[i]<<9)+center_tap[i]);

      // Putting Delay on the DQ Pin
      for(j=0; j<(4*NIBBLE_PER_DQS); j++)
        writeIO_RIU(glbl_dq_odly_riu_addr_map[(i*4*NIBBLE_PER_DQS)+j], center_tap[i]);      

      // Putting Delay on the DBI Pin
      if(glbl_wr_dbi_en) {
       if(NIBBLE_PER_DQS == 2)
         writeIO_RIU(glbl_dbi_odly_riu_addr_map[i], center_tap[i]);
      }
      // Putting Delay in the DQS pin
      writeIO_RIU( glbl_dqs_odly_riu_addr_map[i], center_tap[i]);

      DEBUG_PRINTF("INFO: ddr_wrlvl(): rank = %d, nibble = %d, center_tap = %x \n", rank, i, center_tap[i]);
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], TRISTATE_ODLY, (center_tap[i] - TRI_ODLY_OFFSET));

      if(NIBBLE_PER_DQS == 2)
        write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], TRISTATE_ODLY, (center_tap[i] - TRI_ODLY_OFFSET));
    #else

      // Skip for Non-DQS nibble, since TRISTATE is associated with Bit-slice 1 which can be CA pin and LP4_WL_DQS cannot be used.
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], LP4_WL_DQS, dqs_coarse[i]);

      // Putting Delay in the DQS pin
      writeIO_RIU( glbl_dqs_odly_riu_addr_map[i], center_tap[i]);

      DEBUG_PRINTF("INFO: ddr_wrlvl(): rank = %d, nibble = %d, center_tap = %x \n", rank, i, center_tap[i]);
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], TRISTATE_ODLY, (center_tap[i] - TRI_ODLY_OFFSET));
    #endif
  }

  return 0;
}


int stable_0_confirmation (u8 rank, u8 *dqs_coarse)
{
  u8 second_iteration = 0;
  u8 stable_0_detect_count[DQS_BITS];
  u8 all_sample_status[DQS_BITS];
  u8 i;
  u16 ft = 0;
  u32 nibble_mask;
  u32 stable_0_detect_done = 0;

  for(i=0; i<DQS_BITS; i++)
  {
    all_sample_status[i] 	= 0;
    stable_0_detect_count[i] 	= 0;
    stable_0_detect_done        |= (1<<i);
  }

  stable_0_detect_done = ~stable_0_detect_done;

  // INCDEC_CRSE value is programmed to Min Valid Count
  update_dly_mask(&stable_0_detect_done, &nibble_mask); 
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK2, nibble_mask);
  riu_reg_bcast(INCDEC_CRSE, 2, MIN_VLD_CNT);

  do
  {
    if(ft == FT_LIMIT_MAX)
    {
      DEBUG_PRINTF("ERROR: ddr_wrlvl(): STABLE_0_CONFIRMATION STAGE FAILED DUE TO MAX TAP REACHED \n");       
      write_error_nibble_XRAM(0, ~stable_0_detect_done);
      return 1;
    } 
     
    //BroadCast Increment Odelay Value
    update_dly_mask(&stable_0_detect_done, &nibble_mask);
    writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);

    #ifdef DDR4
      riu_reg_bcast(ODLY0, 1, 0x8000); // ODLY Inc Dec Crse ... 15 14 13
    #else
      riu_reg_bcast(ODLY1, 1, 0x8000); // ODLY Inc Dec Crse ... 15 14 13
    #endif

    mult_sample_dqs(all_sample_status);

    // Analyzing the Result Sequentially 
    for(i=0; i<DQS_BITS; i++)
    { 
      if((stable_0_detect_done & (1<<i)) == 0)
      {
        if(all_sample_status[i] == 2)
        {
          stable_0_detect_count[i] += 1;

          if(stable_0_detect_count[i] == MIN_VLD_CNT)
          {
            stable_0_detect_done = stable_0_detect_done | (1<<i);
            stable_0_detect_count[i] = 0;
          }
        }
        else
         stable_0_detect_count[i] = 0;
      }
    }

    DEBUG_PRINTF("INFO: ddr_wrlvl(): STABLE_0_CONFIRMATION: ft value = %d, second_iteration = %d \n", ft, second_iteration);
    DEBUG_PRINTF("INFO: ddr_wrlvl(): STABLE_0_CONFIRMATION: stable_0_detect_done = %x \n", stable_0_detect_done);

    if(second_iteration && (stable_0_detect_done != 0xFFFFFFFF) && (ft > glbl_max_limit_90_degree))
    {
      DEBUG_PRINTF("ERROR: ddr_wrlvl(): ERROR IN STABLE_0_CONFIRMATION STAGE IN SECOND ITERATION STAGE\n");
      write_error_nibble_XRAM(0, ~stable_0_detect_done);
      return 1;
    }

    // Decrement the wl_dly_rnk based on the broadcast mechanism and reversing the fine tap to the start value
    if((second_iteration == 0) && (ft == MIN_VLD_CNT-1))
    {
      ft = 0;
      second_iteration = 1;

      for(i=0; i<DQS_BITS; i++)
      {
        if((stable_0_detect_done & (1<<i)) == 0)
        {
          if(dqs_coarse[i] == 0)
            dqs_coarse[i] = 3;
          else
            dqs_coarse[i] = dqs_coarse[i] - 1;
           
          #ifdef DDR4
            write_riu_nibble_reg(glbl_map_riu[i*NIBBLE_PER_DQS], WLDLYRNK0+rank, dqs_coarse[i]<<9);
          #else
            write_riu_nibble_reg(glbl_map_riu[i*NIBBLE_PER_DQS], LP4_WL_DQS, dqs_coarse[i]);
          #endif
        }
      }

      // Broadcast Decrement for all those nibbles for which second iteration need to be done
      update_dly_mask(&stable_0_detect_done, &nibble_mask);
      writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
      
      #ifdef DDR4
        riu_reg_bcast(ODLY0, 1, 0x6000); // ODLY Inc Dec Crse ... 15 14 13
      #else
        riu_reg_bcast(ODLY1, 1, 0x6000); // ODLY Inc Dec Crse ... 15 14 13
      #endif

      // Bit slice Reset
      riu_reg_bcast(BS_RESET_CTRL, 0, 0x1<<CLR_GATE_POS);
      bitslice_reset_bcast(0, 0x1<<BS_RESET_POS);
    }
    else
      ft = ft + 1;

  } while (stable_0_detect_done != 0xFFFFFFFF);

  return 0;
}


void mult_sample_dqs(u8 *all_sample_status)
{
  u8 i, j;
  u32 compare_data_stable_nibble0;
  u32 compare_data_stable_nibble1;

  #ifdef DDR4
    writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 1);                  
  #endif

  writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);
  writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

  writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
  while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
  
  compare_data_stable_nibble0 = readIO_DMC(ral_addr_of_compare_data_stable_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK));
  compare_data_stable_nibble1 = readIO_DMC(ral_addr_of_compare_data_stable_nibble1_in_ddrmc_main_ddrmc_main_bank(BLK));

  for(i=0;i<DQS_BITS; i++) 
  {
    if((i*NIBBLE_PER_DQS) < 9)
      all_sample_status[i] = (0x00000003) & (compare_data_stable_nibble0 >> (2*i*NIBBLE_PER_DQS)); 
    else
      all_sample_status[i] = (0x00000003) & (compare_data_stable_nibble1 >> (2*(i*NIBBLE_PER_DQS-9))); 
  }

  #ifdef DDR4
    writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0);                  
  #endif

  return; 
}


void setup_xiphy_wrlvl(u8 wl_train)
{
  u8 i,j;
  u16 rx_gate   = 0;

  // Enabling write leveling in XPHY
  riu_reg_bcast(WL_TRAIN, 0, wl_train);  

  // Disable the RX gating
  for(i=0; i < DQS_BITS; i++)
  {
    for(j=0; j<NIBBLE_PER_DQS; j++)
    {
      rx_gate = read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + j], CFG_NIBBLE_CTRL0);
    
      if(wl_train == WL_TRAIN_ON)
        rx_gate &= ~RIU_DIS_RX_GATE;
      else if(wl_train == WL_TRAIN_OFF)
        rx_gate |= RIU_DIS_RX_GATE;
 
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + j], CFG_NIBBLE_CTRL0, rx_gate);
    }
  }
}

void get_90degree_phase_remove()
{
  u8 i;
  u32 nibble_ctrl_1;

  for(i=0; i<DQS_BITS; i++)
  {
    nibble_ctrl_1 = read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], CFG_NIBBLE_CTRL1);
   
    write_riu_nibble_reg(glbl_map_riu[(NIBBLE_PER_DQS*i)],CFG_NIBBLE_CTRL1, (nibble_ctrl_1 & 0xFFFFFDF7));

    nibble_ctrl_1 = read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], CFG_NIBBLE_CTRL1);

    write_riu_nibble_reg(glbl_map_riu[(NIBBLE_PER_DQS*i) + 1],CFG_NIBBLE_CTRL1, (nibble_ctrl_1 & 0xFFFFFDFF));
  }
}
