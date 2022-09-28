double sum_kernel(double *inputVector, int N)
{
    double t = 0;
    
    for(int i = 0; i < N; i++)
    {
        t += inputVector[i];
    }

    return t;
}