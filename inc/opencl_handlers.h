/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_handlers.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 13:41:56 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/11 10:15:17 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPENCL_HANDLERS_H
# define OPENCL_HANDLERS_H

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
}						t_opencl;

#endif
