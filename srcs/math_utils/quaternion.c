/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:08:41 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/14 15:22:34 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

cl_float4	quat_rotate(cl_float4 q, const cl_float4 v) // rotate point
{
	cl_float4 q_v;
	cl_float4 rotated;

	q_v = v;
	q_v.s[3] = 0;
	q = quat_normalize(q);
	rotated = quat_product(quat_product(q, q_v), quat_conjugate(q));
	rotated.s[3] = 0;
	return (rotated);
}

cl_float4	quat_product(const cl_float4 q1, const cl_float4 q2)
// get result of quaternion after rotating
{
	return ((cl_float4){{
		q1.s[3] * q2.s[0] + q1.s[0] * q2.s[3] + q1.s[1] * q2.s[2] - q1.s[2] * q2.s[1],
		q1.s[3] * q2.s[1] - q1.s[0] * q2.s[2] + q1.s[1] * q2.s[3] + q1.s[2] * q2.s[0],
		q1.s[3] * q2.s[2] + q1.s[0] * q2.s[1] - q1.s[1] * q2.s[0] + q1.s[2] * q2.s[3],
		q1.s[3] * q2.s[3] - q1.s[0] * q2.s[0] - q1.s[1] * q2.s[1] - q1.s[2] * q2.s[2]
	}});
}

cl_float4	quat_conjugate(const cl_float4 q)
{
	return ((cl_float4){{-q.s[0], -q.s[1], -q.s[2], q.s[3]}});
}

cl_float4	angle_to_quat(const cl_float4 rot, const float radian)
{
	float	sine;

	sine = sin(radian * 0.5f);
	return ((cl_float4){{rot.s[0] * sine, rot.s[1] * sine, rot.s[2] * sine, cos(radian * 0.5f)}});
}

cl_float4	quat_sum(const cl_float4 q1, const cl_float4 q2)
{
	return ((cl_float4){{q1.s[0] + q2.s[0], q1.s[1] + q2.s[1], q1.s[2] + q2.s[2], q1.s[3] + q2.s[3]}});
}

cl_float4	quat_scalar_product(const cl_float4 q, const float scale)
{
	return ((cl_float4){{q.s[0] * scale, q.s[1] * scale, q.s[2] * scale, q.s[3] * scale}});
}

cl_float4	quat_slerp(const cl_float4 dest, const cl_float4 src, const float angle, const float t)
{
	float	sine;
	cl_float4	q1;
	cl_float4	q2;

	sine = sin(angle);
	q1 = quat_scalar_product(dest, sin((1 - t) * angle) / sine);
	q2 = quat_scalar_product(src, sin(t * angle) / sine);
	return ((cl_float4){{q1.s[0] + q2.s[0], q1.s[1] + q2.s[1], q1.s[2] + q2.s[2], q1.s[3] + q2.s[3]}});
}

float	quat_abs(const cl_float4 q)
{
	return (sqrt(q.s[0] * q.s[0] + q.s[1] * q.s[1] + q.s[2] * q.s[2] + q.s[3] * q.s[3]));
}

cl_float4	quat_normalize(cl_float4 q)
{
	return (quat_scalar_product(q, 1 / (quat_abs(q))));
}
