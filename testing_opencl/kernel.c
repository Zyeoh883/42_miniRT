/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:54:52 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/09 15:02:52 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <OpenCL/opencl.h>

#define G

__kernel void	findprime(G __global int *result, const int n)
{
	int	candidate;
	int	count;
	int	isprime;
	int	i;

	candidate = 2;
	count = 0;
	while (count < n)
	{
		isprime = 1;
		i = 2;
		while (i * i <= candidate)
		{
			if (candidate % i == 0)
			{
				isprime = 0;
				break ;
			}
			i++;
		}
		if (isprime)
			count++;
		if (count == n)
		{
			*result = candidate;
			return ;
		}
		candidate++;
	}
}
