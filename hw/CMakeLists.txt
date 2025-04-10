Cmake_minimum_required(VERSION 3.24)

enable_language(C CXX)

set(CMAKE_C_COMPILER_LAUNCHER ccache)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-O0 -g -DVL_DEBUG")
set(OVERRIDES "" CACHE STRING "Override Verilator Aruguments")

if(POLICY CMP0144)
    cmake_policy(SET CMP0144 NEW)
endif()

project(mudkip)

enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(SIM_HEADER_DIR "${CMAKE_SOURCE_DIR}/include/")
set(VERILATOR_INCLUDE "/usr/local/share/verilator/include/")
set(SVDPI_INCLUDE "/usr/local/share/verilator/include/vltstd/")
set(TB_SRC "${CMAKE_SOURCE_DIR}/src")
set(RTL_DIR "${CMAKE_SOURCE_DIR}/rtl")

file(GLOB_RECURSE
                VERILATOR_HEADER_FILES
                "${VERILATOR_INCLUDE}/*.h"
                "${VERILATOR_INCLUDE}/*.hpp"
)

find_package(verilator)

file(GLOB_RECURSE RTL_FILES "rtl/*.v" "rtl/*.sv")
file(GLOB_RECURSE RTL_PKG_FILES "rtl/*_pkg.sv")
list(REMOVE_ITEM RTL_FILES ${RTL_PKG_FILES})
list(PREPEND RTL_FILES ${RTL_PKG_FILES})

file(GLOB_RECURSE VHD_FILES "rtl/*.vhd" "rtl/*.vhdl")
file(GLOB_RECURSE IP_FILES "rtl/ips/*stub.v")

file(GLOB_RECURSE SIM_HEADER_FILES)
file(GLOB SIM_HEADER_FILES "${CMAKE_SOURCE_DIR}/include/*.h" "${CMAKE_SOURCE_DIR}/include/*.hpp}")
set(SIM_SRC_FILES "${CMAKE_SOURCE_DIR}/src/Sim.cpp")

find_package(GTest REQUIRED)

add_library(sim STATIC "${SIM_SRC_FILES}")
target_include_directories(sim PUBLIC ${SIM_HEADER_DIR} ${VERILATOR_INCLUDE} ${SVDPI_INCLUDE})
target_link_libraries(sim
    PRIVATE
    GTest::gtest_main
    -L"${CMAKE_SOURCE_DIR}/lib"
    "${CMAKE_SOURCE_DIR}/lib/libgmp.so.11"
    "${CMAKE_SOURCE_DIR}/lib/libmpfr.so.4"
    "${CMAKE_SOURCE_DIR}/lib/libIp_floating_point_v7_1_bitacc_cmodel.so"
)

function(verilate_command TARGET_NAME TOP_MODULE)
    verilate(
        ${TARGET_NAME}
        SOURCES ${RTL_FILES}
        TOP_MODULE "${TOP_MODULE}"
        PREFIX "V${TOP_MODULE}"
        TRACE_FST
        TRACE_THREADS 4
        INCLUDE_DIRS ${RTL_DIR}
        VL_DEBUG
        VERILATOR_ARGS ${OVERRIDES}
    )
endfunction()

function(mudkip_verilate TOP_MODULE)
    message ("Verilating ${TOP_MODULE}...")
    file(GLOB ${TOP_MODULE}_files "${CMAKE_SOURCE_DIR}/src/${TOP_MODULE}/testbench.cpp" "${TB_SRC}/*/*.hpp")
    add_executable(${TOP_MODULE} ${${TOP_MODULE}_files})
    target_include_directories(${TOP_MODULE} PUBLIC ${SIM_HEADER_DIR})
    target_link_libraries(${TOP_MODULE} PRIVATE sim)
    verilate_command(${TOP_MODULE} ${TOP_MODULE})
endfunction()

mudkip_verilate("basic_sync_fifo")
mudkip_verilate("spmv_network_op_tb")
mudkip_verilate("spmv_network_tb")
mudkip_verilate("spmv_tb")
mudkip_verilate("fproduct")
mudkip_verilate("vector_ram_tb")
mudkip_verilate("so_rr_arbiter")
