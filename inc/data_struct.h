/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_struct.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:16:07 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/21 17:11:56 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_STRUCT_H
# define DATA_STRUCT_H

typedef struct s_vector
{
	float		i;
	float		j;
	float		k;
}				t_vector;

typedef struct s_quat
{
	float		w;
	float		i;
	float		j;
	float		k;
}				t_quat;

typedef struct s_camera
{
	t_vector	position;
	t_quat		quat;
	float		fov;
	float		aspect_ratio;
	float		pixel_width;
	float		pixel_height;
}				t_camera;

typedef struct s_ray
{
	t_vector	position;
	t_vector	direction;
}				t_ray;

typedef struct s_sphere
{
	t_vector	position;
	t_quat		quat;
	float		radius;
	int			color;
}				t_sphere;

// __attribute__((packed,aligned(4))
// __attribute__((packed))

#endif