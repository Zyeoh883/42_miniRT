/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quaternion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 12:08:41 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/18 19:47:24 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	quat_rotate(float p[4], float q[4], float result[4])
{
	float	q_conjugate[4];
	float	temp[4];

	quat_conjugate(q, q_conjugate);
	quat_product(q, p, temp);
	quat_product(temp, q_conjugate, result);
}

void	quat_product(float A[4], float B[4], float result[4])
{
	float	v_1[3];
	float	v_2[3];

	result[0] = (A[3] * B[3] - vector_dot_product(A, B));
	vector_cross_product(A, B, result);
	ft_memcpy(v_1, A, sizeof(float[3]));
	ft_memcpy(v_2, B, sizeof(float[3]));
	vector_scalar_product(v_1, B[3]);
	vector_addition(result, v_1);
	vector_scalar_product(v_2, A[3]);
	vector_addition(result, v_2);
}

void	quat_conjugate(float quat[4], float conjugate[4])
{
	conjugate[0] = -quat[0];
	conjugate[1] = -quat[1];
	conjugate[2] = -quat[2];
	conjugate[3] = quat[3];
}

void	angle_to_quat(float radian, float rot_vector[3], float result[4])
{
	float	sine;

	radian /= 2;
	sine = sin(radian);
	result[3] = cos(radian);
	result[0] = sine * rot_vector[0];
	result[1] = sine * rot_vector[1];
	result[2] = sine * rot_vector[2];
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
	float	sine;
	float	quat_0[4];
	float	quat_1[4];

	ft_memcpy(quat_0, dest, sizeof(float[4]));
	ft_memcpy(quat_1, src, sizeof(float[4]));
	sine = sin(steps[1]);
	quat_scalar_product(quat_0, sin((1 - steps[0]) * steps[1]) / sine);
	quat_scalar_product(quat_1, sin(steps[0] * steps[1]) / sine);
	quat_sum(quat_0, quat_1, result);
}

float	quat_abs(float quat[4])
{
	return (sqrt(quat[0] * quat[0] + quat[1] * quat[1] + quat[2] * quat[2]
			+ quat[3] * quat[3]));
}

// void quat_normalize(float quat[4])
// {
// 	float abs;

// 	abs =
// }