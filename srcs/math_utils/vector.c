/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:31:47 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/29 20:08:24 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

inline __m128	vector_cross_product(const __m128 v1, const __m128 v2)
{
	__m128	a;
	__m128	b;
	__m128	c;
	__m128	d;
	__m128	result;

	a = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(1, 3, 2, 0));
	b = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(2, 1, 3, 0));
	c = _mm_shuffle_ps(v1, v1, _MM_SHUFFLE(2, 1, 3, 0));
	d = _mm_shuffle_ps(v2, v2, _MM_SHUFFLE(1, 3, 2, 0));

	result = _mm_sub_ps(_mm_mul_ps(a, b), _mm_mul_ps(c, d));
	
	return (_mm_blend_ps(result, _mm_setzero_ps(), 1));
}

inline float	vector_dot_product(const t_vector v1, const t_vector v2)
{
	return (_mm_cvtss_f32(_mm_dp_ps(v1, v2, 0XFF)));
}

inline t_vector	vector_scalar_product(const t_vector v, const float scale)
{
	return (_mm_mul_ps(v, _mm_set1_ps(scale)));
}

inline t_vector	vector_addition(const t_vector v1, const t_vector v2)
{
	return (_mm_add_ps(v1, v2));
}

inline t_vector	vector_subtraction(const t_vector v1, const t_vector v2)
{
	return (_mm_sub_ps(v1, v2));
}

inline void	vector_normalize(t_vector *v)
{
	*v = _mm_mul_ps(*v, _mm_rsqrt_ps(_mm_dp_ps(*v, *v, 0xFF)));
}
