/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_translate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 10:33:06 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/29 21:31:46 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	input_translate(t_camera *camera, int key)
{
	t_vector	v;

	if (key == SPACE_KEY || key == CTRL_KEY)
	{
		v = _mm_set_ps(0, (key == SPACE_KEY ? 2 : -2), 0, 0);
		camera->pos = vector_addition(camera->pos, v);
		return (1);
	}
	if (key == W_KEY)
		v = _mm_set_ps(1, 0, 0, 0);
	else if (key == A_KEY)
		v = _mm_set_ps(0, 0, -1, 0);
	else if (key == D_KEY)
		v = _mm_set_ps(0, 0, 1, 0);
	else if (key == S_KEY)
		v = _mm_set_ps(-1, 0, 0, 0);
	else
		return (0);
	camera->pos = vector_addition(camera->pos, quat_rotate(camera->quat, v));
	// print_m128(camera->pos);
	// printf("%d\n", key);
	return (1);
}

// int	input_rotation(t_data *data) // back up rotation
// {
// 	t_vector v;

// 	if (data->inputs.key == I_KEY)
// 		v = (t_vector){1, 0, 0};
// 	else if (data->inputs.key == J_KEY)
// 		v = (t_vector){0, -1, 0};
// 	else if (data->inputs.key == L_KEY)
// 		v = (t_vector){0, 1, 0};
// 	else if (data->inputs.key == K_KEY)
// 		v = (t_vector){-1, 0, 0};
// 	else
// 		return (0);
// 	if (data->inputs.key == I_KEY || data->inputs.key == K_KEY)
// 		data->camera.quat = quat_product(data->camera.quat, angle_to_quat(v,
// 					M_PI / 16));
// 	else
// 		data->camera.quat = quat_product(angle_to_quat(v, M_PI / 16),
// 				data->camera.quat);
// 	return (1);
// }
