

set(CUPDLP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/cupdlp/")

file(GLOB CUPDLP_INCLUDE_HEADERS
        "${CUPDLP_INCLUDE_DIR}/*.h"
)
file(GLOB CUPDLP_SOURCE_FILES
        "${CUPDLP_INCLUDE_DIR}/*.c"
)

add_library(cupdlp STATIC 
        ${CUPDLP_INCLUDE_HEADERS}
        ${CUPDLP_SOURCE_FILES}
)


target_compile_definitions(cupdlp
        PUBLIC
        # If the debug configuration pass the DEBUG define to the compiler
        $<$<CONFIG:Debug>:-DCUPDLP_DEBUG=1>
)

message(NOTICE "- CPU version PDLP")
target_compile_definitions(cupdlp
        PUBLIC
        -DCUPDLP_CPU=1
)
target_link_libraries(cupdlp m)
