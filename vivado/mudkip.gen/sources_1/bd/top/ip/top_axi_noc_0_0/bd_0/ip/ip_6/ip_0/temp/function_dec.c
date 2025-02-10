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
//       Revision:       $Id: //depot/icm/proj/everest/7t_n1/shadow/shadowbranches/hsm/hsm.hsm_2.4/rtl/ddrmc_7t_n1/microblaze/cal_sw/src/function_dec.c#1 $
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

u8 ddr_cal();
u8 ddr_self_calibrate(u8 power_on_reset);
u8 ddr_init_f0();
u8 ddr_init_f1();
u8 ddr_ca_cal();
u8 ddr_ca_vref_cal(u8 pattern_type);
void set_ca_vref(u8 ca_group_idx, u8 vref_val); 
u8 ddr_dqs_gate(u8 restore_cal);
u8 ddr_rd_cal(u8 compare_pattern);
u8 ddr_rd_dbi_cal();
u8 ddr_rd_vref_cal(u8 pattern_type);
u8 ddr_wrlvl();
u8 ddr_write_dqs_to_dq();
u8 ddr_write_complex();
u8 ddr_write_latency_cal();
u8 ddr_write_vref();
u8 ddr_write_prbs();
u8 ddr_en_vtc();
u8 ddr_read_dqs_track();
u8 lp4_wr_dqs_track(u8 loop_cnt);
u8 cal_freq_switch();
u8 lrdimm_db_mrep();
u8 lrdimm_db_dwl();
u8 lrdimm_db_mrd_cycle();
u8 lrdimm_db_mrd_center();
u8 lrdimm_db_mwd_cycle();
u8 lrdimm_db_mwd_center();

u8 set_globals();
u8 set_freq_globals();
void set_defaults(u8 power_on_reset);
void dummy_write_read();
void dummy_write_multi_rank_switch(u8 rank_phy);
u8 pll_clkoutphy_en(u8 power_on_reset);
u8 pll_clkoutphy_dis();
void bitslice_reset_bcast(u8 bcast_nibble_mask, u8 value);
u8 self_ref_entry();
u8 self_ref_exit();
u8 ddr_freq_switch();
u8 intr_bs_reset(u8 channel);
u8 intr_alert_par_err(u8 channel);
u8 post_cal();
u8 restore_mini_cal();
u8 ddr_restore_val();
u8 ddr_save_val();
u8 lp4_dqs_osc_read(u8 rank, u8 save_val);
u8 intr_lp4_wr_dqs_track (u8 channel);
u8 intr_lp4_dqs_osc_read (u8 rank, u8 channel);
u8 intr_spare_0();
u8 intr_spare_1();
u8 intr_spare_2();

