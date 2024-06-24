/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 17:31:34 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/24 15:00:47 by zyeoh            ###   ########.fr       */
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
	if (data->key == key)
	{
		data->key = -1;
		data->key_held = false;
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
	data->key = key;
	return (0);
}

int	deal_input(t_data *data)
{
	if (data->key == -1)
		return (0);
	// ft_printf("input is %d\n", data->key);
	if (input_translate(data))
		;
	else if (input_rotation(data))
		;
	// else if (input_scale(data))
	// 	;
	// else if (input_perspective(data))
	// 	;
	// ! add slerp to reset key
	// else if (data->key == B_KEY && !data->key_held)
	// {
	// 	// free_map(data->map);
	// 	data->map = copy_map(data->map_copy);
	// 	if (!data->map)
	// 	{
	// 		free_map(data->map_copy);
	// 		free_map(data->map);
	// 		error_reset();
	// 		return (-1);
	// 	}
	// }
	data->key_held = true;
	render_frame(*data);
	return (0);
}

// int	mouse_hook(t_data *data)
// {
	
// 	// data->key_held = true;
// 	// render_frame(*data);
// 	return (0);
// }