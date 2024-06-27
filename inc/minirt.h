/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:49 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/27 06:58:20 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "data_struct.h"
# include "libft.h"
# include "math_utils.h"
# include "mlx.h"
# include "mlx_handlers.h"
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <time.h>

# if defined(__APPLE__)
#  include "macos_defines.h"
# elif defined(__linux__)
#  define PTR_NULL "(nil)"
# endif

// funni stuff
// # include <altivec.h>
# include <xmmintrin.h>
# include <emmintrin.h>
# include <pmmintrin.h>
# include <tmmintrin.h>
# include <smmintrin.h>
# include <nmmintrin.h>
# include <immintrin.h>
// end of funni

# define CAM_SENS M_PI / 24
# define CAM_LOCK 80 * TO_RADIAN

typedef struct s_inputs
{
	int			key;
	int			key_held;
	int			mouse_x;
	int			mouse_y;
}				t_inputs;

typedef struct s_data
{
	t_camera	camera;
	void		*mlx_ptr;
	void		*win_ptr;
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	int			win_height;
	int			win_width;
	t_inputs	inputs;
}				t_data;

// init
t_camera		init_camera(t_data data);
int				initialize(t_data *data);
t_list			*create_ll_objects(void);
t_object		*create_objects_array(t_list *root_node);

void			render_frame(t_data data);

// tests
void			vector_test(void);
void			qua_test(void);

// rays
t_ray			create_ray(t_camera *camera, float i, float j, float width,
					float height);
void			intersect_ray_sphere(t_sphere sphere, t_ray ray, float t[2]);
int				render_ray(t_ray ray);

// error_handling
void			perror_and_exit(char *str, int exit_code);

// utils
void			swap_floats(float *A, float *B);

#endif