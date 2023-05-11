/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgiordan <vgiordan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/24 12:15:31 by vgiordan          #+#    #+#             */
/*   Updated: 2022/11/08 14:33:45 by vgiordan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_in_charset(char const c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;
	int		k;
	char	*result;

	i = 0;
	j = 0;
	k = 0;
	if (!ft_strlen(s1))
		return (ft_strdup(""));
	while (is_in_charset(s1[i], set) == 1)
		i++;
	while (is_in_charset(s1[(int)ft_strlen(s1) - j - 1], set) == 1)
		j++;
	if ((int)ft_strlen(s1) - i - j + 1 <= 1)
		result = malloc(1 * sizeof(char));
	else
		result = malloc(((int)ft_strlen(s1) - i - j + 1) * sizeof(char));
	if (!result)
		return (NULL);
	while (i < (int)ft_strlen(s1) - j)
		result[k++] = s1[i++];
	result[k] = '\0';
	return (result);
}
