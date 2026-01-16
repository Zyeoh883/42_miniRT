/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 18:10:04 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/16 18:11:06 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

cl_float4	quat_sum(const cl_float4 q1, const cl_float4 q2)
{
	return ((cl_float4){{q1.s[0] + q2.s[0],
			q1.s[1] + q2.s[1],
			q1.s[2] + q2.s[2],
			q1.s[3] + q2.s[3]}});
}

cl_float4	quat_scalar_product(const cl_float4 q, const float scale)
{
	return ((cl_float4){{q.s[0] * scale, q.s[1] * scale,
			q.s[2] * scale, q.s[3] * scale}});
}

cl_float4	quat_slerp(const cl_float4 dest, const cl_float4 src,
		const float angle, const float t)
{
	float		sine;
	cl_float4	q1;
	cl_float4	q2;

	sine = sin(angle);
	q1 = quat_scalar_product(dest, sin((1 - t) * angle) / sine);
	q2 = quat_scalar_product(src, sin(t * angle) / sine);
	return ((cl_float4){{q1.s[0] + q2.s[0], q1.s[1] + q2.s[1],
			q1.s[2] + q2.s[2], q1.s[3] + q2.s[3]}});
}

float	quat_abs(const cl_float4 q)
{
	return (sqrt(q.s[0] * q.s[0] + q.s[1] * q.s[1] + q.s[2] * q.s[2] + q.s[3]
			* q.s[3]));
}

cl_float4	quat_normalize(cl_float4 q)
{
	return (quat_scalar_product(q, 1 / (quat_abs(q))));
}
