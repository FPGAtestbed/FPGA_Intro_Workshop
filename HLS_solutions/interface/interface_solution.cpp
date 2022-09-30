#define N_VALS 500

extern "C"
{
    void v_add_kernel(double *A, double *B, double *out)
    {
#pragma HLS interface m_axi port=A bundle=memblock1
#pragma HLS interface m_axi port=B bundle=memblock2
#pragma HLS interface m_axi port=out bundle=memblock1

        for(int i = 0; i < N_VALS; i++)
        {
            out[i] = A[i] + B[i];
        }
    }
}
