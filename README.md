# Introduction to FPGA Programming Workshop

This repo contains materials for the Introduction to FPGA Programming Workshop. 

In the **docs** folder you'll find presentation slides that you may wish to refer back to, as well as information on connecting to the testbed and compiling your kernels. 

**HLS_examples** contains several sub-folders, each of which has a simple example kernel, and a markdown file containing some optimisation exercises. To do an exercise you'll need to go into the `vitis_hls` app and create a new project for each one. 

**sum_reduction_example** contains an example source file for you to convert into a FPGA compilation ready kernel, and compilation instructions for compiling it for emulation. 

**vitis_hls_example** contains some example source files which we'll use to walk you through the process of creating a Vitis HLS project, and analysing kernel performance. 

## Running order for the day 

1. Introduction & Setup 
2. Intro to FPGAs / Accelerators
3. How to write a Kernel
4. How to compile a Kernel
5. OpenCL and host side code
LUNCH BREAK
6. Intro to vitis_hls: C Simulation, C Synthesis/Scheduler, Co-simulation
7. Intro to HLS pragmas
8. HLS pragma exercises 
9. Wrap up

## Recommended HLS Exercise Order

The HLS exercises are designed to be fairly independent of one another, my recommended order would be:
1. Interface
2. Dataflow
3. Array Partition
4. Bind Storage
5. Unroll

I will go through the Pipeline example with you. 

Feel free to try out optimising your own kernel, such as the sum reduction example. 

## Useful Links

- EPCC FPGA Testbed Website: https://fpga.epcc.ed.ac.uk/
  - You can come here to find out more about the testbed, how to run things on it, and apply for access
- Vitis Documentation (Pragmas): https://docs.xilinx.com/r/en-US/ug1399-vitis-hls/HLS-Pragmas
  - Reference docs for available pragmas if you want to find out more
- Vitis Introductory Tutorials: https://github.com/Xilinx/Vitis-Tutorials
  - Tutorials are various levels for using Xilinx FPGAs with HLS. 
- Vitis Libraries: https://github.com/Xilinx/Vitis_Libraries
  - Useful libraries with some degree of optimisation. Good examples of solutions, but may need tweaking if you use them in projects to fit better with your design. 
