/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_struct.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:16:07 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/29 22:44:01 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_STRUCT_H
# define DATA_STRUCT_H

# define SPHERE "S"

// #define t_vector __m128
// #define t_quat __m128

// * reverse order for __m128
typedef	__attribute__((aligned(16))) __m128 t_vector; // {k, j, i, NULL}
typedef	__attribute__((aligned(16))) __m128 t_quat;   // {k, j, i, w}

// typedef struct s_vector
// {
// 	float				i;
// 	float				j;
// 	float				k;
// }						t_vector;

// typedef struct s_quat
// TODO set w to fourth value for speed (small improvement)
// {
// 	float				w;
// 	float				i;
// 	float				j;
// 	float				k;
// }						t_quat;

typedef struct s_ray
{
	__m128				*pos;
	__m128				direction;
}						t_ray;

typedef struct s_sphere
{
	__m128				pos;
	__m128				quat;
	float				radius;
	int					color;
}						t_sphere;

typedef struct s_OBB // ! confirm OBB params
{
	__m128				max;
	__m128				min;
	__m128				quat;
	__m128				half_len;
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