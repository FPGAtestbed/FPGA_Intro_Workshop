#define DATA_TYPE double

double sum_vec(double vec[8])
{
    double sum = 0;
    naive_sum_loop:
    for(int i = 0; i < 8; i++)
    {
        sum += vec[i];
    }
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
        // partition pragmas go here for these matrices 

        assign_matrix_loop:
        for(int i = 0; i < 64; i++)
        {
            M1[i] = A[i];
            M2[i] = B[i];
        }

        // A variable to store the results of multiplications (8 multiplications per row-column product)
        double row_col_prod[8];
        //partition pragma goes here for this array 

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
