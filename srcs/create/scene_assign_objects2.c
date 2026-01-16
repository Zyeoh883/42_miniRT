/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_assign_objects2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 17:33:57 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/16 17:34:49 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	apply_material_properties_base2(t_object *object, char *material_type)
{
	if (!ft_strcmp(material_type, MAT_METAL_IRON))
	{
		object->F_0 = (cl_float3){{0.56f, 0.57f, 0.58f}};
		object->roughness_sqr = 0.4f;
		return (1.0f);
	}
	else if (!ft_strcmp(material_type, MAT_DIELECTRIC_GLASS))
	{
		object->F_0 = (cl_float3){{0.02f, 0.02f, 0.02f}};
		object->roughness_sqr = 0.01f;
		return (0.0f);
	}
	else if (!ft_strcmp(material_type, MAT_METAL_DIELECTRIC_COPPER))
		object->F_0 = (cl_float3){{0.7f, 0.4f, 0.3f}};
	else if (!ft_strcmp(material_type, MAT_METAL_DIELECTRIC_GOLD))
		object->F_0 = (cl_float3){{0.8f, 0.6f, 0.3f}};
	else if (!ft_strcmp(material_type, MAT_MIRROR))
	{
		object->F_0 = (cl_float3){{0.8f, 0.8f, 0.8f}};
		object->roughness_sqr = 0.001f;
		return (1.0f);
	}
	object->roughness_sqr = 0.2f;
	return (0.5f);
}

float	apply_material_properties_base(t_object *object, char *material_type)
{
	if (!material_type || !ft_strcmp(material_type, MAT_DIFFUSE))
	{
		object->F_0 = (cl_float3){{0.0f, 0.0f, 0.0f}};
		object->roughness_sqr = 1.0f;
		return (0.0f);
	}
	else if (!ft_strcmp(material_type, MAT_METAL_COPPER))
	{
		object->F_0 = (cl_float3){{0.95f, 0.42f, 0.17f}};
		object->roughness_sqr = 0.0000003f;
		return (1.0f);
	}
	else if (!ft_strcmp(material_type, MAT_METAL_GOLD))
	{
		object->F_0 = (cl_float3){{1.0f, 0.86f, 0.57f}};
		object->roughness_sqr = 0.3f;
		return (1.0f);
	}
	else if (!ft_strcmp(material_type, MAT_METAL_SILVER))
	{
		object->F_0 = (cl_float3){{0.95f, 0.93f, 0.88f}};
		object->roughness_sqr = 0.1f;
		return (1.0f);
	}
	return (apply_material_properties_base2(object, material_type));
}

void	assign_camera(t_camera *camera, char **line)
{
	printf("Assigning camera\n");
	if (!line[1])
		error_and_exit("Camera position missing", EXIT_FAILURE);
	camera->pos = get_cl_float3(line[1]);
	if (!line[2])
		error_and_exit("Camera direction missing", EXIT_FAILURE);
	camera->quat = get_dir(line[2]);
	if (!line[3])
		error_and_exit("FOV missing", EXIT_FAILURE);
	camera->fov = get_cl_float(line[3]);
	if (camera->fov >= 180 || camera->fov <= 0)
		error_and_exit("Fov must be BETWEEN 0-180", EXIT_FAILURE);
	camera->fov *= TO_RADIAN;
	if (line[4])
		error_and_exit("Unknown trailing characters", EXIT_FAILURE);
}

void	assign_ambient(t_camera *camera, char **line)
{
	printf("Assigning ambient\n");
	if (!line[1])
		error_and_exit("Ambient lighting ratio missing", EXIT_FAILURE);
	camera->ambient.amb_light_ratio = get_cl_float(line[1]);
	if (!line[2])
		error_and_exit("Camera top color missing", EXIT_FAILURE);
	camera->ambient.amb_top_color = get_rgb_value(line[2]);
	if (!line[3])
		error_and_exit("Ambient bottom color missing", EXIT_FAILURE);
	camera->ambient.amb_bot_color = get_rgb_value(line[3]);
	if (line[4])
		error_and_exit("Unknown trailing characters", EXIT_FAILURE);
}
