/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_context.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:38 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/08 17:43:39 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"


void get_context(t_opencl *opencl, t_data *data)
{
  cl_int ret;


  // Create an OpenCL context
  opencl->context = clCreateContext(NULL, 1, &opencl->device, NULL, NULL, &ret);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);

  printf("Created Context\n");

  // Create a command queue
  opencl->queue = clCreateCommandQueueWithProperties(opencl->context, opencl->device, 0, &ret);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);

  printf("Queued context\n");

  // Create memory buffers on the device for the result
  opencl->addr = clCreateBuffer(opencl->context, CL_MEM_WRITE_ONLY, sizeof(int) * data->win_width * data->win_height, NULL, &ret);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);
	opencl->camera = clCreateBuffer(opencl->context, CL_MEM_WRITE_ONLY, sizeof(t_camera), NULL, &ret);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);
	opencl->objects = clCreateBuffer(opencl->context, CL_MEM_WRITE_ONLY, sizeof(t_object) * data->num_objects, NULL, &ret);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);
	opencl->reservoirs = clCreateBuffer(opencl->context, CL_MEM_WRITE_ONLY, sizeof(t_reservoir) * data->win_width * data->win_height, NULL, &ret);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);
  
  printf("Loaded params\n");
  
}








