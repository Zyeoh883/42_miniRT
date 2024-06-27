/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_struct.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:16:07 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/27 10:46:27 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_STRUCT_H
# define DATA_STRUCT_H

# define SPHERE "S"


// * reverse order for __m128
typedef __m128 t_vector;	// {k, j, i, NULL}
typedef __m128 t_quat;		// {k, j, i, w}

// typedef struct s_vector
// {
// 	float				i;
// 	float				j;
// 	float				k;
// }						t_vector;

// typedef struct s_quat // TODO set w to fourth value for speed (small improvement)
// {
// 	float				w;
// 	float				i;
// 	float				j;
// 	float				k;
// }						t_quat;

typedef struct s_camera
{
	__m128				pos;
	__m128				quat;
	float				fov;
	float				aspect_ratio;
	float				pixel_width;
	float				pixel_height;
	float				pitch_angle;
}						t_camera;

typedef struct s_ray
{
	__m128 			*pos;
	__m128			direction;
}						t_ray;

typedef struct s_sphere
{
	__m128			pos;
	__m128				quat;
	float				radius;
	int					color;
}						t_sphere;

typedef struct s_OBB
{
	__m128			max;
	__m128			min;
	__m128				quat;
	__m128			half_len;
}						t_OBB;

typedef struct s_object // ! type == 0 means NULL terminated
{
	struct s_OBB obb;
	char type;
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