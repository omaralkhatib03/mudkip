create_project base_hw /home/oa321/work/mudkip/vivado/base_hw -part xcvc1902-vsva2197-2MP-e-S
set_property board_part xilinx.com:vck190:part0:3.3 [current_project]
create_bd_design "ext_platform" -mode batch
instantiate_example_design -template xilinx.com:design:ext_platform:1.0 -design ext_platform -options { Clock_Options.VALUE {clk_out1 200.000 0 true} Include_AIE.VALUE true Include_BDC.VALUE false Include_DDR.VALUE true IRQS.VALUE 15}
update_compile_order -fileset sources_1
generate_target all [get_files  /home/oa321/work/mudkip/vivado/base_hw/base_hw.srcs/sources_1/bd/ext_platform/ext_platform.bd]
catch { config_ip_cache -export [get_ips -all ext_platform_axi_intc_0_0] }
catch { config_ip_cache -export [get_ips -all ext_platform_clk_wizard_0_0] }
catch { config_ip_cache -export [get_ips -all ext_platform_cips_noc_0] }
catch { config_ip_cache -export [get_ips -all ext_platform_noc_ddr4_0] }
catch { config_ip_cache -export [get_ips -all ext_platform_proc_sys_reset_0_0] }
catch { config_ip_cache -export [get_ips -all ext_platform_icn_ctrl_0] }
catch { config_ip_cache -export [get_ips -all ext_platform_noc_lpddr4_0] }
export_ip_user_files -of_objects [get_files /home/oa321/work/mudkip/vivado/base_hw/base_hw.srcs/sources_1/bd/ext_platform/ext_platform.bd] -no_script -sync -force -quiet
create_ip_run [get_files -of_objects [get_fileset sources_1] /home/oa321/work/mudkip/vivado/base_hw/base_hw.srcs/sources_1/bd/ext_platform/ext_platform.bd]
launch_runs ext_platform_axi_intc_0_0_synth_1 ext_platform_cips_noc_0_synth_1 ext_platform_clk_wizard_0_0_synth_1 ext_platform_icn_ctrl_0_synth_1 ext_platform_noc_ddr4_0_synth_1 ext_platform_noc_lpddr4_0_synth_1 ext_platform_proc_sys_reset_0_0_synth_1 -jobs 28
wait_on_run ext_platform_axi_intc_0_0_synth_1
wait_on_run ext_platform_cips_noc_0_synth_1
wait_on_run ext_platform_clk_wizard_0_0_synth_1
wait_on_run ext_platform_icn_ctrl_0_synth_1
wait_on_run ext_platform_noc_ddr4_0_synth_1
wait_on_run ext_platform_noc_lpddr4_0_synth_1
wait_on_run ext_platform_proc_sys_reset_0_0_synth_1
export_simulation -of_objects [get_files /home/oa321/work/mudkip/vivado/base_hw/base_hw.srcs/sources_1/bd/ext_platform/ext_platform.bd] -directory /home/oa321/work/mudkip/vivado/base_hw/base_hw.ip_user_files/sim_scripts -ip_user_files_dir /home/oa321/work/mudkip/vivado/base_hw/base_hw.ip_user_files -ipstatic_source_dir /home/oa321/work/mudkip/vivado/base_hw/base_hw.ip_user_files/ipstatic -lib_map_path [list {modelsim=/home/oa321/work/mudkip/vivado/base_hw/base_hw.cache/compile_simlib/modelsim} {questa=/home/oa321/work/mudkip/vivado/base_hw/base_hw.cache/compile_simlib/questa} {xcelium=/home/oa321/work/mudkip/vivado/base_hw/base_hw.cache/compile_simlib/xcelium} {vcs=/home/oa321/work/mudkip/vivado/base_hw/base_hw.cache/compile_simlib/vcs} {riviera=/home/oa321/work/mudkip/vivado/base_hw/base_hw.cache/compile_simlib/riviera}] -use_ip_compiled_libs -force -quiet
set_property platform.name {vitis_exten_hw} [current_project]
set_property pfm_name {xilinx:vck190:vitis_exten_hw:0.0} [get_files -all {/home/oa321/work/mudkip/vivado/base_hw/base_hw.srcs/sources_1/bd/ext_platform/ext_platform.bd}]
set_property platform.uses_pr {false} [current_project]
write_hw_platform -force -file /home/oa321/work/mudkip/vivado/base_hw/base_mudkip_hw.xsa
