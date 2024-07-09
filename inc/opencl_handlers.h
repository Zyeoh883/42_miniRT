/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_handlers.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 13:41:56 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/09 17:56:35 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPENCL_HANDLERS_H
# define OPENCL_HANDLERS_H

# include <OpenCL/opencl.h>

typedef struct s_opencl
{
	cl_platform_id		platform; //which openCL
	cl_device_id		device; // which hardware
	cl_context			context; // env for OpenCL operations
	cl_command_queue	queue; // Sequence of commands
	cl_program			program; // holds kernel src codes
	cl_kernel			kernel; // A fucntion written in OpenCL C
	cl_mem				image_buffer; // the result back to host
}						t_opencl;



#endif