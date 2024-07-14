/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_quaternion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:08:41 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/14 15:52:12 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

float4	quat_normalize(float4 q);
float4	quat_product(const float4 q1, const float4 q2);
float4	quat_conjugate(const float4 q);

float4	quat_rotate(float4 q, const float4 v) // rotate point
{
	float4	q_v;
	float4	rotated;

	q_v = v;
	q_v[3] = 0;
	q = quat_normalize(q);
	rotated = quat_product(quat_product(q, q_v), quat_conjugate(q));
	rotated[3] = 0;
	return (rotated);
}

float4	quat_product(const float4 q1, const float4 q2)
{
	return ((float4)(q1[3] * q2[0] + q1[0] * q2[3] + q1[1] * q2[2] - q1[2]
		* q2[1], q1[3] * q2[1] - q1[0] * q2[2] + q1[1] * q2[3] + q1[2]
		* q2[0], q1[3] * q2[2] + q1[0] * q2[1] - q1[1] * q2[0] + q1[2]
		* q2[3], q1[3] * q2[3] - q1[0] * q2[0] - q1[1] * q2[1] - q1[2]
		* q2[2]));
}

float4	quat_conjugate(const float4 q)
{
	float4	q_result;

	q_result = -q;
	q_result[3] = q[3];
	return (q_result);
}

float4	angle_to_quat(const float4 rot, const float radian)
{
	float4	q_result;

	q_result = rot * sin(radian * 0.5f);
	q_result[3] = cos(radian * 0.5f);
	return (q_result);
}

float4	quat_sum(const float4 q1, const float4 q2)
{
	return (q1 + q2);
}

float4	quat_scalar_product(const float4 q, const float scalar)
{
	return (q * scalar);
}

float4	quat_slerp(const float4 dest, const float4 src, const float angle,
		const float t)
{
	float	sine;
	float4	q1;
	float4	q2;

	sine = sin(angle);
	q1 = quat_scalar_product(dest, sin((1 - t) * angle) / sine);
	q2 = quat_scalar_product(src, sin(t * angle) / sine);
	return (q1 + q2);
}

float4	quat_normalize(float4 q)
{
	return (fast_normalize(q));
}
