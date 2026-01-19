/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_translate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 10:33:06 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/16 18:14:21 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

cl_float4	handle_wasd(t_data *data, t_inputs inputs)
{
	cl_float4	v;

	v = (cl_float4){{0, 0, 0, 0}};
	if (inputs.key_wasd[0] || inputs.key_wasd[2])
	{
		v = (cl_float4){{0, 0, -1, 0}};
		if (inputs.key_wasd[0])
			v = (cl_float4){{0, 0, 1, 0}};
		v = quat_rotate(data->camera->quat, v);
		v.s[1] = 0;
		v = vector_normalize(v);
	}
	return (v);
}

void	input_translate(t_data *data, t_inputs inputs)
{
	cl_float4	v;

	v = handle_wasd(data, inputs);
	if (inputs.key == SPACE_KEY)
		v = vector_addition((cl_float4){{0, 1, 0, 0}}, v);
	else if (inputs.key == CTRL_KEY)
		v = vector_addition((cl_float4){{0, -1, 0, 0}}, v);
	if (inputs.key_wasd[3])
		v = vector_addition(quat_rotate(data->camera->quat, (cl_float4){{1, 0,
					0, 0}}), v);
	if (inputs.key_wasd[1])
		v = vector_addition(quat_rotate(data->camera->quat, (cl_float4){{-1, 0,
					0, 0}}), v);
	if (v.s[0] + v.s[1] + v.s[2])
		v = vector_normalize(v);
	data->camera->pos = vector_addition(data->camera->pos,
			vector_scalar_product(v, 1 + data->inputs.shift));
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
// M_PI / 16));
// 	else
// 		data->camera.quat = quat_product(angle_to_quat(v, M_PI / 16),
// 				data->camera.quat);
// 	return (1);
// }
