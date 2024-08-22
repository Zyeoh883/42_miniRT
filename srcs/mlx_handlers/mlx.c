/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 17:31:34 by zyeoh             #+#    #+#             */
/*   Updated: 2024/08/22 17:47:37 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

inline void	my_mlx_put_pixels(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	deal_key_release(int key, t_data *data)
{
	if (data->inputs.key == key)
	{
		data->inputs.key = -1;
		data->inputs.key_held = false;
	}
	if (key == SHIFT_KEY)
		data->inputs.shift = false;
	else if (key == W_KEY)
		data->inputs.key_wasd[0] = 0;
	else if (key == A_KEY)
		data->inputs.key_wasd[1] = 0;
	else if (key == S_KEY)
		data->inputs.key_wasd[2] = 0;
	else if (key == D_KEY)
		data->inputs.key_wasd[3] = 0;
	return (0);
}

int	deal_key_press(int key, t_data *data) // ! does not free
{
	if (key == ESC_KEY)
	{
		// free_map(data->map);
		// free_map(data->map_copy);
		mlx_destroy_window(data->mlx_ptr, data->win_ptr);
		exit(0);
	}
	if (key == SHIFT_KEY)
	{
		data->inputs.shift = true;
		return (0);
	}
	if (key == W_KEY)
		data->inputs.key_wasd[0] = 1;
	else if (key == A_KEY)
		data->inputs.key_wasd[1] = 1;
	else if (key == S_KEY)
		data->inputs.key_wasd[2] = 1;
	else if (key == D_KEY)
		data->inputs.key_wasd[3] = 1;
	else
		data->inputs.key = key;
	return (0);
}

int	deal_input(t_data *data)
{
	if (data->inputs.key == -1 && !data->inputs.key_wasd[0] && !data->inputs.key_wasd[1] && !data->inputs.key_wasd[2] && !data->inputs.key_wasd[3])
		return (0);
	input_translate(data, data->inputs);
	// ! add slerp to reset key
	// else if (data->inputs.key == B_KEY && !data->inputs.key_held)
	// {
	// 	// free_map(data->inputs.map);
	// 	data->inputs.map = copy_map(data->inputs.map_copy);
	// 	if (!data->inputs.map)
	// 	{
	// 		free_map(data->inputs.map_copy);
	// 		free_map(data->inputs.map);
	// 		error_reset();
	// 		return (-1);
	// 	}
	// }
	data->inputs.key_held = true;
	render_frame(data, data->opencl);
	return (0);
}

int	mouse_hook(int x, int y, t_data *data)
{
	int dy;
	int dx;
	
	dy = y - data->inputs.mouse_y;
	dx = x - data->inputs.mouse_x;
	input_translate(data, data->inputs);
	// deal_input(data);
	data->camera->quat = quat_normalize(data->camera->quat);
	if ((dy > 0 && data->inputs.pitch_angle < CAM_LOCK) || (dy < 0 && data->inputs.pitch_angle > -CAM_LOCK))
	{
		data->inputs.pitch_angle += CAM_SENS * dy;
		data->camera->quat = quat_product(data->camera->quat, angle_to_quat((cl_float4){{1, 0, 0, 0}}, CAM_SENS * dy));
	}
	if (dx != 0)
		data->camera->quat = quat_product(angle_to_quat((cl_float4){{0, 1, 0, 0}}, CAM_SENS * dx), data->camera->quat);
	mlx_mouse_move(data->win_ptr, data->win_width * 0.5, data->win_height * 0.5); // ! might not work with int
	render_frame(data, data->opencl);
	return (0);
}
