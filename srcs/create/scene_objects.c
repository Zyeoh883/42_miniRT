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
// char type, cl_float3 pos, cl_float4 quat, int color)

t_object	*assign_object(char *line)
{
  t_object	*object;
  float metallic;
  char **split;

  object = ft_calloc(1, sizeof(t_object));
  split = ft_split(line, ',');
  if (!object || !split)
    perror_and_exit("malloc", EXIT_FAILURE);
  object->emission = (cl_float3){{0.1f, 0.1f, 0.1f}};
  if (**split == SPHERE)
    assign_sphere(object, split);
  else if (**split == PLANE)
    assign_plane(object, split);
  else if (**split == LIGHT)
    assign_light(object, split);

	object->obj_type = *split[0];
  object->color = get_rgb_value(split[1]);
  object->diffuse_albedo = inv_rgb_float(object->color);
  // object->specular_albedo = (cl_float3){{0.5f, 0.5f, 0.5f}};
  // printf("%c %x %f %f %f\n",object->obj_type, object->color, object->albedo.x, object->albedo.y, object->albedo.z);
	object->pos = get_vec_value(split[2]);
  // object->mat_type = split[4] ? *split[4] : 'D';
  object->F_0 = (cl_float3){{0.2, 0.2, 0.2}}; // never 0.0f 
  metallic = 0.2f;
  object->specular_albedo = (cl_float3){{ object->diffuse_albedo.x * metallic,
                        object->diffuse_albedo.y *  metallic,
                        object->diffuse_albedo.z *  metallic }};
  object->diffuse_albedo = (cl_float3){{ object->diffuse_albedo.x * (1 - metallic),
                        object->diffuse_albedo.y * (1 - metallic),
                        object->diffuse_albedo.z * (1 - metallic)}}; 
  object->roughness_sqr = 0.5e-1f;
  // object->emission = (cl_float3){{0.05f, 0.05f, 0.05f}};
  free_str_arr(split);
	return (object);
}

t_OBB	assign_sphere_obb(t_sphere sphere)
{
	t_OBB	obb;

	// obb.pos = sphere.pos;
	// obb.quat = sphere.quat;
	// obb.half_len = _mm_set_ps(sphere.radius, sphere.radius, sphere.radius, 0);
	obb.half_len = to_float3(sphere.radius);
  return (obb);
}

void	assign_sphere(t_object *object, char **split)
{
  object->obj_type = SPHERE;
  object->sphere.radius =  ft_atoi(split[3]);
  // object->obb = assign_sphere_obb(object->sphere);
}

void	assign_plane(t_object *object, char **split)
{
  object->obj_type = PLANE;
  object->dir = get_vec_value(split[3]);
}

void	assign_light(t_object *object, char **split)
{
  object->obj_type = LIGHT;
  object->sphere.radius =  ft_atoi(split[3]);
  object->emission = (cl_float3){{2.0f, 2.0f, 2.0f}};
  // object->obb = assign_sphere_obb(object->sphere);
}

t_cyclinder	assign_cyclinder(cl_float radius, cl_float height)
{
	t_cyclinder	cyclinder;

	cyclinder.radius = radius;
	cyclinder.height = height;
	return (cyclinder);
}

