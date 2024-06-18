/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_struct.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:16:07 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/18 15:20:52 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_STRUCT_H
# define DATA_STRUCT_H

typedef struct s_camera
{
	float	position[3];
	float	quad[4];
	float	fov;
}			t_camera;

// __attribute__((packed,aligned(4))
// __attribute__((packed))

#endif