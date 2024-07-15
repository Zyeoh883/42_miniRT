/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:30:35 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/15 09:47:42 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_UTILS_H
# define MATH_UTILS_H

# define TO_RADIAN M_PI / 180
# define TO_DEGREE M_PI / 180

// void		vector_cross_product(float A[3], float B[3], float result[3]);
// float		vector_dot_product(float A[3], float B[3]);
// void		vector_scalar_product(float vector[3], float scale);
// void		vector_sum(float A[3], float B[3], float result[3]);
// void		vector_addition(float dest[3], float src[3]);

// void	quat_product(float q1[4], float q2[4], float q_result[4]);
// void	quat_product(float A[4], float B[4], float q_result[4]);
// void	quat_conjugate(float quat[4], float conjugate[4]);
// void	angle_to_quat(float radian, float rot_vector[3], float q_result[4]);
// void	quat_sum(float A[4], float B[4], float q_result[4]);
// void	quat_scalar_product(float quat[4], float scale);
// void	quat_slerp(float dest[4], float src[4], float steps[2],
// 			float q_result[4]);
// float	quat_abs(float quat[4]);
cl_float4	vector_cross_product(const cl_float4 v1, const cl_float4 v2);
float		vector_dot_product(const cl_float4 v1, cl_float4 v2);
cl_float4	vector_scalar_product(const cl_float4 v, float scale);
cl_float4	vector_sum(const cl_float4 v1, const cl_float4 v2);
cl_float4	vector_addition(const cl_float4 v1, const cl_float4 v2);
cl_float4	vector_subtraction(const cl_float4 v1, const cl_float4 v2);
cl_float4	vector_normalize(cl_float4 v);
// cl_float4	vector_normalize(cl_float4 v);

cl_float4	quat_rotate(cl_float4 q, const cl_float4 v);
cl_float4	quat_product(const cl_float4 q1, const cl_float4 q2);
cl_float4	quat_conjugate(const cl_float4 q);
cl_float4	angle_to_quat(const cl_float4 rot, const float radian);
cl_float4	quat_sum(const cl_float4 q1, const cl_float4 q2);
cl_float4	quat_scalar_product(const cl_float4 q, const float scale);
cl_float4	quat_slerp(const cl_float4 dest, const cl_float4 src,
				const float angle, const float t);
float		quat_abs(const cl_float4 q);
cl_float4	quat_normalize(cl_float4 q);

void		print_vector(cl_float4 v);

#endif