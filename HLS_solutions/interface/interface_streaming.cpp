#define N_VALS 500

extern "C"
{
    void v_add_kernel(double *A, double *B, double *out)
    {
#pragma HLS interface axis port=A 
#pragma HLS interface axis port=B 
#pragma HLS interface axis port=out 

        for(int i = 0; i < N_VALS; i++)
        {
            out[i] = A[i] + B[i];
        }
    }
}
