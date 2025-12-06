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

t_object assign_object(char **line) {
  t_object object;
  float metallic;

  object.emission = (cl_float3){{1, 1, 1}};
  object.F_0 = (cl_float3){{0.1f, 0.071f, 0.029f}}; // never 0.0f
  object.roughness_sqr = 0.5e-10f;
  metallic = 0.2f;

  if (!line[1])
    perror_and_exit("Position in float3 missing", EXIT_FAILURE);
  object.pos = get_cl_float3(line[1]);


  if (ft_strcmp(*line, SPHERE))
    assign_sphere(&object, line);
  else if (ft_strcmp(*line, PLANE))
    assign_plane(&object, line);
  else if (ft_strcmp(*line, LIGHT))
    assign_light(&object, line);
  return (object);
  // object->mat_type = split[4] ? *split[4] : 'D';

  object.specular_albedo = (cl_float3){{object.diffuse_albedo.x * metallic,
                                         object.diffuse_albedo.y * metallic,
                                         object.diffuse_albedo.z * metallic}};
  object.diffuse_albedo =
      (cl_float3){{object.diffuse_albedo.x * (1.0f - metallic),
                   object.diffuse_albedo.y * (1.0f - metallic),
                   object.diffuse_albedo.z * (1.0f - metallic)}};

  return object;
}

// t_OBB assign_sphere_obb(t_sphere sphere) {
//   t_OBB obb;
//
//   // obb.pos = sphere.pos;
//   // obb.quat = sphere.quat;
//   // obb.half_len = _mm_set_ps(sphere.radius, sphere.radius, sphere.radius, 0);
//   obb.half_len = to_float3(sphere.radius);
//   return (obb);
// }

void assign_sphere(t_object *object, char **line) {
  object->obj_type = CL_SPHERE; 

  if (!line[2])
    perror_and_exit("Sphere Radius missing", EXIT_FAILURE);
  object->sphere.radius = get_cl_float(line[2]);
  
  if (!line[3])
    perror_and_exit("RGB values missing", EXIT_FAILURE);
  object->diffuse_albedo = get_rgb_value(line[3]);
}

void assign_plane(t_object *object, char **line) {
  object->obj_type = CL_PLANE;

  if (!line[2])
    perror_and_exit("Plane Direction missing", EXIT_FAILURE);
  object->dir = get_cl_float3(line[2]);
  
  if (!line[3])
    perror_and_exit("RGB values missing", EXIT_FAILURE);
  object->diffuse_albedo = get_rgb_value(line[3]);

}

void assign_light(t_object *object, char **line) {
  float light_ratio;

  object->obj_type = CL_LIGHT;
  object->emission = (cl_float3){{100.0f, 100.0f, 100.0f}};
  // object->obb = assign_sphere_obb(object->sphere);
  if (!line[2])
    perror_and_exit("Light Radius missing", EXIT_FAILURE);
  object->sphere.radius = get_cl_float(line[3]);
  
  if (!line[3])
    perror_and_exit("Light ratio missing", EXIT_FAILURE);

  light_ratio = get_cl_float(line[3]);
  if (light_ratio < 0 || light_ratio > 1)
    perror_and_exit("Light ratio must be between 0-1", EXIT_FAILURE);
  object->emission = (cl_float3){{100.0f * light_ratio, 100.0f * light_ratio, 100.0f * light_ratio}};

  if (!line[4])
    perror_and_exit("RGB values missing", EXIT_FAILURE);
  object->diffuse_albedo = get_rgb_value(line[4]);

}

