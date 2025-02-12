Cmake_minimum_required(VERSION 3.24)

enable_language(C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-O0 -g -DVL_DEBUG")

if(POLICY CMP0144)
  cmake_policy(SET CMP0144 NEW)
endif()

project(mudkip)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(SIM_HEADER_DIR "${CMAKE_SOURCE_DIR}/include/")
set(VERILATOR_INCLUDE "/usr/local/share/verilator/include/")
set(TB_SRC "${CMAKE_SOURCE_DIR}/src")
set(RTL_DIR "${CMAKE_SOURCE_DIR}/rtl")

file(GLOB_RECURSE
        VERILATOR_HEADER_FILES 
        "${VERILATOR_INCLUDE}/*.h"
        "${VERILATOR_INCLUDE}/*.hpp"
)

find_package(verilator)

file(GLOB_RECURSE RTL_FILES "rtl/*.v" "rtl/*.sv")
file(GLOB_RECURSE VHD_FILES "rtl/*.vhd" "rtl/*.vhdl")
file(GLOB_RECURSE IP_FILES "rtl/ips/*stub.v")

file(GLOB_RECURSE SIM_HEADER_FILES )
file(GLOB SIM_SRC_FILES "${CMAKE_SOURCE_DIR}/src/*") 
file(GLOB SIM_HEADER_FILES "${CMAKE_SOURCE_DIR}/include/*.h" "${CMAKE_SOURCE_DIR}/include/*.hpp}")

add_library(sim OBJECT "${SIM_SRC_FILES}")
target_include_directories(sim PUBLIC ${SIM_HEADER_DIR} ${VERILATOR_INCLUDE})

function(mudkip_verilate TOP_MODULE)
  
  message ("Verilating ${TOP_MODULE}...")
  file(GLOB ${TOP_MODULE}_files "${CMAKE_SOURCE_DIR}/src/${TOP_MODULE}/testbench.cpp" "${TB_SRC}/*/*.hpp")
  add_executable(${TOP_MODULE} ${${TOP_MODULE}_files})
  target_include_directories(${TOP_MODULE} PUBLIC ${SIM_HEADER_DIR})
  target_link_libraries(${TOP_MODULE} PRIVATE sim)

  verilate(
    ${TOP_MODULE} 
    SOURCES ${RTL_FILES} 
    TOP_MODULE "${TOP_MODULE}" 
    PREFIX "V${TOP_MODULE}" 
    TRACE_FST 
    TRACE_THREADS 2
    INCLUDE_DIRS ${RTL_DIR}
    VL_DEBUG
  )

endfunction()

mudkip_verilate("basic_sync_fifo")

