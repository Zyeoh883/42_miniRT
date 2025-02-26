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
    x = (x ^ 61) ^ (x >> 16);
    x = x + (x << 3);
    x = x ^ (x >> 4);
    x = x * 0x27d4eb2d;
    x = x ^ (x >> 15);
    return x;
}


// Linear congruential generator was here

t_ray	create_ray(U __constant t_camera *camera, float i, float j)
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

 int intersect_scene(t_ray ray, U __constant t_object *objects, t_object *hit_object, float *closest_t)
 {
 	float2			t;
  *closest_t = INFINITY;

 	while (objects->obj_type != 0)
 	{
 		t = ray_intersection(objects, ray);
 		if (t[0] > 0 && t[0] < *closest_t)
 		{
 			*closest_t = t[0];
 			*hit_object = *objects;
 		}
 		if (t[1] > 0 && t[1] < *closest_t)
 		{
 			*closest_t = t[1];
      *hit_object = *objects;
 		}
 		objects++;
 	}
   if (*closest_t == INFINITY)
     return 0;
   return 1;
 }


// t_ray sample_hemisphere(float3 hit_point, float3 normal, unsigned int *seed0)
// {
//   t_ray new_ray;
//
//   new_ray.pos = hit_point + normal * 0.0001f;
//
//
//   /* compute two random numbers to pick a random point on the hemisphere above the hitpoint*/
// 	float rand1 = 2.0f * M_PI * GetRandomFloat(seed0);
// 	float rand2 = GetRandomFloat(seed0);
// 	float rand2s = sqrt(rand2);
//
//   /* create a local orthogonal coordinate frame centered at the hitpoint */
// 	float3 w = normal;
// 	float3 axis = fabs(w.x) > 0.1f ? (float3)(0.0f, 1.0f, 0.0f) : (float3)(1.0f, 0.0f, 0.0f);
// 	float3 u = normalize(cross(axis, w));
// 	float3 v = cross(w, u);
//
// 	/* use the coordinte frame and random numbers to compute the next ray direction */
// 	new_ray.dir = fast_normalize(u * cos(rand1)*rand2s + v*sin(rand1)*rand2s + w*sqrt(1.0f - rand2));
//
//   return (new_ray);
// }

// t_ray sample_reflection(float3 hit_point, float3 normal, float3 view_dir, float roughness, unsigned int* seed0)
// {
//     float2 rand = (float2)(GetRandomFloat(seed0), GetRandomFloat(seed0));
//     float3 half_dir = GGX_Sample(rand, normal, roughness);
//     float3 reflect_dir = reflect(-view_dir, half_dir);
//     return (t_ray){hit_point + normal * 1e-5f, reflect_dir};
// }

float3 get_normal(t_object *hit_object, float3 hit_point, float3 ray_dir)
{
  float3 normal;

  if (hit_object->obj_type == SPHERE)
    normal = hit_point - hit_object->pos;
  else if (hit_object->obj_type == PLANE)
    normal = hit_object->dir;

  normal = fast_normalize(normal);
	return (dot(normal, ray_dir) < 0.0f ? normal : -normal);
}


