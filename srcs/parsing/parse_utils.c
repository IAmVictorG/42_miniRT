#include "../../includes/miniRT.h"

void go_to_next_arg(char **line)
{
	while (**line && (!is_space(**line)))
	{
		(*line)++;
	}
	while (**line && (is_space(**line)))
	{
		(*line)++;
	}
}

char *get_arg(char *line)
{
	int i;

	i = 0;
	while (*line && is_space(*line))
	{
		line++;;
	}
	while (line[i] && !is_space(line[i]))
	{
		i++;
	}
	return (ft_substr(line, 0, i));
}

void set_rgb(char *str, t_color *color)
{
	int i;
	int	start;
	char *r;

	start = 0;
	i = 0;
	while (str[i] && str[i] != ',')
		i++;
	r = ft_substr(str, start, i);	
	color->r = atoi(r);
	free(r);
	start = ++i;
	while (str[i] && str[i] != ',')
		i++;
	r = ft_substr(str, start, i - start);	
	color->g = atoi(r);
	free(r);
	start = ++i;
	while (str[i] && str[i] != ',')
		i++;
	r = ft_substr(str, start, i - start);	
	color->b = atoi(r);
	free(r);
}

void	set_vector(char *str, t_vec3 *vec)
{
	int i;
	int	start;
	char *r;

	start = 0;
	i = 0;
	while (str[i] && str[i] != ',')
		i++;
	r = ft_substr(str, start, i);	
	vec->x = atof(r);
	free(r);
	start = ++i;
	while (str[i] && str[i] != ',')
		i++;
	r = ft_substr(str, start, i - start);	
	vec->y  = atof(r);
	free(r);
	start = ++i;
	while (str[i] && str[i] != ',')
		i++;
	r = ft_substr(str, start, i - start);	
	vec->z  = atof(r);
	free(r);
}