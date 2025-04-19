#####################################################
#############     Set Up Project    ################
#####################################################

if { $argc != 2 } {
    puts "The script requires the project file and the directory of the ip's (*.xci)"
    puts "Please try again."
    exit
} 

set hw_dir [lindex $argv 0]
set project_dir [lindex $argv 1]

set rtl_dir "$hw_dir/rtl"
set ip_dir "$hw_dir/ips"

create_project mudkip "$project_dir" -part xcvc1902-vsva2197-2MP-e-S 
set_property board_part xilinx.com:vck190:part0:3.3 [current_project]
set_property platform.extensible true [current_project]

#####################################################
#############     Add Ips to Project    #############
####################################################

exec make -C $hw_dir ips

proc add_ip {subdir} {
  set fname [file tail $subdir]

  if { $fname == "build" } {
    puts "Skipping build" 
    return
  }  

  set ip_file [glob "$subdir/$fname.xci"]

  import_ip $ip_file
}

set ip_subdirs [glob -dir $ip_dir -type d *]
puts $ip_subdirs

foreach subdir [glob -dir $ip_dir -type d *] {
    if {[file isdirectory $subdir]} {
        add_ip $subdir
    }
}

#####################################################
#############     Add RTL to Project    #############
#####################################################

foreach subdir [glob -dir $rtl_dir -type d *] {
    if {[file isdirectory $subdir]} {
        foreach file [glob -nocomplain -directory $subdir *.v *.sv *.vhd] {
            add_files $file
        }
    }
}

update_compile_order -fileset sources_1

#####################################################
################      BD Journal     ################
#####################################################

create_bd_design "top"
update_compile_order -fileset sources_1
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:versal_cips:3.4 versal_cips_0
endgroup
startgroup
create_bd_cell -type ip -vlnv xilinx.com:ip:axi_noc:1.1 axi_noc_0
endgroup
create_bd_cell -type module -reference rf_top rf_top_0
apply_bd_automation -rule xilinx.com:bd_rule:cips -config { board_preset {Yes} boot_config {Custom} configure_noc {/axi_noc_0} debug_config {JTAG} design_flow {Full System} mc_type {DDR} num_mc_ddr {1} num_mc_lpddr {None} pl_clocks {1} pl_resets {None}}  [get_bd_cells versal_cips_0]
apply_bd_automation -rule xilinx.com:bd_rule:axi4 -config { Clk_master {/versal_cips_0/fpd_cci_noc_axi0_clk (799 MHz)} Clk_slave {Auto} Clk_xbar {Auto} Master {/versal_cips_0/FPD_CCI_NOC_0} Slave {/rf_top_0/axil_ps_if} ddr_seg {Auto} intc_ip {/axi_noc_0} master_apm {0}}  [get_bd_intf_pins rf_top_0/axil_ps_if]
apply_bd_automation -rule xilinx.com:bd_rule:board -config { Manual_Source {Auto}}  [get_bd_pins rst_versal_cips_0_333M/ext_reset_in]
connect_bd_net [get_bd_pins rst_versal_cips_0_333M/peripheral_aresetn] [get_bd_pins rf_top_0/rst_n]
startgroup
endgroup
validate_bd_design
save_bd_design

#####################################################
##############    None BD Journal    ################
#####################################################

make_wrapper -files [get_files "$project_dir/mudkip.srcs/sources_1/bd/top/top.bd"] -top
add_files -norecurse "$project_dir/mudkip.gen/sources_1/bd/top/hdl/top_wrapper.v"
set_property top top_wrapper [current_fileset]

#generate_target all [get_files "$project_dir/mudkip.srcs/sources_1/bd/top/top.bd"]

#####################################################
##################       Done      ##################
#####################################################

close_project