float3 path_trace(t_ray in_ray, U __constant t_object *objects, t_sample_data sample_data)
{
  t_object hit_object;
  t_ray out_ray;
  float3 hit_point;
  float3 normal;
  float3 bxdf;
  float t;
  float2 seed;
  float pdf;
  // float3 accum_color = (float3)(0.0f, 0.0f, 0.0f);
  float3 mask = (float3)(1.0f, 1.0f, 1.0f);
 
  if (!intersect_scene(in_ray, objects, &hit_object, &t))
    return (to_float3(0));
  sample_data.n_bounce = 3;
  while (sample_data.n_bounce-- > 0)
  {
    if (!intersect_scene(in_ray, objects, &hit_object, &t))
      return(mask * 0.5f); 
    hit_point = in_ray.pos + in_ray.dir * t;
    normal = get_normal(&hit_object, hit_point, in_ray.dir);
    seed.x = sample_random(sample_data, 1);
    seed.y = sample_random(sample_data, 2);

    // out_ray = sample_hemisphere(hit_point, normal, seed0);
    // brdf = BRDF(in_ray.dir, out_ray.dir, normal, &hit_object);
  
    bxdf = sample_bxdf(sample_random(sample_data, 3), seed, in_ray.dir, &out_ray.dir, normal, &hit_object, &pdf);
    // printf("brdf : %f %f %f\n", brdf[0], brdf[1], brdf[2]);
    // accum_color +=  brdf * max(0.0f, dot(in_ray.dir, normal)) * hit_object->emission * hit_object->color;  
    // mask *= brdf * fmax(dot(fast_normalize(out_ray.dir), normal), 0.0f);
      // return (accum_color + hit_object->emission);
    // accum_color += mask * hit_object.emission;
    if (pdf == -1)
      return 0;
    if (hit_object.obj_type == LIGHT)
      return(hit_object.emission * mask);
    mask *= bxdf / pdf;
    in_ray = out_ray;
    in_ray.pos = hit_point + normal * 0.001f;
  }
  // printf("hit-----------------------\n");
  return (mask * 0.5f);
}


__kernel void	render_scene(U __global uchar *addr,
	U __constant t_camera *camera, U __constant t_object *objects)
{
U __global uchar	*dst;
  t_ray ray;
	float3			color = (float3)(0.0f, 0.0f, 0.0f);
	t_sample_data sample_data;
  unsigned int result = 0;

	sample_data.x = get_global_id(0);
	sample_data.y = get_global_id(1);
  // x = 1280 / 2;
  // y = 720 / 2;

	ray = create_ray(camera, sample_data.x, sample_data.y);



  sample_data.sample_index = 100;
  float inv_samples = 1.0f / sample_data.sample_index;


  // unsigned int seed = x + y * camera->line_length + HashUInt32(x);

  while (--sample_data.sample_index > 0)
  {
    // printf("test\n");sample_data.x, sample_data.y), objects, sample_data);
    // color += path_trace(ray, objects, sample_data);
    color += path_trace(create_ray(camera, sample_data.x, sample_data.y), objects, sample_data);
    // color += path_trace(create_ray(camera, sample_data.x + 0.2, sample_data.y + 0.2), objects, sample_data);
    // color += path_trace(create_ray(camera, sample_data.x - 0.2, sample_data.y - 0.2), objects, sample_data);
    // color += path_trace(create_ray(camera, sample_data.x + 0.2, sample_data.y - 0.2), objects, sample_data);
    // color += path_trace(create_ray(camera, sample_data.x - 0.2, sample_data.y + 0.2), objects, sample_data);
  }

  color *= inv_samples;

	dst = addr + (sample_data.y * camera->line_length + sample_data.x * (camera->bytes_per_pixel));
  // printf("%f %f %f\n", color[0], color[1], color[2]);
  // color *= 0xFF;
  color = color / (color + 1.0f);
  // if (color[0] > 0.1f && color[1] > 0.1f && color[2] > 0.1f)
  //   printf("%f %f %f\n", color[0], color[1], color[2]);
  // printf("%f %f %f\n", color[0], color[1], color[2]);
  result += (unsigned int)(fmin(color.x, 1.0f) * 0xFF) << 16;
  result += (unsigned int)(fmin(color.y, 1.0f) * 0xFF) << 8;
  result += (unsigned int)(fmin(color.z, 1.0f) * 0xFF);
  // if (result >= 0xFAFAFA)
  //     printf("%f %f %f\n", color[0], color[1], color[2]);
  // printf("%f %f %f\n", camera->pos[0], camera->pos[1], camera->pos[2]);
  // if (result > 0)
  //   printf("%x %f %f %f\n", result, color[0], color[1], color[2]);
  // printf("%x\n", result);
 	*(__global unsigned int *)dst = result;
}

