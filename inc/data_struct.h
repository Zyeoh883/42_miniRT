/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_struct.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/13 19:16:07 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/14 11:19:45 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_STRUCT_H
# define DATA_STRUCT_H

typedef struct s_article
{
	char	*title;
	char	*author;
	char	*body;
	int		no_comments;
	int		no_claps;

} __attribute__((packed)) t_article;

typedef struct s_vector
{
	int		vector[4];
}			t_vector;

// __attribute__((packed,aligned(4))
// __attribute__((packed))

#endif