/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 19:58:48 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/19 19:58:50 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

float	ft_atof(const char *str)
{
	float	result;
	float	decimal;

	result = (float)ft_atol(str);
	while (*str && *str != '.')
		str++;
	if (*str)
	{
		str++;
		decimal = (float)ft_atol(str);
		while (*str++)
			decimal /= 10;
		if (result < 0)
			result -= decimal;
		else
			result += decimal;
	}
	return (result);
}
