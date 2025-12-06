





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

cl_float get_cl_float(char *str)
{
  char **split;

  if (!str)
    perror_and_exit("Float missing", EXIT_FAILURE);
  
  split = ft_split_set(str, BASE_10_DIGITS);
  if (!split)
    perror_and_exit("malloc", EXIT_FAILURE);

  if (ft_str_arr_len(split) != 1 && ft_strcmp(*split, ".") != 0)
    perror_and_exit("Not in float format. One period and numbers only", EXIT_FAILURE);
  free_str_arr(split);

  if(ft_strlen(str) > LONG_NUM_DIGITS + 1)
    perror_and_exit("Float digits must be less than 19 digits", EXIT_FAILURE);
  return ft_atof(str);
}


cl_float3 get_cl_float3(char *str)
{
  cl_float3 result;
  char **split;


  if (!str)
    perror_and_exit("Float3 missing", EXIT_FAILURE);
  split = ft_split(str, ',');
  if (!split)
    perror_and_exit("malloc", EXIT_FAILURE);
  if (ft_str_arr_len(split) != 3)
    perror_and_exit("Must be 3 Floats", EXIT_FAILURE);
  
  result.x = get_cl_float(split[0]);
  result.y = get_cl_float(split[1]);
  result.z = get_cl_float(split[2]);
  // result = (cl_float3)(get_cl_float(split[0]), get_cl_float(split[1]), get_cl_float(split[2]));
  free_str_arr(split);
  return result;
}

cl_float3 get_rgb_value(char *str)
{
  cl_float3 result;
  char **split;
  float num[3];
  int n;

  if (!str)
    perror_and_exit("RGB value missing", EXIT_FAILURE);
 
  split = ft_split(str, ',');
  if (!split)
    perror_and_exit("malloc", EXIT_FAILURE);
  if (ft_str_arr_len(split) != 3)
    perror_and_exit("Must be 3 Integers", EXIT_FAILURE);

  n = -1;
  while (++n < 3)
  {
    num[n] = (float)ft_atol(split[n]);
    if (num[n] < 0 || num[n] > 256)
      perror_and_exit("Integer must be between 0-256", EXIT_FAILURE);
    num[n] = (cl_float)(num[n]) / 256;
  }
  result = (cl_float3){{num[0], num[1], num[2]}};
  free_str_arr(split);
  return(result);  
}

