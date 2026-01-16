/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_program.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 17:26:19 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/16 22:31:44 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// char *build_options = "-cl-opt-disable -cl-strict-aliasing";

void	print_program_build_error(t_opencl *opencl, int ret)
{
	char	*log;
	size_t	log_size;

	printf("clBuildProgram error: %d\n", ret);
	print_cl_error(ret);
	clGetProgramBuildInfo(opencl->program, opencl->device, CL_PROGRAM_BUILD_LOG,
		0, NULL, &log_size);
	log = (char *)malloc(log_size);
	clGetProgramBuildInfo(opencl->program, opencl->device, CL_PROGRAM_BUILD_LOG,
		log_size, log, NULL);
	printf("Build log:\n%s\n", log);
	free(log);
	exit(EXIT_FAILURE);
}

void	get_program(t_opencl *opencl, char **c_files, size_t c_size[9])
{
	cl_int			ret;
	cl_device_info	power_info;
	char			*build_options;

	opencl->program = clCreateProgramWithSource(opencl->context, 8,
			(const char **)c_files, c_size, &ret);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);
	printf("Loaded program\n");
	ret = clGetDeviceInfo(opencl->device, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
			sizeof(power_info), &power_info, NULL);
	build_options = "";
	ret = clBuildProgram(opencl->program, 1, &opencl->device, build_options,
			NULL, NULL);
	if (ret != CL_SUCCESS)
		print_program_build_error(opencl, ret);
	printf("Program Built\n");
}
