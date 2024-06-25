/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 17:31:34 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/25 14:52:15 by zyeoh            ###   ########.fr       */
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
	data->inputs.key = key;
	return (0);
}

int	deal_input(t_data *data)
{
	if (data->inputs.key == -1)
		return (0);
	// ft_printf("input is %d\n", data->inputs.key);
	if (input_translate(data))
		;
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
	render_frame(*data);
	return (0);
}

int	mouse_hook(int x, int y, t_data *data)
{
	input_translate(data);
	quat_normalize(&data->camera.quat);
	if (y - data->inputs.mouse_y > 0 && data->camera.pitch_angle < CAM_LOCK)
	{
		data->camera.pitch_angle += CAM_SENS;
		data->camera.quat = quat_product(data->camera.quat, angle_to_quat((t_vector){1, 0, 0}, CAM_SENS));
	}
	else if (y - data->inputs.mouse_y < 0 && data->camera.pitch_angle > -CAM_LOCK)
	{
		data->camera.pitch_angle -= CAM_SENS;
		data->camera.quat = quat_product(data->camera.quat, angle_to_quat((t_vector){-1, 0, 0}, CAM_SENS));
	}
	if (x - data->inputs.mouse_x > 0)
		data->camera.quat = quat_product(angle_to_quat((t_vector){0, 1, 0}, CAM_SENS), data->camera.quat);
	else if (x - data->inputs.mouse_x < 0)
		data->camera.quat = quat_product(angle_to_quat((t_vector){0, -1, 0}, CAM_SENS), data->camera.quat);
	mlx_mouse_move(data->win_ptr, data->win_width / 2, data->win_height / 2);
	render_frame(*data);	
	return (0);
}
	
		// if (data->inputs.key == I_KEY || data->inputs.key == K_KEY)
		// 	data->camera.quat = quat_product(data->camera.quat, angle_to_quat(v,
		// 				M_PI / 16));