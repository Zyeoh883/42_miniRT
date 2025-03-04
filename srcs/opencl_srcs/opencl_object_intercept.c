/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_object_intercept.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:13:56 by zyeoh             #+#    #+#             */
/*   Updated: 2024/08/22 20:54:09 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

float2	intersect_ray_sphere(U __constant t_object *object, t_ray ray);
float2	intersect_ray_plane(U __constant t_object *object, t_ray ray);
// bool rayAABBIntersect(float3 ray_ori, float3 ray_dir, float3 aabbMin, float3 aabbMax);

float2 ray_intersection(U __constant t_object *object, t_ray ray)
{
	if (object->obj_type == SPHERE || object->obj_type == LIGHT)
  {
    // if (!rayAABBIntersect(ray.pos, ray.dir, object->pos - object->obb.half_len, object->pos + object->obb.half_len))
    //   return ((float2)(INFINITY, INFINITY));
		return (intersect_ray_sphere(object, ray));
  }
	else if (object->obj_type == PLANE)
		return (intersect_ray_plane(object, ray));
	return((float2)(INFINITY, INFINITY));
}

float2	intersect_ray_sphere(U __constant t_object *object, t_ray ray)
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

float2	intersect_ray_plane(U __constant t_object *object, t_ray ray)
{
	float t;
	float denom;

	denom = dot(object->dir, ray.dir);
	t = dot(object->pos - ray.pos, object->dir) / denom;
	if (t >= 0)
		return ((float2)(t, INFINITY));
	return ((float2)(INFINITY, INFINITY));
}

// ! do this first
float2	intersect_ray_cyclinder(U __constant t_object *object, t_ray ray)
{
	float4	sphere_to_camera;
	float	a;
	float	b;
	float	c;
	float	discriminant;
	float t;
	float denom;

	denom = dot(object->dir, ray.dir);
	t = dot(object->pos - ray.pos, object->dir) / denom;
	if (t >= 0)
		return ((float2)(t, INFINITY));
	return ((float2)(INFINITY, INFINITY));
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
