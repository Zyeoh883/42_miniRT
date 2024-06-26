/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:33:56 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/26 18:50:26 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// (2 * i / width) lies between 0 to 2
// (2 * i / width - 1) lies between -1 to 1, an offset is added

t_ray	create_ray(t_camera *camera, float i, float j, float width, float height)
{
	t_ray	ray;

	ray.pos = &camera->pos;
	// print_vector(*ray.pos);
	ray.direction.i = (2 * i / width - 1) * camera->pixel_width;
	ray.direction.j = (1 - 2 * j / height) * camera->pixel_height;
	ray.direction.k = 1;
	ray.direction = quat_rotate(camera->quat, ray.direction);
	vector_normalize(&ray.direction);
	// ray.direction = vector_normalize(ray.direction);
	return (ray);
}

int	render_ray(t_ray ray)
{
	t_sphere	sphere[8];
	float		t[2];
	t_sphere	*closest_sphere;
	float		closest_t;
	int			n;

	// printf("1. ");
	// print_vector(*ray.pos);
	// rot
	sphere[0].quat = (t_quat){1, 0, 0, 0};
	sphere[0].pos = (t_vector){0, 0, 3};
	sphere[0].radius = 1;
	sphere[0].color = 0xFFFFFF;
	// data,
	sphere[1].quat = (t_quat){1, 0, 0, 0};
	sphere[1].pos = (t_vector){0, -10, 3};
	sphere[1].radius = 1;
	sphere[1].color = 0xFF0000;
	// 3rd
	sphere[2].quat = (t_quat){1, 0, 0, 0};
	sphere[2].pos = (t_vector){0, 0, 10};
	sphere[2].radius = 1;
	sphere[2].color = 0x00FF00;
	// 4th
	sphere[3].quat = (t_quat){1, 0, 0, 0};
	sphere[3].pos = (t_vector){0, 10, 3};
	sphere[3].radius = 1;
	sphere[3].color = 0x0000FF;
	// 5th
	sphere[4].quat = (t_quat){1, 0, 0, 0};
	sphere[4].pos = (t_vector){0, 0, -10};
	sphere[4].radius = 1;
	sphere[4].color = 0xFF00FF;
	// 6th
	sphere[5].quat = (t_quat){1, 0, 0, 0};
	sphere[5].pos = (t_vector){10, 0, 3};
	sphere[5].radius = 1;
	sphere[5].color = 0xFFFF00;
	// 7th
	sphere[6].quat = (t_quat){1, 0, 0, 0};
	sphere[6].pos = (t_vector){-10, 0, 3};
	sphere[6].radius = 1;
	sphere[6].color = 0x00FFFF;
	// looing through spheres
	closest_sphere = NULL;
	closest_t = INFINITY;
	n = -1;
	// printf("2. ");
	// print_vector(*ray.pos);
	while (++n < 7)
	{
		intersect_ray_sphere(sphere[n], ray, t);
		if (t[0] < closest_t && t[0] > 0)
		{
			closest_t = t[0];
			closest_sphere = &sphere[n];
		}
		if (t[1] < closest_t && t[1] > 0)
		{
			closest_t = t[1];
			closest_sphere = &sphere[n];
		}
	}
	if (closest_sphere == NULL)
		return (0);
	return (closest_sphere->color);
}


	// sphere[0] = (t_sphere){
	// 	.quat = (t_quat){1, 0, 0, 0},
	// 	.pos = (t_vector){0, 0, 3},
	// 	.radius = 1,
	// 	.color = 0xFFFFFF,
	// };