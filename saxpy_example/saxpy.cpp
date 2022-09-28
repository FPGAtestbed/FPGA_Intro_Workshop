extern "C" {
void saxpy(double* x, double *y, double* out, double a, int N) {
  for(int i=0; i<N; ++i) {
    out[i] = a*x[i] + y[i];
  }
}
}
