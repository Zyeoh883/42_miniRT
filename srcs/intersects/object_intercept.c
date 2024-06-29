/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_intercept.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:13:56 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/29 22:42:08 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// TODO validate aabb SIMB SSE
int	intersect_aabb(t_ray ray, t_vector max, t_vector min)
{

	__m128 t0;
    __m128 t1;
	__m128 tmin;
    __m128 tmax;
	__m128 mask;

	t0 = _mm_div_ps(_mm_sub_ps(min, *ray.pos), ray.direction);
    t1 = _mm_div_ps(_mm_sub_ps(max, *ray.pos), ray.direction);

	tmin = _mm_min_ps(t0, t1);
   	tmax = _mm_max_ps(t0, t1);

	mask = _mm_cmpgt_ps(tmax, _mm_setzero_ps());
    mask = _mm_or_ps(mask, _mm_cmpgt_ps(tmin, _mm_setzero_ps()));

	return (_mm_movemask_ps(mask) != 0);
}

// int	intersect_aabb(t_ray ray, t_vector max, t_vector min)
// {
// 	float	t[2];
// 	float	temp[2];

// 	t[0] = (min.i - ray.pos->i) / ray.direction.i;
// 	t[1] = (max.i - ray.pos->i) / ray.direction.i;
// 	if (t[0] > t[1])
// 		swap_floats(&t[1], &t[0]);
// 	temp[0] = (min.j - ray.pos->j) / ray.direction.j;
// 	temp[1] = (max.j - ray.pos->j) / ray.direction.j;
// 	if (temp[0] > temp[1])
// 		swap_floats(&temp[0], &temp[1]);
// 	if (t[0] > temp[1] || temp[0] > t[1])
// 		return (false);
// 	if (temp[0] > t[0])
// 		t[0] = temp[0];
// 	if (temp[1] > t[1])
// 		t[1] = temp[1];
// 	temp[0] = (min.k - ray.pos->k) / ray.direction.k;
// 	temp[1] = (max.k - ray.pos->k) / ray.direction.k;
// 	if (temp[0] > temp[1])
// 		swap_floats(&temp[0], &temp[1]);
// 	if (t[0] > temp[1] || temp[0] > t[1])
// 		return (false);
// 	return (t[1] > 0 || temp[1] > 0);
// }

// int	intersect_obb(t_ray ray, float t[2])
// {
// 	t_vector local_origin;
// 	t_vector local_direction;

// }

void	intersect_ray_sphere(t_sphere sphere, t_ray ray, float t[2])
{
	t_vector	sphere_to_camera;
	float		a;
	float		b;
	float		c;
	float		discriminant;

	sphere_to_camera = vector_subtraction(*ray.pos, sphere.pos);
	// printf("3. ");
	// print_vector(*ray.pos);
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
