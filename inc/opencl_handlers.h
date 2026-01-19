/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_handlers.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 13:41:56 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/19 19:56:11 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPENCL_HANDLERS_H
# define OPENCL_HANDLERS_H

# define TOTAL_C_FILES 13

typedef struct s_opencl
{
	cl_platform_id		platform[2]; //which openCL
	cl_device_id		device; // which hardware
	cl_context			context; // env for OpenCL operations
	cl_command_queue	queue; // Sequence of commands
	cl_program			program; // holds kernel src codes
	cl_kernel			kernel; // A fucntion written in OpenCL C
	cl_mem				addr; // the result back to host
	cl_mem				camera;
	cl_mem				objects;
	cl_mem				reservoirs;
}						t_opencl;

char	**get_cfiles(size_t *c_size);
void	print_cl_error(cl_int error);

void	get_platform_and_devices(t_opencl *opencl,
			cl_device_id *gpu_list, cl_platform_id *platform_list,
			cl_uint total_gpus);

void	get_context(t_opencl *opencl, t_data *data);

void	get_program(t_opencl *opencl, char **c_files, size_t c_size[9]);

void	get_kernel(t_opencl *opencl);

void	queue_buffer(t_opencl *opencl, t_data *data);
#endif
