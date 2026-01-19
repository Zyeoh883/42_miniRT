/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_platform_and_devices.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 13:46:32 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/16 22:24:14 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

cl_uint	count_total_gpus(cl_platform_id *platforms, cl_uint num_platforms)
{
	cl_uint	total;
	cl_uint	count;
	cl_uint	n;

	total = 0;
	count = 0;
	n = -1;
	while (++n < num_platforms)
	{
		clGetDeviceIDs(platforms[n], CL_DEVICE_TYPE_GPU, 0, NULL, &count);
		total += count;
	}
	if (!total)
		perror_and_exit("No GPUS available to use", EXIT_FAILURE);
	return (total);
}

void	fill_gpu_list(t_opencl *opencl, cl_device_id *gpu_list,
		cl_platform_id *platform_list, cl_uint num_platforms)
{
	cl_uint	platform_gpus;
	cl_uint	n;
	cl_uint	m;
	cl_uint	current_idx;

	n = -1;
	m = 0;
	current_idx = 0;
	while (++n < num_platforms)
	{
		if (clGetDeviceIDs(opencl->platform[n], CL_DEVICE_TYPE_GPU, 0, NULL,
				&platform_gpus) != CL_SUCCESS)
			perror_and_exit("Platform fetch failed", EXIT_FAILURE);
		if (platform_gpus > 0)
		{
			if (clGetDeviceIDs(opencl->platform[n], CL_DEVICE_TYPE_GPU,
					platform_gpus, &gpu_list[current_idx], NULL) != CL_SUCCESS)
				perror_and_exit("GPU fetch from platform failed", EXIT_FAILURE);
			m = -1;
			while (++m < platform_gpus)
				platform_list[current_idx] = opencl->platform[n];
			current_idx += platform_gpus;
		}
	}
}

void	print_gpu_list(cl_device_id *gpu_list, cl_platform_id *platform_list,
		cl_uint total_gpus)
{
	char	platform_name[1024];
	char	device_name[1024];
	cl_uint	n;

	n = -1;
	while (++n < total_gpus)
	{
		clGetPlatformInfo(platform_list[n], CL_PLATFORM_NAME,
			sizeof(platform_name), platform_name, NULL);
		clGetDeviceInfo(gpu_list[n], CL_DEVICE_NAME, sizeof(device_name),
			device_name, NULL);
		printf("%u: [%s] %s\n", n, platform_name, device_name);
	}
}

void	user_choose_gpu(t_opencl *opencl, cl_device_id *gpu_list,
		cl_platform_id *platform_list, cl_uint total_gpus)
{
	char	*buffer;
	cl_uint	n;

	print_gpu_list(gpu_list, platform_list, total_gpus);
	printf("\nSelect a GPU to use (enter the number):\n");
	n = -1;
	buffer = get_next_line(STDIN_FILENO);
	if (buffer && ft_isdigit(*buffer))
		n = ft_atoi(buffer);
	if (buffer)
		free(buffer);
	if (n >= total_gpus)
	{
		printf("Invalid selection. Using the first GPU by default.\n");
		n = 0;
	}
	opencl->device = gpu_list[n];
	opencl->platform[0] = platform_list[n];
}

void	get_platform_and_devices(t_opencl *opencl, cl_device_id *gpu_list,
		cl_platform_id *platform_list, cl_uint total_gpus)
{
	cl_uint	num_platforms;

	if (clGetPlatformIDs(2, opencl->platform, &num_platforms) != CL_SUCCESS)
		perror_and_exit("Failed to get platforms", EXIT_FAILURE);
	printf("Available platforms: %u\n", num_platforms);
	total_gpus = count_total_gpus(opencl->platform, num_platforms);
	platform_list = ft_calloc(total_gpus, sizeof(cl_platform_id));
	gpu_list = ft_calloc(total_gpus, sizeof(cl_device_id));
	if (!platform_list || !gpu_list)
		perror_and_exit("Malloc Failed", EXIT_FAILURE);
	fill_gpu_list(opencl, gpu_list, platform_list, num_platforms);
	user_choose_gpu(opencl, gpu_list, platform_list, total_gpus);
	free(gpu_list);
	free(platform_list);
	printf("Retrieved platformID and deviceID\n");
}
