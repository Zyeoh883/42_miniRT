/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:08:41 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/12 16:22:58 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

inline cl_float4	quat_rotate(cl_float4 q, const cl_float4 v) // rotate point
{
	cl_float4 q_v;
	cl_float4 rotated;

	q_v = (cl_float4){0, v.s[0], v.s[1], v.s[2]};
	quat_normalize(&q);
	rotated = quat_product(quat_product(q, q_v), quat_conjugate(q));
	return ((cl_float4){rotated.s[0], rotated.s[1], rotated.s[2]});
}

inline cl_float4	quat_product(const cl_float4 q1, const cl_float4 q2)
// get result of quaternion after rotating
{
	return ((cl_float4){
		q1.s[3] * q2.s[3] - q1.s[0] * q2.s[0] - q1.s[1] * q2.s[1] - q1.s[2] * q2.s[2],
		q1.s[3] * q2.s[0] + q1.s[0] * q2.s[3] + q1.s[1] * q2.s[2] - q1.s[2] * q2.s[1],
		q1.s[3] * q2.s[1] - q1.s[0] * q2.s[2] + q1.s[1] * q2.s[3] + q1.s[2] * q2.s[0],
		q1.s[3] * q2.s[2] + q1.s[0] * q2.s[1] - q1.s[1] * q2.s[0] + q1.s[2] * q2.s[3],
	});
}

inline cl_float4	quat_conjugate(const cl_float4 q)
{
	return ((cl_float4){q.s[3], -q.s[0], -q.s[1], -q.s[2]});
}

inline cl_float4	angle_to_quat(const cl_float4 rot, const float radian)
{
	float	sine;

	sine = sin(radian / 2);
	return ((cl_float4){cos(radian / 2), rot.s[0] * sine, rot.s[1] * sine, rot.s[2] * sine});
}

inline cl_float4	quat_angle(const cl_float4 rot, const float radian)
{
	float	sine;

	sine = sin(radian / 2);
	return ((cl_float4){cos(radian), rot.s[0] * sine, rot.s[1] * sine, rot.s[2] * sine});
}

inline cl_float4	quat_sum(const cl_float4 q1, const cl_float4 q2)
{
	return ((cl_float4){q1.s[3] + q2.s[3], q1.s[0] + q2.s[0], q1.s[1] + q2.s[1], q1.s[2] + q2.s[2]});
}

inline cl_float4	quat_scalar_product(const cl_float4 q, const float scale)
{
	return ((cl_float4){q.s[3] * scale, q.s[0] * scale, q.s[1] * scale, q.s[2] * scale});
}

inline cl_float4	quat_slerp(const cl_float4 dest, const cl_float4 src, const float angle, const float t)
{
	float	sine;
	cl_float4	q1;
	cl_float4	q2;

	sine = sin(angle);
	q1 = quat_scalar_product(dest, sin((1 - t) * angle) / sine);
	q2 = quat_scalar_product(src, sin(t * angle) / sine);
	return ((cl_float4){q1.s[3] + q2.s[3], q1.s[0] + q2.s[0], q1.s[1] + q2.s[1], q1.s[2] + q2.s[2]});
}

inline float	quat_abs(const cl_float4 q)
{
	return (sqrt(q.s[3] * q.s[3] + q.s[0] * q.s[0] + q.s[1] * q.s[1] + q.s[2] * q.s[2]));
}

inline void	quat_normalize(cl_float4 *q)
{
	*q = quat_scalar_product(*q, 1 / (quat_abs(*q)));
}