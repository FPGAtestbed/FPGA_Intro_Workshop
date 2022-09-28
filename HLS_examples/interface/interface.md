# Interface

This pragma is used to define how the solution interfaces to memory external to it i.e. arguments in your top level function. Vitis HLS can do this for you, but sometimes the choices that it makes aren't necessarily the best ones possible. 

This pragma is one of the most complex and has many options which we will not get into, so for a full description you can see the documentation at: https://docs.xilinx.com/r/en-US/ug1399-vitis-hls/pragma-HLS-interface

## Options 
In this example we'll only use these options:

`#pragma HLS interface <mode> port=<variable name> bundle=<string>`

`mode` has a wide range of options, but we'll focus on two: `m_axi` (AXI4 interface, makes the global memory buffers available as RAM with various options for bursting) and `axis` (AXI4-Stream interface, for streaming data through kernels, sequential data access only). 

`port` just takes the name of the argument that you want this interface to apply to. 

`bundle` is essentially the identity of a given interface. Ports with compatible `mode` and `bundle` options are grouped into a single interface: reading / writing to different variables the same port in parallel will be subject to limitations. You can name your `bundle` what you like, but the name should be all lower case! 

## Exercises

The example is very simple, just a kernel to add two vectors together and store the answer in an output vector. 
```
#define N_VALS 500

extern "C"
{
    void v_add_kernel(double *A, double *B, double *out)
    {
        for(int i = 0; i < N_VALS; i++)
        {
            out[i] = A[i] + B[i];
        }
    }
}
```
A perfectly pipelined implementation of this loop should run in approximately 500 cycles, if we process each element one cycle after the previous. (There will also be some additional latency for read and write requests to the global memory.) 

1. Run C-Synthesis on this simple example and note the total latency. There are a few things worth taking note of here:
    - In the `SW-To-HW Mapping` section of the summary you can see that `A`, `B`, and `out` are all mapped to the same interface: `m_axi_gmem`. So we have random access to each of these variables, but in each clock cycle we can perform one read and one write operation (we have one read and one write port). 
    - Open the schedule viewer and look at the top level `v_add_kernel`. You'll see a large operation, taking around 70 cycles, that refers to the write request for `out`. This happens outside the loop, because all the write accesses are sequential, so we can batch them all into one request and send out a single request. 
    - Now open up the `V_ADD_LOOP` in the schedule viewer. Here, we have two read request operations (about 70 cycles) during _every_ loop iteration: this is because we're jumping around in memory between `A` and `B`. These read requests can be pipelined so they don't affect the efficiency of the loop too much, but they are wasted work. It's also good to be aware of such accesses as they can greatly add to latency if you have them showing up in places that can't be pipelined so easily. 
    - You should see an `II Violation` because of a `Resource Limitation` in our `V_ADD_LOOP`. This is because we don't have enough ports to our memory to start a new iteration of the loop every clock cycle! We're being held up by having to wait for two read operations - one for each array - before we can start on the next iteration of the loop, which also needs to read. 

2. One way we can address this problem is to make sure that `A` and `B` are in separate banks with different interfaces. Inside the function, before the loop, add some `hls interface` pragmas to assign `A` and `B` to `m_axi` interfaces with different bundles; you can assign `out` to either bundle. Re-run the C-Synthesis and check the total latency. Take a look at the schedule viewer and check that your read requests are now being batched outside of the loop. (**N.B.** If you want this to work on the hardware, you have to make sure that these arrays are explicitly mapped to separate memory banks in the `connectivity` section of your `cfg` file! If you create separate interfaces for `A` and `B`, but then put both arrays in the same memory bank, then you will still have to wait for sequential accesses.)

3. So far, we've been implementing these arrays as RAM. Since we always read/write these arrays sequentially, we can also implement it using streams!
    - Start by changing your interfaces from `m_axi` to `axis`
    - Remove the `bundle` options -- streams get their own interfaces so it doesn't take a bundle option! (There's no point bundling streams since you can't access them out of order anyway.)
    - Re-run C-Synthesis and note the new latency. If you inspect the scheduler, you should see that lengthy read/write requests are gone! 
    - **Warning**: whenever you use streams you need to be very careful to obey the stream semantics. Generally it's better to use types like `hls::stream` which enforce the semantics; when you use arrays implemented as streams then you should read the values into temp variables if you want to pass them around. 