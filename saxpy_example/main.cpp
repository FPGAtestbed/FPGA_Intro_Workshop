#include <string>
#include <vector>
#include <iostream>
#include <host_support.h>
#include <CL/cl2.hpp>

const int N = 1000;
const double a = 2.0;
const int BUFFER_SIZE = N*sizeof(double);
const std::string XCLBIN_FILE = "saxpy.xclbin";

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
    cl::Buffer, cl::Buffer, cl::Buffer, double, int
  > saxpy_kernel = cl::Kernel(program, "saxpy");

  // create device-side buffers (i.e. allocate device memory)
  cl::Buffer x_d(context, CL_MEM_READ_ONLY, sizeof(double) * N, NULL, &err);
  cl::Buffer y_d(context, CL_MEM_READ_ONLY, sizeof(double) * N, NULL, &err);
  cl::Buffer out_d(context, CL_MEM_WRITE_ONLY, sizeof(double) * N, NULL, &err);
      
  // CREATE CORRESPONDING HOST-SIDE BUFFERS
  double *x_h = (double *)queue.enqueueMapBuffer(x_d, CL_TRUE, CL_MAP_WRITE, 0, sizeof(double) * N);
  double *y_h = (double *)queue.enqueueMapBuffer(y_d, CL_TRUE, CL_MAP_WRITE, 0, sizeof(double) * N);
  double *out_h = (double *)queue.enqueueMapBuffer(out_d, CL_TRUE, CL_MAP_WRITE | CL_MAP_READ, 0, sizeof(double) * N);
      
  // LOAD DATA INTO HOST-SIDE
  for(int i = 0; i < N; i++) {
      x_h[i] = i/3.0;
      y_h[i] = i/2.0;
  }

  // TRANSFER DATA TO DEVICE
  queue.enqueueMigrateMemObjects({x_d, y_d},0);

  // QUEUE KERNEL
  saxpy_kernel(cl::EnqueueArgs(queue, cl::NDRange(1), cl::NDRange(1)), x_d, y_d, out_d, a, N);

  // SYNCHRONISE
  queue.finish();

  // TRANSFER DATA TO HOST
  queue.enqueueMigrateMemObjects({out_d}, CL_MIGRATE_MEM_OBJECT_HOST);

  // SYNCHRONISE AGAIN
  queue.finish();

  bool kernel_worked = true;
  for(int i = 0; i < N; i++) {
    if(out_h[i] != a*x_h[i] + y_h[i]) {
      kernel_worked = false;
      break;
    }
  }

  if (kernel_worked) {
    std::cout << "It worked!" << std::endl;
  } else {
    std::cout << "Something went wrong..." << std::endl;
  }

  return 0;
}
