/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/14 11:54:40 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/27 13:56:34 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	vector_test(void)
{
    float arr[4];
	t_vector	v1;
	t_vector	v2;
	t_vector	result;

    v1 = _mm_set_ps(3, 2, 1, 0);
	// v2.i = 1 / sqrt(3);
	// v2.j = 1 / sqrt(3);
	// v2.k = 1 / sqrt(3);
    v2 = _mm_set_ps(7, 5, 1, 0);
	result = vector_cross_product(v1, v2);
    _mm_store_ps(arr, result);
	printf("Cross-Product: %f %f %f\n", arr[1], arr[2], arr[3]);
	printf("Dot-Product: %f\n", vector_dot_product(v1, v2));
}

// void	qua_test(void)
// {
// 	t_quat		q1;
// 	t_quat		q2;
// 	t_quat		q_result;
// 	t_vector	rot_vector;

// 	q1.w = 9;
// 	q1.i = 2;
// 	q1.j = 4;
// 	q1.k = 9;
	
// 	q2.w = 3;
// 	q2.i = 2;
// 	q2.j = 16;
// 	q2.k = 6;
	
// 	q_result = quat_product(q1, q2);
// 	printf("quat Dot-Product: %f %f %f %f\n", q_result.w, q_result.i, q_result.j,
// 		q_result.k);
// 	q_result = quat_conjugate(q_result);
// 	printf("Conjugate: %f %f %f %f\n\n", q_result.w, q_result.i, q_result.j,
// 		q_result.k);
		
// 	rot_vector.i = 0;
// 	rot_vector.j = 1;
// 	rot_vector.k = 0;
	
// 	q_result = angle_to_quat(rot_vector, M_PI / 2);
// 	printf("Quaternion angle: %f %f %f %f\n\n\n", q_result.w, q_result.i, q_result.j,
// 		q_result.k);
	
// 	t_vector	result;

// 	q1.w = 0;
// 	q1.i = 1;
// 	q1.j = 0;
// 	q1.k = 0;
	
// 	q2.w = 1;
// 	q2.i = 0;
// 	q2.j = 0;
// 	q2.k = 0;

// 	printf("Rotated vector: %f %f %f\n", result.i, result.j, result.k);
// 	float t = 0;
// 	float angle = M_PI / 2;
// 	int n = -1;
// 	// while (++n < 3)
// 	// {
// 	q_result = quat_slerp(q1, q2, angle, t);
// 	printf("%d. Quaternion q_result: %f %f %f %f\n\n", n, q_result.w, q_result.i, q_result.j,
// 	q_result.k);
// 	t += 0.5;
// 	// }	
// }

// void	test(t_data data, t_camera camera)
// {
// 	t_vector	rot_vector;
// 	t_quat		rot_quat_y;
// 	t_quat		rot_quat_z;
// 	t_quat		rot_quat;
// 	t_ray		ray;

// 	// t_vector	res_vector;
// 	// orient camera quat
// 	rot_vector.i = 2;
// 	rot_vector.j = 2;
// 	rot_vector.k = 2;
// 	vector_normalize(&rot_vector);
// 	// print_vector(rot_vector);
// 	rot_vector.i = 0;
// 	rot_vector.j = 1;
// 	rot_vector.k = 0;
// 	rot_quat_y = angle_to_quat(rot_vector, atan(camera.pixel_width)
// 			* data.win_width / 2);
// 	rot_vector.i = 0;
// 	rot_vector.j = 0;
// 	rot_vector.k = 1;
// 	rot_quat_z = angle_to_quat(rot_vector, atan(camera.pixel_width)
// 			* data.win_width / 2);
// 	// print_quat(rot_quat_y);
// 	// print_quat(rot_quat_z);
// 	rot_quat = quat_product(rot_quat_y, rot_quat_z);
// 	// print_quat(rot_quat);
// 	// res_vector = quat_rotate(rot_quat, rot_vector);
// 	// print_vector(res_vector);
// 	// quat_rotate(camera.quat, rot_quat, camera.quat);
// 	// print_quat(camera.quat);
// 	ray = create_ray(camera, 0, 0, data.win_width, data.win_height);
// 	print_vector(ray.direction);
// }
