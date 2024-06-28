/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:30:35 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/28 17:29:24 by zyeoh            ###   ########.fr       */
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
__m128	vector_cross_product(const __m128 v1, const __m128 v2);
float		vector_dot_product(const __m128 v1, __m128 v2);
__m128	vector_scalar_product(const __m128 v, float scale);
__m128	vector_sum(const __m128 v1, const __m128 v2);
__m128	vector_addition(const __m128 v1, const __m128 v2);
__m128	vector_subtraction(const __m128 v1, const __m128 v2 );
void		vector_normalize(__m128 *v);
// __m128	vector_normalize(__m128 v);

__m128	quat_rotate(t_quat q, const __m128 v);
t_quat		quat_product(const t_quat q1, const t_quat q2);
t_quat		quat_conjugate(const t_quat q);
t_quat		angle_to_quat(const __m128 rot, const float radian);
t_quat		quat_sum(const t_quat q1, const t_quat q2);
t_quat		quat_scalar_product(const t_quat q, const float scale);
t_quat		quat_slerp(const t_quat dest, const t_quat src, const float angle, const float t);
float		quat_abs(const t_quat q);
void		quat_normalize(t_quat *q);

void		print_vector(__m128 v);
void		print_quat(t_quat q);

#endif