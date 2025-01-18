/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_struct.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:16:07 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/15 15:42:11 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_STRUCT_H
# define DATA_STRUCT_H

# define SPHERE 'S'
# define PLANE 'P'
# define CYCLINDER 'C'
# define LIGHT 'L'

typedef struct __attribute__ ((aligned(16))) s_ray
{
	cl_float3		pos; // ! changed from pointer to value
	cl_float3		dir;
}						t_ray;

typedef struct __attribute__ ((aligned(16))) s_sphere
{
	cl_float		radius;
}						t_sphere;

typedef struct __attribute__ ((aligned(16))) s_cyclinder
{
	cl_float		radius;
	cl_float		height;
}						t_cyclinder;

typedef struct __attribute__ ((aligned(16))) s_OBB
{
	cl_float4		pos;
	cl_float4		quat;
	cl_float4		half_len;
}						t_OBB;

	// struct s_OBB obb;
typedef struct __attribute__ ((aligned(16))) s_object
{
	cl_uchar		obj_type;
	cl_uchar		mat_type; // D, M, G, R, P
	cl_int			color;
	cl_float3		pos;
	cl_float3		dir;
  cl_float    k_ambient;
  cl_float3   albedo; //diffuse for lambartian_BRDF
  cl_float3   F_0; // basic_reflectivity; // F_0 Freshnel component
  cl_float    roughness_sqr; // NDF and geometric component
  cl_float    metallic; // diffuse and specular blending
  cl_float    k_specular;
  cl_float    emission; 
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

/*// light*/
/* typedef struct __attribute__ ((aligned(16))) s_light {*/
/**/
/* }          t_light;*/
/**/
#endif
