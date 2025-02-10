Cmake_minimum_required(VERSION 3.24)

enable_language(C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-O0 -g")
set(CMAKE_EXE_LINKER_FLAGS "-ldl -lrt")

project(Layth)

set(XSI_HEADER_FILES "${CMAKE_SOURCE_DIR}/include/")
set(TB_SRC "${CMAKE_SOURCE_DIR}/src")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FindXSimConf.cmake)

file(GLOB XVL_SRC_FILES "${CMAKE_SOURCE_DIR}/src/*") 
file(GLOB_RECURSE RTL_FILES "rtl/*.v" "rtl/*.sv")
file(GLOB_RECURSE VHD_FILES "rtl/*.vhd" "rtl/*.vhdl")
file(GLOB_RECURSE IP_FILES "rtl/ips/*stub.v")

# ram_naive_16_1024_1r1w.vdb

add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/xsim.dir/work/*.vdb
    COMMAND xvhdl --2008 --relax -incr -nolog ${VHD_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS ${VHD_FILES}
    COMMENT "Running xvhdl..."
)

add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/xsim.dir
    COMMAND xvlog -incr -L hbm_v1_0_16 -L fifo_generator_v13_2_12 -L blk_mem_gen_v8_4_9 --relax -nolog -sv ${RTL_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS ${RTL_FILES}
    COMMENT "Running xvlog..."
)

add_library(xsi OBJECT "${XVL_SRC_FILES}")
target_include_directories(xsi PUBLIC "${xSIM_HEADER_FILES}" "${XSI_HEADER_FILES}")
target_link_libraries(xsi PUBLIC "${XILINX_LIB}/libxv_simulator_kernel.so")

include(GetGtest.cmake)

add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/xsim.dir/work
    COMMAND xvlog -incr -L hbm_v1_0_16 -L fifo_generator_v13_2_12 -L blk_mem_gen_v8_4_9 --relax -nolog -sv ${IP_FILES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS ${IP_FILES}
    COMMENT "Compiling Ips..."
)

add_custom_target(ips ALL DEPENDS ${CMAKE_SOURCE_DIR}/xsim.dir/work)
add_custom_target(xvhdl ALL DEPENDS ${CMAKE_SOURCE_DIR}/xsim.dir/work/*.vdb)
add_custom_target(xvlog ALL DEPENDS ${CMAKE_SOURCE_DIR}/xsim.dir)

function(xelab TOP_MODULE)
    message("Created Target: ${CMAKE_SOURCE_DIR}/xsim.dir/work.${TOP_MODULE}/xsimk.so")

    add_custom_command (
        OUTPUT ${CMAKE_SOURCE_DIR}/xsim.dir/work.${TOP_MODULE}/xsimk.so
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND xelab --relax -L work -debug typical -dll -nolog ${TOP_MODULE} -L fifo_generator_v13_2_12 -L blk_mem_gen_v8_4_9 -L xbip_utils_v3_0_14 -L axi_utils_v2_0_10 -L xbip_pipe_v3_0_10 -L xbip_dsp48_wrapper_v3_0_6 -L mult_gen_v12_0_22 -L floating_point_v7_1_19 -L blk_mem_gen_v8_4_9 -L unisims_ver -L unimacro_ver -L secureip -L xpm --snapshot work.${TOP_MODULE}
        DEPENDS ${RTL_FILES} ${VHD_FILES}
        COMMENT "Running xelab for ${TOP_MODULE}"
    )
    add_custom_target (xsimk_${TOP_MODULE} ALL DEPENDS ${CMAKE_SOURCE_DIR}/xsim.dir/work.${TOP_MODULE}/xsimk.so)
    add_dependencies (xsimk_${TOP_MODULE} ips)
    add_dependencies (xsimk_${TOP_MODULE} xvhdl)
    add_dependencies (xsimk_${TOP_MODULE} xvlog)
endfunction()


add_subdirectory(src/vector_adder)
add_subdirectory(src/basic_sync_fifo)
# add_subdirectory(src/bfly_processor)
add_subdirectory(src/fl_vadd)

file(GLOB_RECURSE RTL_FILES ${CMAKE_SOURCE_DIR} "rtl/*")
file(GLOB top_files "./src/top/testbench.cpp" "${TB_SRC}/*/*.hpp")
xelab("top")
add_executable(top ${top_files})
add_dependencies(top xsimk_top)
target_include_directories(xsi PUBLIC "${XSI_HEADER_FILES}")
target_link_libraries(top PRIVATE xsi)



