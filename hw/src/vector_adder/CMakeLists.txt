enable_language(CXX)


xelab("vector_adder_tb")
file(GLOB vector_adder_files "${CMAKE_SOURCE_DIR}/src/vector_adder/testbench.cpp" "${TB_SRC}/*/*.hpp")
add_executable(vector_adder ${vector_adder_files} ${RTL_FILES})
add_dependencies(vector_adder xsimk_vector_adder_tb)
target_include_directories(xsi PUBLIC "${XSI_HEADER_FILES}")
target_link_libraries(vector_adder PRIVATE xsi dl rt)
