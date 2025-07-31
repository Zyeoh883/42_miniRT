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
  uint condition;
  uint index;
  uint object_index;
 	float2			t;
  *closest_t = INFINITY;

  index = -1;
 	while (objects[++index].obj_type != 0)
 	{
 		t = ray_intersection(objects + index, ray);
    condition = (t[0] > 0 && t[0] < *closest_t);
    *closest_t = select(*closest_t, t[0], condition);
    object_index = select(object_index, index, condition);
    
    condition = (t[1] > 0 && t[1] < *closest_t);
    *closest_t = select(*closest_t, t[1], condition);
    object_index = select(object_index, index, condition);

 		// if (t[0] > 0 && t[0] < *closest_t)
 		// {
 		// 	*closest_t = t[0];
 		// 	*hit_object = *objects;
 		// }
 		// if (t[1] > 0 && t[1] < *closest_t)
 		// {
 		// 	*closest_t = t[1];
 		//    *hit_object = *objects;
 		// }
 		// objects++;
 	}
   if (*closest_t == INFINITY)
     return 0;
  *hit_object = objects[object_index];
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

  normal = select(normal, hit_point - hit_object->pos, (int3)(hit_object->obj_type == SPHERE) << 31);
  normal = select(normal, hit_object->dir, (int3)(hit_object->obj_type == PLANE) << 31);

  // if (hit_object->obj_type == SPHERE)
  //   normal = hit_point - hit_object->pos;
  // else if (hit_object->obj_type == PLANE)
  //   normal = hit_object->dir;

  normal = fast_normalize(normal);
	// return (dot(normal, ray_dir) < 0.0f ? normal : -normal);
  return (select(normal, -normal, (int3)(dot(normal, ray_dir) > 0) << 31));
}


t_candidate path_trace(t_ray in_ray, U __constant t_object *objects, t_sample_data sample_data, t_reservoir *reservoir)
{
  t_object hit_object;
  t_ray out_ray;
  float3 hit_point;
  float3 normal;
  float3 bxdf;
  float t;
  float2 seed;
  float pdf= 1;
  float pdf_temp = 1;
  // float3 accum_color = (float3)(0.0f, 0.0f, 0.0f);
  float3 throughput = (float3)(1.0f, 1.0f, 1.0f);

  t_candidate candidate;

  candidate = init_candidate();
 
  if (!intersect_scene(in_ray, objects, &hit_object, &t))
    return candidate;
  sample_data.n_bounce = 2;
  while (true)
  {
    // printf("%d\n", sample_data.n_bounce);
    if (!intersect_scene(in_ray, objects, &hit_object, &t))
    {
      // update_reservoir(reservoir, candidate, sample_data);
      // candidate.radiance = throughput * 0.5f   
      // candidate.radiance = throughput * (float3)(0.1f, 0.3f, 0.8f);
      candidate.pdf = pdf_temp;
      candidate.weight = throughput;
      candidate.incident_direction = -in_ray.dir;
      return candidate;
    }// if (t < 1e-2f)
    if (hit_object.obj_type == LIGHT)
    {
      candidate.radiance = hit_object.emission * throughput;
      candidate.incident_direction = -in_ray.dir;
      candidate.pdf = 1.0f;
      candidate.weight = throughput;
      // update_reservoir(reservoir, candidate, sample_data);
      return candidate;
    }
    //   return (throughput * 0.1f);
    hit_point = in_ray.pos + in_ray.dir * t;
    normal = get_normal(&hit_object, hit_point, in_ray.dir);
    seed.x = sample_random(sample_data, 1);
    seed.y = sample_random(sample_data, 2);
    bxdf = sample_bxdf(sample_random(sample_data, 3), seed, in_ray.dir, &out_ray.dir, normal, &hit_object, &pdf, sample_data);
    // if (pdf == -1)
    //   return candidate;
    if (pdf < 0.0f)
    { 
        // Return current surface emission or black
        candidate.radiance = hit_object.emission * throughput;
        candidate.incident_direction = -in_ray.dir;
        candidate.pdf = pdf;
        candidate.weight = throughput;
        return candidate;
    }
    pdf_temp *= pdf;

      // return(hit_object.emission * throughput);

    throughput *= bxdf / pdf;
    in_ray = out_ray;
    in_ray.pos = hit_point + normal * 0.001f;
    // update_reservoir(reservoir, candidate, sample_data);
    if (sample_data.n_bounce-- == 0)
      break;
    // sample_data.n_bounce--;
  }
  // printf("hit-----------------------\n");
  // candidate.radiance = throughput * 0.5f;
  return candidate;
}

