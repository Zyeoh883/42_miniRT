/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oriented_bounding_box.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:07:23 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/30 19:30:18 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_OBB	assign_sphere_obb(t_sphere sphere)
{
	t_OBB	obb;

	obb.pos = sphere.pos;
	obb.quat = sphere.quat;
	obb.half_len = _mm_set_ps(sphere.radius, sphere.radius, sphere.radius, 0);
	return (obb);
}
