/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:49 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/23 14:11:50 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "../libft/libft.h"
# include "data_struct.h"
# include "math_utils.h"
# include "mlx.h"
# include <math.h>
# include <stdbool.h>
# include <stdio.h>

# if defined(__APPLE__)
#  include "macos_defines.h"
# elif defined(__linux__)
#  define PTR_NULL "(nil)"
# endif

typedef struct s_data
{
	t_camera	*camera;
	void		*mlx_ptr;
	void		*win_ptr;
	void		*img;
	char		*addr;
	int			bits_per_pixel;
	int			line_length;
	int			endian;
	int			win_height;
	int			win_width;
	int			key;
	int			key_held;
}				t_data;

// custom mlx
void			my_mlx_put_pixels(t_data *data, int x, int y, int color);
int				deal_key_release(int key, t_data *data);
int				deal_key_press(int key, t_data *data);
int				deal_input(t_data *data);

// tests
void			vector_test(void);
void			qua_test(void);

#endif