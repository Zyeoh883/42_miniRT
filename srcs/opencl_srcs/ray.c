/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:33:56 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/12 14:19:44 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define u

typedef struct __attribute__ ((aligned(16))) s_camera {
    float4 pos;
    float4 quat;
    float pixel_width;
    float pixel_height;
    int win_height;
    int win_width;
    char num_objects;
    int bytes_per_pixel;
    int line_length;
} t_camera;

typedef struct __attribute__ ((aligned(16))) s_ray {
    float4 pos;
    float4 direction;
} t_ray;

typedef struct __attribute__ ((aligned(16))) s_sphere {
    float4 pos;
    float4 quat;
    float radius;
    int color;
} t_sphere;

typedef struct __attribute__ ((aligned(16))) s_object {
    uchar type;
    union {
        struct __attribute__ ((aligned(16))) s_sphere sphere;
    };
} t_object;

// Function prototype
t_ray create_ray(__global t_camera *camera, int i, int j);

// Function definition
t_ray create_ray(__global t_camera *camera, int i, int j) {
    t_ray ray;
    ray.pos = camera->pos;
    ray.direction = (float4)(
        1,
        (1 - 2 * j / (float)camera->win_height) * camera->pixel_height,
        (2 * i / (float)camera->win_width - 1) * camera->pixel_width,
        0
    );
    return ray;
}


int	render_ray(t_ray ray, t_object *objects)
{
	float2		t;
	t_sphere	*closest_sphere;
	float		closest_t;
	int			n;

	closest_sphere = 0;
	closest_t = INFINITY;
	n = 0;
	while (objects->type != 0)
	{
		intersect_ray_sphere(objects->sphere, ray, t);
		if (t[0] < closest_t && t[0] > 0)
		{
			closest_t = t[0];
			closest_sphere = &objects->sphere;
		}
		if (t[1] < closest_t && t[1] > 0)
		{
			closest_t = t[1];
			closest_sphere = &objects->sphere;
		}
		objects++;
	}
	// if (n == 1)
	// 	return (0xFFFFFF);
	// else
	// 	return (0);
	if (closest_sphere == 0)
		return (1);
	return (closest_sphere->color);
}


__kernel void render_scene(__global uchar *addr, __global t_camera *camera, __global t_object *objects) {
    __global uchar *dst;
    int color;
    int x;
    int y;

    x = get_global_id(0);
    y = get_global_id(1);
    
	dst = addr + (y * camera->line_length + x * (camera->bytes_per_pixel));
	// t_ray ray = create_ray(camera, x, y);
    // color = render_ray(create_ray(camera, x, y), objects);
    color = x * 0xFF / camera->win_width;
    color += (y * 0xFF / camera->win_height) << 8;
    // color += y;

	*(__global unsigned int *)dst = color;
}

// __kernel void	render_scene(u __global uchar *addr, u __global t_camera *camera,
// 	u __global t_object *objects)
// {
// 	// t_ray	ray;
// 	uchar	*dst;
// 	// int color;
// 	int	x;
// 	int	y;

// 	x = get_global_id(0);
// 	y = get_global_id(1);
// 	// ray = create_ray(camera, i, j);
// 	// color = render_ray(create_ray(camera, i, j), objects);
// 	// if (color)
// 		// put color in image
// 	dst = (uchar *)addr + (y * camera->line_length + x * (camera->bytes_per_pixel));
// 	*(unsigned int *)dst = 0xFFFFFF;
// }




// #include "minirt.h"

// typedef struct __attribute__ ((aligned(16))) s_camera
// {
// 	float4	pos;
// 	float4	quat;
// 	float	pixel_width;
// 	float	pixel_height;
// 	int		win_height;
// 	int		win_width;
// 	char	num_objects;
// 	int		bytes_per_pixel;
// 	int		line_length;
// }						t_camera;

// typedef struct __attribute__ ((aligned(16))) s_ray
// {
// 	float4	pos; // ! changed from pointer to value
// 	float4	direction;
// }						t_ray;

// typedef struct __attribute__ ((aligned(16))) s_sphere
// {
// 	float4	pos;
// 	float4	quat;
// 	float		radius;
// 	int			color;
// }						t_sphere;

// typedef struct __attribute__ ((aligned(16)))  s_object
// {
// 	// struct __attribute__ ((aligned(16))) s_OBB obb;
// 	uchar				type;
// 	union
// 	{
// 		struct __attribute__ ((aligned(16))) s_sphere	sphere;
// 	};
// }						t_object;

// #define u


// t_ray	create_ray(t_camera *camera, int i, int j);
// // (2 * i / width) lies between 0 to 2
// // (2 * i / width - 1) lies between -1 to 1, an offset is added

// t_ray	create_ray(t_camera *camera, int i, int j)
// {
// 	t_ray	ray;
// 	// int	height;
// 	// int	width;

// 	ray.pos = camera->pos;
// 	ray.direction = (float4){
// 		1,
// 		(1 - 2 * j / camera->win_height) * camera->pixel_height,
// 		(2 * i / camera->win_width - 1) * camera->pixel_width,
// 		0
// 	};
// 	// ray.direction = quat_rotate(camera->quat, ray.direction);
// 	// vector_normalize(&ray.direction);
// 	// print_m128(ray.direction);
// 	return (ray);
// }

// int	render_ray(t_ray ray, t_object *objects)
// {
// 	float		t[2];
// 	t_sphere	*closest_sphere;
// 	float		closest_t;
// 	int			n;

// 	closest_sphere = 0;
// 	closest_t = INFINITY;
// 	// printf("2. ");
// 	// print_vector(*ray.pos);
// 	n = 0;
// 	while (objects->type != 0)
// 	{
// 		intersect_ray_sphere(objects->sphere, ray, t);
// 		if (t[0] < closest_t && t[0] > 0)
// 		{
// 			closest_t = t[0];
// 			closest_sphere = &objects->sphere;
// 		}
// 		if (t[1] < closest_t && t[1] > 0)
// 		{
// 			closest_t = t[1];
// 			closest_sphere = &objects->sphere;
// 		}
// 		objects++;
// 	}
// 	// if (n == 1)
// 	// 	return (0xFFFFFF);
// 	// else
// 	// 	return (0);
// 	if (closest_sphere == 0)
// 		return (1);
// 	return (closest_sphere->color);
// }

// __kernel void	render_scene(u __global uchar *addr, u __global t_camera *camera,
// 	u __global t_object *objects)
// {
// 	// t_ray	ray;
// 	uchar	*dst;
// 	// int color;
// 	int	x;
// 	int	y;

// 	x = get_global_id(0);
// 	y = get_global_id(1);
// 	// ray = create_ray(camera, i, j);
// 	// color = render_ray(create_ray(camera, i, j), objects);
// 	// if (color)
// 		// put color in image
// 	dst = (uchar *)addr + (y * camera->line_length + x * (camera->bytes_per_pixel));
// 	*(unsigned int *)dst = 0xFFFFFF;
// }


