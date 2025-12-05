







#include "minirt.h"



int is_valid_object_id(char *id)
{
  if (ft_strcmp(id, SPHERE))
    return 1;
  if (ft_strcmp(id, PLANE))
    return 1;
  if (ft_strcmp(id, CYLINDER))
    return 1;
  if (ft_strcmp(id, LIGHT))
    return 1;
  return 0;
}


int is_valid_camera_ambient_id(char *id)
{
  if (ft_strcmp(id, CAMERA))
    return 1;
  if (ft_strcmp(id, AMBIENT))
    return 1;
  return 0;
}


int valid_rt_file(t_list *root_node)
{
  char **str;

  while (*root_node)
  {
    str = root_node->content;
    if(ft_strcmp(str, AMBIENT) && valid_ambient(str))
      exit(EXIT_FAILURE);
    if(ft_strcmp(str, CAMERA) && valid_camera(str))
      exit(EXIT_FAILURE);
    if(ft_strcmp(str, LIGHT) && valid_light(str))
      exit(EXIT_FAILURE);
    if(ft_strcmp(str, SPHERE) && valid_sphere(str))
      exit(EXIT_FAILURE);
    if(ft_strcmp(str, PLANE) && valid_plane(str))
      exit(EXIT_FAILURE);
    if(ft_strcmp(str, CYLINDER) && valid_cylinder(str))
      exit(EXIT_FAILURE);

  }
}
