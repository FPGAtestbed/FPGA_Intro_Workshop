# Bind Storage Pragma

The `bind_storage` pragma can be used to control the way that variables are stored in memory. 

`#pragma hls bind_storage variable=<variable> type=<type> impl=<implementation>`

This pragma should come after your variable declaration. The `variable` parameter should just be the relevant variable. 

## Options:

Types of storage options are: 
- `auto`: Compiler will decide for you
- `ram_1p`: RAM with single read/write port
- `ram_2p`: RAM with two ports: one read only and one read/write
- `ram_s2p`: RAM with two ports: one read only and one write only
- `ram_t2p`: RAM with two read/write ports 
- `ram_1wnr`: RAM with one write and N read ports (N inferred)
- `rom_1p`: ROM with single port
- `rom_2p`: ROM with dual port
- `rom_np`: ROM with multiple ports
- `fifo`: first in first out

Implementation includes:
- `auto`: the default; the compiler will decide the implementation
- `lutram`: LUT (look-up table) RAM, default for very small data 
- `bram`: Block RAM, uses 18kb RAM blocks distributed through fabric. Default for more substantial arrays. 
- `uram`: Ultra-RAM, larger memory but less localised. 

All three of these have a latency of 1-3 cycles. Selection of implementation won't be important for our minimal examples but may become important as resources have more pressure on them. 

## Exercise

Take a look at `bind_storage_example.cpp`. 
```
double local_var[N_VALS];
#pragma HLS bind_storage variable=local_var type=ram_1p impl=bram
```
Here the `local_var` array is being implemented as RAM with a single read/write port. This supports random access, but can only perform one read/write operation per clock cycle. 

1. Run C-Sythesis on the exampla and note the total latency. You should see that there are `II violations` as the compiler cannot pipeline the loops with an initiation inveral of 1: check the initiation intervals of the two loops `ASSIGN_LOOP` and `OUTPUT_LOOP`. You should see that the reason is `Resource Limitation` -- we are waiting around for memory ports to be free! 
2. The `ASSIGN_LOOP` assigns pairs of values in `local_var`. Change the `type` of storage so that two elements of this array can be written at the same time. Re-run C-Synthesis: how have the initiation intervals and latency changed? 
3. The `OUTPUT_LOOP` reads four values from `local_var` to generate each output. Change the `type` of the storage so that all four of these reads can be done at the same time. Re-run C-Synthesis: how has this affected the initiation intervals and the overall latency?
4. Which storage solution is better? Comment out the pragma and re-run the C-Synthesis to see what the compiler does automatically. How does it compare to your choice?

## Vitis Docs

See https://docs.xilinx.com/r/en-US/ug1399-vitis-hls/pragma-HLS-bind_storage for more information. 