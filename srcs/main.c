/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:38 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/11 11:31:29 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_camera	*init_camera(t_data *data, int win_height, int win_width)
{
	t_camera		*camera;
	cl_float		fov;
	cl_float		aspect_ratio;

	camera = ft_calloc(1, sizeof(t_camera));
	if (!camera)
		exit(EXIT_FAILURE);
	// camera.data = data;
	// camera.objects = NULL;
	camera->pos = (cl_float4) {{0, 0, 0, 0}};
	camera->quat = (cl_float4) {{0, 0, 0, 1}};
	// camera->quat = angle_to_quat(_mm_set_ps(0, 0, 1, 0), M_PI * 2);
	fov = 60 * TO_RADIAN;
	aspect_ratio = (cl_float)win_width / win_height;
	camera->pixel_width = 2 * tan(fov * 0.5f);
	camera->pixel_height = camera->pixel_width / aspect_ratio;
	camera->win_width = win_width;
	camera->win_height = win_height;
	// camera->objects = create_objects_array(create_ll_objects());
	camera->num_objects = 7; // ! hard coded
	camera->bytes_per_pixel = data->bits_per_pixel * 0.125f;
	camera->line_length = data->line_length;
	data->camera = camera;
	return (camera);
}

t_opencl	*init_opencl(t_data *data)
{
	char		**c_files;
	size_t		c_size[1];
	t_opencl	*opencl;
    cl_int ret;

	opencl = ft_calloc(1, sizeof(t_opencl));
	if (!opencl)
		exit(EXIT_FAILURE);

	c_files = ft_calloc(1, sizeof(char *));
	// c_files = ft_calloc(1, sizeof(char *));
	c_files[0] = read_cfile("placeholder.c"); // * load GPU source files
	c_size[0] = ft_strlen(c_files[0]);


	ret = clGetPlatformIDs(1, &opencl->platform, NULL);
    ret = clGetDeviceIDs(opencl->platform, CL_DEVICE_TYPE_GPU, 1, &opencl->device, NULL);

    // Create an OpenCL context
    opencl->context = clCreateContext(NULL, 1, &opencl->device, NULL, NULL, &ret);

    // Create a command queue
    opencl->queue = clCreateCommandQueue(opencl->context, opencl->device, 0, &ret);

    // Create memory buffers on the device for the result
    opencl->addr = clCreateBuffer(opencl->context, CL_MEM_WRITE_ONLY, sizeof(data->addr), NULL, &ret);
	opencl->camera = clCreateBuffer(opencl->context, CL_MEM_WRITE_ONLY, sizeof(t_camera), NULL, &ret);
	opencl->objects = clCreateBuffer(opencl->context, CL_MEM_WRITE_ONLY, sizeof(t_object) * 7, NULL, &ret); // ! hard coded amount
	opencl->camera = clCreateBuffer(opencl->context, CL_MEM_WRITE_ONLY, sizeof(t_camera), NULL, &ret);

	printf("opencl_camera: %p\n", opencl->camera);
	
    // Create a program from the kernel source
    opencl->program = clCreateProgramWithSource(opencl->context, 1, (const char **)c_files, c_size, &ret);

    // Build the program
    ret = clBuildProgram(opencl->program, 1, &opencl->device, NULL, NULL, NULL);

    // Create the OpenCL kernel
    opencl->kernel = clCreateKernel(opencl->program, "render_scene", &ret);

	// Set the arguments of the kernel
    ret = clSetKernelArg(opencl->kernel, 0, sizeof(cl_mem), (void *)opencl->addr);
	ret = clSetKernelArg(opencl->kernel, 1, sizeof(cl_mem), (void *)&data->camera);
	ret = clSetKernelArg(opencl->kernel, 2, sizeof(cl_mem), (void *)data->objects);
    // ret = clSetKernelArg(opencl.kernel, 1, sizeof(int), (void *)arguments_here);
	data->opencl = opencl;
	return (opencl);
}

int	initialize(t_data *data, t_camera *camera)
{
	int win_height;
	int win_width;

	data->mlx_ptr = mlx_init();
	if (!data->mlx_ptr)
		return (0);
	win_width = 1920;
	win_height = 1080;
	// win_width = 600;
	// win_height = 600;
	data->img = mlx_new_image(data->mlx_ptr, win_width, win_height);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_length, &data->endian);
	data->win_ptr = mlx_new_window(data->mlx_ptr, win_width,
			win_height, "miniRT");
	ft_bzero(&data->inputs, sizeof(t_inputs));
	data->inputs.key = -1;
	data->inputs.mouse_x = win_width * 0.5f;
	data->inputs.mouse_y = win_height * 0.5f;
	data->inputs.pitch_angle = 0;
	camera = init_camera(data, win_height, win_width);
	mlx_mouse_move(data->win_ptr, win_width * 0.5f, win_height
		* 0.5f);
	data->objects = create_objects_array(create_ll_objects());
	data->opencl = init_opencl(data);
	render_frame(data, data->opencl);
	return (1);
}

// t_data *data,

void	render_frame(t_data *data, t_opencl *opencl)
{
	cl_int ret;
	size_t global_size[2];
	size_t local_size[2];
	double	time_start;

	// ft_bzero(data->addr, data->win_height * data->line_length);
	
	//execution
	(void)opencl;
	global_size[0] = (size_t) {data->win_width};
	global_size[1] = (size_t) {data->win_height};
    local_size[0] = (size_t) {32};
	local_size[1] = (size_t) {32};
	time_start = (double)clock() / CLOCKS_PER_SEC;
	printf("%p %p\n", data->camera, &data->opencl);
	printf("%p %p\n", opencl->queue, opencl->camera);
	printf("\nHERE\n");
    ret = clEnqueueWriteBuffer(opencl->queue, opencl->camera, CL_TRUE, 0, sizeof(t_camera), data->camera, 0, NULL, NULL);
	printf("\nHERE\n");
	// ret = clEnqueueWriteBuffer(opencl->queue, opencl->objects, CL_TRUE, 0, sizeof(t_object) * 7, data->objects, 0, NULL, NULL); // ! hard coded
    // ret = clEnqueueNDRangeKernel(opencl->queue, opencl->kernel, 2, NULL, (size_t *)&global_size, (size_t *)&local_size, 0, NULL, NULL);
	// ret = clEnqueueReadBuffer(opencl->queue, opencl->addr, CL_TRUE, 0, sizeof(data->addr), data->addr, 0, NULL, NULL);

	printf("%f\n", (double)clock() / CLOCKS_PER_SEC - time_start);
	mlx_put_image_to_window(data, data->win_ptr, data->img, 0, 0);
}
// fflush(stdout);
// print_vector(ray.direction);

int	main(void)
{
	t_opencl	*opencl;
	t_camera	camera;
	t_data		data;

	// create_objects_array(create_ll_objects());
	// vector_test();
	if (!initialize(&data, &camera))
		return (1);
	opencl = init_opencl(&data);
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