inline float3 linear_to_gamma(float3 x)
{
  return sqrt(x);
}

float calculate_candidate_importance_weight(t_candidate candidate) {
    // Handle cases where PDF is zero to avoid division by zero / NaNs
    if (candidate.pdf <= 0.0f) {
        return 0.0f;
    }

    // The importance weight is typically the luminance of the radiance divided by its PDF
    return luma(candidate.radiance / candidate.pdf);
}

// Function to add a new candidate to the reservoir
void update_reservoir(t_reservoir* res, t_candidate new_candidate, t_sample_data sample_data) {
    // 1. Calculate the importance weight of the new candidate
    float candidate_importance_weight = calculate_candidate_importance_weight(new_candidate);

    // 2. Accumulate this weight into the total sum (W)
    res->weighted_sum += candidate_importance_weight;

    // 3. Increment the count of candidates seen (M)
    res->M++;

    float random_val = sample_random(sample_data, 999);

    if (random_val < (candidate_importance_weight / res->weighted_sum)) {
        res->candidate = new_candidate; // Replace the chosen candidate
    }
}

float3 reservoir_final_color(t_reservoir *res)
{
  // float3 color;

  if (res->M == 0 || res->weighted_sum <= 0.0f || res->candidate.pdf <= 0.0f)
        return (float3) (0,0,0); // No valid light found, pixel is black
  
  float effective_weight_ratio = res->weighted_sum / (float)res->M;

  float3 final_color = res->candidate.radiance * (effective_weight_ratio / res->candidate.pdf);
  if (isnan(final_color.x) || isnan(final_color.y) || isnan(final_color.z) ||
      isinf(final_color.x) || isinf(final_color.y) || isinf(final_color.z))
  {
      return (float3)(1.0f, 0.0f, 1.0f);  // Magenta for debug
  }
  
  return final_color;
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



  sample_data.sample_index = 1000;
  float inv_samples = 1.0f / sample_data.sample_index;


  // unsigned int seed = x + y * camera->line_length + HashUInt32(x);
  t_reservoir reservoir;
  t_candidate candidate;

  reservoir = init_reservoir();

  while (--sample_data.sample_index > 0)
  {
    // printf("test\n");sample_data.x, sample_data.y), objects, sample_data);
    // color += path_trace(ray, objects, sample_data);
    // color += path_trace(create_ray(camera, sample_data.x + 0.2, sample_data.y + 0.2), objects, sample_data);
    // color += path_trace(create_ray(camera, sample_data.x - 0.2, sample_data.y - 0.2), objects, sample_data);
    // color += path_trace(create_ray(camera, sample_data.x + 0.2, sample_data.y - 0.2), objects, sample_data);
    // color += path_trace(create_ray(camera, sample_data.x - 0.2, sample_data.y + 0.2), objects, sample_data);

    candidate = path_trace(create_ray(camera, sample_data.x, sample_data.y), objects, sample_data, &reservoir);
    // candidate = init_candidate();
    // candidate.weight = 0.5f;
    // candidate.pdf = 0.5f;
    // candidate.radiance = (float3)(0.8f, 0.0f, 0.0f);
    // color += candidate.radiance;
    update_reservoir(&reservoir, candidate, sample_data);
    // add_sample_to_reservoir(&reservoir, candidate, sample_data);
  }


	dst = addr + (sample_data.y * camera->line_length + sample_data.x * (camera->bytes_per_pixel));
  // printf("%f %f %f\n", color[0], color[1], color[2]);
  // if (reservoir.candidate.pdf <= 0.0f)
  //   printf("pdf = %f\n", reservoir.candidate.pdf);
  color = reservoir_final_color(&reservoir);

  // color *= inv_samples;
  color = color / (color + 1.0f);
  // if (color[0] > 0.1f && color[1] > 0.1f && color[2] > 0.1f)
  //   printf("%f %f %f\n", color[0], color[1], color[2]);
  color = linear_to_gamma(color);
  // printf("%f %f %f\n", color[0], color[1], color[2]);
  result += (unsigned int)(color.x * 0xFF) << 16;
  result += (unsigned int)(color.y * 0xFF) << 8;
  result += (unsigned int)(color.z * 0xFF);
  // if (result >= 0xFAFAFA)
  //     printf("%f %f %f\n", color[0], color[1], color[2]);
  // printf("%f %f %f\n", camera->pos[0], camera->pos[1], camera->pos[2]);
  // if (result > 0)
  //   printf("%x %f %f %f\n", result, color[0], color[1], color[2]);
  // printf("%x\n", result);
 	*(__global unsigned int *)dst = result;
}

