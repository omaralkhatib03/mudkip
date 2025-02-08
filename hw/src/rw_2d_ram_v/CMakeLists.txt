enable_language(CXX)

xelab("rw_2d_ram_v")
file(GLOB rw_2d_ram_v_files "${CMAKE_SOURCE_DIR}/src/rw_2d_ram_v/testbench.cpp" "${TB_SRC}/*/*.hpp")
add_executable(rw_2d_ram_v ${rw_2d_ram_v_files} ${RTL_FILES})
add_dependencies(rw_2d_ram_v xsimk_rw_2d_ram_v)
target_include_directories(xsi PUBLIC "${XSI_HEADER_FILES}")
target_link_libraries(rw_2d_ram_v PRIVATE xsi)
