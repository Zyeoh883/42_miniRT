/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:08:41 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/16 18:11:12 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

cl_float3	quat_rotate(cl_float4 q, cl_float4 q_v) // rotate point
{
	cl_float4	rotated;

	q_v.s[3] = 0;
	q = quat_normalize(q);
	rotated = quat_product(quat_product(q, q_v), quat_conjugate(q));
	return ((cl_float3){{rotated.s[0], rotated.s[1], rotated.s[2]}});
}

// get result of quaternion after rotating
cl_float4	quat_product(const cl_float4 q1, const cl_float4 q2)
{
	return ((cl_float4){{q1.s[3] * q2.s[0] + q1.s[0] * q2.s[3] + q1.s[1]
			* q2.s[2] - q1.s[2] * q2.s[1], q1.s[3] * q2.s[1] - q1.s[0] * q2.s[2]
			+ q1.s[1] * q2.s[3] + q1.s[2] * q2.s[0], q1.s[3] * q2.s[2] + q1.s[0]
			* q2.s[1] - q1.s[1] * q2.s[0] + q1.s[2] * q2.s[3], q1.s[3] * q2.s[3]
			- q1.s[0] * q2.s[0] - q1.s[1] * q2.s[1] - q1.s[2] * q2.s[2]}});
}

cl_float4	quat_conjugate(const cl_float4 q)
{
	return ((cl_float4){{-q.s[0], -q.s[1], -q.s[2], q.s[3]}});
}

cl_float4	angle_to_quat(const cl_float4 rot, const float radian)
{
	float	sine;

	sine = sin(radian * 0.5f);
	return ((cl_float4){{rot.s[0] * sine,
			rot.s[1] * sine, rot.s[2] * sine,
			cos(radian * 0.5f)}});
}
