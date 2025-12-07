/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:38 by zyeoh             #+#    #+#             */
/*   Updated: 2024/08/22 20:32:42 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"



void print_cl_error(cl_int error)
{
    switch (error) {
        case CL_IMAGE_FORMAT_NOT_SUPPORTED: printf("Error: CL_IMAGE_FORMAT_NOT_SUPPORTED\n"); break;
        case CL_IMAGE_FORMAT_MISMATCH: printf("Error: CL_IMAGE_FORMAT_MISMATCH\n"); break;
        case CL_MEM_COPY_OVERLAP: printf("Error: CL_MEM_COPY_OVERLAP\n"); break;
        case CL_PROFILING_INFO_NOT_AVAILABLE: printf("Error: CL_PROFILING_INFO_NOT_AVAILABLE\n"); break;
        case CL_COMPILER_NOT_AVAILABLE: printf("Error: CL_COMPILER_NOT _AVAILABLE\n"); break;
        case CL_DEVICE_NOT_AVAILABLE: printf("Error: CL_DEVICE_NOT_AVAILABLE\n"); break;
        case CL_DEVICE_NOT_FOUND: printf("Error: CL_DEVICE_NOT_FOUND\n"); break;
        case CL_INVALID_KERNEL: printf("Error: CL_INVALID_KERNEL\n"); break;
        case CL_INVALID_WORK_DIMENSION: printf("Error: CL_INVALID_WORK_DIMENSION\n"); break;
        case CL_INVALID_WORK_GROUP_SIZE: printf("Error: CL_INVALID_WORK_GROUP_SIZE\n"); break;
        case CL_INVALID_WORK_ITEM_SIZE: printf("Error: CL_INVALID_WORK_ITEM_SIZE\n"); break;
        case CL_INVALID_GLOBAL_OFFSET: printf("Error: CL_INVALID_GLOBAL_OFFSET\n"); break;
        case CL_OUT_OF_RESOURCES: printf("Error: CL_OUT_OF_RESOURCES\n"); break;
        case CL_MEM_OBJECT_ALLOCATION_FAILURE: printf("Error: CL_MEM_OBJECT_ALLOCATION_FAILURE\n"); break;
        case CL_INVALID_EVENT_WAIT_LIST: printf("Error: CL_INVALID_EVENT_WAIT_LIST\n"); break;
        case CL_OUT_OF_HOST_MEMORY: printf("Error: CL_OUT_OF_HOST_MEMORY\n"); break;
        case CL_INVALID_VALUE: printf("Error: CL_INVALID_VALUE\n"); break;
        default: printf("Unknown OpenCL error: %d\n", error);
    }
}

t_camera	*init_camera(t_data *data, int win_height, int win_width)
{
	t_camera		*camera;
	cl_float		aspect_ratio;

	camera = ft_calloc(1, sizeof(t_camera));
	if (!camera)
		exit(EXIT_FAILURE);
  *camera = get_camera(data->file_content);
	camera->pos = (cl_float4) {{0, 0, 0}};
	camera->quat = (cl_float4) {{0, 0, 0, 1}};
	// fov = FOV * TO_RADIAN;
	aspect_ratio = (cl_float)win_width / win_height;
	camera->pixel_width = 2 * tan(camera->fov * 0.5f);
	camera->pixel_height = camera->pixel_width / aspect_ratio;
	camera->win_width = win_width;
	camera->win_height = win_height;
	camera->bytes_per_pixel = data->bits_per_pixel * 0.125f;
	camera->line_length = data->line_length;
	return (camera);
}

t_opencl	*init_opencl(t_data *data)
{
	char		**c_files;
	size_t		c_size[9];
	t_opencl	*opencl;
  cl_int ret;

	opencl = ft_calloc(1, sizeof(t_opencl));
	if (!opencl)
		exit(EXIT_FAILURE);

	c_files = ft_calloc(9, sizeof(char *));
	c_files[0] = read_cfile("srcs/opencl_srcs/opencl.h");
	c_files[1] = read_cfile("srcs/opencl_srcs/ray.c"); // * load GPU source files
	c_files[2] = read_cfile("srcs/opencl_srcs/opencl_vector.c");
	c_files[3] = read_cfile("srcs/opencl_srcs/opencl_quaternion.c");
	c_files[4] = read_cfile("srcs/opencl_srcs/opencl_object_intercept.c");
	c_files[5] = read_cfile("srcs/opencl_srcs/brdf.c");
	c_files[6] = read_cfile("srcs/opencl_srcs/bxdf.c");
	c_files[7] = read_cfile("srcs/opencl_srcs/utils.c");
	c_size[0] = ft_strlen(c_files[0]);
	c_size[1] = ft_strlen(c_files[1]);
	c_size[2] = ft_strlen(c_files[2]);
	c_size[3] = ft_strlen(c_files[3]);
	c_size[4] = ft_strlen(c_files[4]);
	c_size[5] = ft_strlen(c_files[5]);
	c_size[6] = ft_strlen(c_files[6]);
	c_size[7] = ft_strlen(c_files[7]);
  printf("Loaded c_files\n");

	ret = clGetPlatformIDs(2, opencl->platform, NULL);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);
  ret = clGetDeviceIDs(opencl->platform[0], CL_DEVICE_TYPE_GPU, 1, &opencl->device, NULL);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);

  printf("Retrieved platformID and deviceID\n");

	// size_t maxWorkGroupSize;
	// clGetDeviceInfo(opencl->device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &maxWorkGroupSize, NULL);
	// printf("Maximum Workgroup Size: %zu\n", maxWorkGroupSize);

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

  // Create a program from the kernel source
  opencl->program = clCreateProgramWithSource(opencl->context, 8, (const char **)c_files, c_size, &ret);
	if (ret != CL_SUCCESS)
		print_cl_error(ret);

  printf("Loaded program\n");

  // Add these before clBuildProgram
  cl_device_info power_info;
  ret = clGetDeviceInfo(opencl->device, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, 
                     sizeof(power_info), &power_info, NULL);

  // Modify your clBuildProgram call to include build options
  const char *build_options = "-cl-opt-disable -cl-strict-aliasing";
  ret = clBuildProgram(opencl->program, 1, &opencl->device, build_options, NULL, NULL);

  // Build the program
  ret = clBuildProgram(opencl->program, 1, &opencl->device, NULL, NULL, NULL);
	if (ret != CL_SUCCESS)
	{
		printf("clBuildProgram error: %d\n", ret);
		print_cl_error(ret);
		
		// Get the build log
		size_t log_size;
		clGetProgramBuildInfo(opencl->program, opencl->device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
		char *log = (char *)malloc(log_size);
		clGetProgramBuildInfo(opencl->program, opencl->device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
		printf("Build log:\n%s\n", log);
		free(log);
		
		exit(EXIT_FAILURE);
	}

  printf("Program Built\n");

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
  data->opencl = opencl;
  free_cfile(c_files);
	return (opencl);
}


int	initialize(t_data *data, char *filename)
{
  data->file_content = get_rt_file(filename); 
	data->objects = get_objects(data, data->file_content);
	data->mlx_ptr = mlx_init();
	if (!data->mlx_ptr)
		return (0);
	data->win_width = 1980;
	data->win_height = 1080;
	data->img = mlx_new_image(data->mlx_ptr, data->win_width, data->win_height);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_length, &data->endian);
	data->win_ptr = mlx_new_window(data->mlx_ptr, data->win_width, data->win_height, "miniRT");
	ft_bzero(&data->inputs, sizeof(t_inputs));
	data->inputs.key = -1;
	data->inputs.mouse_x = data->win_width * 0.5f;
	data->inputs.mouse_y = data->win_height * 0.5f;
	data->inputs.pitch_angle = 0;
	data->camera = init_camera(data, data->win_height, data->win_width);
  mlx_mouse_move(data->mlx_ptr, data->win_ptr, data->inputs.mouse_x, data->inputs.mouse_y);

  // t_object *temp = data->objects;
  // printf("%c %x, %F %F %F, %F %F %F, %f\n", temp->type, temp->color, (double)(temp->pos.s[0]), (double)(temp->pos.s[1]), (double)temp->pos.s[2], (double)temp->quat.s[0], (double)temp->quat.s[1], (double)temp->quat.s[2], temp->sphere.radius);
  data->inputs.update = 1;
  data->camera = init_camera(data, data->win_height, data->win_width);
	// data->num_objects = ;
  printf("Init done\n");
  // exit(1);
	data->opencl = init_opencl(data);

	return (1);
}

// t_data *data,

int	render_frame(t_data *data)
{
  t_opencl *opencl;
	cl_int ret;
	size_t global_size[2];
	// size_t local_size[2];
	double	time_start;
  static double average = 0;
  // static int count = 0;

  // printf("Start render_frame\n");
  deal_input(data);
  // if (!data->inputs.update)
  //   return 0;
  // printf("Dealt input\n");

  opencl = data->opencl;
	
	//execution
	// printf("%f %f %F %F\n", data->camera->pos.s[0], data->camera->pos.s[1], data->camera->pos.s[2], data->camera->pos.s[3]);
	global_size[0] = 1;
	global_size[1] = 1;
	global_size[0] = data->win_width;
	global_size[1] = data->win_height;
  // printf("Init render_frame local var\n");
	// local_size[0] = 16;
	// local_size[1] = 16;
	time_start = (double)clock() / CLOCKS_PER_SEC;

  // printf("Clock in\n");
	
  ret = clEnqueueWriteBuffer(opencl->queue, opencl->camera, CL_TRUE, 0, sizeof(t_camera), data->camera, 0, NULL, NULL);
	if (ret != CL_SUCCESS)
	{
		printf("ret 1 error: %d\n", ret);
		print_cl_error(ret);
	}
	ret = clEnqueueWriteBuffer(opencl->queue, opencl->objects, CL_TRUE, 0, sizeof(t_object) * data->num_objects, data->objects, 0, NULL, NULL); // ! hard coded
	if (ret != CL_SUCCESS)
	{
		printf("ret 2 error: %d\n", ret);
		print_cl_error(ret);
	}
  ret = clEnqueueNDRangeKernel(opencl->queue, opencl->kernel, 2, NULL, global_size, NULL, 0, NULL, NULL);
	if (ret != CL_SUCCESS)
	{
		printf("ret 3 error: %d\n", ret);
		print_cl_error(ret);
	}
	ret = clEnqueueReadBuffer(opencl->queue, opencl->addr, CL_TRUE, 0, sizeof(int) * data->win_width * data->win_height, data->addr, 0, NULL, NULL);
	if (ret != CL_SUCCESS)
	{
		printf("ret 4 error: %d\n", ret);
		print_cl_error(ret);
	}
	// ret = clEnqueueReadBuffer(opencl->queue, opencl->reservoirs, CL_TRUE, 0, sizeof(t_reservoir) * data->win_width * data->win_height, data->reservoirs, 0, NULL, NULL);
	// if (ret != CL_SUCCESS)
	// {
	// 	printf("ret 5 error: %d\n", ret);
	// 	print_cl_error(ret);
	// }
  // printf("Queued all Buffers\n");

	average = (double)clock() / CLOCKS_PER_SEC - time_start;
  while ((double)clock() / CLOCKS_PER_SEC - time_start < 0.0020f)
		usleep(50);

	printf("%f\n", 1 / average);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img, 0, 0);
  data->inputs.update = 0;
  data->camera->moved = false;
  return 0;
}
// fflush(stdout);
// print_vector(ray.direction);

int	main(int ac, char **av)
{
	t_data		data;

	if (ac != 2 || !initialize(&data, av[1]))
		return (1);
	mlx_mouse_hide(data.mlx_ptr, data.win_ptr);
	mlx_hook(data.win_ptr, 2, 1L << 0, (void*)deal_key_press, &data);
	mlx_hook(data.win_ptr, 3, 1L << 1, (void*)deal_key_release, &data);
	// mlx_hook(data.win_ptr, 0, 0,deal_input, &data);
  mlx_hook(data.win_ptr, 6, 1L << 6, (void*)mouse_hook, &data);
	mlx_loop_hook(data.mlx_ptr, (void*)render_frame, &data);
  mlx_loop(data.mlx_ptr);
	return (0);
}

