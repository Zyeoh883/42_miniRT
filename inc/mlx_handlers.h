/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mlx_handlers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/24 10:43:27 by zyeoh             #+#    #+#             */
/*   Updated: 2026/01/19 19:52:47 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MLX_HANDLERS_H
# define MLX_HANDLERS_H

typedef struct s_data	t_data;
typedef struct s_camera	t_camera;
typedef struct s_inputs	t_inputs;

void					my_mlx_put_pixels(t_data *data, int x, int y,
							int color);
int						deal_key_release(int key, t_data *data);
int						deal_key_press(int key, t_data *data);
int						deal_input(t_data *data);
int						mouse_hook(int x, int y, t_data *data);

// inputs
void					input_translate(t_data *data, t_inputs inputs);
// int						input_rotation(t_data *data);

#endif