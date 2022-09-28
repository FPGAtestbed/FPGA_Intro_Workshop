#include "v_add.h"

void v_add_kernel(double *A, double *B, double *OUT)
{
    #pragma HLS interface m_axi depth=10 port=A bundle=memblock1
    #pragma HLS interface m_axi depth=10 port=B bundle=memblock2
    #pragma HLS interface m_axi depth=10 port=OUT bundle=memblock1
    for (int i = 0; i < N_VALS; i++)
    {
        OUT[i] = A[i] + B[i];
    }
}
