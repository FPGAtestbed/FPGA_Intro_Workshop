# Pipeline

Pipelining is one of the most important operations in FPGA acceleration. So important, in fact, that the HLS tools will almost always do this for you automatically. (Vitis HLS will try to automatically pipeline any loops below a given iteration count, by default 64.) You can alter some options for how Vitis handles loops, but by default it will try to pipeline loops if it can, and will aim for an initiation interval of one. 

`#pragma HLS pipeline II=<number> <off> <rewind> style=<value>`

The **initiation interval** is the number of clock cycles between starting to process one element and the next. A perfectly pipelined loop will start on a new element every clock cycle, pushing data through it with no gaps. 

This perfect pipelining is not possible when the processing of the next element is dependent on some part of the previous element's processing being finished. 

The time for a pipelined loop scales linearly with the initiation interval so keeping this as low as possible is important.


## Options

We will focus the `II` option - the initiation interval. By default, this is set to 1; for some loops it is necessary to set it higher. If the `II` is set too low (by default or otherwise) and the compiler can't find a solution with that value then a warning will be output and an higher `II` adopted. 

Occasionally you may also find a **Timing Violation**, or negative slack, in your model. This means that the solution developed may fail as data required may not be reayd in time. Often the solution for this is to increase the `II` so that there is more slack in that part of the model, and then re-think things! 

The other options are:
- `off`: Optional keyword; turns pipelining off in a loop (if for example pipelining happens automatically)
- `rewind`: allows overlap of successive calls to the same loop as though they were combined into one pipeline. (Normally all iterations of the loop would have to completely finish before the loop can be restarted.) Can also make pipelines slightly more efficient.
- `style`: can be `stp` (default), `flp`, or `frp`. `flp` and `frp` are flushable pipelines; if there is a delay for data to read then these will continue to process the data already in them instead of stalling to wait for the remaining data. 

## Exercise

We have altered the simple addition kernel to make the output instead a cumulative sum in `pipeline_example.cpp`. Note in particular the new loop:
```
SUM_LOOP:
        for(int i = 0; i < N_VALS; i++)
        {
            // replace with your pipeline pragma 
            #pragma HLS pipeline off

            sum += (A[i] + B[i]);
            out[i] = sum;
        }
```
Because of the `sum` variable, every loop iteration is now dependent on the previous iteration and must wait for some operations to finish before the next can start. 

1. Run C-Synthesis on this example. To begin with, we have manually block pipelining using `#pragma HLS pipeline off`. You should see that the latency of the loop is the latency of a single iteration multiplied by the number of iterations.  
2. Comment out the `pipeline` pragma to allow some automatic pipelining. Re-run C-Synthesis on the example. What is the initiation interval of `SUM_LOOP`? Look at the schedule viewer for the loop. How long does an addition take? Can some of the additions overlap?
    - N.B. that the **II Violation** just tells you that the desired initiation interval was not achieved, but it is not an error i.e. the solution will still work as long as there are no **Timing Violations** or negative slack. 
3. Add a `pipeline` pragma inside the loop to set the initiation interval to the correct value. Re-run the C-Synthesis to check that the `II Violation` is gone. Beware of Timing Violations if your II is too small!