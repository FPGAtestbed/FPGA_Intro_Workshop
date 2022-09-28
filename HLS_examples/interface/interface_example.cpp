#define N_VALS 500

extern "C"
{
    void v_add_kernel(double *A, double *B, double *out)
    {
        //pragmas go here!
        //assign A and B to separate bundles so they have independent interfaces
V_ADD_LOOP:
        for(int i = 0; i < N_VALS; i++)
        {
            out[i] = A[i] + B[i];
        }
    }
}
