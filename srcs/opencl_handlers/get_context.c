/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_context.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:38 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/19 16:18:34 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	create_buffer(t_opencl *opencl, t_data *data)
{
	cl_int	ret;

	opencl->addr = clCreateBuffer(opencl->context, CL_MEM_READ_WRITE,
			sizeof(int) * data->win_width * data->win_height, NULL, &ret);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);
	opencl->camera = clCreateBuffer(opencl->context, CL_MEM_WRITE_ONLY,
			sizeof(t_camera), NULL, &ret);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);
	opencl->objects = clCreateBuffer(opencl->context, CL_MEM_WRITE_ONLY,
			sizeof(t_object) * data->num_objects, NULL, &ret);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);
	opencl->reservoirs = clCreateBuffer(opencl->context, CL_MEM_READ_WRITE,
			sizeof(t_reservoir) * data->win_width * data->win_height, NULL,
			&ret);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);
	printf("Created Buffers\n");
}

void	get_context(t_opencl *opencl, t_data *data)
{
	cl_int	ret;

	opencl->context = clCreateContext(NULL, 1, &opencl->device, NULL, NULL,
			&ret);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);
	printf("Created Context\n");
	opencl->queue = clCreateCommandQueueWithProperties(opencl->context,
			opencl->device, 0, &ret);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);
	printf("Queued context\n");
	create_buffer(opencl, data);
}
