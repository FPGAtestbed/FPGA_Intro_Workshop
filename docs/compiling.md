# Compiling 

Go into the folder where you want your compilation to happen. I recommend making some kind of build folder! 
Go into the `sum_reduction_example` folder in your terminal and type:
```
mkdir build
cd build
```

We need to create an emulation configuration file like so:
```
emconfigutil --platform xilinx_u280_xdma_201920_3
```
(There are options you can apply to this command, but we won't be getting into those today.)

This automatically generates a lot of information that the compiler needs in order to build for our specific platform. (In this case, the Alveo u280 board.)

Now the **compile** step:
```
v++ -c -t sw_emu --config ../u280.cfg -k sum_kernel -I../include ../kernel_src/sum_kernel.cpp -o sum.xo
```

and once that is finished, you can run the **linking** step:
```
v++ -l -t sw_emu --config ../u280.cfg ./sum.xo -o sum.xclbin
```

In general, you would put all of this - along with the CPU compilation steps - into a `Makefile`, but it's useful to go through the steps individually to see what is happening. 

Once you've done this you'll have a `sum.xclbin` file (among other things). In order to run this, we'll have to write the CPU side code to talk to our kernel. 

**N.B.** When you run kernels in emulation mode you'll need to set a variable called `XCL_EMULATION_MODE` like so:
```
export XCL_EMULATION_MODE=sw_emu
```
Then afterwards you can run your compiled host code and the runtime will know not to try to look for a real device! 
