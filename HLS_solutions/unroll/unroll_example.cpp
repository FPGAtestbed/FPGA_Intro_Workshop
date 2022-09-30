#include <math.h>
#define N_VALS 500
#define C_SIZE 20

void log_vec(double vec[C_SIZE], double vec2[C_SIZE])
{
    log_loop:
    for(int i = 0; i < C_SIZE; i++)
    {
        vec2[i] = log(vec[i]);
    }
}

extern "C"
{
    void loop_unroll_kernel(double *A, double *B, double *C, double *out)
    {
#pragma hls interface axis port=A 
#pragma hls interface axis port=B 
#pragma hls interface axis port=out 
#pragma hls interface axis port=C

        double temp[N_VALS];
        double c_temp1[C_SIZE], c_temp2[C_SIZE];
		#pragma HLS array_partition variable=c_temp1 complete
        #pragma HLS array_partition variable=c_temp2 complete

        c_assign_loop:
        for(int i = 0; i < C_SIZE; i++)
        {
            c_temp1[i] = C[i];
        }

        log_vec(c_temp1, c_temp2);

        sum_loop:
        for(int i = 0; i < N_VALS; i++)
        {
            temp[i] = A[i] + B[i];
            temp[i] *= c_temp2[i];
            if(i % 2 != 0)
            {
                temp[i] += temp[i-1];
            }
        }

        output_loop:
        for(int i = 0; i < N_VALS; i++)
        {
            out[i] = temp[i];
        }
    }
}
