/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:31:47 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/14 16:06:40 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	vector_cross_product(float A[3], float B[3], float result[3])
{
	result[0] = A[1] * B[2] - A[2] * B[1];
	result[1] = A[2] * B[0] - A[0] * B[2];
	result[2] = A[0] * B[1] - A[1] * B[0];
}

float	vector_dot_product(float A[3], float B[3])
{
	return (A[0] * B[0] + A[1] * B[1] + A[2] * B[2]);
}

void	vector_scalar_product(float vector[3], float scale)
{
	vector[0] *= scale;
	vector[1] *= scale;
	vector[2] *= scale;
}

void	vector_sum(float A[3], float B[3], float result[3])
{
	result[0] = A[0] + B[0];
	result[1] = A[1] + B[1];
	result[2] = A[2] + B[2];
}

void	vector_addition(float dest[3], float src[3])
{
	dest[0] += src[0];
	dest[1] += src[1];
	dest[2] += src[2];
}
