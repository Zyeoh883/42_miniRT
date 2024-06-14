/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:56:38 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/14 14:42:58 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int main(void)
{
	vector_test();
	qua_test();

	return (0);
}

// int	main(void)
// {
// 	t_data	data;

//     data.win_height = 1080;
// 	data.win_width = 1920;
// 	data.mlx_ptr = mlx_init();
//     data.img = mlx_new_image(data.mlx_ptr, data.win_width, data.win_height);
// 	data.addr = mlx_get_data_addr(data.img, &data.bits_per_pixel,
// 			&data.line_length, &data.endian);
// 	data.win_ptr = mlx_new_window(data.mlx_ptr, data.win_width, data.win_height, "miniRT");
// 	mlx_put_image_to_window(&data, data.win_ptr, data.img, 0, 0);
// 	mlx_loop(data.mlx_ptr);
// 	ft_printf("Numbers go vroom\n");

// 	return (0);
// }
