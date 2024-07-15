/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:49 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/15 06:46:50 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

// funni stuff
# include <OpenCL/opencl.h>
// end of funni

# include "data_struct.h"
# include "libft.h"
# include "math_utils.h"
# include "mlx.h"
# include "mlx_handlers.h"
# include "opencl_handlers.h"
// # include <float.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <time.h>

# if defined(__APPLE__)
#  include "macos_defines.h"
# elif defined(__linux__)
#  define PTR_NULL "(nil)"
# endif

# define CAM_SENS M_PI * 0.001
# define CAM_LOCK 80 * TO_RADIAN

//__attribute__((packed))

typedef struct __attribute__ ((aligned(16))) s_camera
{
	cl_float4	pos;
	cl_float4	quat;
	cl_float	pixel_width;
	cl_float	pixel_height;
	cl_int		win_height;
	cl_int		win_width;
	cl_uchar	num_objects;
	cl_int		bytes_per_pixel;
	cl_int		line_length;
}						t_camera;
	// float pitch_angle;
	// t_data *data;
	// t_object *objects;

typedef struct __attribute__ ((aligned(16))) s_inputs
{
	int key;
	int key_held;
	int mouse_x;
	int mouse_y;
	int	shift;
	float pitch_angle; // * Moved pitch angle to inputs
}						t_inputs;

typedef struct __attribute__ ((aligned(16))) s_data
{
	t_camera *camera;
	t_opencl *opencl;
	t_object *objects;
	void *mlx_ptr;
	void *win_ptr;
	void *img;
	char *addr;
	int bits_per_pixel;
	int line_length;
	int endian;
	int win_height;
	int win_width;
	t_inputs inputs;
}						t_data;

// init
t_camera	*init_camera(t_data *data, int win_height, int win_width);
int						initialize(t_data *data, t_camera *camera);
t_list					*create_ll_objects(void);
t_object				*create_objects_array(t_list *root_node);
t_OBB					assign_sphere_obb(t_sphere sphere);

void					render_frame(t_data *data, t_opencl *opencl);

// tests
// void			vector_test(void);
// void			qua_test(void);

// rays
t_ray					create_ray(t_camera *camera, cl_int i, cl_int j);
void					intersect_ray_sphere(t_sphere sphere, t_ray ray,
							float t[2]);
int						render_ray(t_ray ray, t_object *objects);
// int						intersect_aabb(t_ray *ray, t_vector max,
// t_vector min);
int						intersect_obb(t_ray *ray, t_OBB obb);

// error_handling
void					perror_and_exit(char *str, int exit_code);

// utils
void					print_m128(__m128 m);
char					*read_cfile(char *name);

#endif