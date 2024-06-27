/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_math.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 16:01:01 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/27 13:57:13 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	print_vector(t_vector v)
{
	printf("%f %f %f\n\n", v.i, v.j, v.k);
}

void	print_quat(t_quat q)
{
	float	angle;
	
	printf("%f %f %f %f\n", q.w, q.i, q.j, q.k);
	angle = acos(q.w);
    printf("angle : %f\n", angle * 180 / M_PI);
	printf("q_vector: %f %f %f\n\n", q.i / sin(angle / 2), q.j / sin(angle / 2),
		q.k / sin(angle / 2));
}
