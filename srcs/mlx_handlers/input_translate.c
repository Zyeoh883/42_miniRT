/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_translate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 10:33:06 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/24 15:00:12 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	input_translate(t_data *data)
{
	t_vector	v;

	if (data->key == W_KEY)
		v = (t_vector){0, 0, 1};
	else if (data->key == A_KEY)
		v = (t_vector){-1, 0, 0};
	else if (data->key == D_KEY)
		v = (t_vector){1, 0, 0};
	else if (data->key == S_KEY)
		v = (t_vector){0, 0, -1};
	else if (data->key == SPACE_KEY)
		v = (t_vector){0, 1, 0};
	else if (data->key == SHIFT_KEY)
		v = (t_vector){0, -1, 0};
	else
		return (0);
	if (data->key != SPACE_KEY && data->key != SPACE_KEY)
		data->camera.position = vector_addition(data->camera.position, quat_rotate(data->camera.quat, v));
	else
		data->camera.position = vector_addition(data->camera.position, v);
	return (1);
}

int	input_rotation(t_data *data)
{
	t_vector v;

	if (data->key == I_KEY)
		v = (t_vector){1, 0, 0};
	else if (data->key == J_KEY)
		v = (t_vector){0, -1, 0};
	else if (data->key == L_KEY)
		v = (t_vector){0, 1, 0};
	else if (data->key == K_KEY)
		v = (t_vector){-1, 0, 0};
	else
		return (0);
	if (data->key == I_KEY || data->key == K_KEY)
		data->camera.quat = quat_product(data->camera.quat, angle_to_quat(v,
					M_PI / 16));
	else
		data->camera.quat = quat_product(angle_to_quat(v, M_PI / 16),
				data->camera.quat);
	return (1);
}