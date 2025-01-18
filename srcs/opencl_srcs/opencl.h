/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 15:30:40 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/15 14:57:55 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define U

// #define M_PI 3.141592653589793238f

#define SPHERE 'S'
#define PLANE 'P'
#define LIGHT 'L'

typedef struct __attribute__((aligned(16))) s_camera
{
	float3	pos;
	float4	quat;
	float	pixel_width;
	float	pixel_height;
	int		win_height;
	int		win_width;
	char	num_objects;
	int		bytes_per_pixel;
	int		line_length;
}				t_camera;

typedef struct __attribute__((aligned(16))) s_ray
{
	float3	pos;
	float3	dir;
}				t_ray;

typedef struct __attribute__((aligned(16))) s_sphere
{
	float	radius;
}				t_sphere;

typedef struct __attribute__((aligned(16))) s_object
{
	uchar		obj_type;
  uchar   mat_type;
	int			color;
	float3	pos;
	float3	dir;
  float   k_ambient;
  float3  albedo;
  float3  F_0;
  float   roughness_sqr;
  float   metallic;
  float   k_specular;
  float   emission; 
	union
	{
		struct s_sphere	sphere;
	};
}				t_object;

float3			vector_normalize(float3 v);
t_ray			create_ray(U __global t_camera *camera, int i, int j);
int				render_ray(t_ray ray, U __global t_object *objects);
float3			vector_scalar_product(const float3 v, const float scalar);
float2			intersect_ray_sphere(U __global t_object *object, t_ray ray);
float3			quat_rotate(float4 q, const float3 q_v);
float2			ray_intersection(U __global t_object *object, t_ray ray);

float3       BRDF(float3 in, float3 out, float3 normal, U __global t_object *hit_object);
