/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_translate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 10:33:06 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/25 08:39:15 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	input_translate(t_data *data)
{
	t_vector	v;

	if (data->inputs.key == W_KEY)
		v = (t_vector){0, 0, 5};
	else if (data->inputs.key == A_KEY)
		v = (t_vector){-5, 0, 0};
	else if (data->inputs.key == D_KEY)
		v = (t_vector){5, 0, 0};
	else if (data->inputs.key == S_KEY)
		v = (t_vector){0, 0, -5};
	else if (data->inputs.key == SPACE_KEY)
		v = (t_vector){0, 2, 0};
	else if (data->inputs.key == SHIFT_KEY)
		v = (t_vector){0, -2, 0};
	else
		return (0);
	if (data->inputs.key != SPACE_KEY && data->inputs.key != SHIFT_KEY)
		data->camera.position = vector_addition(data->camera.position, quat_rotate(data->camera.quat, v));
	else
		data->camera.position = vector_addition(data->camera.position, v);
	return (1);
}

int	input_rotation(t_data *data)
{
	t_vector v;

	if (data->inputs.key == I_KEY)
		v = (t_vector){1, 0, 0};
	else if (data->inputs.key == J_KEY)
		v = (t_vector){0, -1, 0};
	else if (data->inputs.key == L_KEY)
		v = (t_vector){0, 1, 0};
	else if (data->inputs.key == K_KEY)
		v = (t_vector){-1, 0, 0};
	else
		return (0);
	if (data->inputs.key == I_KEY || data->inputs.key == K_KEY)
		data->camera.quat = quat_product(data->camera.quat, angle_to_quat(v,
					M_PI / 16));
	else
		data->camera.quat = quat_product(angle_to_quat(v, M_PI / 16),
				data->camera.quat);
	return (1);
}