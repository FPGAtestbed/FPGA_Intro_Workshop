#include <string>
#include <vector>
#include <iostream>
#include <host_support.h>
#include <CL/cl2.hpp>

const int N = 1000;
const int BUFFER_SIZE = N*sizeof(double);
const std::string XCLBIN_FILE = "sum.xclbin";

int main(int argc, const char **argv)
{
  // LOAD OPENCL DEVICE + BINARY FILE
  cl::Context* context_p;
  cl::Program* program_p;
  std::vector<cl::Device> devices;
  std::tie(program_p, context_p, devices)=initialiseDevice("Xilinx", "u280", XCLBIN_FILE);
  cl::Device device = devices[0]; // Choose first device from results
  cl::Context context = *context_p;
  cl::Program program = *program_p;
  cl_int err;
  cl::CommandQueue queue(context, device, CL_QUEUE_PROFILING_ENABLE, &err);

  // LOAD KERNEL
  cl::KernelFunctor<
    cl::Buffer, cl::Buffer, int
  > sum_reduction_k = cl::Kernel(program, "sum_kernel");

  // create device-side buffers (i.e. allocate device memory)
  cl::Buffer x_d(context, CL_MEM_READ_ONLY, BUFFER_SIZE, NULL, &err);
  cl::Buffer out_d(context, CL_MEM_WRITE_ONLY, sizeof(double), NULL, &err);

  // CREATE CORRESPONDING HOST-SIDE BUFFERS
  double *x_h = (double *)queue.enqueueMapBuffer(x_d, CL_TRUE, CL_MAP_WRITE, 0, BUFFER_SIZE);
  double *out_h = (double *)queue.enqueueMapBuffer(out_d, CL_TRUE, CL_MAP_WRITE | CL_MAP_READ, 0, sizeof(double));

  // LOAD DATA INTO HOST-SIDE
  for(int i = 0; i < N; i++) {
      x_h[i] = 1.0;
  }

  // TRANSFER DATA TO DEVICE
  queue.enqueueMigrateMemObjects({x_d},0);

  // QUEUE KERNEL
  sum_reduction_k(cl::EnqueueArgs(queue, cl::NDRange(1), cl::NDRange(1)), x_d, out_d, N);

  // SYNCHRONISE
  queue.finish();

  // TRANSFER DATA TO HOST
  queue.enqueueMigrateMemObjects({out_d}, CL_MIGRATE_MEM_OBJECT_HOST);

  // SYNCHRONISE AGAIN
  queue.finish();

  bool kernel_worked = (std::abs(*out_h - N) < 1e-4);

  if (kernel_worked) {
    std::cout << "It worked!" << std::endl;
  } else {
    std::cout << "Something went wrong..." << std::endl;
  }

  return 0;
}
