#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <OpenCL/opencl.h>

//gcc -framework OpenCL test.c && ./a.out

#define MAX_SOURCE_SIZE (0x100000)

const int WIDTH = 1024;
const int HEIGHT = 1024;
const int MAX_ITERATIONS = 1000;

// CPU implementation
void mandelbrot_cpu(unsigned char* output) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            float c_re = (x - WIDTH/2.0) * 4.0/WIDTH;
            float c_im = (y - HEIGHT/2.0) * 4.0/HEIGHT;
            float z_re = c_re, z_im = c_im;
            int i;
            for (i = 0; i < MAX_ITERATIONS; i++) {
                if (z_re*z_re + z_im*z_im > 4.0) break;
                float new_re = z_re*z_re - z_im*z_im;
                float new_im = 2.0*z_re*z_im;
                z_re = c_re + new_re;
                z_im = c_im + new_im;
            }
            output[y*WIDTH + x] = i;
        }
    }
}

// OpenCL kernel
const char* kernelSource =
"__kernel void mandelbrot(__global unsigned char* output, int width, int height, int max_iterations) {\n"
"    int x = get_global_id(0);\n"
"    int y = get_global_id(1);\n"
"    if (x >= width || y >= height) return;\n"
"    float c_re = (x - width/2.0f) * 4.0f/width;\n"
"    float c_im = (y - height/2.0f) * 4.0f/height;\n"
"    float z_re = c_re, z_im = c_im;\n"
"    int i;\n"
"    for (i = 0; i < max_iterations; i++) {\n"
"        if (z_re*z_re + z_im*z_im > 4.0f) break;\n"
"        float new_re = z_re*z_re - z_im*z_im;\n"
"        float new_im = 2.0f*z_re*z_im;\n"
"        z_re = c_re + new_re;\n"
"        z_im = c_im + new_im;\n"
"    }\n"
"    output[y*width + x] = i;\n"
"}\n";

int main() {
    unsigned char* output_cpu = (unsigned char*)malloc(WIDTH * HEIGHT);
    unsigned char* output_gpu = (unsigned char*)malloc(WIDTH * HEIGHT);

    // CPU Mandelbrot
    clock_t start_cpu = clock();
    mandelbrot_cpu(output_cpu);
    clock_t end_cpu = clock();
    double cpu_time = ((double)(end_cpu - start_cpu)) / CLOCKS_PER_SEC;
    printf("CPU Time: %f seconds\n", cpu_time);

    // GPU Mandelbrot
    cl_platform_id platform_id = NULL;
    cl_device_id device_id = NULL;
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

    cl_mem output_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, WIDTH * HEIGHT * sizeof(unsigned char), NULL, &ret);

    cl_program program = clCreateProgramWithSource(context, 1, (const char**)&kernelSource, NULL, &ret);
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    cl_kernel kernel = clCreateKernel(program, "mandelbrot", &ret);

    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&output_mem_obj);
    ret = clSetKernelArg(kernel, 1, sizeof(int), (void*)&WIDTH);
    ret = clSetKernelArg(kernel, 2, sizeof(int), (void*)&HEIGHT);
    ret = clSetKernelArg(kernel, 3, sizeof(int), (void*)&MAX_ITERATIONS);

    size_t global_item_size[2] = {WIDTH, HEIGHT};
    size_t local_item_size[2] = {16, 16};

    clock_t start_gpu = clock();
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 2, NULL, global_item_size, local_item_size, 0, NULL, NULL);
    ret = clEnqueueReadBuffer(command_queue, output_mem_obj, CL_TRUE, 0, WIDTH * HEIGHT * sizeof(unsigned char), output_gpu, 0, NULL, NULL);
    clock_t end_gpu = clock();
    double gpu_time = ((double)(end_gpu - start_gpu)) / CLOCKS_PER_SEC;
    printf("GPU Time: %f seconds\n", gpu_time);

    printf("Speedup: %.2fx\n", cpu_time / gpu_time);

    // Clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(output_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);

    free(output_cpu);
    free(output_gpu);

    return 0;
}