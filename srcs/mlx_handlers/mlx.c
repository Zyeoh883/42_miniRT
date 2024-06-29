/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 17:31:34 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/29 21:35:43 by zyeoh            ###   ########.fr       */
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
	input_translate(data->camera, data->inputs.key);
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
	render_frame(data);
	return (0);
}

int	mouse_hook(int x, int y, t_camera *camera)
{
	int dy;
	int dx;
	
	dy = y - camera->data->inputs.mouse_y;
	dx = x - camera->data->inputs.mouse_x;
	input_translate(camera, camera->data->inputs.key);
	quat_normalize(&camera->quat);
	if ((dy > 0 && camera->pitch_angle < CAM_LOCK) || (dy < 0 && camera->pitch_angle > -CAM_LOCK))
	{
		camera->pitch_angle += CAM_SENS * dy;
		camera->quat = quat_product(camera->quat, angle_to_quat(_mm_set_ps(0, 0, 1, 0), CAM_SENS * dy));
	}
	if (dx != 0)
		camera->quat = quat_product(angle_to_quat(_mm_set_ps(0, 1, 0, 0), CAM_SENS * dx), camera->quat);
	mlx_mouse_move(camera->data->win_ptr, camera->data->win_width * 0.5, camera->data->win_height * 0.5); // ! might not work with int
	render_frame(camera->data);
	return (0);
}
	
		// if (data->inputs.key == I_KEY || data->inputs.key == K_KEY)
		// 	data->camera.quat = quat_product(data->camera.quat, angle_to_quat(v,
		// 				M_PI / 16));

	// if (dy > 0 && camera->pitch_angle < CAM_LOCK)
	// {
	// 	// printf("Lookin up\n");
	// 	camera->pitch_angle += CAM_SENS;
	// 	camera->quat = quat_product(camera->quat, angle_to_quat(_mm_set_ps(0, 0, 1, 0), CAM_SENS * dy));
	// 	// print_m128(camera->quat);
	// }
	// else if (dy < 0 && camera->pitch_angle > -CAM_LOCK)
	// {
	// 	// printf("Lookin down\n");
	// 	camera->pitch_angle -= CAM_SENS;
	// 	camera->quat = quat_product(camera->quat, angle_to_quat(_mm_set_ps(0, 0, -1, 0), CAM_SENS * -dy));
	// }

	// if (dy > 0 && camera->pitch_angle < CAM_LOCK || dy < 0 && camera->pitch_angle > -CAM_LOCK)
	// {
	// 	camera->pitch_angle = CAM_SENS * -(dy < 0);
	// 	camera->quat = quat_product(camera->quat, angle_to_quat(_mm_set_ps(0, 0, 1, 0), CAM_SENS * dy));
	// }