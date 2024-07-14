/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl_object_intercept.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:13:56 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/14 15:50:37 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

float2	intersect_ray_sphere(t_sphere sphere, t_ray ray)
{
	float4		sphere_to_camera;
	float		a;
	float		b;
	float		c;
	float		discriminant;

	sphere_to_camera = vector_subtraction(ray.pos, sphere.pos);
	a = vector_dot_product(ray.direction, ray.direction);
	b = 2 * vector_dot_product(sphere_to_camera, ray.direction);
	c = vector_dot_product(sphere_to_camera, sphere_to_camera) - (sphere.radius
			* sphere.radius);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return ((float2){INFINITY, INFINITY});
	discriminant = sqrt(discriminant);
	return ((float2){(-b + discriminant) / (2 * a),
		(-b - discriminant) / (2 * a)});
}
