/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 14:26:56 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/05 15:36:23 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <OpenCL/opencl.h>

const char* programSource = 
    "__kernel void hello(__global char* string) {"
    "  string[0] = 'H';"
    "  string[1] = 'e';"
    "  string[2] = 'l';"
    "  string[3] = 'l';"
    "  string[4] = 'o';"
    "  string[5] = ',';"
    "  string[6] = ' ';"
    "  string[7] = 'W';"
    "  string[8] = 'o';"
    "  string[9] = 'r';"
    "  string[10] = 'l';"
    "  string[11] = 'd';"
    "  string[12] = '!';"
    "  string[13] = '\\0';"
    "}";

int main() {
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_kernel kernel;
    cl_mem buffer;
    char result[14];
    size_t globalWorkSize[1] = { 1 };
    
    cl_int status;
    status = clGetPlatformIDs(1, &platform, NULL);
    status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    context = clCreateContext(NULL, 1, &device, NULL, NULL, &status);
    queue = clCreateCommandQueue(context, device, 0, &status);

    program = clCreateProgramWithSource(context, 1, &programSource, NULL, &status);
    status = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    kernel = clCreateKernel(program, "hello", &status);

    buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 14 * sizeof(char), NULL, &status);
    status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer);
    status = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, globalWorkSize, NULL, 0, NULL, NULL);

    status = clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, 14 * sizeof(char), result, 0, NULL, NULL);
    printf("%s\n", result);

    clReleaseMemObject(buffer);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    return 0;
}
