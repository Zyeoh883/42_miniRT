/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:31:47 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/19 19:43:53 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vector	vector_cross_product(t_vector v1, t_vector v2)
{
	return ((t_vector){v1.j * v2.k - v1.k * v2.j, v1.k * v2.i - v1.i * v2.k,
		v1.i * v2.j - v1.j * v2.i});
}

float	vector_dot_product(t_vector v1, t_vector v2)
{
	return (v1.i * v2.i + v1.j * v2.j + v1.k * v2.k);
}

t_vector	vector_scalar_product(t_vector v, float scale)
{
	return ((t_vector){v.i * scale, v.j * scale, v.k * scale});
}

t_vector	vector_sum(t_vector v1, t_vector v2)
{
	return ((t_vector){v1.i * v2.i, v1.j * v2.j, v1.k * v2.k});
}

t_vector	vector_addition(t_vector v1, t_vector v2)
{
	return ((t_vector){v1.i + v2.i, v1.j + v2.j, v1.k + v2.k});
}
