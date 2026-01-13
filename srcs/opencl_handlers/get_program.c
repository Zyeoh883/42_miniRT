/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_program.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 17:26:19 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/09 22:47:57 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void get_program(t_opencl *opencl, char **c_files, size_t c_size[9])
{
  cl_int ret;
  
  // Create a program from the kernel source
  opencl->program = clCreateProgramWithSource(opencl->context, 8, (const char **)c_files, c_size, &ret);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);

  printf("Loaded program\n");

  // Add these before clBuildProgram
  cl_device_info power_info;
  ret = clGetDeviceInfo(opencl->device, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, 
                     sizeof(power_info), &power_info, NULL);

  // Modify your clBuildProgram call to include build options
  // char *build_options = "-cl-opt-disable -cl-strict-aliasing";
  char *build_options = "";
  ret = clBuildProgram(opencl->program, 1, &opencl->device, build_options, NULL, NULL);

  // Build the program
  // ret = clBuildProgram(opencl->program, 1, &opencl->device, NULL, NULL, NULL);
	if (ret != CL_SUCCESS)
	{
		printf("clBuildProgram error: %d\n", ret);
		print_cl_error(ret);
		
		// Get the build log
		size_t log_size;
		clGetProgramBuildInfo(opencl->program, opencl->device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
		char *log = (char *)malloc(log_size);
		clGetProgramBuildInfo(opencl->program, opencl->device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
		printf("Build log:\n%s\n", log);
		free(log);
		
		exit(EXIT_FAILURE);
	}

  printf("Program Built\n");

}
