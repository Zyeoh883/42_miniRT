/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:38 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/18 19:47:37 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	main(void)
{
	vector_test();
	qua_test();
	return (0);
}

t_camera	init_camera(t_data data)
{
	t_camera	camera;
	float		rot_vector[3];
	float		rot_quat[4];

	// position
	camera.position[0] = 0;
	camera.position[1] = 0;
	camera.position[2] = 0;
	// direction
	camera.quat[0] = 0;
	camera.quat[1] = 0;
	camera.quat[2] = 1;
	camera.quat[3] = 0;
	// fov
	camera.fov = 90;
	camera.fov *= M_PI / 180;
	camera.aspect_ratio = data.win_width / data.win_height;
	camera.pixel_width = 2 * tan(camera.fov / 2);
	camera.pixel_height = camera.pixel_height / camera.aspect_ratio
		/ data.win_height;
	camera.pixel_width = camera.pixel_width / data.win_width;
	// orient camera quat
	print_quat(camera.quat);
	rot_vector[0] = 0;
	rot_vector[1] = 1;
	rot_vector[2] = 0;
	angle_to_quat(atan(camera.pixel_width) * data.win_width / 2, rot_vector,
		rot_quat);
	print_quat(rot_quat);
	quat_rotate(camera.quat, rot_quat, camera.quat);
	print_quat(camera.quat);
	return (camera);
}

int	initialize(t_data *data)
{
	data->win_height = 1080;
	data->win_width = 1920;
	data->img = mlx_new_image(data->mlx_ptr, data->win_width, data->win_height);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_length, &data->endian);
	data->win_ptr = mlx_new_window(data->mlx_ptr, data->win_width,
			data->win_height, "miniRT");
	return (1);
}

// int	main(void)
// {
// 	t_data		data;
// 	t_camera	camera;

// 	data.mlx_ptr = mlx_init();
// 	if (!initialize(&data))
// 		return (1);
// 	camera = init_camera(data);
// 	// mlx_put_image_to_window(&data, data.win_ptr, data.img, 0, 0);
// 	// mlx_loop(data.mlx_ptr);
// 	return (0);
// }
