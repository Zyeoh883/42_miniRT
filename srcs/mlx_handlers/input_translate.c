/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_translate.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 10:33:06 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/15 07:04:09 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	input_translate(t_data *data, int key)
{
	cl_float4	v;

	if (key == SPACE_KEY)
		v = (cl_float4){{0, 1, 0, 0}};
	else if (key == CTRL_KEY)
		v = (cl_float4){{0, -1, 0, 0}};
	else if (key == D_KEY)
		v = quat_rotate(data->camera->quat, (cl_float4){{1, 0, 0, 0}});
	else if (key == A_KEY)
		v = quat_rotate(data->camera->quat, (cl_float4){{-1, 0, 0, 0}});
	else if (key == W_KEY || key == S_KEY)
	{
		v = (cl_float4){{0, 0, 1, 0}};
		if (key == S_KEY)
			v = (cl_float4){{0, 0, -1, 0}};
		v = quat_rotate(data->camera->quat, v);
		v.s[1] = 0;
		v = vector_normalize(v);
	}
	else
		return ;
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
// 					M_PI / 16));
// 	else
// 		data->camera.quat = quat_product(angle_to_quat(v, M_PI / 16),
// 				data->camera.quat);
// 	return (1);
// }
