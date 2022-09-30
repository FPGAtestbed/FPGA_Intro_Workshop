// Must declare extern "C" to avoid name mangling 
extern "C"
{
    // Kernel should be void and output should be an address in global memory
    // CPU won't be able to access anything that doesn't end up in global memory or
    // an output stream 
    void sum_kernel(double *inputVector, double &output, int N)
    {
        // Don't work on output directly, only want to access input and 
        // output ONCE if we can due to global memory latency
        double t = 0;
        
        for(int i = 0; i < N; i++)
        {
            t += inputVector[i];
        }

        // Single write to output in global memory
        output = t;
    }
}