/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgiordan <vgiordan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/12 18:58:49 by vgiordan          #+#    #+#             */
/*   Updated: 2022/11/08 14:33:28 by vgiordan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
	if (!result)
		return (NULL);
	while (*s1)
		result[i++] = *s1++;
	while (*s2)
		result[i++] = *s2++;
	result[i] = '\0';
	return (result);
}
