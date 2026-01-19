/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_trace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 19:17:43 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/19 19:18:55 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	intersect_scene(t_ray ray, U __global t_object *objects,
		t_object *hit_object, float *closest_t)
{
	uint	condition;
	uint	index;
	uint	object_index;
	float2	t;

	*closest_t = INFINITY;
	object_index = 0;
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
	}
	if (*closest_t == INFINITY)
		return (0);
	*hit_object = objects[object_index];
	return (1);
}

t_candidate	hit_ambient(__global t_camera *camera, t_vars_path_trace x)
{
	x.t_val = 0.5f * (x.in_ray.dir.y + 1.0f);
	x.env_color = mix(camera->ambient.amb_bot_color,
			camera->ambient.amb_top_color, x.t_val);
	x.throughput *= min(x.throughput, (float3)(0.1f));
	x.candidate.radiance = x.throughput * x.env_color
		* camera->ambient.amb_light_ratio;
	x.candidate.pdf = max(x.pdf_temp, 1e-6f);
	x.candidate.weight = x.throughput;
	return (x.candidate);
}

t_candidate	hit_light(t_vars_path_trace x)
{
	x.candidate.radiance += x.hit_object.emission * x.throughput
		* x.hit_object.diffuse_albedo;
	x.candidate.pdf = x.pdf_temp;
	x.candidate.weight = x.throughput;
	return (x.candidate);
}

t_vars_path_trace	hit_object(t_vars_path_trace x, t_sample_data *sample_data)
{
	x.hit_point = x.in_ray.pos + normalize(x.in_ray.dir) * x.t;
	x.normal = get_normal(&x.hit_object, x.hit_point, normalize(x.in_ray.dir));
	x.seed = (float2){sample_random(sample_data), sample_random(sample_data)};
	x.bxdf = sample_bxdf((t_args_bxdf){x.in_ray.dir, &x.out_ray.dir, x.normal,
			&x.hit_object, &x.pdf, x.seed});
	x.pdf_temp *= x.pdf;
	x.cos_theta = fabs(dot(x.out_ray.dir, x.normal));
	x.throughput *= x.bxdf * x.cos_theta / max(x.pdf, 1e-5f);
	x.in_ray = x.out_ray;
	x.in_ray.pos = x.hit_point + x.normal * 0.001f;
	return (x);
}

t_candidate	path_trace(__global t_camera *camera, U __global t_object *objects,
		t_sample_data *sample_data)
{
	t_vars_path_trace	x;

	x.pdf = 1.0f;
	x.pdf_temp = 1;
	x.throughput = (float3)(1.0f, 1.0f, 1.0f);
	x.candidate = init_candidate();
	x.in_ray = create_ray(camera, sample_data);
	while (true)
	{
		if (!intersect_scene(x.in_ray, objects, &x.hit_object, &x.t))
			return (hit_ambient(camera, x));
		if (x.hit_object.obj_type == LIGHT)
			return (hit_light(x));
		x = hit_object(x, sample_data);
		if (x.pdf <= 0.0f)
		{
			x.candidate.radiance = x.hit_object.emission * x.throughput;
			x.candidate.pdf = x.pdf_temp;
			x.candidate.weight = x.throughput;
			return (x.candidate);
		}
		if (sample_data->n_bounce-- == 0)
			break ;
	}
	return (x.candidate);
}
