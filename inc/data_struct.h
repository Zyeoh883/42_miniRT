/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_struct.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:16:07 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/11 15:16:24 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_STRUCT_H
# define DATA_STRUCT_H

# define SPHERE "S"

// #define t_vector __m128
// #define t_quat __m128

// TODO change all t_ vector and quat to __m128
// * reverse order for __m128
// typedef __attribute__((aligned(16))) __m128	t_vector; // {k, j, i, NULL}
// typedef __attribute__((aligned(16))) __m128	t_quat; // {k, j, i, w}

// typedef struct s_vector
// {
// 	float				i;
// 	float				j;
// 	float				k;
// }						t_vector;

// typedef struct s_quat
// TODO set w to fourth value for speed (small improvement)
// {
// 	float				w;
// 	float				i;
// 	float				j;
// 	float				k;
// }						t_quat;

typedef struct __attribute__ ((aligned(16))) s_ray
{
	cl_float4	pos; // ! changed from pointer to value
	cl_float4	direction;
}						t_ray;

typedef struct __attribute__ ((aligned(16))) s_sphere
{
	cl_float4	pos;
	cl_float4	quat;
	cl_float		radius;
	cl_int			color;
}						t_sphere;

typedef struct __attribute__ ((aligned(16))) s_OBB // ! confirm OBB params
{
	cl_float4	pos;
	cl_float4	quat;
	cl_float4	half_len;
}						t_OBB;

	// struct s_OBB obb;
typedef struct __attribute__ ((aligned(16))) s_object // ! type == 0 means NULL terminated
{
	cl_uchar				type;
	union
	{
		struct __attribute__ ((aligned(16))) s_sphere	sphere;
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