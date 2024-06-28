/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:38 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/28 16:29:47 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_camera	init_camera(t_data *data)
{
	t_camera	camera;
	float		fov;

	camera.data = data;
	// pos
	camera.pos = _mm_set_ps(0, 0, 0, 0);
	camera.quat = _mm_set_ps(0, 0, 0, 1);
	// camera.quat = angle_to_quat((t_vector){0, 1, 0}, M_PI / 4);
	// fov
	fov = 60 * TO_RADIAN;
	// printf("win width: %d\n", data.win_width);
	// printf("win height: %d\n", data.win_height);
	// printf("AP: %f\n", camera.aspect_ratio);
	camera.pitch_angle = 0;
	camera.pixel_width = 2 * tan(fov * 0.5f);
	camera.pixel_height = camera.pixel_width / camera.data->aspect_ratio;
	// printf("width: %f\n", camera.pixel_width);
	// printf("height: %f\n\n", camera.pixel_height);
	return (camera);
}

int	initialize(t_data *data, t_camera *camera)
{
	data->mlx_ptr = mlx_init();
	if (!data->mlx_ptr)
		return (0);
	// data->win_width = 1920;
	// data->win_height = 1080;
	data->win_width = 300;
	data->win_height = 300;
	data->aspect_ratio = (float)data->win_width / data->win_height;
	data->img = mlx_new_image(data->mlx_ptr, data->win_width, data->win_height);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_length, &data->endian);
	data->win_ptr = mlx_new_window(data->mlx_ptr, data->win_width,
			data->win_height, "miniRT");
	ft_bzero(&data->inputs, sizeof(t_inputs));
	data->inputs.key = -1;
	data->inputs.mouse_x = data->win_width / 2;
	data->inputs.mouse_y = data->win_height / 2;
	*camera = init_camera(data);
	data->camera = camera;
	mlx_mouse_move(data->win_ptr, data->win_width / 2, data->win_height / 2);
	render_frame(data);
	return (1);
}

// t_data *data,

void	render_frame(t_data *data)
{
	t_ray	ray;
	int		color;
	int		x;
	int		y;

	ft_bzero(data->addr, data->win_height * data->line_length);
	y = -1;
	// printf("camera pos is:");
	// print_vector(data->camera->pos);
	// double time_start = (double)clock() / CLOCKS_PER_SEC;
	while (++y < data->win_height)
	{
		x = -1;
		while (++x < data->win_width)
		{
			ray = create_ray(data->camera, x, y, data->win_width,
					data->win_height);
			// printf("0-> ");
			// if (x == 0 && y == 0)
			// {
			// 	print_vector(ray.direction);
			// 	print_m128(data->camera->quat);
			// }
			color = render_ray(ray);
			// printf("%d\n", color);
			if (color)
				my_mlx_put_pixels(data, x, y, color);
		}
	}
	printf("\n\n\n\n");
	// printf("%f\n", (double)clock() / CLOCKS_PER_SEC - time_start);
	mlx_put_image_to_window(data, data->win_ptr, data->img, 0, 0);
}
// fflush(stdout);
// print_vector(ray.direction);

int	main(void)
{
	t_camera	camera;
	t_data		data;

	// create_objects_array(create_ll_objects());
	// vector_test();
	if (!initialize(&data, &camera))
		return (1);
	// render_frame(data);
	mlx_mouse_hide();
	mlx_hook(data.win_ptr, 2, 0, deal_key_press, &data);
	mlx_hook(data.win_ptr, 3, 1, deal_key_release, &data);
	mlx_loop_hook(data.mlx_ptr, deal_input, &data);
	mlx_hook(data.win_ptr, 6, 1L << 6, mouse_hook, &camera);
	mlx_loop(data.mlx_ptr);
	mlx_put_image_to_window(&data, data.win_ptr, data.img, 0, 0);
	return (0);
}

// mlx_mouse_hook(data.win_ptr, mouse_hook, &data);
// int	main(void)
// {
// 	// vector_test();
// 	qua_test();
// 	init_camera
// 	return (0);
// }