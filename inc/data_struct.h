/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_struct.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:16:07 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/18 16:06:40 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_STRUCT_H
# define DATA_STRUCT_H

typedef struct s_camera
{
	float	position[3];
	float	quat[4];
	float	fov;
	float	aspect_ratio;
	float	pixel_width;
	float	pixel_height;
}			t_camera;

// __attribute__((packed,aligned(4))
// __attribute__((packed))

#endif