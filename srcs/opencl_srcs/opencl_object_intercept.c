/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_object_intercept.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:13:56 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/09 00:58:22 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

float2	intersect_ray_sphere(U __global t_object *object, t_ray ray);
float2	intersect_ray_plane(U __global t_object *object, t_ray ray);
float2	intersect_ray_cylinder(U __global t_object *object, t_ray ray);
// bool rayAABBIntersect(float3 ray_ori, float3 ray_dir, float3 aabbMin, float3 aabbMax);

float2 ray_intersection(U __global t_object *object, t_ray ray)
{
	if (object->obj_type == SPHERE || object->obj_type == LIGHT)
  {
    // if (!rayAABBIntersect(ray.pos, ray.dir, object->pos - object->obb.half_len, object->pos + object->obb.half_len))
    //   return ((float2)(INFINITY, INFINITY));
		return (intersect_ray_sphere(object, ray));
  }
	else if (object->obj_type == PLANE)
		return (intersect_ray_plane(object, ray));
	else if (object->obj_type == CYLINDER)
		return (intersect_ray_cylinder(object, ray));
	return((float2)(INFINITY, INFINITY));
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
	c = dot(sphere_to_camera, sphere_to_camera)
		- (object->sphere.radius * object->sphere.radius);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return ((float2)(INFINITY, INFINITY));
	discriminant = sqrt(discriminant);
	return ((float2)((-b + discriminant) / (2 * a), (-b - discriminant) / (2
				* a)));
}

float2	intersect_ray_plane(U __global t_object *object, t_ray ray)
{
	float t;
	float denom;

	denom = dot(object->dir, ray.dir);
	t = dot(object->pos - ray.pos, object->dir) / denom;
	if (t >= 0)
		return ((float2)(t, INFINITY));
	return ((float2)(INFINITY, INFINITY));
}

float2	intersect_ray_cylinder(U __global t_object *object, t_ray ray)
{
	float2 result = (float2)(INFINITY, INFINITY);

	// Cylinder parameters
	float3 center = object->pos;
	float3 axis = normalize(object->dir);
	float radius = object->cyclinder.radius;
	float height = object->cyclinder.height;

	// Cylinder extends from center - axis * height/2 to center + axis * height/2
	float3 bottom_center = center - axis * (height * 0.5f);
	float3 top_center = center + axis * (height * 0.5f);

	// Transform ray to cylinder coordinate system where axis is Z-axis
	float3 ray_dir = normalize(ray.dir);
	float3 ray_origin = ray.pos;

	// Project ray onto plane perpendicular to axis
	float3 oc = ray_origin - center;
	float proj_oc_axis = dot(oc, axis);
	float proj_dir_axis = dot(ray_dir, axis);

	// Quadratic equation coefficients for infinite cylinder
	float a = dot(ray_dir, ray_dir) - proj_dir_axis * proj_dir_axis;
	float b = 2.0f * (dot(oc, ray_dir) - proj_oc_axis * proj_dir_axis);
	float c = dot(oc, oc) - proj_oc_axis * proj_oc_axis - radius * radius;
	float discriminant = b * b - 4.0f * a * c;

	if (discriminant < 0.0f)
		return result; // No intersection with infinite cylinder

	discriminant = sqrt(discriminant);
	float t1 = (-b - discriminant) / (2.0f * a);
	float t2 = (-b + discriminant) / (2.0f * a);

	// Check cylinder body intersections
	float t_cylinder = INFINITY;
	if (t1 > 0.0f)
	{
		float3 hit_point = ray_origin + ray_dir * t1;
		float dist_along_axis = dot(hit_point - center, axis);
		if (fabs(dist_along_axis) <= height * 0.5f)
			t_cylinder = t1;
	}

	if (t2 > 0.0f && (t_cylinder == INFINITY || t2 < t_cylinder))
	{
		float3 hit_point = ray_origin + ray_dir * t2;
		float dist_along_axis = dot(hit_point - center, axis);
		if (fabs(dist_along_axis) <= height * 0.5f)
			t_cylinder = t2;
	}

	// Check intersection with bottom cap (disk)
	float t_bottom = INFINITY;
	float denom = dot(axis, ray_dir);
	if (fabs(denom) > 1e-6f)
	{
		float t = dot(bottom_center - ray_origin, axis) / denom;
		if (t > 0.0f)
		{
			float3 hit_point = ray_origin + ray_dir * t;
			if (distance(hit_point, bottom_center) <= radius)
				t_bottom = t;
		}
	}

	// Check intersection with top cap (disk)
	float t_top = INFINITY;
	if (fabs(denom) > 1e-6f)
	{
		float t = dot(top_center - ray_origin, axis) / denom;
		if (t > 0.0f)
		{
			float3 hit_point = ray_origin + ray_dir * t;
			if (distance(hit_point, top_center) <= radius)
				t_top = t;
		}
	}

	// Find the closest intersection
	float t_min = INFINITY;
	if (t_cylinder < t_min) t_min = t_cylinder;
	if (t_bottom < t_min) t_min = t_bottom;
	if (t_top < t_min) t_min = t_top;

	if (t_min < INFINITY)
		result.x = t_min;

	return result;
}

// bool rayAABBIntersect(float3 ray_ori, float3 ray_dir, float3 aabbMin, float3 aabbMax)
// {
//     float3 tMin;
//     float3 tMax;
//     float3 t0;
//     float3 t1;
//     float tenter;
//     float texit;
//
//     tMin = (aabbMin - ray_ori) / ray_dir;
//     tMax = (aabbMax - ray_ori) / ray_dir;
//     t0 = fmin(tMin, tMax);
//     t1 = fmax(tMin, tMax);
//     tenter = fmax(fmax(t0.x, t0.y), t0.z);
//     texit = fmin(fmin(t1.x, t1.y), t1.z);
//
//     // if (tenter > texit || texit < 0.0)
//     //     return false;
//
//     return !(tenter > texit || texit < 0.0);
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
