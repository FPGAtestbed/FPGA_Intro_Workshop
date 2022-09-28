# Simple FPGA saxpy Example

This is an extremely minimal example of what is required to run a computational kernel (in this case saxpy) on an FPGA.

## Requirements

On EPCC's FPGA testbed hosted on NextGenIO, you must load the FPGA modules and libraries before building or running the example with a script like:

```bash
#!/usr/bin/env bash

source /home/nx08/shared/fpga/fpga_modules.sh
module load vitis/2021.1
module load vitis_libraries
bash /opt/xilinx/xrt/setup.sh
```

## Building

The host binary, device binary and emconfig are compiled or created separately. When compiling the device code you should specify a target. `sw_emu` takes the shortest amount of time (~30s), `hw` the longest (>1hr). You can compile multiple targets and later choose between them at runtime (see below).

```bash
make host
make device TARGET=<hw/hw_emu/sw_emu>
make emconfig
```

For ease, you can compile host, device and emconfig with a single call to `make TARGET=<...>`.

## Running

The build step above should have created a binary in the main directory call `exe`. Before running this, you must set the environment variable `XCL_EMULATION_MODE` to the required target.

```bash
export XCL_EMULATION_MODE=sw_emu

./exe
```
