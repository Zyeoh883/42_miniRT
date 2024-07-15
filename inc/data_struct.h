/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_struct.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:16:07 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/15 15:04:30 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_STRUCT_H
# define DATA_STRUCT_H

# define SPHERE 'S'
# define PLANE 'P'

typedef struct __attribute__ ((aligned(16))) s_ray
{
	cl_float4		pos; // ! changed from pointer to value
	cl_float4		direction;
}						t_ray;

typedef struct __attribute__ ((aligned(16))) s_sphere
{
	cl_float		radius;
}						t_sphere;

// typedef struct __attribute__ ((aligned(16))) s_plane
// {
// }						t_plane;

typedef struct __attribute__ ((aligned(16))) s_OBB
{
	cl_float4		pos;
	cl_float4		quat;
	cl_float4		half_len;
}						t_OBB;

	// struct s_OBB obb;
typedef struct __attribute__ ((aligned(16))) s_object
{
	cl_uchar		type;
	cl_int			color;
	cl_float4		pos;
	cl_float4		quat;
	union
	{
		struct s_sphere	sphere;
	};
}						t_object;

typedef struct __attribute__ ((aligned(16))) s_BHVnode
{
	struct s_OBB		obb;
	struct s_BHVnode	*left;
	struct s_BHVnode	*right;
	struct t_object		*object;
	int					object_count;
}						t_BHVnode;

// __attribute__((packed,aligned(4))
// __attribute__ ((aligned(16)))

#endif