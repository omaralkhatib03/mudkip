# Create Project
create_project -in_memory -part xcku5p-ffvb676-2-e

add_files ./rtl/vector_ram/vector_ram.sv
add_files ./rtl/vector_ram/vector_ping_pong.sv
add_files ./rtl/vector_ram/vector_ping_pong_ld_wrapper.sv
add_files ./rtl/spmv/spmv_tb.sv
add_files ./rtl/spmv/spmv_kernel.sv
add_files ./rtl/spmv/spmv_pkg.sv
add_files ./rtl/spmv/spmv_kernel_top.sv
add_files ./rtl/spmv/product.sv
add_files ./rtl/interfaces/axi_stream_if.sv
add_files ./rtl/interfaces/vector_ram_if.sv
add_files ./rtl/interfaces/vector_ram_slave_null.sv
add_files ./rtl/interfaces/vector_ram_master_null.sv

set_property top spmv_tb.sv [current_fileset]

eval "synth_design -mode out_of_context -top spmv_tb -part xcku5p-ffvb676-2-e"

close_project
