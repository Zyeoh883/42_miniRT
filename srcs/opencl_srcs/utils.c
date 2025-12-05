/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opencl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 15:30:40 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/15 14:57:55 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


float3 to_float3(float x)
{
    return (float3)(x, x, x);
}

t_candidate init_candidate()
{
  t_candidate candidate;
  
  candidate.radiance = (float3)(0.0f, 0.0f, 0.0f);
  candidate.pdf = 0;
  candidate.weight =  (float3)(0.0f, 0.0f, 0.0f);
  return candidate;
}

t_reservoir init_reservoir()
{
  t_reservoir reservoir;

  reservoir.candidate = init_candidate();
  reservoir.M = 0;
  reservoir.weighted_sum = 0;
  reservoir.seed = 0;
  
  return reservoir;
}

void print_vec(float3 vec)
{
  printf("%f %f %f\n", vec.x, vec.y, vec.z);
}

