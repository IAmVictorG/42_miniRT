/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgiordan <vgiordan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 00:41:03 by victorgiord       #+#    #+#             */
/*   Updated: 2022/11/11 14:08:09 by vgiordan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		s_s1;
	int		s_s2;
	int		i;
	char	*result;

	i = 0;
	s_s1 = ft_strlen(s1);
	s_s2 = ft_strlen(s2);
	result = malloc((s_s1 + s_s2 + 1) * sizeof(char));
	if (!result || !s1 || !s2)
		return (NULL);
	while (*s1)
		result[i++] = *s1++;
	while (*s2)
		result[i++] = *s2++;
	result[i] = '\0';
	return (result);
}

char	*ft_strnjoin(char const *s1, char const *s2, int n)
{
	int		s_s1;
	int		s_s2;
	int		i;
	char	*result;

	i = 0;
	s_s1 = ft_strlen(s1);
	s_s2 = ft_strlen(s2);
	result = malloc((s_s1 + s_s2 + 1) * sizeof(char));
	if (!result || !s1 || !s2)
		return (NULL);
	while (*s1)
		result[i++] = *s1++;
	while (*s2 && n-- > 0)
		result[i++] = *s2++;
	result[i] = '\0';
	return (result);
}
