/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:38 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/24 18:04:05 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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
	// camera.quat = angle_to_quat((t_vector){0, 1, 0}, M_PI / 4);
	// fov
	camera.fov = 45;
	camera.fov *= M_PI / 180;
	// printf("win width: %d\n", data.win_width);
	// printf("win height: %d\n", data.win_height);
	camera.aspect_ratio = (float)data.win_width / data.win_height;
	// printf("AP: %f\n", camera.aspect_ratio);
	camera.pixel_width = 2 * tan(camera.fov / 2);
	camera.pixel_height = camera.pixel_width / camera.aspect_ratio;
	// printf("width: %f\n", camera.pixel_width);
	// printf("height: %f\n\n", camera.pixel_height);
	return (camera);
}

int	initialize(t_data *data)
{
	// data->win_width = 1920;
	// data->win_height = 1080;
	data->win_width = 300;
	data->win_height = 300;
	data->img = mlx_new_image(data->mlx_ptr, data->win_width, data->win_height);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_length, &data->endian);
	data->win_ptr = mlx_new_window(data->mlx_ptr, data->win_width,
			data->win_height, "miniRT");
	data->camera = init_camera(*data);
	ft_bzero(&data->inputs, sizeof(t_inputs));
	data->inputs.key = -1;
	render_frame(*data);
	return (1);
}

// t_data *data,

void	render_frame(t_data data)
{
	t_ray	ray;
	int		color;
	int		x;
	int		y;

	ft_bzero(data.addr, data.win_height * data.line_length);
	y = -1;
	while (++y < data.win_height)
	{
		x = -1;
		while (++x < data.win_width)
		{
			ray = create_ray(data.camera, x, y, data.win_width,
					data.win_height);
			// print_vector(ray.direction);
			color = render_ray(data.camera, ray);
			if (color)
				my_mlx_put_pixels(&data, x, y, color);
		}
	}
	mlx_put_image_to_window(&data, data.win_ptr, data.img, 0, 0);
	// fflush(stdout);
}

int	main(void)
{
	t_data	data;

	data.mlx_ptr = mlx_init();
	if (!initialize(&data))
		return (1);
	// render_frame(data);
	mlx_hook(data.win_ptr, 2, 0, deal_key_press, &data);
	mlx_hook(data.win_ptr, 3, 1, deal_key_release, &data);
	mlx_loop_hook(data.mlx_ptr, deal_input, &data);
	mlx_hook(data.win_ptr, 6, 1L<<6, mouse_hook, &data);
	// mlx_mouse_hook(data.win_ptr, mouse_hook, &data);
	mlx_loop(data.mlx_ptr);
	// mlx_put_image_to_window(&data, data.win_ptr, data.img, 0, 0);
	return (0);
}

// int	main(void)
// {
// 	// vector_test();
// 	qua_test();
// 	init_camera
// 	return (0);
// }