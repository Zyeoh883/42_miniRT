/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_struct.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:16:07 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/19 19:47:02 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_STRUCT_H
# define DATA_STRUCT_H

# define CL_SPHERE 'S'
# define CL_PLANE 'P'
# define CL_CYCLINDER 'C'
# define CL_LIGHT 'L'

typedef struct __attribute__((aligned(16))) s_ray
{
	cl_float3	pos; // ! changed from pointer to value
	cl_float3	dir;
}							t_ray;

typedef struct __attribute__((aligned(16))) s_sphere
{
	cl_float	radius;
}							t_sphere;

typedef struct __attribute__((aligned(16))) s_cylinder
{
	cl_float	radius;
	cl_float	height;
}							t_cyclinder;

typedef struct __attribute__((aligned(16))) s_OBB
{
	cl_float3	quat;
	cl_float3	half_len;
}							t_OBB;

typedef struct __attribute__((aligned(16))) s_object
{
	/*struct s_OBB		obb;*/
	/*cl_uchar		mat_type; // D, M, G, R, P*/
	cl_uchar	obj_type;
	cl_float3	pos;
	cl_float3	dir;
	cl_float3	diffuse_albedo;	// diffuse for lambartian_BRDF
	cl_float3	specular_albedo;
	cl_float3	f_0; // basic_reflectivity; // f_0 Freshnel component
	cl_float	roughness_sqr; // NDF and geometric component
	cl_float3	emission;
	union
	{
		struct s_sphere		sphere;
		struct s_cylinder	cylinder;
	};
}							t_object;

typedef struct __attribute__((aligned(16))) s_BHVnode
{
	struct s_OBB		obb;
	struct s_BHVnode	*left;
	struct s_BHVnode	*right;
	struct t_object		*object;
	int					object_count;
}							t_BHVnode;

//   cl_float3 incident_direction;
typedef struct __attribute__((aligned(16))) s_candidate
{
	cl_float3	radiance;
	cl_float3	weight;
	float		pdf;

}							t_candidate;

typedef struct __attribute__((aligned(16))) s_reservoir
{
	t_candidate	candidate;
	float		weighted_sum;
	uint		m;
	uint		seed;
}							t_reservoir;

// __attribute__((packed,aligned(4))
// __attribute__ ((aligned(16)))

// light*/
/* typedef struct __attribute__ ((aligned(16))) s_light {*/
/**/
/* }          t_light;*/
/**/
#endif
