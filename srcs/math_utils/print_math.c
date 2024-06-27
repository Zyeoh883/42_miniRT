/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_math.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 16:01:01 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/27 22:58:59 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void print_m128(__m128 m)
{
    float arr[4];

    _mm_store_ps(arr, m);
    printf("\n%f %f %f %f\n", arr[0], arr[1], arr[2], arr[3]);
}

void	print_vector(t_vector v)
{
	print_m128(v);
}

// TODO fix this print
// void	print_quat(t_quat q)
// {
// 	float	angle;
	
// 	print_m128(q);
// 	angle = acos(_mm_blend_ps(q, _mm_setzero_ps(), 14));
//     printf("angle : %f\n", angle * 180 / M_PI);
// 	printf("q_vector:", q.i / sin(angle / 2), q.j / sin(angle / 2),
// 		q.k / sin(angle / 2));
// 	print_m128(_mm_blend_ps(_mm_mul_ps(q1), _mm_setzero_ps(), 1))
// }
