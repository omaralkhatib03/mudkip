TARGET=sw_emu
BUILD_DIR=build

CFG_FILE=vsva1902.cfg
PLATFROM?=mudkip 
SYSROOT=/opt/xilinx/SDK/sysroots/cortexa72-cortexa53-xilinx-linux/
ROOTFS=/opt/xilinx/xilinx-versal-common-v2024.1/
INI=xrt.ini

CFLAGS = -Wall -g -I./cupdlp -fPIC -I$(SYSROOT)/usr/include/highs
CFLAGS += -Wno-comment -Wno-unused-variable -Wno-unused-label -Wno-unused-result
CFLAGS += -Wno-unused-but-set-variable
CXXFLAGS += -std=c++11 $(CFLAGS)

SRC_DIR = ./cupdlp
CUPDLP_HDRS = $(wildcard $(SRC_DIR)/*.h) 
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES:.c=.o)))
CFLAGS 	+= -DCUPDLP_CPU=1

LDFLAGS 	+= -L$(BUILD_DIR)
HIGHS_LIBS 	= -lhighs
CUPDLP_LIB 	= -lcupdlp
WRAPPER_A 	= $(BUILD_DIR)/libwrapper_highs.a
PLC_BIN 	= $(BUILD_DIR)/plc

WRAPPER_SRCS_C = mps_lp.c
WRAPPER_SRCS_CPP = wrapper_highs.cpp
WRAPPER_HEADERS = mps_lp.h wrapper_highs.h
WRAPPER_OBJS = $(addprefix $(BUILD_DIR)/, $(WRAPPER_SRCS_C:.c=.o) $(WRAPPER_SRCS_CPP:.cpp=.o))

PLC_SRCS = mps_highs.c
PLC_OBJS = $(addprefix $(BUILD_DIR)/, $(PLC_SRCS:.c=.o))

all: $(BUILD_DIR) $(PLC_BIN) package/sd_card.img  
# all: $(BUILD_DIR) $(PLC_BIN) 

###################################################
############       Compile cuPDLP      ############
###################################################

ifeq ($(DEBUG), 1)
    CFLAGS += -DCUPDLP_DEBUG=1
endif

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(CUPDLP_HDRS) 
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/libcupdlp.a: $(OBJECTS)
	$(AR) rcs $@ $^

$(BUILD_DIR)/%.o: %.c $(wildcard *.h) $(CUPDLP_HDRS) $(BUILD_DIR)/libcupdlp.a
	$(CC) $(CFLAGS) -c $< -o $@ -L$(BUILD_DIR) $(CUPDLP_LIB) $(HIGHS_LIBS)

$(BUILD_DIR)/%.o: %.cpp $(wildcard *.h) $(CUPDLP_HDRS) $(BUILD_DIR)/libcupdlp.a
	$(CXX) $(CFLAGS) -c $< -o $@ -L$(BUILD_DIR) $(CUPDLP_LIB) $(HIGHS_LIBS)

$(WRAPPER_A): $(WRAPPER_OBJS) 
	ar rcs $@ $^

$(PLC_BIN): $(PLC_OBJS) $(WRAPPER_A)
	$(CC) $(CFLAGS) -I/usr/include/xrt -o $@ $^  	\
		$(LDFLAGS) -lwrapper_highs $(HIGHS_LIBS)  	\
		-lOpenCL -lpthread -lstdc++ $(CUPDLP_LIB) 	\
		-lm -lrt -lz -lpthread

###################################################
#############       Vitis Stuff      ##############
###################################################

# Unless specified, use the current directory name as the v++ build target
# vadd.xo: ../../src/vadd.cpp
# 	v++ -c -t $(TARGET) --config $(CFG_FILE) -k vadd -I../../src ../../src/vadd.cpp -o vadd.xo 

$(BUILD_DIR)/hw.xsa: 
	v++ -l -t $(TARGET) --config $(CFG_FILE) -f $(PLATFROM) -o $@ 
	# v++ -l -t $(TARGET) --config $(CFG_FILE) ./vadd.xo -o vadd.xsa

# $(BUILD_DIR)/hw.xsa:
# 	cp $(PLATFORM_REPO_PATHS)/xilinx_vck190_base_202410_1/hw/hw.xsa $(BUILD_DIR)

package/sd_card.img: $(PLC_BIN) $(BUILD_DIR)/emconfig.json $(INI) $(BUILD_DIR)/hw.xsa
	v++ -p -t $(TARGET) --config $(CFG_FILE) $(BUILD_DIR)/hw.xsa 	\
		--package.out_dir package 									\
		--package.rootfs $(ROOTFS)/rootfs.ext4 						\
		--package.sd_file $(ROOTFS)/Image 							\
		--package.sd_file $(INI) 									\
		--package.sd_file $(BUILD_DIR)/emconfig.json 				\
		--package.sd_file $(BUILD_DIR)/plc 							\
		--package.sd_file example/afiro.mps 						\

$(BUILD_DIR)/emconfig.json:
	emconfigutil --platform $(PLATFROM) --nd 1 --od $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $@

.PHONY: clean upload

clean:
	rm -rf build vadd* app.exe *json *csv *log *summary _x package *.json .run .Xil .ipcache *.jou *.xclbin


upload: package/sd_card.img
	cp package/sd_card/BOOT.BIN package/sd_card/boot.scr package/sd_card/Image /media/oa321/boot/ 

	cp package/sd_card/xrt.ini package/sd_card/plc package/sd_card/afiro.mps package/sd_card/a.xclbin /media/oa321/root/home/root/
