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

float GetRandomFloat(unsigned int *seed) {
  *seed = (*seed ^ 61) ^ (*seed >> 16);
  *seed = *seed + (*seed << 3);
  *seed = *seed ^ (*seed >> 4);
  *seed = *seed * 0x27d4eb2d;
  *seed = *seed ^ (*seed >> 15);
  *seed = 1103515245 * (*seed) + 12345;

  return (float)(*seed) * 2.3283064365386963e-10f;
}

unsigned int HashUInt32(unsigned int x) {
  x = (x ^ 61) ^ (x >> 16);
  x = x + (x << 3);
  x = x ^ (x >> 4);
  x = x * 0x27d4eb2d;
  x = x ^ (x >> 15);
  return x;
}

// Linear congruential generator was here

t_ray create_ray(U __global t_camera *camera, t_sample_data *sample_data)
{
  t_ray ray;
  float i;
  float j;
  float offset;

  i = sample_data->x;
  j = sample_data->y;
  // offset = (sample_random(sample_data, i + j) - 0.5f) * 0.5f;
  i += (sample_random(sample_data, i) - 0.5f);
  j += (sample_random(sample_data, j) - 0.5f);


  // printf("offset: %f\n", offset);
  

  ray.pos = camera->pos;
  ray.dir =
      (float3)((2 * i / (float)camera->win_width - 1) * camera->pixel_width,
               (1 - 2 * j / (float)camera->win_height) * camera->pixel_height,
               1);
  // ray.dir = (float3)(camera->win_width / 2, camera->win_height / 2, 1);
  ray.dir = quat_rotate(camera->quat, ray.dir);
  ray.dir = fast_normalize(ray.dir);
  return (ray);
}




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

int intersect_scene(t_ray ray, U __global t_object *objects,
                    t_object *hit_object, float *closest_t) {
  uint condition;
  uint index;
  uint object_index;
  float2 t;
  *closest_t = INFINITY;

  index = -1;
  while (objects[++index].obj_type != 0) {
    t = ray_intersection(objects + index, ray);
    condition = (t[0] > 0 && t[0] < *closest_t);
    *closest_t = select(*closest_t, t[0], condition);
    object_index = select(object_index, index, condition);

    condition = (t[1] > 0 && t[1] < *closest_t);
    *closest_t = select(*closest_t, t[1], condition);
    object_index = select(object_index, index, condition);

  }
  if (*closest_t == INFINITY)
    return 0;
  *hit_object = objects[object_index];
  return 1;
}


float3 get_normal(t_object *hit_object, float3 hit_point, float3 ray_dir) {
  float3 normal;

  // normal = select(normal, hit_point - hit_object->pos,
  //                 (int3)(hit_object->obj_type == SPHERE) << 31);
  // normal = select(normal, hit_object->dir,
  //                 (int3)(hit_object->obj_type == PLANE) << 31);

  if (hit_object->obj_type == SPHERE)
    normal = hit_point - hit_object->pos;
  else if (hit_object->obj_type == PLANE)
    normal = hit_object->dir;

  normal = fast_normalize(normal);
  return (dot(normal, ray_dir) < 0.0f ? normal : -normal);
  // return (select(normal, -normal, (int3)(dot(normal, ray_dir) > 0) << 31));
}

// t_candidate ambient_trace(t_ray in_ray, U __global t_object *objects, t_sample_data *sample_data, t_reservoir *reservoir)
// {
//   t_object hit_object;
//   float2 t;
//   t_candidate candidate;
//
//   candidate = init_candidate();
//
//   if(intersect_scene(in_ray, objects, &hit_object, &t))
//   {
//     candidate.radiance = hit_object.emission; 
//   }
//   return candidate;
//
// }

