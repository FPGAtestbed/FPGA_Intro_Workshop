#include "v_add.h"

void v_add_kernel(double *A, double B[N_VALS], double *OUT)
{
    #pragma HLS interface m_axi port=A bundle=memblock1
    #pragma HLS interface m_axi port=B bundle=memblock2
    #pragma HLS interface m_axi depth=10 port=OUT bundle=memblock1
    for (int i = 0; i < N_VALS; i++)
    {
        OUT[i] = A[i] + B[i];
    }
}
