/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_intercept.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:13:56 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/12 15:34:04 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// int intersect_aabb(t_ray *ray, __m128 max, __m128 min) {
//     // Compute t0 and t1 using FMA if available
//     __m128 t0 = _mm_div_ps(_mm_sub_ps(min, *ray->pos), ray->direction);
//     __m128 t1 = _mm_div_ps(_mm_sub_ps(max, *ray->pos), ray->direction);

//     // Compute tmin and tmax
//     __m128 tmin = _mm_min_ps(t0, t1);
//     __m128 tmax = _mm_max_ps(t0, t1);

//     // Find the largest tmin and the smallest tmax
//     tmin = _mm_max_ps(_mm_max_ps(tmin, _mm_shuffle_ps(tmin, tmin, _MM_SHUFFLE(0, 3, 2, 1))), 
//                       _mm_shuffle_ps(tmin, tmin, _MM_SHUFFLE(0, 1, 3, 2)));
//     tmax = _mm_min_ps(_mm_min_ps(tmax, _mm_shuffle_ps(tmax, tmax, _MM_SHUFFLE(0, 3, 2, 1))), 
//                       _mm_shuffle_ps(tmax, tmax, _MM_SHUFFLE(0, 1, 3, 2)));

//     // Check for intersection
//     __m128 cond = _mm_and_ps(_mm_cmpge_ps(tmax, tmin), _mm_cmpge_ps(tmax, _mm_setzero_ps()));

//     // Only consider the first 3 components (k, j, i)
//     return ((_mm_movemask_ps(cond) & 0x7) == 0x7);
// }

static __m128 get_min(__m128 tmin)
{
	return (_mm_max_ps(_mm_max_ps(tmin, _mm_shuffle_ps(tmin, tmin, _MM_SHUFFLE(0, 3, 2, 1))), 
                                    _mm_shuffle_ps(tmin, tmin, _MM_SHUFFLE(0, 1, 3, 2))));
}

static __m128 get_max(__m128 tmax)
{
	return (_mm_min_ps(_mm_min_ps(tmax, _mm_shuffle_ps(tmax, tmax, _MM_SHUFFLE(0, 3, 2, 1))), 
                                    _mm_shuffle_ps(tmax, tmax, _MM_SHUFFLE(0, 1, 3, 2))));
}

// TODO validate aabb SIMB SSE

int intersect_aabb(t_ray *ray, __m128 max, __m128 min) {
    __m128 t0;
	__m128 t1;
	__m128 tmin;
	__m128 tmax;
	__m128 is_zero;
    
    is_zero = _mm_cmpeq_ps(ray->direction, _mm_setzero_ps());
    tmin = _mm_div_ps(_mm_set1_ps(1.0f), _mm_blendv_ps(ray->direction, _mm_set1_ps(1.0f), is_zero));

    t0 = _mm_mul_ps(_mm_sub_ps(min, *ray->pos), tmin);
    t1 = _mm_mul_ps(_mm_sub_ps(max, *ray->pos), tmin);

    tmin = _mm_min_ps(t0, t1);
    tmax = _mm_max_ps(t0, t1);

    tmin = _mm_blendv_ps(tmin, _mm_set1_ps(-INFINITY), is_zero);
    tmax = _mm_blendv_ps(tmax, _mm_set1_ps(INFINITY), is_zero);

    tmin = get_min(tmin);
    tmax = get_max(tmax);

    __m128 cond = _mm_and_ps(_mm_cmpge_ps(tmax, tmin), _mm_cmpge_ps(tmax, _mm_setzero_ps()));

    return (_mm_movemask_ps(cond) & 0x7) == 0x7;
}

// int intersect_aabb(t_ray *ray, __m128 max, __m128 min) {
// 	float tmin_max[2];
//     __m128 t0;
// 	__m128 t1;
// 	float tmin[4];
// 	float tmax[4];
// 	__m128 is_zero;
    
//     is_zero = _mm_cmpeq_ps(ray->direction, _mm_setzero_ps());
//     is_zero = _mm_div_ps(_mm_set1_ps(1.0f), _mm_blendv_ps(ray->direction, _mm_set1_ps(1.0f), is_zero));

//     t0 = _mm_mul_ps(_mm_sub_ps(min, *ray->pos), is_zero);
//     t1 = _mm_mul_ps(_mm_sub_ps(max, *ray->pos), is_zero);

// 	_mm_store_ps(tmin, _mm_min_ps(t0, t1));
// 	_mm_store_ps(tmax, _mm_max_ps(t0, t1));

// 	tmin_max[0] = FLT_MAX;
// 	tmin_max[1] = -FLT_MAX;
// 	int n = -1;
// 	while (++n < 4)
// 	{
// 		if (tmin_max[0] > tmin[n])
// 			tmin_max[0] = tmin[n];
// 		if (tmin_max[1] < tmax[n])
// 			tmin_max[1] = tmax[n];
// 	}
//     return (tmin_max[1] >= tmin_max[0] && tmin_max[1] > 0);
// }

int	intersect_obb(t_ray *ray, t_OBB obb)
{
	__m128 max;
	__m128 min;

	max = quat_rotate(obb.quat, _mm_add_ps(obb.pos, obb.half_len));
	min = quat_rotate(obb.quat, _mm_sub_ps(obb.pos, obb.half_len));
	// vector_normalize(&ray->direction);

	min = _mm_sub_ps(obb.pos, obb.half_len);
	max = _mm_add_ps(obb.pos, obb.half_len);

	// print_m128(max);
	// print_m128(min);

	return (intersect_aabb(ray, max, min));
}


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
