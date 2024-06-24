/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:38 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/24 09:52:48 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// quat	quat_con(quat quat)
// {
// 	return ((quat){quat.w, -quat.i, -quat.j, -quat.k});
// }

t_camera	*init_camera(t_data data)
{
	t_camera	*camera;

	camera = ft_calloc(1, sizeof(t_camera));
	if (!camera)
		return (NULL);
	// position
	camera->position.i = 0;
	camera->position.j = 0;
	camera->position.k = 0;
	// direction
	camera->quat.w = 1;
	camera->quat.i = 0;
	camera->quat.j = 0;
	camera->quat.k = 0;
	camera->quat = angle_to_quat((t_vector){0, 1, 0}, M_PI / 4);
	// fov
	camera->fov = 90;
	camera->fov *= M_PI / 180;
	printf("win width: %d\n", data.win_width);
	printf("win height: %d\n", data.win_height);
	camera->aspect_ratio = (float)data.win_width / data.win_height;
	printf("AP: %f\n", camera->aspect_ratio);
	camera->pixel_width = 2 * tan(camera->fov / 2);
	camera->pixel_height = camera->pixel_width / camera->aspect_ratio;
	printf("width: %f\n", camera->pixel_width);
	printf("height: %f\n\n", camera->pixel_height);
	return (camera);
}

int	initialize(t_data *data)
{
	data->win_width = 1920;
	data->win_height = 1080;
	// data->win_width = 100;
	// data->win_height = 100;
	data->img = mlx_new_image(data->mlx_ptr, data->win_width, data->win_height);
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_length, &data->endian);
	data->win_ptr = mlx_new_window(data->mlx_ptr, data->win_width,
			data->win_height, "miniRT");
	data->camera = init_camera(*data);
	if (!data->camera)
		return (0);
	return (1);
}

// t_data *data,

void	intersect_ray_sphere(t_camera *camera, t_sphere *sphere, t_ray ray,
		float t[2])
{
	t_vector	sphere_to_camera;
	float		a;
	float		b;
	float		c;
	float		discriminant;

	sphere_to_camera = vector_subtraction(camera->position, sphere->position);
	a = vector_dot_product(ray.direction, ray.direction);
	b = 2 * vector_dot_product(sphere_to_camera, ray.direction);
	c = vector_dot_product(sphere_to_camera, sphere_to_camera) - (sphere->radius * sphere->radius);
	// calculate num of intersects
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
	{
		t[0] = INFINITY;
		t[1] = INFINITY;
		return ;
	}
	discriminant = sqrt(discriminant);
	t[0] = (-b + discriminant) / (2 * a);
	t[1] = (-b - discriminant) / (2 * a);
}

int	render_ray(t_camera *camera, t_ray ray)
{
	t_sphere	sphere;
	int			color_result;
	float		t[2];

	// rot
	sphere.quat.w = 1;
	sphere.quat.i = 0;
	sphere.quat.j = 0;
	sphere.quat.k = 0;
	// pos
	sphere.position.i = 0;
	sphere.position.j = 0;
	sphere.position.k = 9;
	// radius
	sphere.radius = 1;
	// color
	sphere.color = 0xFFFFFF;
	color_result = sphere.color;
	// data,
	intersect_ray_sphere(camera, &sphere, ray, t);
	if (t[0] == INFINITY)
		return (0);
	return (color_result);
}

void	render_frame(t_data *data, t_camera *camera)
{
	t_ray	ray;
	int		color;
	int		x;
	int		y;

	y = -1;
	while (++y < data->win_height)
	{
		x = -1;
		while (++x < data->win_width)
		{
			ray = create_ray(camera, x, y, data->win_width, data->win_height);
			color = render_ray(camera, ray);
			// printf("%d", color);
			my_mlx_put_pixels(data, x, y, color);
		}
		// printf("\n");
	}
	// fflush(stdout);
}

int	main(void)
{
	t_data		data;
	t_camera	camera;

	data.mlx_ptr = mlx_init();
	if (!initialize(&data))
		return (1);
	render_frame(&data, &camera);
	// test(data, camera);
	mlx_put_image_to_window(&data, data.win_ptr, data.img, 0, 0);
	mlx_loop(data.mlx_ptr);
	return (0);
}

// int	main(void)
// {
// 	// vector_test();
// 	qua_test();
// 	init_camera
// 	return (0);
// }