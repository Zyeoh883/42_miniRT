/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_object_intercept.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:13:56 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/15 15:18:16 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

float2	intersect_ray_sphere(U __global t_object *object, t_ray ray);
float2	intersect_ray_plane(U __global t_object *object, t_ray ray);

float2 ray_intersection(U __global t_object *object, t_ray ray)
{
	if (object->type == SPHERE)
		return (intersect_ray_sphere(object, ray));
	else if (object->type == PLANE)
		return (intersect_ray_plane(object, ray));
	return((float2)(INFINITY, INFINITY));
}

float2	intersect_ray_sphere(U __global t_object *object, t_ray ray)
{
	float4	sphere_to_camera;
	float	a;
	float	b;
	float	c;
	float	discriminant;

	sphere_to_camera = vector_subtraction(ray.pos, object->pos);
	a = vector_dot_product(ray.direction, ray.direction);
	b = 2 * vector_dot_product(sphere_to_camera, ray.direction);
	c = vector_dot_product(sphere_to_camera, sphere_to_camera)
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

	denom = quat_dot(object->quat, ray.direction);
	t = quat_dot(object->pos - ray.pos, object->quat) / denom;
	if (t >= 0)
		return ((float2)(t, INFINITY));
	return ((float2)(INFINITY, INFINITY));
}

// float2	ray_intersection(t_object sphere, t_ray ray)
// {
// 	float4		sphere_to_camera;
// 	float		a;
// 	float		b;
// 	float		c;
// 	float		discriminant;

// 	sphere_to_camera = vector_subtraction(ray.pos, sphere.pos);
// 	a = vector_dot_product(ray.direction, ray.direction);
// 	b = 2 * vector_dot_product(sphere_to_camera, ray.direction);
// 	c = vector_dot_product(sphere_to_camera, sphere_to_camera) - (sphere.radius
// 			* sphere.radius);
// 	discriminant = b * b - 4 * a * c;
// 	if (discriminant < 0)
// 		return ((float2){INFINITY, INFINITY});
// 	discriminant = sqrt(discriminant);
// 	return ((float2){(-b + discriminant) / (2 * a),
// 		(-b - discriminant) / (2 * a)});
// }