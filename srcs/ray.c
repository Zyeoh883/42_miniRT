/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:33:56 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/24 15:02:29 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// (2 * i / width) lies between 0 to 2
// (2 * i / width - 1) lies between -1 to 1, an offset is added

t_ray	create_ray(t_camera camera, float i, float j, float width, float height)
{
	t_ray	ray;

	ray.position = (t_vector){camera.position.i, camera.position.j,
		camera.position.k};
	ray.direction.i = (2 * i / width - 1) * camera.pixel_width;
	ray.direction.j = (1 - 2 * j / height) * camera.pixel_height;
	ray.direction.k = 1;
	ray.direction = quat_rotate(camera.quat, ray.direction);
	vector_normalize(&ray.direction);
	// ray.direction = vector_normalize(ray.direction);
	return (ray);
}

void	intersect_ray_sphere(t_camera camera, t_sphere sphere, t_ray ray,
		float t[2])
{
	t_vector	sphere_to_camera;
	float		a;
	float		b;
	float		c;
	float		discriminant;

	sphere_to_camera = vector_subtraction(camera.position, sphere.position);
	a = vector_dot_product(ray.direction, ray.direction);
	b = 2 * vector_dot_product(sphere_to_camera, ray.direction);
	c = vector_dot_product(sphere_to_camera, sphere_to_camera) - (sphere.radius
			* sphere.radius);
	// calculate num of intersects
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
	{
		t[0] = INFINITY;
		t[1] = INFINITY;
		return ;
	}
	discriminant = sqrt(discriminant);
	t[0] = (-b + discriminant) / (2 * a);
	t[1] = (-b - discriminant) / (2 * a);
}

int	render_ray(t_camera camera, t_ray ray)
{
	t_sphere	sphere[2];
	float		t[2];
	t_sphere	*closest_sphere;
	float		closest_t;
	int			n;

	// rot
	sphere[0].quat = (t_quat){1, 0, 0, 0};
	// pos
	sphere[0].position = (t_vector){0, 0, 3};
	// radius
	sphere[0].radius = 1;
	// color
	sphere[0].color = 0xFFFFFF;
	// data,
	sphere[1].quat = (t_quat){1, 0, 0, 0};
	sphere[1].position = (t_vector){0, -1, 3};
	sphere[1].radius = 1;
	sphere[1].color = 0xFF0000;
	// looing througth spheres
	closest_sphere = NULL;
	closest_t = INFINITY;
	n = -1;
	while (++n < 2)
	{
		intersect_ray_sphere(camera, sphere[n], ray, t);
		if (t[0] < closest_t)
		{
			closest_t = t[0];
			closest_sphere = &sphere[n];
		}
		if (t[1] < closest_t)
		{
			closest_t = t[1];
			closest_sphere = &sphere[n];
		}
	}
	if (closest_sphere == NULL)
		return (0);
	return (closest_sphere->color);
}
