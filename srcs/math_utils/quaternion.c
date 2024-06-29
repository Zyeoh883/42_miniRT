/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:08:41 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/29 21:21:17 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

inline __m128	quat_rotate(t_quat q, const __m128 v) // rotate point
{
	t_quat rotated;

	// print_m128(v);
	quat_normalize(&q);
	rotated = quat_product(quat_product(q, v), quat_conjugate(q));
	// quat_normalize(&rotated); // ? Normialize or not?
	return (_mm_blend_ps(rotated, _mm_setzero_ps(), 1));
}

// (w0 * w1 - v0 . v1) + (w0 * v1 + w1 * v0 + v0 x v1)
inline __m128	quat_product( __m128 q1,  __m128 q2)
{
    __m128 real;
    __m128 w1;
    __m128 w2;
    __m128 wv;
    __m128 vv;

	w1 = _mm_shuffle_ps(q1, q1, _MM_SHUFFLE( 0, 0, 0, 0));
	w2 = _mm_shuffle_ps(q2, q2, _MM_SHUFFLE( 0, 0, 0, 0));
    q1 = _mm_blend_ps(q1, _mm_setzero_ps(), 1);
    q2 = _mm_blend_ps(q2, _mm_setzero_ps(), 1);

    real = _mm_sub_ps(_mm_mul_ps(w1, w2), _mm_dp_ps(q1, q2, 0xFF));
    real = _mm_blend_ps(real, _mm_setzero_ps(), 14);

    wv = _mm_add_ps(_mm_mul_ps(w1, q2), _mm_mul_ps(w2, q1));
    vv = vector_cross_product(q1, q2);

    return (_mm_add_ps(real, _mm_add_ps(wv, vv)));
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

inline void	quat_normalize(t_quat *q)
{
	*q = _mm_mul_ps(*q, _mm_rsqrt_ps(_mm_dp_ps(*q, *q, 0xFF)));
}

// inline t_quat	quat_slerp(const t_quat dest, const t_quat src, const float angle, const float t)
// {
// 	float	sine;
// 	t_quat	q1;
// 	t_quat	q2;

// 	sine = sin(angle);
// 	q1 = quat_scalar_product(dest, sin((1 - t) * angle) / sine);
// 	q2 = quat_scalar_product(src, sin(t * angle) / sine);
// 	return ((t_quat){q1.w + q2.w, q1.i + q2.i, q1.j + q2.j, q1.k + q2.k});
// }
