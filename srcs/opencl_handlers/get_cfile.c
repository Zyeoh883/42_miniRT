/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cfile.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:38 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/16 18:26:35 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

char	**get_cfiles(size_t c_size[9])
{
	char	**c_files;

	c_files = ft_calloc(9, sizeof(char *));
	if (!c_files)
		perror_and_exit("Malloc for c_files failed", EXIT_FAILURE);
	c_files[0] = read_cfile("srcs/opencl_srcs/opencl.h");
	c_files[1] = read_cfile("srcs/opencl_srcs/ray.c");
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
	return (c_files);
}
