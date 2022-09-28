#define N_VALS 500

extern "C"
{
    void bind_storage_kernel(double base_val, double *out)
    {
        // Replace the memory storage type below to optimise the multiple memory writes in ASSIGN_LOOP
        // Then try a different storage type to optimise the reads in OUTPUT_LOOP 
        // Which solution is best for overall latency? 
        // Try commenting out the bind_storage pragma completely: is the automatic solution better or worse than yours?
        double local_var[N_VALS];
        #pragma HLS bind_storage variable=local_var type=ram_1p impl=bram

    ASSIGN_LOOP:
        for (int i = 0; i < N_VALS; i += 2)
        {
            local_var[i] = base_val * i;
            local_var[i + 1] = base_val / i;
        }

        double temps[4];
    OUTPUT_LOOP:
        for(int i = 0; i < N_VALS; i++)
        {
            temps[0] = local_var[(i * 17) % N_VALS];
            temps[1] = local_var[(i * 13) % N_VALS];
            temps[2] = local_var[(i * 11) % N_VALS];
            temps[3] = local_var[(i * 19) % N_VALS];
            
            out[i] = temps[0] * temps[2] + temps[1] * temps[3];
        }
    }
}
