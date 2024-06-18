/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:54:40 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/18 19:48:42 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	vector_test(void)
{
	float	arr[3];
	float	brr[3];
	float	result[3];

	arr[0] = 0;
	arr[1] = 0;
	arr[2] = 1;
	brr[0] = 1 / sqrt(3);
	brr[1] = 1 / sqrt(3);
	brr[2] = 1 / sqrt(3);
	vector_cross_product(arr, brr, result);
	printf("Cross-Product: %f %f %f\n", result[0], result[1], result[2]);
	// printf("Dot-Product: %f\n", vector_dot_product(arr, brr));
}

void	qua_test(void)
{
	float	arr[4];
	float	brr[4];
	float	result[4];
	float	rot_vector[4];

	arr[0] = 4;
	arr[1] = 1;
	arr[2] = 3;
	arr[3] = 2;
	
	brr[0] = 5;
	brr[1] = 2;
	brr[2] = 1;
	brr[3] = 3;
	
	quat_product(brr, arr, result);
	printf("quat Dot-Product: %f %f %f %f\n", result[0], result[1], result[2],
		result[3]);
	quat_conjugate(result, result);
	printf("Conjugate: %f %f %f %f\n\n", result[0], result[1], result[2],
		result[3]);
		
	rot_vector[0] = 0;
	rot_vector[1] = 1;
	rot_vector[2] = 0;
	
	angle_to_quat(M_PI / 2, rot_vector, result);
	printf("Quaternion angle: %f %f %f %f\n\n\n", result[0], result[1], result[2],
		result[3]);
	

	arr[0] = 0;
	arr[1] = 1;
	arr[2] = 0;
	arr[3] = 0;
	
	brr[0] = 1;
	brr[1] = 0;
	brr[2] = 0;
	brr[3] = 0;


	float steps[2];
	steps[0] = 0;
	steps[1] = M_PI / 2;
	int n = -1;
	while (++n < 3)
	{
		quat_slerp(arr, brr, steps, result);
		printf("%d. Quaternion result: %f %f %f %f\n\n", n, result[0], result[1], result[2],
		result[3]);
		steps[0] += 0.5;
	}	
}
