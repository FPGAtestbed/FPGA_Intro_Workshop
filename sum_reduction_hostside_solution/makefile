HOST_EXE := exe
TARGET   := sw_emu # Can be hw/hw_emu/sw_emu
PLATFORM := xilinx_u280_xdma_201920_3 # Must match that in .cfg files

# Host building global settings
CXXFLAGS := -I$(XILINX_XRT)/include/ -I$(XILINX_VIVADO)/include/ -Wall -O3 -std=c++11 -L$(XILINX_XRT)/lib/ -lhostsupport -lpthread -lrt -lstdc++
CXXFLAGS2 := -lxilinxopencl

.PHONY: all
all: host device emconfig

.PHONY: host
host: $(HOST_EXE)

.PHONY: device
device: sum_reduction.xclbin

.PHONY: emconfig
emconfig: emconfig.json

sum_reduction.xclbin: sum_reduction.cpp
	v++ -t $(TARGET) --config design.cfg -c -k sum_reduction -o sum_reduction.xo sum_reduction.cpp
	v++ -t $(TARGET) --config link.cfg -l -o sum_reduction.xclbin sum_reduction.xo

# Building Host
$(HOST_EXE): main.cpp
	g++ $(CXXFLAGS) -o $@ '$<' $(CXXFLAGS2)

emconfig.json:
	emconfigutil --platform $(PLATFORM)

# Cleaning stuff
.PHONY: clean
clean:
	rm -f $(HOST_EXE) *.xo *.xclbin emconfig.json *.xo.compile_summary *.log *.xclbin.link_summary *.xclbin.info
	rm -rf _x .Xil .run .ipcache
