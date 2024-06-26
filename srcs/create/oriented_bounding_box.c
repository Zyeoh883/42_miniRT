/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oriented_bounding_box.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:07:23 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/26 15:10:45 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_OBB assign_sphere_obb(t_sphere sphere)
{
    t_OBB obb;

    obb.pos = sphere.pos;
    obb.quat = (t_quat){1, 0, 0, 0};
    obb.half_len = (t_vector){sphere.radius, sphere.radius, sphere.radius};
    return (obb);
}
