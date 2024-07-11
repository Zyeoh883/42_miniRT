/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:33:56 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/11 10:35:11 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

#define u

// (2 * i / width) lies between 0 to 2
// (2 * i / width - 1) lies between -1 to 1, an offset is added

t_ray	create_ray(t_camera *camera, cl_int i, cl_int j)
{
	t_ray	ray;
	// cl_int	height;
	// cl_int	width;

	ray.pos = camera->pos;
	ray.direction = (cl_float4){{
		1,
		(1 - 2 * j / camera->win_height) * camera->pixel_height,
		(2 * i / camera->win_width - 1) * camera->pixel_width,
		0
	}};
	// ray.direction = quat_rotate(camera->quat, ray.direction);
	// vector_normalize(&ray.direction);
	// print_m128(ray.direction);
	return (ray);
}

int	render_ray(t_ray ray, t_object *objects)
{
	float		t[2];
	t_sphere	*closest_sphere;
	float		closest_t;
	int			n;

	closest_sphere = NULL;
	closest_t = INFINITY;
	// printf("2. ");
	// print_vector(*ray.pos);
	n = 0;
	while (objects->type != 0)
	{
		intersect_ray_sphere(objects->sphere, ray, t);
		if (t[0] < closest_t && t[0] > 0)
		{
			closest_t = t[0];
			closest_sphere = &objects->sphere;
		}
		if (t[1] < closest_t && t[1] > 0)
		{
			closest_t = t[1];
			closest_sphere = &objects->sphere;
		}
		objects++;
	}
	// if (n == 1)
	// 	return (0xFFFFFF);
	// else
	// 	return (0);
	if (closest_sphere == NULL)
		return (1);
	return (closest_sphere->color);
}

__kernel void	render_scene(u __global void *addr, u __global t_camera *camera,
	u __global t_object *objects)
{
	// t_ray	ray;
	cl_char	*dst;
	cl_int color;
	cl_int	i;
	cl_int	j;

	i = get_global_id(0);
	j = get_global_id(1);
	// ray = create_ray(camera, i, j);
	// color = render_ray(create_ray(camera, i, j), objects);
	// if (color)
		// put color in image
	dst = addr + (y * camera->line_length + x * (camera->bytes_per_pixel));
	*(unsigned int *)dst = 0xFFFFFF;
}
