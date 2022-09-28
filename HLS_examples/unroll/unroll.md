# Unroll

Loop unrolling means replacing a loop with explicit copies of a command; this uses up more resources but cuts down on loop control overheads. Loops can also be partially unrolled if they are large or of unknown size, so that _N_ loop iterations are replaced by copied commands, and the loop executes fewer times (by a factor of _n_). 

`#pragma hls unroll factor=<number> [region] [skip_exit_check]`

Loop unrolling is usually done automatically for small loops, and for loops inside pipelined loops. 

Unrolled loops can also be useful by allowing elements to be processed in parallel starting on the same cycle (instead of one after the other as in a pipelined loop) if the memory allows it, or by allowing automatic re-ordering of operations into more efficient structures (see exercise). 

## Options

The import option here is `factor`, which specifies how many iterations of the loop to unroll. For example a loop which has 100 iterations which uses `hls unroll factor=5` will undergo 20 iterations, with five copies of the instructions in each iteration. If `factor` is not specified then it is completely unrolled. 

`region` tells the compiler to unroll loops appearing inside the region, but not the loop that the pragma is itself inside of. If `region` is not used then the loop that the pragma is inside of is unrolled. 

`skip_exit_check` is used for partially unrolled loops. The exit check of a partially unrolled loop checks that we haven't gone outside our original number of iterations: for example, if we unroll a loop with 9 iterations with factor 5, we have two iterations each with 5 copies of the instructions. This means 10 executions overall if we execute naively: the exit check makes sure that we don't execute the final instruction set. **Do not use skip_exit_check if you are not certain that your unroll factor is an exact divisor of your iteration count.**

## Exercise

Take a look at `unroll_example.cpp`. There are a few things going on here! The vector `C` is transferred to local, paritioned memory you have random access and can read/write all elements concurrently. 
```
        c_assign_loop:
        for(int i = 0; i < C_SIZE; i++)
        {
            c_temp1[i] = C[i];
        }
```

A function is then called to process this vector (in this case, just taking logs).
```
void log_vec(double vec[C_SIZE], double vec2[C_SIZE])
{
    log_loop:
    for(int i = 0; i < C_SIZE; i++)
    {
        vec2[i] = log(vec[i]);
    }
}
```
This will be automatically pipelined, so the total time will be approximately the time for a log (~10 cycles) plus the number of elements (20). 

1. Run C-Synthesis on the example and note the latency. The latency of the log_loop should be ~30 cycles. Let's start by improving this. Our data is fully partitioned, so we should be able to update all of them in parallel. Add an `unroll` pragma inside the log loop to fully unroll it. Re-run the C-synthesis; latency should have reduced by about 20 cycles. To see the effect, check the scheduler for the top level function. You should now see all the log operations being executed in parallel with no loop! This is useful for creating small vector processing routines. 

The bigger problem is the summation loop, which has some inter-loop dependency. 
```
sum_loop:
        for(int i = 0; i < N_VALS; i++)
        {
            temp[i] = A[i] + B[i];
            temp[i] *= c_temp2[i % C_SIZE];
            if(i % 2 != 0)
            {
                temp[i] += temp[i-1];
            }
        }
```
(You can ignore the reference to `c_temp2` now -- we just need the values to impact the output otherwise the whole calculation of those log values will be optimised away!)

Odd numbered iterations in this loop have dependency on the previous loop iteration, but even numbered iterations can all be calculated independently. This pushes up the latency for even iterations more than is necessary. 

2. Check the initiation interval for the sum_loop. We can improve this by partially unrolling the loop. If each iteration processes an even and an odd instance, then the even value calculations which are independent can be moved to reduce the latency. Add a partial loop unroll pragma inside the sum loop to process two elements at a time, and re-run the C-Synthesis. Check the new initiation interval and see how the scheduling has changed inside the loop! 

3. If you're interested in resource usage, right click on the top level function in the synthesis report and select Synthesis Details Report, and look at the Utilisation Estimates. Compare this between the version with the pragmas removed or commented out to see how much extra you are using to get the efficiency boost. 