/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_object_intercept.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:13:56 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/17 15:42:49 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

float2	intersect_ray_sphere(U __global t_object *object, t_ray ray);
float2	intersect_ray_plane(U __global t_object *object, t_ray ray);
float2	intersect_ray_cylinder(U __global t_object *object, t_ray ray);
// bool rayAABBIntersect(float3 ray_ori, float3 ray_dir, float3 aabbMin,
// float3 aabbMax);

// if (!rayAABBIntersect(ray.pos, ray.dir, object->pos
// - object->obb.half_len, object->pos + object->obb.half_len))
//   return ((float2)(INFINITY, INFINITY));
float2	ray_intersection(U __global t_object *object, t_ray ray)
{
	if (object->obj_type == SPHERE || object->obj_type == LIGHT)
		return (intersect_ray_sphere(object, ray));
	else if (object->obj_type == PLANE)
		return (intersect_ray_plane(object, ray));
	else if (object->obj_type == CYLINDER)
		return (intersect_ray_cylinder(object, ray));
	return ((float2)(INFINITY, INFINITY));
}

float2	intersect_ray_sphere(U __global t_object *object, t_ray ray)
{
	float3	sphere_to_camera;
	float	a;
	float	b;
	float	c;
	float	discriminant;

	sphere_to_camera = ray.pos - object->pos;
	a = dot(ray.dir, ray.dir);
	b = 2 * dot(sphere_to_camera, ray.dir);
	c = dot(sphere_to_camera, sphere_to_camera) - (object->sphere.radius
			* object->sphere.radius);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return ((float2)(INFINITY, INFINITY));
	discriminant = sqrt(discriminant);
	return ((float2)((-b + discriminant) / (2 * a),
		(-b - discriminant) / (2 * a)));
}

float2	intersect_ray_plane(U __global t_object *object, t_ray ray)
{
	float	t;
	float	denom;

	denom = dot(object->dir, ray.dir);
	t = dot(object->pos - ray.pos, object->dir) / denom;
	if (t >= 0)
		return ((float2)(t, INFINITY));
	return ((float2)(INFINITY, INFINITY));
}

// Check cylinder body intersections
// t_cylinder = INFINITY;
// if (t[0] > 0.0f)
// {
// 	hit_point = ray.pos + ray.dir * t[0];
// 	dist_along_axis = dot(hit_point - center, axis);
// 	if (fabs(dist_along_axis) <= height * 0.5f)
// 		t_cylinder = t[0];
// }
// if (t[1] > 0.0f && (t_cylinder == INFINITY || t[1] < t_cylinder))
// {
// 	hit_point = ray.pos + ray.dir * t[1];
// 	dist_along_axis = dot(hit_point - center, axis);
// 	if (fabs(dist_along_axis) <= height * 0.5f)
// 		t_cylinder = t[1];
// }

float2	intersect_ray_cyclinder_cap(t_vars_cyclinder x, t_ray ray)
{
	if (!(fabs(dot(ray.pos + ray.dir * x.t[0] - x.center, x.axis)) <= x.height
			* 0.5f))
		x.t[0] = INFINITY;
	if (!(fabs(dot(ray.pos + ray.dir * x.t[1] - x.center, x.axis)) <= x.height
			* 0.5f))
		x.t[1] = INFINITY;
	x.denom = dot(x.axis, ray.dir);
	if (fabs(x.denom) > 0.0f)
	{
		x.temp = dot(x.bottom_center - ray.pos, x.axis) / x.denom;
		if (x.temp > 0.0f)
			if (distance(ray.pos + ray.dir * x.temp,
					x.bottom_center) <= x.radius && x.temp < x.t[0])
				x.t[0] = x.temp;
		x.temp = dot(x.top_center - ray.pos, x.axis) / x.denom;
		if (x.temp > 0.0f)
			if (distance(ray.pos + ray.dir * x.temp, x.top_center) <= x.radius
				&& x.temp < x.t[1])
				x.t[1] = x.temp;
	}
	return (x.t);
}

float2	intersect_ray_cylinder(U __global t_object *object, t_ray ray)
{
	t_vars_cyclinder	x;

	x.t = (float2)(INFINITY, INFINITY);
	x.center = object->pos;
	x.axis = normalize(object->dir);
	x.radius = object->cyclinder.radius;
	x.height = object->cyclinder.height;
	x.bottom_center = x.center - x.axis * (x.height * 0.5f);
	x.top_center = x.center + x.axis * (x.height * 0.5f);
	ray.dir = normalize(ray.dir);
	x.oc = ray.pos - x.center;
	x.proj_oc_axis = dot(x.oc, x.axis);
	x.proj_dir_axis = dot(ray.dir, x.axis);
	x.a = dot(ray.dir, ray.dir) - x.proj_dir_axis * x.proj_dir_axis;
	x.b = 2.0f * (dot(x.oc, ray.dir) - x.proj_oc_axis * x.proj_dir_axis);
	x.c = dot(x.oc, x.oc) - x.proj_oc_axis * x.proj_oc_axis - x.radius
		* x.radius;
	x.discriminant = x.b * x.b - 4.0f * x.a * x.c;
	if (x.discriminant < 0.0f)
		return (x.t);
	x.discriminant = sqrt(x.discriminant);
	x.t[0] = (-x.b - x.discriminant) / (2.0f * x.a);
	x.t[1] = (-x.b + x.discriminant) / (2.0f * x.a);
	x.t = intersect_ray_cyclinder_cap(x, ray);
	return (x.t);
}

// bool rayAABBIntersect(float3 ray_ori, float3 ray.dir, float3 aabbMin,
// float3 aabbMax)
// {
//     float3 tMin;
//     float3 tMax;
//     float3 t0;
//     float3 t[0];
//     float tenter;
//     float texit;
//
//     tMin = (aabbMin - ray_ori) / ray_dir;
//     tMax = (aabbMax - ray_ori) / ray_dir;
//     t0 = fmin(tMin, tMax);
//     t[0] = fmax(tMin, tMax);
//     tenter = fmax(fmax(t0.x, t0.y), t0.z);
//     texit = fmin(fmin(t[0].x, t[0].y), t[0].z);
//
//     // if (tenter > texit || texit < 0.0)
//     //     return (false);
//
//     return (!(tenter > texit || texit < 0.0));
// }

// float2	ray_intersection(t_object sphere, t_ray ray)
// {
// 	float4		sphere_to_camera;
// 	float		a;
// 	float		b;
// 	float		c;
// 	float		discriminant;

// 	sphere_to_camera = vector_subtraction(ray.pos, sphere.pos);
// 	a = vector_dot_product(ray.dir, ray.dir);
// 	b = 2 * vector_dot_product(sphere_to_camera, ray.dir);
// 	c = vector_dot_product(sphere_to_camera, sphere_to_camera) - (sphere.radius
// 			* sphere.radius);
// 	discriminant = b * b - 4 * a * c;
// 	if (discriminant < 0)
// 		return ((float2){INFINITY, INFINITY});
// 	discriminant = sqrt(discriminant);
// 	return ((float2){(-b + discriminant) / (2 * a),
// 		(-b - discriminant) / (2 * a)});
// }
