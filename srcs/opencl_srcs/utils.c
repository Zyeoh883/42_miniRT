/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 15:30:40 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/19 14:44:44 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

float3	to_float3(float x)
{
	return ((float3)(x, x, x));
}

t_candidate	init_candidate(void)
{
	t_candidate	candidate;

	candidate.radiance = (float3)(0.0f, 0.0f, 0.0f);
	candidate.pdf = 0;
	candidate.weight = (float3)(0.0f, 0.0f, 0.0f);
	return (candidate);
}

t_reservoir	init_reservoir(void)
{
	t_reservoir	reservoir;

	reservoir.candidate = init_candidate();
	// reservoir.M = 0;
	reservoir.weighted_sum = 0;
	reservoir.seed = 0;
	return (reservoir);
}

void	print_vec(float3 vec)
{
	printf("%f %f %f\n", vec.x, vec.y, vec.z);
}

// float gen_seed(uint *seed)
// {
//   *seed = pcg_hash(*seed);
//   return(*seed * 2.3283064365386963e-10f);
// }

float	sample_random(t_sample_data *sample_data)
{
	uint	seed;

	seed = sample_data->seed;
	seed = pcg_hash(seed ^ pcg_hash(sample_data->x));
	seed = pcg_hash(seed ^ pcg_hash(sample_data->y));
	seed = pcg_hash(seed ^ pcg_hash(sample_data->sample_index));
	// seed = pcg_hash(seed ^ pcg_hash(type));
	sample_data->seed = seed;
	return (seed * 2.3283064365386963e-10f);
}