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



float GetRandomFloat(unsigned int* seed)
{
    *seed = (*seed ^ 61) ^ (*seed >> 16);
    *seed = *seed + (*seed << 3);
    *seed = *seed ^ (*seed >> 4);
    *seed = *seed * 0x27d4eb2d;
    *seed = *seed ^ (*seed >> 15);
    *seed = 1103515245 * (*seed) + 12345;

    return (float)(*seed) * 2.3283064365386963e-10f;
}

unsigned int HashUInt32(unsigned int x)
{
#if 0
    x = (x ^ 61) ^ (x >> 16);
    x = x + (x << 3);
    x = x ^ (x >> 4);
    x = x * 0x27d4eb2d;
    x = x ^ (x >> 15);
    return x;
#else
    return 1103515245 * x + 12345;
#endif
}


// Linear congruential generator was here

t_ray	create_ray(U __global t_camera *camera, int i, int j)
{
	t_ray	ray;

	ray.pos = camera->pos;
	ray.dir = (float3)((2 * i / (float)camera->win_width - 1)
			* camera->pixel_width, (1 - 2 * j / (float)camera->win_height)
			* camera->pixel_height, 1);
  // ray.dir = (float3)(camera->win_width / 2, camera->win_height / 2, 1);
	ray.dir = quat_rotate(camera->quat, ray.dir);
  ray.dir = fast_normalize(ray.dir);
	return (ray);
}

 int intersect_scene(t_ray ray, U __global t_object *objects, U __global t_object **hit_object, float *closest_t)
 {
 	float2			t;
  *closest_t = INFINITY;

 	while (objects->obj_type != 0)
 	{
 		t = ray_intersection(objects, ray);
 		if (t[0] > 0 && t[0] < *closest_t)
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


t_ray generate_hemisphere_ray(float3 hit_point, float3 normal, unsigned int *seed0)
{
  t_ray new_ray;

  new_ray.pos = hit_point + normal * 0.0001f;


  /* compute two random numbers to pick a random point on the hemisphere above the hitpoint*/
	float rand1 = 2.0f * M_PI * GetRandomFloat(seed0);
	float rand2 = GetRandomFloat(seed0);
	float rand2s = sqrt(rand2);

  /* create a local orthogonal coordinate frame centered at the hitpoint */
	float3 w = normal;
	float3 axis = fabs(w.x) > 0.1f ? (float3)(0.0f, 1.0f, 0.0f) : (float3)(1.0f, 0.0f, 0.0f);
	float3 u = normalize(cross(axis, w));
	float3 v = cross(w, u);

	/* use the coordinte frame and random numbers to compute the next ray direction */
	new_ray.dir = fast_normalize(u * cos(rand1)*rand2s + v*sin(rand1)*rand2s + w*sqrt(1.0f - rand2));

  return (new_ray);
}


float3 get_normal(U __global t_object *hit_object, float3 hit_point, float3 ray_dir)
{
  float3 normal;

  if (hit_object->obj_type == SPHERE)
    normal = hit_point - hit_object->pos;
  else if (hit_object->obj_type == PLANE)
    normal = hit_object->dir;

  normal = fast_normalize(normal);
	return (dot(normal, ray_dir) < 0.0f ? normal : normal * (-1.0f));
}


float3 path_trace(t_ray in_ray, U __global t_object *objects, unsigned int *seed0)
{
  U __global t_object *hit_object;
  t_ray out_ray;
  float3 hit_point;
  float3 normal;
  float3 brdf;
  float t;
  float3 accum_color = (float3)(0.0f, 0.0f, 0.0f);
  float3 mask = (float3)(1.0f, 1.0f, 1.0f);
 
  int n_bounces = 2;
  while (n_bounces-- > 0)
  {
    if (!intersect_scene(in_ray, objects, &hit_object, &t))
    {
      return(accum_color);
    }
    hit_point = in_ray.pos + in_ray.dir * t;
    normal = get_normal(hit_object, hit_point, in_ray.dir);
    out_ray = generate_hemisphere_ray(hit_point, normal, seed0);
    brdf = BRDF(in_ray.dir, out_ray.dir, normal, hit_object);
    // printf("brdf : %f %f %f\n", brdf[0], brdf[1], brdf[2]);
    // accum_color +=  brdf * max(0.0f, dot(in_ray.dir, normal)) * hit_object->emission * hit_object->color;  
    // mask *= brdf * fmax(dot(fast_normalize(out_ray.dir), normal), 0.0f);
      // return (accum_color + hit_object->emission);
    accum_color += mask * hit_object->emission;
    if (hit_object->obj_type == LIGHT)
      return(accum_color);
    mask *= brdf * max(dot(fast_normalize(out_ray.dir), normal), 0.0f);
    in_ray = out_ray;
  }
  // printf("hit-----------------------\n");
  return (accum_color);
}


__kernel void	render_scene(U __global uchar *addr,
	U __global t_camera *camera, U __global t_object *objects)
{
U __global uchar	*dst;
  t_ray ray;
	float3			color = (float3)(0.0f, 0.0f, 0.0f);
	float3			temp = (float3)(0.0f, 0.0f, 0.0f);
	unsigned int					x;
	unsigned int					y;

	x = get_global_id(0);
	y = get_global_id(1);
  // x = 1280 / 2;
  // y = 720 / 2;

	ray = create_ray(camera, x, y);

  int n_samples = 1000;
  float inv_samples = 1.0f / n_samples;


  unsigned int seed = x + y * camera->line_length + HashUInt32(x);

  while (--n_samples > 0)
  {
    // printf("%u\n", seed);
    temp = path_trace(ray, objects, &seed) * inv_samples;

    color += temp;
  }
  // if (count != 999)
    // printf("%d\n", count);
  // printf("%f %f %f\n", color.x, color[1], color[2]);
	dst = addr + (y * camera->line_length + x * (camera->bytes_per_pixel));
  // printf("%f %f %f\n", color[0], color[1], color[2]);
  // color *= 0xFF;
  unsigned int result = 0;
  // printf("%f %f %f\n", color[0], color[1], color[2]);
  result += (unsigned int)(min(color[0], 1.0f) * 0xFF) << 16;
  result += (unsigned int)(min(color[1], 1.0f) * 0xFF) << 8;
  result += (unsigned int)(min(color[2], 1.0f) * 0xFF);
  // if (result >= 0xFAFAFA)
  //     printf("%f %f %f\n", color[0], color[1], color[2]);
  // printf("%f %f %f\n", camera->pos[0], camera->pos[1], camera->pos[2]);
  // if (result > 0)
  //   printf("%x %f %f %f\n", result, color[0], color[1], color[2]);
  // printf("%x\n", result);
 	*(__global unsigned int *)dst = result;
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
