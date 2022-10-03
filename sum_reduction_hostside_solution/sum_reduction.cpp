extern "C" {
void sum_reduction(double* x, double* out, int N) {
  for(int i=0; i<N; ++i) {
    out += x[i];
  }
}
}