// t_list	*create_ll_objects(void) // !parsing to execution starts here
// {
// 	t_list *root_node;
// 	t_list *node;
// 	t_object *object;
// 	int n;
//
// 	cl_float4 q[9];
// 	cl_float4 pos[9];
// 	cl_char type[9];
// 	int radius[9];
// 	int color[9];
//
// 	// 1st
// 	type[0] = SPHERE;
// 	q[0] = (cl_float4){{0, 0, 0, 1}};
// 	pos[0] = (cl_float4){{0, 0, 3, 0}};
// 	radius[0] = 1;
// 	color[0] = 0xFFFFFF;
// 	// 2nd,
// 	type[1] = SPHERE;
// 	q[1] = (cl_float4){{0, 0, 0, 1}};
// 	pos[1] = (cl_float4){{0, -10, 3, 0}};
// 	radius[1] = 1;
// 	color[1] = 0xFF0000;
// 	// 3rd
// 	type[2] = SPHERE;
// 	q[2] = (cl_float4){{0, 0, 0, 1}};
// 	pos[2] = (cl_float4){{0, 0, 13, 0}};
// 	radius[2] = 1;
// 	color[2] = 0x00FF00;
// 	// 4th
// 	type[3] = SPHERE;
// 	q[3] = (cl_float4){{0, 0, 0, 1}};
// 	pos[3] = (cl_float4){{0, 10, 3, 0}};
// 	radius[3] = 1;
// 	color[3] = 0x0000FF;
// 	// 5th
// 	type[4] = SPHERE;
// 	q[4] = (cl_float4){{0, 0, 0, 1}};
// 	pos[4] = (cl_float4){{0, 0, -7, 0}};
// 	radius[4] = 1;
// 	color[4] = 0xFF00FF;
// 	// 6th
// 	type[5] = SPHERE;
// 	q[5] = (cl_float4){{0, 0, 0, 1}};
// 	pos[5] = (cl_float4){{10, 0, 3, 0}};
// 	radius[5] = 1;
// 	color[5] = 0xFFFF00;
// 	// 7th
// 	type[6] = SPHERE;
// 	q[6] = (cl_float4){{0, 0, 0, 1}};
// 	pos[6] = (cl_float4){{-10, 0, 3, 0}};
// 	radius[6] = 1;
// 	color[6] = 0x00FFFF;
// 	// 8th
// 	type[7] = PLANE;
// 	q[7] = (cl_float4){{0, 1, 0, 0}};
// 	pos[7] = (cl_float4){{0, -15, 0, 0}};
// 	// radius[7] = 1;
// 	color[7] = 0x885588;
// 	// 9th
// 	type[8] = PLANE;
// 	q[8] = (cl_float4){{0, 0, 1, 0}};
// 	pos[8] = (cl_float4){{0, 0, 20, 0}};
// 	// radius[8] = 1;
// 	color[8] = 0x98BFFC;
//
// 	// Add to linked list
// 	object = ft_calloc(1, sizeof(t_object));
// 	if (!object)
// 		perror_and_exit("malloc", EXIT_FAILURE);
// 	*object = assign_object(type[0], pos[0], q[0], color[0]);
// 	object->sphere = assign_sphere(radius[0]);
// 	root_node = ft_lstnew(object);
// 	if (!root_node)
// 		perror_and_exit("malloc", EXIT_FAILURE);
//
// 	n = 0;
// 	while (++n < 9)
// 	{
// 		object = ft_calloc(1, sizeof(t_object));
// 		if (!object)
// 			perror_and_exit("malloc", EXIT_FAILURE);
// 		*object = assign_object(type[n], pos[n], q[n], color[n]);
// 		if (type[n] == SPHERE)
// 			object->sphere = assign_sphere(radius[n]);
// 		node = ft_lstnew(object);
// 		if (!node)
// 			perror_and_exit("malloc", EXIT_FAILURE);
// 		ft_lstadd_back(&root_node, node);
// 	}
// 	return (root_node);
// }


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
  ft_lstclear(&root_node, free);
	return (arr_objects);
}

int	count_objects(t_object *arr_objects)
{
	int	count;

	count = -1;
	while (arr_objects[++count].obj_type != 0)
		;
	return (count);
}

t_object *get_objects(char *filename)
{
  t_list *root_node = NULL;
  t_list *node;
  char *line;
  int fd;

  fd = open(filename, O_RDONLY);
  if (!fd)
    exit(EXIT_FAILURE); // Better handling should be here
  line = get_next_line(fd);
  while (line)
  {
    node = ft_lstnew(assign_object(line));
    if (!root_node)
      root_node = node;
    else
      ft_lstadd_back(&root_node, node);
    free(line);
    line = get_next_line(fd);
  }
  return (create_objects_array(root_node));
}
