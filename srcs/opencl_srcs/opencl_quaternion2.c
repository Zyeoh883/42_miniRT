/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_quaternion2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 15:46:31 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/17 15:46:35 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
