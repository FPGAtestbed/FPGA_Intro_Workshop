extern "C"
{
void sum_kernel(double *inputVector, double *sum, int N)
{
    double t = 0;
    
    for(int i = 0; i < N; i++)
    {
        t += inputVector[i];
    }

    *sum = t;
}
}
