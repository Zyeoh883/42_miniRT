/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:31:11 by zyeoh             #+#    #+#             */
/*   Updated: 2024/06/30 19:45:10 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_object	assign_object(t_OBB obb, char type)
{
	t_object	object;

	object.obb = obb;
	object.type = type;
	return (object);
}

t_sphere	assign_sphere(t_vector pos, float radius, int color)
{
	t_sphere	sphere;

	sphere.pos = pos;
	sphere.quat = _mm_set_ps(0, 0, 0, 1);
	sphere.radius = radius;
	sphere.color = color;
	return (sphere);
}

t_list	*create_ll_objects(void) // !parsing to execution starts here
{
	t_list *root_node;
	t_list *node;
	t_object *object;
	int n;

	t_quat q[7];
	t_vector pos[7];
	int radius[7];
	int color[7];

	// 1st
	q[0] = _mm_set_ps(0, 0, 0, 1);
	pos[0] = _mm_set_ps(3, 0, 0, 0);
	radius[0] = 1;
	color[0] = 0xFFFFFF;
	// 2nd,
	q[1] = _mm_set_ps(0, 0, 0, 1);
	pos[1] = _mm_set_ps(3, -10, 0, 0);
	radius[1] = 1;
	color[1] = 0xFF0000;
	// 3rd
	q[2] = _mm_set_ps(0, 0, 0, 1);
	pos[2] = _mm_set_ps(13, 0, 0, 0);
	radius[2] = 1;
	color[2] = 0x00FF00;
	// 4th
	q[3] = _mm_set_ps(0, 0, 0, 1);
	pos[3] = _mm_set_ps(3, 10, 0, 0);
	radius[3] = 1;
	color[3] = 0x0000FF;
	// 5th
	q[4] = _mm_set_ps(0, 0, 0, 1);
	pos[4] = _mm_set_ps(-7, 0, 0, 0);
	radius[4] = 1;
	color[4] = 0xFF00FF;
	// 6th
	q[5] = _mm_set_ps(0, 0, 0, 1);
	pos[5] = _mm_set_ps(3, 0, 10, 0);
	radius[5] = 1;
	color[5] = 0xFFFF00;
	// 7th
	q[6] = _mm_set_ps(0, 0, 0, 1);
	pos[6] = _mm_set_ps(3, 0, -10, 0);
	radius[6] = 1;
	color[6] = 0x00FFFF;

	// Add to linked list
	object = ft_calloc(1, sizeof(t_object));
	if (!object)
		perror_and_exit("malloc", EXIT_FAILURE);
	object->sphere = assign_sphere(pos[0], radius[0], color[0]);
	object->type = 'S';
	root_node = ft_lstnew(object);
	if (!root_node)
		perror_and_exit("malloc", EXIT_FAILURE);

	n = 0;
	while (++n < 7)
	{
		object = ft_calloc(1, sizeof(t_object));
		if (!object)
			perror_and_exit("malloc", EXIT_FAILURE);
		object->sphere = assign_sphere(pos[n], radius[n], color[n]);
		object->type = 'S';
		node = ft_lstnew(object);
		if (!node)
			perror_and_exit("malloc", EXIT_FAILURE);
		ft_lstadd_back(&root_node, node);
	}
	return (root_node);
}

t_object	*create_objects_array(t_list *root_node)
{
	t_object	*arr_objects;
	t_list		*head;
	int			len;
	int			n;

	len = ft_lstsize(root_node);
	arr_objects = ft_calloc(len + 1, sizeof(t_object));
	if (!arr_objects)
		perror_and_exit("malloc", EXIT_FAILURE);
	head = root_node;
	n = -1;
	while (++n < len)
	{
		// printf("%d\n", n);
		ft_memcpy(arr_objects + n, head->content, sizeof(t_object));
		arr_objects[n].obb = assign_sphere_obb(arr_objects[n].sphere);
		head = head->next;
	}
	ft_lstclear(&root_node, free);
	
	// n = -1;
	// while (arr_objects[++n].type)
	// {
	// 	print_vector(arr_objects[n].sphere.pos);
	// }

	return (arr_objects);
}
