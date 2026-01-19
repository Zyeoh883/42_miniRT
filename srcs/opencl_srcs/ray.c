/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 16:33:56 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/19 19:27:31 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// uint HashUInt32(uint x) {
//   x = (x ^ 61) ^ (x >> 16);
//   x = x + (x << 3);
//   x = x ^ (x >> 4);
//   x = x * 0x27d4eb2d;
//   x = x ^ (x >> 15);
//   return (x);
// }

// Linear congruential generator was here

t_ray	create_ray(U __global t_camera *camera, t_sample_data *sample_data)
{
	t_ray	ray;
	float	i;
	float	j;
	float	offset;

	i = sample_data->x;
	j = sample_data->y;
	i += (sample_random(sample_data) - 0.5f);
	j += (sample_random(sample_data) - 0.5f);
	ray.pos = camera->pos;
	ray.dir = (float3)((2 * i / (float)camera->win_width - 1)
			* camera->pixel_width, (1 - 2 * j / (float)camera->win_height)
			* camera->pixel_height, 1);
	ray.dir = quat_rotate(camera->quat, ray.dir);
	ray.dir = fast_normalize(ray.dir);
	return (ray);
}

// return (dot(x.normal, ray_dir) < 0.0f ? x.normal : -x.normal);
float3	get_normal(t_object *hit_object, float3 hit_point, float3 ray_dir)
{
	t_vars_get_normal	x;

	if (hit_object->obj_type == SPHERE)
		x.normal = hit_point - hit_object->pos;
	else if (hit_object->obj_type == PLANE)
		x.normal = hit_object->dir;
	else if (hit_object->obj_type == CYLINDER)
	{
		x.center = hit_object->pos;
		x.axis = hit_object->dir;
		x.height = hit_object->cyclinder.height;
		x.dist_along_axis = dot(hit_point - x.center, x.axis);
		if (fabs(x.dist_along_axis - x.height / 2.0f) < 0.001f)
			x.normal = x.axis;
		else if (fabs(x.dist_along_axis + x.height / 2.0f) < 0.001f)
			x.normal = -x.axis;
		else
		{
			x.to_hit = hit_point - x.center;
			x.proj_on_axis = dot(x.to_hit, x.axis);
			x.normal = x.to_hit - x.proj_on_axis * x.axis;
		}
	}
	x.normal = fast_normalize(x.normal);
	return (select(-x.normal, x.normal, (float3)dot(x.normal, ray_dir) < 0.0f));
}

// t_candidate ambient_trace(t_ray in_ray, U __global t_object *objects,
// t_sample_data *sample_data, t_reservoir *reservoir)
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
//   return (candidate);
//
// }

// path_trace if checks:

// if (isnan(x.candidate.radiance.x) || isnan(x.candidate.radiance.y) ||
//     isnan(x.candidate.radiance.z) || isinf(x.candidate.radiance.x) ||
//     isinf(x.candidate.radiance.y) || isinf(x.candidate.radiance.z)) {
//   printf("emission: %f %f %f, x.throughput : %f %f %f\n",
//          hit_object.emission.x, hit_object.emission.y,
//          hit_object.emission.z, x.throughput.x, x.throughput.y,
// x.throughput.z);
//   printf("radiance : %f %f %f\n", x.candidate.radiance.x,
//          x.candidate.radiance.y, x.candidate.radiance.z);
// }

t_vars_render_scene	init_vars_render_scene(U __global t_reservoir *reservoirs)
{
	t_vars_render_scene	x;

	x.sample_data.x = get_global_id(0);
	x.sample_data.y = get_global_id(1);
	x.sample_data.sample_index = SAMPLES_PER_RUN;
	x.sample_data.n_bounce = BOUNCE;
	return (x);
}

KERNEL	void	render_scene(U __global uchar *addr,
		U __global t_camera *camera,
		U __global t_object *objects,
		U __global t_reservoir *reservoirs)
{
	t_vars_render_scene	x;

	x = init_vars_render_scene(reservoirs);
	x.reservoir = reservoirs[(int)(x.sample_data.x + x.sample_data.y
			* camera->win_width)];
	if (camera->moved)
		x.reservoir = init_reservoir();
	x.sample_data.seed = x.reservoir.seed;
	while (x.sample_data.sample_index-- > 0)
	{
		x.candidate = path_trace(camera, objects, &x.sample_data);
		update_reservoir(&x.reservoir, x.candidate, &x.sample_data);
	}
	x.reservoir.seed = x.sample_data.seed;
	reservoirs[(int)(x.sample_data.x + x.sample_data.y
			* camera->win_width)] = x.reservoir;
	x.dst = addr + (x.sample_data.y * camera->line_length + x.sample_data.x
			* (camera->bytes_per_pixel));
	x.color = reservoir_final_color(&x.reservoir);
	x.color = linear_to_gamma(x.color / (x.color + 1));
	*(__global unsigned int *)x.dst = (unsigned int)(x.color.x * 0xFF) << 16;
	*(__global unsigned int *)x.dst += (unsigned int)(x.color.y * 0xFF) << 8;
	*(__global unsigned int *)x.dst += (unsigned int)(x.color.z * 0xFF);
}
