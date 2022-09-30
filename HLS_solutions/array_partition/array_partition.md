# Array Partition

This pragma breaks up the memory for C array into multiple chunks with separate read/write ports. This consumes more resources, but allows for more parallel access to data, which can help resolve resource limitation issues. This does not affect how the array is referenced in the code itself -- it is still a single array indexed in the normal way in the code, but the compiler will assess which accesses can be done concurrently based on how the memory has been partitioned. 

`#pragma hls array_partition variable=<variable> <type> factor=<number>`

## Options

`variable` just takes the name of the array that you would like to partition. 

`type` has three potential options:
- `complete`: every element gets its own separate port, essentially treating them all as independent variables. This is the default if no type is specified, but is very wasteful if you have large arrays! Only use if you can actually process the entire array concurrently. 
- `cyclic`: elements alternate between pieces of memory. E.g. if there are 3 pieces for array A then A[0] is in the first, A[1] the second, A[2] the third, A[3] the first again and so on. 
- `block`: array is split into equal contiguous chunks, so if there are two blocks then the first half of the array is stored in one block and the second half of the array is in the other. 

`factor` gives the number of pieces you want to divide you memory into. It is mandatory for `cyclic` and `block`, but not used for `complete`. 

## Exercise

Take a look at `array_partition_example.cpp`. It gives a simple matrix multiplication implementation for matrices stored in row-major order (i.e. a 1D array that stores each row of the matrix one after the other). (We won't go into more efficient matrix algorithms, we're just going to focus on parallelising the naive algorithm as an illustrative example!)

The basic inner loop is quite simple:
```
        mult_loop_i:
        for(int i = 0; i < 8; i++)
        {
            mult_loop_j:
            for(int j = 0; j < 8; j++)
            {
                // do all the multiplications
                mult_loop_k:
                for(int k = 0; k < 8; k++)
                {
                    row_col_prod[k] = M1[i*8 + k] * M2[k*8 + j];
                }
                // add the products together to get one element of the matrix product
                M3[i*8 + j] = sum_vec(row_col_prod);
            }
        }
```
N.B. for each element of `M3` we multiply one _row_ of `M1` with one _column_ of `M2`, so we have different access patterns for each input matrix. 

1. Run C-Synthesis and look at the latency and, in particular, the initiation interval of the multiplication loop. (You will only see on multiplication loop because the inner loops automatically unrolled and the sum function inlined!). Open the schedule viewer for this loop (`mult_loop_i_mult_loop_j` in your schedule viewer) and see how many values from `M1` and `M2` can be read in parallel. Compare this to the number of elements in a row and see if the initiation interval makes sense to you!
2. To improve the initiation interval, we can improve parallelism in our data access. Add `array_partition` pragmas underneath the declarations for `M1`, `M2`, and `row_col_prod` so that we do all 8 reads from `M1` and `M2` and writes to `row_col_prod` in parallel. Take into account the access patterns for each variable and don't add more ports than you need to! **N.B.** Remember that the matrix is in Row-Major order. 
3. Re-run C-Synthesis and see how things have improved. Your initiation interval should now be 1, so we've improved the efficiency significantly. The parallel reads and writes are not easy to see in the scheduler, because they actually happen between overlapping loop iterations! 
4. The sum function that we have can also be improved. We're currently limited to do all eight additions sequentially, because we can't add to our total until we've finished the previous addition! This can be improved by parallel summing elements pair-wise. You can do four additions in parallel first, storing the results in an array. Then this array can be summed pair-wise in parallel (two additions) to give two more temporary values, which can finally be summed into your total. In this case, you should be able to do all eight summations in the time it takes to do just three. 
    - Start by declaring some arrays in your sum function to hold the intermediate values 
    - Use `array_partition` to make sure that all the intermediate values can be accessed concurrently! 
    - write one loop to sum the 8 value vector into 4 intermediate values
    - write another loop to sum the 4 intermediate values into two intermediate values
    - finally add your two values to get your final sum
    - Re-run the C-Synthesis! You should see that your loop latency has reduced and the scheduling has changed significantly. We now read entire rows and columns in parallel, perform all our multiplications in parallel, and then perform a three stage reduction. And all of this is still pipelined! 