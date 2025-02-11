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

t_ray	create_ray(U __constant t_camera *camera, int i, int j)
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

 int intersect_scene(t_ray ray, U __constant t_object *objects, U __constant t_object **hit_object, float *closest_t)
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


t_ray sample_hemisphere(float3 hit_point, float3 normal, unsigned int *seed0)
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

// t_ray sample_reflection(float3 hit_point, float3 normal, float3 view_dir, float roughness, unsigned int* seed0)
// {
//     float2 rand = (float2)(GetRandomFloat(seed0), GetRandomFloat(seed0));
//     float3 half_dir = GGX_Sample(rand, normal, roughness);
//     float3 reflect_dir = reflect(-view_dir, half_dir);
//     return (t_ray){hit_point + normal * 1e-5f, reflect_dir};
// }

float3 get_normal(U __constant t_object *hit_object, float3 hit_point, float3 ray_dir)
{
  float3 normal;

  if (hit_object->obj_type == SPHERE)
    normal = hit_point - hit_object->pos;
  else if (hit_object->obj_type == PLANE)
    normal = hit_object->dir;

  normal = fast_normalize(normal);
	return (dot(normal, ray_dir) < 0.0f ? normal : -normal);
}


float3 path_trace(t_ray in_ray, U __constant t_object *objects, unsigned int *seed0)
{
  U __constant t_object *hit_object;
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
    out_ray = sample_hemisphere(hit_point, normal, seed0);
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
	U __constant t_camera *camera, U __constant t_object *objects)
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
  // printf("%f %f %f\n", color.x, color[1], color[2]);
	dst = addr + (y * camera->line_length + x * (camera->bytes_per_pixel));
  // printf("%f %f %f\n", color[0], color[1], color[2]);
  // color *= 0xFF;
  unsigned int result = 0;
  color = color / (color + 1.0f); 
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

