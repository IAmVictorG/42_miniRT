/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgiordan <vgiordan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 16:28:40 by marvin            #+#    #+#             */
/*   Updated: 2022/11/08 14:07:25 by vgiordan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*char_s1;
	unsigned char	*char_s2;
	int				char_compare;

	char_s1 = (unsigned char *)s1;
	char_s2 = (unsigned char *)s2;
	char_compare = 0;
	if (s1 == s2)
		return (char_compare);
	while (n)
	{
		if (*char_s1 != *char_s2)
		{
			char_compare = *char_s1 - *char_s2;
			break ;
		}
		n--;
		(void)*char_s1++;
		(void)*char_s2++;
	}
	return (char_compare);
}
