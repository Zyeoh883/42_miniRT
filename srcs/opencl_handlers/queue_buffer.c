/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_buffer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 17:45:13 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/16 22:38:07 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	queue_buffer(t_opencl *opencl, t_data *data)
{
	cl_int	ret;
	size_t	global_size[2];

	global_size[0] = data->win_width;
	global_size[1] = data->win_height;
	ret = clEnqueueWriteBuffer(opencl->queue, opencl->camera, CL_TRUE, 0,
			sizeof(t_camera), data->camera, 0, NULL, NULL);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);
	ret = clEnqueueWriteBuffer(opencl->queue, opencl->objects, CL_TRUE, 0,
			sizeof(t_object) * data->num_objects, data->objects, 0, NULL, NULL);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);
	ret = clEnqueueNDRangeKernel(opencl->queue, opencl->kernel, 2, NULL,
			global_size, NULL, 0, NULL, NULL);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);
	ret = clEnqueueReadBuffer(opencl->queue, opencl->addr, CL_TRUE, 0,
			sizeof(int) * data->win_width * data->win_height, data->addr, 0,
			NULL, NULL);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);
}
// ret = clEnqueueReadBuffer(opencl->queue, opencl->reservoirs, CL_TRUE, 0,
// sizeof(t_reservoir) * data->win_width * data->win_height,
// data->reservoirs, 0, NULL, NULL);
// if (ret != CL_SUCCESS)
// {
// 	printf("ret 5 error: %d\n", ret);
// 	print_cl_error(ret);
// }
// printf("Queued all Buffers\n");
