enable_language(CXX)

xelab("rw_2d_ram")
file(GLOB rw_2d_ram_files "${CMAKE_SOURCE_DIR}/src/rw_2d_ram/testbench.cpp" "${TB_SRC}/*/*.hpp")
add_executable(rw_2d_ram ${rw_2d_ram_files} ${RTL_FILES})
add_dependencies(rw_2d_ram xsimk_rw_2d_ram)
target_include_directories(xsi PUBLIC "${XSI_HEADER_FILES}")
target_link_libraries(rw_2d_ram PRIVATE xsi)
