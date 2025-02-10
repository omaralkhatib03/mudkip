# Define directories
if { $argc != 1 } {
    puts "The script requires the project file and the directory of the ip's (*.xci)"
    puts "Please try again."
    exit
} 

set ip_dir [lindex $argv 0]

create_project -in_memory

########################################################
#################### ADD IP's Here #####################
########################################################

if { [file isdirectory "$ip_dir/fp_add_s"] == 0 } {
  create_ip -name floating_point -vendor xilinx.com -library ip -version 7.1 -module_name fp_add_s -dir $ip_dir 

  set_property -dict [list \
    CONFIG.A_Precision_Type {Single} \
    CONFIG.Add_Sub_Value {Add} \
    CONFIG.Axi_Optimize_Goal {Performance} \
    CONFIG.C_A_Exponent_Width {8} \
    CONFIG.C_A_Fraction_Width {24} \
    CONFIG.C_Has_OVERFLOW {false} \
    CONFIG.C_Has_UNDERFLOW {false} \
    CONFIG.C_Latency {5} \
    CONFIG.C_Mult_Usage {Primitive_Usage} \
    CONFIG.C_Optimization {Low_Latency} \
    CONFIG.C_Rate {1} \
    CONFIG.C_Result_Exponent_Width {8} \
    CONFIG.C_Result_Fraction_Width {24} \
    CONFIG.Has_A_TLAST {true} \
    CONFIG.Has_A_TUSER {false} \
    CONFIG.Has_B_TLAST {true} \
    CONFIG.Has_OPERATION_TLAST {false} \
    CONFIG.Maximum_Latency {false} \
    CONFIG.Operation_Type {Add_Subtract} \
    CONFIG.RESULT_TLAST_Behv {AND_all_TLASTs} \
    CONFIG.Result_Precision_Type {Single} \
  ] [get_ips fp_add_s]
}

########################################################
#################### Generate IP's #####################
#################### DO NOT CHANGE #####################
########################################################

proc generate_ip {subdir} {
  set fname [file tail $subdir]

  if { $fname == "build" } {
    puts "Skipping build" 
    return
  }  

  set ip_file [glob "$subdir/$fname.xci"]
  puts "$ip_file"

  if { [file exist $ip_file] != 1} {
    puts "Could not find $ip_file, but $subdir exists"
    return 
  }

  read_ip "$ip_file"

  set locked [get_property IS_LOCKED [get_ips $fname]]
  set upgrade [get_property UPGRADE_VERSIONS [get_ips $fname]]
  
  if {$locked && $upgrade != ""} {
    upgrade_ip [get_ips $fname]
  }

  generate_target all [get_ips $fname]
}

foreach subdir [glob -dir $ip_dir -type d *] {
    if {[file isdirectory $subdir]} {
        generate_ip $subdir
    }
}
