/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_handlers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 10:43:27 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/25 11:20:07 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_HANLDERS_H
# define MLX_HANLDERS_H

typedef struct s_data	t_data;

void					my_mlx_put_pixels(t_data *data, int x, int y,
							int color);
int						deal_key_release(int key, t_data *data);
int						deal_key_press(int key, t_data *data);
int						deal_input(t_data *data);
int						mouse_hook(int x, int y, t_data *data);

// inputs
int						input_translate(t_data *data);
int						input_rotation(t_data *data);

#endif