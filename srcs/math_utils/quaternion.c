/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:08:41 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/14 17:03:43 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	quat_product(float A[4], float B[4], float result[4])
{
	float	v_1[3];
	float	v_2[3];

	result[0] = (A[0] * B[0] - vector_dot_product(A + 1, B + 1));
	vector_cross_product(A + 1, B + 1, result + 1);
	ft_memcpy(v_1, A + 1, sizeof(float [3]));
	ft_memcpy(v_2, B + 1, sizeof(float [3]));
	vector_scalar_product(v_1, B[0]);
	vector_addition(result + 1, v_1);
	vector_scalar_product(v_2, A[0]);
	vector_addition(result + 1, v_2);
}

void	quat_conjugate(float quat[4], float conjugate[4])
{
	conjugate[0] = quat[0];
	conjugate[1] = -quat[1];
	conjugate[2] = -quat[2];
	conjugate[3] = -quat[3];
}

void	angle_to_quat(float radian, float rot_vector[3], float result[4])
{
	float	sine;

	radian /= 2;
	sine = sin(radian);
	result[0] = cos(radian);
	result[1] = sine * rot_vector[0];
	result[2] = sine * rot_vector[1];
	result[3] = sine * rot_vector[2];
}

void	quat_sum(float A[4], float B[4], float result[4])
{
	result[0] = A[0] + B[0];
	result[1] = A[1] + B[1];
	result[2] = A[2] + B[2];
	result[3] = A[3] + B[3];
}

void	quat_scalar_product(float quat[4], float scale)
{
	quat[0] *= scale;
	quat[1] *= scale;
	quat[2] *= scale;
	quat[3] *= scale;
}

void	quat_slerp(float dest[4], float src[4], float steps[2], float result[4])
{
	float sine;
	float quat_0[4];
	float quat_1[4];

	ft_memcpy(quat_0, dest, sizeof(float [4]));
	ft_memcpy(quat_1, src, sizeof(float [4]));
	sine = sin(steps[1]);
	quat_scalar_product(quat_0, sin((1 - steps[0]) * steps[1]) / sine);
	quat_scalar_product(quat_1, sin(steps[0] * steps[1]) / sine);
	quat_sum(quat_0, quat_1, result);
}
