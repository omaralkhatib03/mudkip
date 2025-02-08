
# Create Project 
create_project layth ./layth -part xcvc1902-vsva2197-2MP-e-S
set_property board_part xilinx.com:vck190:part0:3.3 [current_project]

set rtl_dir "./rtl"
set rtl_files [exec find -L ./rtl -type f \( -name "*.sv" -o -name "*.v" \)]

puts "RTL files found:"

foreach rtl_file $rtl_files {
    puts $rtl_file
}

foreach rtl_file $rtl_files {
    add_files -norecurse $rtl_file
}

set_property top top [current_fileset -simset]
set_property top top [current_fileset]

close_project
