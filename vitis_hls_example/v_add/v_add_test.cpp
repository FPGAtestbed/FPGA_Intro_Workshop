#include "v_add.h"
#include "math.h"

bool isClose(double x, double y)
{
    return (abs(x-y)/x) < 1e-10;
}

// This is run when C Simulation or C/RTL Co-simulation are called
// Must return 0 for pass; any other return value is a failure (C style return codes) 
int main()
{
    // Declare dummy data for tests 
    double A[N_VALS], B[N_VALS], C[N_VALS];

    // A = i, B = 2i, therefore expect C = 3i after kernel call
    for(int i = 0; i < N_VALS; i++)
    {
        A[i] = i;
        B[i] = i * 2.0;
    }

    // Call the kernel code like a normal C function - no openCL for C simulation
    v_add_kernel(A, B, C);

    // Check C[i] is approximately 3i. Return 0 if all okay, otherwise return 1
    int ret = 0;
    for(int i = 0; i < N_VALS; i++)
    {
        int expect = 3*i;
        if(C[i] != expect)
        {
            ret = 1;
        }
    }

    return ret;
}