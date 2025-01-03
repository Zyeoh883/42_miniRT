/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:33:56 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/15 15:18:08 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

t_ray	create_ray(U __global t_camera *camera, int i, int j)
{
	t_ray	ray;

	ray.pos = camera->pos;
	ray.direction = (float4)((2 * i / (float)camera->win_width - 1)
			* camera->pixel_width, (1 - 2 * j / (float)camera->win_height)
			* camera->pixel_height, 1, 0);
	ray.direction = quat_rotate(camera->quat, ray.direction);
	return (ray);
}

float get_closest_object(t_ray ray, U __global t_object *objects, t_object **closest_object)
{
	float2		t;
	float			closest_t;

	closest_t = INFINITY;
	while (objects->type != 0)
	{
		t = ray_intersection(objects, ray);
		if (t[0] < closest_t && t[0] > 0)
		{
			closest_t = t[0];
			*closest_object = objects;
		}
		if (t[1] < closest_t && t[1] > 0)
		{
			closest_t = t[1];
			*closest_object = objects;
		}
		objects++;
	}
	return (closest_t);
}

float3 iterative_path_trace(t_ray initial_ray, U __global t_object *objects, uint2 *seed) {
    float3 final_color = (float3)(0.0f, 0.0f, 0.0f);
    float3 throughput = (float3)(1.0f, 1.0f, 1.0f);
    t_ray current_ray = initial_ray;
    
    for (int bounce = 0; bounce < MAX_BOUNCES; bounce++) {
        t_object *hit_obj = get_closest_object(current_ray, objects);
        if (!hit_obj) 
            break;
            
        // Calculate intersection point and normal
        float2 t = ray_intersection(hit_obj, current_ray);
        float4 hit_point = current_ray.pos + 
                          vector_scalar_product(current_ray.direction, t.x);
        float4 normal = vector_normalize(hit_point - hit_obj->pos);
        
        // Add emission if we hit a light
        if (hit_obj->type == LIGHT) {
            final_color += throughput * hit_obj->emission;
            break;
        }
        
        // Calculate direct lighting
        float3 direct_light = calculate_direct_light(hit_point, normal, 
                                                   hit_obj, objects);
        final_color += throughput * direct_light;
        
        // Update throughput for next bounce
        float3 brdf = (float3)(hit_obj->k_diffuse / M_PI);
        throughput *= brdf;
        
        // Russian roulette for path termination
        float p = max(throughput.x, max(throughput.y, throughput.z));
        if (random(seed) > p)
            break;
        throughput /= p;
        
        // Generate new ray direction
        float4 bounce_dir = random_hemisphere_sample(normal, seed);
        float cos_theta = dot(normal, bounce_dir);
        
        // Update ray for next bounce
        current_ray.pos = hit_point + vector_scalar_product(normal, EPSILON);
        current_ray.direction = bounce_dir;
        
        // Account for cosine term and PDF
        throughput *= cos_theta * 2.0f * M_PI;  // PDF = 1/(2π) for hemisphere
    }
    
    return final_color;
}


int rendering_equation(t_ray ray, U __global t_object *objects)
{
  float2 t;
  t_object *hit_object;
  float4 hit_point;
  float4 normal;
  t_ray sample_ray;

  t = get_closest_object(ray, objects, &hit_object);
  hit_point = ray.pos + vector_scalar_product(ray.direction, t);
  normal = vector_normalize(hit_point - hit_object->pos);



}

__kernel void	render_scene(U __global uchar *addr,
	U __global t_camera *camera, U __global t_object *objects)
{
	U __global uchar	*dst;
  t_ray ray;
  t_object *closest_object;
  float2 t; 
	int					color;
	int					x;
	int					y;

	x = get_global_id(0);
	y = get_global_id(1);

  ray = create_ray(camera, x, y);
  color = rendering_equation(ray, objects);
	dst = addr + (y * camera->line_length + x * (camera->bytes_per_pixel));
  *(__global unsigned int *)dst = color;
}

// __kernel void	render_scene(u __global uchar *addr,
		// u __global t_camera *camera,
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
// 	dst = (uchar *)addr + (y * camera->line_length + x
			// * (camera->bytes_per_pixel));
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

// __kernel void	render_scene(u __global uchar *addr,
		// u __global t_camera *camera,
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
// 	dst = (uchar *)addr + (y * camera->line_length + x
			// * (camera->bytes_per_pixel));
// 	*(unsigned int *)dst = 0xFFFFFF;
// }
