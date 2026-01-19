/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bxdf2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 17:22:24 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/19 17:24:59 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

float3	reflect(float3 v, float3 n)
{
	return (normalize(v - 2.0f * dot(v, n) * n));
}

float3	ggx_sample(float3 n, float alpha, float2 s)
{
	t_vars_ggx_sample	x;

	x.phi = 2.0f * (float)M_PI * s.x;
	x.tan_theta_sq = alpha * alpha * s.y / (1.0f - s.y);
	x.cos_theta = 1.0f / sqrt(1.0f + x.tan_theta_sq);
	x.sin_theta = sqrt(max(0.0f, 1.0f - x.cos_theta * x.cos_theta));
	x.axis = select((float3)(1.0f, 0.0f, 0.0f), (float3)(0.0f, 1.0f, 0.0f),
			(int3)(fabs(n.x) > 0.1f));
	x.t = normalize(cross(x.axis, n));
	x.b = cross(n, x.t);
	x.cos_phi = cos(x.phi);
	x.sin_phi = sin(x.phi);
	return (normalize(x.b * x.cos_phi * x.sin_theta + x.t * x.sin_phi
			* x.sin_theta + n * x.cos_theta));
}

int	check_checkerboard(float3 normal)
{
	t_checkerboard	x;

	x.theta = acos(-normal.y);
	x.phi = atan2(-normal.z, normal.x) + (float)M_PI;
	x.u = x.phi * 0.5f * (float) M_1_PI;
	x.v = x.theta * (float) M_1_PI;
	x.tile_u = floor(x.u * 12.0f);
	x.tile_v = floor(x.v * 12.0f);
	if ((x.tile_u + x.tile_v) % 2 == 0)
		return (1);
	return (0);
}
