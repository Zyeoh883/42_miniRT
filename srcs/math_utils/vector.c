/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:31:47 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/16 18:04:35 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

cl_float4	vector_cross_product(const cl_float4 v1, const cl_float4 v2)
{
	return ((cl_float4){{v1.s[1] * v2.s[2] - v1.s[2] * v2.s[1],
			v1.s[2] * v2.s[0] - v1.s[0] * v2.s[2],
			v1.s[0] * v2.s[1] - v1.s[1] * v2.s[0],
			0}});
}

float	vector_dot_product(const cl_float4 v1, const cl_float4 v2)
{
	return (v1.s[0] * v2.s[0] + v1.s[1] * v2.s[1] + v1.s[2] * v2.s[2]);
}

cl_float4	vector_scalar_product(const cl_float4 v, const float scale)
{
	return ((cl_float4){{v.s[0] * scale, v.s[1] * scale, v.s[2] * scale, 0}});
}

cl_float4	vector_sum(const cl_float4 v1, const cl_float4 v2)
{
	return ((cl_float4){{v1.s[0] * v2.s[0],
			v1.s[1] * v2.s[1], v1.s[2] * v2.s[2], 0}});
}

cl_float4	vector_normalize(cl_float4 v)
{
	return (vector_scalar_product(v, 1 / sqrt(v.s[0] * v.s[0] + v.s[1] * v.s[1]
				+ v.s[2] * v.s[2])));
}
