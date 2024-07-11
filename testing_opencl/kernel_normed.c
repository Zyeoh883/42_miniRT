/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel_normed.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:44:15 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/10 21:44:59 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <OpenCL/opencl.h>

#define G

__kernel void	function1(G __global int *input, G __global int *output, int n)
{
	int	gid;

	gid = get_global_id(0);
	if (gid < n)
		output[gid] = input[gid] * 2;
}

int	is_prime(int num)
{
	int	i;

	if (num <= 1)
		return (0);
	i = 2;
	while (i * i <= num)
	{
		if (num % i == 0)
			return (0);
		i++;
	}
	return (1);
}

__kernel void	find_primes(G __global int *numbers, G __global int *results,
	int count)
{
	int	gid;

	gid = get_global_id(0);
	if (gid < count)
		results[gid] = is_prime(numbers[gid]);
}

__kernel void	vector_add(G __global const float *a, G __global const float *b,
	G __global float *result, int num)
{
	int	gid;

	gid = get_global_id(0);
	if (gid < num)
		result[gid] = a[gid] + b[gid];
}
