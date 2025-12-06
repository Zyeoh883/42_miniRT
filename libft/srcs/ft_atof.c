






#include "libft.h"

int	ft_atof(const char *str)
{
	float	result;
  float decimal;

  result = (float)ft_atol(str);

  while (*str && *str != '.')
    str++;

  if (*str)
  {
    str++;
    decimal = (float)ft_atol(str);
    while (decimal > 0)
      decimal /= 10;

    if (result < 0)
      result -= decimal;
    else
      result += decimal;
  }
	return (result);
}
