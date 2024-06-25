/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:30:35 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/25 09:17:24 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_UTILS_H
# define MATH_UTILS_H

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
t_vector	vector_cross_product(const t_vector v1, const t_vector v2);
float		vector_dot_product(const t_vector v1, t_vector v2);
t_vector	vector_scalar_product(const t_vector v, float scale);
t_vector	vector_sum(const t_vector v1, const t_vector v2);
t_vector	vector_addition(const t_vector v1, const t_vector v2);
t_vector	vector_subtraction(const t_vector v1, const t_vector v2 );
void		vector_normalize(t_vector *v);
// t_vector	vector_normalize(t_vector v);

t_vector	quat_rotate(const t_quat q, const t_vector v);
t_quat		quat_product(const t_quat q1, const t_quat q2);
t_quat		quat_conjugate(const t_quat q);
t_quat		angle_to_quat(const t_vector rot, const float radian);
t_quat		quat_sum(const t_quat q1, const t_quat q2);
t_quat		quat_scalar_product(const t_quat q, const float scale);
t_quat		quat_slerp(const t_quat dest, const t_quat src, const float angle, const float t);
float		quat_abs(const t_quat q);
void		quat_normalize(t_quat *q);

void		print_vector(t_vector v);
void		print_quat(t_quat q);

#endif