t_cyclinder assign_cyclinder(cl_float radius, cl_float height) {
  t_cyclinder cyclinder;

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

// t_object *create_objects_array(t_list *root_node) {
//   t_object *arr_objects;
//   t_list *head;
//   int len;
//   int n;
//
//   len = ft_lstsize(root_node);
//   arr_objects = ft_calloc(len + 1, sizeof(t_object));
//   if (!arr_objects)
//     perror_and_exit("malloc", EXIT_FAILURE);
//   head = root_node;
//   n = -1;
//   while (++n < len) {
//     // printf("%d\n", n);
//     ft_memcpy(arr_objects + n, head->content, sizeof(t_object));
//     // arr_objects[n].obb = assign_sphere_obb(arr_objects[n].sphere);
//     head = head->next;
//   }
//   ft_lstclear(&root_node, free);
//   n = -1;
//   while (++n < len) {
//     print_vector(arr_objects[n].pos);
//   }
//   ft_lstclear(&root_node, free);
//   return (arr_objects);
// }

// int count_objects(t_object *arr_objects) {
//   int count;
//
//   count = -1;
//   while (arr_objects[++count].obj_type != 0)
//     ;
//   return (count);
//}

// t_object *get_objects(char *filename) {
//   t_list *root_node = NULL;
//   t_list *node;
//   char *line;
//   int fd;
//
//   fd = open(filename, O_RDONLY);
//   if (!fd)
//     exit(EXIT_FAILURE); // Better handling should be here
//   line = get_next_line(fd);
//   while (line) {
//     node = ft_lstnew(assign_object(line));
//     if (!root_node)
//       root_node = node;
//     else
//       ft_lstadd_back(&root_node, node);
//     free(line);
//     line = get_next_line(fd);
//   }
//   return (create_objects_array(root_node));
// }

t_list *get_rt_file(char *filename) {
  t_list *root_node = NULL;
  t_list *node;
  char **line;
  int fd;
  char *str;

  fd = open(filename, O_RDONLY);
  if (!fd)
    exit(EXIT_FAILURE); // Better handling should be here
  str = get_next_line(fd);
  while (str) {
    line = ft_split_set(str, "  \n");
    if (!line)
      perror_and_exit("malloc", EXIT_FAILURE);
    node = ft_lstnew(line);
    if (!root_node)
      root_node = node;
    else
      ft_lstadd_back(&root_node, node); 
    free(str);
    str = get_next_line(fd);
  }

  // node = root_node;
  // while (node) {
  //   printf("%s\n", (char *)node->content);
  //   node = node->next;
  // }
  // exit(0);
  return (root_node);
}


int count_objects(t_list *root_node)
{
  int object_count;
  int camera_ambient_count;
  char **line;

  object_count = 0;
  camera_ambient_count = 0;
  while (root_node)
  {
    line = (char **)root_node->content;
    if (line && *line && is_valid_object_id(*line))
      object_count++;
    else if (is_valid_camera_ambient_id(*line))
        camera_ambient_count++;
    else
     perror_and_exit("Unknown line[0] in .rt file", EXIT_FAILURE);
    root_node = root_node->next;
  }
  if (!object_count)
    perror_and_exit("No Objects in .rt file", EXIT_FAILURE);
  if (camera_ambient_count != 2)
    perror_and_exit("Given .rt file must have 1 camera and 1 ambient only", EXIT_FAILURE);
  return object_count;
}


t_object *get_objects(t_list *root_node)
{
  t_object *arr_objects;
  int num_objects;
  int n;
  char **line;
 
  num_objects = count_objects(root_node);
  arr_objects = ft_calloc(num_objects, sizeof(t_object));
  if (!arr_objects)
    perror_and_exit("Malloc fail", EXIT_FAILURE);
  n = 0;
  while (root_node)
  {
    line = (char **)root_node->content;
    if (is_valid_object_id(line[0]))
      arr_objects[n++] = assign_object(line);
    root_node = root_node->next;
  }
  return (arr_objects);
}





void assign_camera(t_camera *camera, char **line)
{
  cl_float3 dir;

  if (!line[1])
    perror_and_exit("Camera position missing", EXIT_FAILURE);
  camera->pos = get_cl_float3(line[1]);

  if (!line[2])
    perror_and_exit("Camera direction missing", EXIT_FAILURE);
  dir = get_cl_float3(line[2]);
  camera->quat = (cl_float4){{0, dir.x, dir.y, dir.z}};

  if (!line[3])
    perror_and_exit("FOV missing", EXIT_FAILURE);
  camera->fov = (int)ft_atof(line[3]) * TO_RADIAN;
}


void assign_ambient(t_camera *camera, char **line)
{
  if (!line[1])
    perror_and_exit("Ambient lighting ratio missing", EXIT_FAILURE);
  camera->amb_light_ratio = get_cl_float(line[1]);

  if (!line[2])
    perror_and_exit("Ambient fade ratio missing", EXIT_FAILURE);
  camera->amb_fade_ratio = get_cl_float(line[2]);

  if (!line[3])
    perror_and_exit("Camera top color missing", EXIT_FAILURE);
  camera->amb_top_color = get_cl_float3(line[3]);

  if (!line[4])
    perror_and_exit("Ambient bottom color missing", EXIT_FAILURE);
  camera->amb_bot_color = get_cl_float3(line[4]);

}

t_camera get_camera(t_list *root_node)
{
  t_camera camera;
  char **line;
  int camera_ambient_count[2];

  ft_bzero(camera_ambient_count, sizeof(int) * 2);
  while (root_node)
  {
    line = root_node->content;
    if (ft_strcmp(*line, CAMERA))
    {
      assign_camera(&camera, line);
      camera_ambient_count[0]++;
    }
    else if (ft_strcmp(*line, AMBIENT))
    {
      assign_ambient(&camera, line);
      camera_ambient_count[1]++;
    }
    root_node = root_node->next;
  }
  if (camera_ambient_count[0] + camera_ambient_count[1] != 2 || camera_ambient_count[0] != 1)
    perror_and_exit("Must have only 1 Camera and Ambient" ,EXIT_FAILURE);
  return camera;
}











