/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:38 by zyeoh             #+#    #+#             */
/*   Updated: 2024/08/22 20:32:42 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

unsigned int WangHash(unsigned int x)
{
    x = (x ^ 61) ^ (x >> 16);
    x = x + (x << 3);
    x = x ^ (x >> 4);
    x = x * 0x27d4eb2d;
    x = x ^ (x >> 15);
    return x;
}

float sample_random(t_sample_data sample_data, uint type)
{
    type += sample_data.n_bounce * 5; 
    uint seed = WangHash(sample_data.x);
    seed = WangHash(seed + WangHash(sample_data.y));
    seed = WangHash(seed + WangHash(sample_data.sample_index));
    seed = WangHash(seed + WangHash(type));
    return seed * 2.3283064365386963e-10f;
}


float luma(float3 rgb)
{
    return dot(rgb, (float3)(0.299f, 0.587f, 0.114f));
}

float3 reflect(float3 v, float3 n)
{
    return v - 2.0f * dot(v, n) * n;
}


float3 GGX_Sample(float2 s, float3 n, float alpha)
{
    float phi = TWO_PI * s.x;
    float cos_theta = 1.0f / sqrt(1.0 + (alpha * alpha * s.y) / (1.0 - s.y));
    float sin_theta = sqrt(max(0.0f, 1.0f - cos_theta * cos_theta));

    float3 axis = fabs(n.x) > 0.001f ? (float3)(0.0f, 1.0f, 0.0f) : (float3)(1.0f, 0.0f, 0.0f);
    float3 t = normalize(cross(axis, n));
    float3 b = cross(n, t);

    return normalize(b * cos(phi) * sin_theta + t * sin(phi) * sin_theta + n * cos_theta);
}

// TODO: reduce variable count
float3 sample_diffuse(float2 s, float3 *out, float3 n, float3 diffuse_albedo, float *pdf)
{

  float phi = TWO_PI * s.x;
  float sin_theta = sqrt(s.y);

  float cos_theta = sqrt(1.0f - s.y);

  *pdf = cos_theta * INV_PI;

  float3 dir = (float3)(cos(phi) * sin_theta, sin(phi) * sin_theta, cos_theta);

  /* create a local orthogonal coordinate frame centered at the hitpoint */
	float3 axis = fabs(n.x) > 0.1f ? (float3)(0.0f, 1.0f, 0.0f) : (float3)(1.0f, 0.0f, 0.0f);
	float3 u = normalize(cross(axis, n));
	float3 v = cross(n, u);

	/* use the coordinte frame and random numbers to compute the next ray direction */
	*out = fast_normalize(v * dir.x + u * dir.y + n * dir.z);

  return (diffuse_albedo * INV_PI);
}


float3 sample_specular(float2 s, float3 in, float3* out, float3 normal,t_object *hit_object, float* pdf)
{
  float n_dot_o;
  float n_dot_h;
  float n_dot_i;
  float h_dot_o;
  float3 wh;

    if (hit_object->roughness_sqr <= 1e-4f)
    {
        *out = reflect(in, normal);
        *pdf = 1.0;
        n_dot_o = dot(*out, normal);
        // Don't apply fresnel here, it's applied in the external function
        return to_float3(1.0f / n_dot_o);
    }
    else
    {
        wh = GGX_Sample(s, normal, hit_object->roughness_sqr);
        *out = reflect(in, wh);

        n_dot_o = dot(normal, *out);
        n_dot_h = dot(normal, wh);
        n_dot_i = dot(normal, -in);
        h_dot_o = dot(*out, wh);

        float D = NDF_GGX(hit_object->roughness_sqr, n_dot_h);
        // Don't apply fresnel here, it's applied in the external function
        //float3 F = FresnelSchlick(f0, h_dot_o);
        float G = V_SmithGGXCorrelated(n_dot_i, n_dot_o, hit_object->roughness_sqr);

        *pdf = D * n_dot_h / (4.0f * dot(wh, *out));
  // if (n_dot_h == 0)
  //   printf("here\n");

        return to_float3(D /* F */ * G);
    }
}

int check_checkerboard(float3 normal)
{
  float3 x_axis;
  float3 y_axis;
  float3 z_axis;

  // int3 bools;
  //
  // bools.x = (int)floor(normal.x * 2.);
  // bools.y = (int)floor(normal.y * 2);
  // bools.z = (int)floor(normal.z * 2);

  // bools.x = asin(fabs(cross(normal, (float3)(0, 1, 0))).x / fabs(normal).x);
  // bools.x = (1 - dot(normal, (float3)(0, 0, 1)));
  // bools.y = (int) 2 * (1 - dot(normal, (float3)(0, 1, 0)));
  // bools.x = atan(normal.z * 0.5f);
  //
  // if (bools.x < 0)
  //   bools.x += TWO_PI;
  // bools.x = fmod(1.0f, bools.x);

  // if ((bools.x + bools.y + bools.z) % 2 == 0)
  //   return 1;

  float theta;
  float phi;

  theta = acos(-normal.y);
  phi = atan2(-normal.z, normal.x) + M_PI;

  float u = phi * 0.5f * INV_PI;
  float v = theta * INV_PI;

  int tile_u = floor(u * 12.0f);
  int tile_v = floor(v * 12.0f);

  if ((tile_u + tile_v) % 2 == 0)
    return 1;

  return 0;
}

float3 sample_bxdf(float seed, float2 s, float3 in, float3 *out, float3 normal, t_object *hit_object, float *pdf)
{
  float3 freshnel;
  float specular_weight;
  float diffuse_weight;
  float inv_weight_sum;

  float specular_sampling_pdf;
  float diffuse_sampling_pdf;

  float3 bxdf;

  if (hit_object->obj_type == SPHERE && check_checkerboard(normal))
  {
    *pdf = -1;
    return (NULL);
  }

  in = fast_normalize(in);
  // *out = fast_normalize(*out);

  freshnel = freshnel_schlick(hit_object->F_0, dot(normal, -in)) * hit_object->specular_albedo;

  specular_weight = luma(hit_object->specular_albedo * freshnel);
  diffuse_weight = luma(hit_object->diffuse_albedo * (1 - freshnel));
  inv_weight_sum = 1 / (specular_weight + diffuse_weight);

  specular_sampling_pdf = specular_weight * inv_weight_sum;
  diffuse_sampling_pdf = diffuse_weight * inv_weight_sum;


  if (seed <= specular_sampling_pdf)
  {
    bxdf = freshnel * sample_specular(s, in, out, normal, hit_object, pdf);
    *pdf *= specular_sampling_pdf;
  }
  else
  {
    bxdf = (1 - freshnel) * sample_diffuse(s, out, normal, hit_object->diffuse_albedo, pdf);
    *pdf *= diffuse_sampling_pdf;
  }

  if (*pdf == 0)
    *pdf = 1e-5f;
  // if (*pdf == 0)
  //   printf("here\n");
  return bxdf * fmax(dot(*out, normal), 0.0f);
}
