/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:23:07 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/19 16:41:37 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

cl_float4	get_quat_value(char *str)
{
	cl_float4	quat;
	char		**split;

	split = ft_split(str, '.');
	if (!split)
		perror_and_exit("malloc", EXIT_FAILURE);
	quat = (cl_float4){{ft_atoi(split[0]), ft_atoi(split[1]), ft_atoi(split[2]),
		0}};
	free_str_arr(split);
	return (quat);
}

cl_float3	inv_rgb_float(int rgb)
{
	return ((cl_float3){{(float)((rgb >> 16) % 0x100) / 0xFF,
		(float)((rgb >> 8) % 0x100) / 0xFF, (float)((rgb % 0x100) / 0xFF)}});
}

cl_float3	to_float3(float x)
{
	return ((cl_float3){{x, x, x}});
}

// clReleaseDevice(data->opencl->device);
void	end_program(t_data *data)
{
	clFinish(data->opencl->queue);
	clReleaseMemObject(data->opencl->addr);
	clReleaseMemObject(data->opencl->camera);
	clReleaseMemObject(data->opencl->objects);
	clReleaseMemObject(data->opencl->reservoirs);
	clReleaseProgram(data->opencl->program);
	clReleaseKernel(data->opencl->kernel);
	clReleaseCommandQueue(data->opencl->queue);
	clReleaseContext(data->opencl->context);
	free(data->camera);
	free(data->objects);
	if (data->file_content)
		ft_lstclear(&data->file_content, (void *)free_str_arr);
	mlx_destroy_window(data->mlx_ptr, data->win_ptr);
	mlx_destroy_image(data->mlx_ptr, data->img);
	mlx_destroy_display(data->mlx_ptr);
	free(data->mlx_ptr);
	if (clUnloadPlatformCompiler(data->opencl->platform[0]) != CL_SUCCESS)
		perror_and_exit("Failed to unload platform compiler", EXIT_FAILURE);
	free(data->opencl);
	exit(0);
}
