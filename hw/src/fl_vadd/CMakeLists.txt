enable_language(CXX)


xelab("fl_vadd_top")
file(GLOB fl_vadd_top_files "${CMAKE_SOURCE_DIR}/src/fl_vadd/testbench.cpp" "${TB_SRC}/*/*.hpp")
add_executable(fl_vadd_top ${fl_vadd_top_files} ${RTL_FILES})
add_dependencies(fl_vadd_top xsimk_fl_vadd_top)
target_include_directories(xsi PUBLIC "${XSI_HEADER_FILES}")
target_link_libraries(fl_vadd_top PRIVATE xsi dl rt "${XILINX_LIB}/libxv_simulator_kernel.so")
