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


// Linear congruential generator
static float get_random(unsigned int *seed0, unsigned int *seed1) {

	/* hash the seeds using bitwise and operations and bitshifts */
	*seed0 = 36969 * ((*seed0) & 65535) + ((*seed0) >> 16);  
	*seed1 = 18000 * ((*seed1) & 65535) + ((*seed1) >> 16);

	unsigned int ires = ((*seed0) << 16) + (*seed1);

	/* use union struct to convert int to float */
	union {
		float f;
		unsigned int ui;
	} res;

	res.ui = (ires & 0x007fffff) | 0x40000000;  /* bitwise and, bitwise or */
	return (res.f - 2.0f) / 2.0f;
}

t_ray	create_ray(U __global t_camera *camera, int i, int j)
{
	t_ray	ray;

	ray.pos = camera->pos;
	ray.dir = (float3)((2 * i / (float)camera->win_width - 1)
			* camera->pixel_width, (1 - 2 * j / (float)camera->win_height)
			* camera->pixel_height, 1);
  // ray.dir = (float3)(camera->win_width / 2, camera->win_height / 2, 1);
	ray.dir = quat_rotate(camera->quat, ray.dir);
	return (ray);
}

 int intersect_scene(t_ray ray, U __global t_object *objects, U __global t_object **hit_object, float *closest_t)
 {
 	float2			t;

 	while (objects->obj_type != 0)
 	{
 		t = ray_intersection(objects, ray);
 		if (t[0] > 0 && *closest_t < t[0])
 		{
 			*closest_t = t[0];
 			*hit_object = objects;
 		}
 		if (t[1] > 0 && t[1] < *closest_t)
 		{
 			*closest_t = t[1];
      *hit_object = objects;
 		}
 		objects++;
 	}
   if (*closest_t == INFINITY)
     return 0;
   return 1;
 }


t_ray generate_hemisphere_ray(float3 hit_point, float3 normal, unsigned int *seed0, unsigned int *seed1)
{
  t_ray new_ray;

  new_ray.pos = hit_point + normal * 0.0001f;


  /* compute two random numbers to pick a random point on the hemisphere above the hitpoint*/
	float rand1 = 2.0f * M_PI * get_random(seed0, seed1);
	float rand2 = get_random(seed0, seed1);
	float rand2s = sqrt(rand2);

  /* create a local orthogonal coordinate frame centered at the hitpoint */
	float3 w = normal;
	float3 axis = fabs(w.x) > 0.1f ? (float3)(0.0f, 1.0f, 0.0f) : (float3)(1.0f, 0.0f, 0.0f);
	float3 u = normalize(cross(axis, w));
	float3 v = cross(w, u);

	/* use the coordinte frame and random numbers to compute the next ray direction */
	new_ray.dir = normalize(u * cos(rand1)*rand2s + v*sin(rand1)*rand2s + w*sqrt(1.0f - rand2));

  return (new_ray);
}


float3 get_normal(U __global t_object *hit_object, float3 hit_point, float3 ray_dir)
{
  float3 resultant_normal;

  if (hit_object->obj_type == SPHERE)
    resultant_normal = hit_point - hit_object->pos;
  else if (hit_object->obj_type == PLANE)
    resultant_normal = dot(hit_object->dir, ray_dir) < 0.0f ? hit_object->dir : hit_object->dir * -1.0f;

  return (fast_normalize(resultant_normal));
}


float3 path_trace(t_ray in_ray, U __global t_object *objects, unsigned int seed0, unsigned int seed1)
{
  U __global t_object *hit_object;
  t_ray out_ray;
  float3 hit_point;
  float3 normal;
  float3 brdf;
  float t;
  float3 accum_color = (float3)(0.0f, 0.0f, 0.0f);
 
  int n_bounces = 2;
  while (--n_bounces > 0)
  {
    if (!intersect_scene(in_ray, objects, &hit_object, &t))
    {
      return(accum_color);
    }
    hit_point = in_ray.pos + in_ray.dir * t;
    normal = get_normal(hit_object, hit_point, in_ray.dir);
    out_ray = generate_hemisphere_ray(hit_point, normal, &seed0, &seed1);
    brdf = BRDF(in_ray.dir, out_ray.dir, normal, hit_object);
    accum_color *=  brdf * max(0.0f, dot(in_ray.dir, normal));  
    in_ray = out_ray;
  }
  return (accum_color);
}


__kernel void	render_scene(U __global uchar *addr,
	U __global t_camera *camera, U __global t_object *objects)
{
U __global uchar	*dst;
  t_ray ray;
	float3			color;
	unsigned int					x;
	unsigned int					y;

	// x = get_global_id(0);
	// y = get_global_id(1);
  x = 1280 / 2;
  y = 720 / 2;

	ray = create_ray(camera, x, y);

  int n_samples = 10;
  float inv_samples = 1.0f / n_samples;

  printf("kernell %d\n", n_samples);
  while (--n_samples > 0)
  {
    color += path_trace(ray, objects, x, y) * inv_samples;
  }
	dst = addr + (y * camera->line_length + x * (camera->bytes_per_pixel));
  // printf("%f %f %f\n", color[0], color[1], color[2]);
  // color *= 0xFF;
  unsigned int result = 0;
  // printf("%f %f %f\n", color[0], color[1], color[2]);
  result += (unsigned int)(color[0] * 0xFF) % 0xFF << 16;
  result += (unsigned int)(color[1] * 0xFF) % 0xFF << 8;
  result += (unsigned int)(color[2] * 0xFF) % 0xFF;
  // printf("%f %f %f\n", camera->pos[0], camera->pos[1], camera->pos[2]);
  printf("%x %f %f %f\n", result, color[0], color[1], color[2]);
 	*(__global unsigned int *)dst = 0;
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
