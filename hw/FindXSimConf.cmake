# Omar Alkhatib

message(NOTICE "Adding Xsim Library files ...")
message(NOTICE "Finding Xsim environment")
message(NOTICE "- Xilinx Home detected at $ENV{XILINX_VIVADO}")

set(XILINX_VIVADO_HOME "$ENV{XILINX_VIVADO}")
set(XILINX_LIB "${XILINX_VIVADO_HOME}/lib/lnx64.o")
set(XILINX_COMMON_INCLUDE "${XILINX_VIVADO_HOME}/include")
set(XILINX_XSIM_INCLUDE "${XILINX_VIVADO}/data/xsim/include")

file(GLOB_RECURSE
        xSIM_HEADER_FILES
        "${XILINX_XSIM_INCLUDE}/*.h"
        # "${XILINX_COMMON_INCLUDE}/*.h"
        "${XILINX_XSIM_INCLUDE}/*.hpp"
        # "${XILINX_COMMON_INCLUDE}/*.hpp"
)

# file (GLOB xSIM_SHARED_LIBRARIES)

message(NOTICE
        "    - Xsim Libraries detected at ${XILINX_LIB}")
message(NOTICE
        "    - Xsim include dir at ${XILINX_XSIM_INCLUDE}")