t_candidate path_trace(t_ray in_ray, U __global t_object *objects,
                       t_sample_data *sample_data, t_reservoir *reservoir) {
  t_object hit_object;
  t_ray out_ray;
  float3 hit_point;
  float3 normal;
  float3 bxdf;
  float t;
  float2 seed;
  float pdf = 1.0f;
  float pdf_temp = 1;
  // float3 accum_color = (float3)(0.0f, 0.0f, 0.0f);
  float3 throughput = (float3)(1.0f, 1.0f, 1.0f);
  float3 k_ambient = (float3)(0.1f, 0.1f, 0.5f);
  t_candidate candidate;

  candidate = init_candidate();

  // if (!intersect_scene(in_ray, objects, &hit_object, &t))
  //   return candidate;
  sample_data->n_bounce = 5;
  while (true) {
    // printf("%d\n", sample_data.n_bounce);
    if (!intersect_scene(in_ray, objects, &hit_object, &t)) {
      // update_reservoir(reservoir, candidate, sample_data);
      
      // Mix between blue (up) and white (horizon)
      float t_val = 0.5f * (in_ray.dir.y + 1.0f);
      float3 top_color = (float3)(0.5f, 0.7f, 1.0f); // Sky
      float3 bot_color = (float3)(1.0f, 1.0f, 1.0f); // Horizon
      float3 env_color = mix(bot_color, top_color, t_val) * 0.1f; // 0.5f is intensity
      throughput = min(throughput, (float3)(0.1f));
      candidate.radiance = throughput * env_color;

      candidate.pdf = max(pdf_temp, 1e-6f);
      candidate.weight = 1;
      return candidate;
    } // if (t < 1e-2f)
    if (hit_object.obj_type == LIGHT) {
      // printf("%f, %f\n", hit_object.emission.x, throughput.x);
      candidate.radiance += hit_object.emission * throughput;

      if (isnan(candidate.radiance.x) || isnan(candidate.radiance.y) ||
          isnan(candidate.radiance.z) || isinf(candidate.radiance.x) ||
          isinf(candidate.radiance.y) || isinf(candidate.radiance.z)) {
        printf("emission: %f %f %f, throughput : %f %f %f\n",
               hit_object.emission.x, hit_object.emission.y,
               hit_object.emission.z, throughput.x, throughput.y, throughput.z);
        printf("radiance : %f %f %f\n", candidate.radiance.x,
               candidate.radiance.y, candidate.radiance.z);
      }
      candidate.pdf = pdf_temp;
      candidate.weight = throughput;
      return candidate;
    }
    //   return (throughput * 0.1f);
    hit_point = in_ray.pos + normalize(in_ray.dir) * t;
    normal = get_normal(&hit_object, hit_point, normalize(in_ray.dir));
    seed.x = sample_random(sample_data, 1);
    seed.y = sample_random(sample_data, 2);
    bxdf =
        sample_bxdf(sample_random(sample_data, 3), seed, normalize(in_ray.dir),
                    &out_ray.dir, normal, &hit_object, &pdf, sample_data);
    // candidate.radiance += hit_object.emission * throughput;
    // candidate.pdf = pdf_temp;
    // candidate.weight = throughput;
    // if (pdf == -1)
    //   return candidate;
    if (pdf <= 0.0f) {
      // printf("here\n");
      // Return current surface emission or black
      candidate.radiance = hit_object.emission * throughput;
      candidate.pdf = pdf_temp;
      candidate.weight = throughput;
      return candidate;
    }
    pdf_temp *= pdf;

    // return(hit_object.emission * throughput);
    throughput *= bxdf / max(pdf, 1e-5f);
    if (isnan(throughput.x) || isnan(throughput.y) || isnan(throughput.z) ||
        isinf(throughput.x) || isinf(throughput.y) || isinf(throughput.z)) {
      printf("throughput: %f %f %f\nbxdf: %f %f %f\npdf: %f\n", throughput.x,
             throughput.y, throughput.z, bxdf.x, bxdf.y, bxdf.z, pdf);
      break;
    }

    in_ray = out_ray;
    in_ray.pos = hit_point + normal * 0.001f;
    // update_reservoir(reservoir, candidate, sample_data);
    if (sample_data->n_bounce-- == 0)
      break;
    // sample_data.n_bounce--;
  }
  // printf("hit-----------------------\n");
  // candidate.radiance = throughput * 0.5f;
  // if (isnan(candidate.radiance.x) || isinf(candidate.radiance.x))
  //     printf("candidate.radiance, %f", candidate.radiance.x);
  return candidate;
}

inline float3 linear_to_gamma(float3 x) { return sqrt(x); }

float calculate_candidate_importance_weight(t_candidate candidate) {

  float lum;

  // Handle cases where PDF is zero to avoid division by zero / NaNs
  if (candidate.pdf <= 0.0f) {
    return 0.0f;
  }

  lum = luma(candidate.radiance); 
    // / max(candidate.pdf, 1e-10f);
  // if (isnan(candidate.radiance.x) || isinf(candidate.radiance.x))
  //   printf("candidate.radiance");

  // The importance weight is typically the luminance of the radiance divided by
  // its PDF
  return lum;
}

