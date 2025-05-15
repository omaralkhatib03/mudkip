TARGET=sw_emu
BUILD_DIR=build

CFG_FILE=vsva1902.cfg
PLATFROM?=mudkip 
SYSROOT=/home/oa321/work/mudkip/vivado/pfm/mudkip-lin/SDK/sysroots/cortexa72-cortexa53-xilinx-linux
ROOTFS=/home/oa321/work/mudkip/vivado/pfm/mudkip-lin/images/linux
INI=xrt.ini
XSA=/home/oa321/work/mudkip/vivado/pfm/base_mudkip_hw.xsa
XRT_INCLUDE=/opt/xilinx/xrt/include/

CFLAGS = -Wall -g -I./cupdlp -fPIC -I$(VERSAL_HIGHS_HOME)/include/highs
CFLAGS += -Wno-comment -Wno-unused-variable -Wno-unused-label -Wno-unused-result
CFLAGS += -Wno-unused-but-set-variable
CXXFLAGS += -std=c++17 $(CFLAGS)
SRC_DIR = ./src

CUPDLP_DIR = $(SRC_DIR)/cupdlp
CUPDLP_HDRS = $(wildcard $(CUPDLP_DIR)/*.h) 
SOURCES = $(wildcard $(CUPDLP_DIR)/*.c)
OBJECTS = $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES:.c=.o)))
CFLAGS += -DCUPDLP_CPU=1

LDFLAGS 	= -L$(BUILD_DIR)
LDFLAGS 	= -L$(VERSAL_HIGHS_HOME) 
HIGHS_LIBS 	= -lhighs
CUPDLP_LIB 	= -lcupdlp
PLC_BIN 	= $(BUILD_DIR)/plc

WRAPPER_SRCS_C = mps_lp.c
WRAPPER_SRCS_CPP = wrapper_highs.cpp
WRAPPER_HEADERS = mps_lp.h wrapper_highs.h
WRAPPER_OBJS = $(addprefix $(BUILD_DIR)/, $(WRAPPER_SRCS_C:.c=.o) $(WRAPPER_SRCS_CPP:.cpp=.o))

PLC_SRCS = mps_highs.c
PLC_OBJS = $(addprefix $(BUILD_DIR)/, $(PLC_SRCS:.c=.o))

# all: $(BUILD_DIR) $(PLC_BIN) package/sd_card.img  
all: $(BUILD_DIR) $(PLC_BIN) 

###################################################
############       Compile cuPDLP      ############
###################################################

ifeq ($(DEBUG), 1)
	CFLAGS += -DCUPDLP_DEBUG=1
endif

$(BUILD_DIR)/%.o: $(CUPDLP_DIR)/%.c $(CUPDLP_HDRS) $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(wildcard *.h) $(CUPDLP_HDRS) $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -L$(BUILD_DIR) $(CUPDLP_LIB) $(HIGHS_LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(wildcard *.h) $(CUPDLP_HDRS) $(BUILD_DIR)
	$(CXX) $(CFLAGS) -c $< -o $@ -L$(BUILD_DIR) $(CUPDLP_LIB) $(HIGHS_LIBS)

$(PLC_BIN):  $(OBJECTS) $(WRAPPER_OBJS) $(PLC_OBJS)
	$(CC) $(CFLAGS) -o $@ $^  	\
		$(LDFLAGS) $(HIGHS_LIBS)  					\
		-lOpenCL -lpthread -lstdc++ 				\
		-lm -lrt -lz


###################################################
#############       Vitis Stuff      ##############
###################################################

VPP=v++
HLS_DIR=hls
HLS_SOURCES=$(HLS_DIR)/spmv.cc
HLS_HDRS=$(wildcard $(HLS_DIR)/*.hpp)
TB_SRC=$(HLS_DIR)/tb.cpp

SPMV_KERNEL=spmv
TB=tb
CL_TB=cltb
XCLBIN=$(BUILD_DIR)/$(SPMV_KERNEL).xsa
CLTB_SRC=$(HLS_DIR)/cltb.cpp
UTILS=$(BUILD_DIR)/utils.o

$(BUILD_DIR)/$(SPMV_KERNEL).xo: $(HLS_SOURCES) $(HLS_HDRS)
	$(VPP) -c -t $(TARGET) --config $(CFG_FILE) -k $(SPMV_KERNEL) -I$(HLS_DIR) $(HLS_SOURCES) -o $@ 

$(BUILD_DIR)/$(SPMV_KERNEL).o: $(BUILD_DIR) $(HLS_SOURCES)
	$(CXX) -g -I$(HLS_DIR) -c $(HLS_SOURCES) -o $@ 

$(BUILD_DIR)/%.o: hls/%.cpp $(BUILD_DIR) 
	$(CXX) -g -I$(HLS_DIR) -c $< -o $@ 

$(BUILD_DIR)/$(TB): $(UTILS) $(HLS_SOURCES)
	$(CXX) -I$(HLS_DIR) $(TB_SRC) $(UTILS) -o $@ $(HLS_SOURCES)

$(XCLBIN): $(BUILD_DIR)/$(SPMV_KERNEL).xo
	$(VPP) -l -t $(TARGET) --config $(CFG_FILE) -o $@ $< -I$(HLS_DIR) -R2

$(BUILD_DIR)/$(CL_TB): $(CLTB_SRC) $(UTILS)
	$(CXX) -g -I$(HLS_DIR) -I$(XRT_INCLUDE) \
	-lOpenCL -lxrt_coreutil -o $@ $(UTILS) $(CLTB_SRC)

$(BUILD_DIR):
	mkdir -p $@

####################################################
##############        Package        ###############
####################################################

$(BUILD_DIR)/package/sd_card.img: $(PLC_BIN) $(XCLBIN) $(BUILD_DIR)/emconfig.json $(INI)
	v++ -p -t $(TARGET) --platform $(PLATFROM) ./_x/link/int/spmv.xclbin -o $(BUILD_DIR)/kernel.xclbin \
		--package.out_dir $(BUILD_DIR)/package \
		--package.rootfs ${ROOTFS}/rootfs.ext4 \
		--package.image_format=ext4 \
		--package.kernel_image ${ROOTFS}/Image \
		
$(BUILD_DIR)/emconfig.json:
	emconfigutil --platform $(PLATFROM) --nd 1 --od $(BUILD_DIR)

.PHONY: clean upload

log-clean:
	rm -rf *.log 

clean:
	rm -rf vadd* app.exe *json *csv *log *summary _x package *.json .run .Xil .ipcache *.jou *.xclbin

# upload: package/sd_card.img
	# cp package/sd_card/BOOT.BIN package/sd_card/boot.scr package/sd_card/Image /media/oa321/boot/ 
	# cp package/sd_card/xrt.ini package/sd_card/plc package/sd_card/afiro.mps package/sd_card/a.xclbin /media/oa321/root/home/root/
