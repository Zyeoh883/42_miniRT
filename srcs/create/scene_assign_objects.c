/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_assign_objects.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:32:24 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/17 12:42:55 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	assign_sphere(t_object *object, char **line, int *line_offset)
{
	printf("Assigning sphere\n");
	object->obj_type = CL_SPHERE;
	if (!line[++*line_offset])
		error_and_exit("Sphere Radius missing", EXIT_FAILURE);
	object->sphere.radius = get_cl_float(line[*line_offset]);
	if (!line[++*line_offset])
		error_and_exit("RGB values missing", EXIT_FAILURE);
	object->diffuse_albedo = get_rgb_value(line[*line_offset]);
}

void	assign_plane(t_object *object, char **line, int *line_offset)
{
	printf("Assigning plane\n");
	object->obj_type = CL_PLANE;
	if (!line[++*line_offset])
		error_and_exit("Plane Direction missing", EXIT_FAILURE);
	object->dir = get_dir(line[*line_offset]);
	if (!line[++*line_offset])
		error_and_exit("RGB values missing", EXIT_FAILURE);
	object->diffuse_albedo = get_rgb_value(line[*line_offset]);
}

void	assign_light(t_object *object, char **line, int *line_offset)
{
	float	light_ratio;

	printf("Assigning light\n");
	object->obj_type = CL_LIGHT;
	object->emission = (cl_float3){{100.0f, 100.0f, 100.0f}};
	if (!line[++*line_offset])
		error_and_exit("Light Radius missing", EXIT_FAILURE);
	object->sphere.radius = get_cl_float(line[*line_offset]);
	if (!line[++*line_offset])
		error_and_exit("Light ratio missing", EXIT_FAILURE);
	light_ratio = get_cl_float(line[*line_offset]);
	printf("light : %f\n", light_ratio);
	object->emission = (cl_float3){{100.0f * light_ratio, 100.0f * light_ratio,
		100.0f * light_ratio}};
	if (!line[++*line_offset])
		error_and_exit("RGB values missing", EXIT_FAILURE);
	object->diffuse_albedo = get_rgb_value(line[*line_offset]);
}

void	assign_cylinder(t_object *object, char **line, int *line_offset)
{
	printf("Assigning cylinder\n");
	object->obj_type = CL_CYCLINDER;
	if (!line[++*line_offset])
		error_and_exit("Cylinder Direction missing", EXIT_FAILURE);
	object->dir = get_dir(line[*line_offset]);
	if (!line[++*line_offset])
		error_and_exit("Cylinder Diameter missing", EXIT_FAILURE);
	object->cylinder.radius = get_cl_float(line[*line_offset]) / 2.0f;
	if (!line[++*line_offset])
		error_and_exit("Cylinder Height missing", EXIT_FAILURE);
	object->cylinder.height = get_cl_float(line[*line_offset]);
	if (!line[++*line_offset])
		error_and_exit("Cylinder RGB values missing", EXIT_FAILURE);
	object->diffuse_albedo = get_rgb_value(line[*line_offset]);
}

t_object	assign_object(char **line)
{
	t_object	object;
	int			line_offset;
	float		m;

	line_offset = 0;
	object.emission = (cl_float3){{0, 0, 0}};
	if (line[0] && line[1] && is_material_type(line[1]))
		m = apply_material_properties_base(&object, line[++line_offset]);
	if (!line[++line_offset])
		error_and_exit("Position in float3 missing", EXIT_FAILURE);
	object.pos = get_cl_float3(line[line_offset]);
	if (!ft_strcmp(*line, SPHERE))
		assign_sphere(&object, line, &line_offset);
	else if (!ft_strcmp(*line, PLANE))
		assign_plane(&object, line, &line_offset);
	else if (!ft_strcmp(*line, CYLINDER))
		assign_cylinder(&object, line, &line_offset);
	else if (!ft_strcmp(*line, LIGHT))
		assign_light(&object, line, &line_offset);
	if (line[++line_offset])
		error_and_exit("Unknown trailing characters", EXIT_FAILURE);
	object.specular_albedo = vector_scalar_product(object.diffuse_albedo, m);
	object.diffuse_albedo = vector_scalar_product(object.diffuse_albedo, 1 - m);
	return (object);
}
