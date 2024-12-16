/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 19:25:24 by zyeoh             #+#    #+#             */
/*   Updated: 2024/07/12 12:05:59 by zyeoh            ###   ########.fr       */
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
	if (!result || fd == -1)
		exit(1);
	size = 13;
	while (--size > 0) // skips 42_header
		free(get_next_line(fd));
	line = get_next_line(fd);
	while (line)
	{
		result = ft_strcat_free(result, line);
		line = get_next_line(fd);
	}
	close(fd);
	return (result);
}

void free_cfile(char **c_file)
{
  int n;

  n = -1;
  while (c_file[++n])
    free(c_file[n]);
  free(c_file);
}

void	free_str_arr(char **str_arr)
{
	int	i;

	i = 0;
	if (str_arr == NULL)
		return ;
	while (str_arr[i] != NULL)
	{
		free(str_arr[i]);
		i++;
	}
	free(str_arr);
}

int	get_rgb_value(char *str)
{
	int	result;

	str += 2;
	result = 0;
	while (ft_isalpha(*str) || ft_isdigit(*str))
	{
		result *= 16;
		if (ft_isalpha(*str))
		{
			if (*str >= 97)
				result += *str - 'a' + 10;
			else
				result += *str - 'A' + 10;
		}
		else
			result += *str - '0';
		++str;
	}
	return (result);
} 

cl_float4 get_quat_value(char *str)
{
  cl_float4 quat;
  char **split;
 
  split = ft_split(str, '.');
  if (!split)
    perror_and_exit("malloc", EXIT_FAILURE);
  quat = (cl_float4){{ft_atoi(split[0]), ft_atoi(split[1]), ft_atoi(split[2]), 0}};
  free_str_arr(split);
  return(quat);  
}

