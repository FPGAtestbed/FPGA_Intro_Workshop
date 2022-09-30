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
            // We can try some different HLS pipeline pragmas to get rid of II violation
            // #pragma HLS pipeline II=4 
            //or #pragma HLS pipeline II=3 rewind
            //but the automatic unroll actually appears to work best here!

            sum += (A[i] / B[i]);
            out[i] = sum;
        }
    }
}