u32 readIO_DMC(u32 ioAddr); 
void writeIO_DMC(u32 ioAddr, u32 ioData); 
u32 readIO_RIU(u32 ioAddr); 
void writeIO_RIU(u32 ioAddr, u32 ioData); 
u32 readIO_XRAM(u32 ioAddr); 
void writeIO_XRAM(u32 ioAddr, u32 ioData); 
u32 readIO_IOM(u32 ioAddr); 
void writeIO_IOM(u32 ioAddr, u32 ioData); 
void riu_reg_bcast(u32 reg_addr, u8 mask_reg_num, u32 bcast_reg_val); // Broadcasts the riu reg write command to the selected nibbles. 
u16 read_riu_nibble_reg(u8 nibble,u8 reg_addr); // Read RIU register data from a given nibble, physical nibble address.
void write_riu_nibble_reg(u8 nibble,u8 reg_addr, u16 data); // Write RIU register data to a given nibble, physical nibble address.
void read_mod_write_riu_nibble_reg(u8 nibble,u8 reg_addr, u16 data, u8 mask_pos, u16 mask); // Reads a RIU register data and modify it based mask and new value, and writes it back.
void dq_delay_map(u16 *delay_map, u32 delay_offset);
void update_dly_mask (u32 *status, u32 *nibble_mask);
void update_byte_dly_mask (u32 *status, u32 *nibble_mask);
void update_nibble_dly_mask (u32 *status, u32 *nibble_mask);
void single_write_read(u8 rank, u8 extend, u8 pattern);
void ddr_initialize_seq ();
void map_l2p(u8 bits_riu_map[][2], u8 bits, u8 *lnibble2pnibble_map, u8 *lbit2pnibble_map, u8 *first_nibble, u8 *last_nibble, u32 *nibble_mask); 
void set_nibble2bcast_bits(u8 set_initial_mask, u8 bits); 
void calculate_valid_win(u8 lines, int win_type, u8 step_size, u8 edge_type); 
void map_l2p_mask(u8 total_lines, u8 compare_edge_type); 
void read_compared_status ( int compare_mode, int compare_edge,  u8 w_new_samples, u8 pattern_type); 
u8 read_status( int compare_mode, int compare_edge); 
u8 region_detect(); 
u8 update_delay_sequential( int delay_type, int inc_dec_load);
void update_rd_path_delay();
void get_riu_map (u32 *nibble_reset_mask, u8 *map_riu);
void delay2riu_reg_addres_map(u8 bits_riu_map[][2], u8 bits, u16 *delay_map, u32 delay_offset); 
u8 get_read_latency(); 
u8 get_write_latency(); 
void set_1_seq_command_at_mem(u8 rank, u32 nos_iteration_of_seqs, u32 seq_a_cmd, u32 seq_a_addr, u8 seq_a_bg, u16 seq_a_ba, u32 seq_a2a_dly);
void initialize_u32_array (u32 *array_start_addr, u8 array_size, u32 init_value); 
void initialize_u8_array (u8 *array_start_addr, u8 array_size, u8 init_value); 
void inc_u8_array (u8 *array_start_addr, u8 array_size, u32 mask, u8 step_size); 
void center_align (u8 first_element, u8 last_element, u32 elements_mask, u32 delay_offset,u8 rank,u8 *window_width); 
void initialize_u16_array (u16 *array_start_addr, u8 array_size, u16 init_value); 
void lp4_mode_reg_wr(u8 rank, u8 mode_reg, u8 reg_data); 
void lp4_mode_reg_rd(u8 rank, u8 mode_reg);
void lp4_mpc_wr(u8 rank, u32 opcode, u8 cas2, u8 wr_rd);
void lp4_bank_row_access(u8 rank, u8 cmd_type, u8 bank_addr, u16 row_addr); 
void lp4_bank_col_access(u32 nos_iteration_of_seqs, u8 cmd_a_type, u8 cmd_b_type, u8 bank_addr, u16 col_addr); 
void dm_delay_map(u16 *delay_map, u32 delay_offset); 
void set_2_seq_command_at_mem(u8 rank, u32 nos_iteration_of_seqs, u8 full_write, u32 seq_a_cmd, u32 seq_a_addr, u8 seq_a_bg, u16 seq_a_ba, u32 seq_b_cmd, u32 seq_b_addr, u8 seq_b_bg, u16 seq_b_ba, u32 seq_a2b_dly, u32 seq_b2a_dly);
void dqs_delay_map(u16 *delay_map, u32 delay_offset); 
void read_ca_compared_status ( int compare_mode, u8 w_new_samples); 
void xsdb_dbg(u8 index, u16 address, u16 value);
void write_err_reg(u8 code, u8 nibble, u8 bit); 
void write_warning_reg(u16 code, u8 nibble); 
void cplx_write_sample(u8 rank);
void prbs_write_sample();
void simple_write_sample(u8 first_iteration);
u8 update_ca_delay(u8 delay_val); 
int get_cplx_centre (u16 *left_margin, u16 *right_margin);
int get_noise_left_edge(u8 rank, u8 step_size, u16 *right_edge);
int get_dq_left_shift(u16 *left_margin);
int get_dq_common_valid_window(u16 *dqs_delay, u16 *dq_delay);
int get_dq_per_bit_deskew(u16 *dq_delay, u8 step_size);
int get_dbi_common_valid_window(u16 *dqs_delay, u16 *dbi_delay);
int get_dbi_bit_deskew(u16 *dbi_delay, u8 step_size);
int get_delay_calculation(u16 *left_margin, u16 *dq_delay, u16 *dbi_delay);
int get_right_edge(u16 *dqs_right_delay, u8 step_size, u8 pattern_type);
int get_centre(u16 *dqs_delay, u16 *dq_delay, u16 *dbi_delay, u16 *left_margin, u16 *dqs_right_delay);
int get_wr_center_sanity(u8 rank, u8 complex_en);
int get_noise_right_edge (u8 rank, u8 step_size,u8 pattern_type, u16 *valid_width);
void set_write_vref(u8 vref_value, u8 rank);
u8 banks_refresh_gt(u8 dqs_rank); 
u8 banks_refresh(u8 dqs_rank, u8 ref_stage); 
void set_timing(); 
int get_lp4_noise_left_edge (u8 rank, u8 step_size, u8 pattern_type, u16 *valid_width);
int get_lp4_cplx_centre (u8 rank, u16 *left_margin, u16 *right_margin);

