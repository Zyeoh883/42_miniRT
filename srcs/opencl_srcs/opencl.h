/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 15:30:40 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/14 15:41:54 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define U

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
	float4	pos;
	float4	quat;
	float	radius;
	int		color;
}				t_sphere;

typedef struct __attribute__((aligned(16))) s_object
{
	uchar	type;
	union
	{
		struct s_sphere	sphere;
	};
}				t_object;

float4			vector_normalize(float4 v);
t_ray			create_ray(__global t_camera *camera, int i, int j);
int				render_ray(t_ray ray, __global t_object *objects);
float4			vector_scalar_product(const float4 v, const float scalar);
float2			intersect_ray_sphere(t_sphere sphere, t_ray ray);
float4			quat_rotate(float4 q, const float4 v);
