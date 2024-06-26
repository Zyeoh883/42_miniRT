/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_intercept.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:13:56 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/26 18:25:29 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// void	intersect_aabb(t_camera camera, t_OBB obb, t_ray ray)
// {
// 	t_vector	sphere_to_camera;
// 	float		a;
// 	float		b;
// 	float		c;
// 	float		discriminant;

// 	if (discriminant < 0)
// 	{
// 		t[0] = INFINITY;
// 		t[1] = INFINITY;
// 		return ;
// 	}
// 	discriminant = sqrt(discriminant);
// 	t[0] = (-b + discriminant) / (2 * a);
// 	t[1] = (-b - discriminant) / (2 * a);
// }

// void	intersect_obb(t_camera camera, t_OBB obb, t_ray ray, float t[2])
// {
// 	t_vector	sphere_to_camera;
// 	float		a;
// 	float		b;
// 	float		c;
// 	float		discriminant;

// 	if (discriminant < 0)
// 	{
// 		t[0] = INFINITY;
// 		t[1] = INFINITY;
// 		return ;
// 	}
// 	discriminant = sqrt(discriminant);
// 	t[0] = (-b + discriminant) / (2 * a);
// 	t[1] = (-b - discriminant) / (2 * a);
// }

void	intersect_ray_sphere(t_sphere sphere, t_ray ray, float t[2])
{
	t_vector	sphere_to_camera;
	float		a;
	float		b;
	float		c;
	float		discriminant;

	sphere_to_camera = vector_subtraction(*ray.pos, sphere.pos);
	printf("3. ");
	print_vector(*ray.pos);
	a = vector_dot_product(ray.direction, ray.direction);
	b = 2 * vector_dot_product(sphere_to_camera, ray.direction);
	c = vector_dot_product(sphere_to_camera, sphere_to_camera) - (sphere.radius
			* sphere.radius);
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
