/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:33:56 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/20 22:12:50 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_ray	create_ray(t_camera *camera, int i, int j, int width, int height)
{
	t_ray ray;

	ray.position.i = camera->position.i;
	ray.position.j = camera->position.j;
	ray.position.k = camera->position.k;

    ray.direction.i = (2 * i / width - 1) * camera->pixel_width;
    ray.direction.j = (1 - 2 * j / height) * camera->pixel_height;
    ray.direction.k = -1;

	// ray.direction = quat_rotate(camera->quat, ray.direction);
	vector_normalize(&ray.direction);
	// ray.direction = vector_normalize(ray.direction);


	return (ray);
}
