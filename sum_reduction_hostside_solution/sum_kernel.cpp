extern "C" {
void sum_kernel(double* x, double *out, int N) {
  
  //create a local variable to do the sum
  double t = 0;

  for(int i=0; i<N; ++i) {
    t += x[i];
  }

  //only modify output once because it is in global memory
  *out = t;
}
}
