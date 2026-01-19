/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reservoir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 19:16:36 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/19 19:19:40 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// / max(candidate.pdf, 1e-10f);
// if (isnan(candidate.radiance.x) || isinf(candidate.radiance.x))
//   printf("candidate.radiance");
// The importance weight is typically the luminance of the radiance divided by
// its PDF
float	calculate_candidate_importance_weight(t_candidate candidate)
{
	float	lum;

	if (candidate.pdf <= 0.0f)
		return (0.0f);
	lum = luma(candidate.radiance);
	return (lum);
}

// Function to add a new candidate to the reservoir
void	update_reservoir(t_reservoir *res, t_candidate new_candidate,
		t_sample_data *sample_data)
{
	float	candidate_imp_weight;

	candidate_imp_weight = calculate_candidate_importance_weight(new_candidate);
	res->weighted_sum += candidate_imp_weight;
	res->m++;
	if ((sample_random(sample_data)) < (candidate_imp_weight
			/ res->weighted_sum))
		res->candidate = new_candidate;
}

// if (isnan(res->candidate.pdf) || isinf(res->candidate.pdf))
//   printf("its Pdf");
// Magenta for debug
float3	reservoir_final_color(t_reservoir *res)
{
	float	avg_luminence;
	float	selected_luminence;
	float3	final_color;

	if (res->m == 0 || res->weighted_sum <= 0.0f || res->candidate.pdf <= 0.0f)
		return ((float3)(0, 0, 0));
	avg_luminence = res->weighted_sum / (float)res->m;
	selected_luminence = luma(res->candidate.radiance);
	if (selected_luminence <= 1e-10f)
		return ((float3)(0, 0, 0));
	final_color = (res->candidate.radiance / selected_luminence)
		* avg_luminence;
	if (isnan(final_color.x) || isnan(final_color.y) || isnan(final_color.z)
		|| isinf(final_color.x) || isinf(final_color.y) || isinf(final_color.z))
		return ((float3)(1.0f, 0.0f, 1.0f));
	return (final_color);
}
