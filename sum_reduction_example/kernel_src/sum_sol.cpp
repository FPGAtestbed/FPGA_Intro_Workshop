extern "C"
{
void sum_kernel(double *inputVector, double *sum, int N)
{
    //declare a local variable to do the sum
    double t = 0;
    
    for(int i = 0; i < N; i++)
    {
        t += inputVector[i];
    }

    //only access the output once because it is in global memory
    *sum = t;
}
}
