/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:38 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/20 21:52:08 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// quat	quat_con(quat quat)
// {
// 	return ((quat){quat.w, -quat.i, -quat.j, -quat.k});
// }

t_camera	init_camera(t_data data)
{
	t_camera	camera;

	// position
	camera.position.i = 0;
	camera.position.j = 0;
	camera.position.k = 0;
	// direction
	camera.quat.w = 1;
	camera.quat.i = 0;
	camera.quat.j = 0;
	camera.quat.k = 0;
	// fov
	camera.fov = 90;
	camera.fov *= M_PI / 180;
	camera.aspect_ratio = data.win_width / data.win_height;
	camera.pixel_width = 2 * tan(camera.fov / 2);
	camera.pixel_height = camera.pixel_height / camera.aspect_ratio;
	return (camera);
}

int	initialize(t_data *data)
{
	data->win_width = 1920;
	data->win_height = 1080;
	data->img = mlx_new_image(data->mlx_ptr, data->win_width, data->win_height);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_length, &data->endian);
	data->win_ptr = mlx_new_window(data->mlx_ptr, data->win_width,
			data->win_height, "miniRT");
	return (1);
}

void	test(t_data data, t_camera camera)
{
	t_vector	rot_vector;
	t_quat		rot_quat_y;
	t_quat		rot_quat_z;
	t_quat		rot_quat;
	// t_vector	res_vector;
	t_ray		ray;

	// orient camera quat
	rot_vector.i = 2;
	rot_vector.j = 2;
	rot_vector.k = 2;
	vector_normalize(&rot_vector);
	print_vector(rot_vector);
	rot_vector.i = 0;
	rot_vector.j = 1;
	rot_vector.k = 0;
	rot_quat_y = angle_to_quat(rot_vector, atan(camera.pixel_width)
			* data.win_width / 2);
	rot_vector.i = 0;
	rot_vector.j = 0;
	rot_vector.k = 1;
	rot_quat_z = angle_to_quat(rot_vector, atan(camera.pixel_width)
			* data.win_width / 2);
	// print_quat(rot_quat_y);
	// print_quat(rot_quat_z);
	rot_quat = quat_product(rot_quat_y, rot_quat_z);
	// print_quat(rot_quat);
	// res_vector = quat_rotate(rot_quat, rot_vector);
	// print_vector(res_vector);
	// quat_rotate(camera.quat, rot_quat, camera.quat);
	// print_quat(camera.quat);
	ray = create_ray(&camera, 0, 0, data.win_width, data.win_height);
	print_vector(ray.direction);
}

int	main(void)
{
	t_data		data;
	t_camera	camera;

	data.mlx_ptr = mlx_init();
	if (!initialize(&data))
		return (1);
	camera = init_camera(data);
	test(data, camera);
	// mlx_put_image_to_window(&data, data.win_ptr, data.img, 0, 0);
	// mlx_loop(data.mlx_ptr);
	return (0);
}

// int	main(void)
// {
// 	// vector_test();
// 	qua_test();
// 	init_camera
// 	return (0);
// }