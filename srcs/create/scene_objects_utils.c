/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_objects_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 20:31:35 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/16 17:43:28 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	is_valid_object_id(char *id)
{
	if (!ft_strcmp(id, SPHERE))
		return (1);
	if (!ft_strcmp(id, PLANE))
		return (1);
	if (!ft_strcmp(id, CYLINDER))
		return (1);
	if (!ft_strcmp(id, LIGHT))
		return (1);
	return (0);
}

int	is_valid_camera_ambient_id(char *id)
{
	if (!ft_strcmp(id, CAMERA))
		return (1);
	if (!ft_strcmp(id, AMBIENT))
		return (1);
	return (0);
}

int	is_material_type(char *id)
{
	if (!ft_strcmp(id, MAT_DIFFUSE))
		return (1);
	if (!ft_strcmp(id, MAT_METAL_COPPER))
		return (1);
	if (!ft_strcmp(id, MAT_METAL_GOLD))
		return (1);
	if (!ft_strcmp(id, MAT_METAL_SILVER))
		return (1);
	if (!ft_strcmp(id, MAT_METAL_IRON))
		return (1);
	if (!ft_strcmp(id, MAT_DIELECTRIC_GLASS))
		return (1);
	if (!ft_strcmp(id, MAT_METAL_DIELECTRIC_COPPER))
		return (1);
	if (!ft_strcmp(id, MAT_METAL_DIELECTRIC_GOLD))
		return (1);
	if (!ft_strcmp(id, MAT_MIRROR))
		return (1);
	return (0);
}

// printf("rgb : %s\n", str);
int	is_rgb(char *str)
{
	int		num;
	char	*temp;

	if (!str || ft_strlen(str) > 3)
		return (0);
	temp = str;
	while (ft_isdigit(*str))
		str++;
	num = ft_atoi(temp);
	if (num > 256 || num < 0)
		return (0);
	if (*str)
		return (0);
	return (1);
}
