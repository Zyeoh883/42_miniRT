/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:08:41 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/27 16:13:33 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

inline t_vector	quat_rotate(t_quat q, const t_vector v) // rotate point
{
	t_quat q_v;
	t_quat rotated;

	q_v = (t_quat){0, v.i, v.j, v.k};
	quat_normalize(&q);
	rotated = quat_product(quat_product(q, q_v), quat_conjugate(q));
	return ((t_vector){rotated.i, rotated.j, rotated.k});
}

inline t_quat	quat_product(const t_quat q1, const t_quat q2)
{

	__m128 a;
    __m128 b;
    __m128 c;
    __m128 d;

	a = _mm_shuffle_ps(q1, q1, _MM_SHUFFLE( 1, 1, 1, 1));
	b = _mm_shuffle_ps(q2, q2, _MM_SHUFFLE( 1, 1, 1, 1));
	c = _mm_shuffle_ps(q1, q1, _MM_SHUFFLE( 0, 3, 2, 1));
	d = _mm_shuffle_ps(q2, q2, _MM_SHUFFLE( 0, 3, 2, 1));
	
	return(_mm_sub_ps(_mm_mul_ps(a, b), _mm_mul_ps(c,d)));
}

inline t_quat	quat_conjugate(const t_quat q)
{
	return (_mm_xor_ps(q, _mm_set_ps(-0.0f, -0.0f, -0.0f, 0.0f)));
}

inline t_quat	angle_to_quat(const t_vector rot, const float radian)
{
	__m128 sine;
	__m128 cos;

	sine = _mm_set1_ps(sinf(radian * 0.5f));
	cos = _mm_set1_ps(cosf(radian * 0.5f));
	return (_mm_blend_ps(_mm_mul_ps(rot, sine), cos, 0x1));
}

inline t_quat	quat_sum(const t_quat q1, const t_quat q2)
{
	return (_mm_add_ps(q1, q2));
}

inline t_quat	quat_scalar_product(const t_quat q, const float scale)
{
	return (_mm_mul_ps(q, _mm_set1_ps(scale)));
}

inline t_quat	quat_slerp(const t_quat dest, const t_quat src, const float angle, const float t)
{
	float	sine;
	t_quat	q1;
	t_quat	q2;

	sine = sin(angle);
	q1 = quat_scalar_product(dest, sin((1 - t) * angle) / sine);
	q2 = quat_scalar_product(src, sin(t * angle) / sine);
	return ((t_quat){q1.w + q2.w, q1.i + q2.i, q1.j + q2.j, q1.k + q2.k});
}


inline void	quat_normalize(t_quat *q)
{
	*q = _mm_mul_ps(*q, _mm_rsqrt_ps(_mm_dp_ps(*q, *q, 0xFF)));
}
