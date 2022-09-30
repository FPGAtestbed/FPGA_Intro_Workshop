#include <math.h>
#define N_VALS 500


void process_A(double *A, double *A_prime)
{
process_A_loop:
    for(int i = 0; i < N_VALS; i++)
    {
        A_prime[i] = log(A[i]);
    }
}

void process_B(double *B, double *B_prime)
{
    double tmp_1, tmp_2;
    tmp_1 = 1;
process_B_loop:
    for(int i = 0; i < N_VALS; i++)
    {
        tmp_2 = B[i];
        B_prime[i] = sqrt(tmp_1 * tmp_2);
        tmp_1 = tmp_2;
    }
}

void v_add(double *B_prime, double *A_prime, double *C)
{
output_loop:
    for(int i = 0; i < N_VALS; i++)
    {
        C[i] = B_prime[i] + A_prime[i];
    }
}

extern "C"
{
    void dataflow_kernel(double *A, double *B, double *out)
    {
#pragma HLS interface m_axi port=A bundle=memblock_1
#pragma HLS interface m_axi port=B bundle=memblock_2
#pragma HLS interface m_axi port=out bundle=memblock_1

        //add a pragma here to declare this function a dataflow region
#pragma HLS dataflow

        // We don't want to alter global memory in place! So we have local
        // arrays to hold the result of processing the input arrays 
       double B_prime[N_VALS], A_prime[N_VALS];

       // convert these to streams to allow data to flow efficiently through your design
       // depth=1 is optional but we can do it because we will only need to store one element
       // at a time as our II is equal to one. 
#pragma HLS stream variable=B_prime type=fifo depth=1
#pragma HLS stream variable=A_prime type=fifo depth=1

        // alternatively replace above double array declarations with
        // hls::stream<double> B_prime, A_prime;
        // Replace all reads/writes to B_prime & A_prime with 
        // A_prime.read(), A_prime.write(value) etc. 
        // and change the types in the function signatures 
        // for process_A/B to hls::stream<double>&

        // We apply some procesing function to the two input arrays, which can 
        // be done in parallel
        process_A(A, A_prime);
        process_B(B, B_prime);

        // Add the resulting arrays together. 
        v_add(B_prime, A_prime, out);
    }
}
