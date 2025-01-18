/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_vector.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:31:47 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/14 15:51:14 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

float3	vector_cross_product(const float3 v1, const float3 v2)
{
	return (cross(v1, v2));
}

float	vector_dot_product(const float3 v1, const float3 v2)
{
	return (dot(v1, v2));
}

float3	vector_scalar_product(const float3 v, const float scalar)
{
	return (v * scalar);
}

float3	vector_addition(const float3 v1, const float3 v2)
{
	return (v1 + v2);
}

float3	vector_subtraction(const float3 v1, const float3 v2)
{
	return (v1 - v2);
}

float3	vector_normalize(float3 v)
{
	return (fast_normalize(v));
}
