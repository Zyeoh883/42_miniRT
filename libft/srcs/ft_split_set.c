/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_set.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 19:57:45 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/19 19:57:46 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	string_count(const char *str, char *set)
{
	int	word_count;

	word_count = 0;
	while (*str)
	{
		while (ft_strchr(set, *str) && *str)
			str++;
		if (!ft_strchr(set, *str) && *str)
			word_count++;
		while (!ft_strchr(set, *str) && *str)
			str++;
	}
	return (word_count);
}

static int	string_len(const char *str, char *set)
{
	int	len;

	len = 0;
	while (!ft_strchr(set, str[len]) && str[len])
		len++;
	return (len);
}

static void	str_cpy(char *dest, const char *src, int len)
{
	int	n;

	n = -1;
	while (++n < len)
		dest[n] = src[n];
	dest[n] = '\0';
}

char	**ft_split_set(const char *str, char *set)
{
	char	**split;
	int		n;
	int		len;

	if (!str)
		return (NULL);
	split = (char **)malloc(sizeof(char *) * (string_count(str, set) + 1));
	if (!split)
		return (NULL);
	n = 0;
	while (*str)
	{
		if (!ft_strchr(set, *str) && *str)
		{
			len = string_len(str, set);
			split[n] = (char *)malloc(sizeof(char) * (len + 1));
			str_cpy(split[n], str, len);
			n++;
			str += len - 1;
		}
		str++;
	}
	split[n] = 0;
	return (split);
}

// #include <stdio.h>
//
// int main(void) {
//   char str[] = " hel p  go ikj";
//   char *sep = "   ";
//   char **split;
//
//   sep = ' ';
//   split = ft_split_set(str, sep);
//   while (*split != NULL) {
//     printf("%s\n", *split);
//     free(*split);
//     split++;
//   }
// }
