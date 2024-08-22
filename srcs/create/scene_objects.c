/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zyeoh <zyeoh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/25 14:31:11 by zyeoh             #+#    #+#             */
/*   Updated: 2024/08/22 20:30:29 by zyeoh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_object	assign_object(char type, cl_float4 pos, cl_float4 quat, int color)
{
	t_object	object;

	object.type = type;
	object.pos = pos;
	object.quat = quat;
	object.color = color;
	return (object);
}

t_sphere	assign_sphere(cl_float radius)
{
	t_sphere	sphere;

	sphere.radius = radius;
	return (sphere);
}

t_cyclinder	assign_cyclinder(cl_float radius, cl_float height)
{
	t_cyclinder	cyclinder;

	cyclinder.radius = radius;
	cyclinder.height = height;
	return (cyclinder);
}

t_list	*create_ll_objects(void) // !parsing to execution starts here
{
	t_list *root_node;
	t_list *node;
	t_object *object;
	int n;

	cl_float4 q[9];
	cl_float4 pos[9];
	cl_char type[9];
	int radius[9];
	int color[9];

	// 1st
	type[0] = SPHERE;
	q[0] = (cl_float4){{0, 0, 0, 1}};
	pos[0] = (cl_float4){{0, 0, 3, 0}};
	radius[0] = 1;
	color[0] = 0xFFFFFF;
	// 2nd,
	type[1] = SPHERE;
	q[1] = (cl_float4){{0, 0, 0, 1}};
	pos[1] = (cl_float4){{0, -10, 3, 0}};
	radius[1] = 1;
	color[1] = 0xFF0000;
	// 3rd
	type[2] = SPHERE;
	q[2] = (cl_float4){{0, 0, 0, 1}};
	pos[2] = (cl_float4){{0, 0, 13, 0}};
	radius[2] = 1;
	color[2] = 0x00FF00;
	// 4th
	type[3] = SPHERE;
	q[3] = (cl_float4){{0, 0, 0, 1}};
	pos[3] = (cl_float4){{0, 10, 3, 0}};
	radius[3] = 1;
	color[3] = 0x0000FF;
	// 5th
	type[4] = SPHERE;
	q[4] = (cl_float4){{0, 0, 0, 1}};
	pos[4] = (cl_float4){{0, 0, -7, 0}};
	radius[4] = 1;
	color[4] = 0xFF00FF;
	// 6th
	type[5] = SPHERE;
	q[5] = (cl_float4){{0, 0, 0, 1}};
	pos[5] = (cl_float4){{10, 0, 3, 0}};
	radius[5] = 1;
	color[5] = 0xFFFF00;
	// 7th
	type[6] = SPHERE;
	q[6] = (cl_float4){{0, 0, 0, 1}};
	pos[6] = (cl_float4){{-10, 0, 3, 0}};
	radius[6] = 1;
	color[6] = 0x00FFFF;
	// 8th
	type[7] = PLANE;
	q[7] = (cl_float4){{0, 1, 0, 0}};
	pos[7] = (cl_float4){{0, -15, 0, 0}};
	// radius[7] = 1;
	color[7] = 0x885588;
	// 9th
	type[8] = PLANE;
	q[8] = (cl_float4){{0, 0, 1, 0}};
	pos[8] = (cl_float4){{0, 0, 20, 0}};
	// radius[8] = 1;
	color[8] = 0x98BFFC;

	// Add to linked list
	object = ft_calloc(1, sizeof(t_object));
	if (!object)
		perror_and_exit("malloc", EXIT_FAILURE);
	*object = assign_object(type[0], pos[0], q[0], color[0]);
	object->sphere = assign_sphere(radius[0]);
	root_node = ft_lstnew(object);
	if (!root_node)
		perror_and_exit("malloc", EXIT_FAILURE);

	n = 0;
	while (++n < 9)
	{
		object = ft_calloc(1, sizeof(t_object));
		if (!object)
			perror_and_exit("malloc", EXIT_FAILURE);
		*object = assign_object(type[n], pos[n], q[n], color[n]);
		if (type[n] == SPHERE)
			object->sphere = assign_sphere(radius[n]);
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
		// arr_objects[n].obb = assign_sphere_obb(arr_objects[n].sphere);
		head = head->next;
	}
	ft_lstclear(&root_node, free);
	n = -1;
	while (++n < len)
	{
		print_vector(arr_objects[n].pos);
	}
	return (arr_objects);
}

int	count_objects(t_object *arr_objects)
{
	int	count;

	count = -1;
	while (arr_objects[++count].type != 0)
		;
	return (count);
}
