/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 18:04:38 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/16 18:04:49 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

cl_float4	vector_addition(const cl_float4 v1, const cl_float4 v2)
{
	return ((cl_float4){{v1.s[0] + v2.s[0],
			v1.s[1] + v2.s[1], v1.s[2] + v2.s[2], 0}});
}

cl_float4	vector_subtraction(const cl_float4 v1, const cl_float4 v2)
{
	return ((cl_float4){{v1.s[0] - v2.s[0],
			v1.s[1] - v2.s[1], v1.s[2] - v2.s[2], 0}});
}
