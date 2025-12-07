/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:50:59 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/25 14:51:49 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	perror_and_exit(char *str, int exit_code)
{
	ft_putstr_fd("minirt: ", STDERR_FILENO);
	ft_putendl_fd(str, STDERR_FILENO);
	perror(str);
	exit(exit_code);
}

void	error_and_exit(char *str, int exit_code)
{
	ft_putstr_fd("minirt: ", STDERR_FILENO);
	ft_putendl_fd(str, STDERR_FILENO);
	exit(exit_code);
}