// Function to add a new candidate to the reservoir
void update_reservoir(t_reservoir *res, t_candidate new_candidate,
                      t_sample_data *sample_data) {
  // 1. Calculate the importance weight of the new candidate
  float candidate_importance_weight =
      calculate_candidate_importance_weight(new_candidate);

  // 2. Accumulate this weight into the total sum (W)
  res->weighted_sum += candidate_importance_weight;

  // 3. Increment the count of candidates seen (M)
  res->M++;

  float random_val = sample_random(sample_data, 999);

  if (random_val < (candidate_importance_weight / res->weighted_sum)) {
    // printf("here\n");
    res->candidate = new_candidate; // Replace the chosen candidate
  }
}

float3 reservoir_final_color(t_reservoir *res) {
  // float3 color;

  // if (isnan(res->M) || isinf(res->M))
  //   printf("Its M");
  // if (isnan(res->weighted_sum) || isinf(res->weighted_sum))
  //   printf("weighted_sum");
  if (isnan(res->candidate.pdf) || isinf(res->candidate.pdf))
    printf("its Pdf");

  if (res->M == 0 || res->weighted_sum <= 0.0f || res->candidate.pdf <= 0.0f)
    return (float3)(0, 0, 0); // No valid light found, pixel is black

  float avg_luminence = res->weighted_sum / (float)res->M;

  float selected_luminence = luma(res->candidate.radiance);

  float3 final_color = (res->candidate.radiance / selected_luminence) * avg_luminence;
  

  if (isnan(final_color.x) || isnan(final_color.y) || isnan(final_color.z) ||
      isinf(final_color.x) || isinf(final_color.y) || isinf(final_color.z)) {
    return (float3)(1.0f, 0.0f, 1.0f); // Magenta for debug
  }


  return final_color;
}

__kernel void render_scene(U __global uchar *addr,
                           U __global t_camera *camera,
                           U __global t_object *objects,
                           U __global t_reservoir *reservoirs) {
  U __global uchar *dst;
  t_ray ray;
  float3 color = (float3)(0.0f, 0.0f, 0.0f);
  t_sample_data sample_data;
  unsigned int result = 0;

  sample_data.x = get_global_id(0);
  sample_data.y = get_global_id(1);
  // x = 1280 / 2;
  // y = 720 / 2;
  

  // ray = create_ray(camera, sample_data.x, sample_data.y);

  // float inv_samples = 1.0f / sample_data.sample_index;

  t_reservoir reservoir;
  t_candidate candidate;

  reservoir = reservoirs[(int)(sample_data.x + sample_data.y * camera->win_width)];
  
  // if (sample_data.x == 0 && sample_data.y == 0)
  //   printf("seed = %f\n", reservoir.seed);

  // sample_data.seed = 0;
  sample_data.seed = reservoir.seed;
  sample_data.sample_index = 5;
  // if (sample_data.x == 0 && sample_data.y == 0)
  // {
  //
  //   int count = 25;
  //
  //   while (--count)
  //   {
  //     printf("test: %f\n", sample_random(&sample_data, 123));
  //   }
  // }

  if (camera->moved)
    reservoir = (t_reservoir){0};
  while (--sample_data.sample_index > 0) {

    // candidate = ambient_trace(create_ray(came))

    candidate = path_trace(create_ray(camera, &sample_data),
                           objects, &sample_data, &reservoir);
    // if (isnan(candidate.radiance.x) || isinf(candidate.radiance.x))
    //   printf("candidate.radiance, %f", candidate.radiance.x);
    update_reservoir(&reservoir, candidate, &sample_data);
  }

  reservoir.seed = sample_data.seed;
  reservoirs[(int)(sample_data.x + sample_data.y * camera->win_width)] =
      reservoir;
  // if (sample_data.x + sample_data.y == 0)
  // {
  //   printf("%u\n", sample_data.seed);
  // }

  dst = addr + (sample_data.y * camera->line_length +
                sample_data.x * (camera->bytes_per_pixel));
  // printf("%f %f %f\n", color[0], color[1], color[2]);
  // if (reservoir.candidate.pdf <= 0.0f)
  //   printf("pdf = %f\n", reservoir.candidate.pdf);
  color = reservoir_final_color(&reservoir);

  // color *= inv_samples;
  color = color / (color + 1);
  // if (color[0] > 0.1f && color[1] > 0.1f && color[2] > 0.1f)
  //   printf("%f %f %f\n", color[0], color[1], color[2]);
  color = linear_to_gamma(color);
  // printf("%f %f %f\n", color[0], color[1], color[2]);
  result += (unsigned int)(color.x * 0xFF) << 16;
  result += (unsigned int)(color.y * 0xFF) << 8;
  result += (unsigned int)(color.z * 0xFF);

  *(__global unsigned int *)dst = result;
}
