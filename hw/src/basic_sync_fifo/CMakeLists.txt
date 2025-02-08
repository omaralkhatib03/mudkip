enable_language(CXX)

xelab("basic_sync_fifo")
file(GLOB basic_sync_fifo_files "${CMAKE_SOURCE_DIR}/src/basic_sync_fifo/testbench.cpp" "${TB_SRC}/*/*.hpp")
add_executable(basic_sync_fifo ${basic_sync_fifo_files} ${RTL_FILES})
add_dependencies(basic_sync_fifo xsimk_basic_sync_fifo)
target_include_directories(xsi PUBLIC "${XSI_HEADER_FILES}")
target_link_libraries(basic_sync_fifo PRIVATE xsi)
