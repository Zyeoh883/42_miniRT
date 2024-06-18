/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_math.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 16:01:01 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/18 19:32:18 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	print_vector(float vector[3])
{
	printf("%f %f %f\n", vector[0], vector[1], vector[2]);
}

void	print_quat(float quat[4])
{
	float	angle;
    float temp[4];

    ft_memcpy(temp, quat, sizeof(float [4]));
    // temp 
	printf("%f %f %f %f\n", temp[0], temp[1], temp[2], temp[3]);
	angle = acos(temp[0]);
	printf("q_vector: %f %f %f\n", temp[1] / sin(angle / 2), temp[2] / sin(angle / 2),
		temp[3] / sin(angle / 2));
    printf("angle : %f\n\n", angle * 180 / M_PI);
}
