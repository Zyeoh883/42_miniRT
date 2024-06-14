/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:30:35 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/14 16:17:08 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_UTILS_H
# define MATH_UTILS_H

void	vector_cross_product(float A[3], float B[3], float result[3]);
float	vector_dot_product(float A[3], float B[3]);
void	vector_scalar_product(float vector[3], float scale);
void	vector_sum(float A[3], float B[3], float result[3]);
void	vector_addition(float dest[3], float src[3]);

void	quat_product(float A[4], float B[4], float result[4]);
void	quat_conjugate(float quat[4], float conjugate[4]);
void	angle_to_quat(float radian, float rot_vector[3], float result[4]);
void	quat_sum(float A[4], float B[4], float result[4]);
void	quat_scalar_product(float quat[4], float scale);
void	quat_slerp(float dest[4], float src[4], float steps[2],
			float result[4]);
#endif