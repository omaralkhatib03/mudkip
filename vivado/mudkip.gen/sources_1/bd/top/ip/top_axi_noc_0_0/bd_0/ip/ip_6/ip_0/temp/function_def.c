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
//       Revision:       $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/function_def.c#16 $
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

#include "bd_d0e7_MC0_ddrc_0_phy_cal_config_define.h"
#include "bd_d0e7_MC0_ddrc_0_phy_io_config_define.h"
#include "xsdb_reg_define.h"

#include  "mc_reg_define.h"
#include  "cal_reg_define.h"
#include  "internal_reg_define.h"
#include  "riu_reg_define.h"
#include "global_variable_define.h"
#include "function_dec.c"
#include "xsdb_reg_define.h"


#ifdef ENABLE_MICROBLAZE_BFM
   
   void send_done(u8 status) {
      send_control_simulation(fd_request, 0, status, DebugLogFile);
      return ;
   }
#endif

   u32 readIO_DMC(u32 ioAddr) {
      u32 read_data;
      u32 io_address = (DDRMC | ioAddr);
#ifdef ENABLE_MICROBLAZE_BFM
      #ifdef MICROBLAZE_BFM_DEBUG_VERBOSITY_HIGH
         fprintf(DebugLogFile, "INFO: readIO_DMC(): Launch read request:  Addr=%x io_address=%x \n", ioAddr, io_address);
      #endif
      read_data = readIO_simulation(fd_request, fd_response, io_address, DebugLogFile);
      #ifdef MICROBLAZE_BFM_DEBUG_VERBOSITY_HIGH
         fprintf(DebugLogFile, "INFO: readIO_DMC(): Return read response: Addr=%x io_address=%x read_data=%x\n", ioAddr, io_address, read_data);
      #endif
#else
      read_data = Xil_In32(io_address);
#endif
      return read_data;
   }

   void writeIO_DMC(u32 ioAddr, u32 ioData) {
      u32 io_address = (DDRMC | ioAddr);
#ifdef ENABLE_MICROBLAZE_BFM
      #ifdef MICROBLAZE_BFM_DEBUG_VERBOSITY_HIGH
         fprintf(DebugLogFile, "INFO: writeIO_DMC(): Launch write request: Addr=%x, io_address=%x Data=%x\n", ioAddr, io_address, ioData);
      #endif
      writeIO_simulation(fd_request, io_address, ioData, DebugLogFile);
#else
      Xil_Out32(io_address, ioData);
#endif
      return;
   }

   u32 readIO_RIU(u32 ioAddr) {
      u32 read_data;
      u32 io_address = (X_PHY_RIU | (ioAddr << 2));
#ifdef ENABLE_MICROBLAZE_BFM
      #ifdef MICROBLAZE_BFM_DEBUG_VERBOSITY_HIGH
         fprintf(DebugLogFile, "INFO: readIO_RIU(): Launch read request:  Addr=%x io_address=%x \n", ioAddr, io_address);
      #endif
      read_data = readIO_simulation(fd_request, fd_response, io_address, DebugLogFile);
      #ifdef MICROBLAZE_BFM_DEBUG_VERBOSITY_HIGH
         fprintf(DebugLogFile, "INFO: readIO_RIU(): Return read response: Addr=%x io_address=%x read_data=%x\n", ioAddr, io_address, read_data);
      #endif
#else
      read_data = Xil_In32(io_address);
#endif
      return read_data;
   }

   void writeIO_RIU(u32 ioAddr, u32 ioData) {
      u32 io_address = (X_PHY_RIU | (ioAddr << 2));
#ifdef ENABLE_MICROBLAZE_BFM
      #ifdef MICROBLAZE_BFM_DEBUG_VERBOSITY_HIGH
         fprintf(DebugLogFile, "INFO: writeIO_RIU(): Launch write request: Addr=%x, io_address=%x Data=%x\n", ioAddr, io_address, ioData);
      #endif
      writeIO_simulation(fd_request, io_address, ioData, DebugLogFile);
#else
      Xil_Out32(io_address, ioData);
#endif
      return;
   }

   u32 readIO_XRAM(u32 ioAddr) {
      u32 read_data;
      u32 io_address = EXCHANGE_RAM | ioAddr;
#ifdef ENABLE_MICROBLAZE_BFM
      #ifdef MICROBLAZE_BFM_DEBUG_VERBOSITY_HIGH
         fprintf(DebugLogFile, "INFO: readIO_XRAM(): Launch read request:  Addr=%x io_address=%x \n", ioAddr, io_address);
      #endif
      read_data = readIO_simulation(fd_request, fd_response, io_address, DebugLogFile);
      #ifdef MICROBLAZE_BFM_DEBUG_VERBOSITY_HIGH
         fprintf(DebugLogFile, "INFO: readIO_XRAM(): Return read response: Addr=%x io_address=%x read_data=%x\n", ioAddr, io_address, read_data);
      #endif
#else
      read_data = Xil_In32(io_address);
#endif
      return read_data;
   }

   void writeIO_XRAM(u32 ioAddr, u32 ioData) {
      u32 io_address = EXCHANGE_RAM | ioAddr;
#ifdef ENABLE_MICROBLAZE_BFM
      #ifdef MICROBLAZE_BFM_DEBUG_VERBOSITY_HIGH
         fprintf(DebugLogFile, "INFO: writeIO_XRAM(): Launch write request: Addr=%x, io_address=%x Data=%x\n", ioAddr, io_address, ioData);
      #endif
      writeIO_simulation(fd_request, io_address, ioData, DebugLogFile);
#else
      Xil_Out32(io_address, ioData);
#endif
      return;
   }

   u32 readIO_MBDR(u32 ioAddr) {
      u32 read_data;
      u32 io_address = RAM_DATA | ioAddr;
#ifdef ENABLE_MICROBLAZE_BFM
      #ifdef MICROBLAZE_BFM_DEBUG_VERBOSITY_HIGH
         fprintf(DebugLogFile, "INFO: readIO_MBDR(): Launch read request:  Addr=%x io_address=%x \n", ioAddr, io_address);
      #endif
      read_data = readIO_simulation(fd_request, fd_response, io_address, DebugLogFile);
      #ifdef MICROBLAZE_BFM_DEBUG_VERBOSITY_HIGH
         fprintf(DebugLogFile, "INFO: readIO_MBDR(): Return read response: Addr=%x io_address=%x read_data=%x\n", ioAddr, io_address, read_data);
      #endif
#else
      read_data = Xil_In32(io_address);
#endif
      return read_data;
   }

   void writeIO_MBDR(u32 ioAddr, u32 ioData) {
      u32 io_address = RAM_DATA | ioAddr;
#ifdef ENABLE_MICROBLAZE_BFM
      #ifdef MICROBLAZE_BFM_DEBUG_VERBOSITY_HIGH
         fprintf(DebugLogFile, "INFO: writeIO_MBDR(): Launch write request: Addr=%x, io_address=%x Data=%x\n", ioAddr, io_address, ioData);
      #endif
      writeIO_simulation(fd_request, io_address, ioData, DebugLogFile);
#else
      Xil_Out32(io_address, ioData);
#endif
      return;
   }

   u32 readIO_IOM(u32 ioAddr) {
      u32 read_data;
      u32 io_address = ioAddr;
#ifdef ENABLE_MICROBLAZE_BFM
      #ifdef MICROBLAZE_BFM_DEBUG_VERBOSITY_HIGH
         fprintf(DebugLogFile, "INFO: readIO_IOM(): Launch read request:  Addr=%x io_address=%x \n", ioAddr, io_address);
      #endif
      read_data = readIO_simulation(fd_request, fd_response, io_address, DebugLogFile);
      #ifdef MICROBLAZE_BFM_DEBUG_VERBOSITY_HIGH
         fprintf(DebugLogFile, "INFO: readIO_IOM(): Return read response: Addr=%x io_address=%x read_data=%x\n", ioAddr, io_address, read_data);
      #endif
#else
      read_data = Xil_In32(io_address);
#endif
      return read_data;
   }

   void writeIO_IOM(u32 ioAddr, u32 ioData) {
      u32 io_address = ioAddr;
#ifdef ENABLE_MICROBLAZE_BFM
      #ifdef MICROBLAZE_BFM_DEBUG_VERBOSITY_HIGH
         fprintf(DebugLogFile, "INFO: writeIO_IOM(): Launch write request: Addr=%x, io_address=%x Data=%x\n", ioAddr, io_address, ioData);
      #endif
      writeIO_simulation(fd_request, io_address, ioData, DebugLogFile);
#else
      Xil_Out32(io_address, ioData);
#endif
      return;
   }

u32 readIO_FABRIC(u32 ioAddr) {
      u32 read_data;
      u32 io_address = (FABRIC | (ioAddr << 2));
#ifdef ENABLE_MICROBLAZE_BFM
      #ifdef MICROBLAZE_BFM_DEBUG_VERBOSITY_HIGH
         fprintf(DebugLogFile, "INFO: readIO_FABRIC(): Launch read request:  Addr=%x io_address=%x \n", ioAddr, io_address);
      #endif
      read_data = readIO_simulation(fd_request, fd_response, io_address, DebugLogFile);
      #ifdef MICROBLAZE_BFM_DEBUG_VERBOSITY_HIGH
         fprintf(DebugLogFile, "INFO: readIO_FABRIC(): Return read response: Addr=%x io_address=%x read_data=%x\n", ioAddr, io_address, read_data);
      #endif
#else
      read_data = Xil_In32(io_address);
#endif
      return read_data;
   }

void writeIO_FABRIC(u32 ioAddr, u32 ioData) {
      u32 io_address = (FABRIC | (ioAddr << 2));
#ifdef ENABLE_MICROBLAZE_BFM
      #ifdef MICROBLAZE_BFM_DEBUG_VERBOSITY_HIGH
         fprintf(DebugLogFile, "INFO: writeIO_FABRIC(): Launch write request: Addr=%x, io_address=%x Data=%x\n", ioAddr, io_address, ioData);
      #endif
      writeIO_simulation(fd_request, io_address, ioData, DebugLogFile);
#else
      Xil_Out32(io_address, ioData);
#endif
      return;
   }

void riu_reg_bcast(u32 reg_addr, u8 mask_reg_num, u32 bcast_reg_val) {
   u32 riu_addr =  (
                      ( (reg_addr << RIU_REG_ADDR_POS) & RIU_REG_ADDR_MASK ) | 
                      ( (mask_reg_num << RIU_BCAST_MASK_POS) & RIU_BCAST_MASK_MASK ) | 
                      ( (0x1 << RIU_BCAST_EN_POS) & RIU_BCAST_EN_MASK )  
                   );
   //riu_addr = riu_addr<<2;
   writeIO_RIU(riu_addr,bcast_reg_val);
}

void set_1_seq_command_at_mem(u8 rank, u32 nos_iteration_of_seqs, 
                   u32 seq_a_cmd, u32 seq_a_addr, u8 seq_a_bg, u16 seq_a_ba,
                   u32 seq_a2a_dly)
{

   u32 cnts;
   u16 rd_cmd_cnts = 0;
   u16 total_cmd_cnts;
   u8 mirror_en = (glbl_xpi_mirror_en & (seq_a_cmd == MEM_MODE_REG_WRITE) & (rank%2 == 1)) ? 1 : 0;

   //writeIO_DMC(ral_addr_of_seq_cs_bits_in_ddrmc_main_ddrmc_main_bank(BLK),1<<rank); //Rank select           
   writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK),rank); // Selects xphy rank
   writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (0x10<<rank) | (mirror_en << 1)); // Selects DDR Mem rank

   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),seq_a_cmd); //seq_*_cmd[5*4-1:0] is composed of the CS,ACT,RAS,CAS,WE for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), seq_a_addr); //seq_*_addr[8*4-1:0] is composed of the Address[17:0] for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), seq_a_ba); //seq_*_ba[2*4-1:0] is composed of the BA[1:0] for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), seq_a_bg); //seq_*_bg[2*4-1:0] is composed of the BG[1:0] for four edges of the memory clock (per controller clock)

   writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK),seq_a2a_dly); //delay in controller clock from sequence B to sequence A assertion if two sequences are enabled 

   if(!glbl_wrlvl)
     writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0); // Sequence mode - 0: Normal mode; 1: Write level Mode; 2: Address calibration mode;

   writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //number of sequences - 0: Sequence A enabled; 1: Sequence A, and B enabled; 2: Sequence A, B, and C enabled;

   total_cmd_cnts = nos_iteration_of_seqs;
   if(seq_a_cmd == MEM_COL_READ) {
      rd_cmd_cnts = total_cmd_cnts ;
   } 

   cnts = rd_cmd_cnts ;
   cnts = (cnts << 16) | (total_cmd_cnts & 0x0000FFFF); 

   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), cnts); // number of iterartion of the sequences; 
   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

   #ifdef DDR4
     // Writing on SIDE_B if this is RDIMM
     if(glbl_rdimm_en & (seq_a_cmd == MEM_MODE_REG_WRITE) & ((seq_a_bg != 0x1) | (seq_a_ba != 0x3))) // Write to Side-B only for MRS commands: only for MRS0-6. Exclude MRS7.
     {
        // Enable the inversion 
        writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (0x10<<rank) | (mirror_en << 1) | 0x4); 

        // Writing for Side B
        writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
        while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

        // Disable the inversion & mirror
        writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (0x10<<rank)); 
     } 

     if((glbl_clamshell_cfg == 1) && (seq_a_cmd == MEM_MODE_REG_WRITE)) { // Clamshell configuration, mode register write
        // Enable the mirror
        mirror_en = glbl_xpi_mirror_en;
        writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (0x10<<rank) | (mirror_en << 1)); // Selects DDR Mem rank

        // Writing to Mirrored components of clamshell
        writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
        while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

        // Disable the mirror
        writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (0x10<<rank)); 
     }
   #endif  
}

void set_2_seq_command_at_mem(u8 rank, u32 nos_iteration_of_seqs, u8 full_write, 
                   u32 seq_a_cmd, u32 seq_a_addr, u8 seq_a_bg, u16 seq_a_ba,
                   u32 seq_b_cmd, u32 seq_b_addr, u8 seq_b_bg, u16 seq_b_ba,
                   u32 seq_a2b_dly, u32 seq_b2a_dly)
{

   u32 cnts;
   u16 rd_cmd_cnts = 0;
   u16 total_cmd_cnts;
   u32 extend_write_data = readIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK));

   writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK),rank); // Selects xphy rank
   writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),((0x10<<rank) | (extend_write_data & 0xFFFFFF0F))); // Selects DDR Mem rank

   writeIO_DMC(ral_addr_of_seq_b_rank_in_ddrmc_main_ddrmc_main_bank(BLK),rank); // Selects xphy rank
   writeIO_DMC(ral_addr_of_seq_b_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10<<rank); // Selects DDR Mem rank

   if(full_write)
   {
     if (glbl_margin_check) { //Sending activate only for 2D eye scan
       set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, MEM_ROW_ACTIVE /*u32 seq_a_cmd*/, 
                                  0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/, 0 /*u32 seq_a2a_dly*/);
     }
     writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),seq_a_cmd); //seq_*_cmd[5*4-1:0] is composed of the CS,ACT,RAS,CAS,WE for four edges of the memory clock (per controller clock)
     writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), seq_a_addr); //seq_*_addr[8*4-1:0] is composed of the Address[17:0] for four edges of the memory clock (per controller clock)
     writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), seq_a_ba); //seq_*_ba[2*4-1:0] is composed of the BA[1:0] for four edges of the memory clock (per controller clock)
     writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), seq_a_bg); //seq_*_bg[2*4-1:0] is composed of the BG[1:0] for four edges of the memory clock (per controller clock)

     writeIO_DMC(ral_addr_of_seq_b_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),seq_b_cmd); //seq_*_cmd[5*4-1:0] is composed of the CS,ACT,RAS,CAS,WE for four edges of the memory clock (per controller clock)
     writeIO_DMC(ral_addr_of_seq_b_addr_in_ddrmc_main_ddrmc_main_bank(BLK), seq_b_addr); //seq_*_addr[8*4-1:0] is composed of the Address[17:0] for four edges of the memory clock (per controller clock)
     writeIO_DMC(ral_addr_of_seq_b_ba_in_ddrmc_main_ddrmc_main_bank(BLK), seq_b_ba); //seq_*_ba[2*4-1:0] is composed of the BA[1:0] for four edges of the memory clock (per controller clock)
     writeIO_DMC(ral_addr_of_seq_b_bg_in_ddrmc_main_ddrmc_main_bank(BLK), seq_b_bg); //seq_*_bg[2*4-1:0] is composed of the BG[1:0] for four edges of the memory clock (per controller clock)

     writeIO_DMC(ral_addr_of_seq_a_b_dly_in_ddrmc_main_ddrmc_main_bank(BLK),seq_a2b_dly); //delay in controller clock from sequence A to sequence B assertion if two or three command sequence is enabled 
     writeIO_DMC(ral_addr_of_seq_b_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK),seq_b2a_dly); //delay in controller clock from sequence B to sequence A assertion if two sequences are enabled 


     writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0); // Sequence mode - 0: Normal mode; 1: Write level Mode; 2: Address calibration mode;
     writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 1); //number of sequences - 0: Sequence A enabled; 1: Sequence A, and B enabled; 2: Sequence A, B, and C enabled;

     total_cmd_cnts = nos_iteration_of_seqs;
     if(seq_a_cmd == MEM_COL_READ) {
        rd_cmd_cnts = total_cmd_cnts >> 1;
     } 
     if(seq_b_cmd == MEM_COL_READ) {
        rd_cmd_cnts = rd_cmd_cnts + (total_cmd_cnts >> 1);
     }

     cnts = rd_cmd_cnts ;
     cnts = (cnts << 16) | (total_cmd_cnts & 0x0000FFFF); 

     writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), cnts); // number of iterartion of the sequences; 
   }

   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
}

void set_3_seq_command_at_mem(u8 rank, u32 nos_iteration_of_seqs, 
                   u32 seq_a_cmd, u32 seq_a_addr, u8 seq_a_bg, u16 seq_a_ba,
                   u32 seq_b_cmd, u32 seq_b_addr, u8 seq_b_bg, u16 seq_b_ba,
                   u32 seq_c_cmd, u32 seq_c_addr, u8 seq_c_bg, u16 seq_c_ba,
                   u32 seq_a2b_dly, u32 seq_b2c_dly, u32 seq_c2a_dly)
{

   u32 cnts;
   u16 rd_cmd_cnts = 0;
   u16 total_cmd_cnts;

   writeIO_DMC(ral_addr_of_seq_cs_bits_in_ddrmc_main_ddrmc_main_bank(BLK),1<<rank); //Rank select           
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),seq_a_cmd); //seq_*_cmd[5*4-1:0] is composed of the CS,ACT,RAS,CAS,WE for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), seq_a_addr); //seq_*_addr[8*4-1:0] is composed of the Address[17:0] for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), seq_a_ba); //seq_*_ba[2*4-1:0] is composed of the BA[1:0] for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), seq_a_bg); //seq_*_bg[2*4-1:0] is composed of the BG[1:0] for four edges of the memory clock (per controller clock)

   writeIO_DMC(ral_addr_of_seq_b_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),seq_b_cmd); //seq_*_cmd[5*4-1:0] is composed of the CS,ACT,RAS,CAS,WE for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_b_addr_in_ddrmc_main_ddrmc_main_bank(BLK), seq_b_addr); //seq_*_addr[8*4-1:0] is composed of the Address[17:0] for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_b_ba_in_ddrmc_main_ddrmc_main_bank(BLK), seq_b_ba); //seq_*_ba[2*4-1:0] is composed of the BA[1:0] for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_b_bg_in_ddrmc_main_ddrmc_main_bank(BLK), seq_b_bg); //seq_*_bg[2*4-1:0] is composed of the BG[1:0] for four edges of the memory clock (per controller clock)

   writeIO_DMC(ral_addr_of_seq_c_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),seq_c_cmd); //seq_*_cmd[5*4-1:0] is composed of the CS,ACT,RAS,CAS,WE for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_c_addr_in_ddrmc_main_ddrmc_main_bank(BLK), seq_c_addr); //seq_*_addr[8*4-1:0] is composed of the Address[17:0] for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_c_ba_in_ddrmc_main_ddrmc_main_bank(BLK), seq_c_ba); //seq_*_ba[2*4-1:0] is composed of the BA[1:0] for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_c_bg_in_ddrmc_main_ddrmc_main_bank(BLK), seq_c_bg); //seq_*_bg[2*4-1:0] is composed of the BG[1:0] for four edges of the memory clock (per controller clock)

   writeIO_DMC(ral_addr_of_seq_a_b_dly_in_ddrmc_main_ddrmc_main_bank(BLK),seq_a2b_dly); //delay in controller clock from sequence A to sequence B assertion if two or three command sequence is enabled 
   writeIO_DMC(ral_addr_of_seq_b_c_dly_in_ddrmc_main_ddrmc_main_bank(BLK),seq_b2c_dly); //delay in controller clock from sequence B to sequence C assertion if three sequences are enabled 
   writeIO_DMC(ral_addr_of_seq_c_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK),seq_c2a_dly); //delay in controller clock from sequence C to sequence A assertion if three sequences are enabled 


   writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0); // Sequence mode - 0: Normal mode; 1: Write level Mode; 2: Address calibration mode;
   writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 2); //number of sequences - 0: Sequence A enabled; 1: Sequence A, and B enabled; 2: Sequence A, B, and C enabled;

   total_cmd_cnts = nos_iteration_of_seqs;
   if(seq_a_cmd == MEM_COL_READ) {
      rd_cmd_cnts = total_cmd_cnts/3;
   } 
   if(seq_b_cmd == MEM_COL_READ) {
      rd_cmd_cnts = rd_cmd_cnts + (total_cmd_cnts/3);
   }
   if(seq_c_cmd == MEM_COL_READ) {
      rd_cmd_cnts = rd_cmd_cnts + (total_cmd_cnts/3);
   }

   cnts = rd_cmd_cnts ;
   cnts = (cnts << 16) | (total_cmd_cnts & 0x0000FFFF); 

   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), cnts); // number of iterartion of the sequences; 
   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

}

void set_command_at_mem(u8 rank, u8 seq_cke, u8 seq_reset, u8 seq_mode, u8 nos_of_seq, u32 nos_iteration_of_seqs, 
                   u32 seq_a_cmd, u32 seq_a_addr, u8 seq_a_bg, u16 seq_a_ba,
                   u32 seq_b_cmd, u32 seq_b_addr, u8 seq_b_bg, u16 seq_b_ba,
                   u32 seq_c_cmd, u32 seq_c_addr, u8 seq_c_bg, u16 seq_c_ba,
                   u32 seq_a2a_dly, u32 seq_a2b_dly, u32 seq_b2a_dly, u32 seq_b2c_dly, u32 seq_c2a_dly)
{

   u32 cnts;
   u16 rd_cmd_cnts = 0;
   u16 total_cmd_cnts;

   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),seq_a_cmd); //seq_*_cmd[5*4-1:0] is composed of the CS,ACT,RAS,CAS,WE for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), seq_a_addr); //seq_*_addr[8*4-1:0] is composed of the Address[17:0] for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), seq_a_ba); //seq_*_ba[2*4-1:0] is composed of the BA[1:0] for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), seq_a_bg); //seq_*_bg[2*4-1:0] is composed of the BG[1:0] for four edges of the memory clock (per controller clock)

   writeIO_DMC(ral_addr_of_seq_b_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),seq_b_cmd); //seq_*_cmd[5*4-1:0] is composed of the CS,ACT,RAS,CAS,WE for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_b_addr_in_ddrmc_main_ddrmc_main_bank(BLK), seq_b_addr); //seq_*_addr[8*4-1:0] is composed of the Address[17:0] for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_b_ba_in_ddrmc_main_ddrmc_main_bank(BLK), seq_b_ba); //seq_*_ba[2*4-1:0] is composed of the BA[1:0] for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_b_bg_in_ddrmc_main_ddrmc_main_bank(BLK), seq_b_bg); //seq_*_bg[2*4-1:0] is composed of the BG[1:0] for four edges of the memory clock (per controller clock)

   writeIO_DMC(ral_addr_of_seq_c_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),seq_c_cmd); //seq_*_cmd[5*4-1:0] is composed of the CS,ACT,RAS,CAS,WE for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_c_addr_in_ddrmc_main_ddrmc_main_bank(BLK), seq_c_addr); //seq_*_addr[8*4-1:0] is composed of the Address[17:0] for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_c_ba_in_ddrmc_main_ddrmc_main_bank(BLK), seq_c_ba); //seq_*_ba[2*4-1:0] is composed of the BA[1:0] for four edges of the memory clock (per controller clock)
   writeIO_DMC(ral_addr_of_seq_c_bg_in_ddrmc_main_ddrmc_main_bank(BLK), seq_c_bg); //seq_*_bg[2*4-1:0] is composed of the BG[1:0] for four edges of the memory clock (per controller clock)

   writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK),seq_a2a_dly); //delay in controller clock from sequence A to next sequence A assertion  if only one sequence is enabled
   writeIO_DMC(ral_addr_of_seq_a_b_dly_in_ddrmc_main_ddrmc_main_bank(BLK),seq_a2b_dly); //delay in controller clock from sequence A to sequence B assertion if two or three command sequence is enabled 
   writeIO_DMC(ral_addr_of_seq_b_c_dly_in_ddrmc_main_ddrmc_main_bank(BLK),seq_b2c_dly); //delay in controller clock from sequence B to sequence C assertion if three sequences are enabled 
   writeIO_DMC(ral_addr_of_seq_c_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK),seq_c2a_dly); //delay in controller clock from sequence C to sequence A assertion if three sequences are enabled 
   writeIO_DMC(ral_addr_of_seq_b_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK),seq_b2a_dly); //delay in controller clock from sequence B to sequence A assertion if two sequences are enabled 


   writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), seq_mode); // Sequence mode - 0: Normal mode; 1: Write level Mode; 2: Address calibration mode;
   writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), nos_of_seq); //number of sequences - 0: Sequence A enabled; 1: Sequence A, and B enabled; 2: Sequence A, B, and C enabled;

   total_cmd_cnts = nos_iteration_of_seqs;
   if(seq_a_cmd == MEM_COL_READ) {
      rd_cmd_cnts = total_cmd_cnts/3;
   } 
   if(seq_b_cmd == MEM_COL_READ) {
      rd_cmd_cnts = rd_cmd_cnts + (total_cmd_cnts/3);
   }
   if(seq_c_cmd == MEM_COL_READ) {
      rd_cmd_cnts = rd_cmd_cnts + (total_cmd_cnts/3);
   }

   cnts = rd_cmd_cnts ;
   cnts = (cnts << 16) | (total_cmd_cnts & 0x0000FFFF); 

   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), cnts); // number of iterartion of the sequences; 
   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
}


void initialize_u32_array (u32 *array_start_addr, u8 array_size, u32 init_value) {
   u8 nibble;
   for(nibble=0; nibble < array_size; nibble++) {
      array_start_addr[nibble] = init_value;
   }
}
void initialize_u16_array (u16 *array_start_addr, u8 array_size, u16 init_value) {
   u8 nibble;
   for(nibble=0; nibble < array_size; nibble++) {
      array_start_addr[nibble] = init_value;
   }
}

void initialize_u8_array (u8 *array_start_addr, u8 array_size, u8 init_value) {
   u8 nibble;
   for(nibble=0; nibble < array_size; nibble++) {
      array_start_addr[nibble] = init_value;
   }
}

void inc_u8_array (u8 *array_start_addr, u8 array_size, u32 mask, u8 step_size) {
   u8 nibble;
   DEBUG_PRINTF("INFO: inc_u8_array(): Noise window increment mask = 0x%08X \n", mask);
   for(nibble=0; nibble < array_size; nibble++, mask = mask >> 1) {
      if(!((mask) & 0x1)) {
         array_start_addr[nibble] = array_start_addr[nibble] + step_size;
         DEBUG_PRINTF("INFO: inc_u8_array(): Noise window width [%d] = %d \n", nibble, array_start_addr[nibble]);
      }
   }
}


void center_align (u8 first_element, u8 last_element, u32 elements_mask, u32 delay_offset,u8 rank,u8 *window_width) { 
   u8 i;
   u8 index;
   u16 read_delay;
   u16 fine_delay;
   u16 coarse_fine_delay;
   index = 0;
   for(i = first_element; i <= last_element; i++) {
      if(!((elements_mask >> i) & 0x1)) {
         read_delay = read_riu_nibble_reg(i,RLDLYRNK0+rank);
         fine_delay = ((read_delay & 0x1FF)-GT_STATUS_B2B_STABLE_ONE_COUNT)-(window_width[index] - GT_STATUS_B2B_STABLE_ONE_COUNT)/2;
         coarse_fine_delay = (read_delay & 0x1E00) | (fine_delay & 0x01FF);
         write_riu_nibble_reg(i /*u8 nibble*/, RLDLYRNK0+ rank /*u8 reg_addr*/, coarse_fine_delay|0x8000);
         //Reset Bit slice
         // read_mod_write_riu_nibble_reg(i /*u8 nibble*/, RIU_ADDR_BS_CTRL /*u8 reg_addr*/, 0xFF /*u16 data*/, 0 /*u8 mask_pos*/, RIU_BS_RESET_MASK /*u16 mask*/); 
         // read_mod_write_riu_nibble_reg(i /*u8 nibble*/, RIU_ADDR_BS_CTRL /*u8 reg_addr*/, 0x00 /*u16 data*/, 0 /*u8 mask_pos*/, RIU_BS_RESET_MASK /*u16 mask*/); 
         write_riu_nibble_reg(i /*u8 nibble*/, BS_RESET_CTRL /*u8 reg_addr*/, BS_RESET_WT); // bit slice resets
         write_riu_nibble_reg(i /*u8 nibble*/, BS_RESET_CTRL /*u8 reg_addr*/, 0x0); // bit slice resets
         DEBUG_PRINTF("INFO: center_align(): rank = %d, nibble = %d, noise width = %d, coarse = %d, fine = %d \n", rank, i, window_width[index]-GT_STATUS_B2B_STABLE_ONE_COUNT+1, (coarse_fine_delay>>9), fine_delay);
         index++;
      }
   }
}

void write_riu_nibble_reg(u8 nibble,u8 reg_addr, u16 data) {
   writeIO_RIU((reg_addr & RIU_REG_ADDR_MASK) | (nibble << RIU_NIB_POS), data); //0x1000 is ORed to enable riu access
}

u16 read_riu_nibble_reg(u8 nibble,u8 reg_addr) {
   u16 riu_rdata;
   riu_rdata = readIO_RIU((reg_addr & RIU_REG_ADDR_MASK) | (nibble << RIU_NIB_POS)); //0x1000 is ORed to enable riu access
   return riu_rdata;
}

void read_mod_write_riu_nibble_reg(u8 nibble,u8 reg_addr, u16 data, u8 mask_pos, u16 mask) {
   u16 read_data;
   u16 write_data;
   read_data = read_riu_nibble_reg(nibble,reg_addr);
   read_data = read_data & (~(mask << mask_pos));
   write_data = read_data | ((mask & data) << mask_pos); 
   write_riu_nibble_reg(nibble,reg_addr, write_data); 
}

u8 get_write_latency() {
   u8 wr_lat;
   u8 wr_lat_value;
   u8 cwl;
   u32 reg_safe_config5;
   #ifdef LPDDR4
      u8 wr_lat_set_b;
      wr_lat = (glbl_lp4_mr2 >> 3) & 0x7;
      wr_lat_set_b = (glbl_lp4_mr2 >> 6) & 0x1;
      glbl_wr_pre_2tck_cwl_adjust = 0;
      if(wr_lat_set_b) {
         switch (wr_lat) {
            case 0x1: wr_lat_value = 8; break;
            case 0x2: wr_lat_value = 12; break;
            case 0x3: wr_lat_value = 18; break;
            case 0x4: wr_lat_value = 22; break;
            case 0x5: wr_lat_value = 26; break;
            case 0x6: wr_lat_value = 30; break;
            case 0x7: wr_lat_value = 34; break;
            default: wr_lat_value = 4; break;
         }
      } else {
         switch (wr_lat) {
            case 0x1: wr_lat_value = 6; break;
            case 0x2: wr_lat_value = 8; break;
            case 0x3: wr_lat_value = 10; break;
            case 0x4: wr_lat_value = 12; break;
            case 0x5: wr_lat_value = 14; break;
            case 0x6: wr_lat_value = 16; break;
            case 0x7: wr_lat_value = 18; break;
            default: wr_lat_value = 4; break;
         }
      }
      DEBUG_PRINTF("INFO: get_write_latency(): MR2 = 0x%04X , Cas Write Latency = %d \n", glbl_lp4_mr2, wr_lat_value);
   #else
      wr_lat = (glbl_mr2 >> 3) & 0x7;
      switch (wr_lat) {
         case 0x1: wr_lat_value = 10; break;
         case 0x2: wr_lat_value = 11; break;
         case 0x3: wr_lat_value = 12; break;
         case 0x4: wr_lat_value = 14; break;
         case 0x5: wr_lat_value = 16; break;
         case 0x6: wr_lat_value = 18; break;
         case 0x7: wr_lat_value = 20; break;
         default: wr_lat_value = 9; break;
      }
      DEBUG_PRINTF("INFO: get_write_latency(): MR2 = 0x%04X , Cas Write Latency = %d \n", glbl_mr2, wr_lat_value);

      // Check for write latency adjustment in 2 tCK write preamble mode
      // CWL adjustment is needed for 1st set. CAL adjustment is NOT needed for 2nd set.
      if(glbl_wr_pre_2tck_mode) { // 2 tCK write preamble mode
        if(wr_lat == 0x7) { // CWL = 20. always 2nd set
          glbl_wr_pre_2tck_cwl_adjust = 0;
        } else if(wr_lat == 0x6) { // CWL = 18
          if(glbl_tck < 750) { // 1st set. > 2666
            glbl_wr_pre_2tck_cwl_adjust = 1;
          } else { // 1st set
            glbl_wr_pre_2tck_cwl_adjust = 0;
          }
        } else if(wr_lat == 0x5) { // CWL = 16
          if(glbl_tck < 833) { // 1st set. > 2400
            glbl_wr_pre_2tck_cwl_adjust = 1;
          } else { // 1st set
            glbl_wr_pre_2tck_cwl_adjust = 0;
          }
        } else { // CWL = 14. always 1st set
          glbl_wr_pre_2tck_cwl_adjust = 1;
        }
        DEBUG_PRINTF("INFO: get_write_latency(): 2 tCK write preamble mode. tck = %d, wr_lat = %d, cwl adjust = %d\n", glbl_tck, wr_lat, glbl_wr_pre_2tck_cwl_adjust);
      } else {
        glbl_wr_pre_2tck_cwl_adjust = 0;
        DEBUG_PRINTF("INFO: get_write_latency(): 1 tCK write preamble mode\n");
      }

      wr_lat = (glbl_mr5[0] & 0x7);
      switch (wr_lat) {
        case 0x1: wr_lat_value += 4; break; 
        case 0x2: wr_lat_value += 5; break;
        case 0x3: wr_lat_value += 6; break;
        case 0x4: wr_lat_value += 8; break;
        default: wr_lat_value = wr_lat_value; break;
      }
      DEBUG_PRINTF("INFO: get_write_latency(): MR5 = 0x%04X , Cas Write Latency = %d \n", glbl_mr5[0], wr_lat_value);
   #endif

   reg_safe_config5 = readIO_DMC(ral_addr_of_reg_safe_config5_in_ddrmc_main_ddrmc_main_bank(BLK));
   cwl = (reg_safe_config5 >> 8) & 0x7F; // [14:8]

   reg_safe_config5 = reg_safe_config5 & ~(0xF << 28); // [31:28]
   reg_safe_config5 = reg_safe_config5 | (glbl_add_cmd_delay << 28) | (glbl_add_cmd_delay_en << 31);

   writeIO_DMC(ral_addr_of_reg_safe_config5_in_ddrmc_main_ddrmc_main_bank(BLK), reg_safe_config5);
   
   if(glbl_add_cmd_delay_en) {
      wr_lat_value += ((glbl_add_cmd_delay + 1) << 1);
      DEBUG_PRINTF("INFO: get_write_latency(): Add command delay enabled. add_cmd_delay = %d, Cas Write Latency = %d\n", glbl_add_cmd_delay, wr_lat_value);
   }

   DEBUG_PRINTF("INFO: get_write_latency(): reg_safe_config5.cwl = %d, Cas Write Latency = %d\n", cwl, wr_lat_value);
   return wr_lat_value;
}

u8 get_read_latency() {
   u8 cas_latency;
   #ifdef LPDDR4
      cas_latency = (glbl_lp4_mr2 & 0x7); 
      switch (cas_latency) {
         case 0x0:  cas_latency = 6;  glbl_mem_read_latency_dbi_offset = 0; break;
         case 0x1:  cas_latency = 10; glbl_mem_read_latency_dbi_offset = 2; break;
         case 0x2:  cas_latency = 14; glbl_mem_read_latency_dbi_offset = 2; break;
         case 0x3:  cas_latency = 20; glbl_mem_read_latency_dbi_offset = 2; break;
         case 0x4:  cas_latency = 24; glbl_mem_read_latency_dbi_offset = 4; break;
         case 0x5:  cas_latency = 28; glbl_mem_read_latency_dbi_offset = 4; break;
         case 0x6:  cas_latency = 32; glbl_mem_read_latency_dbi_offset = 4; break;
         case 0x7:  cas_latency = 36; glbl_mem_read_latency_dbi_offset = 4; break;
         default:   cas_latency = 6;  glbl_mem_read_latency_dbi_offset = 0; break;
      }
      DEBUG_PRINTF("INFO: get_read_latency(): LPDDR4 MR2 = 0x%04X , Cas Latency = %d \n", glbl_lp4_mr2, cas_latency);
   #else
      cas_latency = (((glbl_mr0 >> 12) & 0x1) << 4) | (((glbl_mr0 >> 4) & 0x7) << 1) | ((glbl_mr0 >> 2) & 0x1);
      switch (cas_latency) {
         case 0x0:  cas_latency = 9;  break;
         case 0x1:  cas_latency = 10; break;
         case 0x2:  cas_latency = 11; break;
         case 0x3:  cas_latency = 12; break;
         case 0x4:  cas_latency = 13; break;
         case 0x5:  cas_latency = 14; break;
         case 0x6:  cas_latency = 15; break;
         case 0x7:  cas_latency = 16; break;
         case 0x8:  cas_latency = 18; break;
         case 0x9:  cas_latency = 20; break;
         case 0xA:  cas_latency = 22; break;
         case 0xB:  cas_latency = 24; break;
         case 0xC:  cas_latency = 23; break;
         case 0xD:  cas_latency = 17; break;
         case 0xE:  cas_latency = 19; break;
         case 0xF:  cas_latency = 21; break;
         case 0x10: cas_latency = 25; break;
         case 0x11: cas_latency = 26; break;
         case 0x12: cas_latency = 27; break;
         case 0x13: cas_latency = 28; break;
         case 0x14: cas_latency = 29; break;
         case 0x15: cas_latency = 30; break;
         case 0x16: cas_latency = 31; break;
         case 0x17: cas_latency = 32; break;
         default:   cas_latency = 32; break;
      }
      DEBUG_PRINTF("INFO: get_read_latency(): DDR4 MR0 = 0x%04X , Cas Latency = %d \n", glbl_mr0, cas_latency);
   #endif

   if(glbl_add_cmd_delay_en) {
      cas_latency += ((glbl_add_cmd_delay + 1) << 1);
      DEBUG_PRINTF("INFO: get_read_latency(): Add command delay enabled. add_cmd_delay = %d, Cas Latency = %d\n", glbl_add_cmd_delay, cas_latency);
   }

   return cas_latency;
}

void update_dly_mask (u32 *status, u32 *nibble_mask)
{
  u8 i;

  *nibble_mask = 0;

  for(i=0; i<DQS_BITS; i++)
  {
    if((*status & (1<<i)) == 0)
      *nibble_mask |= (1 << glbl_dqs_bits_riu_map[i][0]);
    else
      *nibble_mask = ~(~(*nibble_mask) | (1 << glbl_dqs_bits_riu_map[i][0]));
  }
}

void update_byte_dly_mask (u32 *status, u32 *nibble_mask)
{
  u8 i;

  *nibble_mask = 0;

  for(i=0; i<DQS_BITS; i++)
  {
    if((*status & (1<<i)) == 0)
      *nibble_mask |= (1 << glbl_dbi_bits_riu_map[i][0]);
    else
      *nibble_mask = ~(~(*nibble_mask) | (1 << glbl_dbi_bits_riu_map[i][0]));
  }
}

void update_nibble_dly_mask (u32 *status, u32 *nibble_mask)
{
  u8 i, j;
  u8 flag;
  static u8 map_riu[NIBBLE_PER_DQS * DQS_BITS];

  *nibble_mask = 0;

  for(i=0; i<DQS_BITS; i++)
  {
    flag = 0;

    if(NIBBLE_PER_DQS == 2)
    {
      map_riu[(2*i)] = glbl_dqs_bits_riu_map[i][0];

      for(j=0; j<5; j++)
      {
        if(!flag)
        {
          if(map_riu[(2*i)] != glbl_dq_bits_riu_map[(8*i) + j][0])
          {
            map_riu[(2*i) + 1] = glbl_dq_bits_riu_map[(8*i) + j][0];
            flag = 1;
          }
        }
      }
    }
    else
    {
      map_riu[i] = glbl_dqs_bits_riu_map[i][0];
      DEBUG_PRINTF("INFO: update_nibble_dly_mask(): x4 nibble map_riu[%d] = %d \n", i,map_riu[i]);
    }

    if((*status & (1<<i)) == 0)
    {
      if(NIBBLE_PER_DQS == 2)        
        *nibble_mask |= ((1 << map_riu[2*i]) | (1 << map_riu[(2*i) + 1]));
      else
        *nibble_mask |= (1 << map_riu[i]);
    }
    else
    {
      if(NIBBLE_PER_DQS == 2)
      {
        *nibble_mask = ~(~(*nibble_mask) | (1 << map_riu[(2*i)]));
        *nibble_mask = ~(~(*nibble_mask) | (1 << map_riu[(2*i) + 1]));
      }
      else
        *nibble_mask = ~(~(*nibble_mask) | (1 << map_riu[i]));
    }
  }
}

void ddr_initialize_seq ()
{
  writeIO_DMC(ral_addr_of_seq_cs_bits_in_ddrmc_main_ddrmc_main_bank(BLK),1);
  writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0); 
  writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0); 
  writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
  writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK),0);
  writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK),0);
  writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK),0);
  writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

  // PRECHARGE COMMAND
  writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0xa); // CS ACT RAS CAS WE = 0 1 0 1 0
  writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK),1);
  writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

  // ACTIVATE COMMAND
  writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);  // CS ACT RAS CAS WE = 0 0 0 0 0
  writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK),1);
  writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
  
}

void delay2riu_reg_addres_map(u8 bits_riu_map[][2], u8 bits, u16 *delay_map, u32 delay_offset) {
   int i;
   for(i=0; i<bits; i++) {
      delay_map[i] = (bits_riu_map[i][0] << RIU_NIB_POS) | ((bits_riu_map[i][1]+delay_offset) & RIU_REG_ADDR_MASK);
   }
   return ;
}
void dq_delay_map(u16 *delay_map, u32 delay_offset) {
   static u16 dq_riu_map[DQ_BITS][2] = DQ_RIU_MAP;
   int i;
   for(i=0; i<DQ_BITS; i++) {
      delay_map[i] = (dq_riu_map[i][0] << RIU_NIB_POS) | ((dq_riu_map[i][1]+delay_offset) & RIU_REG_ADDR_MASK);
   }
   return ;
}
void dqs_delay_map(u16 *delay_map, u32 delay_offset) {
   static u16 dqs_riu_map[DQS_BITS][2] = DQS_RIU_MAP;
   int i;
   for(i=0; i<DQS_BITS; i++) {
      delay_map[i] = (dqs_riu_map[i][0] << RIU_NIB_POS) | ((dqs_riu_map[i][1]+delay_offset) & RIU_REG_ADDR_MASK);
   }
   return ;
}
void dm_delay_map(u16 *delay_map, u32 delay_offset) {
   static u16 dm_riu_map[DBI_BITS][2] = DBI_RIU_MAP;
   int i;
   for(i=0; i<DBI_BITS; i++) {
      delay_map[i] = (dm_riu_map[i][0] << RIU_NIB_POS) | ((dm_riu_map[i][1]+delay_offset) & RIU_REG_ADDR_MASK);
   }
   return ;
}

void lp4_mode_reg_wr(u8 rank, u8 mode_reg, u8 reg_data) {
//                    cs  0     1     2     3     4     5                            
//            (MRW-1) H   L     H     H     L     L     OP7 

//                        6     7     8     9     10    11                               
//                    L   MA0   MA1   MA2   MA3   MA4   MA5

//                        12    13    14    15    16    17                               
//            (MRW-2) H   L     H     H     L     H     OP6

//                        18    19    20    21    22    23                               
//                    L   OP0   OP1   OP2   OP3   OP4   OP5

   u32 ca = LP4_MODE_REG_WRITE  | ((mode_reg&0x3F)<<(6-0)) | ((reg_data&0x80)>>(7-5)) | ((reg_data&0x40)<<(17-6)) | ((reg_data&0x3F)<<(18-0)); 

   u32 seq_a_cntrl = readIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK));
   seq_a_cntrl = (seq_a_cntrl & ~0xF0) | (0x10<<rank); // Mode register read from selected rank
   writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), seq_a_cntrl);
   writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK),rank); // Selects xphy rank

   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK),ca);
   u8 cs  = 0x5;
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),cs);
   writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK),1); //0: 2 commands per sequence; 1: 4 commands per sequence
   writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK),4);
   if(!glbl_wrlvl & !glbl_lp4_mode_default)
     writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0); // Sequence mode - 0: Normal mode; 1: Write level Mode; 2: Address calibration mode;
   writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //number of sequences - 0: Sequence A enabled; 1: Sequence A, and B enabled; 2: Sequence A, B, and C enabled;
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
}

void lp4_mode_reg_rd(u8 rank, u8 mode_reg)
{
  u32 ca = LP4_MODE_REG_READ | ((mode_reg&0x3F)<<(6-0)) | ((0x12) << 12);
  u32 cnt = (0x1 << 16) | 0x1; // Read count, command count

  u32 seq_a_cntrl = readIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK));
  seq_a_cntrl = (seq_a_cntrl & ~0xF0) | (0x10<<rank); // Mode register read from selected rank
  writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), seq_a_cntrl);
  writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK),rank); // Selects xphy rank

  writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK),ca);
  u16 seq_a_cmd  = (0x2 << 7) | 0x5; // It is read
  writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), seq_a_cmd);
  writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK),1); //0: 2 commands per sequence; 1: 4 commands per sequence
  writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK),4);
  if(!glbl_wrlvl & !glbl_lp4_mode_default)
     writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0); // Sequence mode - 0: Normal mode; 1: Write level Mode; 2: Address calibration mode;
  writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //number of sequences - 0: Sequence A enabled; 1: Sequence A, and B enabled; 2: Sequence A, B, and C enabled;
  writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK),cnt);
  writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
  while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
}

void lp4_mpc_wr(u8 rank, u32 opcode, u8 cas2, u8 wr_rd)
{
  // Operands in MPC with CAS-2 commands are anyways need to be driven LOW. Need not assign them.
  u32 ca = ((opcode & 0x40) >> 1) | ((opcode & 0x3F) << 6) | (cas2 ? (0x12 << 12) : 0x0);
  u32 cmd  = (cas2 ? 0x5 : 0x1) | (wr_rd << 7); // wr_rd: write (1), read (2), non-data (0)
  u32 cnt = (wr_rd == 2) ? ((0x1 << 16) | 0x1) : 0x1; // Read count, command count

  u32 seq_a_cntrl = readIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK));
  seq_a_cntrl = (seq_a_cntrl & ~0xF0) | (0x10<<rank); // selected rank
  writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), seq_a_cntrl);
  writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK),rank); // Selects xphy rank

  writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK),ca);
  writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), cmd);
  writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), cas2); //0: 2 commands per sequence; 1: 4 commands per sequence
  writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK),4);
  if(!glbl_wrlvl & !glbl_lp4_mode_default)
     writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0); // Sequence mode - 0: Normal mode; 1: Write level Mode; 2: Address calibration mode;
  writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //number of sequences - 0: Sequence A enabled; 1: Sequence A, and B enabled; 2: Sequence A, B, and C enabled;
  writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), cnt);
  writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
  while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

  return;
}

void lp4_bank_row_access(u8 rank, u8 cmd_type, u8 bank_addr, u16 row_addr) {
//                    cs  0     1     2     3     4     5                            
//Activate -1 (ACT-1) H - H   - L   - R12 - R13 - R14 - R15  

//                        6     7     8     9     10    11                               
//                    L - BA0 - BA1 - BA2 - V   - R10 - R11 

//                        12    13    14    15    16    17                               
//Activate -2 (ACT-2) H - H   - H   - R6  - R7  - R8  - R9  

//                        18    19    20    21    22    23                               
//                    L - R0  - R1  - R2  - R3  - R4  - R5  
//
//                    cs  0     1     2     3     4     5                            
//Precharge (PRE)     H - L   - L   - L   - L   - H   - AB 

//                        6     7     8     9     10    11                               
//                    L - BA0 - BA1 - BA2 - V   - V   - V 
//

   writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK),rank); // Selects xphy rank
   writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),0x10<<rank); // Selects DDR Mem rank

   writeIO_DMC(ral_addr_of_seq_b_rank_in_ddrmc_main_ddrmc_main_bank(BLK),rank); // Selects xphy rank
   writeIO_DMC(ral_addr_of_seq_b_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),0x10<<rank); // Selects DDR Mem rank

   u32 ca = ((bank_addr&0x7)<<(6-0)) | ((row_addr&0x0F000)>>(12-2)) |  ((row_addr&0xC00)<<(10-10)) | ((row_addr&0x3C0)<<(14-6)) | ((row_addr&0x3F)<<(18-0)); 
   if(cmd_type == ACTIVATE) {
      ca = LP4_ROW_ACTIVE | ca;
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0x5);
      writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK),1); //0: 2 commands per sequence; 1: 4 commands per sequence
   } else {
      ca = LP4_ROW_PREACHARGE | ca;
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0x1);
      writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK),0); //0: 2 commands per sequence; 1: 4 commands per sequence
   }
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK),ca);

   writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK),4);

   if(!glbl_wrlvl & !glbl_lp4_mode_default)
      writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0); // Sequence mode - 0: Normal mode; 1: Write level Mode; 2: Address calibration mode;
   writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //number of sequences - 0: Sequence A enabled; 1: Sequence A, and B enabled; 2: Sequence A, B, and C enabled;

   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
}

void lp4_bank_col_access(u32 nos_iteration_of_seqs, u8 cmd_a_type, u8 cmd_b_type, u8 bank_addr, u16 col_addr) {
//                    cs  0     1     2     3     4     5                            
//Mask Write -1       H - L   - L   - H   - H   - L   - L 
//                        6     7     8     9     10    11                               
//                    L - BA0 - BA1 - BA2 - V   - C9  - AP 
//                    cs  0     1     2     3     4     5                            
//Write -1            H - L   - L   - H   - L   - L   - BL 
//                        6     7     8     9     10    11                               
//                    L - BA0 - BA1 - BA2 - V   - C9  - AP 
//                    cs  0     1     2     3     4     5                            
//Read -1             H - L   - H   - L   - L   - L   - BL
//                        6     7     8     9     10    11                               
//                    L - BA0 - BA1 - BA2 - V   - C9  - AP
//                        12    13    14    15    16    17                               
//CAS-2               H - L   - H   - L   - L   - H   - C8 
//                        18    19    20    21    22    23                               
//                    L - C2  - C3  - C4  - C5  - C6  - C7 

   u32 cnts;
   u16 rd_cmd_cnts = 0;
   u16 total_cmd_cnts;
  
   u32 ca_a = ((bank_addr&0x7)<<6) |  ((col_addr&0x200)<<(10-9)) | ((col_addr&0x100)<<(17-8)) | ((col_addr&0xFC)<<(18-2)); 
   u32 ca_b = ((bank_addr&0x7)<<6) |  ((col_addr&0x200)<<(10-9)) | ((col_addr&0x100)<<(17-8)) | ((col_addr&0xFC)<<(18-2)); 

   if(cmd_a_type==COL_RD) {
      ca_a = ca_a | LP4_COL_READ;
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0x105);
   } else {
      ca_a = ca_a | LP4_COL_WRITE;
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0x085);
   }
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK),ca_a);

   if(cmd_b_type==COL_RD) {
      ca_b = ca_b | LP4_COL_READ;
      writeIO_DMC(ral_addr_of_seq_b_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0x145);
   } else {
      ca_b = ca_b | LP4_COL_WRITE;
      writeIO_DMC(ral_addr_of_seq_b_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0xA5);
   }

   writeIO_DMC(ral_addr_of_seq_b_addr_in_ddrmc_main_ddrmc_main_bank(BLK),ca_b);

   writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK),1); //0: 2 commands per sequence; 1: 4 commands per sequence
   writeIO_DMC(ral_addr_of_seq_b_ba_in_ddrmc_main_ddrmc_main_bank(BLK),1); //0: 2 commands per sequence; 1: 4 commands per sequence

   total_cmd_cnts = nos_iteration_of_seqs;
   if(cmd_a_type == COL_RD) {
      rd_cmd_cnts = total_cmd_cnts >> 1;
   } 
   if(cmd_b_type == COL_RD) {
      rd_cmd_cnts = rd_cmd_cnts + (total_cmd_cnts >> 1);
   }

   cnts = rd_cmd_cnts ;
   cnts = (cnts << 16) | (total_cmd_cnts & 0x0000FFFF); 

   writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 1); //number of sequences - 0: Sequence A enabled; 1: Sequence A, and B enabled; 2: Sequence A, B, and C enabled;

   writeIO_DMC(ral_addr_of_seq_a_b_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 0x40);
   writeIO_DMC(ral_addr_of_seq_b_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 0x40);

   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK),cnts);
   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0);
}

u8 banks_refresh_gt(u8 dqs_rank) {
   u8 rank;
   u8 status;

   if(glbl_refresh_en == 0)
      return 0;

   DEBUG_PRINTF("INFO: banks_refresh_gt(): Memory Refresh function called \n");
   for(rank = 0; rank < RANKS ; rank++) {
      writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK),rank); // Selects xphy rank
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),0x10<<rank); // Selects DDR Mem rank
      #ifdef LPDDR4
         ////Close/Precharge the opened Bank
         //lp4_bank_row_access(0 /*u8 rank*/, PRECHARGE /*u8 cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/); 
         ////                    cs  0     1     2     3     4     5                            
         ////Refresh  -1 (REF)   H - L   - L   - L   - H   - L   - AB   
         ////                        6     7     8     9     10    11                               
         ////                    L - BA0 - BA1 - BA2 - V   - V   - V   
         ////Relase the memory from preamble training mode
         DEBUG_PRINTF("INFO: banks_refresh_gt(): LPDDR4: Release the memory from Read preamble training mode \n");
         lp4_mode_reg_wr(rank, 13, glbl_lp4_mr13);
         
      #else
          // Release the memory from Preamble Training Mode
          DEBUG_PRINTF("INFO: banks_refresh_gt(): Release the memory from Preamble Training Mode \n");
          set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                                MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr4 & MEM_DISABLE_PMB_TRAIN /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x0 /*u16 seq_a_ba*/,
                                0 /*u32 seq_a2a_dly*/);
          // Release the memory from MPR mode
          DEBUG_PRINTF("INFO: banks_refresh_gt(): Release the memory from MPR Mode \n");
          glbl_mr3 = glbl_mr3 & MEM_DISABLE_MPR;
          set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                                MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr3 /*u32 seq_a_addr*/, 0x0 /*u8 seq_a_bg*/, 0x3 /*u16 seq_a_ba*/,
                                0 /*u32 seq_a2a_dly*/);
    
         //Close/Precharge the opened Bank
         DEBUG_PRINTF("INFO: banks_refresh_gt(): Closing opened row before starting the calibration\n");
         set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                          MEM_ROW_PREACHARGE /*u32 seq_a_cmd*/, 0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/,
                          0/*u32 seq_a2a_dly*/);
      #endif
   }

   // Send refresh commands
   status = banks_refresh(0, 0x1);
   if(status) {
      return 1;
   }

   #ifdef LPDDR4
   //Put the memory in preamble training mode
   DEBUG_PRINTF("INFO: banks_refresh_gt(): LPDDR4: Putting the memory in Read preamble training mode \n");
   lp4_mode_reg_wr(dqs_rank, 13, glbl_lp4_mr13|0x2);

   //Set MPC Read DQ CAL command
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0x105);
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), LP4_MPC_RD_DQ);
   #else
   // Before putting the memory in the read preamble training mode put the memory in MPR Mode
   DEBUG_PRINTF("INFO: banks_refresh_gt(): Putting the memory in read preamble training mode \n");
   glbl_mr3 = glbl_mr3 | MEM_ENABLE_MPR;
   set_1_seq_command_at_mem(dqs_rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                         MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr3 /*u32 seq_a_addr*/, 0x0 /*u8 seq_a_bg*/, 0x3 /*u16 seq_a_ba*/,
                         0 /*u32 seq_a2a_dly*/);

   // Put memory in the DQS Training Mode
   DEBUG_PRINTF("INFO: banks_refresh_gt(): Putting the memory in DQS training mode \n");
   set_1_seq_command_at_mem(dqs_rank /*u8 rank*/, 1 /*u32 nos_iteration_of_(seqs*/, 
                          MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr4 | MEM_ENABLE_PMB_TRAIN /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x0 /*u16 seq_a_ba*/,
                         0 /*u32 seq_a2a_dly*/);
   

   ////Set Column read command
   DEBUG_PRINTF("INFO: banks_refresh_gt(): setup column access command before starting the calibration\n");
   set_1_seq_command_at_mem(dqs_rank/*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                    MEM_COL_READ /*u32 seq_a_cmd*/, 0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/,
                    2/*u32 seq_a2a_dly*/);

   ////Set Column read command
   //writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),MEM_COL_READ);
   #endif
   return 0;
}

u8 banks_refresh(u8 dqs_rank, u8 ref_stage) {
   u8 loop;
   u8 rank;
   u8 lrank;
   u32 ref;
   u8 side_cnt;
   u16 t_rfc;
   u8 lcl_3ds_lranks;
   u32 num_ref_cur;
   u8 ref_limit = (glbl_ref_mode_4x ? 33 : (glbl_ref_mode_2x ? 17 : 9));
   u32 t_refi = T_REFI;

   if(glbl_refresh_en == 0)
      return 0;

   if(glbl_fix_ref_en == 1) {
      num_ref_cur = glbl_fix_num_ref;
   } else {
      glbl_ddrmc_clk_cnt = (readIO_DMC(ral_addr_of_ddrmc_clk_cnt_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & ~(0x1 << 31)); // Exclude busy bit

#ifdef DDR4
      if(glbl_ref_mode_4x)
         glbl_num_ref_total = ((DDRMC_CLK_CNT_LOAD - glbl_ddrmc_clk_cnt) * glbl_tck * 2 * 4) / t_refi;
      else if(glbl_ref_mode_2x)
         glbl_num_ref_total = ((DDRMC_CLK_CNT_LOAD - glbl_ddrmc_clk_cnt) * glbl_tck * 2 * 2) / t_refi;
      else // 1x
         glbl_num_ref_total = ((DDRMC_CLK_CNT_LOAD - glbl_ddrmc_clk_cnt) * glbl_tck * 2) / t_refi;
#else
      glbl_num_ref_total = ((DDRMC_CLK_CNT_LOAD - glbl_ddrmc_clk_cnt) * glbl_tck * 2) / t_refi;
#endif

      num_ref_cur = glbl_num_ref_total - glbl_num_ref_prev;
      glbl_num_ref_prev = glbl_num_ref_total;

      writeIO_DMC(ral_addr_of_ila_mux_d_0_0_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_ddrmc_clk_cnt);
   }

   glbl_num_ref_cur = (num_ref_cur << glbl_ref_speed); // Temperature based refresh speed: DDR4 - 1x/2x. LP4 - 1x/2x/4x

   writeIO_DMC(ral_addr_of_ila_mux_d_0_0_in_ddrmc_main_ddrmc_main_bank(BLK), ((glbl_ref_speed << 30) | (glbl_num_ref_cur << 16) | (glbl_num_ref_prev << 8) | glbl_num_ref_total));

   glbl_num_ref_iter++;
   writeIO_XRAM(XRAM_CAL_POST_STATUS + 0xC, glbl_num_ref_iter);
//   writeIO_MBDR(RAM_DATA_CAL_OFFSET + 0xF00 + glbl_num_ref_iter * 4, (glbl_num_ref_cur << 24) | ref_stage);

   if(glbl_num_ref_cur > ref_limit) { // pending refreshes
      glbl_num_ref_limit_cnt++;
      writeIO_XRAM(XRAM_CAL_POST_STATUS, glbl_num_ref_limit_cnt);
      // return 1;
   } else if(glbl_num_ref_cur == 0) { // Return, if no refreshes to be scheduled
      return 0;
   }

   if(glbl_num_ref_cur > glbl_num_ref_cur_max) { // maximum refreshes
      glbl_num_ref_cur_max = glbl_num_ref_cur;
      writeIO_XRAM(XRAM_CAL_POST_STATUS + 0x4, glbl_num_ref_cur_max);
      writeIO_XRAM(XRAM_CAL_POST_STATUS + 0x8, ref_stage);
   }

#ifdef DDR4
   // SIDE registers are used for refresh commands only
   if(glbl_side_ref == 0) { // Load SIDE registers with Refresh commands for once
      side_cnt = 0;
      for(rank=0; rank<RANKS; rank++) {
         lcl_3ds_lranks = (glbl_3ds_lranks == 8) ? 4 : glbl_3ds_lranks; // EDT-988720. Select high 4 logical ranks
         for(lrank=0; lrank<lcl_3ds_lranks; lrank++) {
            writeIO_DMC(ral_addr_of_seq_side_cntrl0_in_ddrmc_main_ddrmc_main_bank(BLK) + (side_cnt * 4), (0x10 << rank)); // [7:4] - CS
            writeIO_DMC(ral_addr_of_seq_side_addr0_in_ddrmc_main_ddrmc_main_bank(BLK) + (side_cnt * 4), (lrank << 28)); // [29:28] - 3DS C-bit

            if(glbl_3ds_en) { // 3DS configuration
               if(lrank == (lcl_3ds_lranks - 1)) { // Last logical rank to next physical rank
                  t_rfc = glbl_t_rfc_slr;
               } else { // Logical rank to Logical rank
                  t_rfc = glbl_t_rfc_dlr;
               }
            } else { // non-3DS configuration
               t_rfc = glbl_t_rfc_slr;
            }

            writeIO_DMC(ral_addr_of_seq_side_cmd0_in_ddrmc_main_ddrmc_main_bank(BLK) + (side_cnt * 4), ((t_rfc << 9) | 0x20 | MEM_BANK_REF)); // [31:9] - dly, [5] - CKE, [4:0] - CS/ACT/RAS/CAS/WE

            side_cnt++;
            DEBUG_PRINTF("INFO: banks_refresh(): Loading seq side registers with refresh commands. rank = %d, lrank = %d, side_cnt = %d\n", rank, lrank, side_cnt);
         }
      }
      writeIO_DMC(ral_addr_of_seq_side_cmd_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), side_cnt);
      glbl_side_ref = 1;
   }

   for(ref=0; ref < glbl_num_ref_cur; ref++) {
      glbl_side_cmd_en = 1;
      writeIO_DMC(ral_addr_of_seq_side_cmd_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

      loop = 0;
      while(readIO_DMC(ral_addr_of_seq_status_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x1) {
         DEBUG_PRINTF("INFO: banks_refresh(): While waiting for SIDE sequence commands to be served. ref = %d, loop = %d\n", ref, loop++);
      }
      glbl_side_cmd_en = 0;

      if(glbl_3ds_lranks == 8) {
         writeIO_DMC(ral_addr_of_seq_3ds_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1); // EDT-988720. Select high 4 logical ranks
         glbl_side_cmd_en = 1;
         writeIO_DMC(ral_addr_of_seq_side_cmd_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

         loop = 0;
         while(readIO_DMC(ral_addr_of_seq_status_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x1) {
            DEBUG_PRINTF("INFO: banks_refresh(): While waiting for SIDE sequence commands to be served. ref = %d, loop = %d\n", ref, loop++);
         }
         glbl_side_cmd_en = 0;

         writeIO_DMC(ral_addr_of_seq_3ds_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0); // Keep the default value
      }

      DEBUG_PRINTF("INFO: banks_refresh(): Completed refresh for all ranks. Refresh number = %d\n", ref);
   }

#else
   u32 ca = LP4_BANK_REF | (1<<5);

   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK),ca);  // Set the command
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0x1); // Select the chip select
   writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK),0); //0: 2 commands per sequence; 1: 4 commands per sequence
   if(!glbl_wrlvl & !glbl_lp4_mode_default)
      writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0); // Sequence mode - 0: Normal mode; 1: Write level Mode; 2: Address calibration mode;
   writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //number of sequences - 0: Sequence A enabled; 1: Sequence A, and B enabled; 2: Sequence A, B, and C enabled;
   writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_t_rfc_slr); //delay in controller clock from sequence B to sequence A assertion if two sequences are enabled 
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_num_ref_cur);

   for(rank = 0; rank < RANKS ; rank++) {
      writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK),rank); // Selects xphy rank
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),0x10<<rank); // Selects DDR Mem rank
      DEBUG_PRINTF("INFO: banks_refresh(): Refreshing the memory banks for Rank = %d\n", rank);

      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      while(readIO_DMC(ral_addr_of_seq_status_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x1);
   }

   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 4); //delay in controller clock from sequence B to sequence A assertion if two sequences are enabled 
   writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK),dqs_rank); // Selects xphy rank
   writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),0x10<<dqs_rank); // Selects DDR Mem rank
#endif
   DEBUG_PRINTF("INFO: banks_refresh(): Completed Refresh for all ranks\n");
   return 0;
}

u32 l2p_nibble_status (u32 log_nib_status) {
   u8 nibble;
   u8 phy_nibble;
   u32 phy_nib_status = 0;

   for(nibble=0; nibble < DQ_NIBBLES; nibble++) {
      phy_nibble = glbl_map_riu[nibble];
      phy_nib_status |= ((log_nib_status >> nibble) & 0x1) << phy_nibble;
   }

   return phy_nib_status;
}

void map_l2p(u8 bits_riu_map[][2], u8 bits, u8 *lnibble2pnibble_map, u8 *lbit2pnibble_map, u8 *first_nibble, u8 *last_nibble, u32 *nibble_mask) {
   u8 bit;
   u8 nibble;
   u8 j,k;
   u8 match;
   *nibble_mask=0;

   for(bit=0,j=0;bit<bits;bit++) {
      nibble = bits_riu_map[bit][0];
      if((*first_nibble >= nibble) | (bit == 0))
         *first_nibble = nibble;
      if(*last_nibble <= nibble)
         *last_nibble = nibble;
      lbit2pnibble_map[bit] = nibble; 
      *nibble_mask = *nibble_mask | (1<<nibble);
      match = 0;
      if(j==0) {
         lnibble2pnibble_map[j] = nibble;
         j=1;
      } else {
         for(k=0;k<j;k++) {
            if(lnibble2pnibble_map[k] == nibble) {
               match = 1; 
            }
         }
         if(match == 0) {
            lnibble2pnibble_map[j] = nibble;
            j++;
         }
      }
      DEBUG_PRINTF("INFO: map_l2p() lnibble2pnibble_map[%d] = %d, lbit2pnibble_map[%d] = %d  \n", bit, lnibble2pnibble_map[bit], bit, lbit2pnibble_map[bit]);
   }
   *nibble_mask = ~(*nibble_mask);
}

void single_write_read(u8 rank, u8 extend, u8 pattern)
{
  u32 extend_write_data;

  DEBUG_PRINTF("INFO: single_write_read(): start\n");
  writeIO_DMC(ral_addr_of_seq_cs_bits_in_ddrmc_main_ddrmc_main_bank(BLK),1<<rank); //Rank select           

  // Writing Pre Main and Post Data
  writeIO_DMC(ral_addr_of_seq_a_dq_early_in_ddrmc_main_ddrmc_main_bank(BLK), pattern);
  writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), pattern);

  #ifdef LPDDR4
    writeIO_DMC(ral_addr_of_seq_a_dq_1_in_ddrmc_main_ddrmc_main_bank(BLK), pattern);
  #endif

  writeIO_DMC(ral_addr_of_seq_a_dq_late_in_ddrmc_main_ddrmc_main_bank(BLK), pattern);

  // Extend Write Command 
  if(extend)
  {
    extend_write_data = readIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK));
    writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (extend_write_data | 0x00000009));
  }

  writeIO_DMC(ral_addr_of_seq_a_b_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 0x30);

  #ifdef LPDDR4
    lp4_bank_col_access(2, COL_WR, COL_RD, 0, 0);  
  #else
    set_2_seq_command_at_mem(rank /* rank */, 2 /* no of iteration */, 1 /* full write */, MEM_COL_WRITE /* seq_a_cmd */,
                            0 /* seq_a_addr */, 0 /* seq_a_bg */, 0 /* seq_ba */, MEM_COL_READ /* seq_b_cmd */, 
                            0 /*seq_b_addr*/, 0 /*seq_b_bg*/,0 /*seq_b_ba*/, 40 /*seq_a2b_dly*/, 40/*seq_b2a_dly*/);
  #endif

  if(extend)
    writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (extend_write_data & 0xFFFFFFF6));
}

void calculate_valid_win(u8 lines,  int win_type, u8 step_size, u8 edge_type) {
   u8 bit_cnt;
   u8 check_sequential = 0;
   if(edge_type == RISE) {
      if(win_type == FULL) {
         if(!( (glbl_bits_error_status_31_0 ) || (glbl_bits_error_status_63_32 ) || (glbl_bits_error_status_95_64 )   )   ) {
            //Here, assumption is the error bits are sticky and not getting cleared, in case of FULL window detection.
            return;
            //for(bit_cnt=0; bit_cnt < lines; bit_cnt++) {
               //glbl_common_valid_win = glbl_common_valid_win + step_size;
               //glbl_stable_valid_win[bit_cnt] = glbl_stable_valid_win[bit_cnt] + step_size;
            //}
         } else {
            check_sequential = 1;
         }
      } else {
         if(!( (glbl_bits_error_status_31_0 ) || (glbl_bits_error_status_63_32 ) || (glbl_bits_error_status_95_64 )   )   ) {
            for(bit_cnt=0; bit_cnt < lines; bit_cnt++) {
               if((glbl_stable_valid_win[bit_cnt])>glbl_min_vld_cnt) {
                  check_sequential = 1;
               }
            }
            if(check_sequential == 0) {
               for(bit_cnt=0; bit_cnt < lines; bit_cnt++) {
                  glbl_stable_valid_win[bit_cnt] = glbl_stable_valid_win[bit_cnt] + step_size;
               }
            }
         } else {
            check_sequential = 1;
         }
      }
      if(check_sequential == 1) {
         if(win_type == FULL) {
            glbl_bit_mask_31_0 = glbl_bit_mask_31_0 | glbl_bits_error_status_31_0; 
            glbl_bit_mask_63_32 = glbl_bit_mask_63_32 | glbl_bits_error_status_63_32; 
            glbl_bit_mask_95_64 = glbl_bit_mask_95_64 | glbl_bits_error_status_95_64; 
            //for(bit_cnt=0; bit_cnt < lines; bit_cnt++) {
            //   if(bit_cnt < 32) {
            //      if(!((glbl_bit_mask_31_0 >> bit_cnt) & 0x1)) {
            //         glbl_stable_valid_win[bit_cnt] = glbl_stable_valid_win[bit_cnt] + step_size;
            //      }
            //   } else if (bit_cnt >= 32 && bit_cnt < 64) {
            //      if(!((glbl_bit_mask_63_32 >> (bit_cnt-32)) & 0x1)) {
            //         glbl_stable_valid_win[bit_cnt] = glbl_stable_valid_win[bit_cnt] + step_size;
            //      }
            //   } else if (bit_cnt >= 64) {
            //      if(!((glbl_bit_mask_95_64 >> (bit_cnt-64)) & 0x1)) {
            //         glbl_stable_valid_win[bit_cnt] = glbl_stable_valid_win[bit_cnt] + step_size;
            //      }
            //   }
            //   DEBUG_PRINTF("INFO: calculate_valid_win(): glbl_stable_valid_win[%d] = %d \n", bit_cnt, glbl_stable_valid_win[bit_cnt]);
            //}
         } else {
            for(bit_cnt=0; bit_cnt < lines; bit_cnt++) {
               if((bit_cnt==0) || (bit_cnt==32) || (bit_cnt==64) ) {
                  glbl_bit_pos = 0x1;
               } else {
                  glbl_bit_pos = glbl_bit_pos<<1;
               }
               if(bit_cnt < 32) {
            //      if(!((glbl_bit_mask_31_0 >> bit_cnt) & 0x1)) {
                     if((glbl_bits_error_status_31_0 & glbl_bit_pos)) {
                        glbl_stable_valid_win[bit_cnt] = 0;
                     } else {
                        glbl_stable_valid_win[bit_cnt] = glbl_stable_valid_win[bit_cnt] + step_size;
                     }
                     if((glbl_stable_valid_win[bit_cnt]) >= glbl_min_vld_cnt) {
                        glbl_bit_mask_31_0 = glbl_bit_mask_31_0 | glbl_bit_pos;
                     }
             //     }
               } else if (bit_cnt < 64) {
             //     if(!((glbl_bit_mask_63_32 >> (bit_cnt-32)) & 0x1)) {
                     if((glbl_bits_error_status_63_32 & glbl_bit_pos)) {
                        glbl_stable_valid_win[bit_cnt] = 0;
                     } else {
                        glbl_stable_valid_win[bit_cnt] = glbl_stable_valid_win[bit_cnt] + step_size;
                     }
                     if((glbl_stable_valid_win[bit_cnt]) >= glbl_min_vld_cnt) {
                        glbl_bit_mask_63_32 = glbl_bit_mask_63_32 | glbl_bit_pos;
                     }
              //    }
               } else if (bit_cnt >= 64) {
               //   if(!((glbl_bit_mask_95_64 >> (bit_cnt-64)) & 0x1)) {
                     if((glbl_bits_error_status_95_64 & glbl_bit_pos)) {
                        glbl_stable_valid_win[bit_cnt] = 0;
                     } else {
                        glbl_stable_valid_win[bit_cnt] = glbl_stable_valid_win[bit_cnt] + step_size;
                     }
                     if((glbl_stable_valid_win[bit_cnt]) >= glbl_min_vld_cnt) {
                        glbl_bit_mask_95_64 = glbl_bit_mask_95_64 | glbl_bit_pos;
                     }
                //  }
               } 
               DEBUG_PRINTF("INFO: calculate_valid_win(): glbl_stable_valid_win[%d] = %d \n", bit_cnt, glbl_stable_valid_win[bit_cnt]);
            }
         }
      }
   } else {
      if(win_type == FULL) {
         if(!( (glbl_bits_error_status_nqtr_17_0 ) )) {
            return;
            //for(bit_cnt=0; bit_cnt < lines; bit_cnt++) {
            //   glbl_common_valid_win_nqtr = glbl_common_valid_win_nqtr + step_size;
               //glbl_stable_valid_win_nqtr[bit_cnt] = glbl_stable_valid_win_nqtr[bit_cnt] + step_size;
            //}
         } else {
            check_sequential = 1;
         }
      } else {
         if(!( (glbl_bits_error_status_nqtr_17_0 )   )) {
            for(bit_cnt=0; bit_cnt < lines; bit_cnt++) {
               if((glbl_stable_valid_win_nqtr[bit_cnt])>glbl_min_vld_cnt) {
                  check_sequential = 1;
               }
            }
            if(check_sequential == 0) {
               for(bit_cnt=0; bit_cnt < lines; bit_cnt++) {
                  glbl_stable_valid_win_nqtr[bit_cnt] = glbl_stable_valid_win_nqtr[bit_cnt] + step_size;
               }
            }
         } else {
            check_sequential = 1;
         }
      }
      if(check_sequential == 1) {
         if(win_type == FULL) {
            glbl_bit_mask_nqtr_17_0 = glbl_bit_mask_nqtr_17_0 | glbl_bits_error_status_nqtr_17_0; 
            //for(bit_cnt=0; bit_cnt < lines; bit_cnt++) {
            //   if(bit_cnt < 32) {
            //      if(!((glbl_bit_mask_nqtr_17_0 >> bit_cnt) & 0x1)) {
            //         glbl_stable_valid_win_nqtr[bit_cnt] = glbl_stable_valid_win_nqtr[bit_cnt] + step_size;
            //      }
            //   }
            //   DEBUG_PRINTF("INFO: calculate_valid_win(): glbl_stable_valid_win_nqtr[%d] = %d \n", bit_cnt, glbl_stable_valid_win_nqtr[bit_cnt]);
            //}
         } else {
            for(bit_cnt=0, glbl_bit_pos = 1; bit_cnt < lines; bit_cnt++, glbl_bit_pos = glbl_bit_pos << 1) {
               if(bit_cnt < 32) {
                  if(!((glbl_bit_mask_nqtr_17_0 & glbl_bit_pos))) {
                     if((glbl_bits_error_status_nqtr_17_0 & glbl_bit_pos)) {
                        glbl_stable_valid_win_nqtr[bit_cnt] = 0;
                     } else {
                        glbl_stable_valid_win_nqtr[bit_cnt] = glbl_stable_valid_win_nqtr[bit_cnt] + step_size;
                     }
                     if(glbl_stable_valid_win_nqtr[bit_cnt] >= glbl_min_vld_cnt) {
                        glbl_bit_mask_nqtr_17_0 = glbl_bit_mask_nqtr_17_0 | glbl_bit_pos;
                     }
                  }
               } 
               DEBUG_PRINTF("INFO: calculate_valid_win(): glbl_stable_valid_win_nqtr[%d] = %d \n", bit_cnt, glbl_stable_valid_win_nqtr[bit_cnt]);
            }
         }
      }
   }
}

void map_l2p_mask(u8 total_lines, u8 compare_edge_type) {
   u8   line;
   if(compare_edge_type == RISE) {
      if((glbl_iteration!=0) && (glbl_bit_mask_31_0 == glbl_bit_mask_31_0_last) && (glbl_bit_mask_63_32 == glbl_bit_mask_63_32_last) && (glbl_bit_mask_95_64 == glbl_bit_mask_95_64_last)) {
         glbl_update_nibble_mask = 0;
         return;
      }
      glbl_nibble_mask = 0xFFFFFFFF; 
      glbl_update_nibble_mask = 1;
      for(line = 0; line < total_lines; line++) {
         if((line==0) || (line==32) || (line==64) ) {
            glbl_bit_pos = 0x1;
         } else {
            glbl_bit_pos = glbl_bit_pos<<1;
         }
         if(line < 32) {
            if(!((glbl_bit_mask_31_0 & glbl_bit_pos))) {
               glbl_nibble_mask = glbl_nibble_mask & (~(0x1<<glbl_lbit2pnibble_map[line])); 
            }
         } else if(line < 64) {
            if(!((glbl_bit_mask_63_32 & glbl_bit_pos))) {
               glbl_nibble_mask = glbl_nibble_mask & (~(0x1<<glbl_lbit2pnibble_map[line])); 
            }
         } else if(line < 96) {
            if(!((glbl_bit_mask_95_64 & glbl_bit_pos))) {
               glbl_nibble_mask = glbl_nibble_mask & (~(0x1<<glbl_lbit2pnibble_map[line])); 
            }
         }
      }
      glbl_bit_mask_31_0_last  = glbl_bit_mask_31_0;
      glbl_bit_mask_63_32_last = glbl_bit_mask_63_32;
      glbl_bit_mask_95_64_last = glbl_bit_mask_95_64;
   } else {
      if((glbl_iteration!=0)&&(glbl_bit_mask_nqtr_17_0 == glbl_bit_mask_nqtr_17_0_last)) {
         glbl_update_nibble_mask_nqtr = 0;
         return;
      }
      glbl_nibble_mask_nqtr = 0xFFFFFFFF; 
      glbl_update_nibble_mask_nqtr = 1;
      for(line = 0, glbl_bit_pos=1; line < total_lines; line++,glbl_bit_pos=glbl_bit_pos<<1) {
         if(line < 32) {
            if(!((glbl_bit_mask_nqtr_17_0 & glbl_bit_pos))) {
               glbl_nibble_mask_nqtr = glbl_nibble_mask_nqtr & (~(0x1<<glbl_lbit2pnibble_map[line])); 
            }
          }
       }
       glbl_bit_mask_nqtr_17_0_last = glbl_bit_mask_nqtr_17_0;
   }
   return;
} 

void read_compared_status ( int compare_mode, int compare_edge,  u8 w_new_samples, u8 pattern_type) {
   //u8 bit_cnt ;
   //u8 nibble;
   u8 rank;
   //u32 error_bits = 0;
   //u32 error_nibbles = 0;
   u16 error_bytes   = 0;
   //u32 error_temp;
   u32 cplx_config;
   u32 cplx_status;

   // cplx_config register bits
   u32 start= 1; //0; // [0] Start flag
   u8  write_cal=0; // [1] "0: Read Cal 1: Write Cal" 
   u8  rank_en=0; // [5:2] A 1 in a given bit position means that given rank is enabled in the test pattern. Rank0 is enabled by default.
   u8  dm_en=0; // [6] "0: DM is not asserted for Write Pattern 1: DM pattern is enabled (only for write cal)" 

   // cplx_status register bits
   u8 busy;
   u8 error_status;
//   u8 done;
//   u8 error;

   if(w_new_samples == 1) {
      if(glbl_clear_error == 1) {
         if(pattern_type == SIMPLE || pattern_type == PRBS) {
            writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x1);
            writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
         }
      }
      if(pattern_type == COMPLEX) {
         if (glbl_margin_check) {
              rank_en |= (0x1 << glbl_margin_rank);
         } else {
           for(rank=0; rank < RANKS; rank += glbl_rank_inc) {
              rank_en |= (0x1 << rank);
           }
         }
         cplx_config = (start&0x1) | ((write_cal&0x1)<<1) | ((rank_en&0xF)<<2) | ((dm_en&0x1)<<6) | ((glbl_cplx_loop_cnt&0xFF)<<7) | ((glbl_cplx_row_cnt&0x7)<<15) | ((CPLX_BANK_CNT&0x3)<<18) | ((glbl_cplx_burst_array_index&0x1F)<<20) | ((CPLX_PATTERN_LENGTH&0x3F)<<25);
         cplx_status = readIO_DMC(ral_addr_of_cplx_status_in_ddrmc_main_ddrmc_main_bank(BLK));
         busy = cplx_status & 0x1;
         while(busy == 1) {
            error_status = read_status(compare_mode,compare_edge);
            if(error_status) {
               writeIO_DMC(ral_addr_of_cplx_status_in_ddrmc_main_ddrmc_main_bank(BLK),0x4); // Reset the Complex cal State Machine
               while(!(readIO_DMC(ral_addr_of_cplx_status_in_ddrmc_main_ddrmc_main_bank(BLK))&0x2)); // Wait for the Done
               writeIO_DMC(ral_addr_of_cplx_status_in_ddrmc_main_ddrmc_main_bank(BLK),0x0); // Release the reset of the Complex cal State Machine
               break;
            }
            cplx_status = readIO_DMC(ral_addr_of_cplx_status_in_ddrmc_main_ddrmc_main_bank(BLK));
            busy = cplx_status & 0x1;
         }
         if(glbl_cal_first_iteration == 0) {
            update_rd_path_delay();
            error_status = read_status(compare_mode,compare_edge);
            if(glbl_clear_error == 1) {
               writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x1);
               writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
            }
            cplx_config = ((cplx_config | (start&0x1)) & (~(0xF<<2)))  | ((rank_en&0xF)<<2);
            writeIO_DMC(ral_addr_of_cplx_config_in_ddrmc_main_ddrmc_main_bank(BLK), cplx_config);
            cplx_config = cplx_config & (~start);
            writeIO_DMC(ral_addr_of_cplx_config_in_ddrmc_main_ddrmc_main_bank(BLK), cplx_config);
            return;
         } else {
            writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x1);
            writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
            cplx_config = ((cplx_config | (start&0x1)) & (~(0xF<<2)))  | ((rank_en&0xF)<<2);
            writeIO_DMC(ral_addr_of_cplx_config_in_ddrmc_main_ddrmc_main_bank(BLK), cplx_config);
            cplx_config = cplx_config & (~start);
            writeIO_DMC(ral_addr_of_cplx_config_in_ddrmc_main_ddrmc_main_bank(BLK), cplx_config);
            busy = 1;
            //done = 0;
            //error = 0;
            while(busy == 1) {
               cplx_status = readIO_DMC(ral_addr_of_cplx_status_in_ddrmc_main_ddrmc_main_bank(BLK));
               //done = (cplx_status >> 1) & 0x1;
               busy = cplx_status & 0x1;
               error_status = read_status(compare_mode,compare_edge);
               if(error_status) {
                  writeIO_DMC(ral_addr_of_cplx_status_in_ddrmc_main_ddrmc_main_bank(BLK),0x4); // Reset the Complex cal State Machine
                  while(!(readIO_DMC(ral_addr_of_cplx_status_in_ddrmc_main_ddrmc_main_bank(BLK))&0x2)); // Wait for the Done
                  writeIO_DMC(ral_addr_of_cplx_status_in_ddrmc_main_ddrmc_main_bank(BLK),0x0); // Release the reset of the Complex cal State Machine
                  return;
               }
            }
            //error = (cplx_status >> 2) & 0x1;
            //riu_reg_bcast(BS_RESET_CTRL, 0, 0x1<<CLR_GATE_POS);
            //bitslice_reset_bcast(0, BS_RESET_WT);
            //cplx_config = cplx_config | start;
            //}
            //error_status = read_status(compare_mode,compare_edge);
            //return;
         }
      } else if(pattern_type == PRBS) {
         writeIO_DMC(ral_addr_of_prbs_start_in_ddrmc_main_ddrmc_main_bank(BLK),0x1);
         writeIO_DMC(ral_addr_of_prbs_start_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
         while(readIO_DMC(ral_addr_of_prbs_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & 0x1);
      } else {
         if (glbl_margin_check) {
            writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK),glbl_margin_rank); // Selects xphy rank
            writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),0x10<<glbl_margin_rank); // Selects DDR Mem rank
            //#ifdef DDR4
            //   writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK),mpr);
            //#endif
            //riu_reg_bcast(BS_RESET_CTRL, 0, 0x1<<CLR_GATE_POS);
            //bitslice_reset_bcast(0, BS_RESET_WT);
            writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
            while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK))) {
               error_status = read_status(compare_mode,compare_edge);
               if(error_status) {
                  writeIO_DMC(ral_addr_of_seq_clr_in_ddrmc_main_ddrmc_main_bank(BLK),1); // Selects xphy rank
                  writeIO_DMC(ral_addr_of_seq_clr_in_ddrmc_main_ddrmc_main_bank(BLK),1); // Selects xphy rank
                  writeIO_DMC(ral_addr_of_seq_clr_in_ddrmc_main_ddrmc_main_bank(BLK),0); // Selects xphy rank
                  return;
               }
            }
         } else {
            for (rank = 0; rank < RANKS; rank += glbl_rank_inc) {
               writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK),rank); // Selects xphy rank
               writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),0x10<<rank); // Selects DDR Mem rank
               //#ifdef DDR4
               //   writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK),mpr);
               //#endif
               //riu_reg_bcast(BS_RESET_CTRL, 0, 0x1<<CLR_GATE_POS);
               //bitslice_reset_bcast(0, BS_RESET_WT);
               writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
               while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK))) {
                  error_status = read_status(compare_mode,compare_edge);
                  if(error_status) {
                     writeIO_DMC(ral_addr_of_seq_clr_in_ddrmc_main_ddrmc_main_bank(BLK),1); // Selects xphy rank
                     writeIO_DMC(ral_addr_of_seq_clr_in_ddrmc_main_ddrmc_main_bank(BLK),1); // Selects xphy rank
                     writeIO_DMC(ral_addr_of_seq_clr_in_ddrmc_main_ddrmc_main_bank(BLK),0); // Selects xphy rank
                     writeIO_XRAM(XRAM_CAL_RANK,(1<<rank | 1 << 4));
                     return;
                  }
               }
            }
         }
         //error_status = read_status(compare_mode,compare_edge);
         //return;
      }
   }
   error_status = read_status(compare_mode,compare_edge);
   return;
}

u8 read_status( int compare_mode, int compare_edge) {
   if(compare_mode == BYTES) {
      glbl_bits_error_status_31_0 = readIO_DMC(ral_addr_of_compare_byte_err_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_byte_compare_8_0;
      if(~(glbl_bits_error_status_31_0|glbl_bit_mask_31_0|(~glbl_byte_compare_8_0))){
         return 0;
      } else {
         return 1;
      }
   } else if(compare_mode == NIBBLES) {
      glbl_bits_error_status_31_0 = 0;
      glbl_bits_error_status_nqtr_17_0 = 0;
      if(compare_edge == RISE) {
         glbl_bits_error_status_31_0 = readIO_DMC(ral_addr_of_compare_rise_err_per_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_nibble_compare_17_0;
      //} else if (compare_edge == FALL) {
         glbl_bits_error_status_nqtr_17_0 = readIO_DMC(ral_addr_of_compare_fall_err_per_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_nibble_compare_17_0;
      } else {
         glbl_bits_error_status_31_0 = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_nibble_compare_17_0;
      }
      if(~(glbl_bits_error_status_31_0 | glbl_bits_error_status_nqtr_17_0 | (glbl_bit_mask_31_0 & glbl_bit_mask_nqtr_17_0) | (~glbl_nibble_compare_17_0))) {
         return 0;
      } else {
         return 1;
      }
   } else {
      if(compare_edge == RISE) {
         glbl_bits_error_status_31_0  = readIO_DMC(ral_addr_of_compare_rise_err_per_bit0_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_bit_compare_31_0;
         glbl_bits_error_status_63_32 = readIO_DMC(ral_addr_of_compare_rise_err_per_bit1_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_bit_compare_63_32;
         glbl_bits_error_status_95_64 = readIO_DMC(ral_addr_of_compare_rise_err_per_bit2_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_bit_compare_95_64;
      } else if(compare_edge == FALL) {
         glbl_bits_error_status_31_0  = readIO_DMC(ral_addr_of_compare_fall_err_per_bit0_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_bit_compare_31_0;
         glbl_bits_error_status_63_32 = readIO_DMC(ral_addr_of_compare_fall_err_per_bit1_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_bit_compare_63_32;
         glbl_bits_error_status_95_64 = readIO_DMC(ral_addr_of_compare_fall_err_per_bit2_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_bit_compare_95_64;
      } else {
         glbl_bits_error_status_31_0  = readIO_DMC(ral_addr_of_compare_err_per_bit0_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_bit_compare_31_0;
         glbl_bits_error_status_63_32 = readIO_DMC(ral_addr_of_compare_err_per_bit1_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_bit_compare_63_32;
         glbl_bits_error_status_95_64 = readIO_DMC(ral_addr_of_compare_err_per_bit2_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_bit_compare_95_64;
      }
      if(~((glbl_bits_error_status_31_0 | glbl_bit_mask_31_0 | (~glbl_bit_compare_31_0)) & (glbl_bits_error_status_63_32 | glbl_bit_mask_63_32 | (~glbl_bit_compare_63_32)) & (glbl_bits_error_status_95_64 | glbl_bit_mask_95_64 | (~glbl_bit_compare_95_64)))) {
         return 0;
      } else {
         return 1;
      }
   }
}
void read_ca_compared_status ( int compare_mode, u8 w_new_samples) {
#ifdef LPDDR4
   u16 error_bytes   = 0;
   u32 error_bits    = 0;
   u32 lb_error_bits_31_0    = 0;
   u32 lb_error_bits_63_32   = 0;
   u32 pb_error_bits_31_0    = 0;
   u32 pb_error_bits_63_32   = 0;
   u8  wait          = 0;
   u8  bit           = 0;

   if(w_new_samples == 1 || glbl_clear_error) {
      writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x1);
      writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);
   }

   if(glbl_exptd_pt == 7) {
      writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK),7); // 0-10101010 3-00000000 4-01010101  7-11111111
      writeIO_DMC(ral_addr_of_seq_cs_cal_in_ddrmc_main_ddrmc_main_bank(BLK),0x0180);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
//
//      writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK),3); // 0-10101010 3-00000000 4-01010101  7-11111111
//      writeIO_DMC(ral_addr_of_seq_cs_cal_in_ddrmc_main_ddrmc_main_bank(BLK),0x0060);
//      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
//      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
      //writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK),3); // 0-10101010 3-00000000 4-01010101  7-11111111
      //writeIO_DMC(ral_addr_of_seq_cs_cal_in_ddrmc_main_ddrmc_main_bank(BLK),0x0060);
      //writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      //while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
   } else {
      writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK),3); // 0-10101010 3-00000000 4-01010101  7-11111111
      writeIO_DMC(ral_addr_of_seq_cs_cal_in_ddrmc_main_ddrmc_main_bank(BLK),0x0180);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
//
//      writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK),7); // 0-10101010 3-00000000 4-01010101  7-11111111
//      writeIO_DMC(ral_addr_of_seq_cs_cal_in_ddrmc_main_ddrmc_main_bank(BLK),0x0060);
//      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
//      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
      //writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK),7); // 0-10101010 3-00000000 4-01010101  7-11111111
      //writeIO_DMC(ral_addr_of_seq_cs_cal_in_ddrmc_main_ddrmc_main_bank(BLK),0x0060);
      //writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      //while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
   }

      lb_error_bits_31_0 = readIO_DMC(ral_addr_of_compare_err_per_bit0_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_bit_compare_31_0;
      DEBUG_PRINTF("INFO: read_ca_compared_status(): read bits[31:0] compared status 0x%08X \n", lb_error_bits_31_0);
      lb_error_bits_63_32 = readIO_DMC(ral_addr_of_compare_err_per_bit1_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_bit_compare_63_32;
      DEBUG_PRINTF("INFO: read_ca_compared_status(): read bits[63:32] compared status 0x%08X \n", lb_error_bits_63_32);

      // Arrange logical bit status to physical bit status order
      for(bit=0; bit < DQ_BITS; bit++) {
         if(bit<32) {
            if(glbl_dq_lbit2pbit_map[bit]<32) {
               if((lb_error_bits_31_0>>bit) & 0x1) {
                  pb_error_bits_31_0 = pb_error_bits_31_0 | (1<<glbl_dq_lbit2pbit_map[bit]);
               }
            } else {
               if((lb_error_bits_31_0>>bit) & 0x1) {
                  pb_error_bits_63_32 = pb_error_bits_63_32 | (1<<(glbl_dq_lbit2pbit_map[bit]-32));
               }
            }
         } else {
            if(glbl_dq_lbit2pbit_map[bit]<32) {
               if((lb_error_bits_63_32>>(bit-32)) & 0x1) {
                  pb_error_bits_31_0 = pb_error_bits_31_0 | (1<<glbl_dq_lbit2pbit_map[bit]);
               }
            } else {
               if((lb_error_bits_63_32>>(bit-32)) & 0x1) {
                  pb_error_bits_63_32 = pb_error_bits_63_32 | (1<<(glbl_dq_lbit2pbit_map[bit]-32));
               }
            }
         }
      }

      error_bits = (((pb_error_bits_31_0>>8)&0x3F)|((pb_error_bits_31_0>>18)&0xFC0));
      glbl_bits_error_status_31_0 = error_bits;
      error_bits = (((pb_error_bits_63_32>>8)&0x3F)|((pb_error_bits_63_32>>18)&0xFC0));
      glbl_bits_error_status_31_0 = glbl_bits_error_status_31_0 | (error_bits<<12);

      // For the CA group wise comparision set the error bits for all the ca bits if any of the ca bits has error.
      if(glbl_vref_cal==1 ) { //|| glbl_region_type==PARTIAL ) {
         if(glbl_bits_error_status_31_0 & 0x3F) {
            glbl_bits_error_status_31_0 = glbl_bits_error_status_31_0 | 0x3F;
         } 
         if(glbl_bits_error_status_31_0 & (0x3F<<6)) {
            glbl_bits_error_status_31_0 = glbl_bits_error_status_31_0 | (0x3F<<6);
         } 
         if(glbl_bits_error_status_31_0 & (0x3F<<12)) {
            glbl_bits_error_status_31_0 = glbl_bits_error_status_31_0 | (0x3F<<12);
         } 
         if(glbl_bits_error_status_31_0 & (0x3F<<18)) {
            glbl_bits_error_status_31_0 = glbl_bits_error_status_31_0 | (0x3F<<18);
         } 
      }
      if(PINOUT_SELECT==23) {
         glbl_bits_error_status_31_0 = ((glbl_bits_error_status_31_0>>12)&0xFC0) | ((glbl_bits_error_status_31_0>>6)&0xFC0) | ((glbl_bits_error_status_31_0>>6)&0x3F) |(glbl_bits_error_status_31_0&0x3F) ;
      }
      if(compare_mode == BYTES) {
         u8 byte_cnts=0;
         u8 bytes_status=0;
         for(byte_cnts=0;byte_cnts<CA_BITS/6;byte_cnts++) {
            if(((glbl_bits_error_status_31_0>>(6*byte_cnts))&0x3F) != 0)
               bytes_status = bytes_status | (1<<byte_cnts);
         }
         glbl_bits_error_status_31_0 = bytes_status;
         DEBUG_PRINTF("INFO: read_ca_compared_status(): read bytes[8:0] compared status 0x%08X \n", glbl_bits_error_status_31_0);
      }
#endif
}

u8 region_detect() {
   u32 mask_revert = 0;
   u32 mask_revert_nqtr = 0;
   // Initialize loggic nibble mask and stable valid window to 0
   glbl_nibble_mask      = glbl_nibble_mask_initial;
   DEBUG_PRINTF("INFO: regrion_detect(): Calibration Stage = %s : updated the nibble mask for pqtr nibbles = 0x%08X \n", glbl_stage_code_string[glbl_cal_code], glbl_nibble_mask);
   glbl_nibble_mask_nqtr = 0xFFFFFFFF; 
   if(glbl_dual_edge_region) {
      glbl_nibble_mask_nqtr = glbl_nibble_mask_initial;
   }
   glbl_bit_mask_nqtr_17_0 = 0;

   glbl_bits_error_status_nqtr_17_0 = 0;
   glbl_bits_error_status_31_0 = 0;
   glbl_bits_error_status_63_32 = 0;
   glbl_bits_error_status_95_64 = 0;

   glbl_bit_mask_31_0=0;
   glbl_bit_mask_63_32=0;
   glbl_bit_mask_95_64=0;

   glbl_iteration = 0;
 
   u8 status=0;
   
   u8 nibble;
   u32 mask; 
   int step = glbl_inc_dec_load*glbl_step_size;
   static u16 pqtr_delay[27];

   /************************************************************************************************************************/
   // Setup the Initial mask for all the bits/nibbles/bytes which has to be delayed during the region detection
   /************************************************************************************************************************/
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask); 
      //if(((glbl_delay_type == DQ_DELAY) || (glbl_delay_type == DBI_DELAY) || (glbl_delay_type == CA_DELAY)))
      if(glbl_delay_type == DQ_DELAY || glbl_delay_type == CA_DELAY) {
         if(glbl_delay_type == DQ_DELAY) {
            initialize_u16_array(glbl_pnibble2pbs_bcast_map,27,0xF0C3);
            riu_reg_bcast(IODLY_BCAST_MASK,0,0x3); 
         } else {
            initialize_u16_array(glbl_pnibble2pbs_bcast_map,27,0xFFFF);
            set_nibble2bcast_bits( 1, glbl_compare_data_bits);
         }
      } else if (glbl_delay_type == DQ_DBI_DELAY) { // Used for compelx data calibraiton, where dq and dbi moves together.
         // Unmask all the bits of all nibbles for broadcasting the delay inc/dec to the DQ and DBI bits.
         riu_reg_bcast(IODLY_BCAST_MASK,0,0x0); 
         //for(nibble = 0; nibble < 27; nibble++) {
         //   write_riu_nibble_reg(nibble /*u8 nibble*/, IODLY_BCAST_MASK /*u8 reg_addr*/, 0);
         //}
      }
//      riu_reg_bcast(CRSE_DLY,0,glbl_step_size); 
      riu_reg_bcast(INCDEC_CRSE,0,glbl_step_size); 
      if(glbl_compare_data_type == NIBBLES) {
         glbl_compare_data_bits = DQ_NIBBLES;
      }  
   /************************************************************************************************************************/

   /************************************************************************************************************************/
   // Setup the internal delay counter to track the delay counts for asserting error when they reach to limits
   /************************************************************************************************************************/
      u16 delay_max = 0;
      u16 delay_init_val;
      u8 bit_cnt;
      if(glbl_delay_type == P_DELAY || glbl_delay_type == N_DELAY || glbl_delay_type == P_N_DELAY) {
         if(glbl_delay_type == P_DELAY || glbl_delay_type == P_N_DELAY) {
            for(nibble = 0; nibble<DQ_NIBBLES; nibble++) {
               delay_init_val = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble],PQTR);
               DEBUG_PRINTF("INFO: region_detect(): delay_init_val[nibble][%d] = %d \n", nibble, delay_init_val);
               if(delay_init_val>delay_max) {
                  delay_max=delay_init_val;
               }
            } 
         }
         if(glbl_delay_type == N_DELAY || glbl_delay_type == P_N_DELAY) {
            for(nibble = 0; nibble<DQ_NIBBLES; nibble++) {
               delay_init_val = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[nibble],NQTR);
               DEBUG_PRINTF("INFO: region_detect(): delay_init_val[nibble][%d] = %d \n", nibble, delay_init_val);
               if(delay_init_val>delay_max) {
                  delay_max=delay_init_val;
               }
            }
         }
         DEBUG_PRINTF("INFO: region_detect(): For the QTR delay the max start delay at start of the phase is = %d \n", delay_max);
      } else if(glbl_delay_type == P_N_DQ_DELAY) {
         for(bit_cnt = 0; bit_cnt < DQ_BITS; bit_cnt++) { 
            delay_init_val = readIO_RIU(glbl_dq_idly_riu_addr_map[bit_cnt])&0x1FF;
            DEBUG_PRINTF("INFO: region_detect(): delay_init_val[%d] = %d \n", bit_cnt, delay_init_val);
            if(delay_init_val>delay_max) {
               delay_max=delay_init_val;
            }
         }
         for(nibble = 0; nibble<DBI_BITS; nibble++) {
            delay_init_val = read_riu_nibble_reg(glbl_dbi_lbit2pnibble_map[nibble],PQTR);
            DEBUG_PRINTF("INFO: region_detect(): delay_init_val[nibble][%d] = %d \n", nibble, delay_init_val);
            if(delay_init_val>delay_max) {
               delay_max=delay_init_val;
            }
            delay_init_val = read_riu_nibble_reg(glbl_dbi_lbit2pnibble_map[nibble],NQTR);
            DEBUG_PRINTF("INFO: region_detect(): delay_init_val[nibble][%d] = %d \n", nibble, delay_init_val);
            if(delay_init_val>delay_max) {
               delay_max=delay_init_val;
            }
         }
         DEBUG_PRINTF("INFO: region_detect(): For the DQ or QTR  delay the max start delay at start of the phase is = %d \n", delay_max);
      } else if(glbl_delay_type == DQ_DELAY) {
         for(bit_cnt = 0; bit_cnt < DQ_BITS; bit_cnt++) { 
            delay_init_val = readIO_RIU(glbl_dq_idly_riu_addr_map[bit_cnt])&0x1FF;
            DEBUG_PRINTF("INFO: region_detect(): delay_init_val[%d] = %d \n", bit_cnt, delay_init_val);
            if(delay_init_val>delay_max) {
               delay_max=delay_init_val;
            }
         }
         DEBUG_PRINTF("INFO: region_detect(): For the DQ delay the max start delay at start of the phase is = %d \n", delay_max);
      } else if(glbl_delay_type == DBI_DELAY) {
         for(bit_cnt = 0; bit_cnt < DBI_BITS; bit_cnt++) { 
            delay_init_val = readIO_RIU(glbl_dbi_idly_riu_addr_map[bit_cnt])&0x1FF;
            DEBUG_PRINTF("INFO: region_detect(): delay_init_val[%d] = %d \n", bit_cnt, delay_init_val);
            if(delay_init_val>delay_max) {
               delay_max=delay_init_val;
            }
         }
         DEBUG_PRINTF("INFO: region_detect(): For the DBI delay the max start delay at start of the phase is = %d \n", delay_max);
      } 
      #ifdef LPDDR4
         if(glbl_delay_type == CA_DELAY && glbl_compare_data_bits == CA_BITS) {
            for(bit_cnt = 0; bit_cnt < CA_BITS; bit_cnt++) { 
               delay_init_val = readIO_RIU(glbl_ca_odly_riu_addr_map[bit_cnt])&0x1FF;
               DEBUG_PRINTF("INFO: region_detect(): delay_init_val[%d] = %d \n", bit_cnt, delay_init_val);
               if(delay_init_val>delay_max) {
                  delay_max=delay_init_val;
               }
            }
            DEBUG_PRINTF("INFO: region_detect(): For the CA delay the max start delay at start of the phase is = %d \n", delay_max);
         } else if(glbl_delay_type == CA_DELAY && glbl_compare_data_bits == CS_BITS) {
            for(bit_cnt = 0; bit_cnt < CS_BITS; bit_cnt++) { 
               delay_init_val = readIO_RIU(glbl_odly_riu_addr_map[bit_cnt])&0x1FF;
               DEBUG_PRINTF("INFO: region_detect(): delay_init_val[%d] = %d \n", bit_cnt, delay_init_val);
               if(delay_init_val>delay_max) {
                  delay_max=delay_init_val;
               }
            }
            DEBUG_PRINTF("INFO: region_detect(): For the CS delay the max start delay at start of the phase is = %d \n", delay_max);
         }
      #endif
   /************************************************************************************************************************/

   /************************************************************************************************************************/
   // Start the region detect phase
   /************************************************************************************************************************/
      DEBUG_PRINTF("INFO: region_detect(): ################################ Calibration Stage = %s : Phase No. = %d, Started with bits = %d, initial_glbl_nibble_mask = 0x%08X ################ \n", glbl_stage_code_string[glbl_cal_code], glbl_phase_no, glbl_compare_data_bits, glbl_nibble_mask);
      do {
         DEBUG_PRINTF("INFO: region_detect(): ################################ Calibration Stage = %s : Phase No. = %02X Iterartion No. = %d ################ \n", glbl_stage_code_string[glbl_cal_code], glbl_phase_no, glbl_iteration);
         if(glbl_iteration == 0) {
            glbl_cal_first_iteration = 1;
         } else {
            glbl_cal_first_iteration = 0;
         }


         /************************************************************************************************************************/
         // Read comparision status of the bits/nibbles/bytes whose region is getting detected
         /************************************************************************************************************************/
            if(glbl_delay_type == CA_DELAY) {
               read_ca_compared_status (glbl_compare_data_type /*int compare_mode*/, 1 /*u8 w_new_samples*/); 
            } else {
               read_compared_status (glbl_compare_data_type /*compare_mode*/,glbl_compare_edge_type /*compare_edge*/,  1, glbl_pattern_type); 
            }
            DEBUG_PRINTF("INFO: region_detect(): Calibration Stage = %s : glbl_bits_error_status_95_64 = 0x%08X, glbl_bits_error_status_63_32 = 0x%08X, glbl_bits_error_status_31_0 = 0x%08X \n", glbl_stage_code_string[glbl_cal_code], glbl_bits_error_status_95_64, glbl_bits_error_status_63_32, glbl_bits_error_status_31_0);
         /************************************************************************************************************************/

         /************************************************************************************************************************/
         // Calculate the valid window of the bits/nibbles/bytes whose region is getting detected
         /************************************************************************************************************************/
            calculate_valid_win(glbl_compare_data_bits, glbl_region_type, glbl_step_size, RISE); 
            DEBUG_PRINTF("INFO: region_detect(): Calibration Stage = %s : glbl_bit_mask_95_64 = 0x%08X, glbl_bit_mask_63_32 = 0x%08X, glbl_bit_mask_31_0 = 0x%08X \n", glbl_stage_code_string[glbl_cal_code], glbl_bit_mask_95_64, glbl_bit_mask_63_32, glbl_bit_mask_31_0);
            DEBUG_PRINTF("INFO: region_detect(): Calibration Stage = %s : glbl_bit_mask_95_64_last = 0x%08X, glbl_bit_mask_63_32_last = 0x%08X, glbl_bit_mask_31_0_last = 0x%08X \n", glbl_stage_code_string[glbl_cal_code], glbl_bit_mask_95_64_last, glbl_bit_mask_63_32_last, glbl_bit_mask_31_0_last);
         /************************************************************************************************************************/

         /************************************************************************************************************************/
         // Calculate and setup physical nibble wise mask whose region is being detected
         /************************************************************************************************************************/
            map_l2p_mask(glbl_compare_data_bits, RISE); 
            DEBUG_PRINTF("INFO: region_detect(): Calibration Stage = %s : glbl_nibble_mask = 0x%08X \n", glbl_stage_code_string[glbl_cal_code], glbl_nibble_mask);
            //writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask); 
            DEBUG_PRINTF("INFO: region_detect(): Calibration Stage = %s : updated the nibble mask for pqtr nibbles = 0x%08X \n", glbl_stage_code_string[glbl_cal_code], glbl_nibble_mask);
         /************************************************************************************************************************/

         /************************************************************************************************************************/
         // If the comparison is bit wise then calculate and setup physical bit wise mask whose region is being detected
         /************************************************************************************************************************/
            if(glbl_update_nibble_mask) {
               if(glbl_compare_data_type == BITS || glbl_delay_type == CA_DELAY) {
                  if(glbl_delay_type == DQ_DELAY || glbl_delay_type == CA_DELAY) {
                     set_nibble2bcast_bits(0, glbl_compare_data_bits);
                     DEBUG_PRINTF("INFO: region_detect(): Calibration Stage = %s : updated the bits mask \n", glbl_stage_code_string[glbl_cal_code]);
                  }
               }
            }
         /************************************************************************************************************************/



         /************************************************************************************************************************/
         // If the second edge comparision is also required in case of nibble wise delay increment and the nibble wise data comparison
         /************************************************************************************************************************/
            if(glbl_dual_edge_region) {
                  DEBUG_PRINTF("INFO: region_detect(): ****************************************** NQTR status ******************************************\n", glbl_stage_code_string[glbl_cal_code], glbl_bits_error_status_nqtr_17_0);
               /************************************************************************************************************************/
               // Read comparision status of the bits/nibbles/bytes whose region is getting detected
               /************************************************************************************************************************/
                  //read_compared_status ( NIBBLES /*compare_mode*/, FALL /*compare_edge*/,  0, glbl_pattern_type); 
                  DEBUG_PRINTF("INFO: region_detect(): Calibration Stage = %s : glbl_bits_error_status_nqtr_17_0 = 0x%08X \n", glbl_stage_code_string[glbl_cal_code], glbl_bits_error_status_nqtr_17_0);
               /************************************************************************************************************************/

               /************************************************************************************************************************/
               // Calculate the valid window of the bits/nibbles/bytes whose region is getting detected
               /************************************************************************************************************************/
                  calculate_valid_win(glbl_compare_data_bits, glbl_region_type, glbl_step_size, FALL); 
                  DEBUG_PRINTF("INFO: region_detect(): Calibration Stage = %s : glbl_bit_mask_nqtr_17_0 = 0x%08X \n", glbl_stage_code_string[glbl_cal_code], glbl_bit_mask_nqtr_17_0);
                  DEBUG_PRINTF("INFO: region_detect(): Calibration Stage = %s : glbl_bit_mask_nqtr_17_0_last = 0x%08X \n", glbl_stage_code_string[glbl_cal_code], glbl_bit_mask_nqtr_17_0_last);
               /************************************************************************************************************************/

               /************************************************************************************************************************/
               // Calculate and setup physical nibble wise mask whose region is being detected
               /************************************************************************************************************************/
                  map_l2p_mask(glbl_compare_data_bits, FALL); 
                  DEBUG_PRINTF("INFO: region_detect(): Calibration Stage = %s : glbl_nibble_mask_nqtr = 0x%08X \n", glbl_stage_code_string[glbl_cal_code], glbl_nibble_mask_nqtr);
             //     if(glbl_update_nibble_mask || glbl_update_nibble_mask_nqtr) {   
                     //writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask_nqtr); 
                     DEBUG_PRINTF("INFO: region_detect(): Calibration Stage = %s : updated the nibble mask for nqtr nibbles = 0x%08X \n", glbl_stage_code_string[glbl_cal_code], glbl_nibble_mask_nqtr);
              //    }
               /************************************************************************************************************************/

            }
         /************************************************************************************************************************/

         /************************************************************************************************************************/
         // Updatd the delays
         /************************************************************************************************************************/
            if((glbl_pattern_type == SIMPLE) || (glbl_pattern_type == PRBS)) {
               update_rd_path_delay();
            }
         /************************************************************************************************************************/

         /************************************************************************************************************************/
         // Check whether the iteration has crossed the limit
         /************************************************************************************************************************/
            DEBUG_PRINTF("INFO: region_detect(): glbl_nibble_mask = 0x%08X \n", glbl_nibble_mask);
            DEBUG_PRINTF("INFO: region_detect(): glbl_nibble_mask_nqtr = 0x%08X \n", glbl_nibble_mask_nqtr);
            if(glbl_iteration>500) {
               DEBUG_PRINTF("ERROR: region_detect(): Delay taps got saturated \n");
               glbl_iteration_limit=500;
               write_error_nibble_XRAM(1, (~glbl_nibble_mask) | (~glbl_nibble_mask_nqtr));
               return 1;
            }
            glbl_iteration=glbl_iteration+glbl_step_size;
            mask  = glbl_nibble_mask & glbl_nibble_mask_nqtr;
            if(glbl_cal_first_iteration == 1) {
               mask_revert = glbl_nibble_mask ^ glbl_nibble_mask_initial;
               mask_revert_nqtr = glbl_nibble_mask_nqtr ^ glbl_nibble_mask_initial;
            }
            //if(glbl_delay_type == P_DELAY || glbl_delay_type == N_DELAY || glbl_delay_type == P_N_DQ_DELAY) {
            //   for(nibble = 0, glbl_bit_pos=1; nibble < 27 ; nibble++, glbl_bit_pos=glbl_bit_pos<<1) {
            //      if((mask & glbl_bit_pos) == 0) {
            //         pqtr_delay[nibble] = pqtr_delay[nibble] + step;  
            //         if(pqtr_delay[nibble] > 500) {
            //            DEBUG_PRINTF("WARNING: region_detect(): Delay taps got saturated for pqtr_delay[%d] = %d \n", nibble, pqtr_delay[nibble]);
            //            return 1;
            //         }
            //      }
            //   }
            //}
            delay_max = delay_max + glbl_step_size*glbl_inc_dec_load;
            if((delay_max>500) && (glbl_inc_dec_load == INC)) {
               DEBUG_PRINTF("WARNING: region_detect(): Delay taps got saturated, delay_max = %d \n", delay_max);
               break;
            }
            if((delay_max>600) && (glbl_inc_dec_load == DEC)) {
               DEBUG_PRINTF("WARNING: region_detect(): Delay taps underflow, delay_max = %d \n", delay_max);
               break;
            }
            if(glbl_iteration >  glbl_iteration_limit) {
               DEBUG_PRINTF("WARNING: region_detect(): Iteration limit %d has reached \n", glbl_iteration);
               break;
            }
         /************************************************************************************************************************/
         
 
            DEBUG_PRINTF("INFO: region_detect(): glbl_nibble_mask = 0x%08X \n", glbl_nibble_mask);
            DEBUG_PRINTF("INFO: region_detect(): glbl_nibble_mask_nqtr = 0x%08X \n", glbl_nibble_mask_nqtr);
      } while (glbl_nibble_mask != 0xFFFFFFFF || glbl_nibble_mask_nqtr != 0xFFFFFFFF);
   /************************************************************************************************************************/
   u8 busy;
   u32 cplx_status;
   if(glbl_pattern_type == COMPLEX) {
         do {
            cplx_status = readIO_DMC(ral_addr_of_cplx_status_in_ddrmc_main_ddrmc_main_bank(BLK));
            busy = cplx_status & 0x1;
         } while (busy==1);
      /************************************************************************************************************************/
      // Setup the Initial mask for all the bits/nibbles/bytes which has to be delayed during the region detection
      /************************************************************************************************************************/
         //if(glbl_cal_first_iteration == 0) {
            glbl_nibble_mask = glbl_nibble_mask_initial | mask_revert;
            writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask); 
            if(glbl_delay_type == DQ_DELAY) {
               riu_reg_bcast(IODLY_BCAST_MASK,0,0x3); 
            } else if (glbl_delay_type == DQ_DBI_DELAY) { // Used for compelx data calibraiton, where dq and dbi moves together.
               riu_reg_bcast(IODLY_BCAST_MASK,0,0x0); 
            }
//            riu_reg_bcast(CRSE_DLY,0,glbl_step_size); 
            riu_reg_bcast(INCDEC_CRSE,0,glbl_step_size); 
            if(glbl_inc_dec_load == INC) {
               glbl_inc_dec_load = DEC;
            } else {
               glbl_inc_dec_load = INC;
            }
            status = update_delay_sequential( glbl_delay_type /*int glbl_delay_type*/, glbl_inc_dec_load /*int glbl_inc_dec_load*/); 
            if(glbl_dual_edge_region) {
               glbl_nibble_mask = glbl_nibble_mask_initial | mask_revert_nqtr;
               writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask); 
               status = update_delay_sequential( N_DELAY/*int glbl_delay_type*/, glbl_inc_dec_load /*int glbl_inc_dec_load*/); 
            }
            writeIO_RIU(RPI_BCAST_NIBBLE_MASK0,0xFFFFFFFF); 
         //}
   }
   if(delay_max>500) {
      glbl_iteration_limit=500;
      write_error_nibble_XRAM(1, (~glbl_nibble_mask) | (~glbl_nibble_mask_nqtr));
      return 1;
   }
            
   glbl_iteration_limit=500;
   return status;
}

void update_rd_path_delay() {
   u8 status;
   writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask); 
   if(glbl_delay_type == CA_DELAY) {
      status = update_ca_delay(0);
   } else {
      status = update_delay_sequential( glbl_delay_type /*int glbl_delay_type*/, glbl_inc_dec_load /*int glbl_inc_dec_load*/); 
   }
   DEBUG_PRINTF("INFO: update_rd_path_delay(): glbl_nibble_mask = 0x%08X \n", glbl_nibble_mask);
   DEBUG_PRINTF("INFO: update_rd_path_delay(): glbl_nibble_mask_nqtr = 0x%08X \n", glbl_nibble_mask_nqtr);
   if(glbl_dual_edge_region) {
      /************************************************************************************************************************/
      // Updatd the delays
      /************************************************************************************************************************/
         writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_nibble_mask_nqtr); 
         status = update_delay_sequential( N_DELAY /*int glbl_delay_type*/, glbl_inc_dec_load /*int glbl_inc_dec_load*/); 
      /************************************************************************************************************************/
   }
}

void set_nibble2bcast_bits(u8 set_initial_mask, u8 bits) {
   u8 bit;
   u16 temp;
   u8 phy_nibble_idx;
   u8 phy_bit_idx;

   // Based on the logical bit mask sets the physical bit mask for both IDELAY and ODELAY
   for(bit = 0; bit < bits; bit++) {
      if(set_initial_mask) {
         if(bit < 32) {
            //if(~((glbl_bit_mask_31_0>>bit) & 0x1)) {
               // 0-Physical Nibble index; 1-Bit Index in the physical nibble
               phy_nibble_idx = glbl_bits_riu_map[bit][0];
               phy_bit_idx    = glbl_bits_riu_map[bit][1];
               glbl_pnibble2pbs_bcast_map[phy_nibble_idx] = glbl_pnibble2pbs_bcast_map[phy_nibble_idx] & (~(0x41 << phy_bit_idx));
               write_riu_nibble_reg(phy_nibble_idx/*u8 nibble*/, IODLY_BCAST_MASK /*u8 reg_addr*/, glbl_pnibble2pbs_bcast_map[phy_nibble_idx]);
            //}
         } else if (bit < 64) {
            //if(~((glbl_bit_mask_63_32>>bit) & 0x1)) {
               // 0-Physical Nibble index; 1-Bit Index in the physical nibble
               phy_nibble_idx = glbl_bits_riu_map[bit][0];
               phy_bit_idx    = glbl_bits_riu_map[bit][1];
               glbl_pnibble2pbs_bcast_map[phy_nibble_idx] = glbl_pnibble2pbs_bcast_map[phy_nibble_idx] & (~(0x41 << phy_bit_idx));
               write_riu_nibble_reg(phy_nibble_idx/*u8 nibble*/, IODLY_BCAST_MASK /*u8 reg_addr*/, glbl_pnibble2pbs_bcast_map[phy_nibble_idx]);
            //}
         } else {
            //if(~((glbl_bit_mask_95_64>>bit) & 0x1)) {
               // 0-Physical Nibble index; 1-Bit Index in the physical nibble
               phy_nibble_idx = glbl_bits_riu_map[bit][0];
               phy_bit_idx    = glbl_bits_riu_map[bit][1];
               glbl_pnibble2pbs_bcast_map[phy_nibble_idx] = glbl_pnibble2pbs_bcast_map[phy_nibble_idx] & (~(0x41 << phy_bit_idx));
               write_riu_nibble_reg(phy_nibble_idx/*u8 nibble*/, IODLY_BCAST_MASK /*u8 reg_addr*/, glbl_pnibble2pbs_bcast_map[phy_nibble_idx]);
            //}
         }
      } else {
         if((bit==0) || (bit==32) || (bit==64) ) {
            glbl_bit_pos = 0x1;
         } else {
            glbl_bit_pos = glbl_bit_pos<<1;
         }
         if(bit < 32) {
            if(glbl_bit_mask_31_0 & glbl_bit_pos) {
               // 0-Physical Nibble index; 1-Bit Index in the physical nibble
               phy_nibble_idx = glbl_bits_riu_map[bit][0];
               phy_bit_idx    = glbl_bits_riu_map[bit][1];
               temp = glbl_pnibble2pbs_bcast_map[phy_nibble_idx];
               glbl_pnibble2pbs_bcast_map[phy_nibble_idx] = glbl_pnibble2pbs_bcast_map[phy_nibble_idx] | (0x41 << phy_bit_idx);
               if(temp != glbl_pnibble2pbs_bcast_map[phy_nibble_idx]) {
                  write_riu_nibble_reg(phy_nibble_idx/*u8 nibble*/, IODLY_BCAST_MASK /*u8 reg_addr*/, glbl_pnibble2pbs_bcast_map[phy_nibble_idx]);
               }
            }
         } else if (bit < 64) {
            if(glbl_bit_mask_63_32 & glbl_bit_pos) {
               // 0-Physical Nibble index; 1-Bit Index in the physical nibble
               phy_nibble_idx = glbl_bits_riu_map[bit][0];
               phy_bit_idx    = glbl_bits_riu_map[bit][1];
               temp = glbl_pnibble2pbs_bcast_map[phy_nibble_idx];
               glbl_pnibble2pbs_bcast_map[phy_nibble_idx] = glbl_pnibble2pbs_bcast_map[phy_nibble_idx] | (0x41 << phy_bit_idx);
               if(temp != glbl_pnibble2pbs_bcast_map[phy_nibble_idx]) {
                  write_riu_nibble_reg(phy_nibble_idx/*u8 nibble*/, IODLY_BCAST_MASK /*u8 reg_addr*/, glbl_pnibble2pbs_bcast_map[phy_nibble_idx]);
               }
            }
         } else {
            if(glbl_bit_mask_95_64 & glbl_bit_pos) {
               // 0-Physical Nibble index; 1-Bit Index in the physical nibble
               phy_nibble_idx = glbl_bits_riu_map[bit][0];
               phy_bit_idx    = glbl_bits_riu_map[bit][1];
               temp = glbl_pnibble2pbs_bcast_map[phy_nibble_idx];
               glbl_pnibble2pbs_bcast_map[phy_nibble_idx] = glbl_pnibble2pbs_bcast_map[phy_nibble_idx] | (0x41 << phy_bit_idx);
               if(temp != glbl_pnibble2pbs_bcast_map[phy_nibble_idx]) {
                  write_riu_nibble_reg(phy_nibble_idx/*u8 nibble*/, IODLY_BCAST_MASK /*u8 reg_addr*/, glbl_pnibble2pbs_bcast_map[phy_nibble_idx]);
               }
            }
         }
      }
   }
   DEBUG_PRINTF("INFO: set_nibble2bcast_bits() glbl_bit_mask_31_0 = 0x%08X \n", glbl_bit_mask_31_0);
   DEBUG_PRINTF("INFO: set_nibble2bcast_bits() glbl_bit_mask_63_32= 0x%08X \n", glbl_bit_mask_63_32);
   DEBUG_PRINTF("INFO: set_nibble2bcast_bits() glbl_bit_mask_95_64= 0x%08X \n", glbl_bit_mask_95_64);
   for(bit = 0; bit < 27; bit++) {
      DEBUG_PRINTF("INFO: set_nibble2bcast_bits() glbl_pnibble2pbs_bcast_map[%d] = 0x%08X \n", bit, glbl_pnibble2pbs_bcast_map[bit]);
   }
}

u8 update_delay_sequential( int delay_type, int inc_dec_load) { 
   u8  nibble;
   u8  bit_cnt;
   int steps;                                   
   
   if(delay_type == DQ_DELAY || delay_type == DQ_DBI_DELAY || delay_type == P_N_DQ_DELAY || delay_type == DBI_DELAY || delay_type == CA_DELAY) { //IDELAY update
      if(inc_dec_load == LOAD) { // Load absolute value in the delay register
         for(bit_cnt = 0; bit_cnt < DQ_BITS; bit_cnt++) {
            if((bit_cnt==0) || (bit_cnt==32) || (bit_cnt==64) ) {
               glbl_bit_pos = 0x1;
            } else {
               glbl_bit_pos = glbl_bit_pos<<1;
            }
            if( (bit_cnt < 32 && (!(glbl_bit_mask_31_0 & glbl_bit_pos))) || (bit_cnt >= 32 && bit_cnt < 64 && (!(glbl_bit_mask_63_32 & glbl_bit_pos))) || (bit_cnt >= 64 && bit_cnt < 95 && (!(glbl_bit_mask_95_64 & glbl_bit_pos))) ) {
               DEBUG_PRINTF("INFO: update_delay_sequential(): Sequential loading for Data bit = %d, Delay Taps = %d \n", bit_cnt, glbl_step_size);
               //write_riu_nibble_reg(nibble /*u8 nibble*/, IDLY0+glbl_dq_bits_riu_map[bit_cnt][1] /*u8 reg_addr*/, glbl_step_size);
               writeIO_RIU( glbl_dq_idly_riu_addr_map[bit_cnt], glbl_step_size);
            }
         }   
      } else if (inc_dec_load == INC) { // Increment the delay
         if(glbl_en_qtr_dly_upd_wbs) {
            DEBUG_PRINTF("INFO: update_delay_sequential(): Broadcast increment for IDELAY Taps of the data bits of the data nibbles \n");
            riu_reg_bcast(IODLY_BCAST_CTRL,0,(0x1<<IDLY_CRSE_EN_POS)|(0x1<<IDLY_BCAST_EN_POS)|(0x1<<IDLY_INC_POS)); 
         } else {
            for(steps=0; steps<glbl_step_size; steps++) {
               riu_reg_bcast(IODLY_BCAST_CTRL,0,(0x1<<IDLY_BCAST_EN_POS)|(0x1<<IDLY_INC_POS)); 
            }
         }
      } else { // Decrement the delay
         if(glbl_en_qtr_dly_upd_wbs) {
            DEBUG_PRINTF("INFO: update_delay_sequential(): Broadcast decrement for IDELAY Taps of the data bits of the data nibbles \n");
            riu_reg_bcast(IODLY_BCAST_CTRL,0,(0x1<<IDLY_CRSE_EN_POS)|(0x1<<IDLY_BCAST_EN_POS)); 
         } else {
            for(steps=0; steps<glbl_step_size; steps++) {
               riu_reg_bcast(IODLY_BCAST_CTRL,0,(0x1<<IDLY_BCAST_EN_POS)); 
            }
         }
      }
   }

   if (delay_type == P_DELAY || delay_type == P_N_DELAY || delay_type == P_N_DQ_DELAY) { //PQTR update
      if(inc_dec_load == LOAD) {
         for(nibble = glbl_first_nibble, glbl_bit_pos = 1<<glbl_first_nibble; nibble <= glbl_last_nibble ; nibble++, glbl_bit_pos = glbl_bit_pos<<1) {
            if(!((glbl_nibble_mask & glbl_bit_pos))) {
               DEBUG_PRINTF("INFO: update_delay_sequential(): Sequential loading for Physical Nibble = %d, PQTR Taps = %d \n", nibble, glbl_step_size);
               write_riu_nibble_reg(nibble /*u8 nibble*/, PQTR /*u8 reg_addr*/, glbl_step_size);
            }
         }
      } else if(inc_dec_load == INC) {
         if(glbl_en_qtr_dly_upd_wbs) {
            DEBUG_PRINTF("INFO: update_delay_sequential(): Broadcast increment for PQTR Taps of the data nibbles \n");
            riu_reg_bcast(PQTR,0,(0x1<<QTR_INC_DLY_POS)|(0x1<<QTR_CRSE_EN_POS)); 
         } else {
            for(steps=0; steps<glbl_step_size; steps++) {
               riu_reg_bcast(PQTR,0,(0x1<<QTR_INC_DLY_POS)); 
            }
         }
         
      } else {
         if(glbl_en_qtr_dly_upd_wbs) {
            DEBUG_PRINTF("INFO: update_delay_sequential(): Broadcast decrement for PQTR Taps of the data nibbles \n");
            riu_reg_bcast(PQTR,0,(0x1<<QTR_DEC_DLY_POS)|(0x1<<QTR_CRSE_EN_POS)); 
         } else {
            for(steps=0; steps<glbl_step_size; steps++) {
               riu_reg_bcast(PQTR,0,(0x1<<QTR_DEC_DLY_POS)); 
            }
         }
      }
   }
   if(delay_type == N_DELAY || delay_type == P_N_DELAY || delay_type == P_N_DQ_DELAY) {//NQTR update
      if(inc_dec_load == LOAD) {
         for(nibble = glbl_first_nibble, glbl_bit_pos = 1<<glbl_first_nibble; nibble <= glbl_last_nibble ; nibble++, glbl_bit_pos = glbl_bit_pos<<1) {
            if(!((glbl_nibble_mask & glbl_bit_pos))) {
               DEBUG_PRINTF("INFO: update_delay_sequential(): Sequential loading for Physical Nibble = %d, NQTR Taps = %d \n", nibble, glbl_step_size);
               write_riu_nibble_reg(nibble /*u8 nibble*/, NQTR /*u8 reg_addr*/, glbl_step_size);
            }
         }
      } else if(inc_dec_load == INC) {
         if(glbl_en_qtr_dly_upd_wbs) {
            DEBUG_PRINTF("INFO: update_delay_sequential(): Broadcast increment for NQTR Taps of the data nibbles \n");
            riu_reg_bcast(NQTR,0,(0x1<<QTR_INC_DLY_POS)|(0x1<<QTR_CRSE_EN_POS)); 
         } else {
            for(steps=0; steps<glbl_step_size; steps++) {
               riu_reg_bcast(NQTR,0,(0x1<<QTR_INC_DLY_POS)); 
            }
         }
      } else {
         if(glbl_en_qtr_dly_upd_wbs) {
            DEBUG_PRINTF("INFO: update_delay_sequential(): Broadcast decrement for NQTR Taps of the data nibbles \n");
            riu_reg_bcast(NQTR,0,(0x1<<QTR_DEC_DLY_POS)|(0x1<<QTR_CRSE_EN_POS)); 
         } else {
            for(steps=0; steps<glbl_step_size; steps++) {
               riu_reg_bcast(NQTR,0,(0x1<<QTR_DEC_DLY_POS)); 
            }
         }
      }
   }
//   //Reset Bit slice
   if(glbl_en_qtr_dly_upd_wbs) {
      bitslice_reset_bcast(0, BS_RESET_WT);
   }

   return 0;
}


// Mapping DQS on the Even numbered map_riu incase of x8
void get_riu_map (u32 *nibble_reset_mask, u8 *map_riu)
{
  u8 i, j;
  u8 flag;

  for(i=0; i<DQS_BITS; i++)
  {
    flag = 0;

    if(NIBBLE_PER_DQS == 2)
    {
      map_riu[(2*i)] = glbl_dqs_bits_riu_map[i][0];

      for(j=0; j<5; j++)
      {
        if(!flag)
        {
          if(map_riu[(2*i)] != glbl_dq_bits_riu_map[(8*i) + j][0])
          {
            map_riu[(2*i) + 1] = glbl_dq_bits_riu_map[(8*i) + j][0];
            flag = 1;
          }
        }
      }  

      DEBUG_PRINTF("INFO: get_riu_map(): x8 lower nibble map_riu[%d] = %d \n", 2*i,map_riu[2*i]);
      DEBUG_PRINTF("INFO: get_riu_map(): x8 upper nibble map_riu[%d] = %d \n", (2*i)+1, map_riu[(2*i) + 1]);

      nibble_reset_mask[0] |= (1 << map_riu[(2*i)]);
      nibble_reset_mask[1] |= (1 << map_riu[(2*i) + 1]);

    }
    else
    {
      map_riu[i] = glbl_dqs_bits_riu_map[i][0];
      DEBUG_PRINTF("INFO: get_riu_map(): x4 nibble map_riu[%d] = %d \n", i,map_riu[i]);

      *nibble_reset_mask |= (1 << map_riu[i]);
    } 
  }
}

u8 update_ca_delay(u8 delay_val) {
#ifdef LPDDR4
   u8 ca_bit;
   u8 ca_group;
   u8 bit_slip_en;
   u16 tx_90_slip_en;
   u16 tx_90_slip_en_read;
   u8 coarse_taps;
   u32 write_nibbles_mask;
   u8 steps;
   DEBUG_PRINTF("INFO: update_ca_delay(): Updateing delay with the Mask = 0x%08X \n", glbl_bit_mask_31_0);
   if(glbl_delay_stage == COARSE) {
      write_nibbles_mask = readIO_DMC(ral_addr_of_xpi_lp4_write_nib_mask_in_ddrmc_main_ddrmc_main_bank(BLK));
      writeIO_DMC(ral_addr_of_xpi_lp4_write_nib_mask_in_ddrmc_main_ddrmc_main_bank(BLK),0xFFFFFFFF);
     
      DEBUG_PRINTF("******************INFO: update_ca_delay(): Updating Coarse delays ********************** \n", ca_bit);
      for(ca_bit=0,glbl_bit_pos=1; ca_bit<CA_BITS; ca_bit++,glbl_bit_pos=glbl_bit_pos<<1) {
         DEBUG_PRINTF("******************INFO: update_ca_delay(): for bit = %d ********************** \n", ca_bit);
         if( !(glbl_bit_mask_31_0 & glbl_bit_pos) ) {
            tx_90_slip_en_read  = read_riu_nibble_reg(glbl_bits_riu_map[ca_bit][0], CFG_NIBBLE_CTRL1);
            if(glbl_inc_dec_load != LOAD) {
               DEBUG_PRINTF("INFO: update_ca_delay(): Read tx 90 cfg_nibble_ctrl1 = 0x%04X, of physical_nibble = %d and the nibble bit = %d \n", tx_90_slip_en_read, glbl_bits_riu_map[ca_bit][0], glbl_bits_riu_map[ca_bit][1]);
               tx_90_slip_en       = (tx_90_slip_en_read>>(2+glbl_bits_riu_map[ca_bit][1])) & 0x1;
               coarse_taps = !tx_90_slip_en; 
            } else {
               coarse_taps = delay_val;
            }
            DEBUG_PRINTF("INFO: update_ca_delay(): new coarse delay 0x%01X \n", coarse_taps);
            tx_90_slip_en = coarse_taps      & 0x1;
            if(tx_90_slip_en) {
               tx_90_slip_en = tx_90_slip_en_read | (0x4<<glbl_bits_riu_map[ca_bit][1]);
            } else {
               tx_90_slip_en = tx_90_slip_en_read & (~(0x4<<glbl_bits_riu_map[ca_bit][1]));
            }
            write_riu_nibble_reg(glbl_lbit2pnibble_map[ca_bit], CFG_BS_RST_MASK, ~(0x1<<glbl_bits_riu_map[ca_bit][1]));
            write_riu_nibble_reg(glbl_lbit2pnibble_map[ca_bit], BS_RESET_CTRL, 0x1<<BS_RESET_POS);
            write_riu_nibble_reg(glbl_lbit2pnibble_map[ca_bit], BS_RESET_CTRL, 0x1<<BS_RESET_POS);
            write_riu_nibble_reg(glbl_lbit2pnibble_map[ca_bit], BS_RESET_CTRL, 0x1<<BS_RESET_POS);
            DEBUG_PRINTF("INFO: update_ca_delay(): loading tx phase 90 = 0x%04X \n", tx_90_slip_en);
            write_riu_nibble_reg(glbl_lbit2pnibble_map[ca_bit], CFG_NIBBLE_CTRL1, tx_90_slip_en);
            write_riu_nibble_reg(glbl_lbit2pnibble_map[ca_bit], BS_RESET_CTRL, 0x1<<BS_RESET_POS);
            write_riu_nibble_reg(glbl_lbit2pnibble_map[ca_bit], BS_RESET_CTRL, 0x1<<BS_RESET_POS);
            write_riu_nibble_reg(glbl_lbit2pnibble_map[ca_bit], BS_RESET_CTRL, 0x0);
         }
      } 
      writeIO_DMC(ral_addr_of_xpi_lp4_write_nib_mask_in_ddrmc_main_ddrmc_main_bank(BLK),write_nibbles_mask);
   } else {
      DEBUG_PRINTF("******************INFO: update_ca_delay(): Updating Fine delays ********************** \n", ca_bit);
      if(glbl_inc_dec_load == LOAD) {
         for(ca_bit=0,glbl_bit_pos=1; ca_bit<CA_BITS; ca_bit++,glbl_bit_pos=glbl_bit_pos<<1) {
            DEBUG_PRINTF("******************INFO: update_ca_delay(): for bit = %d ********************** \n", ca_bit);
            if( !(glbl_bit_mask_31_0 & glbl_bit_pos) ) {
               DEBUG_PRINTF("INFO: update_ca_delay(): loading fine delay = %d \n", delay_val );
               writeIO_RIU(glbl_odly_riu_addr_map[ca_bit], delay_val);
            }
         }
      } else if(glbl_inc_dec_load == INC) {
         DEBUG_PRINTF("INFO: update_delay_sequential(): Broadcast increment for ODELAY Taps of the CA bits \n");
         for(steps=0; steps<glbl_step_size; steps++) {
            riu_reg_bcast(IODLY_BCAST_CTRL,0,(0x1<<ODLY_BCAST_EN_POS)|(0x1<<ODLY_INC_POS)); 
         }
      } else {
         DEBUG_PRINTF("INFO: update_delay_sequential(): Broadcast decrement for ODELAY Taps of the CA bits \n");
         for(steps=0; steps<glbl_step_size; steps++) {
            riu_reg_bcast(IODLY_BCAST_CTRL,0,(0x1<<ODLY_BCAST_EN_POS)); 
         }
      }
   }
         //odelay_read = readIO_RIU(glbl_ca_odly_riu_addr_map[ca_bit]);
         //odelay_write = odelay_read + glbl_step_size*glbl_inc_dec_load;
         //if(odelay_write > 511) {
         //   DEBUG_PRINTF("INFO: update_ca_delay(): fine delay overflow/underflow delay = %d \n", odelay_write);
         //   return 1;
         //} else {
         //   DEBUG_PRINTF("INFO: update_ca_delay(): INC/DEC loading fine delay = %d \n", odelay_write );
         //   writeIO_RIU(glbl_ca_odly_riu_addr_map[ca_bit], odelay_write);
         //}
#endif
   return 0;
}


void dummy_write_read(){

#ifdef DDR4
   // Set default values
   writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK),0xAA); // Setup Address         
   writeIO_DMC(ral_addr_of_seq_cs_bits_in_ddrmc_main_ddrmc_main_bank(BLK),0x1); // Rank select           
   writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0x00);  // Normal mode
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK),1);

   // ACTIVATE command
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0x00);
   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0);

   // WRITE command
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0xC);  // CS ACT RAS CAS WE = 0 1 1 0 0
   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0);

   // READ command
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0xD);  // CS ACT RAS CAS WE = 0 1 1 0 1
   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0);

   // PRECHARGE command
   writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),0xA);  // CS ACT RAS CAS WE = 0 1 0 1 0
   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0);
#endif

   return;
}

void dummy_write_multi_rank_switch(u8 rank_phy) {

// Dummy write used to set correct odelays before disabling and after enabling dynamic odelay mode
   writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK), rank_phy); // Selects xphy rank
   writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0); // Selects DDR Mem rank
   writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 4);
   writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0);

   #ifdef DDR4
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_WRITE);
   #else
      writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), LP4_COL_WRITE);
      writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 1); //0: 2 commands per sequence; 1: 4 commands per sequence
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0x085); // Write command
   #endif

   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

// Add extra delays to avoid the race with consective RIU programming for low frequency memory configurations
   if(glbl_tck > 2000)
      readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK));

   if(glbl_tck > 3000)
      readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK));

   if(glbl_tck > 4000)
      readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK));

   return;
}

u8 set_globals() {
   u8 nibble;
   u8 phy_nibble;
   u8 loop;
   u8 rank;
   u8 loc;
   static u8 lcl_dqs_bits_riu_map[DQS_BITS][2] = DQS_RIU_MAP ;
   static u8 lcl_dq_bits_riu_map[DQ_BITS][2]   = DQ_RIU_MAP  ;
   static u8 lcl_dq_bits_iob_map[DQ_BITS][2]   = DQ_IOB_MAP;
   static u8 lcl_dbi_bits_riu_map[DBI_BITS][2] = DBI_RIU_MAP ;
   static u8 cal_stage[64] = CAL_SEQ;
   u8 cal_stage_override;
   u16 calib_ctrl;
   u16 nibble_ctrl0;
   u16 nibble_ctrl2;
   u32 reg_config0;
   u32 reg_retry_0;
   u32 phy_rank_override;
   u32 wdata;
   u32 cal_en_lsb;
   u32 cal_en_msb;
   u8 cal_stage_en;
   u32 addr;

   glbl_cal_done = 0;
   glbl_refresh_en = 0;
   glbl_cal_error = 0;
   glbl_first_error_code = 0;
   glbl_error_phy_nibble = 0;
   glbl_error_nibble = 0;
   glbl_post_cal_error = 0;
   glbl_post_cal_status_tracking = 0;
   glbl_post_cal_status_self_ref = 0;
   glbl_read_dqs_track_en = 0;
   glbl_freq_f1 = 0;
   glbl_freq_sre = 0;
   glbl_freq_switch = 0;
   glbl_rl_dly_qtr_max = 0;
   glbl_pit1_cnt = 0;
   glbl_ddrmc_clk_cnt = 0;
   glbl_num_ref_total = 0;
   glbl_num_ref_cur = 0;
   glbl_num_ref_cur_max = 0;
   glbl_num_ref_limit_cnt = 0;
   glbl_num_ref_iter = 0;
   glbl_num_ref_prev = 0;
   glbl_fix_ref_en = 1;
   glbl_fix_num_ref = 1;
   glbl_side_ref = 0;
   glbl_ref_mode_1x = 0;
   glbl_ref_mode_2x = 0;
   glbl_ref_mode_4x = 0;
   glbl_ref_temp_sensor_en = 0;
   glbl_ref_speed = 0;
   glbl_wrvref_done = 0;
   glbl_heartbeat = 0;
   glbl_gpo1 = 0;
   glbl_gpo2 = 0;
   glbl_gpo4 = 0;
   glbl_lp4_mode_default = 0;
   glbl_gt_train_en = 0;
   glbl_side_cmd_en = 0;
   glbl_ipr = 0;
   glbl_lrdimm_en = 0;
   glbl_xram_freq_base = 0;
   glbl_pit1_preload = 0xFFFFFFFD; // 2 cycles overhead once lapses
   glbl_pcsr_lock = 0;

   writeIO_IOM(IO_PIT1_PRELOAD, glbl_pit1_preload);
   writeIO_IOM(IO_PIT1_CONTROL, 0x3); // PRELOAD & ENABLE
   glbl_pit1_cnt = readIO_IOM(IO_PIT1_COUNTER);

// Clear the XRAM content before starting the calibration sequence
   DEBUG_PRINTF("INFO: ddr_cal(): Calibration completed. Dumping all the XRAM values\n");
   for(addr=0; addr<8192*4; addr+=4) {
      writeIO_XRAM(addr, 0x0);
   }

   writeIO_XRAM(XRAM_CAL_PRE_STATUS_8_0, 0x1);

// Loading the version information in XRAM
   writeIO_XRAM(XRAM_HEADER_VER, VAL_HEADER_VER);
   writeIO_XRAM(XRAM_C_CODE_MAJOR_VER, VAL_C_CODE_MAJOR_VER);
   writeIO_XRAM(XRAM_C_CODE_MINOR_VER, VAL_C_CODE_MINOR_VER);
   writeIO_XRAM(XRAM_CAL_STATUS_MAP_VER, VAL_CAL_STATUS_MAP_VER);
   writeIO_XRAM(XRAM_PRE_POST_CAL_MAP_VER, VAL_PRE_POST_CAL_MAP_VER);
   writeIO_XRAM(XRAM_ERROR_CODE_VER, VAL_ERROR_CODE_VER);
   writeIO_XRAM(XRAM_WARNING_CODE_VER, VAL_WARNING_CODE_VER);
   writeIO_XRAM(XRAM_REG_MAP_VER, VAL_REG_MAP_VER);
   writeIO_XRAM(XRAM_IP_CONFIG_VER, VAL_IP_CONFIG_VER);
   writeIO_XRAM(XRAM_TAG_VER_0, VAL_TAG_VER_0);
   writeIO_XRAM(XRAM_TAG_VER_1, VAL_TAG_VER_1);
   writeIO_XRAM(XRAM_TAG_VER_2, VAL_TAG_VER_2);

   writeIO_XRAM(XRAM_CAL_PRE_STATUS_8_0, 0x3);

   // Enable NPI register write access
   glbl_pcsr_lock = readIO_DMC(ral_addr_of_pcsr_lock_in_ddrmc_main_ddrmc_main_bank(BLK)); // Record the PCSR lock status
   writeIO_DMC(ral_addr_of_pcsr_lock_in_ddrmc_main_ddrmc_main_bank(BLK), 0xf9e8d7c6);

   glbl_cal_mode_bypass = readIO_DMC(ral_addr_of_cal_mode_in_ddrmc_main_ddrmc_main_bank(BLK));
   DEBUG_PRINTF("INFO: set_globals(): cal_mode_bypass = %d \n", glbl_cal_mode_bypass);

   if(glbl_cal_mode_bypass == 0) { // Calibration Enabled
      #ifdef ICVAR_DELAY_EN
         DEBUG_PRINTF("INFO: set_globals(): Calibration with ICVAR enabled\n");
      #else
         DEBUG_PRINTF("INFO: set_globals(): Calibration with ICVAR disabled\n");
      #endif
   }

   for(loop=0; loop<64; loop++)
      glbl_cal_stage[loop] = cal_stage[loop];

   cal_en_msb = readIO_MBDR(MBDR_CAL_EN_63_32);
   cal_stage_override = (cal_en_msb >> 31) & 0x1;

   if(cal_stage_override) { // Calibration stage overriding
      cal_en_lsb = readIO_MBDR(MBDR_CAL_EN_31_0);
      loc = 0;
      for(loop=0; loop<64; loop++) {
         if(loop < 32)
            cal_stage_en = (cal_en_lsb >> loop) & 0x1;   
         else
            cal_stage_en = (cal_en_msb >> (loop - 32)) & 0x1;   
         
         if(cal_stage_en) {
            glbl_cal_stage[loc] = loop;
            loc++;
         }
      }
      // Clear the remainning stale content
      for(loop=loc; loop<64; loop++) {
         glbl_cal_stage[loop] = 0;
      }
   }

   DEBUG_PRINTF("INFO: set_globals(): Calibration sequence is here\n");
   for(loop=0; glbl_cal_stage[loop] != CAL_DONE; loop++) {
      if((glbl_cal_stage[loop] & 0x1C0) == CAL_SKIP) {
         DEBUG_PRINTF("INFO: set_globals(): CAL Stage Skipped = %x\n", glbl_cal_stage[loop] & 0x3F);
      } else if((glbl_cal_stage[loop] & 0x1C0) == CAL_EN) {
         DEBUG_PRINTF("INFO: set_globals(): CAL Stage Enabled = %x\n", glbl_cal_stage[loop] & 0x3F);
      } else { 
         DEBUG_PRINTF("ERROR: set_globals(): CAL Stage control ERROR = %x\n", glbl_cal_stage[loop]);
      }
   }
   DEBUG_PRINTF("INFO: set_globals(): CAL Stage Enabled = %x\n", glbl_cal_stage[loop] & 0x3F);

   glbl_restore_cal_en = readIO_IOM(IO_GPI1) & 0x1; // bit-0
   DEBUG_PRINTF("INFO: set_globals(): restore_cal_en = %d\n", glbl_restore_cal_en);

   for(loop=0; loop<DQS_BITS; loop++)
      for(loc=0; loc<2; loc++)
         glbl_dqs_bits_riu_map[loop][loc]  = lcl_dqs_bits_riu_map[loop][loc];

   // Check for BISC enable
   calib_ctrl = read_riu_nibble_reg(glbl_dqs_bits_riu_map[0][0], CFG_CALIB_CTRL);
   glbl_bisc_en = (calib_ctrl >> SELF_CALIBRATE_POS) & 0x1;

   DEBUG_PRINTF("INFO: set_globals(): BISC_EN = %d \n", glbl_bisc_en);
   writeIO_XRAM(XRAM_CAL_PRE_STATUS_8_0, 0x7);

   // Check DIMM Type
   reg_config0 = readIO_DMC(ral_addr_of_reg_config0_in_ddrmc_main_ddrmc_main_bank(BLK)); 

   glbl_num_ch = (reg_config0 >> 17) & 0x1;
   glbl_ddr4_2t = (reg_config0 >> 1) & 0x1;

#ifdef DDR4
   glbl_lrdimm_en = (((reg_config0 >> 12) & 0x3) == 0x3) ? 1 : 0;
   glbl_rdimm_en = glbl_lrdimm_en | ((((reg_config0 >> 12) & 0x3) == 0x2) ? 1 : 0);
   glbl_3ds_lranks = (0x1 << ((reg_config0 >> 20) & 0x3)); // Bits [21:20]
   glbl_3ds_en = (glbl_3ds_lranks != 1) ? 1 : 0;
   DEBUG_PRINTF("INFO: set_globals(): glbl_rdimm_en = %d, glbl_lrdimm_en = %d, glbl_3ds_en = %d, glbl_3ds_lranks = %d\n", glbl_rdimm_en, glbl_lrdimm_en, glbl_3ds_en, glbl_3ds_lranks);

   reg_retry_0 = readIO_DMC(ral_addr_of_reg_retry_0_in_ddrmc_main_ddrmc_main_bank(BLK)); 
   glbl_clamshell_cfg = reg_retry_0 & 0x1; // Clamshell configuration: ddrmc_main_es2_clamshell_cfg    (if_retry_regs_config.reg_retry_0_tpar_unknown[0])
#endif

   u8 pinout_swap_po_left2right_en = 0x1 & (readIO_DMC(ral_addr_of_reg_pinout_in_ddrmc_main_ddrmc_main_bank(0))>>8);

// Loading the configuration information in XRAM
#ifdef DDR4
   writeIO_XRAM(XRAM_MEM_TYPE, 0x1);
   writeIO_XRAM(XRAM_LRANKS, glbl_3ds_lranks);
#else
   writeIO_XRAM(XRAM_MEM_TYPE, 0x2);
   writeIO_XRAM(XRAM_LRANKS, 0x0);
#endif
   writeIO_XRAM(XRAM_PHY_RANKS, (glbl_lrdimm_en ? SLOTS : RANKS));
   writeIO_XRAM(XRAM_MEM_RANKS, RANKS);
   writeIO_XRAM(XRAM_BYTES, DQS_BITS);
   writeIO_XRAM(XRAM_NIBBLES, DQ_BITS/4);
   writeIO_XRAM(XRAM_BITS_PER_BYTE, DQ_BITS/DQS_BITS);
   writeIO_XRAM(XRAM_DBI_PINS, (NIBBLE_PER_DQS == 2) ? DQS_BITS : 0);
   writeIO_XRAM(XRAM_SLOTS, SLOTS);
   writeIO_XRAM(XRAM_PINOUT_SELECT, (pinout_swap_po_left2right_en << 8) | PINOUT_SELECT);
   writeIO_XRAM(XRAM_DIMM_TYPE, (reg_config0 >> 12) & 0x3); // [13:12]
   writeIO_XRAM(XRAM_NUM_CHANNELS, glbl_num_ch+1);
#if MODE_FREQ_SWITCH == CAL_EN
   writeIO_XRAM(XRAM_DUAL_FREQ_EN, 0x1);
#else   
   writeIO_XRAM(XRAM_DUAL_FREQ_EN, 0x0);
#endif  
   u8 dram_size = (reg_config0 >> 8) & 0x7;
   u8 dram_size1 = (dram_size == 0) ? 2 : (dram_size == 2) ? 3 : 4;
   writeIO_XRAM(XRAM_DRAM_SIZE, dram_size1); // [10:8]
   writeIO_XRAM(XRAM_MAX_BITS, 72);
   writeIO_XRAM(XRAM_MAX_NIBBLES, MAX_DQS_BITS);
   writeIO_XRAM(XRAM_MAX_BYTES, MAX_DQS_BITS);
   writeIO_XRAM(XRAM_MAX_DBI_PINS, 9);
   writeIO_XRAM(XRAM_SYS_CLK_8_0, (XPLL_SYSCLK_TP & 0x1FF));
   writeIO_XRAM(XRAM_SYS_CLK_17_9, ((XPLL_SYSCLK_TP >> 9) & 0x1FF));
   writeIO_XRAM(XRAM_UB_CLK_8_0, 0x2); // FIXME
   writeIO_XRAM(XRAM_UB_CLK_17_9, 0x2); // FIXME


// Loading the F0 configuration information in XRAM
   writeIO_XRAM(XRAM_F0_TCK_LSB, F0_TCK);
   writeIO_XRAM(XRAM_F0_TCK_MSB, (F0_TCK >> 9));
   writeIO_XRAM(XRAM_F0_XPLL_CLKOUTFB_LT, XPLL_F0_CLKFBOUT_LT);
   writeIO_XRAM(XRAM_F0_XPLL_CLKOUTFB_HT, XPLL_F0_CLKFBOUT_HT);
   writeIO_XRAM(XRAM_F0_XPLL_DIVCLK_LT, XPLL_F0_DIVCLK_LT);
   writeIO_XRAM(XRAM_F0_XPLL_DIVCLK_HT, XPLL_F0_DIVCLK_HT);

#ifdef DDR4
   static u32 lcl_f0_mr1[4] = F0_MR1;
   static u32 lcl_f0_mr5[4] = F0_MR5;
   writeIO_XRAM(XRAM_F0_MR0_8_0, F0_MR0 & 0x1FF);
   writeIO_XRAM(XRAM_F0_MR0_17_9, (F0_MR0 >> 9) & 0x1FF);
   writeIO_XRAM(XRAM_F0_MR1_8_0, lcl_f0_mr1[0] & 0x1FF);
   writeIO_XRAM(XRAM_F0_MR1_17_9, (lcl_f0_mr1[0] >> 9) & 0x1FF);

   wdata = (((lcl_f0_mr1[3] >> 8) & 0x7) << 6) | (((lcl_f0_mr1[2] >> 8) & 0x7) << 3) | ((lcl_f0_mr1[1] >> 8) & 0x7); // MR1[10:8]: RTT_NOM
   writeIO_XRAM(XRAM_F0_MR1_RTT_NOM, wdata);

   writeIO_XRAM(XRAM_F0_MR2_8_0, F0_MR2 & 0x1FF);
   writeIO_XRAM(XRAM_F0_MR2_17_9, (F0_MR2 >> 9) & 0x1FF);
   writeIO_XRAM(XRAM_F0_MR3_8_0, F0_MR3 & 0x1FF);
   writeIO_XRAM(XRAM_F0_MR3_17_9, (F0_MR3 >> 9) & 0x1FF);
   writeIO_XRAM(XRAM_F0_MR4_8_0, F0_MR4 & 0x1FF);
   writeIO_XRAM(XRAM_F0_MR4_17_9, (F0_MR4 >> 9) & 0x1FF);
   writeIO_XRAM(XRAM_F0_MR5_8_0, lcl_f0_mr5[0] & 0x1FF);
   writeIO_XRAM(XRAM_F0_MR5_17_9, (lcl_f0_mr5[0] >> 9) & 0x1FF);

   wdata = (((lcl_f0_mr5[3] >> 6) & 0x7) << 6) | (((lcl_f0_mr5[2] >> 6) & 0x7) << 3) | ((lcl_f0_mr5[1] >> 6) & 0x7); // MR5[8:6]: RTT_PARK
   writeIO_XRAM(XRAM_F0_MR5_RTT_PARK, wdata);

   writeIO_XRAM(XRAM_F0_MR6_8_0, F0_MR6 & 0x1FF);
   writeIO_XRAM(XRAM_F0_MR6_17_9, (F0_MR6 >> 9) & 0x1FF);
   writeIO_XRAM(XRAM_F0_RCD_RC0A, F0_RCD_RC0A);
   writeIO_XRAM(XRAM_F0_RCD_RC0F, F0_RCD_RC0F);
   writeIO_XRAM(XRAM_F0_RCD_RC3X, F0_RCD_RC3X);
   writeIO_XRAM(XRAM_F0_DB_BC0A, F0_DB_BC0A);
   writeIO_XRAM(XRAM_F0_DB_F0BC6X, F0_DB_F0BC6X);
#else
   writeIO_XRAM(XRAM_F0_MR1_8_0, F0_MR1 & 0xFF);
   writeIO_XRAM(XRAM_F0_MR2_8_0, F0_MR2 & 0xFF);
   writeIO_XRAM(XRAM_F0_MR3_8_0, F0_MR3 & 0xFF);
   writeIO_XRAM(XRAM_F0_MR4_8_0, F0_MR4 & 0xFF);
   writeIO_XRAM(XRAM_F0_MR5_8_0, F0_MR5 & 0xFF);
   writeIO_XRAM(XRAM_F0_LP4_MR12, F0_MR12);
   writeIO_XRAM(XRAM_F0_LP4_MR13, F0_MR13);
   writeIO_XRAM(XRAM_F0_LP4_MR14, F0_MR14);
   writeIO_XRAM(XRAM_F0_LP4_MR23, F0_MR23);
#endif
   writeIO_XRAM(XRAM_F0_READ_LATENCY, 0x1); // FIXME
   writeIO_XRAM(XRAM_F0_WRITE_LATENCY, 0x1); // FIXME
   writeIO_XRAM(XRAM_F0_DRAM_2T_MODE, glbl_ddr4_2t);
   writeIO_XRAM(XRAM_F0_DM_DBI_EN, 0x1); // FIXME
   writeIO_XRAM(XRAM_F0_ADD_CMD_DLY, (F0_ADD_CMD_DELAY_EN << 3) | F0_ADD_CMD_DELAY); // EN-[3], DLY-[2:0]
   writeIO_XRAM(XRAM_F0_PAR_ALERT_EN, 0x1); // FIXME

// Loading the F1 configuration information in XRAM
   writeIO_XRAM(XRAM_F1_TCK_LSB, F1_TCK);
   writeIO_XRAM(XRAM_F1_TCK_MSB, (F1_TCK >> 9));
   writeIO_XRAM(XRAM_F1_XPLL_CLKOUTFB_LT, XPLL_F1_CLKFBOUT_LT);
   writeIO_XRAM(XRAM_F1_XPLL_CLKOUTFB_HT, XPLL_F1_CLKFBOUT_HT);
   writeIO_XRAM(XRAM_F1_XPLL_DIVCLK_LT, XPLL_F1_DIVCLK_LT);
   writeIO_XRAM(XRAM_F1_XPLL_DIVCLK_HT, XPLL_F1_DIVCLK_HT);
#ifdef DDR4
   static u32 lcl_f1_mr1[4] = F1_MR1;
   static u32 lcl_f1_mr5[4] = F1_MR5;
   writeIO_XRAM(XRAM_F1_MR0_8_0, F1_MR0 & 0x1FF);
   writeIO_XRAM(XRAM_F1_MR0_17_9, (F1_MR0 >> 9) & 0x1FF);
   writeIO_XRAM(XRAM_F1_MR1_8_0, lcl_f1_mr1[0] & 0x1FF);
   writeIO_XRAM(XRAM_F1_MR1_17_9, (lcl_f1_mr1[0] >> 9) & 0x1FF);

   wdata = (((lcl_f1_mr1[3] >> 8) & 0x7) << 6) | (((lcl_f1_mr1[2] >> 8) & 0x7) << 3) | ((lcl_f1_mr1[1] >> 8) & 0x7); // MR1[10:8]: RTT_NOM
   writeIO_XRAM(XRAM_F1_MR1_RTT_NOM, wdata);

   writeIO_XRAM(XRAM_F1_MR2_8_0, F1_MR2 & 0x1FF);
   writeIO_XRAM(XRAM_F1_MR2_17_9, (F1_MR2 >> 9) & 0x1FF);
   writeIO_XRAM(XRAM_F1_MR3_8_0, F1_MR3 & 0x1FF);
   writeIO_XRAM(XRAM_F1_MR3_17_9, (F1_MR3 >> 9) & 0x1FF);
   writeIO_XRAM(XRAM_F1_MR4_8_0, F1_MR4 & 0x1FF);
   writeIO_XRAM(XRAM_F1_MR4_17_9, (F1_MR4 >> 9) & 0x1FF);
   writeIO_XRAM(XRAM_F1_MR5_8_0, lcl_f1_mr5[0] & 0x1FF);
   writeIO_XRAM(XRAM_F1_MR5_17_9, (lcl_f1_mr5[0] >> 9) & 0x1FF);

   wdata = (((lcl_f1_mr5[3] >> 6) & 0x7) << 6) | (((lcl_f1_mr5[2] >> 6) & 0x7) << 3) | ((lcl_f1_mr5[1] >> 6) & 0x7); // MR5[8:6]: RTT_PARK
   writeIO_XRAM(XRAM_F1_MR5_RTT_PARK, wdata);

   writeIO_XRAM(XRAM_F1_MR6_8_0, F1_MR6 & 0x1FF);
   writeIO_XRAM(XRAM_F1_MR6_17_9, (F1_MR6 >> 9) & 0x1FF);
   writeIO_XRAM(XRAM_F1_RCD_RC0A, F1_RCD_RC0A);
   writeIO_XRAM(XRAM_F1_RCD_RC0F, F1_RCD_RC0F);
   writeIO_XRAM(XRAM_F1_RCD_RC3X, F1_RCD_RC3X);
   writeIO_XRAM(XRAM_F1_DB_BC0A, F1_DB_BC0A);
   writeIO_XRAM(XRAM_F1_DB_F0BC6X, F1_DB_F0BC6X);
#else
   writeIO_XRAM(XRAM_F1_MR1_8_0, F1_MR1 & 0xFF);
   writeIO_XRAM(XRAM_F1_MR2_8_0, F1_MR2 & 0xFF);
   writeIO_XRAM(XRAM_F1_MR3_8_0, F1_MR3 & 0xFF);
   writeIO_XRAM(XRAM_F1_MR4_8_0, F1_MR4 & 0xFF);
   writeIO_XRAM(XRAM_F1_MR5_8_0, F1_MR5 & 0xFF);
   writeIO_XRAM(XRAM_F1_LP4_MR12, F1_MR12);
   writeIO_XRAM(XRAM_F1_LP4_MR13, F1_MR13);
   writeIO_XRAM(XRAM_F1_LP4_MR14, F1_MR14);
   writeIO_XRAM(XRAM_F1_LP4_MR23, F1_MR23);
#endif
   writeIO_XRAM(XRAM_F1_READ_LATENCY, 0x1); // FIXME
   writeIO_XRAM(XRAM_F1_WRITE_LATENCY, 0x1); // FIXME
   writeIO_XRAM(XRAM_F1_DRAM_2T_MODE, 0x1); // FIXME
   writeIO_XRAM(XRAM_F1_DM_DBI_EN, 0x1); // FIXME
   writeIO_XRAM(XRAM_F1_ADD_CMD_DLY, (F1_ADD_CMD_DELAY_EN << 3) | F1_ADD_CMD_DELAY); // EN-[3], DLY-[2:0]
   writeIO_XRAM(XRAM_F1_PAR_ALERT_EN, 0x1); // FIXME

   writeIO_XRAM(XRAM_CAL_PRE_STATUS_8_0, 0xF);

   writeIO_XRAM(XRAM_CAL_POST_STATUS_SELF_REF, 0x0);
   writeIO_XRAM(XRAM_CAL_POST_STATUS_TRACKING, 0x0);
   for(loop=0; loop<8; loop++) {
      writeIO_XRAM(XRAM_CAL_POST_STATUS + loop*4, 0x0);
   }

   writeIO_XRAM(XRAM_CAL_ERROR_DATA_NIBBLE_8_0, 0x0);
   writeIO_XRAM(XRAM_CAL_ERROR_DATA_NIBBLE_17_9, 0x0);
   writeIO_XRAM(XRAM_CAL_ERROR_PHY_NIBBLE_8_0, 0x0);
   writeIO_XRAM(XRAM_CAL_ERROR_PHY_NIBBLE_17_9, 0x0);
   writeIO_XRAM(XRAM_CAL_ERROR_PHY_NIBBLE_26_18, 0x0);
   for(loop=0; loop<27; loop++) {
      writeIO_XRAM(XRAM_CAL_ERROR_BIT_PHY_NIBBLE + loop*4, 0x0);
   }

   glbl_xpi_pmi_config = readIO_DMC(ral_addr_of_xpi_pmi_config_in_ddrmc_main_ddrmc_main_bank(BLK)); 
   glbl_xpi_mirror_en = (glbl_xpi_pmi_config >> 4) & 0x1;
   DEBUG_PRINTF("INFO: set_globals(): xpi_pmi_config = %x\n", glbl_xpi_pmi_config);

#ifdef DDR4
   if(RANKS == 1)
      glbl_phy_rank_switch_en = 0;
   else if((glbl_lrdimm_en == 1) && (SLOTS == 1))
      glbl_phy_rank_switch_en = 0;
   else
      glbl_phy_rank_switch_en = 1;

   if(glbl_lrdimm_en == 1) {
      glbl_rank_inc = RANKS/SLOTS;

      if(SLOTS == 1) // All DRAM ranks through xphy rank 0
        phy_rank_override = 0x1;
      else // 2S-2R: DRAM ranks 0 & 1 through xphy rank 0 and DRAM ranks 2 & 3 through xphy rank 2
        phy_rank_override = 0x28281; // [17:16] = [15:14] = [9:8] = [7:6] = 0x2

      writeIO_DMC(ral_addr_of_phy_rank_read_override_in_ddrmc_main_ddrmc_main_bank(BLK), phy_rank_override);
      writeIO_DMC(ral_addr_of_phy_rank_write_override_in_ddrmc_main_ddrmc_main_bank(BLK), phy_rank_override);
   } else if(glbl_rdimm_en == 1) {
      glbl_rank_inc = 1;
   } else {
      glbl_rank_inc = 1;
   }

   if(glbl_restore_cal_en == 1) { // Only in restore calibration. It is done after memory initialization (init_done) for POR
      if(glbl_clamshell_cfg == 1) { // Clamshell configuration
         wdata = readIO_DMC(ral_addr_of_seq_side_addr15_in_ddrmc_main_ddrmc_main_bank(BLK));
         wdata |= (0x1 << 28); // ddrmc_main_es2_clamshell_status (if_cal_seq_regs_status.seq_side_addr15_cbit[0])
         writeIO_DMC(ral_addr_of_seq_side_addr15_in_ddrmc_main_ddrmc_main_bank(BLK), wdata);
      }
   }

#else
   glbl_lp4_ch1_nibble_base = glbl_num_ch ? (DQS_BITS*NIBBLE_PER_DQS)/2 : (DQS_BITS*NIBBLE_PER_DQS); // Channels with equal width
   glbl_ch0_mem_rank0_xphy_rank = 0;
   glbl_ch0_mem_rank1_xphy_rank = 1;
   glbl_ch1_mem_rank0_xphy_rank = 0;
   glbl_ch1_mem_rank1_xphy_rank = 1;

   if(RANKS == 1)
      glbl_phy_rank_switch_en = 0;
   else
      glbl_phy_rank_switch_en = 1;

   glbl_rank_inc = 1;
#endif

   // Set the MRx global variables
   set_freq_globals();

   #ifdef DDR4
      if((glbl_mr5[0] & 0x0C00) != 0)
         glbl_wr_dbi_en = 1;
      else
         glbl_wr_dbi_en = 0;
   #else
      if(((glbl_lp4_mr3 & 0x0080) != 0) || (((~glbl_lp4_mr13) & 0x0020) != 0))
         glbl_wr_dbi_en = 1; 
      else
         glbl_wr_dbi_en = 0;
   #endif
   DEBUG_PRINTF("INFO: set_globals(): glbl_wr_dbi_en = %d \n", glbl_wr_dbi_en);

   glbl_xpi_write_dm_dbi = readIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK)); 
   glbl_xpi_read_dbi = readIO_DMC(ral_addr_of_xpi_read_dbi_in_ddrmc_main_ddrmc_main_bank(BLK)); 
   DEBUG_PRINTF("INFO: set_globals(): xpi_write_dm_dbi = %x, glbl_xpi_read_dbi = %x\n", glbl_xpi_write_dm_dbi, glbl_xpi_read_dbi);

   writeIO_XRAM(XRAM_CAL_PRE_STATUS_8_0, 0x1F);

   for(loop=0; loop<DQS_BITS; loop++)
      glbl_wrvref_value[loop] = glbl_wrvref_default_value;

   for(loop=0; loop<DQ_BITS; loop++)
     for(loc=0; loc<2; loc++)
       glbl_dq_bits_riu_map[loop][loc]   = lcl_dq_bits_riu_map[loop][loc];

   for(loop=0; loop<DQ_BITS; loop++)
     for(loc=0; loc<2; loc++)
       glbl_dq_bits_iob_map[loop][loc]   = lcl_dq_bits_iob_map[loop][loc];

   for(loop=0; loop<DBI_BITS; loop++)
     for(loc=0; loc<2; loc++)
       glbl_dbi_bits_riu_map[loop][loc] = lcl_dbi_bits_riu_map[loop][loc];

   u8 lb_loop;
   u8 lb_nibble;
   u8 lb_bit;
   u8 pb_bit;
   u8 pb_loop;
   for(lb_loop=0; lb_loop<DQ_BITS; lb_loop++) {
      lb_nibble = glbl_dq_bits_riu_map[lb_loop][0];
      lb_bit    = glbl_dq_bits_riu_map[lb_loop][1];
      for(pb_loop=0; pb_loop<DQ_BITS; pb_loop++) {
         if((glbl_dq_bits_iob_map[pb_loop][0] == lb_nibble) && (glbl_dq_bits_iob_map[pb_loop][1] == lb_bit)) {
            glbl_dq_lbit2pbit_map[lb_loop] = pb_loop;
            break;
         }
      }
      DEBUG_PRINTF("INFO: set_globals(): Logical DQ[%d] is mapped to Physical DQ[%d] \n", lb_loop, glbl_dq_lbit2pbit_map[lb_loop]);
   }

   for(lb_loop=0; lb_loop<DQ_BITS; lb_loop++) {
      pb_bit = glbl_dq_lbit2pbit_map[lb_loop];
      glbl_dq_pbit2lbit_map[pb_bit] = lb_loop;
      DEBUG_PRINTF("INFO: set_globals(): Physical DQ[%d] is mapped to Logical DQ[%d] \n", pb_bit, glbl_dq_pbit2lbit_map[pb_bit]);
   }

   glbl_ecc_en = (readIO_DMC(ral_addr_of_reg_pinout_in_ddrmc_main_ddrmc_main_bank(0)) >> 5) & 0x1;

   glbl_reg_scrub1 = readIO_DMC(ral_addr_of_reg_scrub1_in_ddrmc_main_ddrmc_main_bank(0));
   glbl_reg_scrub4 = readIO_DMC(ral_addr_of_reg_scrub4_in_ddrmc_main_ddrmc_main_bank(0));
   glbl_reg_scrub5 = readIO_DMC(ral_addr_of_reg_scrub5_in_ddrmc_main_ddrmc_main_bank(0));
   glbl_reg_scrub6 = readIO_DMC(ral_addr_of_reg_scrub6_in_ddrmc_main_ddrmc_main_bank(0));
   glbl_reg_scrub7 = readIO_DMC(ral_addr_of_reg_scrub7_in_ddrmc_main_ddrmc_main_bank(0));
   glbl_reg_scrub8 = readIO_DMC(ral_addr_of_reg_scrub8_in_ddrmc_main_ddrmc_main_bank(0));

#ifdef LPDDR4
   u8 temp_loc;
  
   static u8 lcl_cs_bits_riu_map[LP4_MAX_CS_BITS*LP4_MAX_RANKS][2]   = CS_RIU_MAP ;
   for(loop=0; loop<LP4_MAX_CS_BITS*LP4_MAX_RANKS; loop++)
     for(loc=0; loc<2; loc++) {
        temp_loc = lcl_cs_bits_riu_map[loop][loc];
        if((pinout_swap_po_left2right_en==1) && (loc==0))
           glbl_cs_bits_riu_map[loop][loc]   = (2-temp_loc/9)*9 + (((temp_loc%9) == 8 ) ? 8 : (7-(temp_loc%9)));
        else
           glbl_cs_bits_riu_map[loop][loc]   = temp_loc;
      }

   static u8 lcl_ca_bits_riu_map[LP4_MAX_CS_BITS*6][2]   = CA_RIU_MAP ;
   for(loop=0; loop<CA_BITS; loop++)
     for(loc=0; loc<2; loc++) {
        temp_loc = lcl_ca_bits_riu_map[loop][loc];
        if((pinout_swap_po_left2right_en==1) && (loc==0))
           glbl_ca_bits_riu_map[loop][loc]   = (2-temp_loc/9)*9 + (((temp_loc%9) == 8 ) ? 8 : (7-(temp_loc%9)));
        else
           glbl_ca_bits_riu_map[loop][loc]   = temp_loc;
      }
        

   for(rank=0; rank<2; rank++) {
      for(nibble=0; nibble<4; nibble++) { // x16 granularity
         glbl_cal_tdqsosc[rank][nibble] = 0;
         glbl_cur_tdqsosc[rank][nibble] = 0;
         glbl_cal_tdqs2dq[rank][nibble] = 500;
         glbl_cur_tdqs2dq[rank][nibble] = 500;
         glbl_ref_rate[rank][nibble] = 0x3; // 1x refresh: default
      }
      for(nibble=0; nibble<16; nibble++) { // nibble granularity
         glbl_cal_wl_dly_rnk[rank][nibble] = 0;
      }
   }

#endif

/******************************************************************************************************/
/******* Initial map creation in global variables *****************************************************/
/******************************************************************************************************/
   map_l2p(glbl_dqs_bits_riu_map, DQS_BITS, glbl_dqs_lnibble2pnibble_map, glbl_dqs_lbit2pnibble_map, &glbl_dqs_first_nibble, &glbl_dqs_last_nibble  ,&glbl_dqs_nibble_mask_initial);
   DEBUG_PRINTF("INFO: set_globals(): DQS Map bits = %d, first nibble = %d, last nibble = %d, nibble mask = 0x%08X\n", DQS_BITS, glbl_dqs_first_nibble, glbl_dqs_last_nibble, glbl_dqs_nibble_mask_initial);

   map_l2p(glbl_dq_bits_riu_map,  DQ_BITS,  glbl_dq_lnibble2pnibble_map,  glbl_dq_lbit2pnibble_map,  &glbl_dq_first_nibble,  &glbl_dq_last_nibble   ,&glbl_dq_nibble_mask_initial);
   DEBUG_PRINTF("INFO: set_globals(): DQ Map bits = %d, first nibble = %d, last nibble = %d, nibble mask = 0x%08X\n", DQ_BITS, glbl_dq_first_nibble, glbl_dq_last_nibble, glbl_dq_nibble_mask_initial);

   map_l2p(glbl_dbi_bits_riu_map, DBI_BITS, glbl_dbi_lnibble2pnibble_map, glbl_dbi_lbit2pnibble_map, &glbl_dbi_first_nibble, &glbl_dbi_last_nibble  ,&glbl_dbi_nibble_mask_initial);
   DEBUG_PRINTF("INFO: set_globals(): DBI Map bits = %d, first nibble = %d, last nibble = %d, nibble mask = 0x%08X\n", DBI_BITS, glbl_dbi_first_nibble, glbl_dbi_last_nibble, glbl_dbi_nibble_mask_initial);

#ifdef LPDDR4
   map_l2p(glbl_ca_bits_riu_map,  CA_BITS,  glbl_ca_lnibble2pnibble_map,  glbl_ca_lbit2pnibble_map,  &glbl_ca_first_nibble,  &glbl_ca_last_nibble   ,&glbl_ca_nibble_mask_initial);
   DEBUG_PRINTF("INFO: set_globals(): CA Map bits = %d, first nibble = %d, last nibble = %d, nibble mask = 0x%08X\n", CA_BITS, glbl_ca_first_nibble, glbl_ca_last_nibble, glbl_ca_nibble_mask_initial);

   map_l2p(glbl_cs_bits_riu_map,  LP4_MAX_CS_BITS*LP4_MAX_RANKS,  glbl_cs_lnibble2pnibble_map,  glbl_cs_lbit2pnibble_map,  &glbl_cs_first_nibble,  &glbl_cs_last_nibble   ,&glbl_cs_nibble_mask_initial);
   DEBUG_PRINTF("INFO: set_globals(): CS Map bits = %d, first nibble = %d, last nibble = %d, nibble mask = 0x%08X\n", LP4_MAX_CS_BITS*LP4_MAX_RANKS, glbl_cs_first_nibble, glbl_cs_last_nibble, glbl_cs_nibble_mask_initial);
   for(loop=0;loop<LP4_MAX_CS_BITS*LP4_MAX_RANKS;loop++) {
      DEBUG_PRINTF("INFO: set_globals(): glbl_cs_lbit2pnibble_map[%d]=%d, glbl_cs_lnibble2pnibble_map[%d]=%d \n", loop, glbl_cs_lbit2pnibble_map[loop], loop, glbl_cs_lnibble2pnibble_map[loop]);
   }
   delay2riu_reg_addres_map(glbl_ca_bits_riu_map, CA_BITS, glbl_ca_odly_riu_addr_map, ODLY0); 
   delay2riu_reg_addres_map(glbl_cs_bits_riu_map, LP4_MAX_CS_BITS*LP4_MAX_RANKS, glbl_cs_odly_riu_addr_map, ODLY0); 
#endif   

   delay2riu_reg_addres_map(glbl_dq_bits_riu_map,  DQ_BITS,  glbl_dq_idly_riu_addr_map,  IDLY0); 
   delay2riu_reg_addres_map(glbl_dq_bits_riu_map,  DQ_BITS,  glbl_dq_odly_riu_addr_map,  ODLY0); 
   delay2riu_reg_addres_map(glbl_dbi_bits_riu_map, DBI_BITS, glbl_dbi_idly_riu_addr_map, IDLY0); 
   delay2riu_reg_addres_map(glbl_dbi_bits_riu_map, DBI_BITS, glbl_dbi_odly_riu_addr_map, ODLY0); 
   delay2riu_reg_addres_map(glbl_dqs_bits_riu_map, DQS_BITS, glbl_dqs_odly_riu_addr_map, ODLY0); 

   get_riu_map(glbl_nibble_reset_mask, glbl_map_riu);
   
   writeIO_XRAM(XRAM_CAL_PRE_STATUS_8_0, 0x3F);

   writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
   riu_reg_bcast(INCDEC_CRSE, 0, 0x8);

   // Enable to stop fixdly_rdy based on BISC overflow/underflow errors.
//   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
//      nibble_ctrl0 = read_riu_nibble_reg(glbl_map_riu[nibble], CFG_NIBBLE_CTRL0);
//      nibble_ctrl0 |= (0x1 << FIXDLY_MASK_EN_POS);
//      write_riu_nibble_reg(glbl_map_riu[nibble], CFG_NIBBLE_CTRL0, nibble_ctrl0);
//   }

   glbl_bit_compare_31_0  = 0;
   glbl_bit_compare_63_32 = 0;
   glbl_bit_compare_95_64 = 0;
   u8 bit;
   for(bit=0;bit<DQ_BITS;bit++) {
      if((bit==0) || (bit==32) || (bit==64) ) {
         glbl_bit_pos = 0x1;
      } else {
         glbl_bit_pos = glbl_bit_pos<<1;
      }
      if(bit<32)
         glbl_bit_compare_31_0  = glbl_bit_compare_31_0  | glbl_bit_pos;
      else if(bit<64) {
         glbl_bit_compare_63_32 = glbl_bit_compare_63_32 | glbl_bit_pos;
      } else {
         glbl_bit_compare_95_64 = glbl_bit_compare_95_64 | glbl_bit_pos;
      }
   }
   glbl_nibble_compare_17_0 = 0;
   for(nibble=0,glbl_bit_pos=1;nibble<DQ_NIBBLES;nibble++,glbl_bit_pos=glbl_bit_pos<<1) {
      glbl_nibble_compare_17_0 = glbl_nibble_compare_17_0 | glbl_bit_pos;
   }
   glbl_byte_compare_8_0 = 0;
   for(nibble=0,glbl_bit_pos=1;nibble<(DQ_NIBBLES/2);nibble++,glbl_bit_pos=glbl_bit_pos<<1) {
      glbl_byte_compare_8_0 = glbl_byte_compare_8_0 | glbl_bit_pos;
   }
   DEBUG_PRINTF("INFO: set_globals(): glbl_bit_compare_95_64 = 0x%08X, glbl_bit_compare_63_32 = 0x%08X, glbl_bit_compare_31_0 = 0x%08X \n", glbl_bit_compare_95_64, glbl_bit_compare_63_32, glbl_bit_compare_31_0);
   DEBUG_PRINTF("INFO: set_globals(): glbl_nibble_compare_17_0 = 0x%08X \n", glbl_nibble_compare_17_0);
   DEBUG_PRINTF("INFO: set_globals(): glbl_byte_compare_8_0 = 0x%08X \n", glbl_byte_compare_8_0);

   //Set default vref across all the data nibbles.
   DEBUG_PRINTF("INFO: set_globals(): Updated the Vref of Physical daya nibbles\n");
   writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial); 
   riu_reg_bcast(VREF_CTRL, 0, (XPIO_VREF_DEFAULT | 0x400)); // 10th bit sets the fab2iob_r2rdac_mode, [9:0] is fab2iob_rwrdac_sel

#if IBUF_DISABLE == 1
   for(nibble=0; nibble<DQ_NIBBLES; nibble++) {
      phy_nibble = glbl_map_riu[nibble];
      nibble_ctrl2 = read_riu_nibble_reg(phy_nibble, DUP_NIBBLE_CTRL2);
      nibble_ctrl2 |= 0x3F; // [5:0], ODT = 1
      write_riu_nibble_reg(phy_nibble, DUP_NIBBLE_CTRL2, nibble_ctrl2);
   }
#endif

   glbl_ch0_nibble_mask = 0;
   glbl_ch1_nibble_mask = 0;
   for(nibble=0; nibble<DQ_NIBBLES; nibble++) {
      if(nibble < DQ_NIBBLES/2)
         glbl_ch0_nibble_mask |= (0x1 << glbl_map_riu[nibble]);
      else
         glbl_ch1_nibble_mask |= (0x1 << glbl_map_riu[nibble]);
   }
   glbl_ch0_nibble_mask = ~glbl_ch0_nibble_mask; // Invert it for the mask
   glbl_ch1_nibble_mask = ~glbl_ch1_nibble_mask; // Invert it for the mask

   #ifdef ENABLE_MICROBLAZE_BFM
   for (glbl_cal_index=0; glbl_cal_stage[glbl_cal_index]!=CAL_DONE; glbl_cal_index++) {
      glbl_cal_status = glbl_cal_stage[glbl_cal_index];
         switch(glbl_cal_status) {
            case PHY_BISC:
               glbl_stage_code_string[glbl_cal_status] = "PHY_BISC";
               break;
            case MEM_INIT:
               glbl_stage_code_string[glbl_cal_status] = "MEM_INIT";
               break;
            #ifdef LPDDR4
               case F0_LPDDR4_CS_CA_TRAIN:
                  glbl_stage_code_string[glbl_cal_status] = "F0_LPDDR4_CS_CA_TRAIN";
                  break;
               case F1_LPDDR4_CS_CA_TRAIN:
                  glbl_stage_code_string[glbl_cal_status] = "F1_LPDDR4_CS_CA_TRAIN";
                  break;
               case F0_LPDDR4_CA_VREF_TRAIN:
                  glbl_stage_code_string[glbl_cal_status] = "F0_LPDDR4_CA_VREF_TRAIN";
                  break;
               case F1_LPDDR4_CA_VREF_TRAIN:
                  glbl_stage_code_string[glbl_cal_status] = "F1_LPDDR4_CA_VREF_TRAIN";
                  break;
            #endif
            #ifdef DDR4
               case F0_LRDIMM_DB_MREP: 
                  glbl_stage_code_string[glbl_cal_status] = "F0_LRDIMM_DB_MREP";
                  break;
               case F1_LRDIMM_DB_MREP: 
                  glbl_stage_code_string[glbl_cal_status] = "F1_LRDIMM_DB_MREP";
                  break;
               case F0_LRDIMM_DB_MRD_CYCLE: 
                  glbl_stage_code_string[glbl_cal_status] = "F0_LRDIMM_DB_MRD_CYCLE";
                  break;
               case F1_LRDIMM_DB_MRD_CYCLE: 
                  glbl_stage_code_string[glbl_cal_status] = "F1_LRDIMM_DB_MRD_CYCLE";
                  break;
               case F0_LRDIMM_DB_MRD_CENTER: 
                  glbl_stage_code_string[glbl_cal_status] = "F0_LRDIMM_DB_MRD_CENTER";
                  break;
               case F1_LRDIMM_DB_MRD_CENTER: 
                  glbl_stage_code_string[glbl_cal_status] = "F1_LRDIMM_DB_MRD_CENTER";
                  break;
               case F0_LRDIMM_DB_DWL: 
                  glbl_stage_code_string[glbl_cal_status] = "F0_LRDIMM_DB_DWL";
                  break;
               case F1_LRDIMM_DB_DWL: 
                  glbl_stage_code_string[glbl_cal_status] = "F1_LRDIMM_DB_DWL";
                  break;
               case F0_LRDIMM_DB_MWD_CYCLE: 
                  glbl_stage_code_string[glbl_cal_status] = "F0_LRDIMM_DB_MWD_CYCLE";
                  break;
               case F1_LRDIMM_DB_MWD_CYCLE: 
                  glbl_stage_code_string[glbl_cal_status] = "F1_LRDIMM_DB_MWD_CYCLE";
                  break;
               case F0_LRDIMM_DB_MWD_CENTER: 
                  glbl_stage_code_string[glbl_cal_status] = "F0_LRDIMM_DB_MWD_CENTER";
                  break;
               case F1_LRDIMM_DB_MWD_CENTER: 
                  glbl_stage_code_string[glbl_cal_status] = "F1_LRDIMM_DB_MWD_CENTER";
                  break;
            #endif
            case F0_DQS_GATE_CAL:
               glbl_stage_code_string[glbl_cal_status] = "F0_DQS_GATE_CAL";
               break;
            case F1_DQS_GATE_CAL:
               glbl_stage_code_string[glbl_cal_status] = "F1_DQS_GATE_CAL";
               break;
            case F0_WRITE_LEVELING: 
               glbl_stage_code_string[glbl_cal_status] = "F0_WRITE_LEVELING";
               break;
            case F1_WRITE_LEVELING: 
               glbl_stage_code_string[glbl_cal_status] = "F1_WRITE_LEVELING";
               break;
            case F0_READ_DQ_CAL: 
               glbl_stage_code_string[glbl_cal_status] = "F0_READ_DQ_CAL";
               break;
            case F1_READ_DQ_CAL: 
               glbl_stage_code_string[glbl_cal_status] = "F1_READ_DQ_CAL";
               break;
            case F0_WRITE_DQ_DBI_CAL: 
               glbl_stage_code_string[glbl_cal_status] = "F0_WRITE_DQ_DBI_CAL";
               break;
            case F1_WRITE_DQ_DBI_CAL: 
               glbl_stage_code_string[glbl_cal_status] = "F1_WRITE_DQ_DBI_CAL";
               break;
            case F0_WRITE_LATENCY_CAL: 
               glbl_stage_code_string[glbl_cal_status] = "F0_WRITE_LATENCY_CAL";
               break;
            case F1_WRITE_LATENCY_CAL: 
               glbl_stage_code_string[glbl_cal_status] = "F1_WRITE_LATENCY_CAL";
               break;
            case F0_READ_DBI_CAL: 
               glbl_stage_code_string[glbl_cal_status] = "F0_READ_DBI_CAL";
               break;
            case F1_READ_DBI_CAL: 
               glbl_stage_code_string[glbl_cal_status] = "F1_READ_DBI_CAL";
               break;
            case F0_READ_DQ_VREF_CAL:
               glbl_stage_code_string[glbl_cal_status] = "F0_READ_DQ_VREF_CAL";
               break;
            case F1_READ_DQ_VREF_CAL:
               glbl_stage_code_string[glbl_cal_status] = "F1_READ_DQ_VREF_CAL";
               break;
            case F0_READ_DQ_DBI_CAL_COMPLEX:
               glbl_stage_code_string[glbl_cal_status] = "F0_READ_DQ_DBI_CAL_COMPLEX";
               break;
            case F1_READ_DQ_DBI_CAL_COMPLEX:
               glbl_stage_code_string[glbl_cal_status] = "F1_READ_DQ_DBI_CAL_COMPLEX";
               break;
            case F0_WRITE_DQ_VREF_CAL: 
               glbl_stage_code_string[glbl_cal_status] = "F0_WRITE_DQ_VREF_CAL";
               break;
            case F1_WRITE_DQ_VREF_CAL: 
               glbl_stage_code_string[glbl_cal_status] = "F1_WRITE_DQ_VREF_CAL";
               break;
            case F0_WRITE_DQ_DBI_CAL_COMPLEX: 
               glbl_stage_code_string[glbl_cal_status] = "F0_WRITE_DQ_DBI_CAL_COMPLEX";
               break;
            case F1_WRITE_DQ_DBI_CAL_COMPLEX: 
               glbl_stage_code_string[glbl_cal_status] = "F1_WRITE_DQ_DBI_CAL_COMPLEX";
               break;
            case F0_PRBS_READ: 
               glbl_stage_code_string[glbl_cal_status] = "F0_PRBS_READ";
               break;
            case F1_PRBS_READ: 
               glbl_stage_code_string[glbl_cal_status] = "F1_PRBS_READ";
               break;
            case F0_PRBS_WRITE: 
               glbl_stage_code_string[glbl_cal_status] = "F0_PRBS_WRITE";
               break;
            case F1_PRBS_WRITE: 
               glbl_stage_code_string[glbl_cal_status] = "F1_PRBS_WRITE";
               break;
            case EN_VT_TRACK: 
               glbl_stage_code_string[glbl_cal_status] = "EN_VT_TRACK";
               break;
            case READ_DQS_TRACK: 
               glbl_stage_code_string[glbl_cal_status] = "READ_DQS_TRACK";
               break;
            #ifdef LPDDR4
            case LPDDR4_WR_DQS_TRACK: 
               glbl_stage_code_string[glbl_cal_status] = "LPDDR4_WR_DQS_TRACK";
               break;
            #endif
            case FREQ_SWITCH: 
               glbl_stage_code_string[glbl_cal_status] = "FREQ_SWITCH";
               break;
            default:
               glbl_stage_code_string[glbl_cal_status] = "Unknow Stage";
         } // Switch glbl_cal_stage
      }
      glbl_cal_status = glbl_cal_stage[glbl_cal_index];
      glbl_stage_code_string[glbl_cal_status] = "CAL_DONE";
   #endif

   // Load calibration sequence in XRAM
   for (glbl_cal_index=0; glbl_cal_stage[glbl_cal_index]!=CAL_DONE; glbl_cal_index++) {
      glbl_cal_status = glbl_cal_stage[glbl_cal_index];
      writeIO_XRAM((XRAM_CAL_SEQ_STATUS + 4*glbl_cal_index), glbl_cal_status);
      if(glbl_cal_index == 0) {
         writeIO_XRAM(XRAM_CAL_POINTER, glbl_cal_status);
         glbl_gpo3 = (glbl_gpo3 & GPO3_CAL_STATUS_CLR) | (glbl_cal_status << GPO3_CAL_STATUS_POS);
         writeIO_IOM(IO_GPO3, glbl_gpo3);
      }
   }
   writeIO_XRAM((XRAM_CAL_SEQ_STATUS + 4*glbl_cal_index), glbl_cal_stage[glbl_cal_index]);

   writeIO_XRAM(XRAM_CAL_PRE_STATUS_8_0, 0x7F);

#ifndef ENABLE_MICROBLAZE_BFM
#ifndef VNC_DIS_INTR
   static XIOModule IOModule;

   // Enable the interrupts
   writeIO_IOM(IO_IRQ_ENABLE, IRQ_EN_ALERT);

   // Register the interrupts 
   microblaze_register_handler((XInterruptHandler)gpio_interrupt_handler, (void *)(&IOModule));

   // Clear any out standing interrupt request
   writeIO_IOM(IO_IRQ_ACK,0xFFFFFFFF);

   // Enable the interrupt in the Microblaze core
   microblaze_enable_interrupts();
   writeIO_XRAM(XRAM_CAL_PRE_STATUS_8_0, 0xFF);
#endif // VNC_DIS_INTR
#endif

//Loading mission mode controls
   writeIO_XRAM(XRAM_ECC_EN, glbl_ecc_en); 
   writeIO_XRAM(XRAM_REF_EN, glbl_reg_config1); 
   writeIO_XRAM(XRAM_PER_RD_EN, glbl_reg_config2); 
   writeIO_XRAM(XRAM_TRAF_SELF_REF_EN,0);          
   writeIO_XRAM(XRAM_TRAF_POWER_DOWN_EN,0);        
   writeIO_XRAM(XRAM_SCRUB_EN,(glbl_reg_scrub8 ^ 1) & 1);                  
   writeIO_XRAM(XRAM_BISC_EN, glbl_bisc_en);
   writeIO_XRAM(XRAM_DQS_GATE_PHY_UB,1);
#if(MODE_READ_DQS_TRACK == CAL_EN)
   writeIO_XRAM(XRAM_DQS_GATE_TRACK_EN,1);
#else
   writeIO_XRAM(XRAM_DQS_GATE_TRACK_EN,0);
#endif
#ifdef LPDDR4
   writeIO_XRAM(XRAM_LP4_OSC_TRACK_EN,1);
#else
   writeIO_XRAM(XRAM_LP4_OSC_TRACK_EN,0);
#endif
 
   return 0;
}

void set_timing() {
   u32 reg_safe_config7;

   reg_safe_config7 = readIO_DMC(ral_addr_of_reg_safe_config7_in_ddrmc_main_ddrmc_main_bank(BLK));
   glbl_t_rfc_slr = reg_safe_config7 & 0x3FF;
   glbl_t_rfc_dlr = (reg_safe_config7 >> 10) & 0x3FF;
   #ifdef DDR4
      glbl_t_rfc_slr = glbl_t_rfc_slr + 8 + 4; // Add offset as per ODS + extra offset
      glbl_t_rfc_dlr = glbl_t_rfc_dlr + 8 + 4; // Add offset as per ODS + extra offset
   #else
      glbl_t_rfc_slr = glbl_t_rfc_slr + 10 + 4; // Add offset as per ODS + extra offset
      glbl_t_rfc_dlr = glbl_t_rfc_dlr + 10 + 4; // Add offset as per ODS + extra offset
   #endif

//   if(glbl_ref_mode_4x) { // 4x mode
//      DEBUG_PRINTF("INFO: set_timing(): Refresh mode = Fixed 4x. tRFC_SLR = %d, tRFC_DLR = %d\n", glbl_t_rfc_slr, glbl_t_rfc_dlr);
//      glbl_t_rfc_slr_1x = (glbl_t_rfc_slr * 24) / 10; // Worst case 4x to 1x conversion rate is 240%.
//      glbl_t_rfc_dlr_1x = (glbl_t_rfc_dlr * 24) / 10;
//      DEBUG_PRINTF("INFO: set_timing(): Refresh mode = Fixed 1x. tRFC_SLR = %d, tRFC_DLR = %d\n", glbl_t_rfc_slr_1x, glbl_t_rfc_dlr_1x);
//   } else if(glbl_ref_mode_2x) { // 2x mode
//      DEBUG_PRINTF("INFO: set_timing(): Refresh mode = Fixed 2x. tRFC_SLR = %d, tRFC_DLR = %d\n", glbl_t_rfc_slr, glbl_t_rfc_dlr);
//      glbl_t_rfc_slr_1x = (glbl_t_rfc_slr * 17) / 10; // Worst case 2x to 1x conversion rate is 170%.
//      glbl_t_rfc_dlr_1x = (glbl_t_rfc_dlr * 17) / 10;
//      DEBUG_PRINTF("INFO: set_timing(): Refresh mode = Fixed 1x. tRFC_SLR = %d, tRFC_DLR = %d\n", glbl_t_rfc_slr_1x, glbl_t_rfc_dlr_1x);
//   } else { // Normal 1x mode
//      glbl_t_rfc_slr_1x = glbl_t_rfc_slr;
//      glbl_t_rfc_dlr_1x = glbl_t_rfc_dlr;
//      DEBUG_PRINTF("INFO: set_timing(): Refresh mode = Fixed 1x. tRFC_SLR = %d, tRFC_DLR = %d\n", glbl_t_rfc_slr_1x, glbl_t_rfc_dlr_1x);
//   }

   return;
}

void set_defaults(u8 power_on_reset) {
   glbl_en_qtr_dly_upd_wbs = 1;
   u8 rank;
   u8 loop;
   u8 nibble;
   u8 status;
   u8 mirror_en;
   u8 fifo_rden_lat;
   u32 reg_safe_config5;
   u8 rcd_tpdm_offset;
   u32 rcd_tpdm;
   u32 db_tpdm;
   u8 db_rcd_tpdm_offset;
   u8 rcd_rc0f_offset;
   u32 xpi_wrdata_all_nib, xpi_oe_all_nib;

   glbl_wrlvl = 0;
   glbl_rd_dbi_en = 0;
   glbl_vref_cal = 0;
   glbl_min_vld_cnt = MIN_VLD_CNT;
   glbl_cal_first_iteration = 0;
   
   set_timing();

   // Setting bit-slice reset masks
   set_data_bs_rst_mask();

   writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
   riu_reg_bcast(INCDEC_CRSE, 0, 0x8);

   if(glbl_bisc_en == 0) {
      // Loading QTR delays in PQTR and NQTR
      glbl_qtr_dly = (glbl_qtr_dly > 400) ? 400 : glbl_qtr_dly; // Ceil it for lower frequencies.

      DEBUG_PRINTF("INFO: set_defaults(): Loading QTR delays in PQTR and NQTR = %d\n", glbl_qtr_dly);
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
      riu_reg_bcast(PQTR, 0, glbl_qtr_dly);  //- Load PQTR with QTR_DLY
      riu_reg_bcast(NQTR, 0, glbl_qtr_dly);  //- Load NQTR with QTR_DLY
   }

   // Enable CS assertion during the command generation
   writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),0xF0);
   writeIO_DMC(ral_addr_of_seq_b_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),0xF0);
   writeIO_DMC(ral_addr_of_seq_c_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),0xF0);

   // Adjust the RIU Read data valid in RPI module
   writeIO_RIU(RIU_R_VALID_ADJ,0x0);

   #ifdef DDR4
      writeIO_DMC(ral_addr_of_seq_cmd_por_in_ddrmc_main_ddrmc_main_bank(BLK), 0x7<<4); // Make the Power on Reset_n as 1, CKE as 1, CS_n as 1 
   #else
      writeIO_DMC(ral_addr_of_seq_cmd_por_in_ddrmc_main_ddrmc_main_bank(BLK), 0x6<<4); // Make the Power on Reset_n as 1, CKE as 1, CS as 0
   #endif

   #ifdef DDR4
   if(glbl_rdimm_en) {
      DEBUG_PRINTF("INFO: set_defaults(): Writing MPR3 for rdimm enable\n");
      for(rank=0; rank < RANKS; rank++) {
         mirror_en = (glbl_xpi_mirror_en & (rank%2 == 1)) ? 1 : 0;

         writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK), rank); // Selects xphy rank
         writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
         writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

         writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (0x10<<rank) | (mirror_en << 1));
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_MODE_REG_WRITE);
         writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
         writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0x3);

         // Write MR3 (Put the side A page 1 in MPR mode).
         glbl_mr3 = (glbl_mr3 | MEM_ENABLE_MPR);
         glbl_mr3 = (glbl_mr3 & ~(0x3)) | 0x1; // MPR - Page 1
         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_mr3);
         writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

         // Writing for Side B. Enable the inversion 
         writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (0x10<<rank) | (mirror_en << 1) | 0x4); 

         // Write MR3 (Put the side B page 0 in MPR mode).
         glbl_mr3 = (glbl_mr3 | MEM_ENABLE_MPR);
         glbl_mr3 = (glbl_mr3 & ~(0x3)) | 0x0; // MPR - Page 0
         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_mr3);
         writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

         // Disable the inversion
         writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (0x10<<rank) | (mirror_en << 1));

         // Write MPR3 Register
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_WRITE);
         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0xAD); // Inverted version of 0x55
         writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0); // MPR0 write
         writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);

         // tWR_MPR wait time (tMOD + AL + PL). tMOD = max(24nCK, 15ns).
         for(loop=0; loop<10; loop++) {
            status = readIO_DMC(ral_addr_of_cal_status_in_ddrmc_main_ddrmc_main_bank(BLK));
         }

         // Write MR3 (Exit from MPR mode).
         glbl_mr3 = (glbl_mr3 & MEM_DISABLE_MPR);
         ddr_mrs_write (rank, 0x3, glbl_mr3);
      } 
   }
   #endif

   if(glbl_cal_mode_bypass == 1) {
      return;
   }

   #ifdef DDR4
   for(rank=0; rank < RANKS; rank++) {
      DEBUG_PRINTF("INFO: ddr_init_f0(): Putting the memory in Vref training mode \n");
      ddr_mrs_write (rank, 0x6, (glbl_mr6 | 0x80));

      DEBUG_PRINTF("INFO: ddr_init_f0(): Writing Default Vref Value \n");
      ddr_mrs_write (rank, 0x6, (glbl_mr6 | 0x80));

      DEBUG_PRINTF("INFO: ddr_init_f0(): Releasing the memory from Vref training mode \n");
      ddr_mrs_write (rank, 0x6, glbl_mr6);
   }
   #endif

   #ifdef LPDDR4
      glbl_lp4_mode_default = 1; // Disable the seq_mode updates during initialization
      writeIO_DMC(ral_addr_of_seq_ck_cal_in_ddrmc_main_ddrmc_main_bank(BLK),0xF0F0); // Drive LP4 slow clock
      writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 4); // Enable LP4 slow clock mode
   #endif

   glbl_mem_read_latency = get_read_latency(); 
   glbl_mem_write_latency = get_write_latency(); 

   #ifdef LPDDR4
      glbl_init_write_latency = glbl_mem_write_latency + 4; // 4 cycle command + 1 tDQSS
   #else
      if(glbl_lrdimm_en) { // LRDIMM case
         // RCD tPDM range is (1.0 -> 1.3)ns plus tCK/2. Picking up the MIN value.
         rcd_tpdm = (1000 + glbl_tck/2);

         // DB tPDM range is (1.37 -> 1.62)ns plus tCK/4. Picking up the MAX value.
         db_tpdm = (1620 + glbl_tck/4);

         // DB tPDM - RCD tPDM. Likely DB tPDM is more than RCD tPDM under worst case conditions
         db_rcd_tpdm_offset = ((db_tpdm - rcd_tpdm) / glbl_tck) + 1; // Floor it

         if((glbl_rcd_rc0f & 0x4) == 0) { // 0/1/2/3 value - 1/2/3/4 nCK latency adder
            rcd_rc0f_offset = (glbl_rcd_rc0f & 0x3) + 1;
         } else {
            rcd_rc0f_offset = 0;
         }

         glbl_init_write_latency = glbl_mem_write_latency + rcd_rc0f_offset - db_rcd_tpdm_offset;
         DEBUG_PRINTF("INFO: set_defaults(): RDIMM write latency case: TCK = %d, rcd_tpdm = %d, db_tpdm = %d\n", glbl_tck, rcd_tpdm, db_tpdm);
         DEBUG_PRINTF("INFO: set_defaults(): RDIMM write latency case: TCK = %d, db_rcd_tpdm_offset = %d, RC0F = %x, rcd_rc0f_offset = %d\n", glbl_tck, db_rcd_tpdm_offset, glbl_rcd_rc0f, rcd_rc0f_offset);

      } else if(glbl_rdimm_en) { // RDIMM case (excluding LRDIMM)
         // RCD tPDM range is (1.0 -> 1.3)ns plus tCK/2. Picking up the MIN value.
         rcd_tpdm_offset = (1000 + glbl_tck/2) / glbl_tck;

         if((glbl_rcd_rc0f & 0x4) == 0) { // 0/1/2/3 value - 1/2/3/4 nCK latency adder
            rcd_rc0f_offset = (glbl_rcd_rc0f & 0x3) + 1;
         } else {
            rcd_rc0f_offset = 0;
         }

         glbl_init_write_latency = glbl_mem_write_latency + rcd_rc0f_offset + rcd_tpdm_offset;
         DEBUG_PRINTF("INFO: set_defaults(): RDIMM write latency case: TCK = %d, rcd_tpdm_offset = %d, RC0F = %x, rcd_rc0f_offset = %d\n", glbl_tck, rcd_tpdm_offset, glbl_rcd_rc0f, rcd_rc0f_offset);

      } else {
         glbl_init_write_latency = glbl_mem_write_latency;
      }
   #endif

   DEBUG_PRINTF("INFO: set_defaults(): Memory write latency = %d, Initial write latency = %d\n", glbl_mem_write_latency, glbl_init_write_latency);

   glbl_write_latency = glbl_init_write_latency;

   xpi_oe_all_nib     = readIO_DMC(ral_addr_of_xpi_oe_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK));
   xpi_wrdata_all_nib = readIO_DMC(ral_addr_of_xpi_wrdata_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK)); 

   glbl_xpi_oe_val = readIO_DMC(ral_addr_of_xpi_oe_in_ddrmc_main_ddrmc_main_bank(BLK));
   DEBUG_PRINTF("INFO: set_defaults(): xpi_oe_all_nib = %x, xpi_wrdata_all_nib = %x, xpi_oe_val = %x\n", xpi_oe_all_nib, xpi_wrdata_all_nib, glbl_xpi_oe_val);
   
   xpi_oe_all_nib     = (xpi_oe_all_nib & ~(0x7F)) | glbl_write_latency;
   xpi_wrdata_all_nib = (xpi_wrdata_all_nib & ~(0x7F)) | glbl_write_latency;

   writeIO_DMC(ral_addr_of_xpi_oe_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK), xpi_oe_all_nib);
   writeIO_DMC(ral_addr_of_xpi_wrdata_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK),  xpi_wrdata_all_nib);

   if(glbl_mem_read_latency <= 12)
      glbl_initial_read_latency = 9;
   else
      glbl_initial_read_latency = glbl_mem_read_latency - 4; // RTL is adding additional latencies, hence setting back by 4 for now.

   fifo_rden_lat = (glbl_mem_read_latency / 2) + 5; // Convert from tCK to mc_clk cycles and add some offset to consider internal timing
   writeIO_DMC(ral_addr_of_fifo_rden_in_ddrmc_main_ddrmc_main_bank(BLK), fifo_rden_lat);

   DEBUG_PRINTF("INFO: set_defaults(): Memory read latency = %d, Initial read latency = %d, FIFO read latency = %d\n", glbl_mem_read_latency, glbl_initial_read_latency, fifo_rden_lat);

   // Dynamic Mode Enable
   DEBUG_PRINTF("INFO: set_defaults(): Disabling dynamic mode for TX\n");
   writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
   riu_reg_bcast(DIS_DYN_MODE, 0, 0x1);

   #ifdef LPDDR4
      if(!(glbl_lp4_mr13 & (0x1 << 5))) { //MR13[5]=0:DM Enabled; MR13[5]=1: DM Disabled  // FIXME: This programming need to be taken care by PMC
         // Enable write DM mode in the cal logic.
         writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1); 
         DEBUG_PRINTF("INFO: set_defaults(): LPDDR4: Enabled the DM in the CAL RTL \n");
      }

      if(glbl_lp4_mr3 & (0x1 << 6)) {
         // Disable the Read DBI in RTL and the Memory; It will get enabled after the Read DBI calibration.
         // Write MPR register to Disable the Read DBI mode into the DRAM.
         for(rank=0; rank<RANKS; rank++) {
            lp4_mode_reg_wr(rank, 3, glbl_lp4_mr3&(~(0x1<<6)));
         }
         writeIO_DMC(ral_addr_of_xpi_read_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0); 
         DEBUG_PRINTF("INFO: set_defaults(): Changing the MR3 to 0x%08X from value 0x%08X \n",glbl_lp4_mr3&(~(0x1<<6)), glbl_lp4_mr3);
         DEBUG_PRINTF("INFO: set_defaults(): LPDDR4: Disabled the Read DBI these will get enabled after the Read DBI calibration phase \n");
      } 

   #else
      if(glbl_mr5[0] & (0x1 << 12)) { // Read DBI enable
         // Disable the Read DBI in RTL and the Memory; It will get enabled after the Read DBI calibration.
         // Write MPR register to Disable the Read DBI mode into the DRAM.
         for(rank=0; rank<RANKS; rank++) {
            set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                                  MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr5[rank] & (~(0x1<<12)) /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x1 /*u16 seq_a_ba*/,
                                  0x30 /*u32 seq_a2a_dly*/);
         } 
         DEBUG_PRINTF("INFO: set_defaults(): DDR4: Disabled the Read DBI these will get enabled after the Read DBI calibration phase \n");
         writeIO_DMC(ral_addr_of_xpi_read_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0); 
      }
   #endif

   // Default DM Setting For LPDDR4
   #ifdef LPDDR4
     writeIO_DMC(ral_addr_of_seq_a_dm_early_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
     writeIO_DMC(ral_addr_of_seq_a_dm_0_in_ddrmc_main_ddrmc_main_bank(BLK),1);
     writeIO_DMC(ral_addr_of_seq_a_dm_1_in_ddrmc_main_ddrmc_main_bank(BLK),1);
     writeIO_DMC(ral_addr_of_seq_a_dm_late_in_ddrmc_main_ddrmc_main_bank(BLK),1);
   #endif

   glbl_lp4_mode_default = 0; // Enable the seq_mode as needed

#ifdef TEST_MPR
   ddr_mrs_write (0, 0x1, glbl_mr1[0]);
   while (1) {
      test_write_read();
   }
#endif
   glbl_iteration_limit=500;
   return;
}

u8 sanity_check_1 () {
   u8 nibble;
   u8 loop;
   u8 rank;
   u32 dq_compare_result;

   // Clear if any stale status
   writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);
   writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

   DEBUG_PRINTF("INFO: sanity_check_1(): Start\n");

   #ifdef DDR4
      for(rank=0; rank<RANKS; rank++) {
         // ACTIVATE Command
         set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, MEM_ROW_ACTIVE /*u32 seq_a_cmd*/, 
                                  0 /*u32 seq_a_addr*/, 0 /*u8 seq_a_bg*/, 0 /*u16 seq_a_ba*/, 0 /*u32 seq_a2a_dly*/);

         // sanity Check 1: write - read alternatively
         for(loop=0; loop<4; loop++) {
            writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), (loop + 9));
            writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), (loop + 9));

            writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), (loop << 3));

            // WRITE Command
            writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_WRITE);

            writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
            while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

            // READ Command
            writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_READ);

            writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
            while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
         }

         dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_nibble_compare_17_0;
         DEBUG_PRINTF("INFO: sanity_check_1(): rank = %d, dq_compare_result = %x \n", rank, dq_compare_result);

         if(dq_compare_result) { // ERROR case
            DEBUG_PRINTF("ERROR: sanity_check_1(): rank = %d, dq_compare_result = %x\n", rank, dq_compare_result);
            write_error_nibble_XRAM(0, dq_compare_result);
            writeIO_XRAM(XRAM_CAL_RANK,(1<<rank | 1 << 4));
            return 1;
         } else {
            DEBUG_PRINTF("INFO: sanity_check_1(): passed for rank = %d\n", rank);
         }

         // PRECHARGE all banks
         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x400);
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_ROW_PREACHARGE);
         writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
         while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
      } // RANKS

   #else
      u32 ca_a;
      u32 ca_b;
      u8 bank_addr = 0;
      u16 col_addr = 0;

      for(rank=0; rank<RANKS; rank++) {
         writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK),rank); // Selects xphy rank
         writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),0x10<<rank); // Selects DDR Mem rank
         writeIO_DMC(ral_addr_of_seq_b_rank_in_ddrmc_main_ddrmc_main_bank(BLK),rank); // Selects xphy rank
         writeIO_DMC(ral_addr_of_seq_b_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),0x10<<rank); // Selects DDR Mem rank

         // Activate bank 0
         lp4_bank_row_access(rank /*u8 rank*/, ACTIVATE /*cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);

         // sanity Check 1: write - read alternatively
         writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 1); //number of sequences - 0: Sequence A enabled; 1: Sequence A, and B enabled; 2: Sequence A, B, and C enabled;
         writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 1); //0: 2 commands per sequence; 1: 4 commands per sequence
         writeIO_DMC(ral_addr_of_seq_b_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 1); //0: 2 commands per sequence; 1: 4 commands per sequence
         writeIO_DMC(ral_addr_of_seq_a_b_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 0x40);
         writeIO_DMC(ral_addr_of_seq_b_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 0x40);
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 0x00010002); // Total command = 2; read command = 1;
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0x085); // Write command
         writeIO_DMC(ral_addr_of_seq_b_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0x145); // Read command

         for(loop=0; loop<4; loop++) {
            writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 2*loop);
            writeIO_DMC(ral_addr_of_seq_a_dq_1_in_ddrmc_main_ddrmc_main_bank(BLK), (2*loop + 1));
            writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), (((2*loop+1) << 5) | (2*loop)));

            col_addr = (loop << 4); // BL16 boundary
            ca_a = ((bank_addr&0x7)<<6) |  ((col_addr&0x200)<<(10-9)) | ((col_addr&0x100)<<(17-8)) | ((col_addr&0xFC)<<(18-2)); 
            ca_b = ca_a;

            // WRITE Command
            ca_a = ca_a | LP4_COL_WRITE;
            writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), ca_a);

            // READ Command
            ca_b = ca_b | LP4_COL_READ;
            writeIO_DMC(ral_addr_of_seq_b_addr_in_ddrmc_main_ddrmc_main_bank(BLK), ca_b);

            writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
            while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
         }
         writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //number of sequences - 0: Sequence A enabled; 1: Sequence A, and B enabled; 2: Sequence A, B, and C enabled;
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);

         dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_nibble_compare_17_0;
         DEBUG_PRINTF("INFO: sanity_check_1(): rank = %d, dq_compare_result = %x \n", rank, dq_compare_result);

         if(dq_compare_result) { // ERROR case
            DEBUG_PRINTF("ERROR: sanity_check_1(): rank = %d, dq_compare_result = %x\n", rank, dq_compare_result);
            write_error_nibble_XRAM(0, dq_compare_result);
            writeIO_XRAM(XRAM_CAL_RANK,(1<<rank | 1 << 4));
            return 1;
         } else {
            DEBUG_PRINTF("INFO: sanity_check_1(): passed for rank = %d\n", rank);
         }

         // PRECHARGE all banks
         lp4_bank_row_access(rank /*u8 rank*/, PRECHARGE /*u8 cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);
      } // RANKS
   #endif

   DEBUG_PRINTF("INFO: sanity_check_1(): Done\n");

   return 0;
}

u8 sanity_check_2 () {
   u8 nibble;
   u8 loop;
   u8 rank;
   u8 slot_a;
   u8 slot_b;
   u16 cnt;
   u32 dq_compare_result;

   // Clear if any stale status
   writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);
   writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

   #ifdef DDR4
      for(rank=0; rank<RANKS; rank++) {
         // ACTIVATE Command
         writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK), rank); // Selects xphy rank
         writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10<<rank); // Selects DDR Mem rank
         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
         writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
         writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_ROW_ACTIVE);

         writeIO_DMC(ral_addr_of_seq_b_rank_in_ddrmc_main_ddrmc_main_bank(BLK), rank); // Selects xphy rank
         writeIO_DMC(ral_addr_of_seq_b_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10<<rank); // Selects DDR Mem rank
         writeIO_DMC(ral_addr_of_seq_b_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
         writeIO_DMC(ral_addr_of_seq_b_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
         writeIO_DMC(ral_addr_of_seq_b_bg_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);
         writeIO_DMC(ral_addr_of_seq_b_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_ROW_ACTIVE);

         writeIO_DMC(ral_addr_of_seq_a_b_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 8);
         writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 2);

         writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
         while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x40);
         writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0); // Bank group = 0
         writeIO_DMC(ral_addr_of_seq_b_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x80);
         writeIO_DMC(ral_addr_of_seq_b_bg_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1); // Bank group = 1

         for(loop=7; loop>=4; loop--) { // tCCD from 7 to 4

            if((glbl_wr_pre_2tck_mode | glbl_rd_pre_2tck_mode) & (loop == 5)) // tCCD=5 is not supported in 2 tCK preamble mode
               continue;

            writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), loop+4);
            writeIO_DMC(ral_addr_of_seq_b_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), loop+4);
            writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), loop+4);

            slot_a = 0;
            writeIO_DMC(ral_addr_of_seq_a_b_dly_in_ddrmc_main_ddrmc_main_bank(BLK), loop-2);

            if(loop%2) { // Odd tCK cycle gap
               slot_b = 1;
               writeIO_DMC(ral_addr_of_seq_b_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), loop-2+2); // Add extra DMC_CLK
            } else { // Even tCK cycle gap
               slot_b = 0;
               writeIO_DMC(ral_addr_of_seq_b_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), loop-2);
            }

            writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (slot_a << 9) | (0x10<<rank));
            writeIO_DMC(ral_addr_of_seq_b_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (slot_b << 9) | (0x10<<rank));

            // WRITE Commands
            writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_WRITE);
            writeIO_DMC(ral_addr_of_seq_b_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_WRITE);
            writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 16);
            writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
            while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

            // READ Commands
            writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_READ);
            writeIO_DMC(ral_addr_of_seq_b_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_READ);
            writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), ((16 << 16) | 16));
            writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
            while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
         }

         writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10<<rank);
         writeIO_DMC(ral_addr_of_seq_b_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10<<rank);
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
         writeIO_DMC(ral_addr_of_seq_a_b_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 2);
         writeIO_DMC(ral_addr_of_seq_b_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 2);
         writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0); // Sequence A only

         dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_nibble_compare_17_0;
         DEBUG_PRINTF("INFO: sanity_check_2(): rank = %d, dq_compare_result = %x \n", rank, dq_compare_result);

         if(dq_compare_result) { // ERROR case
            DEBUG_PRINTF("ERROR: sanity_check_2(): rank = %d, dq_compare_result = %x\n", rank, dq_compare_result);
            write_error_nibble_XRAM(0, dq_compare_result);
            writeIO_XRAM(XRAM_CAL_RANK,(1<<rank | 1 << 4));
            return 1;
         } else {
            DEBUG_PRINTF("INFO: sanity_check_2(): passed for rank = %d\n", rank);
         }

         // PRECHARGE all banks
         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x400);
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_ROW_PREACHARGE);
         writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
         while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
      } // RANKS

   #else
      u32 ca;
      u8 bank_addr = 0;
      u16 col_addr = 0;

      for(rank=0; rank<RANKS; rank++) {
         writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK),rank); // Selects xphy rank
         writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),0x10<<rank); // Selects DDR Mem rank

         // Activate bank 0
         lp4_bank_row_access(rank /*u8 rank*/, ACTIVATE /*cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);

         // Sanity Check 2: write once, read burst with variable gap
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0x085); // Write command
         writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 1); //0: 2 commands per sequence; 1: 4 commands per sequence

         writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 0x9);
         writeIO_DMC(ral_addr_of_seq_a_dq_1_in_ddrmc_main_ddrmc_main_bank(BLK), 0xA);
         writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), ((0xA << 5) | 0x9));

         col_addr = 0x10; // BL16 boundary
         ca = ((bank_addr&0x7)<<6) |  ((col_addr&0x200)<<(10-9)) | ((col_addr&0x100)<<(17-8)) | ((col_addr&0xFC)<<(18-2)); 

         // WRITE Command
         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), (ca | LP4_COL_WRITE));
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0x085); // Write command

         writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
         while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

         // READ Command
         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), (ca | LP4_COL_READ));
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0x145); // Read command

         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), ((16 << 16) | 16)); // burst 16 

         for(loop=0; loop<4; loop++) {
            writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), loop+2);
            writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
            while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
         }
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
         writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 2);

         dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_nibble_compare_17_0;
         DEBUG_PRINTF("INFO: sanity_check_2(): rank = %d, dq_compare_result = %x \n", rank, dq_compare_result);

         if(dq_compare_result) { // ERROR case
            DEBUG_PRINTF("ERROR: sanity_check_2(): rank = %d, dq_compare_result = %x\n", rank, dq_compare_result);
            write_error_nibble_XRAM(0, dq_compare_result);
            writeIO_XRAM(XRAM_CAL_RANK,(1<<rank | 1 << 4));
            return 1;
         } else {
            DEBUG_PRINTF("INFO: sanity_check_2(): passed for rank = %d\n", rank);
         }

         // Sanity Check 2: write once, read back to back with variable length
         // Write was done and command/address were are already set in Sanity 2 stage.
         for(loop=0; loop<4; loop++) {
            cnt = 2 * (1 << loop);
            writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), ((cnt << 16) | cnt));
            writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
            while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

            cnt = (2 * (1 << loop)) + 1;
            writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), ((cnt << 16) | cnt));
            writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
            while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
         }
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

         dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_nibble_compare_17_0;
         DEBUG_PRINTF("INFO: sanity_check_2(): rank = %d, dq_compare_result = %x \n", rank, dq_compare_result);

         if(dq_compare_result) { // ERROR case
            DEBUG_PRINTF("ERROR: sanity_check_2(): rank = %d, dq_compare_result = %x\n", rank, dq_compare_result);
            write_error_nibble_XRAM(0, dq_compare_result);
            writeIO_XRAM(XRAM_CAL_RANK,(1<<rank | 1 << 4));
            return 1;
         } else {
            DEBUG_PRINTF("INFO: sanity_check_2(): passed for rank = %d\n", rank);
         }

         // PRECHARGE all banks
         lp4_bank_row_access(rank /*u8 rank*/, PRECHARGE /*u8 cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);
      } // RANKS

   #endif

   return 0;
}

u8 sanity_check_ddr () {
   u8 nibble;
   u8 rank;
   u8 loop;
   u8 status;
   u8 rd_error;
   u32 rd_data, exp_data;
   static u32 mem_data[DQS_BITS*NIBBLE_PER_DQS];
   u32 dq_compare_result;

   // Clear if any stale status
   writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);
   writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

   #ifdef DDR4
      // Disable read DBI as it is not operational during MPR reads
      writeIO_DMC(ral_addr_of_xpi_read_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0); 

      // MPR Read check
      for(rank = 0; rank < RANKS; rank++) {
         // Turn on MPR mode in memory
         set_1_seq_command_at_mem(rank, 1, MEM_MODE_REG_WRITE, (glbl_mr3 | MEM_ENABLE_MPR), 0, 3, 0);

         // Set the expected data, read command, address and issue reads
         writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0); // 0xAAAA
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK),MEM_COL_READ);
         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK),0); // MPR0
         writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
         writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

         writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
         while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

         // Turn off MPR mode in memory
         set_1_seq_command_at_mem(rank, 1, MEM_MODE_REG_WRITE, (glbl_mr3 & MEM_DISABLE_MPR), 0, 3, 0);

         dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_nibble_compare_17_0;
         DEBUG_PRINTF("INFO: sanity_check_ddr(): rank = %d, dq_compare_result = %x \n", rank, dq_compare_result);

         if(dq_compare_result) { // ERROR case
            DEBUG_PRINTF("ERROR: sanity_check_ddr(): rank = %d, dq_compare_result = %x\n", rank, dq_compare_result);
            write_error_nibble_XRAM(0, dq_compare_result);
            return 1;
         } else {
            DEBUG_PRINTF("INFO: sanity_check_ddr(): MPR Read check passed for rank = %d\n", rank);
         }
      }

      // Revert back the read DBI settings
      writeIO_DMC(ral_addr_of_xpi_read_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_xpi_read_dbi); 

      // Write-Read sanity check
      // Set the DQ pattern
      writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 11);
      writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), 11);

      for(rank=0; rank<RANKS; rank++) {
         if(glbl_cal_done) { // Issue refreshes during mission mode to maintain the refresh rate to DRAM
            status = banks_refresh(0, 0x20 | rank);
            if(status) {
               return 1;
            }
         }

         writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK), rank); // Selects xphy rank
         writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10<<rank); // Selects DDR Mem rank
         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
         writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
         writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

         // ACTIVATE Command
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_ROW_ACTIVE);
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

         writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
         while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

         // READ Command for memory content retention
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_READ);
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), ((1 << 16) | 1));

         for(loop=0; loop<5; loop++) {
            writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
            while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

            dq_compare_result = 0;
            for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
               rd_data = readIO_DMC(ral_addr_of_bl8_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (nibble * 4));

               if(loop == 0) { // Save the read data for the first time
                  writeIO_DMC(ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (nibble * 4), rd_data);
                  mem_data[nibble] = rd_data;
                  DEBUG_PRINTF("INFO: sanity_check_ddr(): DRAM content read data: rank = %d, nibble = %d, read data = %x\n", rank, nibble, rd_data);

               } else { // Compare the read data for every other time
                  exp_data = mem_data[nibble];

                  if(rd_data != exp_data) {
                     DEBUG_PRINTF("ERROR: sanity_check_ddr(): rank = %d, nibble = %d, loop = %d, expected data = %x, read data = %x\n", rank, nibble, loop, exp_data, rd_data);
                     dq_compare_result |= (0x1 << nibble);
                  }
               }
            }
            if(dq_compare_result) {
               write_error_nibble_XRAM(0, dq_compare_result);
               return 1;
            }
         }
         DEBUG_PRINTF("INFO: sanity_check_ddr(): DRAM content read check passed for rank = %d\n", rank);

         // Clear if any stale status
         writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);
         writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

         // Sanity WRITE Command
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_WRITE);
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

         writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
         while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

         // Sanity READ Command
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_READ);
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), ((1 << 16) | 1));

         writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
         while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

         dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_nibble_compare_17_0;
         DEBUG_PRINTF("INFO: sanity_check_ddr(): rank = %d, dq_compare_result = %x \n", rank, dq_compare_result);

         if(dq_compare_result) { // ERROR case
            DEBUG_PRINTF("ERROR: sanity_check_ddr(): rank = %d, dq_compare_result = %x\n", rank, dq_compare_result);
            write_error_nibble_XRAM(0, dq_compare_result);
            return 1;
         } else {
            DEBUG_PRINTF("INFO: sanity_check_ddr(): write-read check passed for rank = %d\n", rank);
         }

         // WRITE Command for memory content retention
         // Select per nibble values
         writeIO_DMC(ral_addr_of_seq_dq_nibble_sel_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);

         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_WRITE);
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

         writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
         while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

         // De-select per nibble values
         writeIO_DMC(ral_addr_of_seq_dq_nibble_sel_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

         // READ Command for mmeory content retention
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_COL_READ);
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), ((1 << 16) | 1));

         writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
         while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

         dq_compare_result = 0;
         for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
            rd_data = readIO_DMC(ral_addr_of_bl8_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (nibble * 4));
            exp_data = mem_data[nibble];

            if(rd_data != exp_data) {
               DEBUG_PRINTF("ERROR: sanity_check_ddr(): rank = %d, nibble = %d, expected data = %x, read data = %x\n", rank, nibble, exp_data, rd_data);
               dq_compare_result |= (0x1 << nibble);
            }
         }
         if(dq_compare_result) {
            write_error_nibble_XRAM(0, dq_compare_result);
            return 1;
         }
         DEBUG_PRINTF("INFO: sanity_check_ddr(): DRAM content write-read check passed for rank = %d\n", rank);

         // PRECHARGE all banks
         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x400);
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_ROW_PREACHARGE);
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

         writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
         while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
      } // RANKS

   #else
      // Disable write & read DBI as it is not operational for MPC writes
      writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
      writeIO_DMC(ral_addr_of_xpi_read_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0); 

      // MPR Read check
      // Set MPC read pattern and compare pattern
      writeIO_DMC(ral_addr_of_seq_dq_pattern0_in_ddrmc_main_ddrmc_main_bank(BLK),0xF0F0F0F0); // 0xC3A5
      writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), 18); // 0xC3A5 

      for(rank = 0; rank < RANKS; rank++) {
         // Program the MPR pattern in the memory
         lp4_mode_reg_wr(rank, 32, 0xAA); // Set mode register with the expected pattern for first half of the bl16
         lp4_mode_reg_wr(rank, 40, 0xAA); // Set the mode register with the expected pattern for second half of the bl16
         lp4_mode_reg_wr(rank, 15, 0x00); // Disable inversion of the data bits pattern of dq[7:0]
         lp4_mode_reg_wr(rank, 20, 0x00); // Disable inversion of the data bits pattern of dq[15:8] 

         // Set CAL RTL to consider the commands as read commands.
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0x105);
         // Set MPC Read DQ CAL command
         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), LP4_MPC_RD_DQ);

         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), (1 << 16) | 1);
         writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
         while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

         dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_nibble_compare_17_0;
         DEBUG_PRINTF("INFO: sanity_check_ddr(): rank = %d, dq_compare_result = %x \n", rank, dq_compare_result);

         if(dq_compare_result) { // ERROR case
            DEBUG_PRINTF("ERROR: sanity_check_ddr(): rank = %d, dq_compare_result = %x\n", rank, dq_compare_result);
            write_error_nibble_XRAM(0, dq_compare_result);
            return 1;
         } else {
            DEBUG_PRINTF("INFO: sanity_check_ddr(): MPR Read check passed for rank = %d\n", rank);
         }
      }

      // FIFO based Write-Read sanity check
      // Set the DQ pattern
      writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 0x9);
      writeIO_DMC(ral_addr_of_seq_a_dq_1_in_ddrmc_main_ddrmc_main_bank(BLK), 0xA);
      writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), ((0xA << 5) | 0x9));

      for(rank=0; rank<RANKS; rank++) {
         DEBUG_PRINTF("INFO: sanity_check_ddr(): FIFO based write-read check for rank = %d\n", rank);
         lp4_mpc_wr(rank, LP4_MPC_WR_FIFO, 1, 1);

         lp4_mpc_wr(rank, LP4_MPC_RD_FIFO, 1, 2);

         dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_nibble_compare_17_0;
         DEBUG_PRINTF("INFO: sanity_check_ddr(): rank = %d, dq_compare_result = %x \n", rank, dq_compare_result);

         if(dq_compare_result) { // ERROR case
            DEBUG_PRINTF("ERROR: sanity_check_ddr(): rank = %d, dq_compare_result = %x\n", rank, dq_compare_result);
            write_error_nibble_XRAM(0, dq_compare_result);
            return 1;
         } else {
            DEBUG_PRINTF("INFO: sanity_check_ddr(): FIFO based write-read check passed for rank = %d\n", rank);
         }
      }

      // Revert back the write & read DBI settings
      writeIO_DMC(ral_addr_of_xpi_write_dm_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_xpi_write_dm_dbi);
      writeIO_DMC(ral_addr_of_xpi_read_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_xpi_read_dbi); 

//      u8 bank_addr = 0;
//      u16 col_addr = 0x10; // BL16 boundary
//      u32 ca = ((bank_addr&0x7)<<6) |  ((col_addr&0x200)<<(10-9)) | ((col_addr&0x100)<<(17-8)) | ((col_addr&0xFC)<<(18-2)); 
//
//      // Write-Read sanity check
//      // Set the DQ pattern
//      writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 0x9);
//      writeIO_DMC(ral_addr_of_seq_a_dq_1_in_ddrmc_main_ddrmc_main_bank(BLK), 0xA);
//      writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), ((0xA << 5) | 0x9));
//
//      for(rank=0; rank<RANKS; rank++) {
//         // Activate bank 0
//         lp4_bank_row_access(rank /*u8 rank*/, ACTIVATE /*cmd_type*/, bank_addr /*u8 bank_addr*/, 0 /*u16 row_addr*/);
//
//         // WRITE Command
//         writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 1); //0: 2 commands per sequence; 1: 4 commands per sequence
//         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), (ca | LP4_COL_WRITE));
//         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0x085); // Write command
//         writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 4);
//         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
//
//         writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
//         while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
//
//         // READ Command
//         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), (ca | LP4_COL_READ));
//         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0x145); // Read command
//         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), ((1 << 16) | 1));
//
//         writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
//         while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
//
//         // PRECHARGE all banks
//         lp4_bank_row_access(rank /*u8 rank*/, PRECHARGE /*u8 cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);
//
//         dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_nibble_compare_17_0;
//         DEBUG_PRINTF("INFO: sanity_check_ddr(): rank = %d, dq_compare_result = %x \n", rank, dq_compare_result);
//
//         if(dq_compare_result) { // ERROR case
//            DEBUG_PRINTF("ERROR: sanity_check_ddr(): rank = %d, dq_compare_result = %x\n", rank, dq_compare_result);
//            write_error_nibble_XRAM(0, dq_compare_result);
//            return 1;
//         } else {
//            DEBUG_PRINTF("INFO: sanity_check_ddr(): write-read check passed for rank = %d\n", rank);
//         }
//      } // RANKS
   #endif

   status = banks_refresh(0, 0x24);
   if(status) {
      return 1;
   }
   return 0;
}

u8 ddr_self_calibrate(u8 power_on_reset){
   u8 loop;
   u8 rank;
   u8 nibble;
   u8 phy_nibble;
   u8 fixdly_rdy;
   u8 self_calibrate;
   u16 calib_ctrl;
   u16 nibble_ctrl0;
   u32 wait_cnt = 0;
   u16 pqtr, pqtr_align;
   u16 nqtr, nqtr_align;
   u16 align;
   u16 rl_dly_qtr;
   u32 t_txbit;
   u32 WATCH_DOG_COUNT = 5000; // maximum possible time from self_calibrate to fixdly_rdy from XPHY

   if(glbl_bisc_en == 0) {
      DEBUG_PRINTF("INFO: ddr_self_calibrate(): Skipping self calibration as BISC_EN is 0\n");
      glbl_rl_dly_qtr_max = glbl_qtr_dly;
      DEBUG_PRINTF("INFO: ddr_self_calibrate(): rl_dly_qtr = %d for all the nibbles\n", glbl_qtr_dly);
      for(phy_nibble=0; phy_nibble<27; phy_nibble++) {
         glbl_rl_dly_qtr[phy_nibble] = glbl_qtr_dly;
         write_riu_nibble_reg(phy_nibble, RL_DLY_QTR, glbl_qtr_dly);
      }
   } else {

      DEBUG_PRINTF("INFO: ddr_self_calibrate(): rl_dly_qtr = %d for all the non DQ nibbles\n", glbl_qtr_dly);
      for(phy_nibble=0; phy_nibble<27; phy_nibble++) {
         if((glbl_dq_nibble_mask_initial >> phy_nibble) & 0x1) {
            glbl_rl_dly_qtr[phy_nibble] = glbl_qtr_dly;
            write_riu_nibble_reg(phy_nibble, RL_DLY_QTR, glbl_qtr_dly);
         }
      }

      if(power_on_reset == 0) {
         if(glbl_cal_done == 1) {
            DEBUG_PRINTF("INFO: ddr_self_calibrate(): Clear BISC pause\n");
            writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
            riu_reg_bcast(WL_TRAIN, 0, (0x0 << BISC_PAUSE_POS));

         } else {
            for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
               writeIO_DMC((ral_addr_of_phy_data_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (nibble * 4)), 0x0);
               writeIO_DMC((ral_addr_of_phy_oe_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (glbl_map_riu[nibble] * 4)), 0x0);
            }
            #ifdef LPDDR4
               writeIO_DMC(ral_addr_of_xpi_map_bitslip_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
            #endif
         }

	 // Clear out CAL latencies and XPHY delay settings during frequency switching in calibration
         DEBUG_PRINTF("INFO: ddr_self_calibrate(): Clearing XPHY delays \n");
         #ifdef LPDDR4
            writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~(glbl_dq_nibble_mask_initial & glbl_cs_nibble_mask_initial & glbl_ca_nibble_mask_initial));
            riu_reg_bcast(LP4_WL_DQS, 1, 0x0); 
         #else
            writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
         #endif

         for(rank=0; rank<4; rank++) {
            riu_reg_bcast(WLDLYRNK0 + rank, 1, 0x0); 
            if(rank == 3) // Set update for the last rank which results in updating all the ranks
               riu_reg_bcast(RLDLYRNK0 + rank, 1, 0x8000); 
            else
               riu_reg_bcast(RLDLYRNK0 + rank, 1, 0x0); 
         }

         // It is required set incdec_crse to non-zero value to avoid BISC hang issue
         riu_reg_bcast(IODLY_BCAST_MASK, 1, 0x0); 
         riu_reg_bcast(INCDEC_CRSE, 1, 0x8);

         for(loop=0; loop<6; loop++) {
            riu_reg_bcast(ODLY0 + loop, 1, 0x0); 
            riu_reg_bcast(IDLY0 + loop, 1, 0x0); 
            riu_reg_bcast(IDLY0_ALIGN + loop, 1, 0x0); 
         }
         riu_reg_bcast(TRISTATE_ODLY, 1, 0x0);
         riu_reg_bcast(PQTR, 1, 0x0);
         riu_reg_bcast(NQTR, 1, 0x0);
         riu_reg_bcast(PQTR_ALIGN, 1, 0x0);
         riu_reg_bcast(NQTR_ALIGN, 1, 0x0);
         riu_reg_bcast(MON_ALIGN, 1, 0x0);

         DEBUG_PRINTF("INFO: ddr_self_calibrate(): Set the self_calibrate enable\n");
         for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
            phy_nibble = glbl_map_riu[nibble];
            calib_ctrl = read_riu_nibble_reg(phy_nibble, CFG_CALIB_CTRL);
            calib_ctrl |= 0x5F9; // [10], [8:3], [0]: recalibrate_en & self_calibrate
            write_riu_nibble_reg(phy_nibble, CFG_CALIB_CTRL, calib_ctrl);
         }

         if(glbl_cal_done == 0) {
            DEBUG_PRINTF("INFO: ddr_self_calibrate(): Reset the BISC\n");
            writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
            #ifdef LPDDR4
               bitslice_reset_bcast(1, (0x10 | BS_RESET_WT)); // Set & Clear BISC Reset BS reset and BST reset
            #else
               bitslice_reset_bcast(0, (0x10 | BS_RESET_WT)); // Set & Clear BISC Reset BS reset and BST reset
            #endif
         }
      }

      DEBUG_PRINTF("INFO: ddr_self_calibrate(): Polling for fixdly_rdy: Start\n");
      glbl_rl_dly_qtr_max = 0;

      wait_cnt = 0;
      glbl_error_phy_nibble = 0;
      for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
         phy_nibble = glbl_map_riu[nibble];
         do {
            calib_ctrl = read_riu_nibble_reg(phy_nibble, CFG_CALIB_CTRL);
            fixdly_rdy = (calib_ctrl >> BISC_FIXDLY_RDY_POS) & 0x1;
            self_calibrate = (calib_ctrl >> SELF_CALIBRATE_POS) & 0x1;
            DEBUG_PRINTF("INFO: ddr_self_calibrate(): Polling for self_calibrate = %d, fixdly_rdy = %d for physical nibble = %d. wait_cnt = %d \n", self_calibrate, fixdly_rdy, phy_nibble, wait_cnt);
            wait_cnt++;
         } while ((fixdly_rdy != 1) && (wait_cnt < WATCH_DOG_COUNT));

         if(fixdly_rdy != 1) {
            DEBUG_PRINTF("ERROR: ddr_self_calibrate(): Timeout ERROR waiting for FIXDLY_RDY for nibble = %d\n", phy_nibble);
            DEBUG_PRINTF("ERROR: ddr_self_calibrate(): Possibly it could be due to BISC overflow/underflow errors for nibble = %d\n", phy_nibble);
            glbl_error_phy_nibble |= (0x1 << phy_nibble);
         }

         DEBUG_PRINTF("INFO: ddr_self_calibrate(): fixdly_rdy is asserted for nibble = %d. wait_cnt = %d \n", phy_nibble, wait_cnt);
         calib_ctrl &= ~(0x5F8); // [10], [8:3]: recalibrate_en 
         write_riu_nibble_reg(phy_nibble, CFG_CALIB_CTRL, calib_ctrl);
      }
      DEBUG_PRINTF("INFO: ddr_self_calibrate(): Polling for fixdly_rdy: Done\n");

      if(glbl_error_phy_nibble != 0) {
         write_error_nibble_XRAM(1, glbl_error_phy_nibble);
         if(glbl_cal_done == 0) {
            write_error_XRAM(ERR_CODE_3_XPHY_BISC_TIMEOUT);
         } else if(glbl_restore_cal_en == 1) {
            write_error_XRAM(ERR_CODE_112_RESTORE_CAL_XPHY_BISC_TIMEOUT);
         } else if(glbl_freq_switch == 1) {
            write_error_XRAM(ERR_CODE_99_FREQ_SWITCH_XPHY_BISC_TIMEOUT);
         }
         return 1;
      }

      glbl_error_phy_nibble = 0;
      for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
         phy_nibble = glbl_map_riu[nibble];

         // Derive RL_DLY_QTR value
         DEBUG_PRINTF("INFO: ddr_self_calibrate(): deriving rl_dly_qtr for nibble = %d. wait_cnt = %d \n", phy_nibble, wait_cnt);
         pqtr = read_riu_nibble_reg(phy_nibble, PQTR);
         writeIO_XRAM(XRAM_BISC_PQTR + nibble*4, pqtr);
         pqtr_align = read_riu_nibble_reg(phy_nibble, PQTR_ALIGN);
         writeIO_XRAM(XRAM_BISC_PQTR_ALIGN + nibble*4, pqtr_align);

         nqtr = read_riu_nibble_reg(phy_nibble, NQTR);
         writeIO_XRAM(XRAM_BISC_NQTR + nibble*4, nqtr);
         nqtr_align = read_riu_nibble_reg(phy_nibble, NQTR_ALIGN);
         writeIO_XRAM(XRAM_BISC_NQTR_ALIGN + nibble*4, nqtr_align);

         rl_dly_qtr = ((pqtr - pqtr_align) + (nqtr - nqtr_align)) / 2;
         glbl_rl_dly_qtr[phy_nibble] = rl_dly_qtr;
         writeIO_XRAM(XRAM_BISC_RL_DLY_QTR + nibble*4, rl_dly_qtr);

         if(glbl_cal_done == 0) { // Calibration BISC RL_DLY_QTR values
            writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_CALBISC_RL_DLY_QTR + nibble*4, rl_dly_qtr);
            writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_CALBISC_RL_DLY_PQTR + nibble*4, (pqtr - pqtr_align));
            writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_CALBISC_RL_DLY_NQTR + nibble*4, (nqtr - nqtr_align));
         }
         
         align = read_riu_nibble_reg(phy_nibble, MON_ALIGN);
         writeIO_XRAM(XRAM_BISC_MON_ALIGN + nibble*4, align);
         align = read_riu_nibble_reg(phy_nibble, IDLY0_ALIGN);
         writeIO_XRAM(XRAM_BISC_IDLY0_ALIGN + nibble*4, align);
         align = read_riu_nibble_reg(phy_nibble, IDLY1_ALIGN);
         writeIO_XRAM(XRAM_BISC_IDLY1_ALIGN + nibble*4, align);
         align = read_riu_nibble_reg(phy_nibble, IDLY2_ALIGN);
         writeIO_XRAM(XRAM_BISC_IDLY2_ALIGN + nibble*4, align);
         align = read_riu_nibble_reg(phy_nibble, IDLY3_ALIGN);
         writeIO_XRAM(XRAM_BISC_IDLY3_ALIGN + nibble*4, align);
         align = read_riu_nibble_reg(phy_nibble, IDLY4_ALIGN);
         writeIO_XRAM(XRAM_BISC_IDLY4_ALIGN + nibble*4, align);
         align = read_riu_nibble_reg(phy_nibble, IDLY5_ALIGN);
         writeIO_XRAM(XRAM_BISC_IDLY5_ALIGN + nibble*4, align);

         if(rl_dly_qtr == 0) {
            DEBUG_PRINTF("ERROR: ddr_self_calibrate(): nibble = %d, physical nibble = %d, pqtr = %x, pqtr_align = %x, nqtr = %x, nqtr_align = %x, rl_dly_qtr = %x\n", nibble, phy_nibble, pqtr, pqtr_align, nqtr, nqtr_align, rl_dly_qtr);
            glbl_error_phy_nibble |= (0x1 << phy_nibble);
         }

         if(rl_dly_qtr > glbl_rl_dly_qtr_max)
            glbl_rl_dly_qtr_max = rl_dly_qtr;

         DEBUG_PRINTF("INFO: ddr_self_calibrate(): nibble = %d, physical nibble = %d, pqtr = %x, pqtr_align = %x, nqtr = %x, nqtr_align = %x, rl_dly_qtr = %x\n", nibble, phy_nibble, pqtr, pqtr_align, nqtr, nqtr_align, rl_dly_qtr);

         write_riu_nibble_reg(phy_nibble, RL_DLY_QTR, rl_dly_qtr);
      }
      if(glbl_error_phy_nibble != 0) {
         write_error_nibble_XRAM(1, glbl_error_phy_nibble);
         if(glbl_cal_done == 0) {
            write_error_XRAM(ERR_CODE_4_XPHY_BISC_RLDLYQTR_ZERO);
         } else if(glbl_restore_cal_en == 1) {
            write_error_XRAM(ERR_CODE_113_RESTORE_CAL_XPHY_BISC_RLDLYQTR_ZERO);
         } else if(glbl_freq_switch == 1) {
            write_error_XRAM(ERR_CODE_100_FREQ_SWITCH_XPHY_BISC_RLDLYQTR_ZERO);
         }
         return 1;
      }
   }

   glbl_max_limit_90_degree = glbl_rl_dly_qtr_max + MIN_VLD_CNT + FINE_CRS_STEP_SIZE;
   glbl_max_limit_180_degree = (2*glbl_rl_dly_qtr_max) + MIN_VLD_CNT + FINE_CRS_STEP_SIZE;
   DEBUG_PRINTF("INFO: ddr_self_calibrate(): dly_qtr = %x \n", glbl_rl_dly_qtr_max);
   DEBUG_PRINTF("INFO: ddr_self_calibrate(): max limit 90 degree = %x, max limit 180 degree = %x \n", glbl_max_limit_90_degree, glbl_max_limit_180_degree);

   if(power_on_reset) {
      DEBUG_PRINTF("INFO: ddr_self_calibrate(): Clearing t_txbit_outputs_off to enable the IOs\n");
      t_txbit = readIO_DMC(ral_addr_of_t_txbit_in_ddrmc_main_ddrmc_main_bank(BLK));
      t_txbit &= ~0x1;
      writeIO_DMC(ral_addr_of_t_txbit_in_ddrmc_main_ddrmc_main_bank(BLK), t_txbit);
   }
   //#ifdef DDR4
   //migration from one device to another. Gated by MIGRATION_ENABLE define variavle
   compensate_migration_skews();
   //#endif
   return 0;
}

u8 ddr_en_vtc(){
   u8 nibble;
   u8 phy_nibble;
   u8 status;
   u16 calib_ctrl;
   u16 rl_dly_qtr;
   u16 phy_rdy;
   u32 wait_cnt;
   u32 WATCH_DOG_COUNT = 50000; // maximum possible time from en_vtc to phy_rdy from XPHY

   if(glbl_cal_done == 0) {
      DEBUG_PRINTF("INFO: ddr_en_vtc(): Save/Restore - Save values\n");
      ddr_save_val();
   }

   if(glbl_bisc_en == 0) {
      DEBUG_PRINTF("INFO: ddr_en_vtc(): Skipping en_vtc as BISC_EN is 0\n");
      return 0;
   } else {

      #ifdef LPDDR4
      DEBUG_PRINTF("INFO: ddr_en_vtc(): Disabling VT tracking on ODELAYs\n");
      for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
         calib_ctrl = read_riu_nibble_reg(glbl_map_riu[nibble], CFG_CALIB_CTRL);
         calib_ctrl = calib_ctrl | 0x4; // dis_vttrack_obit
         write_riu_nibble_reg(glbl_map_riu[nibble], CFG_CALIB_CTRL, calib_ctrl);
      }
      #endif
     
      DEBUG_PRINTF("INFO: ddr_en_vtc(): Setting en_vtc\n");
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
      riu_reg_bcast(WL_TRAIN, 0, (0x1 << WL_TRAIN_EN_VTC_POS));

      // Polling for phy_rdy is not needed actually. Adding it to clear RL_DLY_RATIO.
      DEBUG_PRINTF("INFO: ddr_en_vtc(): Polling for phy_rdy: Start\n");
      glbl_error_phy_nibble = 0;
      wait_cnt = 0;
      for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
         phy_nibble = glbl_map_riu[nibble];
         do {
            calib_ctrl = read_riu_nibble_reg(phy_nibble, CFG_CALIB_CTRL);
            phy_rdy = (calib_ctrl >> BISC_PHY_RDY_POS) & 0x1;
            DEBUG_PRINTF("INFO: ddr_en_vtc(): Polling for phy_rdy = %d for physical nibble = %d. wait_cnt = %d \n", phy_rdy, phy_nibble, wait_cnt);
            wait_cnt++;
         } while ((phy_rdy != 1) && (wait_cnt < WATCH_DOG_COUNT));

         if(phy_rdy != 1) {
            DEBUG_PRINTF("ERROR: ddr_en_vtc(): Timeout ERROR waiting for PHY_RDY for nibble = %d\n", phy_nibble);
            glbl_error_phy_nibble |= (0x1 << phy_nibble);
         }
      }
      if(glbl_error_phy_nibble != 0) {
         write_error_nibble_XRAM(1, glbl_error_phy_nibble);
         if(glbl_cal_done == 0) {
            write_error_XRAM(ERR_CODE_108_EN_VTC_FAIL_PHY_RDY_TIMEOUT);
         } else if(glbl_restore_cal_en == 1) {
            write_error_XRAM(ERR_CODE_115_RESTORE_CAL_EN_VTC_FAIL_PHY_RDY_TIMEOUT);
         } else if(glbl_freq_switch == 1) {
            write_error_XRAM(ERR_CODE_109_FREQ_SWITCH_EN_VTC_FAIL_PHY_RDY_TIMEOUT);
         }
         return 1;
      }
      DEBUG_PRINTF("INFO: ddr_en_vtc(): Polling for phy_rdy: Done\n");

      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dqs_nibble_mask_initial);
      riu_reg_bcast(RL_DLY_RATIO, 0, 0x0);  // Avoid BISC VT Tracking for RL_DLY_QTR
      riu_reg_bcast(RD_DBG_RW_INDEX, 0, 0xC);  // Select HALFT_DQSM register in the Debug bus

      for(nibble=0; nibble<DQS_BITS; nibble++) {
         phy_nibble = glbl_dqs_bits_riu_map[nibble][0];
         rl_dly_qtr = read_riu_nibble_reg(phy_nibble, RL_DLY_QTR);
         rl_dly_qtr = rl_dly_qtr & ~(0x1); // Make it EVEN number
         write_riu_nibble_reg(phy_nibble, RL_DLY_QTR, rl_dly_qtr);
      }

      if(glbl_cal_done == 0) {
         status = sanity_check_1();
         if(status != 0) {
            DEBUG_PRINTF("ERROR: ddr_en_vtc(): Sanity check 1 Failed\n");
            if(glbl_cal_done == 0) {
               write_error_XRAM(ERR_CODE_76_EN_VTC_FAIL_POST_SANITY_CHECK_1);
            } else if(glbl_restore_cal_en == 1) {
               write_error_XRAM(ERR_CODE_114_RESTORE_CAL_EN_VTC_FAIL_POST_SANITY_CHECK_1);
            } else if(glbl_freq_switch == 1) {
               write_error_XRAM(ERR_CODE_101_FREQ_SWITCH_EN_VTC_FAIL_POST_SANITY_CHECK_1);
            }
            return 1;
         } else {
            DEBUG_PRINTF("INFO: ddr_en_vtc(): Sanity check 1 Passed\n");
         }
      }
   }

   return 0;
}

u8 ddr_read_dqs_track () {
   u8 nibble;
   u8 phy_nibble;
   u8 status;
   u16 nibble_ctrl2;
   u16 bs_reset_ctrl;

   if(glbl_bisc_en == 0) {
      DEBUG_PRINTF("INFO: ddr_read_dqs_track(): Skipping read dqs tracking as BISC_EN is 0\n");
      return 0;
   } else {
      glbl_read_dqs_track_en = 1;
      nibble_ctrl2 = read_riu_nibble_reg(glbl_dqs_bits_riu_map[0][0], DUP_NIBBLE_CTRL2);
      DEBUG_PRINTF("INFO: ddr_read_dqs_track(): read nibble_ctrl2 = %x\n", nibble_ctrl2);

      nibble_ctrl2 &= ~(1 << DIS_DQS_TRACK_POS);
      DEBUG_PRINTF("INFO: ddr_read_dqs_track(): write nibble_ctrl2 = %x\n", nibble_ctrl2);

      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dqs_nibble_mask_initial);
      riu_reg_bcast(DUP_NIBBLE_CTRL2, 0, nibble_ctrl2);  // Enable read dqs gate tracking - bit 7

      if(glbl_cal_done == 0) { // Sanity check 2 only during calibration
         DEBUG_PRINTF("INFO: ddr_read_dqs_track(): Do the sanity check 2\n");
         status = sanity_check_2();
         if(status != 0) {
            DEBUG_PRINTF("ERROR: ddr_read_dqs_track(): Sanity check 2 Failed\n");
            write_error_XRAM(ERR_CODE_77_CAL_DQS_GATE_TRACKING_FAIL_POST_SANITY_CHECK_1);
            return 1;
         } else {
            DEBUG_PRINTF("INFO: ddr_read_dqs_track(): Sanity check 2 Passed\n");
         }
      } else { // Sanity check ddr during mission mode
         DEBUG_PRINTF("INFO: ddr_read_dqs_track(): Do the sanity check ddr\n");
         status = sanity_check_ddr();
         if(status != 0) {
            DEBUG_PRINTF("ERROR: ddr_read_dqs_track(): Sanity check ddr Failed\n");
            write_error_XRAM(ERR_CODE_93_SELF_REF_EXIT_DQS_GATE_TRACKING_FAIL_POST_SANITY_CHECK);
            return 1;
         } else {
            DEBUG_PRINTF("INFO: ddr_read_dqs_track(): Sanity check ddr Passed\n");
         }
#ifdef TEST_VNC
         u32 loop_cnt = 1;
         status = sanity_check_2();
         if(status != 0) {
           writeIO_XRAM(0x7F60, 0x7);
           write_error_XRAM(ERR_CODE_93_SELF_REF_EXIT_DQS_GATE_TRACKING_FAIL_POST_SANITY_CHECK);
           while(1) { };
           return 1;
         }
       
         status = sanity_check_complex_read(loop_cnt);
         if(status != 0) {
           writeIO_XRAM(0x7F60, 0x8);
           write_error_XRAM(ERR_CODE_93_SELF_REF_EXIT_DQS_GATE_TRACKING_FAIL_POST_SANITY_CHECK);
           while(1) { };
           return 1;
         }
       
         status = sanity_check_complex_write(loop_cnt);
         if(status != 0) {
           writeIO_XRAM(0x7F60, 0x9);
           write_error_XRAM(ERR_CODE_93_SELF_REF_EXIT_DQS_GATE_TRACKING_FAIL_POST_SANITY_CHECK);
           while(1) { };
           return 1;
         }
       
         status = sanity_check_prbs_read(loop_cnt);
         if(status != 0) {
           writeIO_XRAM(0x7F60, 0xA);
           write_error_XRAM(ERR_CODE_93_SELF_REF_EXIT_DQS_GATE_TRACKING_FAIL_POST_SANITY_CHECK);
           while(1) { };
           return 1;
         }
       
         status = sanity_check_prbs_write(loop_cnt);
         if(status != 0) {
           writeIO_XRAM(0x7F60, 0xB);
           write_error_XRAM(ERR_CODE_93_SELF_REF_EXIT_DQS_GATE_TRACKING_FAIL_POST_SANITY_CHECK);
           while(1) { };
           return 1;
         }
#endif
      }

      DEBUG_PRINTF("INFO: ddr_read_dqs_track(): Check for underflow status\n");
      glbl_error_phy_nibble = 0;
      for(nibble=0; nibble<DQS_BITS; nibble++) {
         phy_nibble = glbl_dqs_bits_riu_map[nibble][0];
         bs_reset_ctrl = read_riu_nibble_reg(phy_nibble, BS_RESET_CTRL);
         if((bs_reset_ctrl >> COARSE_UNDERFLOW_POS) & 0x1) {
            DEBUG_PRINTF("ERROR: ddr_read_dqs_track(): coarse underflow for nibble = %d\n", nibble);
            glbl_error_phy_nibble |= (0x1 << phy_nibble);
         }
      }
      if(glbl_error_phy_nibble != 0) {
         write_error_nibble_XRAM(1, glbl_error_phy_nibble);
         if(glbl_cal_done == 0) {
            write_error_XRAM(ERR_CODE_78_CAL_DQS_GATE_TRACKING_RLDLYRNK_COARSE_UNDERFLOW);
         } else {
            write_error_XRAM(ERR_CODE_94_SELF_REF_EXIT_DQS_GATE_TRACKING_RLDLYRNK_COARSE_UNDERFLOW);
         }
         return 1;
      }

      DEBUG_PRINTF("INFO: ddr_read_dqs_track(): Check for overflow status\n");
      glbl_error_phy_nibble = 0;
      for(nibble=0; nibble<DQS_BITS; nibble++) {
         phy_nibble = glbl_dqs_bits_riu_map[nibble][0];
         bs_reset_ctrl = read_riu_nibble_reg(phy_nibble, BS_RESET_CTRL);
         if((bs_reset_ctrl >> COARSE_OVERFLOW_POS) & 0x1) {
            DEBUG_PRINTF("ERROR: ddr_read_dqs_track(): coarse overflow for nibble = %d\n", nibble);
            glbl_error_phy_nibble |= (0x1 << phy_nibble);
         }
      }
      if(glbl_error_phy_nibble != 0) {
         write_error_nibble_XRAM(1, glbl_error_phy_nibble);
         if(glbl_cal_done == 0) {
            write_error_XRAM(ERR_CODE_79_CAL_DQS_GATE_TRACKING_RLDLYRNK_COARSE_OVERFLOW);
         } else {
            write_error_XRAM(ERR_CODE_95_SELF_REF_EXIT_DQS_GATE_TRACKING_RLDLYRNK_COARSE_OVERFLOW);
         }
         return 1;
      }
   } // BISC_EN

   status = banks_refresh(0, 0x25);
   if(status) {
      return 1;
   }
   return 0;
}

#ifdef LPDDR4
u8 lp4_wr_dqs_track (u8 loop_cnt) {
   u8 loop;
   u8 rank;
   u8 status;
   u8 nibble;
   u8 phy_nibble;
   u16 offset;
   u16 wl_dly_rnk;
   u16 cal_crse;
   u16 cal_fine;
   u16 fine_phy;
   u16 fine;
   u16 cal_tdqs2dq;
   u16 cur_tdqs2dq;
   u16 cal_rl_dly_qtr;
   u16 cur_rl_dly_qtr;
   u8 xphy_rank = 0;
   u8 mem_rank0_xphy_rank = 0;
   u8 mem_rank1_xphy_rank = 1;
   u32 phy_rank_write_override;

   DEBUG_PRINTF("INFO: lp4_wr_dqs_track(): ranks = %d. Copying wl_dly_rnk delays into duplicate ranks. tCK = %d\n", RANKS, glbl_tck);
   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
      phy_nibble = glbl_map_riu[nibble];
      for(rank=0; rank<RANKS; rank++) { // Copy the delays into duplicate XPHY ranks
         wl_dly_rnk = read_riu_nibble_reg(phy_nibble, WLDLYRNK0+rank);
         write_riu_nibble_reg(phy_nibble, WLDLYRNK0+rank+2, wl_dly_rnk); // Rank0->Rank2 and Rank1->Rank3
      }
   }

   // Enable rank switching for single rank case also
   if(RANKS == 1) {
      DEBUG_PRINTF("INFO: lp4_wr_dqs_track(): Enabling dynamic mode for TX for single rank configuration also\n");
      glbl_phy_rank_switch_en = 1;
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
      riu_reg_bcast(DIS_DYN_MODE, 0, 0x0);
   }

   // Enable memory rank0 to xphy rank0 and memory rank1 to xphy rank1
   DEBUG_PRINTF("INFO: lp4_wr_dqs_track(): enable static rank override\n");
   phy_rank_write_override = (mem_rank1_xphy_rank << 12) | (mem_rank0_xphy_rank << 10) | (mem_rank1_xphy_rank << 4) | (mem_rank0_xphy_rank << 2) | 0x3;
   writeIO_DMC(ral_addr_of_phy_rank_write_override_in_ddrmc_main_ddrmc_main_bank(BLK), phy_rank_write_override);

   // Check the sanity
   status = sanity_check_1();
   if(status != 0) {
      DEBUG_PRINTF("ERROR: lp4_wr_dqs_track(): Sanity check 1 Failed after enabling static rank override\n");
      write_error_XRAM(ERR_CODE_80_CAL_LP4_OSC_TRACK_FAIL_PRE_SANITY_CHECK_1);
      return 1;
   }

   for(loop=0; loop<loop_cnt; loop++) { // Derive new odelay value and update
      for(rank=0; rank<RANKS; rank++) {
         // Get the new DQS oscillator value
         status = lp4_dqs_osc_read(rank, 0);
         if(status == 1) {
            DEBUG_PRINTF("ERROR: lp4_wr_dqs_track(): while getting the new DQS oscillator value for Rank = %d\n", rank);
            write_error_XRAM(ERR_CODE_83_CAL_LP4_OSC_TRACK_FAIL_DQS_OSC_VALUE_ZERO);
            writeIO_XRAM(XRAM_CAL_RANK,(1<<rank | 1 << 4));
            return 1;
         } else if(status == 2) {
            DEBUG_PRINTF("ERROR: lp4_wr_dqs_track(): while getting the new DQS oscillator value for Rank = %d\n", rank);
            write_error_XRAM(ERR_CODE_84_CAL_LP4_OSC_TRACK_FAIL_DQS_OSC_VALUE_OUT_OF_RANGE);
            writeIO_XRAM(XRAM_CAL_RANK,(1<<rank | 1 << 4));
            return 1;
         }

         if(rank == 0) { // Memory rank 0
            mem_rank0_xphy_rank = (mem_rank0_xphy_rank == 0) ? 2 : 0;
            xphy_rank = mem_rank0_xphy_rank;
         } else { // Memory rank 1
            mem_rank1_xphy_rank = (mem_rank1_xphy_rank == 1) ? 3 : 1;
            xphy_rank = mem_rank1_xphy_rank;
         }

         for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
            phy_nibble = glbl_map_riu[nibble];
            DEBUG_PRINTF("INFO: lp4_wr_dqs_track(): rank = %d, xphy_rank = %d, nibble = %d, phy_nibble = %d, tck = %d\n", rank, xphy_rank, nibble, phy_nibble, glbl_tck);

            cal_rl_dly_qtr = glbl_rl_dly_qtr[phy_nibble];
            // cur_rl_dly_qtr = read_riu_nibble_reg(phy_nibble, RL_DLY_QTR);
            write_riu_nibble_reg(phy_nibble, RD_DBG_RW_INDEX, 0xC); // Select HALFT_DQSM register in the Debug bus
            cur_rl_dly_qtr = (read_riu_nibble_reg(phy_nibble, RD_DBG_RD_STATUS) >> 1); // QTR = HALF/2

            cal_tdqs2dq = glbl_cal_tdqs2dq[rank][nibble/4];
            cur_tdqs2dq = glbl_cur_tdqs2dq[rank][nibble/4];

            wl_dly_rnk = glbl_cal_wl_dly_rnk[rank][nibble];
            cal_crse = (wl_dly_rnk >> 9) & 0xF;
            cal_fine = wl_dly_rnk & 0x1FF;

            DEBUG_PRINTF("INFO: lp4_wr_dqs_track(): cal_rl_dly_qtr = %d, cur_rl_dly_qtr = %d, cal_tdqs2dq = %d, cur_tdqs2dq = %d, cal wl_dly_rnk coarse = %d, cal wl_dly_rnk fine = %d\n", cal_rl_dly_qtr, cur_rl_dly_qtr, cal_tdqs2dq, cur_tdqs2dq, cal_crse, cal_fine);

            // Consider XPHY VT variation
            fine_phy = (cal_fine * cur_rl_dly_qtr) / cal_rl_dly_qtr;

            // Consider DRAM variation
            if(cur_tdqs2dq > cal_tdqs2dq) {
               offset = ((cur_tdqs2dq - cal_tdqs2dq) * cur_rl_dly_qtr * 4) / glbl_tck;
               fine = fine_phy + offset;
            } else {
               offset = ((cal_tdqs2dq - cur_tdqs2dq) * cur_rl_dly_qtr * 4) / glbl_tck;
               fine = fine_phy - offset;
            }

            DEBUG_PRINTF("INFO: lp4_wr_dqs_track(): xphy_rank = %d, cal fine = %d, xphy vt fine = %d, DRAM offset = %d, new fine = %d\n", xphy_rank, cal_fine, fine_phy, offset, fine);
            wl_dly_rnk = (cal_crse << 9) | fine;
            write_riu_nibble_reg(phy_nibble, WLDLYRNK0+xphy_rank, wl_dly_rnk);
         }

         phy_rank_write_override = (mem_rank1_xphy_rank << 12) | (mem_rank0_xphy_rank << 10) | (mem_rank1_xphy_rank << 4) | (mem_rank0_xphy_rank << 2) | 0x3;
         DEBUG_PRINTF("INFO: lp4_wr_dqs_track(): phy_rank_write_override: rank = %d, xphy_rank = %d, mem_rank0_xphy_rank = %d, mem_rank1_xphy_rank = %d, phy_rank_write_override = %x\n", rank, xphy_rank, mem_rank0_xphy_rank, mem_rank1_xphy_rank, phy_rank_write_override);
         writeIO_DMC(ral_addr_of_phy_rank_write_override_in_ddrmc_main_ddrmc_main_bank(BLK), phy_rank_write_override);
      }
   
      // Check the sanity
      status = sanity_check_1();
      if(status != 0) {
         DEBUG_PRINTF("ERROR: lp4_wr_dqs_track(): Sanity check 1 Failed\n");
         write_error_XRAM(ERR_CODE_81_CAL_LP4_OSC_TRACK_FAIL_SANITY_CHECK_1);
         return 1;
      }
   }

   // Enable memory rank0 to xphy rank0 and memory rank1 to xphy rank1
   DEBUG_PRINTF("INFO: lp4_wr_dqs_track(): disable static rank override\n");
   phy_rank_write_override = (mem_rank1_xphy_rank << 12) | (mem_rank0_xphy_rank << 10) | (mem_rank1_xphy_rank << 4) | (mem_rank0_xphy_rank << 2) | 0x0;
   writeIO_DMC(ral_addr_of_phy_rank_write_override_in_ddrmc_main_ddrmc_main_bank(BLK), phy_rank_write_override);

   // Enable phy rank override dynamic mode before cal_done, to set the flags in the RTL correctly. EDT-997224.
   u8 write_cnt = (readIO_DMC(ral_addr_of_phy_rank_write_override_dyn_in_ddrmc_main_ddrmc_main_bank(BLK)) >> 14) & 0x1F;
   u8 ch1_rank_update = 0x3;
   u8 ch0_rank_update = 0x3;
   u8 ch1_pause_writes = 1;
   u8 ch0_pause_writes = 1;
   u8 override_dyn_en = 3;
   u8 ch1_update_pulse = 1;
   u8 ch0_update_pulse = 1;
   u32 phy_rank_write_override_dyn;

   phy_rank_write_override_dyn = (write_cnt << 14) | (ch1_rank_update << 10) | (ch0_rank_update << 6) | (ch1_pause_writes << 5) | (ch0_pause_writes << 4) | override_dyn_en; // Enable it upfront
   writeIO_DMC(ral_addr_of_phy_rank_write_override_dyn_in_ddrmc_main_ddrmc_main_bank(BLK), phy_rank_write_override_dyn);

   phy_rank_write_override_dyn |= (ch1_update_pulse << 3) | (ch0_update_pulse << 2);
   DEBUG_PRINTF("INFO: lp4_wr_dqs_track(): phy_rank_write_override_dyn: phy_rank_write_override_dyn = %x\n", phy_rank_write_override_dyn);
   writeIO_DMC(ral_addr_of_phy_rank_write_override_dyn_in_ddrmc_main_ddrmc_main_bank(BLK), phy_rank_write_override_dyn);

   // Check the sanity
   status = sanity_check_1();
   if(status != 0) {
      DEBUG_PRINTF("ERROR: lp4_wr_dqs_track(): Sanity check 1 Failed after disabling static rank override\n");
      write_error_XRAM(ERR_CODE_82_CAL_LP4_OSC_TRACK_FAIL_POST_SANITY_CHECK_1);
      return 1;
   }

   DEBUG_PRINTF("INFO: lp4_wr_dqs_track(): Sanity check 1 Passed\n");
   return 0;
}

u8 lp4_dqs_osc_read (u8 rank, u8 save_val) {
   u8 nibble;
   u32 dq_3_0;
   u32 dq_7_4;
   u8 bit_log;
   u8 bit_phy;
   u8 dq_log;
   u8 dq_phy;
   u32 loop;
   u8 status;
   static u8 tdqsosc_lsb[DQS_BITS/2];
   static u8 tdqsosc_msb[DQS_BITS/2];

   DEBUG_PRINTF("INFO: lp4_dqs_osc_read(): Start for Rank = %d, tCK = %d\n", rank, glbl_tck);

   // Issue MPC: Start DQS Oscillator
   lp4_mpc_wr(rank, LP4_MPC_START_DQS_OSC, 0, 0);

   glbl_ddrmc_clk_cnt = glbl_lp4_dqs_timer_cnt / 2;
   writeIO_DMC(ral_addr_of_ddrmc_clk_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), ((0x1 << 31) | glbl_ddrmc_clk_cnt));

   while (glbl_ddrmc_clk_cnt != 0) {
      glbl_ddrmc_clk_cnt = (readIO_DMC(ral_addr_of_ddrmc_clk_cnt_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & ~(0x1 << 31)); // Exclude busy bit
   }

   // Issue MRR-18: DQS Oscillator LSB
   lp4_mode_reg_rd(rank, 18);
   u8 bit;
   u32 pb_dq_bits_31_0=0;
   u32 pb_dq_bits_63_32=0;
   u32 lb_dq_bits_31_0=0;
   u32 lb_dq_bits_63_32=0;

   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++){
	   dq_3_0 = readIO_DMC(ral_addr_of_bl8_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (nibble * 4)) & 0xF;
	   if(nibble<8) {
		   lb_dq_bits_31_0  = (dq_3_0<<(nibble*4)) | lb_dq_bits_31_0;
	   } else {
		   lb_dq_bits_63_32 = (dq_3_0<<((nibble-8)*4)) | lb_dq_bits_63_32;
	   }
   }
   l2p_bit_mapper(&pb_dq_bits_31_0, &pb_dq_bits_63_32, &lb_dq_bits_31_0, &lb_dq_bits_63_32);

   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble+=4) { // Every x16
	   if(nibble<8)
		   dq_phy = (pb_dq_bits_31_0>>(nibble*4)) & 0xFF;
	   else
		   dq_phy = (pb_dq_bits_63_32>>((nibble-8)*4)) & 0xFF;
	   tdqsosc_lsb[nibble/4] = dq_phy;
	   DEBUG_PRINTF("INFO: lp4_dqs_osc_read(): Rank = %d, x16 set %d, DQS Oscillator LSB (MRR-18) read: Oscillator value = %x, dq_3_0 = %x, dq_7_4 = %x\n", rank, nibble/4, tdqsosc_lsb[nibble/4], dq_3_0, dq_7_4);
   }

   // Issue MRR-19: DQS Oscillator MSB
   lp4_mode_reg_rd(rank, 19);
   pb_dq_bits_31_0=0;
   pb_dq_bits_63_32=0;
   lb_dq_bits_31_0=0;
   lb_dq_bits_63_32=0;
   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++){
	   dq_3_0 = readIO_DMC(ral_addr_of_bl8_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (nibble * 4)) & 0xF;
	   if(nibble<8) {
		   lb_dq_bits_31_0  = (dq_3_0<<(nibble*4)) | lb_dq_bits_31_0;
	   } else {
		   lb_dq_bits_63_32 = (dq_3_0<<((nibble-8)*4)) | lb_dq_bits_63_32;
	   }
   }
   l2p_bit_mapper(&pb_dq_bits_31_0, &pb_dq_bits_63_32, &lb_dq_bits_31_0, &lb_dq_bits_63_32);
   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble+=4) { // Every x16
	   if(nibble<8)
		   dq_phy = (pb_dq_bits_31_0>>(nibble*4)) & 0xFF;
	   else
		   dq_phy = (pb_dq_bits_63_32>>((nibble-8)*4)) & 0xFF;
	   tdqsosc_msb[nibble/4] = dq_phy;
	   DEBUG_PRINTF("INFO: lp4_dqs_osc_read(): Rank = %d, x16 set %d, DQS Oscillator MSB (MRR-19) read: Oscillator value = %x, dq_3_0 = %x, dq_7_4 = %x\n", rank, nibble/4, tdqsosc_msb[nibble/4], dq_3_0, dq_7_4);
   }

   glbl_error_phy_nibble = 0;
   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble+=4) { // Every x16
      glbl_cur_tdqsosc[rank][nibble/4] = (tdqsosc_msb[nibble/4] << 8) | tdqsosc_lsb[nibble/4];

      // Check for non ZERO values on DQS oscillator reads.
      if(glbl_cur_tdqsosc[rank][nibble/4] == 0) {
         DEBUG_PRINTF("ERROR: lp4_dqs_osc_read(): x16 set %d DQS Oscillator read value is ZERO for Rank = %d\n", nibble/4, rank);
         glbl_error_phy_nibble |= (0x1 << glbl_map_riu[nibble]);
      }
   }
   if(glbl_error_phy_nibble != 0) {
      write_error_nibble_XRAM(1, glbl_error_phy_nibble);
      writeIO_XRAM(XRAM_CAL_RANK,(1<<rank | 1 << 4));
      return 1;
   }

   glbl_error_phy_nibble = 0;
   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble+=4) { // Every x16
      glbl_cur_tdqs2dq[rank][nibble/4] = (glbl_lp4_dqs_timer_cnt * glbl_tck) / (glbl_cur_tdqsosc[rank][nibble/4] * 2);

      // Expected range is 200ps to 800ps. Allowing 5% tolerance for variations & calculations
      DEBUG_PRINTF("INFO: lp4_dqs_osc_read(): Rank = %d, x16 set %d DQS Oscillator value = %d, tdqs2dq = %d, tCK = %d\n", rank, nibble/4, glbl_cur_tdqsosc[rank][nibble/4], glbl_cur_tdqs2dq[rank][nibble/4], glbl_tck);
      if((glbl_cur_tdqs2dq[rank][nibble/4] <= 190) || (glbl_cur_tdqs2dq[rank][nibble/4] >= 840)) {
         DEBUG_PRINTF("ERROR: lp4_dqs_osc_read(): Calculated tDQS2DQ value = %d is out of range for x16 set %d, Rank = %d\n", glbl_cur_tdqs2dq[rank][nibble/4], nibble/4, rank);
         glbl_error_phy_nibble |= (0x1 << glbl_map_riu[nibble]);
      }

      if(save_val == 1){
         glbl_cal_tdqsosc[rank][nibble/4] = glbl_cur_tdqsosc[rank][nibble/4];
         glbl_cal_tdqs2dq[rank][nibble/4] = glbl_cur_tdqs2dq[rank][nibble/4];
      }
   }
   if(glbl_error_phy_nibble != 0) {
      writeIO_XRAM(XRAM_CAL_RANK,(1<<rank | 1 << 4));
      write_error_nibble_XRAM(1, glbl_error_phy_nibble);
      return 2;
   }

   DEBUG_PRINTF("INFO: lp4_dqs_osc_read(): Done for Rank = %d\n", rank);
   return 0;
}

u8 intr_lp4_wr_dqs_track (u8 channel) {
   u8 rank;
   u8 status;
   u8 nibble;
   u8 first_nibble;
   u8 last_nibble;
   u8 phy_nibble;
   u16 offset;
   u16 wl_dly_rnk;
   u16 cal_crse;
   u16 cal_fine;
   u16 fine_phy;
   u16 fine;
   u16 cal_tdqs2dq;
   u16 cur_tdqs2dq;
   u16 cal_rl_dly_qtr;
   u16 cur_rl_dly_qtr;
   u8 xphy_rank = 0;
   u8 write_cnt = (readIO_DMC(ral_addr_of_phy_rank_write_override_dyn_in_ddrmc_main_ddrmc_main_bank(BLK)) >> 14) & 0x1F;
   u8 override_dyn_en = 3;
   u8 ch0_rank_update = 0;
   u8 ch1_rank_update = 0;
   u8 ch0_update_pulse = 0;
   u8 ch1_update_pulse = 0;
   u8 ch0_pause_writes = 1;
   u8 ch1_pause_writes = 1;
   u8 ch0_busy;
   u8 ch1_busy;
   u32 phy_rank_write_override;
   u32 phy_rank_write_override_dyn;

   DEBUG_PRINTF("INFO: intr_lp4_wr_dqs_track(): Start for channel %d\n", channel);

   for(rank=0; rank<RANKS; rank++) {
      // Get the new DQS oscillator value
      status = intr_lp4_dqs_osc_read(rank, channel);
      if(status == 1) {
         DEBUG_PRINTF("ERROR: intr_lp4_wr_dqs_track(): while getting the new DQS oscillator value for Rank = %d\n", rank);
         write_error_XRAM(ERR_CODE_87_LP4_OSC_TRACK_FAIL_DQS_OSC_VALUE_ZERO);
         writeIO_XRAM(XRAM_CAL_RANK,(1<<rank | 1 << 4));
         return 1;
      } else if(status == 2) {
         DEBUG_PRINTF("ERROR: intr_lp4_wr_dqs_track(): while getting the new DQS oscillator value for Rank = %d\n", rank);
         write_error_XRAM(ERR_CODE_88_LP4_OSC_TRACK_FAIL_DQS_OSC_VALUE_OUT_OF_RANGE);
         writeIO_XRAM(XRAM_CAL_RANK,(1<<rank | 1 << 4));
         return 1;
      }

      if(channel == 0) {
         ch0_busy = readIO_DMC(ral_addr_of_xpi_dyn_rank_in_ddrmc_main_ddrmc_main_bank(BLK)) & 0x1;
         if(ch0_busy) {
            DEBUG_PRINTF("ERROR: intr_lp4_wr_dqs_track(): Rank %d Channel %d is still busy, previous update didn't take effect yet!!\n", rank, channel);
            write_error_XRAM(ERR_CODE_89_LP4_OSC_TRACK_FAIL_BUSY_RANK_SWITCHING);
            writeIO_XRAM(XRAM_CAL_RANK,(1<<rank | 1 << 4));
            return 1;
         }

         first_nibble = 0;
         last_nibble = glbl_lp4_ch1_nibble_base;
         ch0_update_pulse = 1;
         ch0_rank_update |= (1 << rank);

         if(rank == 0) { // Memory rank 0
            glbl_ch0_mem_rank0_xphy_rank = (glbl_ch0_mem_rank0_xphy_rank == 0) ? 2 : 0;
            xphy_rank = glbl_ch0_mem_rank0_xphy_rank;
         } else { // Memory rank 1
            glbl_ch0_mem_rank1_xphy_rank = (glbl_ch0_mem_rank1_xphy_rank == 1) ? 3 : 1;
            xphy_rank = glbl_ch0_mem_rank1_xphy_rank;
         }
      } else { // channel 1
         ch1_busy = (readIO_DMC(ral_addr_of_xpi_dyn_rank_in_ddrmc_main_ddrmc_main_bank(BLK)) >> 5) & 0x1;
         if(ch1_busy) {
            DEBUG_PRINTF("ERROR: intr_lp4_wr_dqs_track(): Rank %d Channel %d is still busy, previous update didn't take effect yet!!\n", rank, channel);
            write_error_XRAM(ERR_CODE_89_LP4_OSC_TRACK_FAIL_BUSY_RANK_SWITCHING);
            writeIO_XRAM(XRAM_CAL_RANK,(1<<rank | 1 << 4));
            return 1;
         }

         first_nibble = glbl_lp4_ch1_nibble_base;
         last_nibble = DQS_BITS*NIBBLE_PER_DQS;
         ch1_update_pulse = 1;
         ch1_rank_update |= (1 << rank);

         if(rank == 0) { // Memory rank 0
            glbl_ch1_mem_rank0_xphy_rank = (glbl_ch1_mem_rank0_xphy_rank == 0) ? 2 : 0;
            xphy_rank = glbl_ch1_mem_rank0_xphy_rank;
         } else { // Memory rank 1
            glbl_ch1_mem_rank1_xphy_rank = (glbl_ch1_mem_rank1_xphy_rank == 1) ? 3 : 1;
            xphy_rank = glbl_ch1_mem_rank1_xphy_rank;
         }
      }

      // Derive new odelay value and update
      for(nibble=first_nibble; nibble<last_nibble; nibble++) {
         phy_nibble = glbl_map_riu[nibble];
         DEBUG_PRINTF("INFO: intr_lp4_wr_dqs_track(): rank = %d, xphy_rank = %d, nibble = %d, phy_nibble = %d, tck = %d\n", rank, xphy_rank, nibble, phy_nibble, glbl_tck);

         cal_rl_dly_qtr = glbl_rl_dly_qtr[phy_nibble];
         // cur_rl_dly_qtr = read_riu_nibble_reg(phy_nibble, RL_DLY_QTR);
         write_riu_nibble_reg(phy_nibble, RD_DBG_RW_INDEX, 0xC); // Select HALFT_DQSM register in the Debug bus
         cur_rl_dly_qtr = (read_riu_nibble_reg(phy_nibble, RD_DBG_RD_STATUS) >> 1); // QTR = HALF/2
 
         cal_tdqs2dq = glbl_cal_tdqs2dq[rank][nibble/4];
         cur_tdqs2dq = glbl_cur_tdqs2dq[rank][nibble/4];
 
         wl_dly_rnk = glbl_cal_wl_dly_rnk[rank][nibble];
         cal_crse = (wl_dly_rnk >> 9) & 0xF;
         cal_fine = wl_dly_rnk & 0x1FF;

         DEBUG_PRINTF("INFO: intr_lp4_wr_dqs_track(): cal_rl_dly_qtr = %d, cur_rl_dly_qtr = %d, cal_tdqs2dq = %d, cur_tdqs2dq = %d, cal wl_dly_rnk coarse = %d, cal wl_dly_rnk fine = %d\n", cal_rl_dly_qtr, cur_rl_dly_qtr, cal_tdqs2dq, cur_tdqs2dq, cal_crse, cal_fine);

         // Consider XPHY VT variation
         fine_phy = (cal_fine * cur_rl_dly_qtr) / cal_rl_dly_qtr;

         // Consider DRAM variation
         if(cur_tdqs2dq > cal_tdqs2dq) {
            offset = ((cur_tdqs2dq - cal_tdqs2dq) * cur_rl_dly_qtr * 4) / glbl_tck;
            fine = fine_phy + offset;
         } else {
            offset = ((cal_tdqs2dq - cur_tdqs2dq) * cur_rl_dly_qtr * 4) / glbl_tck;
            fine = fine_phy - offset;
         }

         if(rank == 0) {
            writeIO_XRAM(XRAM_LP4DQSOSCTRACK_WLDLYRNK0_FINE + nibble*4, fine);
            if(glbl_dqsosc_wldlyrnk_fine_max[rank][nibble] < fine) {
               glbl_dqsosc_wldlyrnk_fine_max[rank][nibble] = fine;
               writeIO_XRAM(XRAM_LP4DQSOSCTRACK_WLDLYRNK0_FINE_MAX + nibble*4, fine);
            }
            if(glbl_dqsosc_wldlyrnk_fine_min[rank][nibble] > fine) {
               glbl_dqsosc_wldlyrnk_fine_min[rank][nibble] = fine;
               writeIO_XRAM(XRAM_LP4DQSOSCTRACK_WLDLYRNK0_FINE_MIN + nibble*4, fine);
            }
         } else if (rank == 1) {
            writeIO_XRAM(XRAM_LP4DQSOSCTRACK_WLDLYRNK1_FINE + nibble*4, fine);
            if(glbl_dqsosc_wldlyrnk_fine_max[rank][nibble] < fine) {
               glbl_dqsosc_wldlyrnk_fine_max[rank][nibble] = fine;
               writeIO_XRAM(XRAM_LP4DQSOSCTRACK_WLDLYRNK1_FINE_MAX + nibble*4, fine);
            }
            if(glbl_dqsosc_wldlyrnk_fine_min[rank][nibble] > fine) {
               glbl_dqsosc_wldlyrnk_fine_min[rank][nibble] = fine;
               writeIO_XRAM(XRAM_LP4DQSOSCTRACK_WLDLYRNK1_FINE_MIN + nibble*4, fine);
            }
         }

         DEBUG_PRINTF("INFO: intr_lp4_wr_dqs_track(): xphy_rank = %d, cal fine = %d, xphy vt fine = %d, DRAM offset = %d, new fine = %d\n", xphy_rank, cal_fine, fine_phy, offset, fine);
         wl_dly_rnk = (cal_crse << 9) | fine;
         write_riu_nibble_reg(phy_nibble, WLDLYRNK0+xphy_rank, wl_dly_rnk);
      }
   } // RANKS

   phy_rank_write_override = (glbl_ch1_mem_rank1_xphy_rank << 12) | (glbl_ch1_mem_rank0_xphy_rank << 10) | (glbl_ch0_mem_rank1_xphy_rank << 4) | (glbl_ch0_mem_rank0_xphy_rank << 2);
   DEBUG_PRINTF("INFO: intr_lp4_wr_dqs_track(): phy_rank_write_override: glbl_ch1_mem_rank1_xphy_rank = %d, glbl_ch1_mem_rank0_xphy_rank = %d, glbl_ch0_mem_rank1_xphy_rank = %d, glbl_ch0_mem_rank0_xphy_rank = %d, phy_rank_write_override = %x\n", glbl_ch1_mem_rank1_xphy_rank, glbl_ch1_mem_rank0_xphy_rank, glbl_ch0_mem_rank1_xphy_rank, glbl_ch0_mem_rank0_xphy_rank, phy_rank_write_override);
   writeIO_DMC(ral_addr_of_phy_rank_write_override_in_ddrmc_main_ddrmc_main_bank(BLK), phy_rank_write_override);

   phy_rank_write_override_dyn = (write_cnt << 14) | (ch1_rank_update << 10) | (ch0_rank_update << 6) | (ch1_pause_writes << 5) | (ch0_pause_writes << 4) | override_dyn_en; // Enable it upfront
   writeIO_DMC(ral_addr_of_phy_rank_write_override_dyn_in_ddrmc_main_ddrmc_main_bank(BLK), phy_rank_write_override_dyn);

   phy_rank_write_override_dyn |= (ch1_update_pulse << 3) | (ch0_update_pulse << 2);
   DEBUG_PRINTF("INFO: intr_lp4_wr_dqs_track(): phy_rank_write_override_dyn: channel = %d, phy_rank_write_override_dyn = %x\n", channel, phy_rank_write_override_dyn);
   writeIO_DMC(ral_addr_of_phy_rank_write_override_dyn_in_ddrmc_main_ddrmc_main_bank(BLK), phy_rank_write_override_dyn);

   DEBUG_PRINTF("INFO: intr_lp4_wr_dqs_track(): Done for channel %d\n", channel);
   return 0;
}

u8 intr_lp4_dqs_osc_read (u8 rank, u8 channel) {
   u8 bit_log;
   u8 bit_phy;
   u32 rdata;
   u8 dq_log;
   u8 dq_phy;
   u8 nibble;
   u8 first_nibble;
   u8 last_nibble;
   static u8 tdqsosc_lsb[DQS_BITS/2];
   static u8 tdqsosc_msb[DQS_BITS/2];

   DEBUG_PRINTF("INFO: intr_lp4_dqs_osc_read(): Start for Rank %d Channel %d\n", rank, channel);

   if(channel == 0) {
      first_nibble = 0;
      last_nibble = glbl_lp4_ch1_nibble_base;
   } else {
      first_nibble = glbl_lp4_ch1_nibble_base;
      last_nibble = DQS_BITS*NIBBLE_PER_DQS;
   }
   u8 bit;
   u32 pb_dq_bits_31_0=0;
   u32 pb_dq_bits_63_32=0;
   u32 lb_dq_bits_31_0=0;
   u32 lb_dq_bits_63_32=0;
   if(channel == 0) {
      if(rank == 0) {
         lb_dq_bits_31_0  = readIO_DMC(ral_addr_of_osci_read_mr18_ch0_rank0_0_in_ddrmc_main_ddrmc_main_bank(BLK));
         lb_dq_bits_63_32 = readIO_DMC(ral_addr_of_osci_read_mr18_ch0_rank0_1_in_ddrmc_main_ddrmc_main_bank(BLK));
      } else {
         lb_dq_bits_31_0  = readIO_DMC(ral_addr_of_osci_read_mr18_ch0_rank1_0_in_ddrmc_main_ddrmc_main_bank(BLK));
         lb_dq_bits_63_32 = readIO_DMC(ral_addr_of_osci_read_mr18_ch0_rank1_1_in_ddrmc_main_ddrmc_main_bank(BLK));
      }
   } else {
      if(rank == 0) {
         lb_dq_bits_31_0  = readIO_DMC(ral_addr_of_osci_read_mr18_ch1_rank0_0_in_ddrmc_main_ddrmc_main_bank(BLK));
         lb_dq_bits_63_32 = readIO_DMC(ral_addr_of_osci_read_mr18_ch1_rank0_1_in_ddrmc_main_ddrmc_main_bank(BLK));
      } else {
         lb_dq_bits_31_0  = readIO_DMC(ral_addr_of_osci_read_mr18_ch1_rank1_0_in_ddrmc_main_ddrmc_main_bank(BLK));
         lb_dq_bits_63_32 = readIO_DMC(ral_addr_of_osci_read_mr18_ch1_rank1_1_in_ddrmc_main_ddrmc_main_bank(BLK));
      }
   }
   l2p_bit_mapper(&pb_dq_bits_31_0, &pb_dq_bits_63_32, &lb_dq_bits_31_0, &lb_dq_bits_63_32);

   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble+=4) { // Every x16
	   if(nibble<8)
		   dq_phy = (pb_dq_bits_31_0>>(nibble*4)) & 0xFF;
	   else
		   dq_phy = (pb_dq_bits_63_32>>((nibble-8)*4)) & 0xFF;
	   tdqsosc_lsb[nibble/4] = dq_phy;
	   DEBUG_PRINTF("INFO: intr_lp4_dqs_osc_read(): Rank = %d, Channel %d, x16 set %d DQS Oscillator LSB (MRR-18) read: Oscillator value = %x, rdata = %x, dq_log = %x\n", rank, channel, nibble/4, tdqsosc_lsb, rdata, dq_log);
   }

   pb_dq_bits_31_0=0;
   pb_dq_bits_63_32=0;
   lb_dq_bits_31_0=0;
   lb_dq_bits_63_32=0;
   if(channel == 0) {
      if(rank == 0) {
         lb_dq_bits_31_0  = readIO_DMC(ral_addr_of_osci_read_mr19_ch0_rank0_0_in_ddrmc_main_ddrmc_main_bank(BLK));
         lb_dq_bits_63_32 = readIO_DMC(ral_addr_of_osci_read_mr19_ch0_rank0_1_in_ddrmc_main_ddrmc_main_bank(BLK));
      } else {
         lb_dq_bits_31_0  = readIO_DMC(ral_addr_of_osci_read_mr19_ch0_rank1_0_in_ddrmc_main_ddrmc_main_bank(BLK));
         lb_dq_bits_63_32 = readIO_DMC(ral_addr_of_osci_read_mr19_ch0_rank1_1_in_ddrmc_main_ddrmc_main_bank(BLK));
      }
   } else {
      if(rank == 0) {
         lb_dq_bits_31_0  = readIO_DMC(ral_addr_of_osci_read_mr19_ch1_rank0_0_in_ddrmc_main_ddrmc_main_bank(BLK));
         lb_dq_bits_63_32 = readIO_DMC(ral_addr_of_osci_read_mr19_ch1_rank0_1_in_ddrmc_main_ddrmc_main_bank(BLK));
      } else {
         lb_dq_bits_31_0  = readIO_DMC(ral_addr_of_osci_read_mr19_ch1_rank1_0_in_ddrmc_main_ddrmc_main_bank(BLK));
         lb_dq_bits_63_32 = readIO_DMC(ral_addr_of_osci_read_mr19_ch1_rank1_1_in_ddrmc_main_ddrmc_main_bank(BLK));
      }
   }
   l2p_bit_mapper(&pb_dq_bits_31_0, &pb_dq_bits_63_32, &lb_dq_bits_31_0, &lb_dq_bits_63_32);

   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble+=4) { // Every x16
	   if(nibble<8)
		   dq_phy = (pb_dq_bits_31_0>>(nibble*4)) & 0xFF;
	   else
		   dq_phy = (pb_dq_bits_63_32>>((nibble-8)*4)) & 0xFF;
	   tdqsosc_msb[nibble/4] = dq_phy;
	      DEBUG_PRINTF("INFO: intr_lp4_dqs_osc_read(): Rank = %d, Channel %d, x16 set %d DQS Oscillator MSB (MRR-19) read: Oscillator value = %x, rdata = %x, dq_log = %x\n", rank, channel, nibble/4, tdqsosc_msb, rdata, dq_log);
   }

   glbl_error_phy_nibble = 0;
   for(nibble=first_nibble; nibble<last_nibble; nibble+=4) { // Every x16

      glbl_cur_tdqsosc[rank][nibble/4] = (tdqsosc_msb[nibble/4] << 8) | tdqsosc_lsb[nibble/4];

      // Check for non ZERO values on DQS oscillator reads.
      if(glbl_cur_tdqsosc[rank][nibble/4] == 0) {
         DEBUG_PRINTF("ERROR: intr_lp4_dqs_osc_read(): Channel %d, x16 set %d DQS Oscillator read value is ZERO for Rank = %d\n", channel, nibble/4, rank);
         glbl_error_phy_nibble |= (0x1 << glbl_map_riu[nibble]);
      }
   }
   if(glbl_error_phy_nibble != 0) {
      write_error_nibble_XRAM(1, glbl_error_phy_nibble);
      writeIO_XRAM(XRAM_CAL_RANK,(1<<rank | 1 << 4));
      return 1;
   }

   glbl_error_phy_nibble = 0;
   for(nibble=first_nibble; nibble<last_nibble; nibble+=4) { // Every x16
      glbl_cur_tdqs2dq[rank][nibble/4] = (glbl_lp4_dqs_timer_cnt * glbl_tck) / (glbl_cur_tdqsosc[rank][nibble/4] * 2);

      // Expected range is 200ps to 800ps. Allowing 5% tolerance for variations & calculations
      DEBUG_PRINTF("INFO: intr_lp4_dqs_osc_read(): Rank = %d, Channel %d, x16 set %d DQS Oscillator value = %d, tdqs2dq = %d\n", rank, channel, nibble/4, glbl_cur_tdqsosc[rank][nibble/4], glbl_cur_tdqs2dq[rank][nibble/4]);
      if((glbl_cur_tdqs2dq[rank][nibble/4] <= 190) || (glbl_cur_tdqs2dq[rank][nibble/4] >= 840)) {
         DEBUG_PRINTF("ERROR: intr_lp4_dqs_osc_read(): Calculated tDQS2DQ value = %d is out of range for Channel %d, x16 set %d, Rank = %d\n", glbl_cur_tdqs2dq[rank][nibble/4], channel, nibble/4, rank);
         glbl_error_phy_nibble |= (0x1 << glbl_map_riu[nibble]);
      }

      if(nibble%4 == 0) { // Check for every x16 component
         if(glbl_tdqs2dq_max[rank][nibble/4] < glbl_cur_tdqs2dq[rank][nibble/4]) {
            glbl_tdqs2dq_max[rank][nibble/4] = glbl_cur_tdqs2dq[rank][nibble/4];
            writeIO_XRAM(XRAM_LP4DQSOSCTRACK_TDQS2DQ_MAX + (rank*4 + (nibble/4))*4, glbl_tdqs2dq_max[rank][nibble/4] / 2);
         }

         if(glbl_tdqs2dq_min[rank][nibble/4] > glbl_cur_tdqs2dq[rank][nibble/4]) {
            glbl_tdqs2dq_min[rank][nibble/4] = glbl_cur_tdqs2dq[rank][nibble/4];
            writeIO_XRAM(XRAM_LP4DQSOSCTRACK_TDQS2DQ_MIN + (rank*4 + (nibble/4))*4, glbl_tdqs2dq_min[rank][nibble/4] / 2);
         }

         writeIO_XRAM(XRAM_LP4DQSOSCTRACK_TDQS2DQ + (rank*4 + (nibble/4))*4, glbl_cur_tdqs2dq[rank][nibble/4] / 2); // Store half of tDQS2DQ (200ps to 800ps) to fit it in 9-bit XRAM
      }
   }
   if(glbl_error_phy_nibble != 0) {
      write_error_nibble_XRAM(1, glbl_error_phy_nibble);
      writeIO_XRAM(XRAM_CAL_RANK,(1<<rank | 1 << 4));
      return 2;
   }

   DEBUG_PRINTF("INFO: intr_lp4_dqs_osc_read(): Done for Rank %d Channel %d\n", rank, channel);
   return 0;
}
#endif

u8 ddr_init_f0 () {
   u8 init_status;
   init_status = readIO_DMC(ral_addr_of_seq_init_status_in_ddrmc_main_ddrmc_main_bank(BLK));
// Triggering init state machine is required for every time coming out of reset
      DEBUG_PRINTF("INFO: ddr_init_f0(): Trigger the memory initialization\n");
      writeIO_DMC(ral_addr_of_seq_init_start_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0); // Trigger the memory initialization
      writeIO_DMC(ral_addr_of_seq_init_start_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1); // Trigger the memory initialization

   // Wait for Memory initialization completion
   DEBUG_PRINTF("INFO: ddr_init_f0(): Polling init_done status\n");
   do {
      init_status = readIO_DMC(ral_addr_of_seq_init_status_in_ddrmc_main_ddrmc_main_bank(BLK));
      init_status = init_status & 0x1;
   } while (init_status != 0x1);

   DEBUG_PRINTF("INFO: ddr_init_f0(): Memory initialization completed\n");

   #ifdef DDR4
   u32 wdata;
      if(glbl_clamshell_cfg == 1) { // Clamshell configuration
         wdata = readIO_DMC(ral_addr_of_seq_side_addr15_in_ddrmc_main_ddrmc_main_bank(BLK));
	 wdata |= (0x1 << 28); // ddrmc_main_es2_clamshell_status (if_cal_seq_regs_status.seq_side_addr15_cbit[0])
         writeIO_DMC(ral_addr_of_seq_side_addr15_in_ddrmc_main_ddrmc_main_bank(BLK), wdata);
      }
   #endif

   // Set the default values for the calibration
   set_defaults(1);

   return 0;
}

u8 ddr_init_f1 () {
   u8 nibble, rank, ranks_per_slot, rank_db, rank_phy, rank_rcd;
   u32 rdata;
   u8 vref;
   u32 mbdr_freq_base = (glbl_freq_f1 == 0) ? MBDR_BASE_F0 : MBDR_BASE_F1;

   // Do the DRAM MRx programming for F1 frequency
   DEBUG_PRINTF("INFO: ddr_init_f1(): Memory initialization started for frequency = %d\n", glbl_freq_f1);
   #ifdef DDR4
      for(rank=0; rank<RANKS; rank++) {
         writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK), rank); // Selects xphy rank
         writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
         writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

         // MR3 programming
         ddr_mrs_write (rank, 0x3,  glbl_mr3);

         // MR6 programming
         if(glbl_wrvref_done == 0) { // & (glbl_cal_done == 0)
            ddr_mrs_write (rank, 0x6, (glbl_mr6 | 0x80));
            ddr_mrs_write (rank, 0x6, (glbl_mr6 | 0x80));
            ddr_mrs_write (rank, 0x6,  glbl_mr6);

         } else { // Reload the VREF values again, since tCCD_L can be different across frequencies.
            // Enter PDA Mode: MR3[4] = 1
            glbl_mr3 = glbl_mr3 | 0x10; // A4=1 for PDA mode enable
            ddr_mrs_write (rank, 0x3, glbl_mr3);

            // Enable the Tristate
            writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 1);                  

            // Select per nibble values
            writeIO_DMC(ral_addr_of_seq_dq_nibble_sel_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);

            for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
               writeIO_DMC(ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (nibble * 4), 0xFFFFFFFF);
            }
            
            // MR6 for VREF
            for(nibble=0; nibble<DQS_BITS; nibble++) {
               writeIO_DMC(ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + ((nibble * NIBBLE_PER_DQS) * 4), 0x00000000);
               if(NIBBLE_PER_DQS == 2) {
                  writeIO_DMC(ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + ((nibble * NIBBLE_PER_DQS) * 4) + 4, 0x00000000);
               }

               rdata = readIO_MBDR(mbdr_freq_base + MBDR_MEM_WRITE_VREF + (nibble * 4));
               vref = (rdata >> (rank * 8)) & 0x7F; // [6:0]

               ddr_mrs_write (rank, 0x6, (glbl_mr6 | 0x80));
               ddr_mrs_write (rank, 0x6, ((glbl_mr6 & ~0x3F) | 0x80 | vref));
               ddr_mrs_write (rank, 0x6, ((glbl_mr6 & ~0x3F) | vref));

               writeIO_DMC(ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + ((nibble * NIBBLE_PER_DQS) * 4), 0xFFFFFFFF);
               if(NIBBLE_PER_DQS == 2) {
                  writeIO_DMC(ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + ((nibble * NIBBLE_PER_DQS) * 4) + 4, 0xFFFFFFFF);
               }
            }
            
            for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
               writeIO_DMC(ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (nibble * 4), 0x00000000);
            }
            
            // Exit PDA Mode: MR3[4] = 0
            glbl_mr3 = glbl_mr3 & ~(0x10); // A4=0 for PDA mode disable
            ddr_mrs_write (rank, 0x3,  glbl_mr3);

            // De-select per nibble values
            writeIO_DMC(ral_addr_of_seq_dq_nibble_sel_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

            // Disable the Tristate
            writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0);                  
         }

         // MR5 programming
         ddr_mrs_write (rank, 0x5,  glbl_mr5[rank]);

         // MR4 programming
         ddr_mrs_write (rank, 0x4,  glbl_mr4);

         // MR2 programming
         ddr_mrs_write (rank, 0x2,  glbl_mr2);

         // MR1 programming
         ddr_mrs_write (rank, 0x1,  glbl_mr1[rank]);

         // MR0 programming
         ddr_mrs_write (rank, 0x0,  glbl_mr0);

         // RCD programming
         if(glbl_rdimm_en && ((rank == 0) || ((SLOTS == 2) && (rank == RANKS/2)))) {
            ddr_mrs7_write (rank, 0x0500 | glbl_rcd_rc5x); // RC5X
            ddr_mrs7_write (rank, 0x0800 | glbl_rcd_rc8x); // RC8x
            ddr_mrs7_write (rank, 0x00f0 | glbl_rcd_rc0f); // RC0F
         }
      }
   #else
      // Program MR23 value based on F0/F1
      for(rank=0; rank<RANKS; rank++) {
         lp4_mode_reg_wr(rank, 23, glbl_lp4_mr23);
      }

      // Switch the Frequency Set Point (FSP-OP)
      for(rank=0; rank<RANKS; rank++) {
         lp4_mode_reg_wr(rank, 13, glbl_lp4_mr13);
      }
   #endif

   DEBUG_PRINTF("INFO: ddr_init_f1(): Memory initialization completed for frequency = %d\n", glbl_freq_f1);

   // Set the default values only during calibration sequence
   if(glbl_cal_done == 0) {
      set_defaults(0);
      DEBUG_PRINTF("INFO: ddr_init_f1(): set defaults completed for frequency = %d\n", glbl_freq_f1);
   }

   return 0;
}

u8 pll_clkoutphy_en (u8 power_on_reset) {
   u8 xpll;
   u32 loop;
   u8 nibble;
   u16 calib_ctrl;
   u16 rdata, wdata;
   u8 clkoutphy_divmode;
   u8 clkfbout;
   u8 clkfbout_edge;
   u8 divclk;
   u8 divclk_edge;
   u8 clkout;
   u8 xpll_nibble;
   u8 xpll_lock = 0;
   u8 xpll_calerror = 0;
   static u8 xpll_master[6] = XPLL_MASTER;
   static u8 xpll_slave[6] = {0, 0, 0, 0, 0, 0}; // XPLL_SLAVE;
   u32 WATCH_DOG_COUNT = 5000; // maximum possible time for XPLL to get locked

   writeIO_XRAM(XRAM_CAL_PRE_STATUS_17_9, 0x1);

 // Remove ODISABLE to have gracefull exit during multi-reset testing
    pll_clkoutphy_dis();

   // Program the master XPLL, release the reset and wait for lock
   for(xpll=0; xpll<6; xpll++) {
      if(xpll_master[xpll] == 1) {
         xpll_nibble = BANK0_PLL0_RPI_NIBBLE + xpll;

         if(power_on_reset == 0) {
#if MODE_FREQ_SWITCH == CAL_EN
            DEBUG_PRINTF("INFO: pll_clkoutphy_en(): Apply Master XPLL Reset: xpll_nibble = %d, initstate = 0\n", xpll_nibble);
            write_riu_nibble_reg(xpll_nibble, XPLL_PCSR_MASK, XPLL_PCSR_INITSTATE);
            write_riu_nibble_reg(xpll_nibble, XPLL_PCSR_CONTROL, XPLL_PCSR_INITSTATE);

            // Gatereg results in a glitch on clkoutphy_en for the slave banks
            // DEBUG_PRINTF("INFO: pll_clkoutphy_en(): Apply Master XPLL GateReg: xpll_nibble = %d, gatereg = 1\n", xpll_nibble);
            // write_riu_nibble_reg(xpll_nibble, XPLL_PCSR_MASK, XPLL_PCSR_GATEREG);
            // write_riu_nibble_reg(xpll_nibble, XPLL_PCSR_CONTROL, XPLL_PCSR_GATEREG);

            write_riu_nibble_reg(xpll_nibble, XPLL_REG_0_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_0 : XPLL_F0_REG_0));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_1_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_1 : XPLL_F0_REG_1));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_2_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_2 : XPLL_F0_REG_2));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_3_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_3 : XPLL_F0_REG_3));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_4_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_4 : XPLL_F0_REG_4));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_5_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_5 : XPLL_F0_REG_5));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_6_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_6 : XPLL_F0_REG_6));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_7_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_7 : XPLL_F0_REG_7));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_8_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_8 : XPLL_F0_REG_8));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_9_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_9 : XPLL_F0_REG_9));

            write_riu_nibble_reg(xpll_nibble, XPLL_REG_10_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_10 : XPLL_F0_REG_10));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_11_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_11 : XPLL_F0_REG_11));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_12_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_12 : XPLL_F0_REG_12));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_13_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_13 : XPLL_F0_REG_13));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_14_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_14 : XPLL_F0_REG_14));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_15_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_15 : XPLL_F0_REG_15));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_16_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_16 : XPLL_F0_REG_16));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_17_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_17 : XPLL_F0_REG_17));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_18_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_18 : XPLL_F0_REG_18));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_19_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_19 : XPLL_F0_REG_19));

            write_riu_nibble_reg(xpll_nibble, XPLL_REG_20_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_20 : XPLL_F0_REG_20));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_21_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_21 : XPLL_F0_REG_21));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_22_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_22 : XPLL_F0_REG_22));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_23_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_23 : XPLL_F0_REG_23));

            wdata = (glbl_freq_f1 ? XPLL_F1_REG_24 : XPLL_F0_REG_24);
	    wdata &= ~(0x8); // Disable locked_deskew_2nd - Workaround for CR-1030808
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_24_ADDR, wdata);

            write_riu_nibble_reg(xpll_nibble, XPLL_REG_25_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_25 : XPLL_F0_REG_25));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_26_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_26 : XPLL_F0_REG_26));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_27_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_27 : XPLL_F0_REG_27));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_28_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_28 : XPLL_F0_REG_28));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_29_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_29 : XPLL_F0_REG_29));

            write_riu_nibble_reg(xpll_nibble, XPLL_REG_30_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_30 : XPLL_F0_REG_30));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_31_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_31 : XPLL_F0_REG_31));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_32_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_32 : XPLL_F0_REG_32));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_33_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_33 : XPLL_F0_REG_33));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_34_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_34 : XPLL_F0_REG_34));

            // CLKOUT0/1/2: 952ps correspond to 1050MHz, 2100MBPS which is the lowest of the XPLL VCO range
            // DIVMODE = 0. CLKOUT = 0x0101 for DIV1. 952ps > tCK
            // DIVMODE = 1. CLKOUT = 0x0202 for DIV2. 1904ps > tCK > 952ps
            // DIVMODE = 2. CLKOUT = 0x0404 for DIV4. 3808ps > tCK > 1904ps
            // DIVMODE = 3. CLKOUT = 0x0808 for DIV8. tCK > 3808ps
            if(glbl_tck < 952) {
               write_riu_nibble_reg(xpll_nibble, XPLL_REG_35_ADDR, 0x200 | (0x1000 | (glbl_freq_f1 ? XPLL_F1_REG_35 : XPLL_F0_REG_35)));
            } else {
               write_riu_nibble_reg(xpll_nibble, XPLL_REG_35_ADDR, ~0x200 & (0x1000 | (glbl_freq_f1 ? XPLL_F1_REG_35 : XPLL_F0_REG_35)));
            }

            write_riu_nibble_reg(xpll_nibble, XPLL_REG_36_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_36 : XPLL_F0_REG_36));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_37_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_37 : XPLL_F0_REG_37));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_38_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_38 : XPLL_F0_REG_38));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_39_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_39 : XPLL_F0_REG_39));

            write_riu_nibble_reg(xpll_nibble, XPLL_REG_40_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_40 : XPLL_F0_REG_40));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_41_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_41 : XPLL_F0_REG_41));
            write_riu_nibble_reg(xpll_nibble, XPLL_REG_42_ADDR, (glbl_freq_f1 ? XPLL_F1_REG_42 : XPLL_F0_REG_42));

            // DEBUG_PRINTF("INFO: pll_clkoutphy_en(): Release Master XPLL GateReg: xpll_nibble = %d, gatereg = 0\n", xpll_nibble);
            // write_riu_nibble_reg(xpll_nibble, XPLL_PCSR_MASK, XPLL_PCSR_GATEREG);
            // write_riu_nibble_reg(xpll_nibble, XPLL_PCSR_CONTROL, ~XPLL_PCSR_GATEREG);
#endif
         }

         DEBUG_PRINTF("INFO: pll_clkoutphy_en(): Release Master XPLL Reset: xpll_nibble = %d, initstate = 0\n", xpll_nibble);
         write_riu_nibble_reg(xpll_nibble, XPLL_PCSR_MASK, XPLL_PCSR_INITSTATE);
         write_riu_nibble_reg(xpll_nibble, XPLL_PCSR_CONTROL, ~XPLL_PCSR_INITSTATE);

         // Wait for lock for Master XPLLs
         DEBUG_PRINTF("INFO: pll_clkoutphy_en(): Waiting for Master XPLL Lock: xpll_nibble = %d\n", xpll_nibble);
         for(loop=0; loop < WATCH_DOG_COUNT; loop++) {
            xpll_lock = (read_riu_nibble_reg(xpll_nibble, XPLL_PCSR_STATUS) & XPLL_PCSR_CALDONE);
            xpll_calerror = (read_riu_nibble_reg(xpll_nibble, XPLL_PCSR_STATUS) & XPLL_PCSR_CALERROR);
            DEBUG_PRINTF("INFO: pll_clkoutphy_en(): loop = %d, xpll_nibble = %d, xpll_lock = %x\n", loop, xpll_nibble, xpll_lock);

            if(xpll_lock) {
               DEBUG_PRINTF("INFO: pll_clkoutphy_en(): Master XPLL is Locked: xpll_nibble = %d\n", xpll_nibble);
               break;
            }

            if(xpll_calerror) {
               DEBUG_PRINTF("INFO: pll_clkoutphy_en(): Master XPLL calibration error: xpll_nibble = %d\n", xpll_nibble);
               if(glbl_cal_done == 0) {
                  write_error_XRAM(ERR_CODE_2_XPLL_CAL_ERROR);
               } else if(glbl_restore_cal_en == 1) {
                  write_error_XRAM(ERR_CODE_111_RESTORE_CAL_XPLL_CAL_ERROR);
               } else if(glbl_freq_switch == 1) {
                  write_error_XRAM(ERR_CODE_98_FREQ_SWITCH_XPLL_CAL_ERROR);
               }
               return 1;
            }
         }

         if(xpll_lock == 0) {
            DEBUG_PRINTF("ERROR: pll_clkoutphy_en(): Timeout waiting for Master XPLL Lock: xpll_nibble = %d\n", xpll_nibble);
            if(glbl_cal_done == 0) {
               write_error_XRAM(ERR_CODE_1_XPLL_TIMEOUT);
            } else if(glbl_restore_cal_en == 1) {
               write_error_XRAM(ERR_CODE_110_RESTORE_CAL_XPLL_TIMEOUT);
            } else if(glbl_freq_switch == 1) {
               write_error_XRAM(ERR_CODE_97_FREQ_SWITCH_XPLL_TIMEOUT);
            }
            return 1;
         }
      }
   }

   writeIO_XRAM(XRAM_CAL_PRE_STATUS_17_9, 0x3);

   // Release the reset to slave XPLLs
   for(xpll=0; xpll<6; xpll++) {
      if(xpll_slave[xpll] == 1) {
         xpll_nibble = BANK0_PLL0_RPI_NIBBLE + xpll;

         DEBUG_PRINTF("INFO: pll_clkoutphy_en(): Release Slave XPLL Reset: xpll_nibble = %d, initstate = 0\n", xpll_nibble);
         write_riu_nibble_reg(xpll_nibble, XPLL_PCSR_MASK, XPLL_PCSR_INITSTATE);
         write_riu_nibble_reg(xpll_nibble, XPLL_PCSR_CONTROL, ~XPLL_PCSR_INITSTATE);
      }
   }

   // Wait for lock for Slave XPLLs
   for(xpll=0; xpll<6; xpll++) {
      if(xpll_slave[xpll] == 1) {
         xpll_nibble = BANK0_PLL0_RPI_NIBBLE + xpll;

         DEBUG_PRINTF("INFO: pll_clkoutphy_en(): Waiting for Slave XPLL Lock: xpll_nibble = %d\n", xpll_nibble);
         for(loop=0; loop < WATCH_DOG_COUNT; loop++) {
            xpll_lock = (read_riu_nibble_reg(xpll_nibble, XPLL_PCSR_STATUS) & XPLL_PCSR_CALDONE);
            DEBUG_PRINTF("INFO: pll_clkoutphy_en(): loop = %d, xpll_nibble = %d, xpll_lock = %x\n", loop, xpll_nibble, xpll_lock);

            if(xpll_lock) {
               DEBUG_PRINTF("INFO: pll_clkoutphy_en(): Slave XPLL is Locked: xpll_nibble = %d\n", xpll_nibble);
               break;
            }
         }

         // Check if not Locked
         if(xpll_lock == 0) {
            DEBUG_PRINTF("ERROR: pll_clkoutphy_en(): Timeout waiting for Slave XPLL Lock: xpll_nibble = %d\n", xpll_nibble);
            return 1;
         }
      }
   }

   writeIO_XRAM(XRAM_CAL_PRE_STATUS_17_9, 0x7);

   DEBUG_PRINTF("INFO: pll_clkoutphy_en(): Switch the mc_clk to functional clock\n");
   glbl_gpo1 = (glbl_gpo1 | 0x1); // Bit-0 is mc_clk_mux
   writeIO_IOM(IO_GPO1, glbl_gpo1);

   // Set XPLLs CLKOUTPHY_EN
   for(xpll=0; xpll<6; xpll++) {
      if((xpll_master[xpll] == 1) || (xpll_slave[xpll] == 1)) {
         xpll_nibble = BANK0_PLL0_RPI_NIBBLE + xpll;

         DEBUG_PRINTF("INFO: pll_clkoutphy_en(): XPLL odisable = 0\n");
         write_riu_nibble_reg(xpll_nibble, XPLL_PCSR_MASK, XPLL_PCSR_ODISABLE); // Setting mask to override the odsiable
         write_riu_nibble_reg(xpll_nibble, XPLL_PCSR_CONTROL, ~XPLL_PCSR_ODISABLE); // Programming odisable to 0
      }
   }

   writeIO_XRAM(XRAM_CAL_PRE_STATUS_17_9, 0xF);

   u32 check_time = 0x00010000; // 64k DDRMC cycles
   u32 ddrmc_clk_cnt;
   u32 pit1_cnt_prev;
   u32 pit1_cnt;
   u32 ub_clk_period;
   u32 mc_clk_period;

   ddrmc_clk_cnt = check_time;
   writeIO_DMC(ral_addr_of_ddrmc_clk_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), ((0x1 << 31) | ddrmc_clk_cnt));
   pit1_cnt_prev = readIO_IOM(IO_PIT1_COUNTER);

   while (ddrmc_clk_cnt != 0) {
      ddrmc_clk_cnt = (readIO_DMC(ral_addr_of_ddrmc_clk_cnt_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & ~(0x1 << 31)); // Exclude busy bit
   }
   pit1_cnt = readIO_IOM(IO_PIT1_COUNTER);

   writeIO_MBDR(RAM_DATA_CAL_OFFSET + 0xFF0 + glbl_freq_f1 * 16, (pit1_cnt_prev - pit1_cnt));

   ub_clk_period = (check_time * glbl_tck * 2) / (pit1_cnt_prev - pit1_cnt);
   writeIO_MBDR(RAM_DATA_CAL_OFFSET + 0xFF4 + glbl_freq_f1 * 16, ub_clk_period);

   mc_clk_period = ((pit1_cnt_prev - pit1_cnt) * NOC_CLK_PERIOD * 2) / check_time;
   writeIO_MBDR(RAM_DATA_CAL_OFFSET + 0xFF8 + glbl_freq_f1 * 16, mc_clk_period);

   return 0;
}

u8 pll_clkoutphy_dis () {
   u8 xpll;
   u8 xpll_nibble;
   static u8 xpll_master[6] = XPLL_MASTER;
   static u8 xpll_slave[6] = {0, 0, 0, 0, 0, 0}; // XPLL_SLAVE;

   for(xpll=0; xpll<6; xpll++) {
      if((xpll_master[xpll] == 1) || (xpll_slave[xpll] == 1)) {
         xpll_nibble = BANK0_PLL0_RPI_NIBBLE + xpll;

         DEBUG_PRINTF("INFO: pll_clkoutphy_dis(): XPLL odisable = 1 for XPLL Nibble = %d\n", xpll_nibble);
         write_riu_nibble_reg(xpll_nibble, XPLL_PCSR_MASK, XPLL_PCSR_ODISABLE); // Setting mask to override the odsiable
         write_riu_nibble_reg(xpll_nibble, XPLL_PCSR_CONTROL, XPLL_PCSR_ODISABLE); // Programming odisable to 1
      }
   }

   DEBUG_PRINTF("INFO: pll_clkoutphy_dis(): Switch the mc_clk to default clock\n");
   glbl_gpo1 = (glbl_gpo1 & (~0x1)); // Bit-0 is mc_clk_mux
   writeIO_IOM(IO_GPO1, glbl_gpo1);

   return 0;
}

void bitslice_reset_bcast (u8 bcast_nibble_mask, u8 value) {
   u8 loop;

   DEBUG_PRINTF("INFO: bitslice_reset_bcast(): Apply XPHY bit-slice reset: nibble mask = %x, value = %x\n", bcast_nibble_mask, value);

   riu_reg_bcast(BS_RESET_CTRL, bcast_nibble_mask, value);

   for(loop=0; loop < (glbl_tck / BS_RESET_EXTEND); loop++) { // Extend bit-slice reset for lower frequencies
      riu_reg_bcast(BS_RESET_CTRL, bcast_nibble_mask, value);
   }

   riu_reg_bcast(BS_RESET_CTRL, bcast_nibble_mask, 0x0);

   return;
}

u8 set_freq_globals() {

   DEBUG_PRINTF("INFO: set_freq_globals(): Setting for Frequency %d \n", glbl_freq_f1);

   glbl_tck = (glbl_freq_f1 == 0) ? F0_TCK : F1_TCK;
   glbl_qtr_dly = (glbl_freq_f1 == 0) ? F0_QTR_DLY : F1_QTR_DLY;
   glbl_xram_freq_base = (glbl_freq_f1 == 0) ? 0 : (XRAM_BASE_F1 - XRAM_BASE_F0);

   DEBUG_PRINTF("INFO: set_freq_globals(): TCK = %d \n", glbl_tck);
   DEBUG_PRINTF("INFO: set_freq_globals(): QTR_DLY = %d \n", glbl_qtr_dly);

   #ifdef DDR4
      u8 rank;
      static u32 lcl_f0_mr1[4] = F0_MR1;
      static u32 lcl_f1_mr1[4] = F1_MR1;
      static u32 lcl_f0_mr5[4] = F0_MR5;
      static u32 lcl_f1_mr5[4] = F1_MR5;

      glbl_mr0 = (glbl_freq_f1 == 0) ? F0_MR0 : F1_MR0;
      glbl_mr2 = (glbl_freq_f1 == 0) ? F0_MR2 : F1_MR2;
      glbl_mr3 = (glbl_freq_f1 == 0) ? F0_MR3 : F1_MR3;
      glbl_mr4 = (glbl_freq_f1 == 0) ? F0_MR4 : F1_MR4;
      glbl_mr6 = (glbl_freq_f1 == 0) ? F0_MR6 : F1_MR6;

      glbl_ref_mode_1x = (((glbl_mr3 >> 6) & 0x7) == 0x0) ? 1 : 0;
      glbl_ref_mode_2x = (((glbl_mr3 >> 6) & 0x7) == 0x1) ? 1 : 0;
      glbl_ref_mode_4x = (((glbl_mr3 >> 6) & 0x7) == 0x2) ? 1 : 0;

      glbl_ref_temp_sensor_en = (((glbl_mr3 >> 5) & 0x1) == 0x1) ? 1 : 0;

      glbl_wrvref_default_value = glbl_lrdimm_en ? (DB_F5BC5X & 0x3F) : (glbl_mr6 & 0x3F);

      glbl_wr_pre_2tck_mode = (glbl_mr4 >> 12) & 0x1;
      glbl_rd_pre_2tck_mode = (glbl_mr4 >> 11) & 0x1;

      DEBUG_PRINTF("INFO: set_freq_globals(): MR0 = %x \n", glbl_mr0);
      DEBUG_PRINTF("INFO: set_freq_globals(): MR4 = %x \n", glbl_mr4);
      DEBUG_PRINTF("INFO: set_freq_globals(): MR6 = %x \n", glbl_mr6);
      DEBUG_PRINTF("INFO: set_freq_globals(): MR2 = %x \n", glbl_mr2);
      DEBUG_PRINTF("INFO: set_freq_globals(): MR3 = %x \n", glbl_mr3);

      for(rank=0; rank<RANKS; rank++) {
         glbl_mr1[rank] = (glbl_freq_f1 == 0) ? lcl_f0_mr1[rank] : lcl_f1_mr1[rank];
         glbl_mr5[rank] = (glbl_freq_f1 == 0) ? lcl_f0_mr5[rank] : lcl_f1_mr5[rank];
         DEBUG_PRINTF("INFO: set_freq_globals(): Rank = %d, MR1 = %x \n", rank, glbl_mr1[rank]);
         DEBUG_PRINTF("INFO: set_freq_globals(): Rank = %d, MR5 = %x \n", rank, glbl_mr5[rank]);
      }

      glbl_rcd_rc0f = (glbl_freq_f1 == 0) ? F0_RCD_RC0F : F1_RCD_RC0F;
      glbl_rcd_rc5x = (glbl_freq_f1 == 0) ? F0_RCD_RC5X : F1_RCD_RC5X;
      glbl_rcd_rc8x = (glbl_freq_f1 == 0) ? F0_RCD_RC8X : F1_RCD_RC8X;

      if(glbl_rdimm_en) {
         DEBUG_PRINTF("INFO: set_freq_globals(): RCD_RC0F = %x \n", glbl_rcd_rc0f);
         DEBUG_PRINTF("INFO: set_freq_globals(): RCD_RC5X = %x \n", glbl_rcd_rc5x);
         DEBUG_PRINTF("INFO: set_freq_globals(): RCD_RC8X = %x \n", glbl_rcd_rc8x);
      }

   #endif

   #ifdef LPDDR4
      glbl_lp4_mr1 = (glbl_freq_f1 == 0) ? F0_MR1 : F1_MR1;
      glbl_lp4_mr2 = (glbl_freq_f1 == 0) ? F0_MR2 : F1_MR2;
      glbl_lp4_mr3 = (glbl_freq_f1 == 0) ? F0_MR3 : F1_MR3;
      glbl_lp4_mr4 = (glbl_freq_f1 == 0) ? F0_MR4 : F1_MR4;
      glbl_lp4_mr5 = (glbl_freq_f1 == 0) ? F0_MR5 : F1_MR5;
      glbl_lp4_mr12 = (glbl_freq_f1 == 0) ? F0_MR12 : F1_MR12;
      glbl_lp4_mr13 = (glbl_freq_f1 == 0) ? F0_MR13 : F1_MR13;
      glbl_lp4_mr14 = (glbl_freq_f1 == 0) ? F0_MR14 : F1_MR14;
      glbl_lp4_mr23 = (glbl_freq_f1 == 0) ? F0_MR23 : F1_MR23;

      glbl_ref_mode_1x = 0x1; // ((glbl_lp4_mr4 & 0x7) == 0x3) ? 1 : 0;
      glbl_ref_mode_2x = 0x0; // ((glbl_lp4_mr4 & 0x7) == 0x2) ? 1 : 0;
      glbl_ref_mode_4x = 0x0; // ((glbl_lp4_mr4 & 0x7) == 0x1) ? 1 : 0;

      glbl_ref_temp_sensor_en = 0;
      glbl_ref_speed = 0;

      glbl_wrvref_default_value = glbl_lp4_mr14 & 0x3F;

      glbl_wr_pre_2tck_mode = 1;
      glbl_rd_pre_2tck_mode = 1;

      if((glbl_lp4_mr23 >> 6) & 0x3) {
        glbl_lp4_dqs_timer_cnt = (1024 << ((glbl_lp4_mr23 >> 6) & 0x3)); // MR23[7:6]: 01 - 2048, 10 - 4096, 11 - 8192
      } else {
        glbl_lp4_dqs_timer_cnt = (glbl_lp4_mr23 & 0x3F) * 16; // MR23[5:0] * 16
      }

      DEBUG_PRINTF("INFO: set_freq_globals(): MR1 = %x \n", glbl_lp4_mr1);
      DEBUG_PRINTF("INFO: set_freq_globals(): MR2 = %x \n", glbl_lp4_mr2);
      DEBUG_PRINTF("INFO: set_freq_globals(): MR3 = %x \n", glbl_lp4_mr3);
      DEBUG_PRINTF("INFO: set_freq_globals(): MR4 = %x \n", glbl_lp4_mr4);
      DEBUG_PRINTF("INFO: set_freq_globals(): MR5 = %x \n", glbl_lp4_mr5);
      DEBUG_PRINTF("INFO: set_freq_globals(): MR12 = %x \n", glbl_lp4_mr12);
      DEBUG_PRINTF("INFO: set_freq_globals(): MR13 = %x \n", glbl_lp4_mr13);
      DEBUG_PRINTF("INFO: set_freq_globals(): MR14 = %x \n", glbl_lp4_mr14);
      DEBUG_PRINTF("INFO: set_freq_globals(): MR23 = %x \n", glbl_lp4_mr23);
      DEBUG_PRINTF("INFO: set_freq_globals(): DQS timer count = %d\n", glbl_lp4_dqs_timer_cnt);
   #endif

   glbl_add_cmd_delay    = (glbl_freq_f1 == 0) ? F0_ADD_CMD_DELAY    : F1_ADD_CMD_DELAY;
   glbl_add_cmd_delay_en = (glbl_freq_f1 == 0) ? F0_ADD_CMD_DELAY_EN : F1_ADD_CMD_DELAY_EN;

   glbl_clkfbout_lt = (glbl_freq_f1 == 0) ? XPLL_F0_CLKFBOUT_LT : XPLL_F1_CLKFBOUT_LT;
   glbl_clkfbout_ht = (glbl_freq_f1 == 0) ? XPLL_F0_CLKFBOUT_HT : XPLL_F1_CLKFBOUT_HT;

   glbl_divclk_lt = (glbl_freq_f1 == 0) ? XPLL_F0_DIVCLK_LT : XPLL_F1_DIVCLK_LT;
   glbl_divclk_ht = (glbl_freq_f1 == 0) ? XPLL_F0_DIVCLK_HT : XPLL_F1_DIVCLK_HT;

   DEBUG_PRINTF("INFO: set_freq_globals(): XPLL_CLKFBOUT_LT = %x \n", glbl_clkfbout_lt);
   DEBUG_PRINTF("INFO: set_freq_globals(): XPLL_CLKFBOUT_HT = %x \n", glbl_clkfbout_ht);
   DEBUG_PRINTF("INFO: set_freq_globals(): XPLL_DIVCLK_LT = %x \n", glbl_divclk_lt);
   DEBUG_PRINTF("INFO: set_freq_globals(): XPLL_DIVCLK_HT = %x \n", glbl_divclk_ht);

   glbl_reg_config1 = (glbl_freq_f1 == 0) ? F0_REG_CONFIG1 : F1_REG_CONFIG1;
   glbl_reg_config2 = (glbl_freq_f1 == 0) ? F0_REG_CONFIG2 : F1_REG_CONFIG2;
   glbl_reg_scrub2  = (glbl_freq_f1 == 0) ? F0_REG_SCRUB2  : F1_REG_SCRUB2;
#ifdef LPDDR4
   glbl_reg_mrs_2   = (glbl_freq_f1 == 0) ? F0_REG_MRS_2   : F1_REG_MRS_2;
#endif

#if MODE_FREQ_SWITCH == CAL_EN
   if(glbl_cal_done == 0) { // Values to be loaded during power up calibration for dual frequency usecase.
      // Load the respective complex config2 values for Complex calibration stage
      u32 reg_cplx_config2 = (glbl_freq_f1 == 0) ? F0_CPLX_CONFIG2 : F1_CPLX_CONFIG2;
      writeIO_DMC(ral_addr_of_cplx_config2_in_ddrmc_main_ddrmc_main_bank(BLK), reg_cplx_config2);

      // Load the respective tRFC & tREFI values for PRBS read stage
      u32 reg_safe_config7 = (glbl_freq_f1 == 0) ? F0_REG_SAFE_CONFIG7 : F1_REG_SAFE_CONFIG7;
      writeIO_DMC(ral_addr_of_reg_safe_config7_in_ddrmc_main_ddrmc_main_bank(BLK), reg_safe_config7);

      u32 reg_safe_config6 = (glbl_freq_f1 == 0) ? F0_REG_SAFE_CONFIG6 : F1_REG_SAFE_CONFIG6;
      writeIO_DMC(ral_addr_of_reg_safe_config6_in_ddrmc_main_ddrmc_main_bank(BLK), reg_safe_config6);

   #ifdef TEST_VNC
      // Load DC timing parameters
      writeIO_DMC(ral_addr_of_reg_safe_config0_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_freq_f1 == 0) ? F0_REG_SAFE_CONFIG0 : F1_REG_SAFE_CONFIG0);
      writeIO_DMC(ral_addr_of_reg_safe_config1_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_freq_f1 == 0) ? F0_REG_SAFE_CONFIG1 : F1_REG_SAFE_CONFIG1);
      writeIO_DMC(ral_addr_of_reg_safe_config2_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_freq_f1 == 0) ? F0_REG_SAFE_CONFIG2 : F1_REG_SAFE_CONFIG2);
      writeIO_DMC(ral_addr_of_reg_safe_config3_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_freq_f1 == 0) ? F0_REG_SAFE_CONFIG3 : F1_REG_SAFE_CONFIG3);
      writeIO_DMC(ral_addr_of_reg_safe_config4_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_freq_f1 == 0) ? F0_REG_SAFE_CONFIG4 : F1_REG_SAFE_CONFIG4);
      writeIO_DMC(ral_addr_of_reg_safe_config5_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_freq_f1 == 0) ? F0_REG_SAFE_CONFIG5 : F1_REG_SAFE_CONFIG5);
      writeIO_DMC(ral_addr_of_reg_safe_config8_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_freq_f1 == 0) ? F0_REG_SAFE_CONFIG8 : F1_REG_SAFE_CONFIG8);

      writeIO_DMC(ral_addr_of_reg_config3_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_freq_f1 == 0) ? F0_REG_CONFIG3 : F1_REG_CONFIG3);
      writeIO_DMC(ral_addr_of_reg_config4_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_freq_f1 == 0) ? F0_REG_CONFIG4 : F1_REG_CONFIG4);

      writeIO_DMC(ral_addr_of_reg_retry_0_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_freq_f1 == 0) ? F0_REG_RETRY_0 : F1_REG_RETRY_0);
      writeIO_DMC(ral_addr_of_reg_retry_1_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_freq_f1 == 0) ? F0_REG_RETRY_1 : F1_REG_RETRY_1);

      writeIO_DMC(ral_addr_of_reg_ref_1_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_freq_f1 == 0) ? F0_REG_REF_1 : F1_REG_REF_1);
      writeIO_DMC(ral_addr_of_reg_ref_3_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_freq_f1 == 0) ? F0_REG_REF_3 : F1_REG_REF_3);
      writeIO_DMC(ral_addr_of_reg_com_3_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_freq_f1 == 0) ? F0_REG_COM_3 : F1_REG_COM_3);

      writeIO_DMC(ral_addr_of_reg_mrs_0_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_freq_f1 == 0) ? F0_REG_MRS_0 : F1_REG_MRS_0);
      writeIO_DMC(ral_addr_of_reg_mrs_1_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_freq_f1 == 0) ? F0_REG_MRS_1 : F1_REG_MRS_1);
      writeIO_DMC(ral_addr_of_reg_mrs_7_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_freq_f1 == 0) ? F0_REG_MRS_7 : F1_REG_MRS_7);

      writeIO_DMC(ral_addr_of_xpi_oe_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_freq_f1 == 0) ? F0_XPI_OE : F1_XPI_OE);
      writeIO_DMC(ral_addr_of_xpi_dqs_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_freq_f1 == 0) ? F0_XPI_DQS : F1_XPI_DQS);
   #endif
   }
#endif

   return 0;
}

u8 cal_freq_switch () {
   u8 status;

   if(glbl_cal_done == 0) { // Extra check
      DEBUG_PRINTF("INFO: cal_freq_switch(): Save/Restore - Save values\n");
      status = ddr_save_val();
      if(status != 0) {
         DEBUG_PRINTF("ERROR: cal_freq_switch(): Save/Restore - Save values: Failed\n");
         return 1;
      }
   }

   DEBUG_PRINTF("INFO: cal_freq_switch(): Self Refresh entry\n");
   status = self_ref_entry(); 
   if(status != 0) {
      DEBUG_PRINTF("ERROR: cal_freq_switch(): Self Refresh entry: Failed\n");
      return 1;
   }

   DEBUG_PRINTF("INFO: cal_freq_switch(): Frequncy switching\n");
   status = ddr_freq_switch();
   if(status != 0) {
      DEBUG_PRINTF("ERROR: cal_freq_switch(): Frequncy switching: Failed\n");
      return 1;
   }

   DEBUG_PRINTF("INFO: cal_freq_switch(): Self Refresh exit\n");
   status = self_ref_exit();
   if(status != 0) {
      DEBUG_PRINTF("ERROR: cal_freq_switch(): Self Refresh exit: Failed\n");
      return 1;
   }

   return 0;
}

u8 ddr_freq_switch () {
#ifndef VNC_DIS_FS_PD
   u8 xpll;
   u8 rank;
   u8 nibble;
   u8 phy_nibble;
   u8 pause_rdy;
   u16 calib_ctrl;
   u32 wait_cnt;
   u8 xpll_nibble;
   u8 status = 0;
   u16 nibble_ctrl2;
   static u8 xpll_master[6] = XPLL_MASTER;
   static u8 xpll_slave[6] = {0, 0, 0, 0, 0, 0}; // XPLL_SLAVE;
   u32 WATCH_DOG_COUNT = 5000; // maximum possible time from self_calibrate to fixdly_rdy from XPHY

   if(glbl_phy_rank_switch_en == 1) {
      DEBUG_PRINTF("INFO: ddr_freq_switch(): Disabling dynamic mode for TX\n");
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
      riu_reg_bcast(DIS_DYN_MODE, 0, 0x1);
   }

   // Set the MRx global variables
   set_freq_globals();

   if(glbl_cal_done == 1) {
      DEBUG_PRINTF("INFO: ddr_freq_switch(): Stop Read DQS tracking\n");
      nibble_ctrl2 = read_riu_nibble_reg(glbl_dqs_bits_riu_map[0][0], DUP_NIBBLE_CTRL2);
      nibble_ctrl2 |= (1 << DIS_DQS_TRACK_POS);
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dqs_nibble_mask_initial);
      riu_reg_bcast(DUP_NIBBLE_CTRL2, 0, nibble_ctrl2);  // Disable read dqs gate tracking - bit 7

      if(glbl_bisc_en == 1) {
         DEBUG_PRINTF("INFO: ddr_freq_switch(): Set BISC pause and Clear en_vtc\n");
         writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
         riu_reg_bcast(WL_TRAIN, 0, (0x1 << BISC_PAUSE_POS)); // bisc_pause = 1, en_vtc = 0;

         DEBUG_PRINTF("INFO: ddr_freq_switch(): Polling for pause_rdy\n");
         glbl_error_phy_nibble = 0;
         wait_cnt = 0;
         for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
            phy_nibble = glbl_map_riu[nibble];
            do {
               calib_ctrl = read_riu_nibble_reg(phy_nibble, CFG_CALIB_CTRL);
               pause_rdy = (calib_ctrl >> PAUSE_RDY_POS) & 0x1;
               DEBUG_PRINTF("INFO: ddr_freq_switch(): Polling for pause_rdy = %d for nibble = %d. wait_cnt = %d \n", pause_rdy, phy_nibble, wait_cnt);
               wait_cnt++;
            } while ((pause_rdy != 1) && (wait_cnt < WATCH_DOG_COUNT));

            if(pause_rdy != 1) {
               DEBUG_PRINTF("ERROR: ddr_freq_switch(): Timeout ERROR waiting for PHY_RDY for nibble = %d\n", phy_nibble);
               glbl_error_phy_nibble |= (0x1 << phy_nibble);
            }
         }
         if(glbl_error_phy_nibble != 0) {
            write_error_nibble_XRAM(1, glbl_error_phy_nibble);
            write_error_XRAM(ERR_CODE_96_FREQ_SWITCH_BISC_PAUSE_READY_TIMEOUT);
            return 1;
         }
      }
   }

   DEBUG_PRINTF("INFO: ddr_freq_switch(): De-assert the clkoutphy_en\n");
   pll_clkoutphy_dis();

   // Apply Reset to Slave XPLLs
   for(xpll=0; xpll<6; xpll++) {
      if(xpll_slave[xpll] == 1) {
         xpll_nibble = BANK0_PLL0_RPI_NIBBLE + xpll;
         DEBUG_PRINTF("INFO: ddr_freq_switch(): Apply Slave XPLL Reset: xpll_nibble = %d, initstate = 1\n", xpll_nibble);
         write_riu_nibble_reg(xpll_nibble, XPLL_PCSR_MASK, XPLL_PCSR_INITSTATE);
         write_riu_nibble_reg(xpll_nibble, XPLL_PCSR_CONTROL, XPLL_PCSR_INITSTATE);
      }
   }

   // Apply Reset to Master XPLL
   for(xpll=0; xpll<6; xpll++) {
      if(xpll_master[xpll] == 1) {
         xpll_nibble = BANK0_PLL0_RPI_NIBBLE + xpll;
         DEBUG_PRINTF("INFO: ddr_freq_switch(): Apply Master XPLL Reset: xpll_nibble = %d, initstate = 1\n", xpll_nibble);
         write_riu_nibble_reg(xpll_nibble, XPLL_PCSR_MASK, XPLL_PCSR_INITSTATE);
         write_riu_nibble_reg(xpll_nibble, XPLL_PCSR_CONTROL, XPLL_PCSR_INITSTATE);
      }
   }

   // Program and wait for XPLL Locks
   status = pll_clkoutphy_en(0);
   if(status != 0) {
      DEBUG_PRINTF("ERROR: ddr_freq_switch(): pll clloutphy enable\n");
      return 1;
   }

   DEBUG_PRINTF("INFO: ddr_freq_switch(): Wait for XPHY self calibration\n");
   status = ddr_self_calibrate(0);
   if(status != 0) {
      DEBUG_PRINTF("ERROR: ddr_freq_switch(): Self calibration en_vtc\n");
      return 1;
   }

   if(glbl_cal_done == 1) {
      DEBUG_PRINTF("INFO: ddr_freq_switch(): Restore calibration values\n");
      ddr_restore_val();

      DEBUG_PRINTF("INFO: ddr_freq_switch(): Start en_vtc\n");
      status = ddr_en_vtc();
      if(status != 0) {
         DEBUG_PRINTF("ERROR: ddr_freq_switch(): Start en_vtc\n");
         return 1;
      }
   }

#endif // VNC_DIS_FS_PD
   return 0;
}

u8 cal_restore () {
#ifndef VNC_DIS_FS_PD
   u8 status = 0;
   glbl_cal_done = 1;

   glbl_heartbeat = (glbl_heartbeat == 0) ? 1 : 0;
   glbl_gpo3 = glbl_heartbeat ? (glbl_gpo3 | (1 << GPO3_HEARTBEAT_POS)) : (glbl_gpo3 & ~(1 << GPO3_HEARTBEAT_POS));
   writeIO_IOM(IO_GPO3, glbl_gpo3);

   DEBUG_PRINTF("INFO: cal_restore(): Wait for XPHY self calibration\n");
   status = ddr_self_calibrate(1);
   if(status != 0) {
      DEBUG_PRINTF("ERROR: cal_restore(): Self calibration en_vtc\n");
      return 1;
   }

   #ifdef DDR4
      writeIO_DMC(ral_addr_of_seq_cmd_por_in_ddrmc_main_ddrmc_main_bank(BLK), 0x5<<4); // Make the Power on Reset_n as 1, CKE as 0, CS_n as 1 
      writeIO_DMC(ral_addr_of_seq_cmd_default_in_ddrmc_main_ddrmc_main_bank(BLK), 0x5<<4); // Make default value of Reset_n as 1, CKE as 0, CS as 1
   #else
      writeIO_DMC(ral_addr_of_seq_cmd_por_in_ddrmc_main_ddrmc_main_bank(BLK), 0x4<<4); // Make the Power on Reset_n as 1, CKE as 0, CS as 0
      writeIO_DMC(ral_addr_of_seq_cmd_default_in_ddrmc_main_ddrmc_main_bank(BLK), 0x4<<4); // Make default value of Reset_n as 1, CKE as 0, CS as 0
   #endif

   if(glbl_cal_done == 1) {
      DEBUG_PRINTF("INFO: cal_restore(): Restore calibration values\n");
      ddr_restore_val();

      DEBUG_PRINTF("INFO: cal_restore(): Start en_vtc\n");
      status = ddr_en_vtc();
      if(status != 0) {
         DEBUG_PRINTF("ERROR: cal_restore(): Start en_vtc\n");
         return 1;
      }
   }

   glbl_heartbeat = (glbl_heartbeat == 0) ? 1 : 0;
   glbl_gpo3 = glbl_heartbeat ? (glbl_gpo3 | (1 << GPO3_HEARTBEAT_POS)) : (glbl_gpo3 & ~(1 << GPO3_HEARTBEAT_POS));

   // Set the calibration done and restore done in IOM
   glbl_cal_status = (CAL_PASSED | (CAL_DONE & 0x3F));
   glbl_gpo3 = (glbl_gpo3 & GPO3_CAL_STATUS_CLR) | (glbl_cal_status << GPO3_CAL_STATUS_POS) | GPO3_CAL_DONE | GPO3_RESTORE_DONE;
   writeIO_IOM(IO_GPO3, glbl_gpo3);

#endif // VNC_DIS_FS_PD
   return 0;
}

u8 dis_mission_mode_tracking () {
   u8 scrub_busy;
   u8 scrub_stop;
   u32 loop=0;

#ifdef LPDDR4
   // Mask LP4 oscillator interrupt. It is already done as a part of interrupt subroutine
   // Disable LP4 oscillator tracking
   writeIO_DMC(ral_addr_of_reg_mrs_2_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_reg_mrs_2 & ~(0x3)));
#endif

   // Disable background scrub
   if(glbl_reg_scrub2 & 0x5) { 
      scrub_stop = (glbl_reg_scrub2 << 1);
      writeIO_DMC(ral_addr_of_reg_scrub2_in_ddrmc_main_ddrmc_main_bank(BLK), scrub_stop);

      // Wait for 1us
      glbl_ddrmc_clk_cnt = (1000000 / (glbl_tck * 2));
      writeIO_DMC(ral_addr_of_ddrmc_clk_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), ((0x1 << 31) | glbl_ddrmc_clk_cnt));

      while (glbl_ddrmc_clk_cnt != 0) {
         glbl_ddrmc_clk_cnt = (readIO_DMC(ral_addr_of_ddrmc_clk_cnt_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & ~(0x1 << 31)); // Exclude busy bit
      }

      // Poll for scrub not busy
      do {
         scrub_busy = readIO_DMC(ral_addr_of_reg_scrub3_in_ddrmc_main_ddrmc_main_bank(BLK)) & 0x5;

         if(loop > 10000) { // Watchdog
            write_error_XRAM(ERR_CODE_103_WATCHDOG_TIMEOUT_POLL_SCRUB_BUSY_AFTER_SCRUB_STOP);
            DEBUG_PRINTF("ERROR: dis_mission_mode_tracking(): Timeout polling for scrub busy after disabling the scrubbing");
            return 1;
         }
         loop++;
      } while(scrub_busy != 0);

      // Record the address where it stopped the scrubbing
      glbl_reg_scrub4 = readIO_DMC(ral_addr_of_reg_scrub4_in_ddrmc_main_ddrmc_main_bank(BLK));
      glbl_reg_scrub5 = readIO_DMC(ral_addr_of_reg_scrub5_in_ddrmc_main_ddrmc_main_bank(BLK));
      glbl_reg_scrub6 = readIO_DMC(ral_addr_of_reg_scrub6_in_ddrmc_main_ddrmc_main_bank(BLK));
      glbl_reg_scrub7 = readIO_DMC(ral_addr_of_reg_scrub7_in_ddrmc_main_ddrmc_main_bank(BLK));
   }

   // Disable periodic read
   writeIO_DMC(ral_addr_of_reg_config2_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_reg_config2 & ~(0x1)));

   // Disable refresh. It can be done only after DC raises self_ref_entry request
   // writeIO_DMC(ral_addr_of_reg_config1_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_reg_config1 & ~(0x6)));

   return 0;
}

u8 en_mission_mode_tracking () {
   u32 scrub_busy;
   u32 scrub_done;
   u32 mem_fill_busy;
   u32 loop=0;
   u32 mem_fill_scrub2 = glbl_num_ch ? 0x5 : 0x1; // glbl_reg_scrub2;
   u32 user_refresh = glbl_reg_config1 & 0x1; // ref_mode = 1
   u32 reg_config1 = glbl_num_ch ? 0x6 : 0x2; // [2:1]
   u32 dc_par_err_en;

   // Workaround for EDT-1008424, CR-1077790
   dc_par_err_en = readIO_DMC(ral_addr_of_dc_par_err_en_in_ddrmc_main_ddrmc_main_bank(BLK));
   dc_par_err_en &= ~(0x00009090); // Disable parity logging. Clear 4, 7, 12 & 15 bits
   writeIO_DMC(ral_addr_of_dc_par_err_en_in_ddrmc_main_ddrmc_main_bank(BLK), dc_par_err_en);

   // Enable refresh
   if(user_refresh) { // User refresh case
      if((glbl_cal_done == 0) || (glbl_restore_cal_en == 1)) { // Enable internal refresh for power up calibration or self refresh power down exit scenarios
         glbl_ref_speed = (glbl_ref_speed == 0) ? 0x1 : glbl_ref_speed; // At least 2x refresh_speed for user refresh configuration
         writeIO_DMC(ral_addr_of_reg_ref_0_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_ref_speed);
         writeIO_DMC(ral_addr_of_reg_config1_in_ddrmc_main_ddrmc_main_bank(BLK), reg_config1);
      } // Else do nothing, for regular self refresh & frequency switching scenarios
   } else { // Internal refresh case
      writeIO_DMC(ral_addr_of_reg_config1_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_reg_config1);
   }

   //if(glbl_cal_done == 0) { // Only during power up calibration  //aswin
   if(glbl_cal_done == 0 || glbl_margin_check == 1) { // Only during power up calibration
      if((glbl_ecc_en == 1) && (DIS_MEM_FILL == 0)) { // Mem-fill for ECC configuration
         if(mem_fill_scrub2 & 0x5) { // Scrub is enabled
            writeIO_DMC(ral_addr_of_reg_scrub8_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1); // Mode = mem-fill, loop = once
            writeIO_DMC(ral_addr_of_reg_scrub1_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1); // scrub interval = 1

            writeIO_DMC(ral_addr_of_reg_scrub2_in_ddrmc_main_ddrmc_main_bank(BLK), mem_fill_scrub2); // Trigger scrub for mem-fill

            // Poll for scrub done
            loop = 0;
            do {
               scrub_done = readIO_DMC(ral_addr_of_reg_scrub3_in_ddrmc_main_ddrmc_main_bank(BLK)) & 0xA;

               // if(loop > 10000) { // Watchdog
               //    write_error_XRAM(ERR_CODE_104_WATCHDOG_TIMEOUT_POLL_SCRUB_BUSY_AFTER_SCRUB_ENABLE);
               //    DEBUG_PRINTF("ERROR: en_mission_mode_tracking(): Timeout polling for scrub busy after enabling the scrubbing");
               //    return 1;
               // }

               loop++;
            } while(scrub_done != (mem_fill_scrub2 << 1));

            // Poll for mem_fill_busy to be de-asserted
            do {
               mem_fill_busy = (readIO_DMC(ral_addr_of_reg_scrub3_in_ddrmc_main_ddrmc_main_bank(BLK)) >> 4) & 0x3;
            } while(mem_fill_busy != 0);

            writeIO_DMC(ral_addr_of_reg_scrub1_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_reg_scrub1);
            writeIO_DMC(ral_addr_of_reg_scrub8_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_reg_scrub8);
         }
      }
   }

   // Enable periodic read
   writeIO_DMC(ral_addr_of_reg_config2_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_reg_config1);

   // Enable backgroung scrub
   if(glbl_reg_scrub2 & 0x5) { 
      // Start from where it stopped
      writeIO_DMC(ral_addr_of_reg_scrub4_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_reg_scrub4);
      writeIO_DMC(ral_addr_of_reg_scrub5_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_reg_scrub5);
      writeIO_DMC(ral_addr_of_reg_scrub6_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_reg_scrub6);
      writeIO_DMC(ral_addr_of_reg_scrub7_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_reg_scrub7);

      writeIO_DMC(ral_addr_of_reg_scrub2_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_reg_scrub2);

      // Poll for scrub busy: Not needed since MC is expected to be IDLE
      // loop = 0;
      // do {
      //    scrub_busy = readIO_DMC(ral_addr_of_reg_scrub3_in_ddrmc_main_ddrmc_main_bank(BLK)) & 0x5;

      //    if(loop > 10000) { // Watchdog
      //       write_error_XRAM(ERR_CODE_104_WATCHDOG_TIMEOUT_POLL_SCRUB_BUSY_AFTER_SCRUB_ENABLE);
      //       DEBUG_PRINTF("ERROR: en_mission_mode_tracking(): Timeout polling for scrub busy after enabling the scrubbing");
      //       return 1;
      //    }
      //    loop++;
      // } while(scrub_busy != glbl_reg_scrub2);
   }

#ifdef LPDDR4
   // Interrupt is un-masked as needed
   // Enable LP4 oscillator tracking
   writeIO_DMC(ral_addr_of_reg_mrs_2_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_reg_mrs_2);
#endif
   return 0;
}

u8 self_ref_entry () {
#ifndef VNC_DIS_SELF_REF
   u8 rank;
   u8 lrank;
   u8 status;
   u16 seq_a_rank;
   u32 isr;
   u32 loop=0;
   u32 gpi1 = readIO_IOM(IO_GPI1);
   u8 freq_switch = (gpi1 >> 2) & 0x1; // Switching to different frequency
   u8 target_freq = (gpi1 >> 1) & 0x1; // GPI1 Bit-1 is the frequency setting
   glbl_refresh_en = glbl_cal_done; // Enable refresh only after Power On Reset calibration
   glbl_freq_sre = glbl_freq_f1;

#ifdef FIX_EDT_999807
   if(glbl_cal_done == 1) {
      status = dis_mission_mode_tracking();
      if(status) {
         return 1;
      }

      // Set self-refresh entry REQ to DC
      writeIO_DMC(ral_addr_of_reg_com_0_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);

      // Poll for self-refresh entry request from DC
      do {
         isr = readIO_IOM(IO_IRQ_STATUS);
         isr &= (0x1 << (SELF_REF_ENTRY_REQ + IRQ_OFFSET));

	 if(loop > 10000) { // Watchdog
            write_error_XRAM(ERR_CODE_105_WATCHDOG_TIMEOUT_POLL_SELF_REF_ENTRY_REQ_FROM_DC_FSM);
            return 1;
	 }
	 loop++;
      } while(isr == 0x0);

      // Acknowledgement to DC
      glbl_gpo4 = glbl_gpo4 | (0x1 << SELF_REF_ENTRY_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);
      wait_intr_src_clr(); // Stretch for sampling by dmc_clk
      glbl_gpo4 = glbl_gpo4 & ~(0x1 << SELF_REF_ENTRY_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);

      // Disable refresh. It can be done only after DC raises self_ref_entry request
      if(freq_switch) { // It is required for frequency switching case only.
         writeIO_DMC(ral_addr_of_reg_config1_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_reg_config1 & ~(0x6)));
      }

      // Clear the interrupt
      wait_intr_src_clr();
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (SELF_REF_ENTRY_REQ + IRQ_OFFSET));
   }
#endif

   // Set the global variables as per new frequency
   if(glbl_cal_done == 1) {
      glbl_freq_switch = (target_freq != glbl_freq_sre) ? 1 : 0;
      glbl_freq_f1 = target_freq;
   } else {
      glbl_freq_switch = 1;
      glbl_freq_f1 = (glbl_freq_f1 == 0) ? 1 : 0;
   }

   DEBUG_PRINTF("INFO: self_ref_entry(): De-assert CAL Done\n");
   writeIO_DMC(ral_addr_of_cal_status_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);

   // Enable the ALERT interrupts while cal_done = 0
   // writeIO_IOM(IO_IRQ_ENABLE, IRQ_EN);

   if(glbl_cal_done) {
      DEBUG_PRINTF("INFO: self_ref_entry(): Wait for tRFC time, to maintain refresh to valid command timing\n");
      // Wait for tRFC time, since memory controller would have issued refresh just before raising the self refresh entry request
      glbl_ddrmc_clk_cnt = (glbl_t_rfc_slr / 2);
      writeIO_DMC(ral_addr_of_ddrmc_clk_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), ((0x1 << 31) | glbl_ddrmc_clk_cnt));

      while (glbl_ddrmc_clk_cnt != 0) {
         glbl_ddrmc_clk_cnt = (readIO_DMC(ral_addr_of_ddrmc_clk_cnt_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & ~(0x1 << 31)); // Exclude busy bit
      }

      DEBUG_PRINTF("INFO: self_ref_entry(): Precharge all the memory banks\n");
      #ifdef DDR4
         // Precharge the banks
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_ROW_PREACHARGE);
         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x400); // ALl banks precharge
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

         for(rank=0; rank<RANKS; rank++) {
            writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10<<rank); // Selects DDR Mem rank
            writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

            if(glbl_3ds_en) {
               for(lrank=1; lrank<glbl_3ds_lranks; lrank++) {
                  writeIO_DMC(ral_addr_of_seq_a_cbit_in_ddrmc_main_ddrmc_main_bank(BLK), lrank); // Selects 3DS Logical rank
                  if(lrank >= 4) {
                     writeIO_DMC(ral_addr_of_seq_3ds_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1); // EDT-988720. Select high 4 logical ranks
                  }
                  writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
               }
               writeIO_DMC(ral_addr_of_seq_a_cbit_in_ddrmc_main_ddrmc_main_bank(BLK), 0); // Keep the default value
               writeIO_DMC(ral_addr_of_seq_3ds_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0); // Keep the default value
            }
         }
      #else
         // Precharge the banks
         for(rank=0; rank<RANKS; rank++) {
            writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10<<rank); // Selects DDR Mem rank
            if(rank == 0) {
               lp4_bank_row_access(rank /*u8 rank*/, PRECHARGE /*u8 cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);
            } else {
               writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
               while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
            }
         }
      #endif

//      if(glbl_ref_mode_2x | glbl_ref_mode_4x) { // 2x or 4x modes
//         DEBUG_PRINTF("INFO: self_ref_entry(): Changing refresh mode to normal (1x) mode\n");
//         #ifdef DDR4
//            glbl_mr3 = glbl_mr3 & ~(0x7 << 6); // MR3[8:6] = 0. Moving to Normal mode (1x)
//
//            writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
//            writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
//            writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
//            for(rank=0; rank<RANKS; rank++) {
//               writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK), rank); // Selects xphy rank
//               ddr_mrs_write (rank, 0x3,  glbl_mr3);
//            }
//         #else
//            glbl_lp4_mr4 = (glbl_lp4_mr4 & ~(0x7)) | 0x3; // MR4[2:0] = 3. Moving to Normal mode (1x)
//
//            for(rank=0; rank<RANKS; rank++) {
//               lp4_mode_reg_wr(rank, 4, glbl_lp4_mr4);
//            }
//         #endif
//      }

      DEBUG_PRINTF("INFO: self_ref_entry(): Send Refresh commands to memory\n");
      glbl_fix_ref_en = 1;
      glbl_fix_num_ref = (glbl_ref_mode_4x ? 4 : (glbl_ref_mode_2x ? 2 : 1)); // 1 refresh
      glbl_fix_num_ref = glbl_fix_num_ref * ((glbl_tck >= TCK_TH_REF_SRE) ? 2 : 1); // Double the refresh for lower frequencies
      status = banks_refresh(0, 0x1);
      if(status) {
         return 1;
      }
   }

   seq_a_rank = 0;
   for(rank=0; rank<RANKS; rank++) {
      seq_a_rank |= (0x10 << rank);
   }

   #ifdef DDR4
   if(freq_switch || (!glbl_cal_done)) {
      for(rank=0; rank<RANKS; rank++) {
         ddr_mrs_write (rank, 0x5,  glbl_mr5[rank]&0xFFF8);// Disable the parity latency before switching to the new parity latency
      }
   }
   #endif       
   writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
   
   #ifdef DDR4
      // Program the frequency settings for LRDIMM Data Buffer: BC0A & F0BC6X
      if((freq_switch || (!glbl_cal_done)) & glbl_lrdimm_en) {
         DEBUG_PRINTF("INFO: self_ref_entry(): Programming frequency settings for LRDIMM Data Buffer\n");
         for(rank=0; rank<RANKS; rank += RANKS/SLOTS) {
            glbl_db_bc0a = (glbl_freq_f1 == 0) ? F0_DB_BC0A : F1_DB_BC0A;
            glbl_db_f0bc6x = (glbl_freq_f1 == 0) ? F0_DB_F0BC6X : F1_DB_F0BC6X;
            DEBUG_PRINTF("INFO: self_ref_entry(): DB_BC0A = %x \n", glbl_db_bc0a);
            DEBUG_PRINTF("INFO: self_ref_entry(): DB_F0BC6X = %x \n", glbl_db_f0bc6x);

            ddr_mrs7_write (rank, 0x10A0 | glbl_db_bc0a); // BC0A
            ddr_mrs7_write (rank, 0x1600 | glbl_db_f0bc6x); // F0BC6X
         }
      }

      // Issuing self refresh entry command
      DEBUG_PRINTF("INFO: self_ref_entry(): Issue self refresh entry command to DRAM\n");
      if(glbl_ddr4_2t) {
         writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), 0); // To align CKE & CS timing in 2T mode 
      }
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (0x100 | seq_a_rank)); // Drive CKE
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), ((0x2 << 5) | MEM_SELF_REF_ENTRY)); // CKE=0

      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

      writeIO_DMC(ral_addr_of_seq_cmd_default_in_ddrmc_main_ddrmc_main_bank(BLK), 0x5<<4); // Make default value of Reset_n as 1, CKE as 0, CS as 1
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10); // Remove Drive CKE

      // Program the frequency settings for RDIMM: RC0A & RC3X
      if((freq_switch || (!glbl_cal_done)) & glbl_rdimm_en) {
         DEBUG_PRINTF("INFO: self_ref_entry(): Programming frequency settings for RCD chip\n");
         for(rank=0; rank<RANKS; rank += RANKS/SLOTS) {
            glbl_rcd_rc0a = (glbl_freq_f1 == 0) ? F0_RCD_RC0A : F1_RCD_RC0A;
            glbl_rcd_rc3x = (glbl_freq_f1 == 0) ? F0_RCD_RC3X : F1_RCD_RC3X;
            DEBUG_PRINTF("INFO: self_ref_entry(): RCD_RC0A = %x \n", glbl_rcd_rc0a);
            DEBUG_PRINTF("INFO: self_ref_entry(): RCD_RC3X = %x \n", glbl_rcd_rc3x);

            ddr_mrs7_write (rank, 0x00A0 | glbl_rcd_rc0a); // RC0A
            ddr_mrs7_write_opt (0x0300 | glbl_rcd_rc3x); // RC3X
         }
      }

      if(freq_switch || (!glbl_cal_done)) { // Frequency switching case
         DEBUG_PRINTF("INFO: self_ref_entry(): disabling alert_en since CK is going stop toggling\n");
         writeIO_DMC(ral_addr_of_xpi_pmi_config_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_xpi_pmi_config & ~(0xC)); // alert_en [3:2]

         DEBUG_PRINTF("INFO: self_ref_entry(): Stop toggeling the CK pattern, only in case of frequency switching\n");
         writeIO_DMC(ral_addr_of_default_pattern_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
      }

   #else
      // Issuing self refresh entry command
      DEBUG_PRINTF("INFO: self_ref_entry(): Issue self refresh entry command to DRAM\n");
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), seq_a_rank);
      writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //0: 2 commands per sequence; 1: 4 commands per sequence
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0x075); // Non-Data command
      writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), LP4_SELF_REF_ENTRY);
   
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

      writeIO_DMC(ral_addr_of_seq_cmd_default_in_ddrmc_main_ddrmc_main_bank(BLK), 0x4<<4); // Make default value of Reset_n as 1, CKE as 0, CS as 0
      writeIO_DMC(ral_addr_of_default_pattern_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10);
   #endif

   DEBUG_PRINTF("INFO: self_ref_entry(): Self Refresh Entry: Done\n");
#endif // VNC_DIS_SELF_REF
   return 0;
}

u8 self_ref_exit () {
#ifndef VNC_DIS_SELF_REF
   u8 rank;
   u32 isr;
   u8 srx_done;
   u32 loop = 0;
   u8 status = 0;
   u16 seq_a_rank;
   u32 target_ddrmc_clk_cnt;
   u32 gpi1 = readIO_IOM(IO_GPI1);
   u8 restore_freq_switch = (gpi1 >> 2) & gpi1 & 0x1; // Restoring in different frequency
   u8 dc_fsm_sr = (readIO_DMC(ral_addr_of_reg_com_4_in_ddrmc_main_ddrmc_main_bank(BLK)) >> 2) & 0x1;

#ifdef FIX_EDT_999807
   if(dc_fsm_sr == 1) {
      // Set self-refresh exit REQ to DC
      writeIO_DMC(ral_addr_of_reg_com_0_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10);

      // Poll for self-refresh exit request from DC FSM
      do {
         isr = readIO_IOM(IO_IRQ_STATUS);
         isr &= (0x1 << (SELF_REF_EXIT_REQ + IRQ_OFFSET));

	 if(loop > 1000) { // Watchdog
            write_error_XRAM(ERR_CODE_106_WATCHDOG_TIMEOUT_POLL_SELF_REF_EXIT_REQ_FROM_DC_FSM);
            return 1;
	 }
	 loop++;
      } while(isr == 0x0);

      // Acknowledgement to DC
      glbl_gpo4 = glbl_gpo4 | (0x1 << SELF_REF_EXIT_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);
      wait_intr_src_clr(); // Stretch for sampling by dmc_clk
      glbl_gpo4 = glbl_gpo4 & ~(0x1 << SELF_REF_EXIT_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);

      // Clear the interrupt
      wait_intr_src_clr();
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (SELF_REF_EXIT_REQ + IRQ_OFFSET));
   }
#endif

   DEBUG_PRINTF("INFO: self_ref_exit(): Start toggeling the CK pattern, if not already done\n");
   writeIO_DMC(ral_addr_of_default_pattern_in_ddrmc_main_ddrmc_main_bank(BLK), 0xA5);

   // Set timing as per target frequency during frequency switching
   if((glbl_freq_switch == 1) || (restore_freq_switch == 1) || (glbl_restore_cal_en == 1) || (dc_fsm_sr == 0)) {
      set_timing();
      glbl_side_ref = 0; // Reload side registers with new tRFC values
   
   #ifdef DDR4
      // Preload the Programmable Interval Timer-1 with tSTAB time (required from stable CK_t/CK_c to CKE assertion)
      glbl_ddrmc_clk_cnt = (5000000 / (glbl_tck * 2)); // tSTAB = 5us
      writeIO_DMC(ral_addr_of_ddrmc_clk_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), ((0x1 << 31) | glbl_ddrmc_clk_cnt));

      while (glbl_ddrmc_clk_cnt != 0) {
         glbl_ddrmc_clk_cnt = (readIO_DMC(ral_addr_of_ddrmc_clk_cnt_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & ~(0x1 << 31)); // Exclude busy bit
      }

      DEBUG_PRINTF("INFO: self_ref_exit(): enabling alert_en since CK started toggling\n");
      writeIO_DMC(ral_addr_of_xpi_pmi_config_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_xpi_pmi_config); // alert_en [3:2]
   #endif
   }

   DEBUG_PRINTF("INFO: self_ref_exit(): Issue self refresh exit command to DRAM\n");
   seq_a_rank = 0;
   for(rank=0; rank<RANKS; rank++) {
      seq_a_rank |= (0x10 << rank);
   }
   
   #ifdef DDR4
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), (0x100 | seq_a_rank)); // Drive CKE
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), ((0x3 << 5) | MEM_SELF_REF_EXIT)); // CKE=1

      writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

      writeIO_DMC(ral_addr_of_seq_cmd_default_in_ddrmc_main_ddrmc_main_bank(BLK), 0x7<<4); // Make default value of Reset_n as 1, CKE as 1, CS as 1
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10); // Remove Drive CKE

   #else
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), seq_a_rank);
      writeIO_DMC(ral_addr_of_seq_cmd_default_in_ddrmc_main_ddrmc_main_bank(BLK), 0x6<<4); // Make default value of Reset_n as 1, CKE as 1, CS as 0

      if(glbl_cal_done == 0) { // Only needed during calibration sequence
         glbl_lp4_mode_default = 1; // Disable the seq_mode updates during initialization
         writeIO_DMC(ral_addr_of_seq_ck_cal_in_ddrmc_main_ddrmc_main_bank(BLK),0xF0F0); // Drive LP4 slow clock
         writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 4); // Enable LP4 slow clock mode
      }

      writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK), 0); //0: 2 commands per sequence; 1: 4 commands per sequence
      writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), 0x075); // Non-Data command
      writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), LP4_SELF_REF_EXIT);
   
      writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
      writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
      while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));
   
      writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10);
   #endif

   // Preload the Programmable Interval Timer-1
   writeIO_DMC(ral_addr_of_ddrmc_clk_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), ((0x1 << 31) | DDRMC_CLK_CNT_LOAD));

   glbl_fix_ref_en = 0;
   glbl_num_ref_total = 0;
   glbl_num_ref_cur = 0;
   glbl_num_ref_prev = 0;
   glbl_num_ref_iter = 0;

   // Count for tXS time (tRFC + 10ns) in DDR4, (tRFC + 7.5ns) in LPDDR4
   glbl_ddrmc_clk_cnt = (readIO_DMC(ral_addr_of_ddrmc_clk_cnt_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & ~(0x1 << 31)); // Exclude busy bit
   target_ddrmc_clk_cnt = glbl_ddrmc_clk_cnt - (glbl_t_rfc_slr / 2); // Ignore 10ns, which is too small.

   while (glbl_ddrmc_clk_cnt > target_ddrmc_clk_cnt) {
      glbl_ddrmc_clk_cnt = (readIO_DMC(ral_addr_of_ddrmc_clk_cnt_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & ~(0x1 << 31)); // Exclude busy bit
   }

   if(glbl_cal_done) {
      DEBUG_PRINTF("INFO: self_ref_exit(): Send ZQ calibration short command to memory\n");
      #ifdef DDR4
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_ZQ_SHORT_CAL);
         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
         for(rank=0; rank<RANKS; rank++) {
            writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK),0x10<<rank); // Selects DDR Mem rank
            writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
            while(readIO_DMC(ral_addr_of_seq_status_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)));

            // Count for tZQCS (128 * tCK) time
            glbl_ddrmc_clk_cnt = (readIO_DMC(ral_addr_of_ddrmc_clk_cnt_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & ~(0x1 << 31)); // Exclude busy bit
            target_ddrmc_clk_cnt = glbl_ddrmc_clk_cnt - (128 / 2);

            while (glbl_ddrmc_clk_cnt > target_ddrmc_clk_cnt) {
               glbl_ddrmc_clk_cnt = (readIO_DMC(ral_addr_of_ddrmc_clk_cnt_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & ~(0x1 << 31)); // Exclude busy bit
            }
         }
      #else
         for(rank=0; rank<RANKS; rank++) {
            lp4_mpc_wr(rank, LP4_MPC_START_ZQCAL, 0, 0);
         }

         // Count for tZQCAL (1us) time
         glbl_ddrmc_clk_cnt = (readIO_DMC(ral_addr_of_ddrmc_clk_cnt_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & ~(0x1 << 31)); // Exclude busy bit
         target_ddrmc_clk_cnt = glbl_ddrmc_clk_cnt - ((1000000 / glbl_tck) / 2);

         while (glbl_ddrmc_clk_cnt > target_ddrmc_clk_cnt) {
            glbl_ddrmc_clk_cnt = (readIO_DMC(ral_addr_of_ddrmc_clk_cnt_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & ~(0x1 << 31)); // Exclude busy bit
         }

         for(rank=0; rank<RANKS; rank++) {
            lp4_mpc_wr(rank, LP4_MPC_LATCH_ZQCAL, 0, 0);
         }

         // Count for tZQLAT - Max of (30ns, 8*tCK) time
         glbl_ddrmc_clk_cnt = (readIO_DMC(ral_addr_of_ddrmc_clk_cnt_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & ~(0x1 << 31)); // Exclude busy bit
         target_ddrmc_clk_cnt = glbl_ddrmc_clk_cnt - ((30000 / glbl_tck) / 2); // 30ns would be the worst

         while (glbl_ddrmc_clk_cnt > target_ddrmc_clk_cnt) {
            glbl_ddrmc_clk_cnt = (readIO_DMC(ral_addr_of_ddrmc_clk_cnt_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & ~(0x1 << 31)); // Exclude busy bit
         }
      #endif
   }

   // DRAM MRx need to be programmed during frequency switching
   if((glbl_freq_switch == 1) || (restore_freq_switch == 1)) {
      // Program the memory with new MRx values
      ddr_init_f1();
   }

   if(glbl_cal_done) {
      // Since temperature controlled refresh rate is not known, set it to worst case refresh rate
   #ifdef DDR4
      if(glbl_ref_temp_sensor_en == 1) { // Temperature sensor is enabled
         glbl_ref_speed = 0x1; // 2x Refresh speed
         writeIO_DMC(ral_addr_of_reg_ref_0_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_ref_speed);
      }
   #else
      glbl_ref_speed = 0x2; // 4x Refresh speed (0.25x time)
      writeIO_DMC(ral_addr_of_reg_ref_0_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_ref_speed);
   #endif

      DEBUG_PRINTF("INFO: self_ref_exit(): Send Refresh commands to memory\n");
      status = banks_refresh(0, 0x1);
      if(status) {
         return 1;
      }
   }

   #ifdef DDR4
      // Count for tDLLK time. Max value is at 3200MBPS, which is  1024. // This can be reducded further for lower frequencies by considering the speed grade
      glbl_ddrmc_clk_cnt = (readIO_DMC(ral_addr_of_ddrmc_clk_cnt_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & ~(0x1 << 31)); // Exclude busy bit
      if((glbl_freq_switch == 1) || (restore_freq_switch == 1)) { 
         target_ddrmc_clk_cnt = glbl_ddrmc_clk_cnt - (1024 / 2); // Consider the count from now (MR0.DLL Reset in ddr_init_f1()) during frequency switching self refresh exit
      } else {
         target_ddrmc_clk_cnt = DDRMC_CLK_CNT_LOAD - (1024 / 2); // Consider the count from starting of self refresh exit
      }

      while (glbl_ddrmc_clk_cnt > target_ddrmc_clk_cnt) {
         glbl_ddrmc_clk_cnt = (readIO_DMC(ral_addr_of_ddrmc_clk_cnt_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & ~(0x1 << 31)); // Exclude busy bit
      }
   #endif

   DEBUG_PRINTF("INFO: self_ref_exit(): Send Refresh commands to memory\n");
   if(glbl_cal_done == 1) {
      DEBUG_PRINTF("INFO: self_ref_exit(): Trigger Mini-calibration\n");
      status = restore_mini_cal();
      if(status != 0) {
         return 1;
      }

      status = banks_refresh(0, 0x2);
      if(status) {
         return 1;
      }

//      if(glbl_ref_mode_2x | glbl_ref_mode_4x) { // 2x or 4x modes
//         DEBUG_PRINTF("INFO: self_ref_exit(): Changing refresh mode back to original mode\n");
//         #ifdef DDR4
//            glbl_mr3 = glbl_mr3 & ~(0x7 << 6);
//            if(glbl_ref_mode_2x) // 2x mode
//               glbl_mr3 = glbl_mr3 | (0x1 << 6);
//            else // 4x mode
//               glbl_mr3 = glbl_mr3 | (0x2 << 6);
//
//            writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
//            writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK), 0);
//            writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
//
//            for(rank=0; rank<RANKS; rank++) {
//               writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK), rank); // Selects xphy rank
//               ddr_mrs_write (rank, 0x3,  glbl_mr3);
//            }
//         #else
//            glbl_lp4_mr4 = glbl_lp4_mr4 & ~(0x7);
//            if(glbl_ref_mode_2x) // 2x mode
//               glbl_lp4_mr4 = glbl_lp4_mr4 | (0x2 << 6);
//            else // 4x mode
//               glbl_lp4_mr4 = glbl_lp4_mr4 | (0x1 << 6);
//
//            for(rank=0; rank<RANKS; rank++) {
//               lp4_mode_reg_wr(rank, 4, glbl_lp4_mr4);
//            }
//         #endif
//      }

      // Disable the ALERT interrupts while cal_done = 1
      // writeIO_IOM(IO_IRQ_ENABLE, (IRQ_EN & ~IRQ_EN_ALERT));

      glbl_fix_ref_en = 1;
      glbl_fix_num_ref = (glbl_ref_mode_4x ? 4 : (glbl_ref_mode_2x ? 2 : 1)); // 1 extra refresh before asserting cal_done
      status = banks_refresh(0, 0x3);
      glbl_fix_ref_en = 0;
      if(status) {
         return 1;
      }

      DEBUG_PRINTF("INFO: self_ref_exit(): Assert CAL Done\n");
      writeIO_DMC(ral_addr_of_cal_status_in_ddrmc_main_ddrmc_main_bank(BLK), 0x3);

#ifdef FIX_EDT_999807
   if(dc_fsm_sr == 1) { // Only for non power down cases
      // self refresh exit done to DC
      glbl_gpo4 = glbl_gpo4 | (0x1 << SELF_REF_EXIT_DONE);
      writeIO_IOM(IO_GPO4, glbl_gpo4);
      wait_intr_src_clr(); // Stretch for sampling by dmc_clk
      glbl_gpo4 = glbl_gpo4 & ~(0x1 << SELF_REF_EXIT_DONE);
      writeIO_IOM(IO_GPO4, glbl_gpo4);

      // Poll for self-refresh exit done from DC FSM
      do {
         srx_done = (readIO_DMC(ral_addr_of_reg_com_0_in_ddrmc_main_ddrmc_main_bank(BLK)) >> 6) & 0x1;

	 if(loop > 1000) { // Watchdog
            write_error_XRAM(ERR_CODE_107_WATCHDOG_TIMEOUT_POLL_SELF_REF_EXIT_DONE_FROM_DC_FSM);
            return 1;
	 }
	 loop++;
      } while(srx_done != 0x1);
   }

      status = en_mission_mode_tracking();
      if(status) {
         return 1;
      }
#endif
   }

   DEBUG_PRINTF("INFO: self_ref_exit(): Stopping the DDRMC CLK timer\n");
   writeIO_DMC(ral_addr_of_ddrmc_clk_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), ((0x1 << 31) | 0x0)); // Load count 0

   glbl_restore_cal_en = 0; // Self refresh power down exit has completed successfully
   DEBUG_PRINTF("INFO: self_ref_exit(): Self Refresh Exit: Done\n");

#endif // VNC_DIS_SELF_REF
   return 0;
}

u8 ddr_save_val()
{
   u8 bit, nibble, phy_nibble, phy_bit, byte, pos;
   u8 rank;
   u16 crse, cal_fine, cal_dly, vref;
   u32 addr, val, rdata, wdram;
   static u16 rldlyrnk[4], wldlyrnk[4];
   u32 mbdr_freq_base = (glbl_freq_f1 == 0) ? MBDR_BASE_F0 : MBDR_BASE_F1;

   DEBUG_PRINTF("INFO: ddr_save_val(): Save/Restore - Save values for Frequency %d: Start\n", glbl_freq_f1);

   if(glbl_phy_rank_switch_en == 1) {
      DEBUG_PRINTF("INFO: ddr_save_val(): Dummy write for setting correct odelays\n");
      dummy_write_multi_rank_switch(0);

      DEBUG_PRINTF("INFO: ddr_save_val(): Disabling dynamic mode for TX\n");
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
      riu_reg_bcast(DIS_DYN_MODE, 0, 0x1);
   }

// Calibration BISC values, that are used in restoring
   for(nibble=0; nibble<27; nibble++) {
      writeIO_MBDR(mbdr_freq_base + MBDR_RL_DLY_QTR + (nibble*4), glbl_rl_dly_qtr[nibble]);
      DEBUG_PRINTF("INFO: ddr_save_val(): nibble = %d, calibration qtr = %x\n", nibble, glbl_rl_dly_qtr[nibble]);
   }

// XPHY VREF Related
   wdram = 0;
   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
      addr = (glbl_map_riu[nibble] << RIU_NIB_POS) | VREF_CTRL;
      vref = readIO_RIU(addr);

      wdram |= (vref << ((nibble % 3) * 10)); // [29:20], [19:10], [9:0]
      if((nibble % 3) == 2) {
         writeIO_MBDR(mbdr_freq_base + MBDR_MEM_READ_VREF + ((nibble / 3) * 4), wdram);
         wdram = 0;
      }
   }
   if((nibble % 3) != 0) { // Check if it is the correct location
      writeIO_MBDR(mbdr_freq_base + MBDR_MEM_READ_VREF + ((nibble / 3) * 4), wdram);
   }

// MEM VREF Related
   for(nibble=0; nibble<DQS_BITS; nibble++) {
      vref = glbl_wrvref_value[nibble]; // FIXME: Same for all ranks for now
      wdram = (vref << 24) | (vref << 16) | (vref << 8) | vref;
      writeIO_MBDR(mbdr_freq_base + MBDR_MEM_WRITE_VREF + (nibble * 4), wdram);
   }

#ifdef LPDDR4
// LPDDR4 write DQS oscillator value
   writeIO_MBDR(mbdr_freq_base + MBDR_MEM_LP4_TDQSOSC + 0,  ((glbl_cal_tdqsosc[1][0] << 16) | glbl_cal_tdqsosc[0][0]));
   writeIO_MBDR(mbdr_freq_base + MBDR_MEM_LP4_TDQSOSC + 4,  ((glbl_cal_tdqsosc[1][1] << 16) | glbl_cal_tdqsosc[0][1]));
   writeIO_MBDR(mbdr_freq_base + MBDR_MEM_LP4_TDQSOSC + 8,  ((glbl_cal_tdqsosc[1][2] << 16) | glbl_cal_tdqsosc[0][2]));
   writeIO_MBDR(mbdr_freq_base + MBDR_MEM_LP4_TDQSOSC + 12, ((glbl_cal_tdqsosc[1][3] << 16) | glbl_cal_tdqsosc[0][3]));

   writeIO_MBDR(mbdr_freq_base + MBDR_MEM_LP4_TDQS2DQ + 0,  ((glbl_cal_tdqs2dq[1][0] << 16) | glbl_cal_tdqs2dq[0][0]));
   writeIO_MBDR(mbdr_freq_base + MBDR_MEM_LP4_TDQS2DQ + 4,  ((glbl_cal_tdqs2dq[1][1] << 16) | glbl_cal_tdqs2dq[0][1]));
   writeIO_MBDR(mbdr_freq_base + MBDR_MEM_LP4_TDQS2DQ + 8,  ((glbl_cal_tdqs2dq[1][2] << 16) | glbl_cal_tdqs2dq[0][2]));
   writeIO_MBDR(mbdr_freq_base + MBDR_MEM_LP4_TDQS2DQ + 12, ((glbl_cal_tdqs2dq[1][3] << 16) | glbl_cal_tdqs2dq[0][3]));
#endif

// LRDIMM Data Buffer F0/F1 Related
#ifdef DDR4
   if(glbl_lrdimm_en) {
      for(rank=0; rank<RANKS; rank++) {
         for(nibble=0; nibble<DQS_BITS; nibble++) {

            wdram = (glbl_mrd_lat[rank][nibble] << 6) | glbl_mrep_tap[rank][nibble]; // MRD Latency, MREP Phase
            writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_DB_MREP_MRD_LAT + (rank * MAX_DQS_BITS * 4) + (nibble * 4), wdram);

            wdram = glbl_mrd_tap[rank][nibble]; // MRD center tap
            writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_DB_MRD_CENTER   + (rank * MAX_DQS_BITS * 4) + (nibble * 4), wdram);

            wdram = (glbl_mwd_lat[rank][nibble] << 6) | glbl_dwl_tap[rank][nibble]; // MWD Latency, DWL Phase
            writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_DB_DWL_MWD_LAT  + (rank * MAX_DQS_BITS * 4) + (nibble * 4), wdram);

            wdram = glbl_mwd_tap[rank][nibble]; // MWD center tap
            writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_DB_MWD_CENTER   + (rank * MAX_DQS_BITS * 4) + (nibble * 4), wdram);
         }
      }
   }
#endif

// XPHY F0/F1 Related
   // RL_DLY_RNK Coarse & Fine
   for(nibble=0; nibble<DQS_BITS; nibble++) {
      for(rank=0; rank < 4; rank++) {
         rldlyrnk[rank] = read_riu_nibble_reg(glbl_dqs_bits_riu_map[nibble][0], RLDLYRNK0 + rank);
      }

      wdram = (rldlyrnk[1] << 16) | rldlyrnk[0]; // Rank-1, Rank-0
      writeIO_MBDR(mbdr_freq_base + MBDR_RL_DLY_RNK_CRSE_FINE + ((nibble * 2) * 4), wdram);

      wdram = (rldlyrnk[3] << 16) | rldlyrnk[2]; // Rank-3, Rank-2
      writeIO_MBDR(mbdr_freq_base + MBDR_RL_DLY_RNK_CRSE_FINE + ((nibble * 2) * 4) + 4, wdram);
   }

   // WL_DLY_RNK Coarse & Fine
   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
      for(rank=0; rank < 4; rank++) {
         wldlyrnk[rank] = read_riu_nibble_reg(glbl_map_riu[nibble], WLDLYRNK0 + rank);
      }

      #ifdef LPDDR4
         for(rank=0; rank < RANKS; rank++) {
            glbl_cal_wl_dly_rnk[rank][nibble] = wldlyrnk[rank];
         }
      #endif

      wdram = (wldlyrnk[1] << 16) | wldlyrnk[0]; // Rank-1, Rank-0
      writeIO_MBDR(mbdr_freq_base + MBDR_WL_DLY_RNK_CRSE_FINE + ((nibble * 2) * 4), wdram);

      wdram = (wldlyrnk[3] << 16) | wldlyrnk[2]; // Rank-3, Rank-2
      writeIO_MBDR(mbdr_freq_base + MBDR_WL_DLY_RNK_CRSE_FINE + ((nibble * 2) * 4) + 4, wdram);
   }

   // DQ IDELAY
   wdram = 0;
   for(bit=0; bit<DQ_BITS; bit++) {
      addr = (glbl_dq_bits_riu_map[bit][0] << RIU_NIB_POS) | (glbl_dq_bits_riu_map[bit][1] + IDLY0);
      cal_dly = readIO_RIU(addr) & 0x1FF;
      wdram |= (cal_dly << ((bit % 3) * 9)); // [26:18], [17:9], [8:0]
      if((bit % 3) == 2) {
         writeIO_MBDR(mbdr_freq_base + MBDR_DQ_IDELAY + ((bit / 3) * 4), wdram);
         wdram = 0;
      }
   }
   if((bit % 3) != 0) { // Check if it is the correct location
      writeIO_MBDR(mbdr_freq_base + MBDR_DQ_IDELAY + ((bit / 3) * 4), wdram);
   }

   // DQ ODELAY
   wdram = 0;
   for(bit=0; bit<DQ_BITS; bit++) {
      addr = (glbl_dq_bits_riu_map[bit][0] << RIU_NIB_POS) | (glbl_dq_bits_riu_map[bit][1] + ODLY0);
      cal_dly = readIO_RIU(addr) & 0x1FF;
      wdram |= (cal_dly << ((bit % 3) * 9)); // [26:18], [17:9], [8:0]
      if((bit % 3) == 2) {
         writeIO_MBDR(mbdr_freq_base + MBDR_DQ_ODELAY + ((bit / 3) * 4), wdram);
         wdram = 0;
      }
   }
   if((bit % 3) != 0) { // Check if it is the correct location
      writeIO_MBDR(mbdr_freq_base + MBDR_DQ_ODELAY + ((bit / 3) * 4), wdram);
   }

   // PQTR
   wdram = 0;
   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
      addr = (glbl_map_riu[nibble] << RIU_NIB_POS) | PQTR;
      cal_dly = readIO_RIU(addr) & 0x1FF;
      wdram |= (cal_dly << ((nibble % 3) * 9)); // [26:18], [17:9], [8:0]
      if((nibble % 3) == 2) {
         writeIO_MBDR(mbdr_freq_base + MBDR_DQS_PQTR + ((nibble / 3) * 4), wdram);
         wdram = 0;
      }
   }
   if((nibble % 3) != 0) { // Check if it is the correct location
      writeIO_MBDR(mbdr_freq_base + MBDR_DQS_PQTR + ((nibble / 3) * 4), wdram);
   }

   // NQTR
   wdram = 0;
   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
      addr = (glbl_map_riu[nibble] << RIU_NIB_POS) | NQTR;
      cal_dly = readIO_RIU(addr) & 0x1FF;
      wdram |= (cal_dly << ((nibble % 3) * 9)); // [26:18], [17:9], [8:0]
      if((nibble % 3) == 2) {
         writeIO_MBDR(mbdr_freq_base + MBDR_DQS_NQTR + ((nibble / 3) * 4), wdram);
         wdram = 0;
      }
   }
   if((nibble % 3) != 0) { // Check if it is the correct location
      writeIO_MBDR(mbdr_freq_base + MBDR_DQS_NQTR + ((nibble / 3) * 4), wdram);
   }

   // DQS ODELAY
   wdram = 0;
   for(nibble=0; nibble<DQS_BITS; nibble++) {
      addr = (glbl_dqs_bits_riu_map[nibble][0] << RIU_NIB_POS) | (glbl_dqs_bits_riu_map[nibble][1] + ODLY0);
      cal_dly = readIO_RIU(addr) & 0x1FF;
      wdram |= (cal_dly << ((nibble % 3) * 9)); // [26:18], [17:9], [8:0]
      if((nibble % 3) == 2) {
         writeIO_MBDR(mbdr_freq_base + MBDR_DQS_ODELAY + ((nibble / 3) * 4), wdram);
         wdram = 0;
      }
   }
   if((nibble % 3) != 0) { // Check if it is the correct location
      writeIO_MBDR(mbdr_freq_base + MBDR_DQS_ODELAY + ((nibble / 3) * 4), wdram);
   }

   // DBI IDELAY & ODELAY
   if(NIBBLE_PER_DQS == 2) {
      wdram = 0;
      for(byte=0; byte<DQS_BITS; byte++) {
         addr = (glbl_dbi_bits_riu_map[byte][0] << RIU_NIB_POS) | (glbl_dbi_bits_riu_map[byte][1] + IDLY0);
         cal_dly = readIO_RIU(addr) & 0x1FF;
         wdram |= (cal_dly << ((byte % 3) * 9)); // [26:18], [17:9], [8:0]
         if((byte % 3) == 2) {
            writeIO_MBDR(mbdr_freq_base + MBDR_DBI_IDELAY + ((byte / 3) * 4), wdram);
            wdram = 0;
         }
      }
      if((byte % 3) != 0) { // Check if it is the correct location
         writeIO_MBDR(mbdr_freq_base + MBDR_DBI_IDELAY + ((byte / 3) * 4), wdram);
      }

      wdram = 0;
      for(byte=0; byte<DQS_BITS; byte++) {
         addr = (glbl_dbi_bits_riu_map[byte][0] << RIU_NIB_POS) | (glbl_dbi_bits_riu_map[byte][1] + ODLY0);
         cal_dly = readIO_RIU(addr) & 0x1FF;
         wdram |= (cal_dly << ((byte % 3) * 9)); // [26:18], [17:9], [8:0]
         if((byte % 3) == 2) {
            writeIO_MBDR(mbdr_freq_base + MBDR_DBI_ODELAY + ((byte / 3) * 4), wdram);
            wdram = 0;
         }
      }
      if((byte % 3) != 0) { // Check if it is the correct location
         writeIO_MBDR(mbdr_freq_base + MBDR_DBI_ODELAY + ((byte / 3) * 4), wdram);
      }
   }

   #ifdef LPDDR4
      // LP4_WL_DQS
      wdram = 0;
      for(byte=0; byte<DQS_BITS; byte++) {
         addr = (glbl_dqs_lnibble2pnibble_map[byte] << RIU_NIB_POS) | LP4_WL_DQS;
         val = readIO_RIU(addr) & 0xF;
         wdram |= (val << ((byte % 8) * 4)); // [11:8], [7:4], [3:0]
      }
      writeIO_MBDR(mbdr_freq_base + MBDR_LP4_WL_DQS_CRSE, wdram);

      // LP4 CA ODELAY
      wdram = 0;
      for(bit=0; bit<CA_BITS; bit++) {
         addr = (glbl_ca_bits_riu_map[bit][0] << RIU_NIB_POS) | (glbl_ca_bits_riu_map[bit][1] + ODLY0);
         cal_dly = readIO_RIU(addr) & 0x1FF;
         wdram |= (cal_dly << ((bit % 3) * 9)); // [26:18], [17:9], [8:0]
         if((bit % 3) == 2) {
            writeIO_MBDR(mbdr_freq_base + MBDR_LP4_CA_ODELAY + ((bit / 3) * 4), wdram);
            wdram = 0;
         }
      }
      if((bit % 3) != 0) { // Check if it is the correct location
         writeIO_MBDR(mbdr_freq_base + MBDR_LP4_CA_ODELAY + ((bit / 3) * 4), wdram);
      }

      // LP4 CS ODELAY
      wdram = 0;
      for(bit=0; bit<LP4_MAX_CS_BITS*LP4_MAX_RANKS; bit++) {
         addr = (glbl_cs_bits_riu_map[bit][0] << RIU_NIB_POS) | (glbl_cs_bits_riu_map[bit][1] + ODLY0);
         cal_dly = readIO_RIU(addr) & 0x1FF;
         wdram |= (cal_dly << ((bit % 3) * 9)); // [26:18], [17:9], [8:0]
         if((bit % 3) == 2) {
            writeIO_MBDR(mbdr_freq_base + MBDR_LP4_CS_ODELAY + ((bit / 3) * 4), wdram);
            wdram = 0;
         }
      }
      if((bit % 3) != 0) { // Check if it is the correct location
         writeIO_MBDR(mbdr_freq_base + MBDR_LP4_CS_ODELAY + ((bit / 3) * 4), wdram);
      }

      // LP4 CA tx_out_phase_90
      val = 0x0;
      for(bit=0; bit<CA_BITS; bit++) {
         phy_nibble = glbl_ca_bits_riu_map[bit][0];
         phy_bit = glbl_ca_bits_riu_map[bit][1];
         addr = (phy_nibble << RIU_NIB_POS) | CFG_NIBBLE_CTRL1;
         pos = phy_bit + 2;
         rdata = readIO_RIU(addr);
         val = val | ((rdata >> pos) & 0x1) << bit;
      }
      writeIO_MBDR(mbdr_freq_base + MBDR_LP4_CA_TX_OUT_PHASE_90, val);

      // LP4 CS tx_out_phase_90
      val = 0x0;
      for(bit=0; bit<LP4_MAX_CS_BITS*LP4_MAX_RANKS; bit++) {
         phy_nibble = glbl_cs_bits_riu_map[bit][0];
         phy_bit = glbl_cs_bits_riu_map[bit][1];
         addr = (phy_nibble << RIU_NIB_POS) | CFG_NIBBLE_CTRL1;
         pos = phy_bit + 2;
         rdata = readIO_RIU(addr);
         val = val | ((rdata >> pos) & 0x1) << bit;
      }
      writeIO_MBDR(mbdr_freq_base + MBDR_LP4_CS_TX_OUT_PHASE_90, val);
   #endif

// CAL F0/F1 Related
   // PHY RDEN
   wdram = 0;
   for(nibble=0; nibble<DQS_BITS; nibble++) {
      phy_nibble = glbl_dqs_bits_riu_map[nibble][0];
      addr = ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK) + (phy_nibble * 4);
      val = readIO_DMC(addr) & 0x7F; // [6:0]  
      wdram |=(val << ((nibble % 4) * 8)); // [30:24], [22:16], [14:8], [6:0]
      if((nibble % 4) == 3) {
         writeIO_MBDR(mbdr_freq_base + MBDR_PHY_RDEN + ((nibble / 4) * 4), wdram);
         wdram = 0;
      }
   }
   if((nibble % 4) != 0) { // Check if it is the correct location
      writeIO_MBDR(mbdr_freq_base + MBDR_PHY_RDEN + ((nibble / 4) * 4), wdram);
   }

   // FIFO RDEN
   val = readIO_DMC(ral_addr_of_fifo_rden_in_ddrmc_main_ddrmc_main_bank(BLK));
   writeIO_MBDR(mbdr_freq_base + MBDR_FIFO_RDEN, val);

   // ADD_CMD_DELAY_EN, ADD_CMD_DELAY, XPI_WRDATA_ALL_NIB, XPI_OE_ALL_NIB,
   rdata = readIO_DMC(ral_addr_of_xpi_oe_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK));
   val = rdata & 0x7F; // [6:0]
   wdram = val; // [6:0]

   rdata = readIO_DMC(ral_addr_of_xpi_wrdata_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK));
   val = rdata & 0x7F; // [6:0]
   wdram |= (val << 8); // [14:8]

   rdata = readIO_DMC(ral_addr_of_reg_safe_config5_in_ddrmc_main_ddrmc_main_bank(BLK));
   val = (rdata >> 28) & 0xF; // [31] - ADD_CMD_DELAY_EN, [30:28] - ADD_CMD_DELAY
   wdram |= (val << 16); // [19], [18:16]
   writeIO_MBDR(mbdr_freq_base + MBDR_XPI_OE_WRDATA_ALL_NIB_DLY, wdram);

   // PHY_OE_NIB
   wdram = 0;
   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
      phy_nibble = glbl_map_riu[nibble];
      addr = ral_addr_of_phy_oe_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (phy_nibble * 4);
      val = readIO_DMC(addr) & 0x7; // [2:0]  
      wdram |=(val << ((nibble % 8) * 4)); // ...[10:8], [6:4], [2:0]
      if((nibble % 8 ) == 7) {
         writeIO_MBDR(mbdr_freq_base + MBDR_PHY_OE_NIB + ((nibble / 8) * 4), wdram);
         wdram = 0;
      }
   }
   if((nibble % 8) != 0) { // Check if it is the correct location
      writeIO_MBDR(mbdr_freq_base + MBDR_PHY_OE_NIB + ((nibble / 8) * 4), wdram);
   }

   // PHY_DATA_NIB
   wdram = 0;
   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
      addr = ral_addr_of_phy_data_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (nibble * 4);
      val = readIO_DMC(addr) & 0x7; // [2:0]
      wdram |=(val << ((nibble % 8) * 4)); // ...[10:8], [6:4], [2:0]
      if((nibble % 8 ) == 7) {
         writeIO_MBDR(mbdr_freq_base + MBDR_PHY_DATA_NIB + ((nibble / 8) * 4), wdram);
         wdram = 0;
      }
   }
   if((nibble % 8) != 0) { // Check if it is the correct location
      writeIO_MBDR(mbdr_freq_base + MBDR_PHY_DATA_NIB + ((nibble / 8) * 4), wdram);
   }

   // XPI_MAP_BITSLIP
   val = readIO_DMC(ral_addr_of_xpi_map_bitslip_in_ddrmc_main_ddrmc_main_bank(BLK));
   writeIO_MBDR(mbdr_freq_base + MBDR_XPI_MAP_BITSLIP, val);

   if(glbl_phy_rank_switch_en == 1) {
      DEBUG_PRINTF("INFO: ddr_save_val(): Enabling dynamic mode for TX\n");
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
      riu_reg_bcast(DIS_DYN_MODE, 0, 0x0);

      DEBUG_PRINTF("INFO: ddr_save_val(): Dummy write for setting correct odelays\n");
      dummy_write_multi_rank_switch(0);
   }

   DEBUG_PRINTF("INFO: ddr_save_val(): Save/Restore - Save values: Done\n");
   return 0;
}

u8 ddr_restore_val()
{
   u8 bit, nibble, byte, pos, phy_bit, phy_nibble;
   u8 rank;
   u16 crse, fine, dly, vref, cal_fine, cal_dly;
   u32 addr, rdata, wdata, rdram, val;
   u16 cal_rl_dly_qtr[27];
   u16 rldlyrnk[4], wldlyrnk[4];
   u32 mbdr_freq_base = (glbl_freq_f1 == 0) ? MBDR_BASE_F0 : MBDR_BASE_F1;

   DEBUG_PRINTF("INFO: ddr_restore_val(): Save/Restore - Restore values for Frequency %d: Start\n", glbl_freq_f1);

// Read Calibration BISC values, that are used in restoring
   for(nibble=0; nibble<27; nibble++) {
      cal_rl_dly_qtr[nibble] = readIO_MBDR(mbdr_freq_base + MBDR_RL_DLY_QTR + (nibble*4));
      DEBUG_PRINTF("INFO: ddr_restore_val(): nibble = %d, calibration qtr = %x, current qtr = %x\n", nibble, cal_rl_dly_qtr[nibble], glbl_rl_dly_qtr[nibble]);
   }
   
// XPHY VREF Related
   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
      phy_nibble = glbl_map_riu[nibble];
      addr = (phy_nibble << RIU_NIB_POS) | VREF_CTRL;

      if((nibble % 3) == 0) { 
         rdram = readIO_MBDR(mbdr_freq_base + MBDR_MEM_READ_VREF + (nibble / 3) * 4);
      }

      vref = (rdram >> ((nibble % 3) * 10)) & 0x3FF;

      DEBUG_PRINTF("INFO: ddr_restore_val(): nibble = %d, phy_nibble = %x, addr = %x, vref = %x\n", nibble, phy_nibble, addr, vref);
      writeIO_RIU(addr, (vref | 0x400));  
   }

// XPHY F0/F1 Related
   // RL_DLY_RNK Coarse & Fine
   for(nibble=0; nibble<DQS_BITS; nibble++) {
      phy_nibble = glbl_dqs_bits_riu_map[nibble][0];
      rdram = readIO_MBDR(mbdr_freq_base + MBDR_RL_DLY_RNK_CRSE_FINE + ((nibble * 2) * 4));
      rldlyrnk[0] = rdram & 0x1FFF;
      rldlyrnk[1] = (rdram >> 16) & 0x1FFF;

      rdram = readIO_MBDR(mbdr_freq_base + MBDR_RL_DLY_RNK_CRSE_FINE + ((nibble * 2) * 4) + 4);
      rldlyrnk[2] = rdram & 0x1FFF;
      rldlyrnk[3] = (rdram >> 16) & 0x1FFF;

      for(rank=0; rank<4; rank += 1) {
         crse = (rldlyrnk[rank] >> 9) & 0xF;
         cal_fine = rldlyrnk[rank] & 0x1FF;

         fine = (cal_fine * glbl_rl_dly_qtr[phy_nibble]) / cal_rl_dly_qtr[phy_nibble];
         DEBUG_PRINTF("INFO: ddr_restore_val(): RL_DLY_RNK: rank = %d, nibble = %d, phy_nibble = %d, crse = %d, calibration qtr = %x, current qtr = %x, cal fine = %x, current fine = %x\n", rank, nibble, phy_nibble, crse, cal_rl_dly_qtr[phy_nibble], glbl_rl_dly_qtr[phy_nibble], cal_fine, fine);
         if(rank == 3) // Set update for the last rank which results in updating all the ranks
            write_riu_nibble_reg(phy_nibble, RLDLYRNK0 + rank, (0x8000 | (crse << 9) | fine));
         else
            write_riu_nibble_reg(phy_nibble, RLDLYRNK0 + rank, ((crse << 9) | fine));
      }
   }

   // WL_DLY_RNK Coarse & Fine
   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
      phy_nibble = glbl_map_riu[nibble];
      rdram = readIO_MBDR(mbdr_freq_base + MBDR_WL_DLY_RNK_CRSE_FINE + ((nibble * 2) * 4));
      wldlyrnk[0] = rdram & 0x1FFF;
      wldlyrnk[1] = (rdram >> 16) & 0x1FFF;

      rdram = readIO_MBDR(mbdr_freq_base + MBDR_WL_DLY_RNK_CRSE_FINE + ((nibble * 2) * 4) + 4);
      wldlyrnk[2] = rdram & 0x1FFF;
      wldlyrnk[3] = (rdram >> 16) & 0x1FFF;

      #ifdef LPDDR4
         for(rank=0; rank < RANKS; rank++)
            glbl_cal_wl_dly_rnk[rank][nibble] = wldlyrnk[rank];
      #endif

      for(rank=0; rank<RANKS; rank += glbl_rank_inc) {
         crse = (wldlyrnk[rank] >> 9) & 0xF;
         cal_fine = wldlyrnk[rank] & 0x1FF;

         fine = (cal_fine * glbl_rl_dly_qtr[phy_nibble]) / cal_rl_dly_qtr[phy_nibble];
         DEBUG_PRINTF("INFO: ddr_restore_val(): WL_DLY_RNK: rank = %d, nibble = %d, phy_nibble = %d, crse = %d, calibration qtr = %x, current qtr = %x, cal fine = %x, current fine = %x\n", rank, nibble, phy_nibble, crse, cal_rl_dly_qtr[phy_nibble], glbl_rl_dly_qtr[phy_nibble], cal_fine, fine);
         write_riu_nibble_reg(phy_nibble, WLDLYRNK0 + rank, ((crse << 9) | fine));
      #ifdef LPDDR4
         write_riu_nibble_reg(phy_nibble, WLDLYRNK0 + rank + 2, ((crse << 9) | fine)); // Load the mirror rank also for LP4
      #endif
      }
   }

   // DQ IDELAY
   for(bit=0; bit<DQ_BITS; bit++) {
      phy_nibble = glbl_dq_bits_riu_map[bit][0];
      phy_bit = glbl_dq_bits_riu_map[bit][1];
      addr = (phy_nibble << RIU_NIB_POS) | (phy_bit + IDLY0);

      if((bit % 3) == 0) { 
         rdram = readIO_MBDR(mbdr_freq_base + MBDR_DQ_IDELAY + (bit / 3) * 4);
      }
      cal_dly = (rdram >> ((bit % 3) * 9)) & 0x1FF;

      dly = (cal_dly * glbl_rl_dly_qtr[phy_nibble]) / cal_rl_dly_qtr[phy_nibble];
      DEBUG_PRINTF("INFO: ddr_restore_val(): DQ IDELAY: bit = %d, phy_nibble = %d, phy_bit = %d, calibration qtr = %x, current qtr = %x, cal dly = %x, current dly = %x\n", bit, phy_nibble, phy_bit, cal_rl_dly_qtr[phy_nibble], glbl_rl_dly_qtr[phy_nibble], cal_dly, dly);
      writeIO_RIU(addr, dly);  
   }

   // DQ ODELAY
   for(bit=0; bit<DQ_BITS; bit++) {
      phy_nibble = glbl_dq_bits_riu_map[bit][0];
      phy_bit = glbl_dq_bits_riu_map[bit][1];
      addr = (phy_nibble << RIU_NIB_POS) | (phy_bit + ODLY0);

      if((bit % 3) == 0) { 
         rdram = readIO_MBDR(mbdr_freq_base + MBDR_DQ_ODELAY + (bit / 3) * 4);
      }
      cal_dly = (rdram >> ((bit % 3) * 9)) & 0x1FF;

      dly = (cal_dly * glbl_rl_dly_qtr[phy_nibble]) / cal_rl_dly_qtr[phy_nibble];
      DEBUG_PRINTF("INFO: ddr_restore_val(): DQ ODELAY: bit = %d, phy_nibble = %d, phy_bit = %d, calibration qtr = %x, current qtr = %x, cal dly = %x, current dly = %x\n", bit, phy_nibble, phy_bit, cal_rl_dly_qtr[phy_nibble], glbl_rl_dly_qtr[phy_nibble], cal_dly, dly);
      writeIO_RIU(addr, dly);  
   }

   // DQS PQTR
   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
      phy_nibble = glbl_map_riu[nibble];
      addr = (phy_nibble << RIU_NIB_POS) | PQTR;

      if((nibble % 3) == 0) { 
         rdram = readIO_MBDR(mbdr_freq_base + MBDR_DQS_PQTR + (nibble / 3) * 4);
      }
      cal_dly = (rdram >> ((nibble % 3) * 9)) & 0x1FF;

      dly = (cal_dly * glbl_rl_dly_qtr[phy_nibble]) / cal_rl_dly_qtr[phy_nibble];
      DEBUG_PRINTF("INFO: ddr_restore_val(): DQS PQTR: nibble = %d, phy_nibble = %d, calibration qtr = %x, current qtr = %x, cal dly = %x, current dly = %x\n", nibble, phy_nibble, cal_rl_dly_qtr[phy_nibble], glbl_rl_dly_qtr[phy_nibble], cal_dly, dly);
      writeIO_RIU(addr, dly);  

   }

   // DQS NQTR
   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
      phy_nibble = glbl_map_riu[nibble];
      addr = (phy_nibble << RIU_NIB_POS) | NQTR;

      if((nibble % 3) == 0) { 
         rdram = readIO_MBDR(mbdr_freq_base + MBDR_DQS_NQTR + (nibble / 3) * 4);
      }
      cal_dly = (rdram >> ((nibble % 3) * 9)) & 0x1FF;

      dly = (cal_dly * glbl_rl_dly_qtr[phy_nibble]) / cal_rl_dly_qtr[phy_nibble];
      DEBUG_PRINTF("INFO: ddr_restore_val(): DQS NQTR: nibble = %d, phy_nibble = %d, calibration qtr = %x, current qtr = %x, cal dly = %x, current dly = %x\n", nibble, phy_nibble, cal_rl_dly_qtr[phy_nibble], glbl_rl_dly_qtr[phy_nibble], cal_dly, dly);
      writeIO_RIU(addr, dly);  
   }

   // DQS ODELAY & TRISTATE ODELAY
   for(nibble=0; nibble<DQS_BITS; nibble++) {
      phy_nibble = glbl_dqs_bits_riu_map[nibble][0];
      phy_bit = glbl_dqs_bits_riu_map[nibble][1];
      addr = (phy_nibble << RIU_NIB_POS) | (phy_bit + ODLY0);

      if((nibble % 3) == 0) { 
         rdram = readIO_MBDR(mbdr_freq_base + MBDR_DQS_ODELAY + (nibble / 3) * 4);
      }
      cal_dly = (rdram >> ((nibble % 3) * 9)) & 0x1FF;

      dly = (cal_dly * glbl_rl_dly_qtr[phy_nibble]) / cal_rl_dly_qtr[phy_nibble];
      DEBUG_PRINTF("INFO: ddr_restore_val(): DQS ODELAY: nibble = %d, phy_nibble = %d, phy_bit = %d, calibration qtr = %x, current qtr = %x, cal dly = %x, current dly = %x\n", nibble, phy_nibble, phy_bit, cal_rl_dly_qtr[phy_nibble], glbl_rl_dly_qtr[phy_nibble], cal_dly, dly);
      writeIO_RIU(addr, dly);  

      // TRISTATE ODELAY (nibble-wise: pick it from corresponding DQS ODELAY)
      if(NIBBLE_PER_DQS == 2) { // Write to both the nibbles
         write_riu_nibble_reg(glbl_map_riu[2*nibble], TRISTATE_ODLY, dly);
         write_riu_nibble_reg(glbl_map_riu[2*nibble+1], TRISTATE_ODLY, dly);
      } else {
         write_riu_nibble_reg(phy_nibble, TRISTATE_ODLY, dly);
      }
   }

   // DBI IDELAY & ODELAY
   if(NIBBLE_PER_DQS == 2) {
      for(byte=0; byte<DQS_BITS; byte++) {
         phy_nibble = glbl_dbi_bits_riu_map[byte][0];
         addr = (phy_nibble << RIU_NIB_POS) | IDLY0;

         if((byte %3) == 0) { 
            rdram = readIO_MBDR(mbdr_freq_base + MBDR_DBI_IDELAY + (byte / 3) * 4);
         }
         cal_dly = (rdram >> ((byte % 3) * 9)) & 0x1FF;

         dly = (cal_dly * glbl_rl_dly_qtr[phy_nibble]) / cal_rl_dly_qtr[phy_nibble];
         DEBUG_PRINTF("INFO: ddr_restore_val(): DBI IDELAY: byte = %d, phy_nibble = %d, calibration qtr = %x, current qtr = %x, cal dly = %x, current dly = %x\n", byte, phy_nibble, cal_rl_dly_qtr[phy_nibble], glbl_rl_dly_qtr[phy_nibble], cal_dly, dly);
         writeIO_RIU(addr, dly);  
      }

      for(byte=0; byte<DQS_BITS; byte++) {
         phy_nibble = glbl_dbi_bits_riu_map[byte][0];
         addr = (phy_nibble << RIU_NIB_POS) | ODLY0;

         if((byte %3) == 0) { 
            rdram = readIO_MBDR(mbdr_freq_base + MBDR_DBI_ODELAY + (byte / 3) * 4);
         }
         cal_dly = (rdram >> ((byte % 3) * 9)) & 0x1FF;

         dly = (cal_dly * glbl_rl_dly_qtr[phy_nibble]) / cal_rl_dly_qtr[phy_nibble];
         DEBUG_PRINTF("INFO: ddr_restore_val(): DBI ODELAY: byte = %d, phy_nibble = %d, calibration qtr = %x, current qtr = %x, cal dly = %x, current dly = %x\n", byte, phy_nibble, cal_rl_dly_qtr[phy_nibble], glbl_rl_dly_qtr[phy_nibble], cal_dly, dly);
         writeIO_RIU(addr, dly);  
      }
   }


   #ifdef LPDDR4
      // LP4_WL_DQS
      for(byte=0; byte<DQS_BITS; byte++) {
         phy_nibble = glbl_dqs_lnibble2pnibble_map[byte];
         addr = (phy_nibble << RIU_NIB_POS) | LP4_WL_DQS;

         if((byte % 8) == 0) { 
            rdram = readIO_MBDR(mbdr_freq_base + MBDR_LP4_WL_DQS_CRSE + (byte / 8) * 4);
         }
         crse = (rdram >> ((byte % 8) * 4)) & 0xF;

         DEBUG_PRINTF("INFO: ddr_restore_val(): LP4 WL_DQS: byte = %d, phy_nibble = %d, crse = %d\n", byte, phy_nibble, crse);
         writeIO_RIU(addr, crse);  
      }

      // LP4 CA ODELAY
      for(bit=0; bit<CA_BITS; bit++) {
         phy_nibble = glbl_ca_bits_riu_map[bit][0];
         phy_bit = glbl_ca_bits_riu_map[bit][1];
         addr = (phy_nibble << RIU_NIB_POS) | (phy_bit + ODLY0);

         if((bit %3) == 0) { 
            rdram = readIO_MBDR(mbdr_freq_base + MBDR_LP4_CA_ODELAY + (bit / 3) * 4);
         }
         cal_dly = (rdram >> ((bit % 3) * 9)) & 0x1FF;

         dly = (cal_dly * glbl_rl_dly_qtr[phy_nibble]) / cal_rl_dly_qtr[phy_nibble];
         DEBUG_PRINTF("INFO: ddr_restore_val(): LP4 CA ODELAY: bit = %d, phy_nibble = %d, phy_bit = %d, calibration qtr = %x, current qtr = %x, cal dly = %x, current dly = %x\n", bit, phy_nibble, phy_bit, cal_rl_dly_qtr[phy_nibble], glbl_rl_dly_qtr[phy_nibble], cal_dly, dly);
         writeIO_RIU(addr, dly);  
      }

      // LP4 CS ODELAY
      for(bit=0; bit<LP4_MAX_CS_BITS*LP4_MAX_RANKS; bit++) {
         phy_nibble = glbl_cs_bits_riu_map[bit][0];
         phy_bit = glbl_cs_bits_riu_map[bit][1];
         addr = (phy_nibble << RIU_NIB_POS) | (phy_bit + ODLY0);

         if((bit %3) == 0) { 
            rdram = readIO_MBDR(mbdr_freq_base + MBDR_LP4_CS_ODELAY + (bit / 3) * 4);
         }
         cal_dly = (rdram >> ((bit % 3) * 9)) & 0x1FF;

         dly = (cal_dly * glbl_rl_dly_qtr[phy_nibble]) / cal_rl_dly_qtr[phy_nibble];
         DEBUG_PRINTF("INFO: ddr_restore_val(): LP4 CS ODELAY: bit = %d, phy_nibble = %d, phy_bit = %d, calibration qtr = %x, current qtr = %x, cal dly = %x, current dly = %x\n", bit, phy_nibble, phy_bit, cal_rl_dly_qtr[phy_nibble], glbl_rl_dly_qtr[phy_nibble], cal_dly, dly);
         writeIO_RIU(addr, dly);  
      }

      // LP4 CA tx_out_phase_90
      val = readIO_MBDR(mbdr_freq_base + MBDR_LP4_CA_TX_OUT_PHASE_90);
      DEBUG_PRINTF("INFO: ddr_restore_val(): LP4 CA TX_OUT_PHASE_90: CA_BITS = %d, TX_OUT_PHASE_90 = %x\n", CA_BITS, val);

      for(bit=0; bit<CA_BITS; bit++) {
         phy_nibble = glbl_ca_bits_riu_map[bit][0];
         phy_bit = glbl_ca_bits_riu_map[bit][1];
         addr = (phy_nibble << RIU_NIB_POS) | CFG_NIBBLE_CTRL1;
         pos = phy_bit + 2;
         rdata = readIO_RIU(addr);
         wdata = rdata & ~(0x1 << pos);
         wdata = wdata | (((val >> bit) & 0x1) << pos);
         DEBUG_PRINTF("INFO: ddr_restore_val(): LP4 CA TX_OUT_PHASE_90: bit = %d, phy_nibble = %d, phy_bit = %d, addr = %x, rdata = %x, wdata = %x\n", bit, phy_nibble, phy_bit, addr, rdata, wdata);
         writeIO_RIU(addr, wdata);
      }

      // LP4 CS tx_out_phase_90
      val = readIO_MBDR(mbdr_freq_base + MBDR_LP4_CS_TX_OUT_PHASE_90);
      DEBUG_PRINTF("INFO: ddr_restore_val(): LP4 CS TX_OUT_PHASE_90: LP4_MAX_CS_BITS*LP4_MAX_RANKS= %d, TX_OUT_PHASE_90 = %x\n", LP4_MAX_CS_BITS*LP4_MAX_RANKS, val);
      for(bit=0; bit<LP4_MAX_CS_BITS*LP4_MAX_RANKS; bit++) {
         phy_nibble = glbl_cs_bits_riu_map[bit][0];
         phy_bit = glbl_cs_bits_riu_map[bit][1];
         addr = (phy_nibble << RIU_NIB_POS) | CFG_NIBBLE_CTRL1;
         pos = phy_bit + 2;
         rdata = readIO_RIU(addr);
         wdata = rdata & ~(0x1 << pos);
         wdata = wdata | (((val >> bit) & 0x1) << pos);
         DEBUG_PRINTF("INFO: ddr_restore_val(): LP4 CS TX_OUT_PHASE_90: bit = %d, phy_nibble = %d, phy_bit = %d, addr = %x, rdata = %x, wdata = %x\n", bit, phy_nibble, phy_bit, addr, rdata, wdata);
         writeIO_RIU(addr, wdata);
      }

   // LPDDR4 write DQS oscillator value
      rdram = readIO_MBDR(mbdr_freq_base + MBDR_MEM_LP4_TDQSOSC + 0);
      glbl_cal_tdqsosc[0][0] = rdram & 0xFFFF;
      glbl_cal_tdqsosc[1][0] = (rdram >> 16) & 0xFFFF;

      rdram = readIO_MBDR(mbdr_freq_base + MBDR_MEM_LP4_TDQSOSC + 4);
      glbl_cal_tdqsosc[0][1] = rdram & 0xFFFF;
      glbl_cal_tdqsosc[1][1] = (rdram >> 16) & 0xFFFF;

      rdram = readIO_MBDR(mbdr_freq_base + MBDR_MEM_LP4_TDQSOSC + 8);
      glbl_cal_tdqsosc[0][2] = rdram & 0xFFFF;
      glbl_cal_tdqsosc[1][2] = (rdram >> 16) & 0xFFFF;

      rdram = readIO_MBDR(mbdr_freq_base + MBDR_MEM_LP4_TDQSOSC + 12);
      glbl_cal_tdqsosc[0][3] = rdram & 0xFFFF;
      glbl_cal_tdqsosc[1][3] = (rdram >> 16) & 0xFFFF;
  
      rdram = readIO_MBDR(mbdr_freq_base + MBDR_MEM_LP4_TDQS2DQ + 0);
      glbl_cal_tdqs2dq[0][0] = rdram & 0xFFFF;
      glbl_cal_tdqs2dq[1][0] = (rdram >> 16) & 0xFFFF;

      rdram = readIO_MBDR(mbdr_freq_base + MBDR_MEM_LP4_TDQS2DQ + 4);
      glbl_cal_tdqs2dq[0][1] = rdram & 0xFFFF;
      glbl_cal_tdqs2dq[1][1] = (rdram >> 16) & 0xFFFF;

      rdram = readIO_MBDR(mbdr_freq_base + MBDR_MEM_LP4_TDQS2DQ + 8);
      glbl_cal_tdqs2dq[0][2] = rdram & 0xFFFF;
      glbl_cal_tdqs2dq[1][2] = (rdram >> 16) & 0xFFFF;

      rdram = readIO_MBDR(mbdr_freq_base + MBDR_MEM_LP4_TDQS2DQ + 12);
      glbl_cal_tdqs2dq[0][3] = rdram & 0xFFFF;
      glbl_cal_tdqs2dq[1][3] = (rdram >> 16) & 0xFFFF;
   #endif

   // Apply bit-slice & tristate reset
   writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
   #ifdef LPDDR4
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~(glbl_dq_nibble_mask_initial & glbl_cs_nibble_mask_initial & glbl_ca_nibble_mask_initial));
   #else
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~(glbl_dq_nibble_mask_initial));
   #endif
   riu_reg_bcast(CFG_BS_RST_MASK,1,~0x7F); 
   bitslice_reset_bcast(1, BS_RESET_WT);

// CAL F0/F1 Related
   // Restore the default values, since coming from DDRMC reset state (INITSTATE)
   #ifdef DDR4
      if(glbl_clamshell_cfg == 1) { // Clamshell configuration
         wdata = readIO_DMC(ral_addr_of_seq_side_addr15_in_ddrmc_main_ddrmc_main_bank(BLK));
         wdata |= (0x1 << 28); // ddrmc_main_es2_clamshell_status (if_cal_seq_regs_status.seq_side_addr15_cbit[0])
         writeIO_DMC(ral_addr_of_seq_side_addr15_in_ddrmc_main_ddrmc_main_bank(BLK), wdata);
      }
   #endif

   // PHY RDEN
   for(nibble=0; nibble<DQS_BITS; nibble++) {
      phy_nibble = glbl_dqs_bits_riu_map[nibble][0];
      addr = ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK) + (phy_nibble * 4);
      if((nibble % 4) == 0) {
         rdram = readIO_MBDR(mbdr_freq_base + MBDR_PHY_RDEN + (nibble / 4) * 4);
      }
      wdata = (rdram >> ((nibble % 4) * 8)) & 0x7F;
      DEBUG_PRINTF("INFO: ddr_restore_val(): PHY RDEN: nibble = %d, phy_nibble = %d, addr = %x, wdata = %x\n", nibble, phy_nibble, addr, wdata);
      writeIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK) + (phy_nibble * 4), wdata);  
   }

   // FIFO RDEN
   wdata = readIO_MBDR(mbdr_freq_base + MBDR_FIFO_RDEN);
   DEBUG_PRINTF("INFO: ddr_restore_val(): FIFO RDEN: wdata = %x\n", wdata);
   writeIO_DMC(ral_addr_of_fifo_rden_in_ddrmc_main_ddrmc_main_bank(BLK), wdata);

   // ADD_CMD_DELAY_EN, ADD_CMD_DELAY, XPI_WRDATA_ALL_NIB, XPI_OE_ALL_NIB,
   rdram = readIO_MBDR(mbdr_freq_base + MBDR_XPI_OE_WRDATA_ALL_NIB_DLY);

   val = rdram & 0x7F; // [6:0]
   rdata = readIO_DMC(ral_addr_of_xpi_oe_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK));
   wdata = (rdata & ~(0x7F)) | val; // [6:0]
   DEBUG_PRINTF("INFO: ddr_restore_val(): XPI_OE_ALL_NIB: wdata = %x\n", wdata);
   writeIO_DMC(ral_addr_of_xpi_oe_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK), wdata);

   val = (rdram >> 8) & 0x7F; // [14:8]
   rdata = readIO_DMC(ral_addr_of_xpi_wrdata_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK));
   wdata = (rdata & ~(0x7F)) | val; // [6:0]
   DEBUG_PRINTF("INFO: ddr_restore_val(): XPI_WRDATA_ALL_NIB: wdata = %x\n", wdata);
   writeIO_DMC(ral_addr_of_xpi_wrdata_all_nib_in_ddrmc_main_ddrmc_main_bank(BLK), wdata);

   // ADD_CMD_DELAY
   val = (rdram >> 16) & 0xF; // [19], [18:16]
   rdata = readIO_DMC(ral_addr_of_reg_safe_config5_in_ddrmc_main_ddrmc_main_bank(BLK));
   wdata = (rdata & ~(0xF << 28)) | (val << 28); // [31] - ADD_CMD_DELAY_EN, [30:28] - ADD_CMD_DELAY
   DEBUG_PRINTF("INFO: ddr_restore_val(): REG_SAFE_CONFIG5 - ADD_CMD_DELAY: rdata = %x, val = %x, wdata = %x\n", rdata, val, wdata);
   writeIO_DMC(ral_addr_of_reg_safe_config5_in_ddrmc_main_ddrmc_main_bank(BLK), wdata);

   // PHY_OE_NIB
   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
      phy_nibble = glbl_map_riu[nibble];
      addr = ral_addr_of_phy_oe_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (phy_nibble * 4);
      if((nibble % 8) == 0) {
         rdram = readIO_MBDR(mbdr_freq_base + MBDR_PHY_OE_NIB + (nibble / 8) * 4);
      }

      wdata = (rdram >> ((nibble % 8) * 4)) & 0x7;
      DEBUG_PRINTF("INFO: ddr_restore_val(): PHY_OE_NIB: nibble = %d, phy_nibble = %d, addr = %x, wdata = %x\n", nibble, phy_nibble, addr, wdata);
      writeIO_DMC(ral_addr_of_phy_oe_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (phy_nibble * 4), wdata);  
   }

   // PHY_DATA_NIB
   for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++) {
      addr = ral_addr_of_phy_data_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (nibble * 4);
      if((nibble %8) == 0) {
         rdram = readIO_MBDR(mbdr_freq_base + MBDR_PHY_DATA_NIB + (nibble / 8) * 4);
      }

      wdata = (rdram >> ((nibble % 8) * 4)) & 0x7;
      DEBUG_PRINTF("INFO: ddr_restore_val(): PHY_DATA_NIB: nibble = %d, addr = %x, wdata = %x\n", nibble, addr, wdata);
      writeIO_DMC(ral_addr_of_phy_data_nib0_in_ddrmc_main_ddrmc_main_bank(BLK) + (nibble * 4), wdata);  
   }

   // XPI_MAP_BITSLIP
   wdata = readIO_MBDR(mbdr_freq_base + MBDR_XPI_MAP_BITSLIP);
   DEBUG_PRINTF("INFO: ddr_restore_val(): XPI_MAP_BITSLIP: wdata = %x\n", wdata);
   writeIO_DMC(ral_addr_of_xpi_map_bitslip_in_ddrmc_main_ddrmc_main_bank(BLK), wdata);

   // if(glbl_phy_rank_switch_en == 1) {
   // Dummy transaction is needed for all the configurations to flush the XPI data path, so as to drive the signals to the DRAM correctly
      DEBUG_PRINTF("INFO: ddr_restore_val(): Dummy write for setting correct odelays\n");
      dummy_write_multi_rank_switch(0);

      DEBUG_PRINTF("INFO: ddr_restore_val(): Enabling dynamic mode for TX\n");
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
      riu_reg_bcast(DIS_DYN_MODE, 0, 0x0);

      DEBUG_PRINTF("INFO: ddr_restore_val(): Dummy write for setting correct odelays\n");
      dummy_write_multi_rank_switch(0);
   // }

   DEBUG_PRINTF("INFO: ddr_restore_val(): Save/Restore - Restore values: Done\n");
   return 0;
}

u8 restore_mini_cal()
{
   u8 status = 0;

   DEBUG_PRINTF("INFO: restore_mini_cal(): Mini-calibration: Start\n");
   DEBUG_PRINTF("INFO: restore_mini_cal(): Start dqs_gating stage\n");
   status = ddr_dqs_gate(1);
   if(status != 0) {
      return 1;
   }

   DEBUG_PRINTF("INFO: restore_mini_cal(): Temperature controlled refresh rate check\n");
   status = temp_control_ref_rate();
   if(status != 0) {
      return 1;
   }

   DEBUG_PRINTF("INFO: restore_mini_cal(): Send Refresh commands to memory\n");
   status = banks_refresh(0, 0x4);
   if(status) {
      return 1;
   }

   DEBUG_PRINTF("INFO: restore_mini_cal(): Start Read DQS tracking\n");
   status = ddr_read_dqs_track();
   if(status != 0) {
      return 1;
   }

   DEBUG_PRINTF("INFO: restore_mini_cal(): Send Refresh commands to memory\n");
   status = banks_refresh(0, 0x5);
   if(status) {
      return 1;
   }

   #ifdef LPDDR4
      DEBUG_PRINTF("INFO: restore_mini_cal(): Start LPDDR4 Write oscillator tracking\n");
      // FIXME: Add the code to start the LP4 write oscillator tracking

      u8 dc_fsm_sr = (readIO_DMC(ral_addr_of_reg_com_4_in_ddrmc_main_ddrmc_main_bank(BLK)) >> 2) & 0x1;

      if(dc_fsm_sr == 0) { // Only for DDRMC reset cases: Frequency switching, controller power down
         u32 phy_rank_write_override;
         u32 phy_rank_write_override_dyn;

         // Enable phy rank override dynamic mode before cal_done, to set the flags in the RTL correctly. EDT-997224.
         u8 write_cnt = (readIO_DMC(ral_addr_of_phy_rank_write_override_dyn_in_ddrmc_main_ddrmc_main_bank(BLK)) >> 14) & 0x1F;
         u8 ch1_rank_update = 0x3;
         u8 ch0_rank_update = 0x3;
         u8 ch1_pause_writes = 1;
         u8 ch0_pause_writes = 1;
         u8 override_dyn_en = 3;
         u8 ch1_update_pulse = 1;
         u8 ch0_update_pulse = 1;

         phy_rank_write_override = (glbl_ch1_mem_rank1_xphy_rank << 12) | (glbl_ch1_mem_rank0_xphy_rank << 10) | (glbl_ch0_mem_rank1_xphy_rank << 4) | (glbl_ch0_mem_rank0_xphy_rank << 2);
         writeIO_DMC(ral_addr_of_phy_rank_write_override_in_ddrmc_main_ddrmc_main_bank(BLK), phy_rank_write_override);

         phy_rank_write_override_dyn = (write_cnt << 14) | (ch1_rank_update << 10) | (ch0_rank_update << 6) | (ch1_pause_writes << 5) | (ch0_pause_writes << 4) | override_dyn_en; // Enable it upfront
         writeIO_DMC(ral_addr_of_phy_rank_write_override_dyn_in_ddrmc_main_ddrmc_main_bank(BLK), phy_rank_write_override_dyn);

         phy_rank_write_override_dyn |= (ch1_update_pulse << 3) | (ch0_update_pulse << 2);
         writeIO_DMC(ral_addr_of_phy_rank_write_override_dyn_in_ddrmc_main_ddrmc_main_bank(BLK), phy_rank_write_override_dyn);
      }

   #endif
   DEBUG_PRINTF("INFO: restore_mini_cal(): Mini-calibration: Done\n");

   return 0;
}

u8 sanity_check_complex_read (u32 loop_cnt) {
  u8 nibble;
  u32 loop;
  u8 rank=0;
  u16 rd_data;
  u32 dq_compare_result;

  // cplx_config register bits
  u32 start= 1; //0; // [0] Start flag
  u8  write_cal=0; // [1] "0: Read Cal 1: Write Cal" 
  u8  rank_en = (1 << rank); // [5:2] A 1 in a given bit position means that given rank is enabled in the test pattern. Rank0 is enabled by default.
  u8  dm_en=0; // [6] "0: DM is not asserted for Write Pattern 1: DM pattern is enabled (only for write cal)" 

   u32 cplx_config = ((write_cal&0x1)<<1) | ((rank_en&0xF)<<2) | ((dm_en&0x1)<<6) | ((CPLX_LOOP_CNT&0xFF)<<7) | ((CPLX_ROW_CNT&0x7)<<15) | ((CPLX_BANK_CNT&0x3)<<18) | ((CPLX_BURST_ARRAY_INDEX&0x1F)<<20) | ((CPLX_PATTERN_LENGTH&0x3F)<<25);

  for(loop=0; loop<loop_cnt; loop++) {
    // CLEARING THE COMPARE STATUS
    writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);
    writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

    // START THE COMPLEX CALIB AND THEN STOP
    cplx_config = cplx_config | start;
    writeIO_DMC(ral_addr_of_cplx_config_in_ddrmc_main_ddrmc_main_bank(BLK), cplx_config);
    cplx_config = cplx_config & (~start);
    writeIO_DMC(ral_addr_of_cplx_config_in_ddrmc_main_ddrmc_main_bank(BLK), cplx_config);

    do {
      rd_data = readIO_DMC(ral_addr_of_cplx_status_in_ddrmc_main_ddrmc_main_bank(BLK));
    } while((rd_data & 0x1) != 0x0);

    dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_nibble_compare_17_0;
    DEBUG_PRINTF("INFO: sanity_check_complex_read(): rank = %d, dq_compare_result = %x \n", rank, dq_compare_result);
    glbl_bits_error_status_31_0=dq_compare_result;
    if(dq_compare_result) { // ERROR case
      DEBUG_PRINTF("ERROR: sanity_check_complex_read(): dq_compare_result = %x\n", dq_compare_result);
      write_error_nibble_XRAM(0, dq_compare_result);
      return 1;
    } else {
      DEBUG_PRINTF("INFO: sanity_check_complex_read(): passed\n");
    }
  } // loop_cnt
  return 0;
}

void cplx_write_sample(u8 rank)
{
  u16 rd_data;

   // cplx_config register bits
   u32 start= 1; //0; // [0] Start flag
   u8  write_cal=1; // [1] "0: Read Cal 1: Write Cal" 
   u8  rank_en = (1 << rank); // [5:2] A 1 in a given bit position means that given rank is enabled in the test pattern. Rank0 is enabled by default.
   u8  dm_en=0; // [6] "0: DM is not asserted for Write Pattern 1: DM pattern is enabled (only for write cal)" 

   u32 cplx_config = ((write_cal&0x1)<<1) | ((rank_en&0xF)<<2) | ((dm_en&0x1)<<6) | ((CPLX_LOOP_CNT&0xFF)<<7) | ((CPLX_ROW_CNT&0x7)<<15) | ((CPLX_BANK_CNT&0x3)<<18) | ((CPLX_BURST_ARRAY_INDEX&0x1F)<<20) | ((CPLX_PATTERN_LENGTH&0x3F)<<25);

  // CLEARING THE COMPARE STATUS
  writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);
  writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

  // START THE COMPLEX CALIB AND THEN STOP
  cplx_config = cplx_config | start;
  writeIO_DMC(ral_addr_of_cplx_config_in_ddrmc_main_ddrmc_main_bank(BLK), cplx_config);
  cplx_config = cplx_config & (~start);
  writeIO_DMC(ral_addr_of_cplx_config_in_ddrmc_main_ddrmc_main_bank(BLK), cplx_config);

  do {
    rd_data = readIO_DMC(ral_addr_of_cplx_status_in_ddrmc_main_ddrmc_main_bank(BLK));
  } while((rd_data & 0x1) != 0x0);
}

u8 sanity_check_complex_write (u32 loop_cnt) {
  u8 nibble;
  u32 loop;
  u32 dq_compare_result;

  for(loop=0; loop<loop_cnt; loop++) {
    cplx_write_sample(0);

    dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_nibble_compare_17_0;
    DEBUG_PRINTF("INFO: sanity_check_complex_write(): rank = %d, dq_compare_result = %x \n", rank, dq_compare_result);

    if(dq_compare_result) { // ERROR case
      DEBUG_PRINTF("ERROR: sanity_check_complex_write(): dq_compare_result = %x\n", dq_compare_result);
      write_error_nibble_XRAM(0, dq_compare_result);
      return 1;
    } else {
      DEBUG_PRINTF("INFO: sanity_check_complex_write(): passed\n");
    }
  } // loop_cnt
  return 0;
}

void prbs_write_sample()
{
  u8 rd_loop = 0;
  u8 run_forever = 0;
  u8 frf_enable = 0;
  u8 col_bits = 8;
  u8 mode = 0;
  u8 rank_en = 1;
  u8 dly_rank_switch = 4;
  u16 seq_end = 4;
  u16 loop_end = 2;
  u16 rd_data;  

  u32 prbs_config = (rd_loop | (run_forever << 1) | (frf_enable << 2) | (col_bits << 3) | (mode << 7) | (rank_en << 9) | (dly_rank_switch<< 13) | (0 << 17));
  u32 prbs_cnt = (seq_end | (loop_end << 16));

  // CLEARING THE COMPARE STATUS
  writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);
  writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

  writeIO_DMC(ral_addr_of_prbs_config_in_ddrmc_main_ddrmc_main_bank(BLK), prbs_config);
  writeIO_DMC(ral_addr_of_prbs_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), prbs_cnt);
  writeIO_DMC(ral_addr_of_prbs_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

  do {
    rd_data = readIO_DMC(ral_addr_of_prbs_status_in_ddrmc_main_ddrmc_main_bank(BLK));
  } while((rd_data & 0x1) != 0x0);
}

u8 sanity_check_prbs_write (u32 loop_cnt) {
  u8 nibble;
  u32 loop;
  u32 dq_compare_result;

  for(loop=0; loop<loop_cnt; loop++) {
    prbs_write_sample();

    dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_nibble_compare_17_0;
    DEBUG_PRINTF("INFO: sanity_check_prbs_write(): rank = %d, dq_compare_result = %x \n", rank, dq_compare_result);

    if(dq_compare_result) { // ERROR case
      DEBUG_PRINTF("ERROR: sanity_check_prbs_write(): dq_compare_result = %x\n", dq_compare_result);
      write_error_nibble_XRAM(0, dq_compare_result);
      return 1;
    } else {
      DEBUG_PRINTF("INFO: sanity_check_prbs_write(): passed\n");
    }
  } // loop_cnt
  return 0;
}

u8 sanity_check_prbs_read (u32 loop_cnt) {
  u8 nibble;
  u32 loop;
  u8 rd_loop;
  u8 frf_enable;
  u32 prbs_config;
  u32 prbs_cnt;
  u16 rd_data;  
  u32 dq_compare_result;
  u8 run_forever = 0;
  u8 col_bits = 8;
  u8 mode = 0;
  u8 rank_en = 1;
  u8 dly_rank_switch = 4;
  u16 seq_end;
  u16 loop_end;

  // PRBS Write first before PRBS Read
  rd_loop = 0;
  frf_enable = 0;
  seq_end = 4;
  loop_end = 1;
  prbs_config = (rd_loop | (run_forever << 1) | (frf_enable << 2) | (col_bits << 3) | (mode << 7) | (rank_en << 9) | (dly_rank_switch<< 13) | (0 << 17));
  prbs_cnt = (seq_end | (loop_end << 16));

  writeIO_DMC(ral_addr_of_prbs_config_in_ddrmc_main_ddrmc_main_bank(BLK), prbs_config);
  writeIO_DMC(ral_addr_of_prbs_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), prbs_cnt); // Switch from write to read after 10 writes; And repeate the loop 1 times.
  writeIO_DMC(ral_addr_of_prbs_start_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);

  do {
    rd_data = readIO_DMC(ral_addr_of_prbs_status_in_ddrmc_main_ddrmc_main_bank(BLK));
  } while((rd_data & 0x1) != 0x0);

  // CLEARING THE COMPARE STATUS
  writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);
  writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

  // PRBS Read
  rd_loop = 1;
  frf_enable = 1;
  seq_end = 4;
  loop_end = 2;
  prbs_config = (rd_loop | (run_forever << 1) | (frf_enable << 2) | (col_bits << 3) | (mode << 7) | (rank_en << 9) | (dly_rank_switch<< 13) | (0 << 17));
  prbs_cnt = (seq_end | (loop_end << 16));

  writeIO_DMC(ral_addr_of_prbs_config_in_ddrmc_main_ddrmc_main_bank(BLK), prbs_config);
  writeIO_DMC(ral_addr_of_prbs_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), prbs_cnt); // Switch from write to read after 10 writes; And repeate the loop 2 times.

  for(loop=0; loop<loop_cnt; loop++) {
    writeIO_DMC(ral_addr_of_prbs_start_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);

    do {
      rd_data = readIO_DMC(ral_addr_of_prbs_status_in_ddrmc_main_ddrmc_main_bank(BLK));
    } while((rd_data & 0x1) != 0x0);

    dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_nibble_compare_17_0;
    DEBUG_PRINTF("INFO: sanity_check_prbs_read(): rank = %d, dq_compare_result = %x \n", rank, dq_compare_result);

    if(dq_compare_result) { // ERROR case
      DEBUG_PRINTF("ERROR: sanity_check_prbs_read(): dq_compare_result = %x\n", dq_compare_result);
      write_error_nibble_XRAM(0, dq_compare_result);
      return 1;
    } else {
      DEBUG_PRINTF("INFO: sanity_check_prbs_read(): passed\n");
    }
  } // loop_cnt
  return 0;
}

void simple_write_sample(u8 first_iteration)
{
  u8 current_rank;

  // Clearing the Compare Status
  writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);
  writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

  // Extended Write and Read Command
  for(current_rank = 0; current_rank < 1; current_rank++)
  {
    #ifdef DDR4
      set_2_seq_command_at_mem(current_rank /* rank */, (SAMPLE_CNT << 1) /* no of iteration */, first_iteration /* full write */, MEM_COL_WRITE /* seq_a_cmd */, 
                               0 /* seq_a_addr */, 0 /* seq_a_bg */, 0 /* seq_ba */, MEM_COL_READ /* seq_b_cmd */, 
                               0 /*seq_b_addr*/, 0 /*seq_b_bg*/, 0 /*seq_b_ba*/, 40 /*seq_a2b_dly*/, 40/*seq_b2a_dly*/);
    #else
      if (first_iteration & glbl_margin_check) {
        lp4_bank_row_access(0 /*u8 rank*/, ACTIVATE /*cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);
        lp4_bank_col_access(((SAMPLE_CNT*5) << 1), COL_WR, COL_RD, 0 /* u8 bank_addr */, 0 /* u8 col_addr */);
        //lp4_bank_row_access(0 /*u8 rank*/, PRECHARGE /*cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);
      } else {
        lp4_bank_col_access(((SAMPLE_CNT*5) << 1), COL_WR, COL_RD, 0 /* u8 bank_addr */, 0 /* u8 col_addr */);
      }
    #endif
  }
}

#ifdef DDR4
int get_centre(u16 *dqs_delay, u16 *dq_delay, u16 *dbi_delay, u16 *left_margin, u16 *dqs_right_delay) 
{
  u8 i, j;
  static u16 dq_shift[DQS_BITS];
  static u16 dqs_shift[DQS_BITS];
  static u16 wrlvl_odly_dqs[DQS_BITS][RANKS];
  u8 current_rank;
  u8 negative_gap;
  u8 negative_gap_valid;
  u16 mean_delay;

  // Do the centering based on L and R calculation
  for(i=0; i<DQS_BITS; i++)
  {
    mean_delay = (left_margin[i] + dqs_right_delay[i])/2; 

    DEBUG_PRINTF("INFO: get_centre(): left_margin[%d] = %d, right_margin[%d] = %d\n", i, left_margin[i], i, dqs_right_delay[i]);

    if(dqs_right_delay[i] > left_margin[i])
    {
      dqs_shift[i] = mean_delay;   
      dq_shift[i]  = 0;
    }
    else
    {
      dqs_shift[i] = dqs_right_delay[i];
      dq_shift[i]  = (left_margin[i] - dqs_right_delay[i])/2 ;
    }

    //Check the total DQS shift and then align the DQ and DQS delay
    if(dqs_delay[i] > 0)
    { 
      if(dq_shift[i] > dqs_delay[i])
      {
        dqs_shift[i] += dqs_delay[i];
        dq_shift[i]  -= dqs_delay[i];
      }
      else
      {
        dqs_shift[i] += dq_shift[i]  ;
        dq_shift[i]  = 0;;
      }
    }
  }

  // INC DEC MASK SET FOR DQS 
  for(i=0; i<DQS_BITS; i++)
  {
    write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], INCDEC_CRSE, dqs_shift[i]);

    if(NIBBLE_PER_DQS == 2)
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], INCDEC_CRSE, dqs_shift[i]);
  }

  // Broadcast CRSE Delay For DQS
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dqs_nibble_mask_initial);
  riu_reg_bcast(ODLY0, 1, 0x6000); 

  // Broadcast CRSE Delay For Tristate
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(TRISTATE_ODLY, 1, 0x6000); //Tristate Inc Dec Crse ... 15 14 13

  // INC_DEC MASK SET FOR DQ
  for(i=0; i<DQS_BITS; i++)
  {
    write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], INCDEC_CRSE, dq_shift[i]);

    if(NIBBLE_PER_DQS == 2)
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], INCDEC_CRSE, dq_shift[i]);
  }

  writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(IODLY_BCAST_MASK, 1, 0x000000FF); // 0000_1111_1111 
  riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000038); // 0000_0010_1000 // Increment By coarse

  if(glbl_wr_dbi_en)
  {
    // INC DEC mask set for DBI
    for(i=0; i<DQS_BITS; i++)
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], INCDEC_CRSE, dq_shift[i]);

    // Broadcast Increment the DBI
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dbi_nibble_mask_initial);
    riu_reg_bcast(ODLY0, 1, 0xA000); // Step Wise Increment of ODLY
  }

  bitslice_reset_bcast(1, BS_RESET_WT);

  return 0;
}

int get_delay_calculation(u16 *left_margin, u16 *dq_delay, u16 *dbi_delay)
{
  u8 i,j;
  u8 crse_upd_2step = 0;
  u32 dq_nibble_en_crse_upd_2step = 0;

  for(i=0; i<DQS_BITS; i++)
  {
    for(j=0;j<(4*NIBBLE_PER_DQS); j++)
    {
      if(j == 0)
        left_margin[i] = dq_delay[(i*4*NIBBLE_PER_DQS) + j];
      else if(left_margin[i] > dq_delay[(i*4*NIBBLE_PER_DQS) + j])
        left_margin[i] = dq_delay[(i*4*NIBBLE_PER_DQS) + j];
    }
     
    if(glbl_wr_dbi_en) 
    {
      if(left_margin[i] > dbi_delay[i])
        left_margin[i] = dbi_delay[i];
    }
    DEBUG_PRINTF("INFO: get_delay_calculation(): left_margin[%d] = %d\n", i, left_margin[i]);
  }

  // Setting the INCDEC Register corrosponding to the left margin
  for(i=0; i<DQS_BITS; i++)
  {
    if(left_margin[i] > 0xFF) { // If coarse inc/dec is more than 256, do it in two steps.
      crse_upd_2step = 1;

      dq_nibble_en_crse_upd_2step |= (0x1 << glbl_map_riu[(i*NIBBLE_PER_DQS)]);
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], INCDEC_CRSE, (left_margin[i] - 0xFF));

      if(NIBBLE_PER_DQS == 2) {
        dq_nibble_en_crse_upd_2step |= (0x1 << glbl_map_riu[(i*NIBBLE_PER_DQS) + 1]);
        write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], INCDEC_CRSE, (left_margin[i] - 0xFF));
      }
    } else {
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], INCDEC_CRSE, left_margin[i]);
       
      if(NIBBLE_PER_DQS == 2)
        write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], INCDEC_CRSE, left_margin[i]);
    }
  }

  // BroadCast and Subtract this Left Margin Delay to corrosponding nibbles For DQ 
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(IODLY_BCAST_MASK, 1, 0x000000ff); // 0000_1111_1111 
  riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000028); // 0000_0001_1000

  if(glbl_wr_dbi_en)
  {
    // BroadCast and Subtract this Left Margin Delay to corrosponding nibbles For DBI 
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dbi_nibble_mask_initial);
    riu_reg_bcast(ODLY0, 1, 0x6000); // In this ODLY DBI is Mapped
  }

  // Do delay updates in 2 steps in INCDEC_CRSE is going more than 255
  if(crse_upd_2step == 1) {
    DEBUG_PRINTF("INFO: get_delay_calculation(): INCDEC_CRSE update in two steps. Nibble enable for 2nd step = %x\n", i, dq_nibble_en_crse_upd_2step);
    writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
    riu_reg_bcast(INCDEC_CRSE, 1, 0x0);
    writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, dq_nibble_en_crse_upd_2step);
    riu_reg_bcast(INCDEC_CRSE, 1, 0xFF);

    // BroadCast and Subtract this Left Margin Delay to corrosponding nibbles For DQ 
    writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
    riu_reg_bcast(IODLY_BCAST_MASK, 1, 0x000000ff); // 0000_1111_1111 
    riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000028); // 0000_0001_1000

    if(glbl_wr_dbi_en)
    {
      // BroadCast and Subtract this Left Margin Delay to corrosponding nibbles For DBI 
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dbi_nibble_mask_initial);
      riu_reg_bcast(ODLY0, 1, 0x6000); // In this ODLY DBI is Mapped
    }
  }

  bitslice_reset_bcast(1, BS_RESET_WT);

  return 0;
}
#endif

int get_wr_center_sanity(u8 rank, u8 complex_en)
{
  u32 dq_compare_result;
  u32 rd_data;
  u8 i;

  DEBUG_PRINTF("INFO: get_wr_center_sanity(): start\n");

  if(complex_en)
  {
    cplx_write_sample(rank);

    dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_nibble_compare_17_0;
    DEBUG_PRINTF("INFO: get_wr_center_sanity(): dq_compare_result = %x \n", dq_compare_result);

    if(dq_compare_result) { // ERROR case
      DEBUG_PRINTF("ERROR: get_wr_center_sanity(): dq_compare_result = %x\n", dq_compare_result);
      write_error_nibble_XRAM(0, dq_compare_result);
      return 1;
    }
  }
  else
  {
    single_write_read(0, WR_EXTEND_DATA, 4); 

    dq_compare_result = 0;
    for(i=0; i<DQS_BITS; i++)
    {
      rd_data = (readIO_DMC(ral_addr_of_bl8_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*i*NIBBLE_PER_DQS)));

      if(rd_data != WR_CENTER_SANITY_TEST_PATTERN) {
        DEBUG_PRINTF("ERROR: get_wr_center_sanity(): Data Mismatch for DQS %d. expected data = %x, read data = %x\n", i, WR_CENTER_SANITY_TEST_PATTERN, rd_data);
        dq_compare_result |= (0x1 << (i*NIBBLE_PER_DQS));
      }
    }
    if(dq_compare_result) {
      write_error_nibble_XRAM(0, dq_compare_result);
      return 1;
    }
  }

  return 0;
}

#ifdef DDR4
int get_right_edge (u16 *dqs_right_delay, u8 step_size, u8 pattern_type)
{
  u8 i;
  u32 dq_odelay_per_byte_flag = 0;
  u8 first_iteration = 1;
  u16 dqs_dly_count = 0;
  u32 dq_compare_result;
  u32 nibble_mask;

  // Move The DQS to Right For Finding the R margin 
  for(i=0; i<DQS_BITS; i++)
  {
    dq_odelay_per_byte_flag     |= (1<<i);
  }
  
  dq_odelay_per_byte_flag = ~dq_odelay_per_byte_flag;

  // Setting the Step Size into the INC_DEC Register
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(INCDEC_CRSE, 1, step_size);

  while(dq_odelay_per_byte_flag != 0xFFFFFFFF)
  {
    dqs_dly_count = dqs_dly_count + step_size;
    DEBUG_PRINTF("INFO: get_right_edge(): dqs_dly_count = %d\n", dqs_dly_count);

    if(dqs_dly_count > glbl_max_limit_180_degree) {
      DEBUG_PRINTF("ERROR: get_right_edge(): Delay crossed more than 180 degrees\n");
      write_error_nibble_XRAM(0, ~dq_odelay_per_byte_flag);
      return 1;
    }

    for(i=0; i<DQS_BITS; i++)
    {
      if((dq_odelay_per_byte_flag & (1<<i)) == 0)
        dqs_right_delay[i] += step_size;

      DEBUG_PRINTF("INFO: get_right_edge(): dqs_right_delay[%d] = %d\n",i, dqs_right_delay[i]);
    }

    // Broadcast the Odelay for those bytes for which byte_flag is not set (FOR DQS)
    update_dly_mask(&dq_odelay_per_byte_flag, &nibble_mask);
    writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
    riu_reg_bcast(ODLY0, 1, 0xA000); 

    // Broadcast the Tristate
    update_nibble_dly_mask(&dq_odelay_per_byte_flag, &nibble_mask);
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, nibble_mask);
    riu_reg_bcast(TRISTATE_ODLY, 1, 0xA000); //Tristate Inc Dec Crse ... 15 14 13

    DEBUG_PRINTF("INFO: get_right_edge(): dq_odelay_per_byte_flag = %x\n", dq_odelay_per_byte_flag);

    if(pattern_type == PRBS)
      prbs_write_sample();
    else 
    {
      simple_write_sample(first_iteration);
      first_iteration = 0;
    }

    dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK));  
    DEBUG_PRINTF("INFO: get_right_edge(): dq_compare_result = %x\n", dq_compare_result);

    // Here We broadcast the odelay increment in every dq per dqs
    for(i=0; i<DQS_BITS; i++)
    {
      if((dq_odelay_per_byte_flag & (1<<i)) == 0)
      {
        if(((NIBBLE_PER_DQS == 2) && ((dq_compare_result & 0x3) != 0)) || ((NIBBLE_PER_DQS == 1) && ((dq_compare_result & 0x1) != 0)))
          dq_odelay_per_byte_flag |= (1<<i);
      }

      dq_compare_result = (dq_compare_result >> NIBBLE_PER_DQS);
    }
  }

  // Decrement the step_size for all bytes
  dqs_dly_count -=  step_size;
  DEBUG_PRINTF("INFO: get_right_edge(): final dqs_dly_count = %d\n", dqs_dly_count);

  for(i=0; i<DQS_BITS; i++)
  { 
    dqs_right_delay[i] -= step_size;
    DEBUG_PRINTF("INFO: get_right_edge(): final dqs_right_delay[%d] = %d\n", i, dqs_right_delay[i]);
  }

  // Broadcast the Odelay for those bytes for which byte_flag is not set (FOR DQS)
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, ~glbl_dqs_nibble_mask_initial);
  riu_reg_bcast(ODLY0, 1, 0x6000); 

  // Broadcast the Tristate
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(TRISTATE_ODLY, 1, 0x6000); //Tristate Inc Dec Crse ... 15 14 13

  return 0;
}

int get_dbi_bit_deskew(u16 *dbi_delay, u8 step_size)
{
  u8 i;
  u32 dbi_odelay_per_byte_flag = 0;
  u32 dq_compare_result;
  u16 dbi_dly_count = 0;
  u32 nibble_mask;
  u8 first_iteration = 1;
 
  for(i=0; i<DQS_BITS; i++)
    dbi_odelay_per_byte_flag |= (1<<i);

  dbi_odelay_per_byte_flag = ~dbi_odelay_per_byte_flag;

  writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

  while(dbi_odelay_per_byte_flag != 0xFFFFFFFF)
  {
    dbi_dly_count = dbi_dly_count + step_size;
    DEBUG_PRINTF("INFO: get_dbi_bit_deskew(): dbi_dly_count = %d \n", dbi_dly_count);

    if(dbi_dly_count > glbl_max_limit_180_degree) {
      DEBUG_PRINTF("ERROR: get_dbi_bit_deskew(): Delay crossed more than 180 degrees\n");
      write_error_nibble_XRAM(0, ~dbi_odelay_per_byte_flag);
      return 1;
    }

    for(i=0; i<DQS_BITS; i++) {
      if((dbi_odelay_per_byte_flag & (1<<i)) == 0)
        dbi_delay[i] += step_size;
    }

    // Setting the Step Size into the INC DEC Register
    update_nibble_dly_mask(&dbi_odelay_per_byte_flag, &nibble_mask); 
    writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
    riu_reg_bcast(INCDEC_CRSE, 1, step_size);

    // Broadcast the DBI Odelay for those bytes for which byte flag is not set 
    update_byte_dly_mask(&dbi_odelay_per_byte_flag, &nibble_mask);
    writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
    riu_reg_bcast(ODLY0, 1, 0xA000); // Step Wise Increment of ODLY

    simple_write_sample(first_iteration);
    first_iteration = 0;
   
    dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK));  
    DEBUG_PRINTF("INFO: get_dbi_bit_deskew(): dq_compare_result = %x \n", dq_compare_result);

    // Here We broadcast the odelay increment in every dbi per dqs
    for(i=0; i<DQS_BITS; i++) {
      if((dbi_odelay_per_byte_flag & (1<<i)) == 0) {
        if((dq_compare_result & 0x3) != 0)
          dbi_odelay_per_byte_flag |= (1<<i);
      }

      dq_compare_result = (dq_compare_result >> NIBBLE_PER_DQS);
    }

    DEBUG_PRINTF("INFO: get_dbi_bit_deskew(): dbi_odelay_per_byte_flag = %x \n", dbi_odelay_per_byte_flag);
  }

  dbi_dly_count -=  step_size;
  DEBUG_PRINTF("INFO: get_dbi_bit_deskew(): Final dbi_dly_count = %d \n", dbi_dly_count);
  for(i=0; i<DQS_BITS; i++)
    dbi_delay[i] -= step_size;

  // Broadcast the DBI Odelay for those bytes for which byte flag is not set 
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dbi_nibble_mask_initial);
  riu_reg_bcast(ODLY0, 1, 0x6000); // Step Wise Decrement of ODLY

  return 0;
}

int get_dbi_common_valid_window(u16 *dqs_delay, u16 *dbi_delay)
{
  u8 i;
  u8 byte;
  u32 addr, odly;
  u32 dbi_odelay_per_byte_flag = 0;
  static u16 dqs_count[DQS_BITS];
  static u8 count[DQS_BITS];
  u32 initial_flag;
  u32 nibble_mask;
  u8 first_iteration = 1;
  u16 dbi_dly_count = 0;
  u16 dqs_dly_count = 0;
  u32 dbi_flag = 0;
  u8 dq_compare_result;

  for(i=0; i<DQS_BITS; i++)
  {
    dbi_odelay_per_byte_flag |= (1<<i);
    dqs_count[i]             = 0; 
    count[i] = 0;
  }

  dbi_odelay_per_byte_flag = ~dbi_odelay_per_byte_flag;
  initial_flag = dbi_odelay_per_byte_flag;            

  //----------------------------------------------------------------//
  //-------Start Getting the Valid Window by moving DQS------//
  //----------------------------------------------------------------//

  // Moving DQS to find the Valid Window For DBI Pins
  while((dbi_odelay_per_byte_flag != 0xFFFFFFFF) && (dqs_dly_count <= glbl_max_limit_90_degree))
  {
    simple_write_sample(first_iteration);
    first_iteration = 0;

    dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK));  
    DEBUG_PRINTF("INFO: get_dbi_common_valid_window(): dq_compare_result = %x \n", dq_compare_result);

    for(i=0; i<DQS_BITS; i++)
    {
      if((dbi_odelay_per_byte_flag & (1<<i)) == 0)
      {
        if((dq_compare_result & 0x3) == 0)
        {
          count[i] = count[i] + 1;

          if(count[i] >= MIN_VLD_CNT)
          {
            dbi_odelay_per_byte_flag |= (1<<i);
            dqs_count[i]             = dqs_dly_count;
          }
        }
        else
          count[i] = 0;
      }
 
      dq_compare_result = (dq_compare_result >> NIBBLE_PER_DQS);
    }

    dqs_dly_count = dqs_dly_count + 1; 

    // BroadCast the Odelay for Those Bytes for which byte flag is not set (FOR DQS)
    update_dly_mask(&dbi_odelay_per_byte_flag, &nibble_mask);
    writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
    riu_reg_bcast(ODLY0, 1, 0x8000); 

    // Broadcast the Tristate
    update_nibble_dly_mask(&dbi_odelay_per_byte_flag, &nibble_mask);
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, nibble_mask);
    riu_reg_bcast(TRISTATE_ODLY, 1, 0x8000); //Tristate Inc Dec Crse ... 15 14 13
  }
 
// XRAM_DBG: wrdqdbi_stg4_dqs_delay
  for(byte=0; byte<DQS_BITS; byte++) {
    writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG4_DQS_DELAY + (byte * 4), dqs_count[byte]);
  }
// XRAM_DBG: wrdqdbi_stg4_byte_status
  writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG4_BYTE_STATUS, dbi_odelay_per_byte_flag);
  writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG4_BYTE_STATUS + 4, (dbi_odelay_per_byte_flag >> 9));

  if(dbi_odelay_per_byte_flag != 0xFFFFFFFF)
  {
    // INCDEC Crse setting to revert the DQS delay to the initial value
    for(i=0; i<DQS_BITS; i++)
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], INCDEC_CRSE, glbl_max_limit_90_degree - dqs_count[i]);

    // DQS Bcast to revert it to the LAST Stable Value
    update_dly_mask(&dbi_odelay_per_byte_flag, &nibble_mask);
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, nibble_mask);
    riu_reg_bcast(ODLY0, 1, 0x6000); 

    // Broadcast the tristate
    update_nibble_dly_mask(&dbi_odelay_per_byte_flag, &nibble_mask);
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, nibble_mask);
    riu_reg_bcast(TRISTATE_ODLY, 1, 0x6000); //Tristate Inc Dec Crse ... 15 14 13
  }

  //......... Decrease the DQS By Min Valid Count ..................//

  // Reverting the DQS by MIN_VLD_CNT value
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(INCDEC_CRSE, 1, MIN_VLD_CNT-1);

  dbi_flag = (dbi_odelay_per_byte_flag & initial_flag) | ((~initial_flag) & (~dbi_odelay_per_byte_flag));

  // Broadcast the DQS
  update_dly_mask(&dbi_flag, &nibble_mask);
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
  riu_reg_bcast(ODLY0, 1, 0x6000); 

  // Broadcast the Tristate
  update_nibble_dly_mask(&dbi_flag, &nibble_mask);
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, nibble_mask);
  riu_reg_bcast(TRISTATE_ODLY, 1, 0x6000); //Tristate Inc Dec Crse ... 15 14 13

  for(i=0; i<DQS_BITS; i++)
  {
    if((dbi_odelay_per_byte_flag & (1<<i)) != 0)
      dqs_delay[i] += dqs_count[i] - MIN_VLD_CNT + 1;

    DEBUG_PRINTF("INFO: get_dbi_common_valid_window(): dqs_delay[%d] = %d\n", i, dqs_delay[i]);
  }

// XRAM_DBG: wrdqdbi_stg5_dqs_odly
  for(byte=0; byte<DQS_BITS; byte++) {
    addr = (glbl_dqs_bits_riu_map[byte][0] << RIU_NIB_POS) | (glbl_dqs_bits_riu_map[byte][1] + ODLY0);
    odly = readIO_RIU(addr) & 0x1FF;
    writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG5_DQS_ODLY + (byte * 4), odly);
  }

  //----------------------------------------------------------------//
  //-------Start Getting the Common Valid Window by moving DBI------//
  //----------------------------------------------------------------//
  for(i=0; i<DQS_BITS; i++)
    count[i] = 0; 

  while(dbi_odelay_per_byte_flag != 0xFFFFFFFF)
  {
    simple_write_sample(0);
    dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK));  
    DEBUG_PRINTF("INFO: get_dbi_common_valid_window(): dq_compare_result = %x \n", dq_compare_result);

    for(i=0; i<DQS_BITS; i++)
    {
      if((dbi_odelay_per_byte_flag & (1<<i)) == 0)
      {
        if((dq_compare_result & 0x3) == 0)
        {
          count[i] = count[i] + 1;

          if(count[i] >= MIN_VLD_CNT)
            dbi_odelay_per_byte_flag |= (1<<i);
        }
        else
          count[i] = 0;
      }
      dq_compare_result = (dq_compare_result >> NIBBLE_PER_DQS);
    }

    if(dbi_odelay_per_byte_flag != 0xFFFFFFFF) {
       dbi_dly_count = dbi_dly_count + 1; 

       if(dbi_dly_count > glbl_max_limit_180_degree) {
         DEBUG_PRINTF("ERROR: get_dbi_common_valid_window(): Delay crossed more than 180 degrees\n");
         write_error_nibble_XRAM(0, ~dbi_odelay_per_byte_flag);
         return 1;
       }

       for(i=0; i<DQS_BITS; i++)
       {
         if((dbi_odelay_per_byte_flag & (1<<i)) == 0)
           dbi_delay[i] += 1;
       }

       // Broadcast the Odelay For DBI Bit to increment by 1
       update_byte_dly_mask(&dbi_odelay_per_byte_flag, &nibble_mask);
       writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, nibble_mask);
       riu_reg_bcast(ODLY0, 1, 0x8000); // Step Wise Increment of ODLY
    }
  }

  return 0;
}

int get_dq_per_bit_deskew (u16 *dq_delay, u8 step_size)
{
  u8 i, j;
  u8 bit;
  u32 addr, odly;
  u32 dq_odelay_per_byte_flag = 0;
  static u8 dq_odelay_per_bit_flag[DQS_BITS];
  u8 bit_mask = (NIBBLE_PER_DQS == 1) ? 0xF0 : 0x00;
  u8 dq_compare_result;
  u16 dq_dly_count = 0;
  u32 nibble_mask;
  u8 first_iteration = 1;
  u16 bcast_mask;
  u8 seq_en = 0;

  for(i=0; i<DQS_BITS; i++)
  {
    dq_odelay_per_bit_flag[i] = 0;  
    dq_odelay_per_byte_flag   |= (1<<i);
  }

  dq_odelay_per_byte_flag = ~dq_odelay_per_byte_flag;

  writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

  for(i=0; i<DQS_BITS; i++) {
    write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], IODLY_BCAST_MASK, 0XFF);

    if(NIBBLE_PER_DQS == 2) {
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], IODLY_BCAST_MASK, 0xFF);
    }
  }

  // Setting the Step Size into the INC DEC Register
  update_nibble_dly_mask(&dq_odelay_per_byte_flag, &nibble_mask); 
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
  riu_reg_bcast(INCDEC_CRSE, 1, step_size);

  while(dq_odelay_per_byte_flag != 0xFFFFFFFF)
  {
    simple_write_sample(first_iteration);
    first_iteration = 0;

    glbl_bits_error_status_31_0  = readIO_DMC(ral_addr_of_compare_err_per_bit0_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_bit_compare_31_0;
    glbl_bits_error_status_63_32 = readIO_DMC(ral_addr_of_compare_err_per_bit1_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_bit_compare_63_32;
    glbl_bits_error_status_95_64 = readIO_DMC(ral_addr_of_compare_err_per_bit2_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_bit_compare_95_64;

    if(!( (glbl_bits_error_status_31_0 ) || (glbl_bits_error_status_63_32 ) || (glbl_bits_error_status_95_64 ) ) ) {
      seq_en = 0;
      for(i=0; i<DQS_BITS; i++) {
        for(j=0; j<(4*NIBBLE_PER_DQS); j++) {
          dq_delay[(i*NIBBLE_PER_DQS*4) + j] += step_size;  
        }
      }
    } else {
      seq_en = 1;
    }

    DEBUG_PRINTF("INFO: get_dq_per_bit_deskew(): Nibble sequential processing = %d\n", seq_en);

    // Here We broadcast the odelay increment in every dq per dqs
    if(seq_en == 1) {
      for(i=0; i<DQS_BITS; i++)
      {
        if((dq_odelay_per_byte_flag & (1<<i)) == 0)
        {
          dq_compare_result = (0x0f & readIO_DMC(ral_addr_of_compare_perbit_err_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*NIBBLE_PER_DQS * i)));

          if(NIBBLE_PER_DQS == 2)
            dq_compare_result |= (readIO_DMC(ral_addr_of_compare_perbit_err_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*NIBBLE_PER_DQS * i) + 4) << 4);

          DEBUG_PRINTF("INFO: get_dq_per_bit_deskew(): dq_compare_result = %x \n", dq_compare_result);

          for(j=0; j<(4*NIBBLE_PER_DQS); j++)
          {
            if((dq_odelay_per_bit_flag[i] & (1<<j)) == 0)
            {
              if((dq_compare_result & (1<<j)) != 0) 
              {
                dq_odelay_per_bit_flag[i]     = dq_odelay_per_bit_flag[i] | (1<<j);
                DEBUG_PRINTF("INFO: get_dq_per_bit_deskew(): dq_odelay_per_bit_flag[%d] = %x \n", i, dq_odelay_per_bit_flag[i]); 
              }
            }
          }

          if((dq_odelay_per_bit_flag[i] | bit_mask) == 0xFF)
            dq_odelay_per_byte_flag |= (1<<i);
        }
      }
    }

    DEBUG_PRINTF("INFO: get_dq_per_bit_deskew(): dq_odelay_per_byte_flag = %x \n", dq_odelay_per_byte_flag); 

    if(dq_odelay_per_byte_flag != 0xFFFFFFFF) 
    {
      dq_dly_count = dq_dly_count + step_size;

      if(dq_dly_count > glbl_max_limit_180_degree) {
        DEBUG_PRINTF("ERROR: get_dq_per_bit_deskew(): Delay crossed more than 180 degrees during deskew stage\n");
        write_error_nibble_XRAM(0, ~dq_odelay_per_byte_flag);

        glbl_bits_error_status_31_0 = 0;
        glbl_bits_error_status_63_32 = 0;
        glbl_bits_error_status_95_64 = 0;
        for(i=0; i<DQS_BITS; i++) {
          if(i*BITS_PER_DQS < 32) {
            glbl_bits_error_status_31_0 |= ((~dq_odelay_per_bit_flag[i] & BITS_PER_DQS_MASK) << ((i * BITS_PER_DQS)%32));
          } else if(i*BITS_PER_DQS < 64) {
            glbl_bits_error_status_63_32 |= ((~dq_odelay_per_bit_flag[i] & BITS_PER_DQS_MASK) << ((i * BITS_PER_DQS)%32));
          } else {
            glbl_bits_error_status_95_64 |= ((~dq_odelay_per_bit_flag[i] & BITS_PER_DQS_MASK) << ((i * BITS_PER_DQS)%32));
          }
        }
        return 1;
      }

      if(seq_en == 1) {
        for(i=0; i<DQS_BITS; i++) {
          for(j=0; j<(4*NIBBLE_PER_DQS); j++) {
            if((dq_odelay_per_bit_flag[i] & (1<<j)) == 0)
              dq_delay[(i*NIBBLE_PER_DQS*4) + j] += step_size;  

            DEBUG_PRINTF("INFO: get_dq_per_bit_deskew(): dq_delay[%d] = %d\n", (i*NIBBLE_PER_DQS*4)+j, dq_delay[(i*NIBBLE_PER_DQS*4) + j]);
          }
        }

        // BCAST MASK will be set based on individual byte
        for(i=0; i<DQS_BITS; i++)
        {
          bcast_mask = 0;

          // Logical to Physical Mapping of the Bcast Mask
          for(j=0; j<(NIBBLE_PER_DQS*4); j++)
          {
            if(dq_odelay_per_bit_flag[i] & (1<<j))
            {
              if(glbl_dqs_bits_riu_map[i][0] == glbl_dq_bits_riu_map[(4*i*NIBBLE_PER_DQS) + j][0])  
                bcast_mask |= (0x1 << (glbl_dq_bits_riu_map[(4*i*NIBBLE_PER_DQS) + j][1] - 2));  
              else
                bcast_mask |= (0x1 << (4 + (glbl_dq_bits_riu_map[(4*i*NIBBLE_PER_DQS) + j][1] - 2)));  
            }
          }
         
          write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], IODLY_BCAST_MASK, ((bcast_mask & 0xF) << 8) | 0XFF);

          if(NIBBLE_PER_DQS == 2)
          {
            bcast_mask = (bcast_mask >> 4) & 0xF;
            write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], IODLY_BCAST_MASK, (bcast_mask << 8) | 0xFF);
          }
        }
      }

      // Need BroadCast the ODELAY for those bytes for which byte_flag is not set
      update_nibble_dly_mask(&dq_odelay_per_byte_flag, &nibble_mask);
      writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
      riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000038); // 0000_0000_1100 // Increment By Coarse
    }
    bitslice_reset_bcast(1, BS_RESET_WT);
  }

  for(i=0; i<DQS_BITS; i++)
  {
    for(j=0; j<(4*NIBBLE_PER_DQS); j++)
    {
      dq_delay[(i*NIBBLE_PER_DQS*4) + j] -= step_size;
      DEBUG_PRINTF("INFO: get_dq_per_bit_deskew(): dq_delay[%d] = %d\n", (i*NIBBLE_PER_DQS*4)+j, dq_delay[(i*NIBBLE_PER_DQS*4) + j]);
    }
  }

  // BCAST MASK will be set based on individual byte
  for(i=0; i<DQS_BITS; i++)
  {
    write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], IODLY_BCAST_MASK, 0xFF);

    if(NIBBLE_PER_DQS == 2)
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], IODLY_BCAST_MASK, 0xFF);
  }

  // Need BroadCast the ODELAY for those bytes for which byte_flag is not set
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000028); // 0000_0000_1100 // Decrement by coarse
  bitslice_reset_bcast(1, BS_RESET_WT);

  return 0; 
}   

int get_dq_common_valid_window(u16 *dqs_delay, u16 *dq_delay)
{
  u8 i, j;
  u8 bit, byte;
  u32 addr, odly;
  u32 dq_odelay_per_byte_flag = 0;
  static u8 dq_odelay_per_bit_flag[DQS_BITS];
  u16 dq_dly_count = 0;
  u16 dqs_dly_count = 0;
  u8 bit_mask = (NIBBLE_PER_DQS == 1) ? 0xF0 : 0x00;
  u32 dq_compare_result;
  static u8 count[DQS_BITS][4*NIBBLE_PER_DQS];
  static u16 dqs_count[DQS_BITS];
  u16 bcast_mask;
  u32 initial_flag;
  u32 nibble_mask;
  u8 first_iteration = 1;
  u8 seq_en = 0;

  for(i=0; i<DQS_BITS; i++)
  {
    dq_odelay_per_byte_flag     |= (1<<i);
    dq_odelay_per_bit_flag[i]   = 0;
    dqs_count[i]                = 0; 

    for(j=0; j<(4*NIBBLE_PER_DQS); j++)
      count[i][j] = 0;
  }

  dq_odelay_per_byte_flag = ~dq_odelay_per_byte_flag;
  initial_flag = dq_odelay_per_byte_flag;            

  writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

  //----------------------------------------------------------------//
  //-------Start Getting the Common Valid Window by moving DQS------//
  //----------------------------------------------------------------//

  DEBUG_PRINTF("INFO: get_dq_common_valid_window(): Start Moving DQS to find the Common DQ Valid Window");
  
  for(i=0; i<DQS_BITS; i++) {
    write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], IODLY_BCAST_MASK, (0xF << 8) | 0XFF);

    if(NIBBLE_PER_DQS == 2) {
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], IODLY_BCAST_MASK, (0xF << 8) | 0xFF);
    }
  }

  // BroadCast the Odelay for DQ Bits to increment by 1
  update_nibble_dly_mask(&dq_odelay_per_byte_flag, &nibble_mask);
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, nibble_mask);
  riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000018); // 0000_0000_1100
  bitslice_reset_bcast(1, BS_RESET_WT);

  // Moving DQS to Find the Common Valid Window For DQ Pins
  while((dq_odelay_per_byte_flag != 0xFFFFFFFF) && (dqs_dly_count <= glbl_max_limit_90_degree))
  {
    simple_write_sample(first_iteration);
    first_iteration = 0;

    glbl_bits_error_status_31_0  = readIO_DMC(ral_addr_of_compare_err_per_bit0_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_bit_compare_31_0;
    glbl_bits_error_status_63_32 = readIO_DMC(ral_addr_of_compare_err_per_bit1_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_bit_compare_63_32;
    glbl_bits_error_status_95_64 = readIO_DMC(ral_addr_of_compare_err_per_bit2_in_ddrmc_main_ddrmc_main_bank(BLK)) & glbl_bit_compare_95_64;

    if(!( (glbl_bits_error_status_31_0 ) || (glbl_bits_error_status_63_32 ) || (glbl_bits_error_status_95_64 ) ) ) {
      seq_en = 0;
      for(i=0; i<DQS_BITS; i++) {
        for(j=0; j<(4*NIBBLE_PER_DQS); j++) {
          if(count[i][j] >= MIN_VLD_CNT) {
            seq_en = 1;
            break;
          }
        }
        if(seq_en != 0)
          break;
      }
      if(seq_en == 0) {
        for(i=0; i<DQS_BITS; i++) {
          for(j=0; j<(4*NIBBLE_PER_DQS); j++) {
            count[i][j] = count[i][j] + 1;
            dq_delay[(i*4*NIBBLE_PER_DQS) + j] += 1;
          }
        }
      }
    } else {
      seq_en = 1;
    }

    DEBUG_PRINTF("INFO: get_dq_common_valid_window(): Nibble sequential processing = %d\n", seq_en);

    if(seq_en == 1) {
      for(i=0; i<DQS_BITS; i++)
      {
        if((dq_odelay_per_byte_flag & (1<<i)) == 0)
        {
          dq_compare_result = (0x0F & readIO_DMC(ral_addr_of_compare_perbit_err_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*NIBBLE_PER_DQS * i)));

          if(NIBBLE_PER_DQS == 2)
            dq_compare_result |= (readIO_DMC(ral_addr_of_compare_perbit_err_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*NIBBLE_PER_DQS * i) + 4) << 4);

          DEBUG_PRINTF("INFO: get_dq_common_valid_window(): dq_compare_result = %x \n", dq_compare_result);

          for(j=0; j<(4*NIBBLE_PER_DQS); j++)
          {
            if((dq_odelay_per_bit_flag[i] & (1<<j)) == 0)
            {
              if((dq_compare_result & (1<<j)) == 0)
              {
                count[i][j] = count[i][j] + 1;

                if(count[i][j] >= MIN_VLD_CNT)
                {
                  dq_odelay_per_bit_flag[i] = dq_odelay_per_bit_flag[i] | (1<<j);
                  dqs_count[i] = dqs_dly_count;
                  DEBUG_PRINTF("INFO: get_dq_common_valid_window(): dq_odelay_per_bit_flag[%d] = %x \n", i, dq_odelay_per_bit_flag[i]); 
                  DEBUG_PRINTF("INFO: get_dq_common_valid_window(): dqs_count[%d] = %d \n", i, dqs_count[i]);
                }
              }
              else
                count[i][j] = 0;

              DEBUG_PRINTF("INFO: get_dq_common_valid_window(): count[%d][%d] value = %d \n", i, j, count[i][j]);
            }
          }

          if((dq_odelay_per_bit_flag[i] | bit_mask) == 0xFF)        
            dq_odelay_per_byte_flag |= (1<<i);
        }
      }
    }
 
    DEBUG_PRINTF("INFO: get_dq_common_valid_window(): dq_odelay_per_byte_flag = %x \n", dq_odelay_per_byte_flag); 
    
    dqs_dly_count = dqs_dly_count + 1;

    DEBUG_PRINTF("INFO: get_dq_common_valid_window(): dqs_dly_count = %d \n", dqs_dly_count);

    // Broadcast the Odelay for those bytes for which byte_flag is not set (FOR DQS)
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dqs_nibble_mask_initial);
    riu_reg_bcast(ODLY0, 1, 0x8000); // DQS Inc Dec Crse ... 15 14 13

    // Broadcast the Tristate Corrosponding to DQS
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
    riu_reg_bcast(TRISTATE_ODLY, 1, 0x8000); //Tristate Inc Dec Crse ... 15 14 13
    riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000018);

    if(seq_en == 1) {
      for(i=0; i<DQS_BITS; i++)
      {
        for(j=0; j<(4*NIBBLE_PER_DQS); j++)
        {
          if(((dq_odelay_per_bit_flag[i] & (1<<j)) != 0) && ((dq_odelay_per_byte_flag & (1<<i)) == 0))
          {
            dq_delay[(i*4*NIBBLE_PER_DQS) + j] += 1;

            DEBUG_PRINTF("INFO: get_dq_common_valid_window(): STAGE 1 dq_delay[%d] = %d\n", (i*4*NIBBLE_PER_DQS)+j, dq_delay[(i*4*NIBBLE_PER_DQS) + j]);
          }
        }
      }

      // BitWise Mask Set to enable the Movement of DQ alongwith DQS after getting the valid window for that DQ
      for(i=0; i<DQS_BITS; i++)
      {
        bcast_mask = 0;

        // Logical to physical mapping of bcast mask
        for(j=0; j<(NIBBLE_PER_DQS*4); j++)
        {
          if(((dq_odelay_per_bit_flag[i] & (1<<j)) == 0) || ((dq_odelay_per_byte_flag & (1<<i)) != 0))
          {
            if(glbl_dqs_bits_riu_map[i][0] == glbl_dq_bits_riu_map[(4*i*NIBBLE_PER_DQS) + j][0])  
              bcast_mask |= (0x1 << (glbl_dq_bits_riu_map[(4*i*NIBBLE_PER_DQS) + j][1] - 2));  
            else
              bcast_mask |= (0x1 << (4 + (glbl_dq_bits_riu_map[(4*i*NIBBLE_PER_DQS) + j][1] - 2)));  
          }
        }

        write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], IODLY_BCAST_MASK, ((bcast_mask & 0xF) << 8) | 0XFF);

        if(NIBBLE_PER_DQS == 2)
        {
          bcast_mask = (bcast_mask >> 4) & 0xF;
          write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], IODLY_BCAST_MASK, (bcast_mask << 8) | 0xFF);
        }
      }
    }
    bitslice_reset_bcast(1, BS_RESET_WT);
  }

  DEBUG_PRINTF("INFO: get_dq_common_valid_window(): EXITED FROM THE Common Valid Window : Either Common Valid For all the DQs are found or DQS reached till 90 degree offset \n");

// XRAM_DBG: wrdqdbi_stg1_dqs_delay
  for(byte=0; byte<DQS_BITS; byte++) {
    addr = (glbl_dqs_bits_riu_map[byte][0] << RIU_NIB_POS) | (glbl_dqs_bits_riu_map[byte][1] + ODLY0);
    odly = readIO_RIU(addr) & 0x1FF;
    writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG1_DQS_DELAY + (byte * 4), odly);
  }
// XRAM_DBG: wrdqdbi_stg1_byte_status
  writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG1_BYTE_STATUS, dq_odelay_per_byte_flag);
  writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG1_BYTE_STATUS + 4, (dq_odelay_per_byte_flag >> 9));
// XRAM_DBG: wrdqdbi_stg1_bit_status
  for(byte=0; byte<DQ_BITS/8; byte++) {
    if(NIBBLE_PER_DQS == 1)
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG1_BIT_STATUS, (dq_odelay_per_bit_flag[2*byte+1] << 4) | dq_odelay_per_bit_flag[2*byte]);
    else
      writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG1_BIT_STATUS, dq_odelay_per_bit_flag[byte]);
  }

  DEBUG_PRINTF("INFO: get_dq_common_valid_window(): dq_odelay_per_byte_flag = %x \n", dq_odelay_per_byte_flag);

  // INCDEC crse setting to revert the DQS delay to the initial value
  for(i=0; i<DQS_BITS; i++)
  {
    //write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], INCDEC_CRSE, glbl_max_limit_90_degree - dqs_count[i] + 1);
    write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], INCDEC_CRSE, dqs_dly_count- dqs_count[i] - 1);

    if(NIBBLE_PER_DQS == 2)
      //write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], INCDEC_CRSE, glbl_max_limit_90_degree - dqs_count[i] + 1);
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], INCDEC_CRSE, dqs_dly_count- dqs_count[i] - 1);
  }

  // Bitwise Mask Set to revert the DQ delay to the Last Stable DQS value
  for(i=0; i<DQS_BITS; i++)
  {
    bcast_mask = 0;

    // Logical to physical mapping of bcast mask
    for(j=0; j<(NIBBLE_PER_DQS*4); j++)
    {
      if(((dq_odelay_per_bit_flag[i] & (1<<j)) == 0) || ((dq_odelay_per_byte_flag & (1<<i)) != 0))
      {
        if(glbl_dqs_bits_riu_map[i][0] == glbl_dq_bits_riu_map[(4*i*NIBBLE_PER_DQS) + j][0])  
          bcast_mask |= (0x1 << (glbl_dq_bits_riu_map[(4*i*NIBBLE_PER_DQS) + j][1] - 2));  
        else
          bcast_mask |= (0x1 << (4 + (glbl_dq_bits_riu_map[(4*i*NIBBLE_PER_DQS) + j][1] - 2)));  
      }
    }

    write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], IODLY_BCAST_MASK, ((bcast_mask & 0xF) << 8) | 0XFF);

    if(NIBBLE_PER_DQS == 2)
    {
      bcast_mask = (bcast_mask >> 4) & 0xF;
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], IODLY_BCAST_MASK, (bcast_mask << 8) | 0xFF);
    }
  }
 
  // DQS Bcast to revert it to the Last DQ value
  //update_dly_mask(&dq_odelay_per_byte_flag, &nibble_mask);
  //writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dqs_nibble_mask_initial);
  riu_reg_bcast(ODLY0, 1, 0x6000); 

  // Tristate Broadcast to revert it to the LAST DQ value
  //update_nibble_dly_mask(&dq_odelay_per_byte_flag, &nibble_mask);
  //writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, nibble_mask);
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(TRISTATE_ODLY, 1, 0x6000); //Tristate Inc Dec Crse ... 15 14 13

  // DQ Bcast to revert it to the Last DQ Value
  update_nibble_dly_mask(&dq_odelay_per_byte_flag, &nibble_mask);
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
  riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000028); // Decrement By Coarse
  bitslice_reset_bcast(1, BS_RESET_WT);

  for(i=0; i<DQS_BITS; i++)
  {
    if((dq_odelay_per_byte_flag & (1<<i)) == 0)
    {
      for(j=0; j<(4*NIBBLE_PER_DQS); j++)
      {
        if((dq_odelay_per_bit_flag[i] & (1<<j)) != 0) 
          //dq_delay[(i*NIBBLE_PER_DQS*4) + j] -= (glbl_max_limit_90_degree - dqs_count[i] + 1);
          dq_delay[(i*NIBBLE_PER_DQS*4) + j] -= (dqs_dly_count- dqs_count[i] - 1);
        else
          dq_delay[(i*NIBBLE_PER_DQS*4) + j] = 0;

        DEBUG_PRINTF("INFO: get_dq_common_valid_window(): STATGE 2 dq_delay[%d] = %d\n", (i*4*NIBBLE_PER_DQS)+j, dq_delay[(i*4*NIBBLE_PER_DQS) + j]);
      }
    }
  } 

  dq_odelay_per_byte_flag = initial_flag;

// XRAM_DBG: wrdqdbi_stg2_dqs_odly
  for(byte=0; byte<DQS_BITS; byte++) {
    addr = (glbl_dqs_bits_riu_map[byte][0] << RIU_NIB_POS) | (glbl_dqs_bits_riu_map[byte][1] + ODLY0);
    odly = readIO_RIU(addr) & 0x1FF;
    writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG2_DQS_ODLY + (byte * 4), odly);
  }
// XRAM_DBG: wrdqdbi_stg2_dq_odly
  for(bit=0; bit<DQ_BITS; bit++) {
    addr = (glbl_dq_bits_riu_map[bit][0] << RIU_NIB_POS) | (glbl_dq_bits_riu_map[bit][1] + ODLY0);
    odly = readIO_RIU(addr) & 0x1FF;
    writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG2_DQ_ODLY + (bit * 4), odly);
  }

  //----------------------------------------------------------------//
  //-------Start Getting the Common Valid Window by moving DQ-------//
  //----------------------------------------------------------------//

  for(i=0; i<DQS_BITS; i++)
    for(j=0; j<(4*NIBBLE_PER_DQS); j++)
      count[i][j] = 0;

  while(dq_odelay_per_byte_flag != 0xFFFFFFFF)
  {
    simple_write_sample(first_iteration);

    for(i=0; i<DQS_BITS; i++)
    {
      if((dq_odelay_per_byte_flag & (1<<i)) == 0)
      {
        dq_compare_result = (0x0f & readIO_DMC(ral_addr_of_compare_perbit_err_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*NIBBLE_PER_DQS * i)));

        if(NIBBLE_PER_DQS == 2)
          dq_compare_result |= (readIO_DMC(ral_addr_of_compare_perbit_err_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*NIBBLE_PER_DQS * i) + 4) << 4); 

        DEBUG_PRINTF("INFO: get_dq_common_valid_window(): dq_compare_result = %x \n", dq_compare_result);

        for(j=0; j<(4*NIBBLE_PER_DQS); j++)
        {
          if((dq_odelay_per_bit_flag[i] & (1<<j)) == 0)
          {
            if((dq_compare_result & (1<<j)) == 0)
            {
              count[i][j] = count[i][j] + 1;

              if(count[i][j] >= MIN_VLD_CNT)
              {
                dq_odelay_per_bit_flag[i] = dq_odelay_per_bit_flag[i] | (1<<j);
                DEBUG_PRINTF("INFO: get_dq_common_valid_window(): dq_odelay_per_bit_flag[%d] = %x \n", i, dq_odelay_per_bit_flag[i]); 
              }
            }
            else
              count[i][j] = 0;
          }
        }

        if((dq_odelay_per_bit_flag[i] | bit_mask) == 0xFF)        
          dq_odelay_per_byte_flag |= (1<<i);
      }
    }

    DEBUG_PRINTF("INFO: get_dq_common_valid_window(): dq_odelay_per_byte_flag = %x \n", dq_odelay_per_byte_flag); 
    
    if(dq_odelay_per_byte_flag != 0xFFFFFFFF) 
    {
      dq_dly_count = dq_dly_count + 1;

      if(dq_dly_count > glbl_max_limit_180_degree) {
        DEBUG_PRINTF("ERROR: get_dq_common_valid_window(): Delay crossed more than 180 degrees\n");
        write_error_nibble_XRAM(0, ~dq_odelay_per_byte_flag);
        return 1;
      }

      for(i=0; i<DQS_BITS; i++)
      {
        for(j=0; j<(4*NIBBLE_PER_DQS); j++)
        {
          if((dq_odelay_per_bit_flag[i] & (1<<j)) == 0)
          {
            dq_delay[(i*NIBBLE_PER_DQS*4) + j] += 1;

            DEBUG_PRINTF("INFO: get_dq_common_valid_window(): STATGE 3 dq_delay[%d] = %d\n", (i*4*NIBBLE_PER_DQS)+j, dq_delay[(i*4*NIBBLE_PER_DQS) + j]);
          }
        }
      }

      // BitWise Mask Set to enable the Movement of DQ 
      for(i=0; i<DQS_BITS; i++)
      {
        bcast_mask = 0;

        // Logical to physical mapping of bcast mask
        for(j=0; j<(NIBBLE_PER_DQS*4); j++)
        {
          if(dq_odelay_per_bit_flag[i] & (1<<j))
          {
            if(glbl_dqs_bits_riu_map[i][0] == glbl_dq_bits_riu_map[(4*i*NIBBLE_PER_DQS) + j][0])  
              bcast_mask |= (0x1 << (glbl_dq_bits_riu_map[(4*i*NIBBLE_PER_DQS) + j][1] - 2));  
            else
              bcast_mask |= (0x1 << (4 + (glbl_dq_bits_riu_map[(4*i*NIBBLE_PER_DQS) + j][1] - 2)));  
          }
        }

        write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], IODLY_BCAST_MASK, ((bcast_mask & 0xF) << 8) | 0XFF);

        if(NIBBLE_PER_DQS == 2)
        {
          bcast_mask = (bcast_mask >> 4) & 0xF;
          write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], IODLY_BCAST_MASK, (bcast_mask << 8) | 0xFF);
        }
      }
  
      // BroadCast the Odelay for DQ Bits to increment by 1
      update_nibble_dly_mask(&dq_odelay_per_byte_flag, &nibble_mask);
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, nibble_mask);
      riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000018); // Step Wise Increment of DQ Delay
      bitslice_reset_bcast(1, BS_RESET_WT);
    }
  }

  return 0;
}
#endif

int get_dq_left_shift(u16 *left_margin)
{
  u8 i;

  for(i=0; i<DQS_BITS; i++)
  {
    write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], INCDEC_CRSE, left_margin[i]);

    if(NIBBLE_PER_DQS == 2)
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], INCDEC_CRSE, left_margin[i]);
  }

  writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(IODLY_BCAST_MASK, 1, 0x000000FF); // 0000_1111_1111 
  riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000028); // 0000_0010_1000 // Decrement By coarse
  
  if(glbl_wr_dbi_en)
  {
    // INC DEC mask set for DBI
    for(i=0; i<DQS_BITS; i++)
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], INCDEC_CRSE, left_margin[i]);

    // Broadcast Increment the DBI
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dbi_nibble_mask_initial);
    riu_reg_bcast(ODLY0, 1, 0x6000); // Step Wise Decrement of ODLY
  }
  bitslice_reset_bcast(1, BS_RESET_WT);

  return 0;
}

#ifdef DDR4
int get_cplx_centre (u16 *left_margin, u16 *right_margin)
{
  u8 i;
  static u16 dqs_shift[DQS_BITS];
  static u16 dq_shift[DQS_BITS];
  u16 mean_delay;

  for(i=0; i<DQS_BITS; i++)
  {
    mean_delay = (left_margin[i] + right_margin[i]) / 2 ;  

    DEBUG_PRINTF("INFO: get_cplx_centre(): left_margin[%d] = %d, right_margin[%d] = %d\n", i, left_margin[i], i, right_margin[i]);

    if(right_margin[i] > left_margin[i])
    {
      dqs_shift[i] = mean_delay;
      dq_shift[i]  = 0;  
    }
    else
    {
      dqs_shift[i] = right_margin[i];
      dq_shift[i] = (left_margin[i] - right_margin[i])/2;
    }
  } 

  // INC DEC MASK SET FOR DQS 
  for(i=0; i<DQS_BITS; i++)
  {
    write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], INCDEC_CRSE, dqs_shift[i]);
    write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], INCDEC_CRSE, dqs_shift[i]);
  }

  // Broadcast CRSE Delay For DQS
  #ifdef DDR4
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dqs_nibble_mask_initial);
    riu_reg_bcast(ODLY0, 1, 0x6000); 
  #else
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dqs_nibble_mask_initial);
    riu_reg_bcast(ODLY1, 1, 0x6000); 
  #endif

  // Broadcast CRSE Delay For Tristate
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(TRISTATE_ODLY, 1, 0x6000); //Tristate Inc Dec Crse ... 15 14 13

  // INC_DEC MASK SET FOR DQ
  for(i=0; i<DQS_BITS; i++)
  {
    write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], INCDEC_CRSE, dq_shift[i]);
    
    if(NIBBLE_PER_DQS == 2)
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], INCDEC_CRSE, dq_shift[i]);
  }

  writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(IODLY_BCAST_MASK, 1, 0x000000FF); // 0000_1111_1111 
  riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000038); // 0000_0010_1000 // Increment By coarse

  if(glbl_wr_dbi_en)
  {
    // INC DEC mask set for DBI
    for(i=0; i<DQS_BITS; i++)
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], INCDEC_CRSE, dq_shift[i]);

    // Broadcast Increment the DBI
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dbi_nibble_mask_initial);
    riu_reg_bcast(ODLY0, 1, 0xA000); // Step Wise Increment of ODLY
  } 
  bitslice_reset_bcast(1, BS_RESET_WT);

  return 0;
}

int get_noise_left_edge(u8 rank, u8 step_size, u16 *right_margin)
{
  u8 i, j;
  u8 first_iteration = 1;
  u32 nibble_mask;
  u32 dq_compare_result;
  u16 dqs_dly_count = 0;
  u32 dq_odelay_per_byte_flag = 0;

  for(i=0; i<DQS_BITS; i++)
    dq_odelay_per_byte_flag |= (1<<i);
  
  dq_odelay_per_byte_flag = ~dq_odelay_per_byte_flag;

  // Setting the Step Size into the INC DEC Register
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(INCDEC_CRSE, 1, step_size);

  while(dq_odelay_per_byte_flag != 0xFFFFFFFF)
  {
    dqs_dly_count = dqs_dly_count + step_size;
    DEBUG_PRINTF("INFO: get_noise_left_edge(): dqs_dly_count = %d\n", dqs_dly_count);

    if(dqs_dly_count > glbl_max_limit_180_degree) {
      DEBUG_PRINTF("ERROR: get_noise_left_edge(): Delay crossed more than 180 degrees\n");
      write_error_nibble_XRAM(0, ~dq_odelay_per_byte_flag);
      return 1;
    }

    // Broadcast DQS ODELAY Increment Logic For which byte flag is not set
    #ifdef DDR4
      update_dly_mask(&dq_odelay_per_byte_flag, &nibble_mask);
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, nibble_mask);
      riu_reg_bcast(ODLY0, 1, 0xA000); // Step Wise Increment of ODLY
    #else
      update_dly_mask(&dq_odelay_per_byte_flag, &nibble_mask);
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, nibble_mask);
      riu_reg_bcast(ODLY1, 1, 0xA000); // Step Wise Increment of ODLY
    #endif

    // Broadcast Tristate ODELAY Increment Logic For which byte flag is not set
    update_nibble_dly_mask(&dq_odelay_per_byte_flag, &nibble_mask);
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, nibble_mask);
    riu_reg_bcast(TRISTATE_ODLY, 1, 0xA000); //Tristate Inc Dec Crse ... 15 14 13

    cplx_write_sample (rank);

    dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK));
  
    for(i=0; i<DQS_BITS; i++)
    {
      if((dq_odelay_per_byte_flag & (1<<i)) == 0)
      {
        if(((NIBBLE_PER_DQS == 2)  && ((dq_compare_result & 0x3) != 0)) || ((NIBBLE_PER_DQS == 1)  && ((dq_compare_result & 0x1) != 0)))
        {
          dq_odelay_per_byte_flag |= (1<<i);

          right_margin[i] += dqs_dly_count - step_size;

          DEBUG_PRINTF("INFO: get_noise_left_edge(): right_margin[%d] = %d\n", i, right_margin[i]);
        }
      }

      dq_compare_result = (dq_compare_result >> NIBBLE_PER_DQS);
    }

    DEBUG_PRINTF("INFO: get_noise_left_edge(): dq_odelay_per_byte_flag = %x\n", dq_odelay_per_byte_flag);
  }
  
  // Broadcast the Odelay to decrease the DQS ODELAY by STEP_SIZE 
  #ifdef DDR4
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dqs_nibble_mask_initial);
    riu_reg_bcast(ODLY0, 1, 0x6000); // Step Wise Decrement of ODLY
  #else
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dqs_nibble_mask_initial);
    riu_reg_bcast(ODLY1, 1, 0x6000); // Step Wise Decrement of ODLY
  #endif

  // Broadcast the Odelay to decrease the Tristate ODELAY by STEP_SIZE 
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(TRISTATE_ODLY, 1, 0x6000); //Tristate Inc Dec Crse ... 15 14 13

  return 0;
}
#endif


void set_write_vref(u8 vref_value, u8 rank)
{

  DEBUG_PRINTF("INFO: set_write_vref(): vref functional value = %d \n", vref_value);

#ifdef DDR4
  if(glbl_lrdimm_en) {
     ddr_mrs7_write (rank, 0x1705); // F[7:0]BC7x Register: Function space 5
     ddr_mrs7_write_opt (0x1500 | vref_value); // F5BC5x Register: Host I/F VRef
     ddr_mrs7_write_opt (0x1700); // F[7:0]BC7x Register: Function space 0
  } else {
     DEBUG_PRINTF("INFO: set_write_vref(): Putting the memory in Vref training mode \n");
     set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/,
              MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, ((glbl_mr6 | 0x80) & 0xFFC0) | vref_value /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x2 /*u16 seq_a_ba*/,
              0 /*u32 seq_a2a_dly*/);

     DEBUG_PRINTF("INFO: set_write_vref(): Writing Default Vref Value \n");
     set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/,
              MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, ((glbl_mr6 | 0x80) & 0xFFC0) | vref_value /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x2 /*u16 seq_a_ba*/,
              0 /*u32 seq_a2a_dly*/);

     DEBUG_PRINTF("INFO: set_write_vref(): Releasing the memory from Vref training mode \n");
     set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/,
              MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, ((glbl_mr6 & 0xFF40) | vref_value) /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x2 /*u16 seq_a_ba*/,
              0 /*u32 seq_a2a_dly*/);
  }
#else
     lp4_mode_reg_wr(rank, 14, vref_value | (glbl_lp4_mr14&0x40));
#endif
 
   // Wait for vref_time long, which is 150ns for DDR4 amd 250ns for LPDDR4. // FIXME
   u8 loop;
   u8 status;
   for(loop=0; loop<20; loop++) {
      status = readIO_DMC(ral_addr_of_cal_status_in_ddrmc_main_ddrmc_main_bank(BLK));
   }
}

int get_noise_right_edge (u8 rank, u8 step_size, u8 pattern_type, u16 *valid_width)
{
  u8 i;
  u32 dq_odelay_per_byte_flag = 0;
  u16 dq_dly_count = 0;
  u32 dq_compare_result;
  u32 nibble_mask;
  u8 first_iteration = 1;

  for(i=0; i<DQS_BITS; i++)
    dq_odelay_per_byte_flag |= (1<<i);
  
  dq_odelay_per_byte_flag = ~dq_odelay_per_byte_flag;

  // Setting the Step Size into the INC DEC Register
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(INCDEC_CRSE, 1, step_size);

  while(dq_odelay_per_byte_flag != 0xFFFFFFFF)
  {
    dq_dly_count = dq_dly_count + step_size;
    DEBUG_PRINTF("INFO: get_noise_right_edge(): dq_dly_count = %d\n", dq_dly_count);

    if(dq_dly_count > glbl_max_limit_180_degree) {
      DEBUG_PRINTF("ERROR: get_noise_right_edge(): Delay crossed more than 180 degrees\n");
      write_error_nibble_XRAM(0, ~dq_odelay_per_byte_flag);
      return 1;
    }

    // BroadCast DQ ODELAY Increment Logic for which byte flag is not set
    update_nibble_dly_mask(&dq_odelay_per_byte_flag, &nibble_mask);
    writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
    riu_reg_bcast(IODLY_BCAST_MASK, 1, 0x000000ff); // 0000_1111_1111 
    riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000038); // 0000_0000_1100 // Increment By Coarse

    if(glbl_wr_dbi_en)
    {
      // Need BroadCast the Odelay for those bytes for which Byte Flag is not set
      update_byte_dly_mask (&dq_odelay_per_byte_flag, &nibble_mask);
      writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
      riu_reg_bcast(ODLY0, 1, 0xA000); // In this ODLY DBI is Mapped (Coarse Increment)
    }

    if(pattern_type == COMPLEX)
      cplx_write_sample(rank);
    else if(pattern_type == PRBS)
      prbs_write_sample();
    else
    {
      simple_write_sample(first_iteration);
      first_iteration = 0;
    }

    dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK));

    for(i=0; i<DQS_BITS; i++)
    {
      if((dq_odelay_per_byte_flag & (1<<i)) == 0)
      {
        if(((NIBBLE_PER_DQS == 2)  && ((dq_compare_result & 0x3) != 0)) || ((NIBBLE_PER_DQS == 1)  && ((dq_compare_result & 0x1) != 0)))
        {
          dq_odelay_per_byte_flag |= (1<<i);

          valid_width[i] += dq_dly_count - step_size;

          DEBUG_PRINTF("INFO: get_noise_right_edge(): valid_width[%d] = %d\n", i, valid_width[i]);
        }
      }

      dq_compare_result = (dq_compare_result >> NIBBLE_PER_DQS);
    }
    bitslice_reset_bcast(1, BS_RESET_WT);
  }

  dq_dly_count -= step_size;
  DEBUG_PRINTF("INFO: get_noise_right_edge(): final dq_dly_count = %d\n", dq_dly_count);

  // Need BroadCast the Odelay to decrease the coarsse by step_size For DQ
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(IODLY_BCAST_MASK, 1, 0x000000ff); // 0000_1111_1111 
  riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000028); // 0000_0000_1100 // Decrement by coarse

  // Need BroadCast the Odelay to decrease the coarsse by step_size For DBI/DM
  if(glbl_wr_dbi_en)
  {
    writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, ~glbl_dbi_nibble_mask_initial);
    riu_reg_bcast(ODLY0, 1, 0x6000); // In this ODLY DBI is Mapped (Coarse Decrement)
  }
  bitslice_reset_bcast(1, BS_RESET_WT);
 
  return 0;
}

u8 get_write_window (u8 rank, u8 pattern_type, u16 *left_width, u16 *right_width)
{
  u8 i, j;
  u8 status;

  if(pattern_type == SIMPLE) {
    writeIO_DMC(ral_addr_of_seq_a_dq_0_in_ddrmc_main_ddrmc_main_bank(BLK), 0xC);
    writeIO_DMC(ral_addr_of_seq_a_dq_1_in_ddrmc_main_ddrmc_main_bank(BLK), 0xD);
    writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), ((0xD << 5) | 0xC));
  }

  // Finding Left Margin
  DEBUG_PRINTF("INFO: get_write_window(): Noise Right Edge With Large Step \n"); 
  status = get_noise_right_edge (rank, FINE_CRS_STEP_SIZE, pattern_type, left_width);

  if(status) {
    if(pattern_type == PRBS) {
      write_error_XRAM(ERR_CODE_69_WR_PRBS_FAIL_LEFT_MARGIN_DQ_ODELAY_HIGHER_TAPS);
    }
    return 1;
  }

  DEBUG_PRINTF("INFO: get_write_window(): Noise Right Edge With Small Step \n"); 
  status = get_noise_right_edge (rank, 1, pattern_type, left_width);

  if(status) {
    if(pattern_type == PRBS) {
      write_error_XRAM(ERR_CODE_70_WR_PRBS_FAIL_LEFT_MARGIN_DQ_ODELAY);
    }
    return 1;
  }

  DEBUG_PRINTF("INFO: get_write_window(): Left window is computed\n");
  DEBUG_PRINTF("INFO: get_write_window(): Moving the DQ/DBI to the initial position\n");

  // Setting the INC DEC Register With left_width Value
  for(j=0; j<DQS_BITS; j++)
  {
    write_riu_nibble_reg(glbl_map_riu[(j*NIBBLE_PER_DQS)], INCDEC_CRSE, left_width[j]);

    if(NIBBLE_PER_DQS == 2)
      write_riu_nibble_reg(glbl_map_riu[(j*NIBBLE_PER_DQS) + 1], INCDEC_CRSE, left_width[j]);
  }

  // Delaying the DQ to the initial position
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(IODLY_BCAST_MASK, 1, 0x000000FF); // 0000_1111_1111 
  riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000028); // 0000_0010_1000 // Decrement By coarse

  if(glbl_wr_dbi_en)
  {
    // Broadcast Increment the DBI
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dbi_nibble_mask_initial);
    riu_reg_bcast(ODLY0, 1, 0x6000); // Step Wise Decrement of ODLY
  }
  bitslice_reset_bcast(1, BS_RESET_WT);

  // Finding Right Margin
  DEBUG_PRINTF("INFO: get_write_window(): Noise Left Edge With Large Step \n"); 
#ifdef DDR4
  status = get_right_edge(right_width, FINE_CRS_STEP_SIZE, pattern_type);
#else
  status = get_lp4_noise_left_edge(rank, FINE_CRS_STEP_SIZE, pattern_type, right_width);
#endif

  if(status) {
    if(pattern_type == PRBS) {
      #ifdef DDR4
         write_error_XRAM(ERR_CODE_71_WR_PRBS_FAIL_RIGHT_MARGIN_DQS_ODELAY_HIGHER_TAPS);
      #else
         write_error_XRAM(ERR_CODE_73_WR_PRBS_FAIL_RIGHT_MARGIN_DQ_ODELAY_HIGHER_TAPS);
      #endif
    }
    return 1; 
  }

  DEBUG_PRINTF("INFO: get_write_window(): Noise Left Edge With Small Step \n"); 
#ifdef DDR4
  status = get_right_edge(right_width, 1, pattern_type);
#else
  status = get_lp4_noise_left_edge(rank, 1, pattern_type, right_width);
#endif

  if(status) {
    if(pattern_type == PRBS) {
      #ifdef DDR4
         write_error_XRAM(ERR_CODE_72_WR_PRBS_FAIL_RIGHT_MARGIN_DQS_ODELAY);
      #else
         write_error_XRAM(ERR_CODE_74_WR_PRBS_FAIL_RIGHT_MARGIN_DQ_ODELAY);
      #endif
    }
    return 1; 
  }

  DEBUG_PRINTF("INFO: get_write_window(): Right window is computed\n");
  DEBUG_PRINTF("INFO: get_write_window(): Moving the DQS to the initial position\n");

  // Setting the INC DEC Register With right_width Value
  for(j=0; j<DQS_BITS; j++)
  {
    write_riu_nibble_reg(glbl_map_riu[(j*NIBBLE_PER_DQS)], INCDEC_CRSE, right_width[j]);

    if(NIBBLE_PER_DQS == 2)
      write_riu_nibble_reg(glbl_map_riu[(j*NIBBLE_PER_DQS) + 1], INCDEC_CRSE, right_width[j]);
  }

#ifdef DDR4
  // Delaying the DQS & Tristate to the initial position
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dqs_nibble_mask_initial);
  riu_reg_bcast(ODLY0, 1, 0x6000); // In this DQS ODLY Coarse Decrement

  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(TRISTATE_ODLY, 1, 0x6000); //Tristate Inc Dec Crse ... 15 14 13

#else
  // Delaying the DQ and DBI to the initial position
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(IODLY_BCAST_MASK, 1, 0x000000FF); // 0000_1111_1111 
  riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000038); // 0000_0010_1000 // Increment By coarse

  if(glbl_wr_dbi_en)
  {
    // Broadcast Increment the DBI
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dbi_nibble_mask_initial);
    riu_reg_bcast(ODLY0, 1, 0xA000); // Crse Wise Increment of ODLY
  }
#endif

  // Apply bit-slice reset to all the DQ nibbles
  writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
  bitslice_reset_bcast(0, BS_RESET_WT);
 
  return 0;
}

u8 intr_bs_reset(u8 channel) {
   
   if(glbl_num_ch == 0) { // Single channel
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial); 
   } else { // Dual channel
      if(channel == 0) {
         writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_ch0_nibble_mask);
      } else {
         writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_ch1_nibble_mask);
      }
   }
   riu_reg_bcast(BS_RESET_CTRL, 0, 0x1<<CLR_GATE_POS);
   bitslice_reset_bcast(0, BS_RESET_WT);

   return 0;
}

u8 intr_alert_par_err(u8 channel) {
   
   #ifdef DDR4
   u32 ipr;
   u8  rank;
   u32 seq_a_rank ;
   u32 seq_a_cntrl;
   u32 seq_a_cmd  ;
   u32 seq_a_addr ;
   u32 seq_a_ba   ;
   u32 seq_a_bg   ;
   u32 seq_a_a_dly;
   u32 seq_mode   ;
   u32 seq_type   ;
   u32 seq_cnt    ;

   if(!glbl_cal_done) {
      glbl_cal_status = (CAL_FAILED | (glbl_cal_stage[glbl_cal_index] & 0x3F));
      writeIO_XRAM((XRAM_CAL_SEQ_STATUS + 4*glbl_cal_index), glbl_cal_status);
      writeIO_XRAM(XRAM_CAL_POINTER, glbl_cal_status);
      glbl_gpo3 = (glbl_gpo3 & GPO3_CAL_STATUS_CLR) | (glbl_cal_status << GPO3_CAL_STATUS_POS) | GPO3_CAL_ERROR;
      writeIO_IOM(IO_GPO3, glbl_gpo3);
      write_error_XRAM(ERR_CODE_102_ALERT_PARITY_ERROR_DURING_COMMAND_RETRY);
      return 1;
   } else {
   // As per EDT-996152, the parity error during self refresh flows (similar to power up calibration) is reported as CAL_ERROR.
   #ifdef EDT_996152   
      // Do the retry of commands
      if(!glbl_side_cmd_en) {
         // Backup the last mem interface communication command.
         seq_a_rank  = readIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK)); // Selects xphy rank
         seq_a_cntrl = readIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK)); // Selects DDR Mem rank
         seq_a_cmd   = readIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK)); //seq_*_cmd[5*4-1:0] is composed of the CS,ACT,RAS,CAS,WE for four edges of the memory clock (per controller clock)
         seq_a_addr  = readIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK)); //seq_*_addr[8*4-1:0] is composed of the Address[17:0] for four edges of the memory clock (per controller clock)
         seq_a_ba    = readIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK)); //seq_*_ba[2*4-1:0] is composed of the BA[1:0] for four edges of the memory clock (per controller clock)
         seq_a_bg    = readIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK)); //seq_*_bg[2*4-1:0] is composed of the BG[1:0] for four edges of the memory clock (per controller clock)
         seq_a_a_dly = readIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK)); //delay in controller clock from sequence B to sequence A assertion if two sequences are enabled 
         seq_mode    = readIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK)); // Sequence mode - 0: Normal mode; 1: Write level Mode; 2: Address calibration mode;
         seq_type    = readIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK)); //number of sequences - 0: Sequence A enabled; 1: Sequence A, and B enabled; 2: Sequence A, B, and C enabled;
         seq_cnt     = readIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)); // number of iterartion of the sequences; 
      }

      // Clear the Parity error bit of the memory device through the MR write.
      for(rank = 0; rank < RANKS; rank++) {
         set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/, 
                               MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr5[rank] & MEM_CLR_PARITY_ERR /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x1 /*u16 seq_a_ba*/,
                               0 /*u32 seq_a2a_dly*/);

         if(glbl_rdimm_en && ((rank == 0) || ((SLOTS == 2) && (rank == RANKS/2)))) {
            ddr_mrs7_write (rank, 0x0060 | 0x6); // RC06
         }
      }

      // Clear compare error status                       
      writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x1);
      writeIO_DMC(ral_addr_of_compare_clear_in_ddrmc_main_ddrmc_main_bank(BLK),0x0);

      // Issue bs reset and clear gate for the data nibbles
      intr_bs_reset(0);
      intr_bs_reset(1);

      u8 gt_train = 0; 
      u8 phy_nibble = 0;
      u32 nibble_ctrl2;
      if(glbl_gt_train_en) {
         for(phy_nibble = glbl_dqs_first_nibble; phy_nibble <= glbl_dqs_last_nibble; phy_nibble++) {
            if(!((glbl_nibble_mask>>phy_nibble)&0x1)) {
               nibble_ctrl2 = read_riu_nibble_reg(phy_nibble /*u8 nibble*/, DUP_NIBBLE_CTRL2 /*u8 reg_addr*/);
               gt_train = nibble_ctrl2 & 0x40;
               if(gt_train != 0) {
                  write_riu_nibble_reg(phy_nibble /*u8 nibble*/, DUP_NIBBLE_CTRL2 /*u8 reg_addr*/, nibble_ctrl2 & 0xFFFFFFBF);
                  write_riu_nibble_reg(phy_nibble /*u8 nibble*/, RLDLYRNK0 + seq_a_rank /*u8 reg_addr*/, 0x8000);
                  write_riu_nibble_reg(phy_nibble /*u8 nibble*/, DUP_NIBBLE_CTRL2 /*u8 reg_addr*/, nibble_ctrl2 | 0x00000040);
               }
            }
         }
      }
       
      if(!glbl_side_cmd_en) {
         // Restore the last mem interface communication command.
         writeIO_DMC(ral_addr_of_seq_a_rank_in_ddrmc_main_ddrmc_main_bank(BLK) , seq_a_rank); // Selects xphy rank
         writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), seq_a_cntrl); // Selects DDR Mem rank
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK)  , seq_a_cmd); //seq_*_cmd[5*4-1:0] is composed of the CS,ACT,RAS,CAS,WE for four edges of the memory clock (per controller clock)
         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK) , seq_a_addr); //seq_*_addr[8*4-1:0] is composed of the Address[17:0] for four edges of the memory clock (per controller clock)
         writeIO_DMC(ral_addr_of_seq_a_ba_in_ddrmc_main_ddrmc_main_bank(BLK)   , seq_a_ba); //seq_*_ba[2*4-1:0] is composed of the BA[1:0] for four edges of the memory clock (per controller clock)
         writeIO_DMC(ral_addr_of_seq_a_bg_in_ddrmc_main_ddrmc_main_bank(BLK)   , seq_a_bg); //seq_*_bg[2*4-1:0] is composed of the BG[1:0] for four edges of the memory clock (per controller clock)
         writeIO_DMC(ral_addr_of_seq_a_a_dly_in_ddrmc_main_ddrmc_main_bank(BLK), seq_a_a_dly); //delay in controller clock from sequence B to sequence A assertion if two sequences are enabled 
         writeIO_DMC(ral_addr_of_seq_mode_in_ddrmc_main_ddrmc_main_bank(BLK)   , seq_mode); // Sequence mode - 0: Normal mode; 1: Write level Mode; 2: Address calibration mode;
         writeIO_DMC(ral_addr_of_seq_type_in_ddrmc_main_ddrmc_main_bank(BLK)   , seq_type); //number of sequences - 0: Sequence A enabled; 1: Sequence A, and B enabled; 2: Sequence A, B, and C enabled;
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK)    , seq_cnt); // number of iterartion of the sequences; 

         writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      } else {
         writeIO_DMC(ral_addr_of_seq_side_cmd_start_in_ddrmc_main_ddrmc_main_bank(BLK),1);
      }
      while(readIO_DMC(ral_addr_of_seq_status_in_ddrmc_main_ddrmc_main_bank(BLK)) != 0x1);
 
      // Check if the parity error has occurted again
      ipr = readIO_IOM(IO_IRQ_PENDING);
      if (ipr & (0x1 << (CH0_ALERT_PAR_ERR + IRQ_OFFSET))) { // Channel 0 Alert based on parity error
         // Clear the interrupt
         wait_intr_src_clr();
         writeIO_IOM(IO_IRQ_ACK, 0x1 << (CH0_ALERT_PAR_ERR + IRQ_OFFSET));
         glbl_post_cal_error = 1;
         glbl_post_cal_status_self_ref |= (1 << POST_CAL_STATUS_ALERT_PARITY_ERROR_CH0);
         writeIO_XRAM(XRAM_CAL_POST_STATUS_SELF_REF, glbl_post_cal_status_self_ref);
         glbl_gpo3 |= ((glbl_post_cal_status_self_ref << GPO3_POST_CAL_SELF_REF_POS) | GPO3_POST_CAL_ERROR);
         writeIO_IOM(IO_GPO3, glbl_gpo3);
         write_error_XRAM(ERR_CODE_102_ALERT_PARITY_ERROR_DURING_COMMAND_RETRY);
         return 1;
      }
      if (ipr & (0x1 << (CH1_ALERT_PAR_ERR + IRQ_OFFSET))) { // Channel 0 Alert based on parity error
         // Clear the interrupt
         wait_intr_src_clr();
         writeIO_IOM(IO_IRQ_ACK, 0x1 << (CH1_ALERT_PAR_ERR + IRQ_OFFSET));
         glbl_post_cal_error = 1;
         glbl_post_cal_status_self_ref |= (1 << POST_CAL_STATUS_ALERT_PARITY_ERROR_CH1);
         writeIO_XRAM(XRAM_CAL_POST_STATUS_SELF_REF, glbl_post_cal_status_self_ref);
         glbl_gpo3 |= ((glbl_post_cal_status_self_ref << GPO3_POST_CAL_SELF_REF_POS) | GPO3_POST_CAL_ERROR);
         writeIO_IOM(IO_GPO3, glbl_gpo3);
         write_error_XRAM(ERR_CODE_102_ALERT_PARITY_ERROR_DURING_COMMAND_RETRY);
         return 1;
      }
   #endif

      if(channel == 0) {
         glbl_post_cal_error = 1;
         glbl_post_cal_status_self_ref |= (1 << POST_CAL_STATUS_ALERT_PARITY_ERROR_CH0);
         writeIO_XRAM(XRAM_CAL_POST_STATUS_SELF_REF, glbl_post_cal_status_self_ref);
         glbl_gpo3 |= ((glbl_post_cal_status_self_ref << GPO3_POST_CAL_SELF_REF_POS) | GPO3_POST_CAL_ERROR);
         writeIO_IOM(IO_GPO3, glbl_gpo3);
         write_error_XRAM(ERR_CODE_102_ALERT_PARITY_ERROR_DURING_COMMAND_RETRY);
         return 1;
      } else {
         glbl_post_cal_error = 1;
         glbl_post_cal_status_self_ref |= (1 << POST_CAL_STATUS_ALERT_PARITY_ERROR_CH1);
         writeIO_XRAM(XRAM_CAL_POST_STATUS_SELF_REF, glbl_post_cal_status_self_ref);
         glbl_gpo3 |= ((glbl_post_cal_status_self_ref << GPO3_POST_CAL_SELF_REF_POS) | GPO3_POST_CAL_ERROR);
         writeIO_IOM(IO_GPO3, glbl_gpo3);
         write_error_XRAM(ERR_CODE_102_ALERT_PARITY_ERROR_DURING_COMMAND_RETRY);
         return 1;
      }
   }
   #endif
   return 0;
}

u8 intr_spare_0() {
   u8 i;
   u32 gpi1;

   gpi1 = readIO_IOM(IO_GPI1);

   glbl_gpo1 = (glbl_gpo1 & 0x3); // [1:0]

   for(i=2; i<11; i++) { // [10:2]
      glbl_gpo1 = glbl_gpo1 | ((((gpi1 >> (i+9)) ^ (gpi1 >> (i+10))) & 0x1) << i); // 11 ^ 12 -> 2, 12 ^ 13 -> 3,..., 19 ^ 20 -> 10
   }

   glbl_gpo1 = glbl_gpo1 | (gpi1 & 0xFFFFF800); // [31:11]

   writeIO_IOM(IO_GPO1, glbl_gpo1);

   return 0;
}

u8 intr_spare_1() {
   u32 gpi2;

   gpi2 = readIO_IOM(IO_GPI2);
   glbl_gpo2 = gpi2;
   writeIO_IOM(IO_GPO2, glbl_gpo2);

   return 0;
}

#ifdef DDR4
u8 mpr_read(u8 rank, u8 page, u8 loc) {
   // Turn on MPR mode in memory
   set_1_seq_command_at_mem(rank, 1, MEM_MODE_REG_WRITE, (glbl_mr3 | MEM_ENABLE_MPR | page), 0, 3, 0); // Setting up Page

   // MPR read from page location
   set_1_seq_command_at_mem(rank, 1, MEM_COL_READ, 0, 0, loc, 0);

   // Turn off MPR mode in memory
   set_1_seq_command_at_mem(rank, 1, MEM_MODE_REG_WRITE, (glbl_mr3 & MEM_DISABLE_MPR), 0, 3, 0);

   return 0;
}
#endif

u8 temp_control_ref_rate() {
   u8 rank=0;
   u8 status;
   u8 nibble;
   u8 dq_3_0;
   u8 mr4_rd;
   u32 rdata;
   u32 ui_data;
   u32 exp_data;
   u8 temp_sensor_rd;
   u8 ref_rate;
   u32 pb_dq_bits_31_0=0;
   u32 pb_dq_bits_63_32=0;
   u32 lb_dq_bits_31_0=0;
   u32 lb_dq_bits_63_32=0;
   u8 logical_nib;
   DEBUG_PRINTF("INFO: temp_control_ref_rate(): Checking the DRAM temperature sensor\n");
   DEBUG_PRINTF("INFO: temp_control_ref_rate(): Temperature sensor enabled = %d\n", glbl_ref_temp_sensor_en);

#ifdef DDR4
   if(glbl_ref_temp_sensor_en == 0) { // Temperature sensor is not enabled
      return 0;
   }
  // Page-2, Location-2: MR0[6], MR0[5], MR0[4], MR0[2], MR0[12], MR2[5], MR2[4], MR2[3]
   exp_data = (((glbl_mr0 >>  6) & 0x1) << 7) | (((glbl_mr0 >> 5) & 0x1) << 6) | (((glbl_mr0 >> 4) & 0x1) << 5) | (((glbl_mr0 >> 2) & 0x1) << 4) |
              (((glbl_mr0 >> 12) & 0x1) << 3) | (((glbl_mr2 >> 5) & 0x1) << 2) | (((glbl_mr2 >> 4) & 0x1) << 1) | (((glbl_mr2 >> 3) & 0x1) << 0);

   // Doing MPR read from Page-2, Location-2 for CWL/CL value confirmation
     status = mpr_read(rank, 2, 2); 
   for(u8 nib = 0; nib < DQ_BITS/4;nib++){ //1147055
     rdata = readIO_DMC(ral_addr_of_bl8_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (nib*4) );

     ui_data = (((rdata >>  0) & 0x1) << 7) | (((rdata >>  4) & 0x1) << 6) | (((rdata >>  8) & 0x1) << 5) | (((rdata >> 12) & 0x1) << 4) | 
               (((rdata >> 16) & 0x1) << 3) | (((rdata >> 20) & 0x1) << 2) | (((rdata >> 24) & 0x1) << 1) | (((rdata >> 28) & 0x1) << 0);

        
     if(ui_data != exp_data) {
        DEBUG_PRINTF("ERROR: temp_control_ref_rate(): MPR Read mismatch. Expected data = %x, Received data = %x\n", exp_data, ui_data);
        //write_error_XRAM(ERR_CODE_131_MPR_READ_ERROR);
        //return 1;
     } else {
       logical_nib = nib;
       break;
     }
     if (nib == ((DQ_BITS/4) - 1)){
        write_error_XRAM(ERR_CODE_131_MPR_READ_ERROR);
        return 1;
     }

   }
   // Page-2, Location-0: temperature sensor readout
   status = mpr_read(rank, 2, 0); 

   // Pick up from one of the component, assuming the same from all the components
   // for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++){
   // logical nibble should be derived from iob map 
   for(nibble=0; nibble<1; nibble++){ 
      rdata = readIO_DMC(ral_addr_of_bl8_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (logical_nib)*4 );
	  // rdata = readIO_DMC(ral_addr_of_bl8_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) );
      ui_data = (((rdata >>  0) & 0x1) << 7) | (((rdata >>  4) & 0x1) << 6) | (((rdata >>  8) & 0x1) << 5) | (((rdata >> 12) & 0x1) << 4) | 
                (((rdata >> 16) & 0x1) << 3) | (((rdata >> 20) & 0x1) << 2) | (((rdata >> 24) & 0x1) << 1) | (((rdata >> 28) & 0x1) << 0);

      temp_sensor_rd = (ui_data >> 3) & 0x3; // [4:3]
   }

   if(temp_sensor_rd == 0x2) // 2x Refresh speed
      glbl_ref_speed = 0x1;
   else // 1x Refresh speed
      glbl_ref_speed = 0x0;

   DEBUG_PRINTF("INFO: temp_control_ref_rate(): Temp sensor read = %x, Refresh speed = %x \n", temp_sensor_rd, glbl_ref_speed);
   writeIO_DMC(ral_addr_of_reg_ref_0_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_ref_speed);

#else
   for(rank=0; rank < RANKS; rank++) {
      lp4_mode_reg_rd(rank, 0x4); // MR4

      pb_dq_bits_31_0 = 0;
      pb_dq_bits_63_32 = 0;
      lb_dq_bits_31_0 = 0;
      lb_dq_bits_63_32 = 0;

      for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble++){
         dq_3_0 = readIO_DMC(ral_addr_of_bl8_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (nibble * 4)) & 0xF;
         if(nibble<8) {
            lb_dq_bits_31_0  = (dq_3_0<<(nibble*4)) | lb_dq_bits_31_0;
         } else {
            lb_dq_bits_63_32 = (dq_3_0<<((nibble-8)*4)) | lb_dq_bits_63_32;
         }
      }

      l2p_bit_mapper(&pb_dq_bits_31_0, &pb_dq_bits_63_32, &lb_dq_bits_31_0, &lb_dq_bits_63_32);

      // Capture from all the components & ranks, channel wise (x16)
      for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble+=4) { // Every x16
         if(nibble<8)
            mr4_rd = (pb_dq_bits_31_0>>(nibble*4)) & 0xFF;
         else
            mr4_rd = (pb_dq_bits_63_32>>((nibble-8)*4)) & 0xFF;

         DEBUG_PRINTF("INFO: temp_control_ref_rate(): rank = %d, nibble = %d, MR4 = %x\n", rank, nibble, mr4_rd);
         if(mr4_rd & 0x80) { // TUF = 1
            glbl_ref_rate[rank][nibble/4] = mr4_rd & 0x7;
         }
      }
   }

   // Pick up the higgest refresh rate
   ref_rate = 0x3; // 1x refresh: default
   for(rank=0; rank < RANKS; rank++) {
      for(nibble=0; nibble<DQS_BITS*NIBBLE_PER_DQS; nibble+=4) { // Every x16
         if(glbl_ref_rate[rank][nibble/4] > ref_rate) { // Pick up if the refresh rate is more
            ref_rate = glbl_ref_rate[rank][nibble/4];
         }
      }
   }

   if((ref_rate == 0x6) || (ref_rate == 0x5)) // 4x Refresh speed (0.25x time)
      glbl_ref_speed = 0x2;
   else if(ref_rate == 0x4) // 2x Refresh speed (0.5x time)
      glbl_ref_speed = 0x1;
   else // 1x Refresh speed (1x time)
      glbl_ref_speed = 0x0;

   DEBUG_PRINTF("INFO: temp_control_ref_rate(): Temp sensor read = %x, Refresh speed = %x \n", ref_rate, glbl_ref_speed);
   writeIO_DMC(ral_addr_of_reg_ref_0_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_ref_speed);
#endif

   return 0;
}

u8 intr_spare_2() {
   u32 gpi4;

   gpi4 = readIO_IOM(IO_GPI4);

   glbl_gpo4 = (glbl_gpo4 & 0xC7FF03FF); // [31:30], [26:16], [9:0]

   glbl_gpo4 = glbl_gpo4 | (((gpi4 >> 25) & 0x1) << 10); // 25 -> 10
   glbl_gpo4 = glbl_gpo4 | (((gpi4 >> 26) & 0x1) << 11); // 26 -> 11
   glbl_gpo4 = glbl_gpo4 | (((gpi4 >> 27) & 0x1) << 12); // 27 -> 12
   glbl_gpo4 = glbl_gpo4 | (((gpi4 >> 28) & 0x1) << 13); // 28 -> 13
   glbl_gpo4 = glbl_gpo4 | (((gpi4 >> 29) & 0x1) << 14); // 29 -> 14
   glbl_gpo4 = glbl_gpo4 | (((gpi4 >> 30) & 0x1) << 15); // 30 -> 15

   glbl_gpo4 = glbl_gpo4 | (((gpi4 >> 31) & 0x1) << 27); // 31 -> 27

   glbl_gpo4 = glbl_gpo4 | ((((gpi4 >> 25) ^ (gpi4 >> 26)) & 0x1) << 28); // 25 ^ 26 -> 28
   glbl_gpo4 = glbl_gpo4 | ((((gpi4 >> 26) ^ (gpi4 >> 27)) & 0x1) << 29); // 26 ^ 27 -> 29

   writeIO_IOM(IO_GPO4, glbl_gpo4);

   return 0;
}

void set_mgchk_vref(int vref, int margin){ 
      if (margin == READ_MARGIN) {
         writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial); 
         riu_reg_bcast(VREF_CTRL, 0, (vref | 0x400)); // 10th bit sets the fab2iob_r2rdac_mode, [9:0] is fab2iob_rwrdac_sel
     	 glbl_ddrmc_clk_cnt = (2000000 / (glbl_tck * 2)); //  2us
     	 writeIO_DMC(ral_addr_of_ddrmc_clk_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), ((0x1 << 31) | glbl_ddrmc_clk_cnt));
     	 while (glbl_ddrmc_clk_cnt != 0) {
     	    glbl_ddrmc_clk_cnt = (readIO_DMC(ral_addr_of_ddrmc_clk_cnt_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & ~(0x1 << 31)); // Exclude busy bit
     	 }
      } else {
      	set_write_vref(vref , glbl_margin_rank);
      }
}

void clear_margin(int margin) {

      for (int j = 0 ;j< 18;j++) {
        if (margin == WRITE_MARGIN) {
            writeIO_XRAM(XRAM_WRMARGIN_LEFT + j*4, 0);
            writeIO_XRAM(XRAM_WRMARGIN_RIGHT + j*4, 0);
        } else {
            writeIO_XRAM(XRAM_RDMARGIN_PQTR_LEFT + j*4, 0);
            writeIO_XRAM(XRAM_RDMARGIN_PQTR_RIGHT + j*4, 0);
            writeIO_XRAM(XRAM_RDMARGIN_NQTR_LEFT + j*4, 0);
            writeIO_XRAM(XRAM_RDMARGIN_NQTR_RIGHT + j*4, 0);
        }
      }

}

void set_mgchk_def_vref () {
      u16 def_vref;
      u8  nibble;
      def_vref = (readIO_XRAM(XRAM_MGCHK_DEF_RD_VREF_1) & 0x1) << 9 |   readIO_XRAM(XRAM_MGCHK_DEF_RD_VREF_0) ; 
      writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial); 
      riu_reg_bcast(VREF_CTRL, 0, (def_vref | 0x400)); // 10th bit sets the fab2iob_r2rdac_mode, [9:0] is fab2iob_rwrdac_sel
      glbl_ddrmc_clk_cnt = (2000000 / (glbl_tck * 2)); //  2us
      writeIO_DMC(ral_addr_of_ddrmc_clk_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), ((0x1 << 31) | glbl_ddrmc_clk_cnt));
      while (glbl_ddrmc_clk_cnt != 0) {
         glbl_ddrmc_clk_cnt = (readIO_DMC(ral_addr_of_ddrmc_clk_cnt_status_in_ddrmc_main_ddrmc_main_bank(BLK)) & ~(0x1 << 31)); // Exclude busy bit
      }
      def_vref = readIO_XRAM(XRAM_MGCHK_DEF_WR_VREF) ;
      for (int rank = 0; rank < RANKS;rank++){ 
        set_write_vref(def_vref,rank);
      }
}


void set_initial_vref(){
   u16 vref_initial = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[0],VREF_CTRL) & 0x3FF;
   writeIO_XRAM(XRAM_MGCHK_RD_VREF_1, (vref_initial >> 9 & 0x1));
   writeIO_XRAM(XRAM_MGCHK_RD_VREF_0, (vref_initial & 0x3FF));
#ifdef DDR4
   writeIO_XRAM(XRAM_MGCHK_WR_VREF,glbl_mr6 );
#else
   writeIO_XRAM(XRAM_MGCHK_WR_VREF,glbl_lp4_mr14 );
#endif
}

void read_def_vref(){
   u16 vref_initial = read_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[0],VREF_CTRL) & 0x3FF;
   writeIO_XRAM(XRAM_MGCHK_DEF_RD_VREF_1, (vref_initial >> 9 & 0x1));
   writeIO_XRAM(XRAM_MGCHK_DEF_RD_VREF_0, (vref_initial & 0x3FF));
#ifdef DDR4
   writeIO_XRAM(XRAM_MGCHK_DEF_WR_VREF,glbl_mr6 );
#else
   writeIO_XRAM(XRAM_MGCHK_DEF_WR_VREF,glbl_lp4_mr14 );
#endif
}

u8 post_cal () {
   u8 nibble, phy_nibble, rank;
   u8 status;
   u16 bs_reset_ctrl;
   u16 cur_rldlyqtr;
   u16 read_dqs_track_en;
   u16 lp4_osc_track_en_ch0;
   u16 lp4_osc_track_en_ch1;
   static u16 rldlyqtr_max[DQ_NIBBLES];
   static u16 rldlyqtr_min[DQ_NIBBLES];
   u16 cur_rldlyrnk;
   u8 cur_rldlyrnk_crse;
   u16 cur_rldlyrnk_fine;
   static u16 rldlyrnk_crse_max[DQS_BITS];
   static u16 rldlyrnk_fine_max[DQS_BITS];
   static u16 rldlyrnk_crse_min[DQS_BITS];
   static u16 rldlyrnk_fine_min[DQS_BITS];
   u8 test_mode;
   u16 user_vref;

#ifdef FIX_EDT_999807
   status = en_mission_mode_tracking();
   if(status) {
      return 1;
   }
#endif
   // Lock NPI register write access
   writeIO_DMC(ral_addr_of_pcsr_lock_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_pcsr_lock ? 0x0 : 0xf9e8d7c6));

   read_dqs_track_en = ~(read_riu_nibble_reg(glbl_map_riu[0], DUP_NIBBLE_CTRL2) >> DIS_DQS_TRACK_POS) & 0x1;
   glbl_post_cal_status_tracking &= ~(1 << POST_CAL_STATUS_DQS_GATE_TRACK_RUNNING);
   glbl_post_cal_status_tracking |= (read_dqs_track_en << POST_CAL_STATUS_DQS_GATE_TRACK_RUNNING);

#ifdef LPDDR4
   lp4_osc_track_en_ch0 = (readIO_DMC(ral_addr_of_reg_mrs_2_in_ddrmc_main_ddrmc_main_bank(BLK)) >> 0) & 0x1;
   lp4_osc_track_en_ch1 = (readIO_DMC(ral_addr_of_reg_mrs_2_in_ddrmc_main_ddrmc_main_bank(BLK)) >> 1) & 0x1;

   glbl_post_cal_status_tracking &= ~(1 << POST_CAL_STATUS_LP4_OSC_TRACK_RUNNING_CH0);
   glbl_post_cal_status_tracking |= (lp4_osc_track_en_ch0 << POST_CAL_STATUS_LP4_OSC_TRACK_RUNNING_CH0);

   glbl_post_cal_status_tracking &= ~(1 << POST_CAL_STATUS_LP4_OSC_TRACK_RUNNING_CH1);
   glbl_post_cal_status_tracking |= (lp4_osc_track_en_ch1 << POST_CAL_STATUS_LP4_OSC_TRACK_RUNNING_CH1);
#endif

   writeIO_XRAM(XRAM_CAL_POST_STATUS_TRACKING, glbl_post_cal_status_tracking);

   glbl_gpo3 &= ~(0xFF << GPO3_POST_CAL_TRACKING_POS);
   glbl_gpo3 |= (glbl_post_cal_status_tracking << GPO3_POST_CAL_TRACKING_POS);
   writeIO_IOM(IO_GPO3, glbl_gpo3);

   for(nibble=0; nibble<DQ_NIBBLES; nibble++) {
      rldlyqtr_max[nibble] = 0;
      rldlyqtr_min[nibble] = 511;
   }

   for(nibble=0; nibble<DQS_BITS; nibble++) {
      rldlyrnk_crse_max[nibble] = 0;
      rldlyrnk_fine_max[nibble] = 0;
      rldlyrnk_crse_min[nibble] = 0xF;
      rldlyrnk_fine_min[nibble] = 0x1FF;
   }

#ifdef LPDDR4
   for(rank=0; rank<2; rank++) {
      for(nibble=0; nibble<4; nibble++) {
         glbl_tdqs2dq_max[rank][nibble] = 0x0;
         glbl_tdqs2dq_min[rank][nibble] = 0x3FF;
      }
      for(nibble=0; nibble<16; nibble++) {
         glbl_dqsosc_wldlyrnk_fine_max[rank][nibble] = 0x0;
         glbl_dqsosc_wldlyrnk_fine_min[rank][nibble] = 0x3FF;
      }
   }
#endif

   while (glbl_post_cal_error == 0) { // Indefinite post calibration loop until there is no ERROR
      glbl_heartbeat = (glbl_heartbeat == 0) ? 1 : 0;
      glbl_gpo3 = glbl_heartbeat ? (glbl_gpo3 | (1 << GPO3_HEARTBEAT_POS)) : (glbl_gpo3 & ~(1 << GPO3_HEARTBEAT_POS));
      writeIO_IOM(IO_GPO3, glbl_gpo3);

      if(glbl_bisc_en) { // BISC is enabled
         DEBUG_PRINTF("INFO: post_cal(): Track the RL_DLY_QTR value over VT tracking\n");
         for(nibble=0; nibble<DQ_NIBBLES; nibble++) { // VT Tracking of RL_DLY_QTR
            phy_nibble = glbl_map_riu[nibble];
            // cur_rldlyqtr = read_riu_nibble_reg(phy_nibble, RL_DLY_QTR);
            write_riu_nibble_reg(phy_nibble, RD_DBG_RW_INDEX, 0xC); // Select HALFT_DQSM register in the Debug bus
            cur_rldlyqtr = (read_riu_nibble_reg(phy_nibble, RD_DBG_RD_STATUS) >> 1); // QTR = HALF/2
            writeIO_XRAM(XRAM_VTTRACK_RLDLYQTR + nibble*4, cur_rldlyqtr);

            if(cur_rldlyqtr > rldlyqtr_max[nibble]) {
	       rldlyqtr_max[nibble] = cur_rldlyqtr;
               writeIO_XRAM(XRAM_VTTRACK_RLDLYQTR_MAX + nibble*4, cur_rldlyqtr);
	    }

            if(cur_rldlyqtr < rldlyqtr_min[nibble]) {
	       rldlyqtr_min[nibble] = cur_rldlyqtr;
               writeIO_XRAM(XRAM_VTTRACK_RLDLYQTR_MIN + nibble*4, cur_rldlyqtr);
	    }
         }

         DEBUG_PRINTF("INFO: post_cal(): Track the RL_DLY_RNK0 value over VT tracking\n");
         for(nibble=0; nibble<DQS_BITS; nibble++) {
            phy_nibble = glbl_dqs_bits_riu_map[nibble][0];
            cur_rldlyrnk = read_riu_nibble_reg(phy_nibble, RLDLYRNK0);
            cur_rldlyrnk_crse = (cur_rldlyrnk >> 9) & 0xF;
            cur_rldlyrnk_fine = cur_rldlyrnk & 0x1FF;
            writeIO_XRAM(XRAM_DQSTRACK_RLDLYRNK_CRSE + nibble*4, cur_rldlyrnk_crse);
            writeIO_XRAM(XRAM_DQSTRACK_RLDLYRNK_FINE + nibble*4, cur_rldlyrnk_fine);

	    if((cur_rldlyrnk_crse > rldlyrnk_crse_max[nibble]) ||
	       ((cur_rldlyrnk_crse == rldlyrnk_crse_max[nibble]) && (cur_rldlyrnk_fine > rldlyrnk_fine_max[nibble]))) {
	       rldlyrnk_crse_max[nibble] = cur_rldlyrnk_crse;
	       rldlyrnk_fine_max[nibble] = cur_rldlyrnk_fine;
               writeIO_XRAM(XRAM_DQSTRACK_RLDLYRNK_CRSE_MAX + nibble*4, cur_rldlyrnk_crse);
               writeIO_XRAM(XRAM_DQSTRACK_RLDLYRNK_FINE_MAX + nibble*4, cur_rldlyrnk_fine);
	    }

	    if((cur_rldlyrnk_crse < rldlyrnk_crse_min[nibble]) ||
	       ((cur_rldlyrnk_crse == rldlyrnk_crse_min[nibble]) && (cur_rldlyrnk_fine > rldlyrnk_fine_min[nibble]))) {
	       rldlyrnk_crse_min[nibble] = cur_rldlyrnk_crse;
	       rldlyrnk_fine_min[nibble] = cur_rldlyrnk_fine;
               writeIO_XRAM(XRAM_DQSTRACK_RLDLYRNK_CRSE_MIN + nibble*4, cur_rldlyrnk_crse);
               writeIO_XRAM(XRAM_DQSTRACK_RLDLYRNK_FINE_MIN + nibble*4, cur_rldlyrnk_fine);
	    }
         }

         DEBUG_PRINTF("INFO: post_cal(): Check for underflow status\n");
         glbl_error_phy_nibble = 0;
         for(nibble=0; nibble<DQS_BITS; nibble++) {
            phy_nibble = glbl_dqs_bits_riu_map[nibble][0];
            bs_reset_ctrl = read_riu_nibble_reg(phy_nibble, BS_RESET_CTRL);
            if((bs_reset_ctrl >> COARSE_UNDERFLOW_POS) & 0x1) {
               DEBUG_PRINTF("ERROR: post_cal(): coarse underflow for nibble = %d\n", nibble);
               glbl_error_phy_nibble |= (0x1 << phy_nibble);
            }
         }
         if(glbl_error_phy_nibble != 0) {
            write_error_nibble_XRAM(1, glbl_error_phy_nibble);
            glbl_post_cal_error = 1;
            glbl_post_cal_status_tracking |= (1 << POST_CAL_STATUS_DQS_GATE_TRACK_UNDERFLOW);
            writeIO_XRAM(XRAM_CAL_POST_STATUS_TRACKING, glbl_post_cal_status_tracking);
            glbl_gpo3 |= ((glbl_post_cal_status_tracking << GPO3_POST_CAL_TRACKING_POS) | GPO3_POST_CAL_ERROR);
            writeIO_IOM(IO_GPO3, glbl_gpo3);
            write_error_XRAM(ERR_CODE_85_DQS_GATE_TRACKING_RLDLYRNK_COARSE_UNDERFLOW);
            return 1;
         }

         DEBUG_PRINTF("INFO: post_cal(): Check for overflow status\n");
         glbl_error_phy_nibble = 0;
         for(nibble=0; nibble<DQS_BITS; nibble++) {
            phy_nibble = glbl_dqs_bits_riu_map[nibble][0];
            bs_reset_ctrl = read_riu_nibble_reg(phy_nibble, BS_RESET_CTRL);
            if((bs_reset_ctrl >> COARSE_OVERFLOW_POS) & 0x1) {
               DEBUG_PRINTF("ERROR: post_cal(): coarse overflow for nibble = %d\n", nibble);
               glbl_error_phy_nibble |= (0x1 << phy_nibble);
            }
         }
         if(glbl_error_phy_nibble != 0) {
            write_error_nibble_XRAM(1, glbl_error_phy_nibble);
            glbl_post_cal_error = 1;
            glbl_post_cal_status_tracking |= (1 << POST_CAL_STATUS_DQS_GATE_TRACK_OVERFLOW);
            writeIO_XRAM(XRAM_CAL_POST_STATUS_TRACKING, glbl_post_cal_status_tracking);
            glbl_gpo3 |= ((glbl_post_cal_status_tracking << GPO3_POST_CAL_TRACKING_POS) | GPO3_POST_CAL_ERROR);
            writeIO_IOM(IO_GPO3, glbl_gpo3);
            write_error_XRAM(ERR_CODE_86_DQS_GATE_TRACKING_RLDLYRNK_COARSE_OVERFLOW);
            return 1;
         }
      } else { // BISC is disabled
         wait_idle_heartbeat();
      }
      #ifdef ENABLE_MICROBLAZE_BFM
        if(gpio_interrupt_handler()) glbl_post_cal_error = 1; 
      #endif
      //#ifndef ENABLE_MICROBLAZE_BFM
#ifndef VNC_DIS_INTR
      if(glbl_ipr) {
         if(post_gpio_interrupt_handler ())
            glbl_post_cal_error = 1;
      }
#endif // VNC_DIS_INTR
      //#endif
     //Margin check routine
	
       if ((readIO_XRAM(XRAM_MGCHK_CTRL) >> MGCHK_ENABLE) & 0x01 == 1) {
         read_def_vref(); 
         glbl_pcsr_lock = readIO_DMC(ral_addr_of_pcsr_lock_in_ddrmc_main_ddrmc_main_bank(BLK)); // Record the PCSR lock status
         writeIO_DMC(ral_addr_of_pcsr_lock_in_ddrmc_main_ddrmc_main_bank(BLK), 0xf9e8d7c6);
         status = dis_mission_mode_tracking();
         if(status) { 
           writeIO_XRAM(XRAM_MGCHK_CTRL,(1 << MGCHK_ERROR)|(1 << MGCHK_ENABLE)|(1 << MGCHK_STATUS)); 
           glbl_margin_check = 0;
         } else {
            writeIO_DMC(ral_addr_of_cal_status_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1);
            glbl_margin_check = 1;
            writeIO_XRAM(XRAM_MGCHK_CTRL,(1 << MGCHK_STATUS)|(1 << MGCHK_ENABLE));
         }
       }
           while (glbl_margin_check) {
       	     if (((readIO_XRAM(XRAM_MGCHK_CTRL) >> MGCHK_ENABLE) & 0x01) != 1) { //margin check disable 
               writeIO_DMC(ral_addr_of_cplx_pattern1_in_ddrmc_main_ddrmc_main_bank(BLK), 0xaaaa );
               set_mgchk_def_vref();
               writeIO_DMC(ral_addr_of_cal_status_in_ddrmc_main_ddrmc_main_bank(BLK), 0x3);
               status = en_mission_mode_tracking();
               if(status) 
                  writeIO_XRAM(XRAM_MGCHK_CTRL,(1 << MGCHK_ERROR)|(1 << MGCHK_ACK)|(1 << MGCHK_STATUS)); 
               else 
                 writeIO_XRAM(XRAM_MGCHK_CTRL,0);
               glbl_margin_check = 0;
               writeIO_DMC(ral_addr_of_pcsr_lock_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_pcsr_lock ? 0x0 : 0xf9e8d7c6));
       	     } 
             if ((readIO_XRAM(XRAM_MGCHK_CTRL) >> MGCHK_START) & 0x01 == 1) { //check margin
               writeIO_DMC(ral_addr_of_cplx_pattern1_in_ddrmc_main_ddrmc_main_bank(BLK), 0x5555 );
               //write_error_XRAM(0);
               writeIO_XRAM(XRAM_MGCHK_CTRL,(1 << MGCHK_ENABLE)|(1 << MGCHK_ACK)|(1 << MGCHK_STATUS));
               test_mode = readIO_XRAM(XRAM_MGCHK_TEST_MODE);
               glbl_margin_pat = test_mode & 0x01;
               glbl_margin_rank = readIO_XRAM(XRAM_MGCHK_RANK) & 0x3 ;
               glbl_margin_sample_count = ((readIO_XRAM(XRAM_MGCHK_PAT_CONFIG_1) & 0x7f) << 9);
               glbl_margin_loop = readIO_XRAM(XRAM_MGCHK_PAT_CONFIG_0) & 0xff ;
               glbl_margin_pat_len = readIO_XRAM(XRAM_MGCHK_PAT_CONFIG_1) & 0x3f;
               glbl_margin_bl = readIO_XRAM(XRAM_MGCHK_PAT_CONFIG_2) & 0x1f ;
               glbl_margin_step = readIO_XRAM(XRAM_MGCHK_STEP);
               if (glbl_margin_step == 0) {glbl_margin_step = 1;}
               if (glbl_margin_step > 50) {glbl_margin_step = 50;}
               if (glbl_margin_sample_count == 0) {glbl_margin_sample_count = 1;}
               if (glbl_margin_loop < 3) {glbl_margin_loop = 3;}
               if (glbl_margin_pat_len < 24) {glbl_margin_pat_len = 23;}
               if (glbl_margin_bl < 10) {glbl_margin_bl = 10;}
               if (glbl_margin_rank > (RANKS - 1))  {
                  write_error_XRAM(ERR_CODE_130_2D_MARGIN_CHECK_INVALID_RANK_ERROR);
                  clear_margin(READ_MARGIN);
                  clear_margin(WRITE_MARGIN);
                  writeIO_XRAM(XRAM_MGCHK_CTRL,(1 << MGCHK_ENABLE)|(1 << MGCHK_ACK)| (1 << MGCHK_DONE) | (1 << MGCHK_ERROR) |(1 << MGCHK_STATUS));
               } else {
                  if ((test_mode >> 1) == READ_MARGIN) {  //READ MARGIN
                    clear_margin(READ_MARGIN);
                    user_vref = (readIO_XRAM(XRAM_MGCHK_RD_VREF_1) & 0x1) << 9 |   readIO_XRAM(XRAM_MGCHK_RD_VREF_0) ; 
                    set_mgchk_vref(user_vref, READ_MARGIN);
		    //disabling read dbi
		    read_dbi_settings(0);
                    status = ddr_read_mgchk(glbl_margin_pat);
		    read_dbi_settings(1);
		    //enable read dbi

                    if (status){ 
                       //write_error_XRAM(ERR_CODE_128_2D_MARGIN_CHECK_READ_ERROR);
                       //clear_margin(READ_MARGIN);
                    }
                    precharge_all(glbl_margin_rank);
                 } else {  //WRITE MARGIN
                    clear_margin(WRITE_MARGIN);
                    user_vref = readIO_XRAM(XRAM_MGCHK_WR_VREF) ; 
                    set_mgchk_vref(user_vref, WRITE_MARGIN);
                    status = ddr_write_mgchk(glbl_margin_pat); 
                    if (status) {
                       //write_error_XRAM(ERR_CODE_129_2D_MARGIN_CHECK_WRITE_ERROR);
                       //clear_margin(WRITE_MARGIN);
                    }
                    precharge_all(glbl_margin_rank);
                 }
              writeIO_XRAM(XRAM_MGCHK_CTRL,(1 << MGCHK_ENABLE)|(1 << MGCHK_ACK)| (1 << MGCHK_DONE) | (1 << MGCHK_STATUS));
              }
           } 
          }
   }

   DEBUG_PRINTF("ERROR: post_cal(): Post calibration error\n");
   return glbl_post_cal_error;
}

void read_dbi_settings (u8 set) {
   if (set == 1) {
       if((F0_MODE_READ_DBI_CAL==CAL_EN) || (F1_MODE_READ_DBI_CAL==CAL_EN))
            {
               DEBUG_PRINTF("INFO: rd_dbi_cal(): Enable read DBI mode in DRAM\n");
               writeIO_DMC(ral_addr_of_xpi_read_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_xpi_read_dbi);
               #ifdef LPDDR4
                  for (u8 rank = 0; rank < RANKS; rank++) {
                     lp4_mode_reg_wr(rank, 3, glbl_lp4_mr3);
                  }
                  u32 temp_nibble;
                  for(u8 nibble = 0; nibble<DQ_NIBBLES;nibble++) { //glbl_dqs_first_nibble; nibble <= glbl_dqs_last_nibble ; nibble++)
                     u32 read_latency      = readIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK)+glbl_dq_lnibble2pnibble_map[nibble]*4);
                     writeIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK) + glbl_dq_lnibble2pnibble_map[nibble]*4, read_latency+glbl_mem_read_latency_dbi_offset);
                  }
                  u32 fifo_rd_latency = readIO_DMC(ral_addr_of_fifo_rden_in_ddrmc_main_ddrmc_main_bank(BLK));
                  writeIO_DMC(ral_addr_of_fifo_rden_in_ddrmc_main_ddrmc_main_bank(BLK),fifo_rd_latency+glbl_mem_read_latency_dbi_offset/2);
               #else
                  for (u8 rank = 0; rank < RANKS; rank += glbl_rank_inc) {
                  // Write MPR register to Enable the Read DBI mode into the DRAM.
                  set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/,
                                   MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr5[0] | 0x1000 /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x1 /*u16 seq_a_ba*/,
                                   0x30 /*u32 seq_a2a_dly*/);
                  }
               #endif
            }

    } else {
        if((F0_MODE_READ_DBI_CAL==CAL_EN) || (F1_MODE_READ_DBI_CAL==CAL_EN)) 
	     {
	        DEBUG_PRINTF("INFO: rd_dbi_cal(): Disable read DBI mode in DRAM\n");
                writeIO_DMC(ral_addr_of_xpi_read_dbi_in_ddrmc_main_ddrmc_main_bank(BLK), glbl_xpi_read_dbi&0xFE);
                #ifdef LPDDR4
                   for (u8 rank = 0; rank < RANKS; rank++) {
                      lp4_mode_reg_wr(rank, 3, glbl_lp4_mr3 & (~(0x1<<6)));
                   }
        	       u8 temp_nibble;
        	       for(u8 nibble = 0; nibble<DQ_NIBBLES;nibble++) { //glbl_dqs_first_nibble; nibble <= glbl_dqs_last_nibble ; nibble++)
        	          u32 read_latency      = readIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK)+glbl_dq_lnibble2pnibble_map[nibble]*4);
        	          writeIO_DMC(ral_addr_of_phy_rden0_in_ddrmc_main_ddrmc_main_bank(BLK) + glbl_dq_lnibble2pnibble_map[nibble]*4, read_latency-glbl_mem_read_latency_dbi_offset);
        	       }
        	       u32 fifo_rd_latency = readIO_DMC(ral_addr_of_fifo_rden_in_ddrmc_main_ddrmc_main_bank(BLK));
        	       writeIO_DMC(ral_addr_of_fifo_rden_in_ddrmc_main_ddrmc_main_bank(BLK),fifo_rd_latency-glbl_mem_read_latency_dbi_offset/2);
        	    #else
        	       for (u8 rank = 0; rank < RANKS; rank += glbl_rank_inc) {
        	       // Write MPR register to Enable the Read DBI mode into the DRAM.
        	              set_1_seq_command_at_mem(rank /*u8 rank*/, 1 /*u32 nos_iteration_of_seqs*/,
        	                                       MEM_MODE_REG_WRITE /*u32 seq_a_cmd*/, glbl_mr5[0]&0xEFFF /*u32 seq_a_addr*/, 0x1 /*u8 seq_a_bg*/, 0x1 /*u16 seq_a_ba*/,
        	                                       0x30 /*u32 seq_a2a_dly*/);
        	       }
        	    #endif
	      }
	    
       }
}

void wait_idle_heartbeat () {
   u32 wait_time;
   u32 WAIT_IDLE = (1 << 16);
   // Reduce the heartbeat frequency during idle time, to avoid any race condition between Microblaze and Hardware
   for(wait_time=0; wait_time < WAIT_IDLE; wait_time++) {
      // Do nothing, just wait time
   }
   return;
}

u32 get_cal_stage_time () {
  u32 cal_stage_time;

  glbl_pit1_cnt_prev = glbl_pit1_cnt;
  glbl_pit1_cnt = readIO_IOM(IO_PIT1_COUNTER);
  
  cal_stage_time = ((glbl_pit1_cnt_prev - glbl_pit1_cnt) * 2) / (10 * NOC_CLK_PERIOD); // Convert in terms of 10us

  if(cal_stage_time >= 0x1F0) // Saturate the value, if it goes beyond 0x1F0
     cal_stage_time = 0x1FF;

  return cal_stage_time;
}

void wait_intr_src_clr () {
   u8 wait_time;
   u8 status;
   for(wait_time=0; wait_time < 10; wait_time++) { // To avoid the race between the clearing interrupt source and clearing interrupt in microblaze
      status = readIO_IOM(IO_IRQ_PENDING);
   }
   return;
}

#ifndef VNC_DIS_INTR
#ifdef ENABLE_MICROBLAZE_BFM
u32 gpio_interrupt_handler () {
#else
u32 gpio_interrupt_handler (XIOModule *IOModule) {
#endif
   u32 ipr;
   u8 status = 0;
   u32 set_done = 0;
   u8 intr_done = 0;

   // Enable NPI register write access
   glbl_pcsr_lock = readIO_DMC(ral_addr_of_pcsr_lock_in_ddrmc_main_ddrmc_main_bank(BLK)); // Record the PCSR lock status
   writeIO_DMC(ral_addr_of_pcsr_lock_in_ddrmc_main_ddrmc_main_bank(BLK), 0xf9e8d7c6);

   // Read the 32 bit Interrupt status register
   ipr = readIO_IOM(IO_IRQ_PENDING);
   glbl_ipr = ipr;

   #ifdef ENABLE_MICROBLAZE_BFM
     if(!glbl_ipr) return status; //For BFM case, returning with status 0 when there are no interrupts set.
   #endif
   // Disable all the interrupts, as the subroutines are outside the interrupt handler
   writeIO_IOM(IO_IRQ_ENABLE, 0x0);

   // Interrupt decoding
   if(ipr & (0x1 << (CH0_BS_RST_REQ + IRQ_OFFSET))) { // Channel 0 BS Reset
      // Acknowledgement the DMC
      glbl_gpo4 = glbl_gpo4 | (0x1 << CH0_BS_RST_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);
      wait_intr_src_clr(); // Stretch for sampling by dmc_clk
      glbl_gpo4 = glbl_gpo4 & ~(0x1 << CH0_BS_RST_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);

      // Clear the interrupt
      wait_intr_src_clr();
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (CH0_BS_RST_REQ + IRQ_OFFSET));
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (CH0_ALERT_PAR_ERR + IRQ_OFFSET)); // Clear the alert interrupt in interrupt status register (ISR)

      // Call the service routine
      status = intr_bs_reset(0);
 
      if(status != 0) {
         DEBUG_PRINTF("ERROR: CALIBRATION_MODULE: gpio_interrupt_handler(): Interrupt subroutine FAILED\n");
         glbl_post_cal_error = 1;
         glbl_post_cal_status_tracking |= (1 << CH0_BS_RST_REQ);
         writeIO_XRAM(XRAM_CAL_POST_STATUS_TRACKING, glbl_post_cal_status_tracking);
         glbl_gpo3 |= ((glbl_post_cal_status_tracking << GPO3_POST_CAL_TRACKING_POS) | GPO3_POST_CAL_ERROR);
         writeIO_IOM(IO_GPO3, glbl_gpo3);
      } else {
         // Indicate Done
         set_done = (0x1 << CH0_BS_RST_DONE);
      }
      intr_done = 1;
      glbl_ipr &= ~(0x1 << (CH0_BS_RST_REQ + IRQ_OFFSET));

   } else if(ipr & (0x1 << (CH1_BS_RST_REQ + IRQ_OFFSET))) { // Channel 1 BS Reset
      // Acknowledgement the DMC
      glbl_gpo4 = glbl_gpo4 | (0x1 << CH1_BS_RST_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);
      wait_intr_src_clr(); // Stretch for sampling by dmc_clk
      glbl_gpo4 = glbl_gpo4 & ~(0x1 << CH1_BS_RST_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);

      // Clear the interrupt
      wait_intr_src_clr();
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (CH1_BS_RST_REQ + IRQ_OFFSET));
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (CH1_ALERT_PAR_ERR + IRQ_OFFSET)); // Clear the alert interrupt in interrupt status register (ISR)

      // Call the service routine
      status = intr_bs_reset(1);
 
      if(status != 0) {
         DEBUG_PRINTF("ERROR: CALIBRATION_MODULE: gpio_interrupt_handler(): Interrupt subroutine FAILED\n");
         glbl_post_cal_error = 1;
         glbl_post_cal_status_tracking |= (1 << CH1_BS_RST_REQ);
         writeIO_XRAM(XRAM_CAL_POST_STATUS_TRACKING, glbl_post_cal_status_tracking);
         glbl_gpo3 |= ((glbl_post_cal_status_tracking << GPO3_POST_CAL_TRACKING_POS) | GPO3_POST_CAL_ERROR);
         writeIO_IOM(IO_GPO3, glbl_gpo3);
      } else {
         // Indicate Done to the DMC
         set_done = (0x1 << CH1_BS_RST_DONE);
      }
      intr_done = 1;
      glbl_ipr &= ~(0x1 << (CH1_BS_RST_REQ + IRQ_OFFSET));
  
   } else if(ipr & (0x1 << (SELF_REF_ENTRY_REQ + IRQ_OFFSET))) {  // Self Refresh Entry
#ifndef FIX_EDT_999807
      // Acknowledgement the DMC
      glbl_gpo4 = glbl_gpo4 | (0x1 << SELF_REF_ENTRY_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);
      wait_intr_src_clr(); // Stretch for sampling by dmc_clk
      glbl_gpo4 = glbl_gpo4 & ~(0x1 << SELF_REF_ENTRY_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);

      // Clear the interrupt
      wait_intr_src_clr();
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (SELF_REF_ENTRY_REQ + IRQ_OFFSET));
  
      // Enable Alert interrupts in this subroutine
      writeIO_IOM(IO_IRQ_ENABLE, IRQ_EN_ALERT);
#endif

   } else if(ipr & (0x1 << (SELF_REF_EXIT_REQ + IRQ_OFFSET))) {  // Self Refresh Exit
#ifndef FIX_EDT_999807
      // Acknowledgement the DMC
      glbl_gpo4 = glbl_gpo4 | (0x1 << SELF_REF_EXIT_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);
      wait_intr_src_clr(); // Stretch for sampling by dmc_clk
      glbl_gpo4 = glbl_gpo4 & ~(0x1 << SELF_REF_EXIT_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);

      // Clear the interrupt
      wait_intr_src_clr();
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (SELF_REF_EXIT_REQ + IRQ_OFFSET));
  
      // Enable Alert interrupts in this subroutine
      writeIO_IOM(IO_IRQ_ENABLE, IRQ_EN_ALERT);
#endif

   } else if(ipr & (0x1 << (CH0_ALERT_PAR_ERR + IRQ_OFFSET))) { // Channel 0 Alert based on parity error
      // Clear the interrupt
      wait_intr_src_clr();
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (CH0_ALERT_PAR_ERR + IRQ_OFFSET));
  
      // Call the service routine
      status = intr_alert_par_err(0);
 
      intr_done = 1;
      glbl_ipr &= ~(0x1 << (CH0_ALERT_PAR_ERR + IRQ_OFFSET));

   } else if(ipr & (0x1 << (CH1_ALERT_PAR_ERR + IRQ_OFFSET))) { // Channel 1 Alert based on parity error
      // Clear the interrupt
      wait_intr_src_clr();
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (CH1_ALERT_PAR_ERR + IRQ_OFFSET));
  
      // Call the service routine
      status = intr_alert_par_err(1);
 
      intr_done = 1;
      glbl_ipr &= ~(0x1 << (CH1_ALERT_PAR_ERR + IRQ_OFFSET));

   } else if(ipr & (0x1 << (PMC2UB_SWITCH_FREQ + IRQ_OFFSET))) {  // Frequency switching 
      // Acknowledgement the PMC
      glbl_gpo4 = glbl_gpo4 | (0x1 << FREQ_SWITCH_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);
      wait_intr_src_clr(); // Stretch for sampling by dmc_clk
      glbl_gpo4 = glbl_gpo4 & ~(0x1 << FREQ_SWITCH_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);

      // Clear the interrupt
      wait_intr_src_clr();
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (PMC2UB_SWITCH_FREQ + IRQ_OFFSET));
  
      // Enable Alert interrupts in this subroutine
      writeIO_IOM(IO_IRQ_ENABLE, IRQ_EN_ALERT);

   } else if(ipr & (0x1 << (PMC2UB_SR_EXIT + IRQ_OFFSET))) {  // PMC to UB Self Refresh Exit (power down exit & frequency switching use-cases, where DC is in reset state)
      // Acknowledgement the PMC
      glbl_gpo4 = glbl_gpo4 | (0x1 << UB2PMC_SR_EXIT_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);
      wait_intr_src_clr(); // Stretch for sampling by dmc_clk
      glbl_gpo4 = glbl_gpo4 & ~(0x1 << UB2PMC_SR_EXIT_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);

      // Clear the interrupt
      wait_intr_src_clr();
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (PMC2UB_SR_EXIT + IRQ_OFFSET));

      // Enable Alert interrupts in this subroutine
      writeIO_IOM(IO_IRQ_ENABLE, IRQ_EN_ALERT);

   } else if(ipr & (0x1 << (PMC2UB_SPARE_0 + IRQ_OFFSET))) {  // PMC to UB Interrupt Spare 0
      // Acknowledgement the PMC
      glbl_gpo4 = glbl_gpo4 | (0x1 << UB2PMC_SPARE_0_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);
      wait_intr_src_clr(); // Stretch for sampling by dmc_clk
      glbl_gpo4 = glbl_gpo4 & ~(0x1 << UB2PMC_SPARE_0_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);

      // Clear the interrupt
      wait_intr_src_clr();
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (PMC2UB_SPARE_0 + IRQ_OFFSET));
  
#ifdef FIX_EDT_999807
      // Enable Alert interrupts in this subroutine
      writeIO_IOM(IO_IRQ_ENABLE, IRQ_EN_ALERT);
#else
//      writeIO_IOM(IO_IRQ_ENABLE, IRQ_EN_ALERT); // No need, as there are no memory IO operations
#endif
  
   } else if(ipr & (0x1 << (PMC2UB_SPARE_1 + IRQ_OFFSET))) {  // PMC to UB Interrupt Spare 1
      // Acknowledgement the PMC
      glbl_gpo4 = glbl_gpo4 | (0x1 << UB2PMC_SPARE_1_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);
      wait_intr_src_clr(); // Stretch for sampling by dmc_clk
      glbl_gpo4 = glbl_gpo4 & ~(0x1 << UB2PMC_SPARE_1_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);

      // Clear the interrupt
      wait_intr_src_clr();
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (PMC2UB_SPARE_1 + IRQ_OFFSET));
  
//      writeIO_IOM(IO_IRQ_ENABLE, IRQ_EN_ALERT); // No need, as there are no memory IO operations
  
   } else if(ipr & (0x1 << (PMC2UB_SPARE_2 + IRQ_OFFSET))) {  // PMC to UB Interrupt Spare 2
      // Acknowledgement the PMC
      glbl_gpo4 = glbl_gpo4 | (0x1 << UB2PMC_SPARE_2_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);
      wait_intr_src_clr(); // Stretch for sampling by dmc_clk
      glbl_gpo4 = glbl_gpo4 & ~(0x1 << UB2PMC_SPARE_2_ACK);
      writeIO_IOM(IO_GPO4, glbl_gpo4);

      // Clear the interrupt
      wait_intr_src_clr();
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (PMC2UB_SPARE_2 + IRQ_OFFSET));
  
//      writeIO_IOM(IO_IRQ_ENABLE, IRQ_EN_ALERT); // No need, as there are no memory IO operations
  
#ifdef LPDDR4
   } else if(ipr & (0x1 << (CH0_OSC_VAL_RDY + IRQ_OFFSET))) {  // LPDDR4 Channel 0 DQS Oscillator tracking 
      // Clear the interrupt
      wait_intr_src_clr();
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (CH0_OSC_VAL_RDY + IRQ_OFFSET));
  
      // Call the service routine
      status = intr_lp4_wr_dqs_track(0);
 
      if(status != 0) {
         DEBUG_PRINTF("ERROR: CALIBRATION_MODULE: gpio_interrupt_handler(): Interrupt subroutine FAILED\n");
         glbl_post_cal_error = 1;
         glbl_post_cal_status_tracking |= (1 << POST_CAL_STATUS_LP4_OSC_TRACK_ERROR_CH0);
         writeIO_XRAM(XRAM_CAL_POST_STATUS_TRACKING, glbl_post_cal_status_tracking);
         glbl_gpo3 |= ((glbl_post_cal_status_tracking << GPO3_POST_CAL_TRACKING_POS) | GPO3_POST_CAL_ERROR);
         writeIO_IOM(IO_GPO3, glbl_gpo3);
      }
      intr_done = 1;
      glbl_ipr &= ~(0x1 << (CH0_OSC_VAL_RDY + IRQ_OFFSET));

   } else if(ipr & (0x1 << (CH1_OSC_VAL_RDY + IRQ_OFFSET))) {  // LPDDR4 Channel 1 DQS Oscillator tracking 
      // Clear the interrupt
      wait_intr_src_clr();
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (CH1_OSC_VAL_RDY + IRQ_OFFSET));
  
      // Call the service routine
      status = intr_lp4_wr_dqs_track(1);
 
      if(status != 0) {
         DEBUG_PRINTF("ERROR: CALIBRATION_MODULE: gpio_interrupt_handler(): Interrupt subroutine FAILED\n");
         glbl_post_cal_error = 1;
         glbl_post_cal_status_tracking |= (1 << POST_CAL_STATUS_LP4_OSC_TRACK_ERROR_CH1);
         writeIO_XRAM(XRAM_CAL_POST_STATUS_TRACKING, glbl_post_cal_status_tracking);
         glbl_gpo3 |= ((glbl_post_cal_status_tracking << GPO3_POST_CAL_TRACKING_POS) | GPO3_POST_CAL_ERROR);
         writeIO_IOM(IO_GPO3, glbl_gpo3);
      }
      intr_done = 1;
      glbl_ipr &= ~(0x1 << (CH1_OSC_VAL_RDY + IRQ_OFFSET));
#endif

   }

   if(intr_done == 1) {
      if(set_done != 0) {
         // Indicate Done
         glbl_gpo4 = glbl_gpo4 | set_done;
         writeIO_IOM(IO_GPO4, glbl_gpo4);
         wait_intr_src_clr(); // Stretch for sampling by dmc_clk
         glbl_gpo4 = glbl_gpo4 & ~(set_done);
         writeIO_IOM(IO_GPO4, glbl_gpo4);
      }

      if(glbl_cal_mode_bypass == 1) { // Calibration BYPASS
         // Enable all the Interrupts, except Alert interrupts and DQS Oscillator interrupts
         writeIO_IOM(IO_IRQ_ENABLE, (IRQ_EN & ~IRQ_EN_ALERT & ~IRQ_EN_DQS_OSC));
      } else {
         // Enable all the Interrupts, except Alert interrupts
         writeIO_IOM(IO_IRQ_ENABLE, (IRQ_EN & ~IRQ_EN_ALERT));
      }
   }

   // Lock NPI register write access
   writeIO_DMC(ral_addr_of_pcsr_lock_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_pcsr_lock ? 0x0 : 0xf9e8d7c6));

   writeIO_MBDR(RAM_DATA_CAL_OFFSET + 0xF08, glbl_ipr);

   return status;
}
//#endif
#endif // VNC_DIS_INTR

#ifdef LPDDR4
int update_dq_dbi_delay (u32 odelay_per_byte_flag, u8 *fine_delay, u8 *coarse_delay, u8 inc_dec, u8 step_size, u8 rank ) {
	u16 i,j,k,phy_nibble,wl_dly_rnk;
	u32 fine_inc=0, fine_dec=0, coarse_inc=0, coarse_dec=0, fine_load=0;
	u32 nibble_mask;
	if (odelay_per_byte_flag != 0xFFFFFFFF) {
                glbl_error_phy_nibble = 0;
		for(i=0; i<DQS_BITS; i++)
		{
			phy_nibble = glbl_dq_lnibble2pnibble_map[i*NIBBLE_PER_DQS];

			if((odelay_per_byte_flag & (1<<i)) == 0)
			{
				if(inc_dec == INC){
					if(fine_delay[i] < ((7*glbl_rl_dly_qtr[phy_nibble])/8))
					{
						fine_delay[i] = fine_delay[i] + step_size;
						fine_inc   |= (1<<i);
					}
					else
					{
						fine_delay[i] = 0;
						coarse_delay[i] = coarse_delay[i] + 1;
						if(coarse_delay[i]>5) {
                                                        glbl_error_phy_nibble |= (0x1 << phy_nibble); // Not able to find the noise region within 1tCK limit.
						}	
						coarse_inc |= (1<<i);
						fine_load   |= (1<<i);

					}
				} else {
					if(fine_delay[i] == 0) {
						if(coarse_delay[i]==0) {
                                                        glbl_error_phy_nibble |= (0x1 << phy_nibble); // Not able to decrease the coarse delay
						} else {
							coarse_delay[i] = coarse_delay[i] - 1;
							coarse_dec |= (1<<i);
							fine_delay[i] = (7*glbl_rl_dly_qtr[phy_nibble])/8;
							fine_load   |= (1<<i);
						}
					} else {
						fine_delay[i] = fine_delay[i]-step_size;
						fine_dec   |= (1<<i);
					}
				}
			}
		}

                if(glbl_error_phy_nibble != 0) {
                   write_error_nibble_XRAM(1, glbl_error_phy_nibble);
                   return 1;
		}

		writeIO_RIU(RPI_BCAST_NIBBLE_MASK0, ~glbl_dq_nibble_mask_initial);
		riu_reg_bcast(BS_RESET_CTRL, 0, BS_RESET_WT);
		u32 loop;
		for(loop=0; loop < (glbl_tck / BS_RESET_EXTEND); loop++) { // Extend bit-slice reset for lower frequencies
			riu_reg_bcast(BS_RESET_CTRL, 0, BS_RESET_WT);
		}
		if(coarse_inc != 0)
		{
			for(i=0; i<DQS_BITS; i++) {
				if((coarse_inc>>i)&0x1) {
					for(j=0;j<NIBBLE_PER_DQS;j++) {
						phy_nibble = glbl_dq_lnibble2pnibble_map[i*NIBBLE_PER_DQS+j];
						wl_dly_rnk = read_riu_nibble_reg(phy_nibble, WLDLYRNK0 + rank);
						write_riu_nibble_reg(phy_nibble, WLDLYRNK0 + rank,wl_dly_rnk+0x200);
					}
				}
			}
		}
		if(coarse_dec != 0)
		{
			for(i=0; i<DQS_BITS; i++) {
				if((coarse_dec>>i)&0x1) {
					for(j=0;j<NIBBLE_PER_DQS;j++) {
						phy_nibble = glbl_dq_lnibble2pnibble_map[i*NIBBLE_PER_DQS+j];
						wl_dly_rnk = read_riu_nibble_reg(phy_nibble, WLDLYRNK0 + rank);
						write_riu_nibble_reg(phy_nibble, WLDLYRNK0 + rank,wl_dly_rnk-0x200);
					}
				}
			}
		}
		if(fine_load != 0)
		{
			for(i=0; i<DQS_BITS; i++) {
				if((fine_load>>i)&0x1) {
					for(j=0;j<NIBBLE_PER_DQS;j++) {
						phy_nibble = glbl_dq_lnibble2pnibble_map[i*NIBBLE_PER_DQS+j];
						for(k=0;k<6;k++) {
							if(k!=1) {
								write_riu_nibble_reg(phy_nibble, ODLY0 + k,fine_delay[i]);
							}
						}
					}
				}
			}
		}
		if(fine_inc != 0)
		{
			fine_inc = ~fine_inc;
			update_nibble_dly_mask(&fine_inc, &nibble_mask);
			writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, nibble_mask);
			riu_reg_bcast(IODLY_BCAST_MASK, 1, 0x000000Bf); // 0000_1111_1111 // Enable bitslices 5,4,3,2,0 for odelay; Move DQs and DBI together
			riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000038); // 0000_0000_1100 // Increment By Coarse
		}
		if(fine_dec != 0)
		{
			fine_dec = ~fine_dec;
			update_nibble_dly_mask(&fine_dec, &nibble_mask);
			writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, nibble_mask);
			riu_reg_bcast(IODLY_BCAST_MASK, 1, 0x000000Bf); // 0000_1111_1111 // Enable bitslices 5,4,3,2,0 for odelay; Move DQs and DBI together
			riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000028); // 0000_0000_1100 // Decrement By Coarse
		}
                bitslice_reset_bcast(0, BS_RESET_WT);
	}
	return 0;
}

int get_lp4_dq_dbi_region(u8 data_type, u8 region_type, u8 rank, u8 *coarse, u8 *fine, u8 step_size, u8 inc_dec)
{
	u8 i, j, k, b, phy_nibble;
	u8 expected_min_noise_width=15;
	u16 odly, wl_dly_rnk;
	u32 odelay_per_byte_flag = 0;
	u32 dq_compare_result, bit_comp_result, bit_comp_status;
	u32 nibble_mask;
	static u16 valid_region_width[DQS_BITS*NIBBLE_PER_DQS*4];
	static u16 noise_region_width[DQS_BITS*NIBBLE_PER_DQS*4];
	for(i=0; i<DQS_BITS; i++)
	{
           odelay_per_byte_flag |= (1<<i);
           for(j=0; j<NIBBLE_PER_DQS; j++) {
			for(k=0; k<4; k++) {
				valid_region_width[k + j*4 + i*NIBBLE_PER_DQS*4] = 0;
				noise_region_width[k + j*4 + i*NIBBLE_PER_DQS*4] = 0;
			}
           }
	}

	odelay_per_byte_flag = ~odelay_per_byte_flag;

	writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

	// Setting the Step Size into the INC DEC Register
	writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
	riu_reg_bcast(INCDEC_CRSE, 1, step_size);

	while(odelay_per_byte_flag != 0xFFFFFFFF)
	{
		simple_write_sample(0);
		if(data_type == BITS) {
			dq_compare_result = 0;
			for(i=0; i<DQS_BITS; i++) {
			   for(j=0; j<NIBBLE_PER_DQS; j++) {
				bit_comp_result=0;
				bit_comp_status = readIO_DMC(ral_addr_of_compare_perbit_err_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*NIBBLE_PER_DQS * i + 4*j));
				for(b=0;b<4;b++) {
					if(region_type == FULL){
						if((bit_comp_status & (0x11111111<<b)) == (0x11111111<<b)) {
							bit_comp_result = bit_comp_result | (0x1<<b);
								noise_region_width[b + j*4 + i*NIBBLE_PER_DQS*4] = noise_region_width[b + j*4 + i*NIBBLE_PER_DQS*4]+step_size;
						} else {
								noise_region_width[b + j*4 + i*NIBBLE_PER_DQS*4] = 0;
                                                }
					} else {
						if((bit_comp_status & (0x11111111<<b))!= (0x00000000<<b)) {
							bit_comp_result = bit_comp_result | (0x1<<b);
								valid_region_width[b + j*4 + i*NIBBLE_PER_DQS*4] = 0;
								noise_region_width[b + j*4 + i*NIBBLE_PER_DQS*4] = noise_region_width[b + j*4 + i*NIBBLE_PER_DQS*4]+step_size;
						} else {
								valid_region_width[b + j*4 + i*NIBBLE_PER_DQS*4] = valid_region_width[b + j*4 + i*NIBBLE_PER_DQS*4]+step_size;
								noise_region_width[b + j*4 + i*NIBBLE_PER_DQS*4] = 0;
                                                }

					}
				}
				if(bit_comp_result == 0xF)
					dq_compare_result=dq_compare_result | (1<<(i*NIBBLE_PER_DQS+j));
                            }
			}
		} else {
		   dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK));
		   for(i=0; i<DQS_BITS; i++) {
		      for(j=0; j<NIBBLE_PER_DQS; j++) {
                         if( (  dq_compare_result>>(i*NIBBLE_PER_DQS+j)  ) & 0x1 ) {
		   	    for(b=0;b<4;b++) {
		   	       valid_region_width[b + j*4 + i*NIBBLE_PER_DQS*4] = 0;
		   	       noise_region_width[b + j*4 + i*NIBBLE_PER_DQS*4] = noise_region_width[b + j*4 + i*NIBBLE_PER_DQS*4]+step_size;
                            }
                         } else {
		   	    for(b=0;b<4;b++) {
	                       valid_region_width[b + j*4 + i*NIBBLE_PER_DQS*4] = valid_region_width[b + j*4 + i*NIBBLE_PER_DQS*4]+step_size;
		   	       noise_region_width[b + j*4 + i*NIBBLE_PER_DQS*4] = 0;
                            }
                         }
                      }
                   }
		}
		// Here We broadcast the odelay increment in every dbi per dqs
		for(i=0; i<DQS_BITS; i++)
		{
			if((odelay_per_byte_flag & (1<<i)) == 0)
			{
				if(NIBBLE_PER_DQS==2) {
					if((dq_compare_result & 0x3) == 0x3 && (region_type == FULL || region_type == LOW_NOISE || region_type == DEEP_NOISE ) )
					{
						odelay_per_byte_flag |= (1<<i);
						if(region_type == DEEP_NOISE) {
							for(j=0; j<NIBBLE_PER_DQS; j++) {
								for(k=0; k<4; k++) {
									if(noise_region_width[k + j*4 + i*NIBBLE_PER_DQS*4]<expected_min_noise_width)
										odelay_per_byte_flag &= (~(1<<i));
								}
							}
						}

					} else if ((dq_compare_result & 0x3) != 0x3 && region_type == PARTIAL) {
                                           for(j=0; j<NIBBLE_PER_DQS; j++) {
                                              for(k=0; k<4; k++) {
								if(valid_region_width[k + j*4 + i*NIBBLE_PER_DQS*4]>MIN_VLD_CNT)
						    odelay_per_byte_flag |= (1<<i);
                                              }
                                           }
					}
				} else {
					if((dq_compare_result & 0x1) == 0x1 && (region_type == FULL || region_type == LOW_NOISE ))
					{
						odelay_per_byte_flag |= (1<<i);
						if(region_type == DEEP_NOISE) {
							for(k=0; k<4; k++) {
								if(noise_region_width[k + i*4]<expected_min_noise_width)
									odelay_per_byte_flag &= (~(1<<i));
							}
						}
					} else if ((dq_compare_result & 0x1) != 0x1 && region_type == PARTIAL) {
                                           for(k=0; k<4; k++) {
							if(valid_region_width[k + i*4]>MIN_VLD_CNT)
					         odelay_per_byte_flag |= (1<<i);
                                           }
					}
				}
			}

			dq_compare_result = (dq_compare_result >> NIBBLE_PER_DQS);
		}
		if(update_dq_dbi_delay (odelay_per_byte_flag, fine, coarse, inc_dec, step_size, rank ))
			return 1;

	}

	// XRAM_DBG: LP4: wrdqdbi_stg2a_wldlyrnk_crse: per_dqs, noise detection for DBI pin
	// XRAM_DBG: LP4: wrdqdbi_stg2a_dq_odly: per_dqs, noise detection for DBI pin
	for(i=0; i<DQS_BITS; i++) {
		phy_nibble = glbl_dbi_bits_riu_map[i][0];
		wl_dly_rnk = read_riu_nibble_reg(phy_nibble, WLDLYRNK0 + rank);
		writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG2A_WLDLYRNK_CRSE + (i * 4), (wl_dly_rnk >> 9) & 0xF);

		odly = readIO_RIU((phy_nibble << RIU_NIB_POS) | ODLY0); // Read from ODLY0 for DBI pin.
		writeIO_XRAM(glbl_xram_freq_base + XRAM_F0_WRDQDBI_STG2A_DBI_ODLY + (i * 4), odly);
	}

	return 0;
}

int get_lp4_dq_per_bit_deskew (u8 step_size)
{
  u8 i,j;
  u32 dq_odelay_per_byte_flag = 0;
  static u8 dq_odelay_per_bit_flag[DQS_BITS];
  u16 dq_dly_count = 0;
  u8 bit_mask = (NIBBLE_PER_DQS == 1) ? 0xF0 : 0x00;
  u32 dq_compare_result;
  static u8 count[DQS_BITS][4*NIBBLE_PER_DQS];
  u16 bcast_mask;
  u32 nibble_mask;
  u32 bit_comp_result;
  u32 bit_comp_status;
  int b;

  for(i=0; i<DQS_BITS; i++)
  {
    dq_odelay_per_byte_flag     |= (1<<i);
    dq_odelay_per_bit_flag[i]   = 0;
    
    for(j=0; j<(4*NIBBLE_PER_DQS); j++)
      count[i][j] = 0;
  }

  dq_odelay_per_byte_flag = ~dq_odelay_per_byte_flag;

  writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

  // Setting the Step Size into the INC DEC Register
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(INCDEC_CRSE, 1, step_size);

  while(dq_odelay_per_byte_flag != 0xFFFFFFFF)
  {

    DEBUG_PRINTF("INFO: get_lp4_dq_per_bit_deskew(): dq_odelay_per_byte_flag = %x \n", dq_odelay_per_byte_flag); 

    if(dq_odelay_per_byte_flag != 0xFFFFFFFF)
    {
      dq_dly_count = dq_dly_count + step_size;

      if(dq_dly_count > glbl_max_limit_180_degree) {
        DEBUG_PRINTF("ERROR: get_lp4_dq_per_bit_deskew(): Delay crossed more than 180 degrees\n");
        write_error_nibble_XRAM(0, ~dq_odelay_per_byte_flag);

        glbl_bits_error_status_31_0 = 0;
        glbl_bits_error_status_63_32 = 0;
        glbl_bits_error_status_95_64 = 0;
        for(i=0; i<DQS_BITS; i++) {
          if(i*BITS_PER_DQS < 32) {
            glbl_bits_error_status_31_0 |= ((~dq_odelay_per_bit_flag[i] & BITS_PER_DQS_MASK) << ((i * BITS_PER_DQS)%32));
          } else if(i*BITS_PER_DQS < 64) {
            glbl_bits_error_status_63_32 |= ((~dq_odelay_per_bit_flag[i] & BITS_PER_DQS_MASK) << ((i * BITS_PER_DQS)%32));
          } else {
            glbl_bits_error_status_95_64 |= ((~dq_odelay_per_bit_flag[i] & BITS_PER_DQS_MASK) << ((i * BITS_PER_DQS)%32));
          }
        }
        return 1;
      }

      // Bitwise Mask set to enable the Movement of DQ 
      for(i=0; i<DQS_BITS; i++)
      {
        bcast_mask = 0;

        // Logical to Physical Mapping of the Bcast Mask  
        for(j=0; j<(NIBBLE_PER_DQS*4); j++)
        {
          if(dq_odelay_per_bit_flag[i] & (1<<j))
          {
            //if(glbl_dqs_bits_riu_map[i][0] == glbl_dq_bits_riu_map[(4*i*NIBBLE_PER_DQS) + j][0])  
            if(glbl_dq_lnibble2pnibble_map[i*NIBBLE_PER_DQS] == glbl_dq_bits_riu_map[(4*i*NIBBLE_PER_DQS) + j][0])  
              bcast_mask |= (0x1 << (glbl_dq_bits_riu_map[(4*i*NIBBLE_PER_DQS) + j][1] - 2));  
            else
              bcast_mask |= (0x1 << (4 + (glbl_dq_bits_riu_map[(4*i*NIBBLE_PER_DQS) + j][1] - 2)));  
          }
        }
       
        //write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], IODLY_BCAST_MASK, ((bcast_mask & 0xF) << 8) | 0XFF);
        write_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[(i*NIBBLE_PER_DQS)], IODLY_BCAST_MASK, ((bcast_mask & 0xF) << 8) | 0XFF);

        if(NIBBLE_PER_DQS == 2)
        {
          bcast_mask = (bcast_mask >> 4) & 0xF;
          //write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], IODLY_BCAST_MASK, (bcast_mask << 8) | 0xFF);
          write_riu_nibble_reg(glbl_dq_lnibble2pnibble_map[(i*NIBBLE_PER_DQS) + 1], IODLY_BCAST_MASK, (bcast_mask << 8) | 0xFF);
        }
      }
  
      // Need BroadCast the ODELAY for those bytes for which byte_flag is not set
      update_nibble_dly_mask(&dq_odelay_per_byte_flag, &nibble_mask);
      writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
      riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000038); // 0000_0000_1100 // Increment By Coarse
      bitslice_reset_bcast(1, BS_RESET_WT);
    }
    simple_write_sample(0);

    for(i=0; i<DQS_BITS; i++)
    {
      if((dq_odelay_per_byte_flag & (1<<i)) == 0)
      {
          //dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK));
          dq_compare_result=0;
             bit_comp_result=0;
             bit_comp_status = readIO_DMC(ral_addr_of_compare_perbit_err_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*NIBBLE_PER_DQS * i));
             for(b=0;b<4;b++) {
             	if(bit_comp_status & (0x11111111<<b))  {
                   bit_comp_result = bit_comp_result | (0x1<<b);
                }
             }
             dq_compare_result=bit_comp_result;
             if(NIBBLE_PER_DQS == 2) {
                bit_comp_result=0;
                bit_comp_status = readIO_DMC(ral_addr_of_compare_perbit_err_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK) + (4*NIBBLE_PER_DQS * i) + 4);
                for(b=0;b<4;b++) {
             	   if(bit_comp_status & (0x11111111<<b))  {
                      bit_comp_result = bit_comp_result | (0x1<<b);
                   }
                }
                dq_compare_result=dq_compare_result | (bit_comp_result<<4);
             }

        DEBUG_PRINTF("INFO: get_lp4_dq_per_bit_deskew(): dq_compare_result = %x \n", dq_compare_result);

        for(j=0; j<(4*NIBBLE_PER_DQS); j++)
        {
          if((dq_odelay_per_bit_flag[i] & (1<<j)) == 0)
          {
            if(step_size == 1)
            {
              if((dq_compare_result & (1<<j))  == 0)
              {
                count[i][j] += 1;

                DEBUG_PRINTF("INFO: get_lp4_dq_per_bit_deskew(): count[%d][%d] = %d \n", i, j, count[i][j]);
             
                if(count[i][j] >= (MIN_VLD_CNT))
                {
                  dq_odelay_per_bit_flag[i] = dq_odelay_per_bit_flag[i] | (1<<j);
                  DEBUG_PRINTF("INFO: get_lp4_dq_per_bit_deskew(): dq_odelay_per_bit_flag[%d] = %x \n", i, dq_odelay_per_bit_flag[i]); 
                }
              }
              else
                count[i][j] = 0;
            }
            else
            {
              if((dq_compare_result & (1<<j)) == 0)
              {
                dq_odelay_per_bit_flag[i] = dq_odelay_per_bit_flag[i] | (1<<j);
                DEBUG_PRINTF("INFO: get_lp4_dq_per_bit_deskew(): dq_odelay_per_bit_flag[%d] = %x \n", i, dq_odelay_per_bit_flag[i]); 
              }
            }
          }
        }  

        DEBUG_PRINTF("INFO: get_lp4_dq_per_bit_deskew(): dq_odelay_per_bit_flag[%d] = %x \n", i, dq_odelay_per_bit_flag[i]);

        if((dq_odelay_per_bit_flag[i] | bit_mask) == 0xFF)
          dq_odelay_per_byte_flag |= (1<<i);
      }
    }
  }

  // BCAST MASK will be set based on individual byte
  for(i=0; i<DQS_BITS; i++)
  {
    write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], IODLY_BCAST_MASK, 0xFF);

    if(NIBBLE_PER_DQS == 2)
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], IODLY_BCAST_MASK, 0xFF);
  }

  // Need BroadCast the ODELAY for those bytes for which byte_flag is not set
  if(step_size != 1)
  {
    writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
    riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000028); // 0000_0000_1100 // Decrement by coarse
  }

  bitslice_reset_bcast(1, BS_RESET_WT);
  return 0;
}

int get_lp4_dbi_bit_deskew (u8 step_size)
{
  u8 i;
  u32 dbi_odelay_per_byte_flag = 0;
  u32 dq_compare_result;
  u16 dbi_dly_count = 0;
  u32 nibble_mask;
  static u8 count[DQS_BITS];
 
	for(i=0; i<DQS_BITS; i++) {
    dbi_odelay_per_byte_flag |= (1<<i);
		count[i]=0;
	}
 
  dbi_odelay_per_byte_flag = ~dbi_odelay_per_byte_flag;
   
  writeIO_DMC(ral_addr_of_seq_expected_pat_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0);

  // Setting the Step Size into the INC DEC Register
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(INCDEC_CRSE, 1, step_size);

  while(dbi_odelay_per_byte_flag != 0xFFFFFFFF)
  {

   
    if(dbi_odelay_per_byte_flag != 0xFFFFFFFF)
    {
      dbi_dly_count = dbi_dly_count + step_size;

      DEBUG_PRINTF("INFO: get_lp4_dbi_bit_deskew(): dbi_dly_count = %d \n", dbi_dly_count);

      if(dbi_dly_count > glbl_max_limit_180_degree) {
        DEBUG_PRINTF("ERROR: get_lp4_dbi_bit_deskew(): Delay crossed more than 180 degrees\n");
        write_error_nibble_XRAM(0, ~dbi_odelay_per_byte_flag);
        return 1;
      }

      // Broadcast the DBI Odelay for those bytes for which byte flag is not set 
      update_byte_dly_mask(&dbi_odelay_per_byte_flag, &nibble_mask);
      writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
      riu_reg_bcast(ODLY0, 1, 0xA000); // Step Wise Increment of ODLY

      DEBUG_PRINTF("INFO: get_lp4_dbi_bit_deskew(): dbi_odelay_per_byte_flag = %x \n", dbi_odelay_per_byte_flag);
    }
    simple_write_sample(0);

        dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK));
        DEBUG_PRINTF("INFO: get_lp4_dbi_bit_deskew(): dq_compare_result = %x \n", dq_compare_result);

        // Here We Broadcast the odelay increment in every DBI per DQS
        for(i=0; i<DQS_BITS; i++)
        {
          if((dbi_odelay_per_byte_flag & (1<<i)) == 0)
          {
            if(step_size == 1)
            {
              if((dq_compare_result & 0x3) == 0)
              {
                count[i] += 1;

                DEBUG_PRINTF("INFO: get_lp4_dbi_bit_deskew(): count[%d] = %d \n", i, count[i]);

                if(count[i] >= MIN_VLD_CNT)
                {
                  dbi_odelay_per_byte_flag |= (1<<i);
                  DEBUG_PRINTF("INFO: get_lp4_dbi_bit_deskew(): dbi_odelay_per_byte_flag = %x \n", dbi_odelay_per_byte_flag);
                }
              }
              else
                count[i] = 0;
            }
            else
            {
              if((dq_compare_result & 0x3) == 0)
              {
                dbi_odelay_per_byte_flag |= (1<<i);
                DEBUG_PRINTF("INFO: get_lp4_dbi_bit_deskew(): dbi_odelay_per_byte_flag = %x \n", dbi_odelay_per_byte_flag);
              }
            }
          }
          dq_compare_result = (dq_compare_result >> NIBBLE_PER_DQS);
        }
  }
 
  // Broadcast the DBI Odelay for those bytes for which byte flag is not set 
  if(step_size != 1)
  {
    writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, ~glbl_dbi_nibble_mask_initial);
    riu_reg_bcast(ODLY0, 1, 0x6000); // Step Wise Decrement of ODLY
  }

  return 0;
}

int get_lp4_centering(u8 rank, u16 *valid_width)
{
  u8 i;
  u8 phy_nibble;
  u16 dq_odly;
  u16 wl_dly_rnk;
  u32 dq_odelay_per_byte_flag = 0; 
  u32 nibble_mask;

  for(i=0; i<DQS_BITS; i++)
    dq_odelay_per_byte_flag |= (1<<i);

  dq_odelay_per_byte_flag = ~dq_odelay_per_byte_flag;

  for(i=0; i<DQS_BITS; i++)
  {
    if(NIBBLE_PER_DQS == 2) 
    {
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], INCDEC_CRSE, ((valid_width[i]+MIN_VLD_CNT) / 2));
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], INCDEC_CRSE, ((valid_width[i]+MIN_VLD_CNT) / 2));
    }
    else
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], INCDEC_CRSE, ((valid_width[i]+MIN_VLD_CNT)  / 2));
  }
  
  // Decrement DQ delay by Step Size
  update_nibble_dly_mask(&dq_odelay_per_byte_flag, &nibble_mask);
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
  riu_reg_bcast(IODLY_BCAST_MASK, 1, 0x000000ff); // 0000_1111_1111 
  riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000028); // 0000_0000_1100 // Decrement by coarse

  // Decrement DBI Delay by Step Size
  update_byte_dly_mask (&dq_odelay_per_byte_flag, &nibble_mask);
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
  riu_reg_bcast(ODLY0, 1, 0x6000); // In this ODLY DBI is Mapped (Coarse Decrement)

  for(i=0; i<DQS_BITS*NIBBLE_PER_DQS; i++) {
    phy_nibble = glbl_map_riu[i];
    wl_dly_rnk = read_riu_nibble_reg(phy_nibble, WLDLYRNK0 + rank);
    dq_odly = read_riu_nibble_reg(phy_nibble, ODLY0 + 2);
    write_riu_nibble_reg(phy_nibble, WLDLYRNK0 + rank, (wl_dly_rnk & 0x1E00) | dq_odly);
  }
  bitslice_reset_bcast(1, BS_RESET_WT);

  return 0;
}

int get_lp4_noise_left_edge (u8 rank, u8 step_size, u8 pattern_type, u16 *valid_width)
{
  u8 i, j;
  u32 dq_odelay_per_byte_flag = 0;
  u16 dq_dly_count = 0;
  u32 dq_compare_result;
  u32 nibble_mask;
  static u16 odly[4], odly_min;

  for(i=0; i<DQS_BITS; i++)
    dq_odelay_per_byte_flag |= (1<<i);
  
  dq_odelay_per_byte_flag = ~dq_odelay_per_byte_flag;

  // Setting the Step Size into the INC DEC Register
  update_nibble_dly_mask(&dq_odelay_per_byte_flag, &nibble_mask); 
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
  riu_reg_bcast(INCDEC_CRSE, 1, step_size);

  // Finding the Minimum Odelay Value Across Each Byte
  for(i=0; i<DQS_BITS; i++)
  {
    odly[0] = (0x1FF & read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], ODLY2));
    odly[1] = (0x1FF & read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], ODLY3));
    odly[2] = (0x1FF & read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], ODLY4));
    odly[3] = (0x1FF & read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], ODLY5));

    for(j=0; j<4; j++)
    {
      if(j==0)
        odly_min = odly[0];
      else if(odly[j] < odly_min)
        odly_min = odly[j]; 
    } 

    if(NIBBLE_PER_DQS == 2)
    {
      odly[0] = (0x1FF & read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], ODLY2));
      odly[1] = (0x1FF & read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], ODLY3));
      odly[2] = (0x1FF & read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], ODLY4));
      odly[3] = (0x1FF & read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], ODLY5));

      for(j=0; j<4; j++)
      {
        if(odly[j] < odly_min)
          odly_min = odly[j]; 
      }
    }

    if(glbl_wr_dbi_en)
    {
      odly[0] = (0x1FF & read_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], ODLY0));

      if(odly[0] < odly_min)
        odly_min = odly[0];
    }
  }

  while(dq_odelay_per_byte_flag != 0xFFFFFFFF)
  {
    dq_dly_count = dq_dly_count + step_size;
    DEBUG_PRINTF("INFO: get_lp4_noise_left_edge(): dq_dly_count = %d\n", dq_dly_count);

    if(dq_dly_count > glbl_max_limit_180_degree) {
      DEBUG_PRINTF("ERROR: get_lp4_noise_left_edge(): Delay crossed more than 180 degrees\n");
      write_error_nibble_XRAM(0, ~dq_odelay_per_byte_flag);
      return 1;
    }

    // BroadCast DQ ODELAY Decrement Logic for which byte flag is not set
    update_nibble_dly_mask(&dq_odelay_per_byte_flag, &nibble_mask);
    writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
    riu_reg_bcast(IODLY_BCAST_MASK, 1, 0x000000ff); // 0000_1111_1111 
    riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000028); // 0000_0000_1100 // Decrement By Coarse
    bitslice_reset_bcast(1, BS_RESET_WT);

    if(glbl_wr_dbi_en)
    {
      // Need BroadCast the Odelay for those bytes for which Byte Flag is not set
      update_byte_dly_mask (&dq_odelay_per_byte_flag, &nibble_mask);
      writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
      riu_reg_bcast(ODLY0, 1, 0x6000); // In this ODLY DBI is Mapped (Coarse Decrement)
    }
    bitslice_reset_bcast(1, BS_RESET_WT);

    if(pattern_type == COMPLEX)
      cplx_write_sample(rank);
    else if(pattern_type == PRBS)
      prbs_write_sample();
    else
      simple_write_sample(0);

    dq_compare_result = readIO_DMC(ral_addr_of_compare_nibble_err_in_ddrmc_main_ddrmc_main_bank(BLK));

    for(i=0; i<DQS_BITS; i++)
    {
      if((dq_odelay_per_byte_flag & (1<<i)) == 0)
      {
        if(((NIBBLE_PER_DQS == 2)  && ((dq_compare_result & 0x3) != 0)) || ((NIBBLE_PER_DQS == 1)  && ((dq_compare_result & 0x1) != 0)))
        {
          dq_odelay_per_byte_flag |= (1<<i);

          valid_width[i] += dq_dly_count - step_size;

          DEBUG_PRINTF("INFO: get_lp4_noise_left_edge(): valid_width[%d] = %d\n", i, valid_width[i]);
        }
        else if((dq_dly_count + step_size) > odly_min)
        {
          dq_odelay_per_byte_flag |= (1<<i);

          valid_width[i] += dq_dly_count - step_size;

          DEBUG_PRINTF("INFO: get_lp4_noise_left_edge(): valid_width[%d] = %d\n", i, valid_width[i]);
        }
      }

      dq_compare_result = (dq_compare_result >> NIBBLE_PER_DQS);
    }

    DEBUG_PRINTF("INFO: get_lp4_noise_left_edge(): dq_odelay_per_byte_flag = %x\n", dq_odelay_per_byte_flag);
  }

    dq_dly_count -= step_size;
    DEBUG_PRINTF("INFO: get_lp4_noise_left_edge(): final dq_dly_count = %d\n", dq_dly_count);

  // Need BroadCast the Odelay to increase the coarsse by step_size For DQ
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, ~glbl_dq_nibble_mask_initial);
  riu_reg_bcast(IODLY_BCAST_MASK, 1, 0x000000ff); // 0000_1111_1111 
  riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000038); // 0000_0000_1100 // Increment by coarse

  // Need BroadCast the Odelay to increase the coarsse by step_size For DBI/DM
  if(glbl_wr_dbi_en)
  {
    writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, ~glbl_dbi_nibble_mask_initial);
    riu_reg_bcast(ODLY0, 1, 0xA000); // In this ODLY DBI is Mapped (Coarse Increment)
  }
  bitslice_reset_bcast(1, BS_RESET_WT);
 
  return 0;
}

int get_lp4_cplx_centre (u8 rank, u16 *left_margin, u16 *right_margin)
{
  u8 i;
  u8 phy_nibble;
  u16 dq_odly;
  u16 wl_dly_rnk;
  static u16 dq_shift[DQS_BITS];
  u32 nibble_mask;
  u32 dq_odelay_per_byte_flag = 0;

  for(i=0; i<DQS_BITS; i++)
    dq_odelay_per_byte_flag |= (1<<i);

  dq_odelay_per_byte_flag = ~dq_odelay_per_byte_flag;

  for(i=0; i<DQS_BITS; i++)
  {
    dq_shift[i] = (left_margin[i] + right_margin[i]) / 2 ;  

    DEBUG_PRINTF("INFO: get_lp4_cplx_centre(): dq_shift[%d] = %d\n", i, dq_shift[i]);
  } 

  // INC_DEC MASK SET FOR DQ
  for(i=0; i<DQS_BITS; i++)
  {
    write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS)], INCDEC_CRSE, dq_shift[i]);
    
    if(NIBBLE_PER_DQS == 2)
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], INCDEC_CRSE, dq_shift[i]);
  }

  update_nibble_dly_mask(&dq_odelay_per_byte_flag, &nibble_mask); 
  writeIO_RIU( RPI_BCAST_NIBBLE_MASK1, nibble_mask);
  riu_reg_bcast(IODLY_BCAST_MASK, 1, 0x000000FF); // 0000_1111_1111 
  riu_reg_bcast(IODLY_BCAST_CTRL, 1, 0x00000038); // 0000_0010_1000 // Decrement By coarse

  if(glbl_wr_dbi_en)
  {
    // INC DEC mask set for DBI
    for(i=0; i<DQS_BITS; i++)
      write_riu_nibble_reg(glbl_map_riu[(i*NIBBLE_PER_DQS) + 1], INCDEC_CRSE, dq_shift[i]);

    // Broadcast Increment the DBI
    update_byte_dly_mask(&dq_odelay_per_byte_flag, &nibble_mask);
    writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, nibble_mask);
    riu_reg_bcast(ODLY0, 1, 0xA000); // Step Wise Increment of ODLY
  } 

  for(i=0; i<DQS_BITS*NIBBLE_PER_DQS; i++) {
    phy_nibble = glbl_map_riu[i];
    wl_dly_rnk = read_riu_nibble_reg(phy_nibble, WLDLYRNK0 + rank);
    dq_odly = read_riu_nibble_reg(phy_nibble, ODLY0 + 2);
    write_riu_nibble_reg(phy_nibble, WLDLYRNK0 + rank, (wl_dly_rnk & 0x1E00) | dq_odly);
  }
  bitslice_reset_bcast(1, BS_RESET_WT);

  return 0;
}
#endif

void set_data_bs_rst_mask() {
   writeIO_RIU(RPI_BCAST_NIBBLE_MASK1, ~glbl_dqs_nibble_mask_initial);
   riu_reg_bcast(CFG_BS_RST_MASK,1,~0x7F); 
   writeIO_RIU(RPI_BCAST_NIBBLE_MASK2, ~(glbl_dq_nibble_mask_initial | (~glbl_dqs_nibble_mask_initial)));
   riu_reg_bcast(CFG_BS_RST_MASK,2,~0x7D); 
} 

#ifdef LPDDR4
void set_ca_vref(u8 ca_group_idx, u8 vref_val) {
   DEBUG_PRINTF("INFO: ddr_ca_cal(): vref_val = 0x%08X \n",vref_val);
   //u8 dq_3_0 = glbl_lp4_mr12 & 0x0F;
   //u8 dq_7_4 = (glbl_lp4_mr12 & 0xF0)>>4;
   // Map the logical dq bits to physical dq bits
   u32 pb_dq_bits_31_0  = 0;
   u32 pb_dq_bits_63_32 = 0;
   u8 bit;
   u8 nibble;
   for(bit=ca_group_idx*16; bit < ca_group_idx*16+16; bit++) {
      if(glbl_dq_pbit2lbit_map[bit]<32) {
         if((vref_val>>(bit%8)) & 0x1) {
            pb_dq_bits_31_0 = pb_dq_bits_31_0 | (1<<glbl_dq_pbit2lbit_map[bit]);
         }
      } else {
         if((vref_val>>(bit%8)) & 0x1) {
            pb_dq_bits_63_32 = pb_dq_bits_63_32 | (1<<(glbl_dq_pbit2lbit_map[bit]-32));
         }
      }
   }
   DEBUG_PRINTF("INFO: ddr_ca_cal(): pb_dq_bits_31_0  = 0x%08X \n", pb_dq_bits_31_0 );
   DEBUG_PRINTF("INFO: ddr_ca_cal(): pb_dq_bits_63_32 = 0x%08X \n", pb_dq_bits_63_32);

     // dq_7_4_o=0;
     //    dq_7_4_o = dq_7_4 | (dq_7_4_o << 4);
   u8 i;
   u8  dq_3_0;
   u32 dq_3_0_o;
   static u32 dq_nibble[18];
   for(nibble=ca_group_idx*4;nibble<ca_group_idx*4+4;nibble++) {
      dq_3_0_o=0;  
      if(nibble<8) {
         dq_3_0 = (pb_dq_bits_31_0 >> (4*nibble) ) & 0xF;
      } else {
         dq_3_0 = (pb_dq_bits_63_32>> (4*nibble) ) & 0xF;
      }
      for(i=0;i<8;i++) {
         dq_3_0_o = dq_3_0 | (dq_3_0_o << 4);
      }
      dq_nibble[nibble] = dq_3_0_o;
      writeIO_DMC(ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK)+nibble*4,dq_nibble[nibble]);
      DEBUG_PRINTF("INFO: ddr_ca_cal(): dq_nibble[%d]  = 0x%08X \n", nibble,dq_nibble[nibble]);
      //writeIO_DMC(ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK)+nibble*4,dq_3_0_o);
   }
   //writeIO_DMC(ral_addr_of_seq_dq_nibble0_in_ddrmc_main_ddrmc_main_bank(BLK),dq_nibble[0]);
   //writeIO_DMC(ral_addr_of_seq_dq_nibble1_in_ddrmc_main_ddrmc_main_bank(BLK),dq_nibble[1]);
   //writeIO_DMC(ral_addr_of_seq_dq_nibble2_in_ddrmc_main_ddrmc_main_bank(BLK),dq_nibble[2]);
   //writeIO_DMC(ral_addr_of_seq_dq_nibble3_in_ddrmc_main_ddrmc_main_bank(BLK),dq_nibble[3]);
   //writeIO_DMC(ral_addr_of_seq_dq_nibble4_in_ddrmc_main_ddrmc_main_bank(BLK),dq_nibble[4]);
   //writeIO_DMC(ral_addr_of_seq_dq_nibble5_in_ddrmc_main_ddrmc_main_bank(BLK),dq_nibble[5]);
   //writeIO_DMC(ral_addr_of_seq_dq_nibble6_in_ddrmc_main_ddrmc_main_bank(BLK),dq_nibble[6]);
   //writeIO_DMC(ral_addr_of_seq_dq_nibble7_in_ddrmc_main_ddrmc_main_bank(BLK),dq_nibble[7]);
   //writeIO_DMC(ral_addr_of_seq_dq_nibble8_in_ddrmc_main_ddrmc_main_bank(BLK),dq_nibble[8]);
   //writeIO_DMC(ral_addr_of_seq_dq_nibble9_in_ddrmc_main_ddrmc_main_bank(BLK),dq_nibble[9]);
   //writeIO_DMC(ral_addr_of_seq_dq_nibble10_in_ddrmc_main_ddrmc_main_bank(BLK),dq_nibble[10]);
   //writeIO_DMC(ral_addr_of_seq_dq_nibble11_in_ddrmc_main_ddrmc_main_bank(BLK),dq_nibble[11]);
   //writeIO_DMC(ral_addr_of_seq_dq_nibble12_in_ddrmc_main_ddrmc_main_bank(BLK),dq_nibble[12]);
   //writeIO_DMC(ral_addr_of_seq_dq_nibble13_in_ddrmc_main_ddrmc_main_bank(BLK),dq_nibble[13]);
   //writeIO_DMC(ral_addr_of_seq_dq_nibble14_in_ddrmc_main_ddrmc_main_bank(BLK),dq_nibble[14]);
   //writeIO_DMC(ral_addr_of_seq_dq_nibble15_in_ddrmc_main_ddrmc_main_bank(BLK),dq_nibble[15]);
   //writeIO_DMC(ral_addr_of_seq_dq_nibble16_in_ddrmc_main_ddrmc_main_bank(BLK),dq_nibble[16]);
   //writeIO_DMC(ral_addr_of_seq_dq_nibble17_in_ddrmc_main_ddrmc_main_bank(BLK),dq_nibble[17]);
}
#endif

//#ifndef ENABLE_MICROBLAZE_BFM
#ifndef VNC_DIS_INTR
u32 post_gpio_interrupt_handler () {
   u8 status = 0;
   u32 set_done = 0;

   // Enable NPI register write access
   glbl_pcsr_lock = readIO_DMC(ral_addr_of_pcsr_lock_in_ddrmc_main_ddrmc_main_bank(BLK)); // Record the PCSR lock status
   writeIO_DMC(ral_addr_of_pcsr_lock_in_ddrmc_main_ddrmc_main_bank(BLK), 0xf9e8d7c6);

   // Interrupt decoding
   if(glbl_ipr & (0x1 << (SELF_REF_ENTRY_REQ + IRQ_OFFSET))) {  // Self Refresh Entry
  
#ifndef FIX_EDT_999807
      // Call the service routine
      status = self_ref_entry();
 
      if(status != 0) {
         DEBUG_PRINTF("ERROR: CALIBRATION_MODULE: gpio_interrupt_handler(): Interrupt subroutine FAILED\n");
         glbl_post_cal_error = 1;
         glbl_post_cal_status_self_ref |= (1 << POST_CAL_STATUS_SELF_REF_ENTRY_ERROR);
         writeIO_XRAM(XRAM_CAL_POST_STATUS_SELF_REF, glbl_post_cal_status_self_ref);
         glbl_gpo3 |= ((glbl_post_cal_status_self_ref << GPO3_POST_CAL_SELF_REF_POS) | GPO3_POST_CAL_ERROR);
         writeIO_IOM(IO_GPO3, glbl_gpo3);
      } else {
         // Indicate Done to the DMC
         set_done = (0x1 << SELF_REF_ENTRY_DONE);
      }

   #ifdef LPDDR4
      // Clear the unwanted interrupts that arise due to race conditions
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (CH0_OSC_VAL_RDY + IRQ_OFFSET));
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (CH1_OSC_VAL_RDY + IRQ_OFFSET));
   #endif
#endif

   } else if(glbl_ipr & (0x1 << (SELF_REF_EXIT_REQ + IRQ_OFFSET))) {  // Self Refresh Exit
  
#ifndef FIX_EDT_999807
      // Call the service routine
      status = self_ref_exit();
 
      if(status != 0) {
         DEBUG_PRINTF("ERROR: CALIBRATION_MODULE: gpio_interrupt_handler(): Interrupt subroutine FAILED\n");
         glbl_post_cal_error = 1;
         glbl_post_cal_status_self_ref |= (1 << POST_CAL_STATUS_SELF_REF_EXIT_ERROR);
         writeIO_XRAM(XRAM_CAL_POST_STATUS_SELF_REF, glbl_post_cal_status_self_ref);
         glbl_gpo3 |= ((glbl_post_cal_status_self_ref << GPO3_POST_CAL_SELF_REF_POS) | GPO3_POST_CAL_ERROR);
         writeIO_IOM(IO_GPO3, glbl_gpo3);
      } else {
         // Indicate Done to the DMC
         set_done = (0x1 << SELF_REF_EXIT_DONE);
      }

   #ifdef LPDDR4
      // Clear the unwanted interrupts that arise due to race conditions
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (CH0_OSC_VAL_RDY + IRQ_OFFSET));
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (CH1_OSC_VAL_RDY + IRQ_OFFSET));
   #endif
#endif

   } else if(glbl_ipr & (0x1 << (PMC2UB_SWITCH_FREQ + IRQ_OFFSET))) {  // Frequency switching 
  
      // Call the service routine
      status = ddr_freq_switch();

      if(status != 0) {
         DEBUG_PRINTF("ERROR: CALIBRATION_MODULE: gpio_interrupt_handler(): Interrupt subroutine FAILED\n");
         glbl_post_cal_error = 1;
         glbl_post_cal_status_self_ref |= (1 << POST_CAL_STATUS_FREQ_SWITCH_RESTORE_ERROR);
         writeIO_XRAM(XRAM_CAL_POST_STATUS_SELF_REF, glbl_post_cal_status_self_ref);
         glbl_gpo3 |= ((glbl_post_cal_status_self_ref << GPO3_POST_CAL_SELF_REF_POS) | GPO3_POST_CAL_ERROR);
         writeIO_IOM(IO_GPO3, glbl_gpo3);
      } else {
         // Indicate Done to the PMC
         set_done = (0x1 << FREQ_SWITCH_DONE);
      }

   } else if(glbl_ipr & (0x1 << (PMC2UB_SR_EXIT + IRQ_OFFSET))) {  // PMC to UB Self Refresh Exit (All the use-cases)
  
      // Call the service routine
      status = self_ref_exit();

      if(status != 0) {
         DEBUG_PRINTF("ERROR: CALIBRATION_MODULE: gpio_interrupt_handler(): Interrupt subroutine FAILED\n");
         glbl_post_cal_error = 1;
         glbl_post_cal_status_self_ref |= (1 << POST_CAL_STATUS_SELF_REF_EXIT_ERROR);
         writeIO_XRAM(XRAM_CAL_POST_STATUS_SELF_REF, glbl_post_cal_status_self_ref);
         glbl_gpo3 |= ((glbl_post_cal_status_self_ref << GPO3_POST_CAL_SELF_REF_POS) | GPO3_POST_CAL_ERROR);
         writeIO_IOM(IO_GPO3, glbl_gpo3);
      } else {
         // Indicate Done to the PMC
         set_done = (0x1 << UB2PMC_SR_EXIT_DONE);
      }

   #ifdef LPDDR4
      // Clear the unwanted interrupts that arise due to race conditions
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (CH0_OSC_VAL_RDY + IRQ_OFFSET));
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (CH1_OSC_VAL_RDY + IRQ_OFFSET));
   #endif

   } else if(glbl_ipr & (0x1 << (PMC2UB_SPARE_0 + IRQ_OFFSET))) {  // PMC to UB Interrupt Spare 0
  
#ifdef FIX_EDT_999807
      // Call the service routine
      status = self_ref_entry();

      if(status != 0) {
         DEBUG_PRINTF("ERROR: CALIBRATION_MODULE: gpio_interrupt_handler(): Interrupt subroutine FAILED\n");
         glbl_post_cal_error = 1;
         glbl_post_cal_status_self_ref |= (1 << POST_CAL_STATUS_SELF_REF_ENTRY_ERROR);
         writeIO_XRAM(XRAM_CAL_POST_STATUS_SELF_REF, glbl_post_cal_status_self_ref);
         glbl_gpo3 |= ((glbl_post_cal_status_self_ref << GPO3_POST_CAL_SELF_REF_POS) | GPO3_POST_CAL_ERROR);
         writeIO_IOM(IO_GPO3, glbl_gpo3);
      } else {
         // Indicate Done to the DMC & PMC
         set_done = (0x1 << UB2PMC_SPARE_0_DONE) | (0x1 << SELF_REF_ENTRY_DONE);
      }

   #ifdef LPDDR4
      // Clear the unwanted interrupts that arise due to race conditions
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (CH0_OSC_VAL_RDY + IRQ_OFFSET));
      writeIO_IOM(IO_IRQ_ACK, 0x1 << (CH1_OSC_VAL_RDY + IRQ_OFFSET));
   #endif

#else
      // Call the service routine
      status = intr_spare_0();

      // Indicate Done to the PMC
      set_done = (0x1 << UB2PMC_SPARE_0_DONE);
#endif

   } else if(glbl_ipr & (0x1 << (PMC2UB_SPARE_1 + IRQ_OFFSET))) {  // PMC to UB Interrupt Spare 1
  
      // Call the service routine
      status = intr_spare_1();

      // Indicate Done to the PMC
      set_done = (0x1 << UB2PMC_SPARE_1_DONE);

   } else if(glbl_ipr & (0x1 << (PMC2UB_SPARE_2 + IRQ_OFFSET))) {  // PMC to UB Interrupt Spare 2

      // Call the service routine
      status = intr_spare_2();

      // Indicate Done to the PMC
      set_done = (0x1 << UB2PMC_SPARE_2_DONE);

   }

   // Disable all the Interrupts, especially Alert interrupts (As the interrupt subroutine is completed)
   writeIO_IOM(IO_IRQ_ENABLE, 0x0);

   // Clear all the pending interrupts
   glbl_ipr = 0;

   if(set_done != 0) {
      // Indicate Done to the PMC/DC
      glbl_gpo4 = glbl_gpo4 | set_done;
      writeIO_IOM(IO_GPO4, glbl_gpo4);
      wait_intr_src_clr(); // Stretch for sampling by dmc_clk
      glbl_gpo4 = glbl_gpo4 & ~(set_done);
      writeIO_IOM(IO_GPO4, glbl_gpo4);
   }

   if(glbl_cal_mode_bypass == 1) { // Calibration BYPASS
      // Enable all the Interrupts, except Alert interrupts and DQS Oscillator interrupts
      writeIO_IOM(IO_IRQ_ENABLE, (IRQ_EN & ~IRQ_EN_ALERT & ~IRQ_EN_DQS_OSC));
   } else {
      // Enable all the Interrupts, except Alert interrupts
      writeIO_IOM(IO_IRQ_ENABLE, (IRQ_EN & ~IRQ_EN_ALERT));
   }

   // Lock NPI register write access
   writeIO_DMC(ral_addr_of_pcsr_lock_in_ddrmc_main_ddrmc_main_bank(BLK), (glbl_pcsr_lock ? 0x0 : 0xf9e8d7c6));

   return status;
}
#endif // VNC_DIS_INTR
//#endif //ENABLE_MICROBLAZE_BFM

void l2p_bit_mapper(u32 *pb_dq_bits_31_0, u32 *pb_dq_bits_63_32, u32 *lb_dq_bits_31_0, u32 *lb_dq_bits_63_32) {
	u8 bit;
	for(bit=0; bit<DQ_BITS; bit++) {
		if(bit<32){
			if(glbl_dq_lbit2pbit_map[bit]<32)
				*pb_dq_bits_31_0  = ((((*lb_dq_bits_31_0) >>     bit    )&0x1)<< glbl_dq_lbit2pbit_map[bit]    ) | (*pb_dq_bits_31_0);
			else
				*pb_dq_bits_63_32 = ((((*lb_dq_bits_31_0) >>     bit    )&0x1)<<(glbl_dq_lbit2pbit_map[bit]-32)) | (*pb_dq_bits_63_32);
		} else {
			if(glbl_dq_lbit2pbit_map[bit]<32)
				*pb_dq_bits_31_0  = ((((*lb_dq_bits_63_32)>>    (bit-32))&0x1)<< glbl_dq_lbit2pbit_map[bit]    ) | (*pb_dq_bits_31_0);
			else
				*pb_dq_bits_63_32 = ((((*lb_dq_bits_63_32)>>    (bit-32))&0x1)<<(glbl_dq_lbit2pbit_map[bit]-32)) | (*pb_dq_bits_63_32);

		}

	}
}

void precharge_all (u8 rank) {
   u8 lrank;
#ifdef DDR4
         // Precharge the banks
         writeIO_DMC(ral_addr_of_seq_a_cmd_in_ddrmc_main_ddrmc_main_bank(BLK), MEM_ROW_PREACHARGE);
         writeIO_DMC(ral_addr_of_seq_a_addr_in_ddrmc_main_ddrmc_main_bank(BLK), 0x400); // ALl banks precharge
         writeIO_DMC(ral_addr_of_seq_cnt_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

         //for(rank=0; rank<RANKS; rank++) {
            writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10<<rank); // Selects DDR Mem rank
            writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);

            if(glbl_3ds_en) {
               for(lrank=1; lrank<glbl_3ds_lranks; lrank++) {
                  writeIO_DMC(ral_addr_of_seq_a_cbit_in_ddrmc_main_ddrmc_main_bank(BLK), lrank); // Selects 3DS Logical rank
                  if(lrank >= 4) {
                     writeIO_DMC(ral_addr_of_seq_3ds_in_ddrmc_main_ddrmc_main_bank(BLK), 0x1); // EDT-988720. Select high 4 logical ranks
                  }
                  writeIO_DMC(ral_addr_of_seq_start_in_ddrmc_main_ddrmc_main_bank(BLK), 1);
               }
               writeIO_DMC(ral_addr_of_seq_a_cbit_in_ddrmc_main_ddrmc_main_bank(BLK), 0); // Keep the default value
               writeIO_DMC(ral_addr_of_seq_3ds_in_ddrmc_main_ddrmc_main_bank(BLK), 0x0); // Keep the default value
            }
         //}
      #else
         // Precharge the banks
         //for(rank=0; rank<RANKS; rank++) {
            //writeIO_DMC(ral_addr_of_seq_a_cntrl_in_ddrmc_main_ddrmc_main_bank(BLK), 0x10<<rank); // Selects DDR Mem rank
             //lp4_bank_row_access(rank /*u8 rank*/, PRECHARGE /*u8 cmd_type*/, 0 /*u8 bank_addr*/, 0 /*u16 row_addr*/);
         //}
      #endif

}

void compensate_migration_skews () {
   #if MIGRATION_ENABLE == 1
   u8 ck_migr_bits_map[CK_MIGR_BITS][3] = CK_MIGR_NIBBLE_BIT_DLY;
   for (u8 bit = 0;bit<CK_MIGR_BITS;bit++){
         u32 addr = (ck_migr_bits_map[bit][0] << RIU_NIB_POS) | (ck_migr_bits_map[bit][1] + ODLY0);
	 u32 dly = ck_migr_bits_map[bit][2] * 4 * glbl_rl_dly_qtr[ck_migr_bits_map[bit][0]] / glbl_tck;
         writeIO_RIU(addr, dly);  
   }

   u8 addr_migr_bits_map[ADDR_MIGR_BITS][3] =  ADDR_MIGR_NIBBLE_BIT_DLY;
   for (u8 bit = 0;bit<ADDR_MIGR_BITS;bit++){
         u32 addr = (addr_migr_bits_map[bit][0] << RIU_NIB_POS) | (addr_migr_bits_map[bit][1] + ODLY0);
	 u32 dly = addr_migr_bits_map[bit][2] * 4 * glbl_rl_dly_qtr[addr_migr_bits_map[bit][0]] / glbl_tck;
         writeIO_RIU(addr, dly);  
   }

   u8 bg_migr_bits_map[BG_MIGR_BITS][3] =  BG_MIGR_NIBBLE_BIT_DLY;
   for (u8 bit = 0;bit<BG_MIGR_BITS;bit++){
         u32 addr = (bg_migr_bits_map[bit][0] << RIU_NIB_POS) | (bg_migr_bits_map[bit][1] + ODLY0);
	 u32 dly = bg_migr_bits_map[bit][2] * 4 * glbl_rl_dly_qtr[bg_migr_bits_map[bit][0]] / glbl_tck;
         writeIO_RIU(addr, dly);  
   }

   u8 ba_migr_bits_map[BA_MIGR_BITS][3] =  BA_MIGR_NIBBLE_BIT_DLY;
   for (u8 bit = 0;bit<BA_MIGR_BITS;bit++){
         u32 addr = (ba_migr_bits_map[bit][0] << RIU_NIB_POS) | (ba_migr_bits_map[bit][1] + ODLY0);
	 u32 dly = ba_migr_bits_map[bit][2] * 4 * glbl_rl_dly_qtr[ba_migr_bits_map[bit][0]] / glbl_tck;
         writeIO_RIU(addr, dly);  
   }

   u8 cs_migr_bits_map[CS_MIGR_BITS][3] =  CS_MIGR_NIBBLE_BIT_DLY;
   for (u8 bit = 0;bit<CS_MIGR_BITS;bit++){
         u32 addr = (cs_migr_bits_map[bit][0] << RIU_NIB_POS) | (cs_migr_bits_map[bit][1] + ODLY0);
	 u32 dly = cs_migr_bits_map[bit][2] * 4 * glbl_rl_dly_qtr[cs_migr_bits_map[bit][0]] / glbl_tck;
         writeIO_RIU(addr, dly);  
   } 

   u8 cke_migr_bits_map[CKE_MIGR_BITS][3] =  CKE_MIGR_NIBBLE_BIT_DLY;
   for (u8 bit = 0;bit<CKE_MIGR_BITS;bit++){
         u32 addr = (cke_migr_bits_map[bit][0] << RIU_NIB_POS) | (cke_migr_bits_map[bit][1] + ODLY0);
	 u32 dly = cke_migr_bits_map[bit][2] * 4 * glbl_rl_dly_qtr[cke_migr_bits_map[bit][0]] / glbl_tck;
         writeIO_RIU(addr, dly);  
   } 

   u8 act_migr_bits_map[ACT_MIGR_BITS][3] =  ACT_MIGR_NIBBLE_BIT_DLY;
   for (u8 bit = 0;bit<ACT_MIGR_BITS;bit++){
         u32 addr = (act_migr_bits_map[bit][0] << RIU_NIB_POS) | (act_migr_bits_map[bit][1] + ODLY0);
	 u32 dly = act_migr_bits_map[bit][2] * 4 * glbl_rl_dly_qtr[act_migr_bits_map[bit][0]] / glbl_tck;
         writeIO_RIU(addr, dly);  
   }

   u8 parity_migr_bits_map[PARITY_MIGR_BITS][3] =  PARITY_MIGR_NIBBLE_BIT_DLY;
   for (u8 bit = 0;bit<PARITY_MIGR_BITS;bit++){
         u32 addr = (parity_migr_bits_map[bit][0] << RIU_NIB_POS) | (parity_migr_bits_map[bit][1] + ODLY0);
	 u32 dly = parity_migr_bits_map[bit][2] * 4 * glbl_rl_dly_qtr[parity_migr_bits_map[bit][0]] / glbl_tck;
         writeIO_RIU(addr, dly);  
   }

   u8 odt_migr_bits_map[ODT_MIGR_BITS][3] =  ODT_MIGR_NIBBLE_BIT_DLY;
   for (u8 bit = 0;bit<ODT_MIGR_BITS;bit++){
         u32 addr = (odt_migr_bits_map[bit][0] << RIU_NIB_POS) | (odt_migr_bits_map[bit][1] + ODLY0);
	 u32 dly = odt_migr_bits_map[bit][2] * 4 * glbl_rl_dly_qtr[odt_migr_bits_map[bit][0]] / glbl_tck;
         writeIO_RIU(addr, dly);  
   }

   u8 cid_migr_bits_map[CID_MIGR_BITS][3] =  CID_MIGR_NIBBLE_BIT_DLY;
   for (u8 bit = 0;bit<CID_MIGR_BITS;bit++){
         u32 addr = (cid_migr_bits_map[bit][0] << RIU_NIB_POS) | (cid_migr_bits_map[bit][1] + ODLY0);
	 u32 dly = cid_migr_bits_map[bit][2] * 4 * glbl_rl_dly_qtr[cid_migr_bits_map[bit][0]] / glbl_tck;
         writeIO_RIU(addr, dly);  
   }
#endif
}