void ddr_mrs7_write (u8 rank_rcd, u32 value);
void set_data_bs_rst_mask(); 
u8 sanity_check_1();
u8 sanity_check_2();
u8 sanity_check_ddr();
void wait_intr_src_clr();
void wait_idle_heartbeat();

#ifdef ENABLE_MICROBLAZE_BFM
u32 gpio_interrupt_handler (); 
u32 post_gpio_interrupt_handler (); 
#else
u32 gpio_interrupt_handler (XIOModule *IOModule); 
u32 post_gpio_interrupt_handler (); 
#endif
int get_noise_right_edge (u8 rank, u8 step_size, u8 pattern_type, u16 *valid_width);
int get_lp4_dq_per_bit_deskew (u8 step_size);
int get_lp4_centering(u8 rank, u16 *valid_width);
int get_lp4_dbi_bit_deskew (u8 step_size);
u8 get_write_window (u8 rank, u8 pattern_type, u16 *left_width, u16 *right_width);
u8 get_write_window_margin (u8 rank, u8 pattern_type, u16 *left_width, u16 *right_width);
u32 readIO_FABRIC(u32 ioAddr);
void release_ca_cs_training(u8 rank);
void setup_ca_cal(u8 rank);
void writeIO_FABRIC(u32 ioAddr, u32 ioData);
u8 cal_restore ();
void ddr_mrs_write (u8 rank, u32 mrx, u32 value) ;
void ddr_mrs7_write_opt (u32 value);
void l2p_bit_mapper(u32 *pb_dq_bits_31_0, u32 *pb_dq_bits_63_32, u32 *lb_dq_bits_31_0, u32 *lb_dq_bits_63_32);
u32 get_cal_stage_time ();
u8 sanity_check_complex_read (u32 loop_cnt);
u8 sanity_check_complex_write (u32 loop_cnt);
u8 sanity_check_prbs_read (u32 loop_cnt);
u8 sanity_check_prbs_write (u32 loop_cnt);
void writeIO_MBDR(u32 ioAddr, u32 ioData);
int get_lp4_dq_dbi_region(u8 data_type, u8 region_type, u8 rank, u8 *coarse, u8 *fine, u8 step_size, u8 inc_dec);
int update_dq_dbi_delay (u32 odelay_per_byte_flag, u8 *fine_delay, u8 *coarse_delay, u8 inc_dec, u8 step_size, u8 rank );

u8 mpr_read(u8 rank, u8 page, u8 loc);
u8 temp_control_ref_rate();
u32 l2p_nibble_status (u32 log_nib_status);
void compensate_migration_skews () ;
void read_dbi_settings(u8 set);
