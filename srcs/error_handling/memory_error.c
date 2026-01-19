/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:50:59 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/16 17:56:01 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
// void	print_cl_error(cl_int error)
// {
// 	switch (error)
// 	{
// 	case CL_IMAGE_FORMAT_NOT_SUPPORTED:
// 		printf("Error: CL_IMAGE_FORMAT_NOT_SUPPORTED\n");
// 		break ;
// 	case CL_IMAGE_FORMAT_MISMATCH:
// 		printf("Error: CL_IMAGE_FORMAT_MISMATCH\n");
// 		break ;
// 	case CL_MEM_COPY_OVERLAP:
// 		printf("Error: CL_MEM_COPY_OVERLAP\n");
// 		break ;
// 	case CL_PROFILING_INFO_NOT_AVAILABLE:
// 		printf("Error: CL_PROFILING_INFO_NOT_AVAILABLE\n");
// 		break ;
// 	case CL_COMPILER_NOT_AVAILABLE:
// 		printf("Error: CL_COMPILER_NOT _AVAILABLE\n");
// 		break ;
// 	case CL_DEVICE_NOT_AVAILABLE:
// 		printf("Error: CL_DEVICE_NOT_AVAILABLE\n");
// 		break ;
// 	case CL_DEVICE_NOT_FOUND:
// 		printf("Error: CL_DEVICE_NOT_FOUND\n");
// 		break ;
// 	case CL_INVALID_KERNEL:
// 		printf("Error: CL_INVALID_KERNEL\n");
// 		break ;
// 	case CL_INVALID_WORK_DIMENSION:
// 		printf("Error: CL_INVALID_WORK_DIMENSION\n");
// 		break ;
// 	case CL_INVALID_WORK_GROUP_SIZE:
// 		printf("Error: CL_INVALID_WORK_GROUP_SIZE\n");
// 		break ;
// 	case CL_INVALID_WORK_ITEM_SIZE:
// 		printf("Error: CL_INVALID_WORK_ITEM_SIZE\n");
// 		break ;
// 	case CL_INVALID_GLOBAL_OFFSET:
// 		printf("Error: CL_INVALID_GLOBAL_OFFSET\n");
// 		break ;
// 	case CL_OUT_OF_RESOURCES:
// 		printf("Error: CL_OUT_OF_RESOURCES\n");
// 		break ;
// 	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
// 		printf("Error: CL_MEM_OBJECT_ALLOCATION_FAILURE\n");
// 		break ;
// 	case CL_INVALID_EVENT_WAIT_LIST:
// 		printf("Error: CL_INVALID_EVENT_WAIT_LIST\n");
// 		break ;
// 	case CL_OUT_OF_HOST_MEMORY:
// 		printf("Error: CL_OUT_OF_HOST_MEMORY\n");
// 		break ;
// 	case CL_INVALID_VALUE:
// 		printf("Error: CL_INVALID_VALUE\n");
// 		break ;
// 	default:
// 		printf("Unknown OpenCL error: %d\n", error);
// 	}
// }

void	print_cl_error(cl_int error)
{
	printf("cl_error: %d\n", error);
}

void	perror_and_exit(char *str, int exit_code)
{
	ft_putstr_fd("minirt: ", STDERR_FILENO);
	ft_putendl_fd(str, STDERR_FILENO);
	perror(str);
	exit(exit_code);
}

void	error_and_exit(char *str, int exit_code)
{
	ft_putstr_fd("minirt: ", STDERR_FILENO);
	ft_putendl_fd(str, STDERR_FILENO);
	exit(exit_code);
}
