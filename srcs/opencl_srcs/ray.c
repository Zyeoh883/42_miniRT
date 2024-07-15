/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:33:56 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/15 15:18:08 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

t_ray	create_ray(U __global t_camera *camera, int i, int j)
{
	t_ray	ray;

	ray.pos = camera->pos;
	ray.direction = (float4)((2 * i / (float)camera->win_width - 1)
			* camera->pixel_width, (1 - 2 * j / (float)camera->win_height)
			* camera->pixel_height, 1, 0);
	ray.direction = quat_rotate(camera->quat, ray.direction);
	return (ray);
}

int	render_ray(t_ray ray, U __global t_object *objects)
{
	__global t_object	*closest_object;
	float2				t;
	float				closest_t;

	closest_object = 0;
	closest_t = INFINITY;
	while (objects->type != 0)
	{
		t = ray_intersection(objects, ray);
		if (t[0] < closest_t && t[0] > 0)
		{
			closest_t = t[0];
			closest_object = objects;
		}
		if (t[1] < closest_t && t[1] > 0)
		{
			closest_t = t[1];
			closest_object = objects;
		}
		objects++;
	}
	if (closest_object == 0)
		return (1);
	return (closest_object->color);
}

__kernel void	render_scene(U __global uchar *addr,
	U __global t_camera *camera, U __global t_object *objects)
{
	U __global uchar	*dst;
	int					color;
	int					x;
	int					y;

	x = get_global_id(0);
	y = get_global_id(1);
	color = render_ray(create_ray(camera, x, y), objects);
	dst = addr + (y * camera->line_length + x * (camera->bytes_per_pixel));
	if (color)
		*(__global unsigned int *)dst = color;
}

// __kernel void	render_scene(u __global uchar *addr,
		// u __global t_camera *camera,
// 	u __global t_object *objects)
// {
// 	// t_ray	ray;
// 	uchar	*dst;
// 	// int color;
// 	int	x;
// 	int	y;

// 	x = get_global_id(0);
// 	y = get_global_id(1);
// 	// ray = create_ray(camera, i, j);
// 	// color = render_ray(create_ray(camera, i, j), objects);
// 	// if (color)
// 		// put color in image
// 	dst = (uchar *)addr + (y * camera->line_length + x
			// * (camera->bytes_per_pixel));
// 	*(unsigned int *)dst = 0xFFFFFF;
// }

// #include "minirt.h"

// typedef struct __attribute__ ((aligned(16))) s_camera
// {
// 	float4	pos;
// 	float4	quat;
// 	float	pixel_width;
// 	float	pixel_height;
// 	int		win_height;
// 	int		win_width;
// 	char	num_objects;
// 	int		bytes_per_pixel;
// 	int		line_length;
// }						t_camera;

// typedef struct __attribute__ ((aligned(16))) s_ray
// {
// 	float4	pos; // ! changed from pointer to value
// 	float4	direction;
// }						t_ray;

// typedef struct __attribute__ ((aligned(16))) s_sphere
// {
// 	float4	pos;
// 	float4	quat;
// 	float		radius;
// 	int			color;
// }						t_sphere;

// typedef struct __attribute__ ((aligned(16)))  s_object
// {
// 	// struct __attribute__ ((aligned(16))) s_OBB obb;
// 	uchar				type;
// 	union
// 	{
// 		struct __attribute__ ((aligned(16))) s_sphere	sphere;
// 	};
// }						t_object;

// #define u

// t_ray	create_ray(t_camera *camera, int i, int j);
// // (2 * i / width) lies between 0 to 2
// // (2 * i / width - 1) lies between -1 to 1, an offset is added

// t_ray	create_ray(t_camera *camera, int i, int j)
// {
// 	t_ray	ray;
// 	// int	height;
// 	// int	width;

// 	ray.pos = camera->pos;
// 	ray.direction = (float4){
// 		1,
// 		(1 - 2 * j / camera->win_height) * camera->pixel_height,
// 		(2 * i / camera->win_width - 1) * camera->pixel_width,
// 		0
// 	};
// 	// ray.direction = quat_rotate(camera->quat, ray.direction);
// 	// vector_normalize(&ray.direction);
// 	// print_m128(ray.direction);
// 	return (ray);
// }

// int	render_ray(t_ray ray, t_object *objects)
// {
// 	float		t[2];
// 	t_sphere	*closest_sphere;
// 	float		closest_t;
// 	int			n;

// 	closest_sphere = 0;
// 	closest_t = INFINITY;
// 	// printf("2. ");
// 	// print_vector(*ray.pos);
// 	n = 0;
// 	while (objects->type != 0)
// 	{
// 		intersect_ray_sphere(objects->sphere, ray, t);
// 		if (t[0] < closest_t && t[0] > 0)
// 		{
// 			closest_t = t[0];
// 			closest_sphere = &objects->sphere;
// 		}
// 		if (t[1] < closest_t && t[1] > 0)
// 		{
// 			closest_t = t[1];
// 			closest_sphere = &objects->sphere;
// 		}
// 		objects++;
// 	}
// 	// if (n == 1)
// 	// 	return (0xFFFFFF);
// 	// else
// 	// 	return (0);
// 	if (closest_sphere == 0)
// 		return (1);
// 	return (closest_sphere->color);
// }

// __kernel void	render_scene(u __global uchar *addr,
		// u __global t_camera *camera,
// 	u __global t_object *objects)
// {
// 	// t_ray	ray;
// 	uchar	*dst;
// 	// int color;
// 	int	x;
// 	int	y;

// 	x = get_global_id(0);
// 	y = get_global_id(1);
// 	// ray = create_ray(camera, i, j);
// 	// color = render_ray(create_ray(camera, i, j), objects);
// 	// if (color)
// 		// put color in image
// 	dst = (uchar *)addr + (y * camera->line_length + x
			// * (camera->bytes_per_pixel));
// 	*(unsigned int *)dst = 0xFFFFFF;
// }
