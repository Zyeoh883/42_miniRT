/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:49 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/19 19:52:10 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

// funni stuff
# include <CL/opencl.h>
// end of funni

// this is temporary for debug
# include "data_struct.h"
# include "libft.h"
# include "math_utils.h"
# include "mlx.h"
# include "mlx_handlers.h"
# include "opencl_handlers.h"
# include <X11/Xlib.h>
// # include <float.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <time.h>

# if defined(__APPLE__)
#  include "macos_defines.h"
# elif defined(__linux__)
#  include "linux_defines.h"
#  define PTR_NULL "(nil)"
# endif

# define BASE_10_DIGITS "1234567890"
# define LONG_NUM_DIGITS 19

# define WIN_WIDTH 2560
# define WIN_HEIGHT 1600
# define CAM_SENS 0.001
# define CAM_LOCK 80
# define FOV 60

// Objects ID
# define AMBIENT "A"
# define CAMERA "C"
# define LIGHT "L"
# define SPHERE "sp"
# define PLANE "pl"
# define CYLINDER "cy"

// Material Types
# define MAT_DIFFUSE "D"
# define MAT_METAL_COPPER "MC"
# define MAT_METAL_GOLD "MG"
# define MAT_METAL_SILVER "MS"
# define MAT_METAL_IRON "MI"
# define MAT_DIELECTRIC_GLASS "DG"
# define MAT_METAL_DIELECTRIC_COPPER "MDC"
# define MAT_METAL_DIELECTRIC_GOLD "MDG"
# define MAT_MIRROR "M"

// __attribute__((packed))
typedef struct __attribute__((aligned(16))) s_ambient
{
	cl_float	amb_light_ratio;
	cl_float	amb_fade_ratio;
	cl_float3	amb_top_color;
	cl_float3	amb_bot_color;
}				t_ambient;

typedef struct __attribute__((aligned(16))) s_camera
{
	cl_float3	pos;
	cl_float4	quat;
	cl_float	pixel_width;
	cl_float	pixel_height;
	cl_int		win_height;
	cl_int		win_width;
	cl_float	fov;
	cl_int		bytes_per_pixel;
	cl_int		line_length;

	t_ambient	ambient;
	cl_uchar	moved;
	// cl_float amb_light_ratio;
	// cl_float amb_fade_ratio;
	// cl_float3 amb_top_color;
	// cl_float3 amb_bot_color;
}				t_camera;
// float pitch_angle;
// t_data *data;
// t_object *objects;

typedef struct __attribute__((aligned(16))) s_inputs
{
	// int key_wasd_held[4];
	int		key_wasd[4];
	int		key;
	int		key_held;
	int		mouse_x;
	int		mouse_y;
	int		shift;
	float	pitch_angle;
	int		update;
}				t_inputs;

typedef struct __attribute__((aligned(16))) s_data
{
	t_list		*file_content;
	t_camera	*camera;
	t_opencl	*opencl;
	t_object	*objects;
	t_reservoir	*reservoirs;
	int			num_objects;
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
t_camera		*init_camera(t_data *data, int win_height, int win_width);
int				initialize(t_data *data, char *filename);
t_list			*create_ll_objects(void);
t_object		*create_objects_array(t_list *root_node);
t_OBB			assign_sphere_obb(t_sphere sphere);
t_list			*get_rt_file(char *filename);

t_object		*get_objects(t_data *data, t_list *root_node);
t_camera		get_camera(t_list *root_node);

int				render_frame(t_data *data);

// create_objects

void			assign_sphere(t_object *object, char **split, int *line_offset);
void			assign_plane(t_object *object, char **split, int *line_offset);
void			assign_cylinder(t_object *object, char **line,
					int *line_offset);
void			assign_light(t_object *object, char **split, int *line_offset);

void			assign_camera(t_camera *camera, char **line);
void			assign_ambient(t_camera *camera, char **line);
// t_OBB assign_sphere_obb(t_sphere sphere);

// create utils

int				is_valid_object_id(char *id);
int				is_valid_camera_ambient_id(char *id);
int				is_material_type(char *id);
cl_float		get_cl_float(char *str);
cl_float3		get_cl_float3(char *str);
cl_float3		get_rgb_value(char *str);
cl_float3		get_dir(char *str);

// tests
// void			vector_test(void);
// void			qua_test(void);

// rays
t_ray			create_ray(t_camera *camera, cl_int i, cl_int j);
void			intersect_ray_sphere(t_sphere sphere, t_ray ray, float t[2]);
// int						render_ray(t_ray ray, t_object
// *objects); int
// intersect_aabb(t_ray *ray, t_vector max, t_vector min);
int				intersect_obb(t_ray *ray, t_OBB obb);

// error_handling
void			perror_and_exit(char *str, int exit_code);
void			error_and_exit(char *str, int exit_code);

// utils
void			print_m128(__m128 m);
char			*read_cfile(char *name);
void			free_cfile(char **c_file);
void			free_str_arr(char **str_arr);
int				count_objects(t_list *root_node);
int				ft_str_arr_len(char **str);
int				is_rgb(char *str);
cl_float4		get_quat_value(char *str);
cl_float3		inv_rgb_float(int rgb);
cl_float		get_cl_float(char *str);
cl_float3		get_cl_float3(char *str);
cl_float3		get_dir(char *str);
cl_float3		get_rgb_value(char *str);

t_object		assign_object(char **line);
void			assign_cylinder(t_object *object, char **line,
					int *line_offset);
void			assign_light(t_object *object, char **line, int *line_offset);
void			assign_plane(t_object *object, char **line, int *line_offset);
void			assign_sphere(t_object *object, char **line, int *line_offset);
void			assign_camera(t_camera *camera, char **line);
void			assign_ambient(t_camera *camera, char **line);

float			apply_material_properties_base(t_object *object,
					char *material_type);
float			apply_material_properties_base2(t_object *object,
					char *material_type);

void			end_program(t_data *data);
#endif
