/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:31:47 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/27 11:28:09 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

inline t_vector	vector_cross_product(const t_vector v1, const t_vector v2)
{
	return ((t_vector){v1.j * v2.k - v1.k * v2.j, v1.k * v2.i - v1.i * v2.k,
		v1.i * v2.j - v1.j * v2.i});
}

inline float	vector_dot_product(const t_vector v1, const t_vector v2)
{
	__m128 mul;
    __m128 sum;
    mul = _mm_mul_ps(v1, v2);
    sum = _mm_hadd_ps(mul, mul);
    sum = _mm_hadd_ps(sum, sum);
    return _mm_cvtss_f32(mul);
}

inline t_vector	vector_scalar_product(const t_vector v, const float scale)
{
	return ((t_vector){v.i * scale, v.j * scale, v.k * scale});
}

inline t_vector	vector_sum(const t_vector v1, const t_vector v2)
{
	return ((t_vector){v1.i * v2.i, v1.j * v2.j, v1.k * v2.k});
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
	*v = vector_scalar_product(*v, 1 / sqrt(v->i * v->i + v->j * v->j + v->k
				* v->k));
}
