/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:08:41 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/20 20:45:55 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

 void	tester(void)
{
	return ;
}

t_vector	quat_rotate(t_quat q, t_vector v) // rotate point
{
	t_quat	q_v;
	t_quat	rotated;

	q_v.w = 0;
	q_v.i = v.i;
	q_v.j = v.j;
	q_v.k = v.k;
	rotated = quat_product(quat_product(q, q_v), quat_conjugate(q));
	return ((t_vector){rotated.i, rotated.j, rotated.k});
}

t_quat	quat_product(t_quat q1, t_quat q2)
// get result of quaternion after rotating
{
	return ((t_quat){
		q1.w * q2.w - q1.i * q2.i - q1.j * q2.j - q1.k * q2.k,
		q1.w * q2.i + q1.i * q2.w + q1.j * q2.k - q1.k * q2.j,
		q1.w * q2.j - q1.i * q2.k + q1.j * q2.w + q1.k * q2.i,
		q1.w * q2.k + q1.i * q2.j - q1.j * q2.i + q1.k * q2.w,
	});
}

t_quat	quat_conjugate(t_quat q)
{
	return ((t_quat){q.w, -q.i, -q.j, -q.k});
}

t_quat	angle_to_quat(t_vector rot, float radian)
{
	float	sine;

	radian /= 2;
	sine = sin(radian);
	return ((t_quat){cos(radian), rot.i * sine, rot.j * sine, rot.k * sine});
}

t_quat	quat_sum(t_quat q1, t_quat q2)
{
	return ((t_quat){q1.w + q2.w, q1.i + q2.i, q1.j + q2.j, q1.k + q2.k});
}

t_quat	quat_scalar_product(t_quat q, float scale)
{
	return ((t_quat){q.w * scale, q.i * scale, q.j * scale, q.k * scale});
}

t_quat	quat_slerp(t_quat dest, t_quat src, float angle, float t)
{
	float	sine;
	t_quat	q1;
	t_quat	q2;

	sine = sin(angle);
	q1 = quat_scalar_product(dest, sin((1 - t) * angle) / sine);
	q2 = quat_scalar_product(src, sin(t * angle) / sine);
	return ((t_quat){q1.w + q2.w, q1.i + q2.i, q1.j + q2.j, q1.k + q2.k});
}

float	quat_abs(t_quat q)
{
	return (sqrt(q.w * q.w + q.i * q.i + q.j * q.j + q.k * q.k));
}

t_quat	quat_normalize(t_quat q)
{
	return (quat_scalar_product(q, sqrt(quat_abs(q))));
}

// void	quat_product(float A[4], float B[4], float q_result[4])
// // get result of quaternion after rotating
// {
// 	float v_1[3];
// 	float v_2[3];

// 	q_result[3] = (A[3] * B[3] - vector_dot_product(A, B));
// 	vector_cross_product(A, B, q_result);
// 	ft_memcpy(v_1, A, sizeof(float[3]));
// 	ft_memcpy(v_2, B, sizeof(float[3]));
// 	vector_scalar_product(v_1, B[3]);
// 	vector_addition(q_result, v_1);
// 	vector_scalar_product(v_2, A[3]);
// 	vector_addition(q_result, v_2);
// }