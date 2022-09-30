#define N_VALS 500

extern "C"
{
    void cumulative_kernel(double *A, double *B, double *out)
    {
#pragma HLS interface m_axi port=A   bundle=memblock_1
#pragma HLS interface m_axi port=B   bundle=memblock_2
#pragma HLS interface m_axi port=out bundle=memblock_1 

        double sum = 0;

SUM_LOOP:
        for(int i = 0; i < N_VALS; i++)
        {
            // replace with your pipeline pragma 
            #pragma HLS pipeline off

            sum += (A[i] / B[i]);
            out[i] = sum;
        }
    }
}
