





#include "minirt.h"



int is_valid_object_id(char *id)
{
  if (!ft_strcmp(id, SPHERE))
    return 1;
  if (!ft_strcmp(id, PLANE))
    return 1;
  if (!ft_strcmp(id, CYLINDER))
    return 1;
  if (!ft_strcmp(id, LIGHT))
    return 1;
  return 0;
}


int is_valid_camera_ambient_id(char *id)
{
  if (!ft_strcmp(id, CAMERA))
    return 1;
  if (!ft_strcmp(id, AMBIENT))
    return 1;
  return 0;
}

int is_rgb(char *str)
{
  int num;
  char *temp;

  // printf("rgb : %s\n", str);
  if (!str || ft_strlen(str) > 3)
    return 0;

  temp = str;
  while (ft_isdigit(*str))
    str++;

  num = ft_atoi(temp);
  if (num > 256 || num < 0)
    return 0;
  if (*str)
    return 0;
  return 1;
}


cl_float get_cl_float(char *str)
{
  char *temp;

  if (!str)
    error_and_exit("Float missing", EXIT_FAILURE);
  if(ft_strlen(str) > LONG_NUM_DIGITS + 1)
    error_and_exit("Float digits must be less than 19 digits", EXIT_FAILURE);  
  temp = str;
  if (*temp == '-' || *temp == '+')
    temp++;
  while (ft_isdigit(*temp))
    temp++;
  if (*temp++ != '.')
    error_and_exit("Period missing", EXIT_FAILURE);
  if (!ft_isdigit(*temp))
    error_and_exit("unknown character after period .", EXIT_FAILURE);
  while (ft_isdigit(*temp))
    temp++;
  if (*temp)
    error_and_exit("Trailing characters after float", EXIT_FAILURE);

  printf("atof : %s | %f\n", str, ft_atof(str));
  return ft_atof(str);
}


cl_float3 get_cl_float3(char *str)
{
  cl_float3 result;
  char **split;


  if (!str)
    error_and_exit("Float3 missing", EXIT_FAILURE);
  split = ft_split(str, ',');
  if (!split)
    error_and_exit("malloc", EXIT_FAILURE);
  if (ft_str_arr_len(split) != 3)
    error_and_exit("Must be 3 Floats", EXIT_FAILURE);
  
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
    error_and_exit("RGB value missing", EXIT_FAILURE);
 
  split = ft_split(str, ',');
  if (!split)
    error_and_exit("malloc", EXIT_FAILURE);
  if (ft_str_arr_len(split) != 3)
    error_and_exit("Must be 3 Integers", EXIT_FAILURE);

  n = -1;
  while (++n < 3)
  {
    if (!is_rgb(split[n]))
      error_and_exit("Integer must be between 0-256", EXIT_FAILURE);
    num[n] = (cl_float)ft_atol(split[n]);
    num[n] = num[n] / 256;
    // printf("%f\n", num[n]);
  }
  result = (cl_float3){{num[0], num[1], num[2]}};
  free_str_arr(split);
  return(result);  
}

