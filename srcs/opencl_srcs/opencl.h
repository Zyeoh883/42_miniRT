/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 15:30:40 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/09 19:23:54 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define U

// #define M_PI 3.141592653589793238f

#define SPHERE 'S'
#define PLANE 'P'
#define CYLINDER 'C'
#define LIGHT 'L'

#define INV_PI 1 / (float)M_PI
#define TWO_PI 2 * (float)M_PI

typedef struct __attribute__((aligned(16))) s_candidate
{
  float3 radiance;
  float3 weight;
  float pdf;

}       t_candidate;

typedef struct __attribute__((aligned(16))) s_reservoir
{
  t_candidate candidate;
  float weighted_sum;
  uint M;
  float seed;
}       t_reservoir;

typedef struct __attribute__((aligned(16))) s_ambient
{
  float amb_light_ratio;
  float3 amb_top_color;
  float3 amb_bot_color;
} t_ambient;

typedef struct __attribute__((aligned(16))) s_camera
{
	float3	pos;
	float4	quat;
	float	pixel_width;
	float	pixel_height;
	int		win_height;
	int		win_width;
  float   fov;
	int		bytes_per_pixel;
	int		line_length;
  
  t_ambient ambient;
  // float amb_light_ratio;
  // float amb_fade_ratio;
  // float3 amb_top_color;
  // float3 amb_bot_color;

  char  moved;
}				t_camera;

typedef struct __attribute__((aligned(16))) s_ray
{
	float3	pos;
	float3	dir;
}				t_ray;

typedef struct __attribute__((aligned(16))) s_sphere
{
	float	radius;
}				t_sphere;

typedef struct __attribute__ ((aligned(16))) s_cylinder
{
	float		radius;
	float		height;
}						t_cyclinder;

/*typedef struct __attribute__ ((aligned(16))) s_OBB*/
/*{*/
/*	float3		quat;*/
/*	float3		half_len;*/
/*}						t_OBB;*/

typedef struct __attribute__((aligned(16))) s_object
{
	/*struct s_OBB		obb;*/
  /*uchar   mat_type;*/
	uchar		obj_type;
	float3	pos;
	float3	dir;
  float3  diffuse_albedo;
  float3  specular_albedo;
  float3  F_0;
  float   roughness_sqr;
  float3   emission; 
	union
	{
		struct s_sphere	sphere;
    struct s_cylinder cyclinder;
	};
}				t_object;

typedef struct __attribute__((aligned(16))) s_sample_data
{
  uint x;
  uint y;
  uint sample_index;
  uint n_bounce;
  uint seed;
}       t_sample_data;

float3			vector_normalize(float3 v);
t_ray			create_ray(U __global t_camera *camera, t_sample_data *sample_data);
int				render_ray(t_ray ray, U __global t_object *objects);
float3			vector_scalar_product(const float3 v, const float scalar);
float2			intersect_ray_sphere(U __global t_object *object, t_ray ray);
float3			quat_rotate(float4 q, const float3 q_v);
float2			ray_intersection(U __global t_object *object, t_ray ray);


t_candidate init_candidate();
t_reservoir init_reservoir();

float3       BRDF(float3 in, float3 out, float3 normal, t_object *hit_object);


float sample_random(t_sample_data *sample_data, uint type);
float3 sample_bxdf(float seed, float2 s, float3 in, float3 *out, float3 normal, t_object *hit_object, float *pdf, t_sample_data *sample_data);
float luma(float3 rgb);


void update_reservoir(t_reservoir* res, t_candidate new_candidate, t_sample_data *sample_data);
float3 reservoir_final_color(t_reservoir *res);

// utils

float3 to_float3(float x);
void print_vec(float3 vec);
