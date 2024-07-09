/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:38 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/09 21:30:35 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_camera	init_camera(t_data *data)
{
	t_camera	camera;
	float		fov;

	camera.data = data;
	camera.objects = NULL;
	camera.pos = _mm_set_ps(0, 0, 0, 0);
	camera.quat = _mm_set_ps(0, 0, 0, 1);
	// camera.quat = angle_to_quat(_mm_set_ps(0, 0, 1, 0), M_PI * 2);
	fov = 60 * TO_RADIAN;
	camera.pitch_angle = 0;
	camera.pixel_width = 2 * tan(fov * 0.5f);
	camera.pixel_height = camera.pixel_width / camera.data->aspect_ratio;
	camera.objects = create_objects_array(create_ll_objects());
	return (camera);
}

t_opencl	init_opencl(t_data *data)
{
	char		*c_files[1];
	size_t		*c_size[1];
	t_opencl	opencl;
	cl_uint ret_num_devices; // ? replace to NULL in code?
    cl_uint ret_num_platforms; // ? replace to NULL in code?
    cl_int ret;

	c_files[0] = read_cfile("test.c");
	c_size[0] = sizeof(c_files[0] - 1);


	ret = clGetPlatformIDs(1, &opencl.platform, &ret_num_platforms);
    ret = clGetDeviceIDs(&opencl.platform, CL_DEVICE_TYPE_GPU, 1, &opencl.device, &ret_num_devices);

    // char device_name[128];
    // clGetDeviceInfo(opencl.device, CL_DEVICE_NAME, sizeof(device_name), device_name, NULL);
    // printf("Using device: %s\n", device_name);

    // cl_device_type device_type;
    // clGetDeviceInfo(opencl.device, CL_DEVICE_TYPE, sizeof(device_type), &device_type, NULL);
    // if (device_type & CL_DEVICE_TYPE_GPU) {
    //     printf("Device is a GPU\n");
    // } else {
    //     printf("Warning: Device is not a GPU\n");
    // }

    // Create an OpenCL context
    opencl.context = clCreateContext(NULL, 1, &opencl.device, NULL, NULL, &ret);

    // Create a command queue
    opencl.queue = clCreateCommandQueue(opencl.context, opencl.device, 0, &ret);

    // Create memory buffers on the device for the result
    opencl.image_buffer = clCreateBuffer(opencl.context, CL_MEM_WRITE_ONLY, sizeof(int), NULL, &ret);

    // Create a program from the kernel source
    opencl.program = clCreateProgramWithSource(opencl.context, 1, c_files, c_size, &ret);

    // Build the program
    ret = clBuildProgram(opencl.program, 1, &opencl.device, NULL, NULL, NULL);

    // Create the OpenCL kernel
    opencl.kernel = clCreateKernel(opencl.program, "findprime", &ret);

	// Set the arguments of the kernel
    ret = clSetKernelArg(opencl.kernel, 0, sizeof(cl_mem), (void *)&opencl.image_buffer);
    ret = clSetKernelArg(opencl.kernel, 1, sizeof(int), (void *)arguments_here);	
}

int	initialize(t_data *data, t_camera *camera)
{
	data->mlx_ptr = mlx_init();
	if (!data->mlx_ptr)
		return (0);
	data->win_width = 1920;
	data->win_height = 1080;
	// data->win_width = 600;
	// data->win_height = 600;
	data->aspect_ratio = (float)data->win_width / data->win_height;
	data->img = mlx_new_image(data->mlx_ptr, data->win_width, data->win_height);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_length, &data->endian);
	data->win_ptr = mlx_new_window(data->mlx_ptr, data->win_width,
			data->win_height, "miniRT");
	ft_bzero(&data->inputs, sizeof(t_inputs));
	data->inputs.key = -1;
	data->inputs.mouse_x = data->win_width * 0.5f;
	data->inputs.mouse_y = data->win_height * 0.5f;
	*camera = init_camera(data);
	data->camera = camera;
	mlx_mouse_move(data->win_ptr, data->win_width * 0.5f, data->win_height
		* 0.5f);
	render_frame(data);
	return (1);
}

// t_data *data,

void	render_frame(t_data *data)
{
	int		color;
	int		x;
	int		y;
	double	time_start;

	// t_ray	ray;
	ft_bzero(data->addr, data->win_height * data->line_length);
	y = -1;
	// printf("camera pos is:");
	// print_vector(data->camera->pos);
	time_start = (double)clock() / CLOCKS_PER_SEC;
	while (++y < data->win_height)
	{
		x = -1;
		while (++x < data->win_width)
		{
			// ray = create_ray(data->camera, x, y, data->win_width,
			// 		data->win_height);
			// printf("0-> ");
			// if (x == 0 && y == 0)
			// {
			// 	print_vector(ray.direction);
			// 	// print_m128(data->camera->quat);
			// }
			color = render_ray(create_ray(data->camera, x, y, data->win_width,
						data->win_height), data->camera->objects);
			if (color) // branch fallback is cheaper
				my_mlx_put_pixels(data, x, y, color);
		}
	}
	printf("%f\n", (double)clock() / CLOCKS_PER_SEC - time_start);
	mlx_put_image_to_window(data, data->win_ptr, data->img, 0, 0);
}
// fflush(stdout);
// print_vector(ray.direction);

int	main(void)
{
	t_opencl	opencl;
	t_camera	camera;
	t_data		data;

	// create_objects_array(create_ll_objects());
	// vector_test();
	if (!initialize(&data, &camera))
		return (1);
	// render_frame(data);
	mlx_mouse_hide();
	mlx_hook(data.win_ptr, 2, 0, deal_key_press, &data);
	mlx_hook(data.win_ptr, 3, 1, deal_key_release, &data);
	mlx_loop_hook(data.mlx_ptr, deal_input, &data);
	mlx_hook(data.win_ptr, 6, 1L << 6, mouse_hook, &camera);
	mlx_loop(data.mlx_ptr);
	mlx_put_image_to_window(&data, data.win_ptr, data.img, 0, 0);
	return (0);
}

// mlx_mouse_hook(data.win_ptr, mouse_hook, &data);
// int	main(void)
// {
// 	// vector_test();
// 	qua_test();
// 	init_camera
// 	return (0);
// }