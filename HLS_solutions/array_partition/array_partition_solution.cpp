#define DATA_TYPE double

double sum_vec(double vec[8])
{
    double sum = 0;

    // buffer variables to do pairwise additions
    // should be completely partitioned to accesses all in parallel
    double buff4[4], buff2[2];
    #pragma HLS array_partition variable = buff4 complete
    #pragma HLS array_partition variable = buff2 complete

    // first set of (parallel) pairwise sums
    sum_vec_loop1:
    for(int i = 0; i < 4; i++)
    {
        buff4[i] = vec[i] + vec[i + 4];
    }

    // second set of (parallel) pairwise sums
    sum_vec_loop2:
    for(int i = 0; i < 2; i++)
    {
        buff2[i] = buff4[i] + buff4[i + 2];
    }

    // final sum
    sum = buff2[0] + buff2[1];

    return sum;
}

extern "C"{
    void matrix_multiply_kernel(DATA_TYPE *A, DATA_TYPE *B, DATA_TYPE *OUT)
    {
// Setting up interfaces to global memory: see interface examples for more info!
#pragma HLS INTERFACE m_axi port=A bundle=gmem1
#pragma HLS INTERFACE m_axi port=B bundle=gmem2
#pragma HLS INTERFACE m_axi port=OUT bundle=gmem1

        double M1[64], M2[64], M3[64];
        // Cyclic partitioning means that for each set of 8 elements, each subsequent element  
        // is in a different memory block. So for each row of M1, each element is in a 
        // different memory block and can be accessed in concurrently
        #pragma HLS array_partition variable=M1 cyclic factor=8

        // Block partitioning means that each row of M2 is a separate block of
        // memory, and so for a given column of M2 each element is in a different 
        // block and can be accessed concurrently
        #pragma HLS array_partition variable=M2 block factor=8

        assign_matrix_loop:
        for(int i = 0; i < 64; i++)
        {
            M1[i] = A[i];
            M2[i] = B[i];
        }

        // A variable to store the results of multiplications (8 multiplications per row-column product)
        double row_col_prod[8];

        // We want complete partitioning here because because during one row-column multiplication
        // (eight individual multiplications) we fill out all eight elements (one element for each 
        // individual multiplication)
        #pragma HLS array_partition variable=row_col_prod complete

        //Trivial implementation of Matrix multiply
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

        output_loop:
        for(int i = 0; i < 64; i++)
        {
            OUT[i] = M3[i];
        }
    }
}
