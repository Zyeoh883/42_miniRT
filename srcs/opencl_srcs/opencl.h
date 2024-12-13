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
#define SPHERE 'S'
#define PLANE 'P'

typedef struct __attribute__((aligned(16))) s_camera
{
	float4	pos;
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
	float4	pos;
	float4	direction;
}				t_ray;

typedef struct __attribute__((aligned(16))) s_sphere
{
	float	radius;
}				t_sphere;

typedef struct __attribute__((aligned(16))) s_object
{
	uchar		type;
	int			color;
	float4		pos;
	float4		quat;
  float3   k_ambient;
  float3   k_diffuse;
  float3   k_specular;
	union
	{
		struct s_sphere	sphere;
	};
}				t_object;

float4			vector_normalize(float4 v);
t_ray			create_ray(U __global t_camera *camera, int i, int j);
int				render_ray(t_ray ray, U __global t_object *objects);
float4			vector_scalar_product(const float4 v, const float scalar);
float2			intersect_ray_sphere(U __global t_object *object, t_ray ray);
float4			quat_rotate(float4 q, const float4 v);
float2			ray_intersection(U __global t_object *object, t_ray ray);
