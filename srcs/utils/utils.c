/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 19:25:24 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/09 21:18:35 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

char	*ft_strcat_free(char *str1, char *str2)
{
	char	*result;
	int		size;

	if (!str1 || !str2)
		return (NULL);
	size = ft_strlen(str1) + ft_strlen(str2) + 1;
	result = ft_calloc(size, sizeof(char));
	ft_strlcat(result, str1, size);
	ft_strlcat(result, str2, size);
	free(str1);
	free(str2);
	return (result);
}

char	*read_cfile(char *name)
{
	char	*result;
	char	*line;
	int		size;
	int		fd;

	result = ft_strdup("");
	fd = open(name, O_RDONLY);
	if (!result && !fd)
		exit(1);
	size = 14;
	while (--size > 0) // skips 42_header
		line = get_next_line(fd);
	while (line)
	{
		result = ft_strcat_free(result, line);
		line = get_next_line(fd);
	}
	close(fd);
	return (result);
}
