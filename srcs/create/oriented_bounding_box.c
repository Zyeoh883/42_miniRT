/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oriented_bounding_box.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:07:23 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/12 20:23:03 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// obb.pos = sphere.pos;:with
// obb.quat = sphere.quat;
// obb.half_len = _mm_set_ps(sphere.radius, sphere.radius, sphere.radius, 0);
t_OBB	assign_sphere_obb(t_sphere sphere)
{
	t_OBB	obb;

	obb.half_len = to_float(sphere.radius);
	return (obb);
}
