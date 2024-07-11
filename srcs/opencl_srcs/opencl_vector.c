/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:31:47 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/11 09:12:11 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

inline cl_float4	vector_cross_product(const cl_float4 v1, const cl_float4 v2)
{
	cl_float4	a;
	cl_float4	b;
	cl_float4	c;
	cl_float4	d;
	cl_float4	result;

	a = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(1, 3, 2, 0));
	b = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(2, 1, 3, 0));
	c = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(2, 1, 3, 0));
	d = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(1, 3, 2, 0));

	result = _mm_sub_ps(_mm_mul_ps(a, b), _mm_mul_ps(c, d));
	
	return (_mm_blend_ps(result, _mm_setzero_ps(), 1));
}

inline float	vector_dot_product(const cl_float4 v1, const cl_float4 v2)
{
	return (_mm_cvtss_f32(_mm_dp_ps(v1, v2, 0XFF)));
}

inline cl_float4	vector_scalar_product(const cl_float4 v, const float scale)
{
	return (_mm_mul_ps(v, _mm_set1_ps(scale)));
}

inline cl_float4	vector_addition(const cl_float4 v1, const cl_float4 v2)
{
	return (_mm_add_ps(v1, v2));
}

inline cl_float4	vector_subtraction(const cl_float4 v1, const cl_float4 v2)
{
	return (_mm_sub_ps(v1, v2));
}

inline void	vector_normalize(cl_float4 *v)
{
	*v = _mm_mul_ps(*v, _mm_rsqrt_ps(_mm_dp_ps(*v, *v, 0xFF)));
}
