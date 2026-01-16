/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:38 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/09 21:43:58 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// fov = FOV * TO_RADIAN;
t_camera	*init_camera(t_data *data, int win_height, int win_width)
{
	t_camera	*camera;
	cl_float	aspect_ratio;

	camera = ft_calloc(1, sizeof(t_camera));
	if (!camera)
		exit(EXIT_FAILURE);
	*camera = get_camera(data->file_content);
	camera->pos = (cl_float4){{0, 0, 0}};
	camera->quat = (cl_float4){{0, 0, 0, 1}};
	aspect_ratio = (cl_float)win_width / win_height;
	camera->pixel_width = 2 * tan(camera->fov * 0.5f);
	camera->pixel_height = camera->pixel_width / aspect_ratio;
	camera->win_width = win_width;
	camera->win_height = win_height;
	camera->bytes_per_pixel = data->bits_per_pixel * 0.125f;
	camera->line_length = data->line_length;
	return (camera);
}

// size_t maxWorkGroupSize;
// clGetDeviceInfo(opencl->device, CL_DEVICE_MAX_WORK_GROUP_SIZE,
//	sizeof(size_t), &maxWorkGroupSize, NULL);
// printf("Maximum Workgroup Size: %zu\n", maxWorkGroupSize);
t_opencl	*init_opencl(t_data *data)
{
	char		**c_files;
	size_t		c_size[9];
	t_opencl	*opencl;

	c_files = NULL;
	opencl = ft_calloc(1, sizeof(t_opencl));
	if (!opencl)
		exit(EXIT_FAILURE);
	c_files = get_cfiles(c_size);
	get_platform_and_devices(opencl, NULL, NULL, 0);
	get_context(opencl, data);
	get_program(opencl, c_files, c_size);
	get_kernel(opencl);
	free_cfile(c_files);
	data->opencl = opencl;
	return (opencl);
}

//  data->win_width = 2560;
// data->win_height = 1600;
int	initialize(t_data *data, char *filename)
{
	data->file_content = get_rt_file(filename);
	data->objects = get_objects(data, data->file_content);
	data->mlx_ptr = mlx_init();
	if (!data->mlx_ptr)
		return (0);
	data->win_width = 2560;
	data->win_height = 1600;
	data->img = mlx_new_image(data->mlx_ptr, data->win_width, data->win_height);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_length, &data->endian);
	data->camera = init_camera(data, data->win_height, data->win_width);
	data->opencl = init_opencl(data);
	data->win_ptr = mlx_new_window(data->mlx_ptr, data->win_width,
			data->win_height, "miniRT");
	ft_bzero(&data->inputs, sizeof(t_inputs));
	data->inputs.key = -1;
	data->inputs.mouse_x = data->win_width * 0.5f;
	data->inputs.mouse_y = data->win_height * 0.5f;
	data->inputs.pitch_angle = 0;
	data->inputs.update = 1;
	mlx_mouse_move(data->mlx_ptr, data->win_ptr, data->inputs.mouse_x,
		data->inputs.mouse_y);
	printf("Init done\n");
	return (1);
}

int	render_frame(t_data *data)
{
	double	time_start;
	double	average;

	average = 0;
	deal_input(data);
	time_start = (double)clock() / CLOCKS_PER_SEC;
	queue_buffer(data->opencl, data);
	average = (double)clock() / CLOCKS_PER_SEC - time_start;
	while ((double)clock() / CLOCKS_PER_SEC - time_start < 0.0020f)
		usleep(50);
	printf("%f\n", 1 / average);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img, 0, 0);
	data->inputs.update = 0;
	data->camera->moved = false;
	return (0);
}

// mlx_hook(data.win_ptr, 0, 0,deal_input, &data);
int	main(int ac, char **av)
{
	t_data	data;

	if (ac != 2 || !initialize(&data, av[1]))
		return (1);
	mlx_mouse_hide(data.mlx_ptr, data.win_ptr);
	mlx_hook(data.win_ptr, 2, 1L << 0, (void *)deal_key_press, &data);
	mlx_hook(data.win_ptr, 3, 1L << 1, (void *)deal_key_release, &data);
	mlx_hook(data.win_ptr, 6, 1L << 6, (void *)mouse_hook, &data);
	mlx_loop_hook(data.mlx_ptr, (void *)render_frame, &data);
	mlx_loop(data.mlx_ptr);
	return (0);
}
