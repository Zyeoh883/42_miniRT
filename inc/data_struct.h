/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_struct.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:16:07 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/25 19:30:56 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_STRUCT_H
# define DATA_STRUCT_H

# define SPHERE "S"

typedef struct s_vector
{
	float				i;
	float				j;
	float				k;
}						t_vector;

typedef struct s_quat
{
	float				w;
	float				i;
	float				j;
	float				k;
}						t_quat;

typedef struct s_camera
{
	t_vector			pos;
	t_quat				quat;
	float				fov;
	float				aspect_ratio;
	float				pixel_width;
	float				pixel_height;
	float				pitch_angle;
}						t_camera;

typedef struct s_ray
{
	t_vector			pos;
	t_vector			direction;
}						t_ray;

typedef struct s_sphere
{
	t_vector			pos;
	t_quat				quat;
	float				radius;
	int					color;
}						t_sphere;

typedef struct s_OBB
{
	t_vector			pos;
	t_quat				quat;
	t_vector			half_len;
}						t_OBB;

typedef struct s_object // ! type == 0 means NULL terminated
{
	struct s_OBB		obb;
	char				type;
	union
	{
		struct s_sphere	sphere;
	};
}						t_object;

typedef struct s_BHVnode
{
	struct s_OBB		obb;
	struct s_BHVnode	*left;
	struct s_BHVnode	*right;
	struct t_object		*object;
	int					object_count;
}						t_BHVnode;

// __attribute__((packed,aligned(4))
// __attribute__((packed))

#endif