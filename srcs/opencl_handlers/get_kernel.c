/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_kernel.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 17:39:24 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/08 17:42:01 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minirt.h"


void get_kernel(t_opencl *opencl)
{

  cl_int ret;

  // Create the OpenCL kernel
  opencl->kernel = clCreateKernel(opencl->program, "render_scene", &ret);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);

  printf("Created kernel\n");
	// Set the arguments of the kernel
  ret = clSetKernelArg(opencl->kernel, 0, sizeof(cl_mem), (void *)&opencl->addr);
	ret = clSetKernelArg(opencl->kernel, 1, sizeof(cl_mem), (void *)&opencl->camera);
	ret = clSetKernelArg(opencl->kernel, 2, sizeof(cl_mem), (void *)&opencl->objects);
	ret = clSetKernelArg(opencl->kernel, 3, sizeof(cl_mem), (void *)&opencl->reservoirs);


}









