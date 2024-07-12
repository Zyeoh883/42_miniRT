/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_vector.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:31:47 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/12 12:01:42 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

inline float4	vector_cross_product(const float4 v1, const float4 v2)
{
	float4	a;
	float4	b;
	float4	c;
	float4	d;
	float4	result;

	a = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(1, 3, 2, 0));
	b = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(2, 1, 3, 0));
	c = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(2, 1, 3, 0));
	d = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(1, 3, 2, 0));

	result = _mm_sub_ps(_mm_mul_ps(a, b), _mm_mul_ps(c, d));
	
	return (_mm_blend_ps(result, _mm_setzero_ps(), 1));
}

inline float	vector_dot_product(const float4 v1, const float4 v2)
{
	return (_mm_cvtss_f32(_mm_dp_ps(v1, v2, 0XFF)));
}

inline float4	vector_scalar_product(const float4 v, const float scalar)
{
	return ((float4){v[0] * scalar, v[1] * scalar, v[2] * scalar, v[3] * scalar});
}

inline float4	vector_addition(const float4 v1, const float4 v2)
{
	return ((float4){v[0] + v[0], v[1] + v[1], v[2] + v[2], v[3] + v[3]});
}

inline float4	vector_subtraction(const float4 v1, const float4 v2)
{
	return ((float4){v[0] - v[0], v[1] - v[1], v[2] - v[2], v[3] - v[3]});
}

inline void	vector_normalize(float4 *v)
{
	*v = _mm_mul_ps(*v, _mm_rsqrt_ps(_mm_dp_ps(*v, *v, 0xFF)));
}
