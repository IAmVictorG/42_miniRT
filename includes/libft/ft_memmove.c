/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgiordan <vgiordan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 15:11:38 by marvin            #+#    #+#             */
/*   Updated: 2022/11/08 14:07:25 by vgiordan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t n)
{
	char	*char_src;
	char	*char_dst;

	char_src = (char *)src;
	char_dst = (char *)dst;
	if (dst == NULL && src == NULL)
		return (NULL);
	if (dst > src && (src + n) > dst)
		while (n--)
			*(char_dst + n) = *(char_src + n);
	else
	{
		while (n--)
		{
			*(char_dst++) = *(char_src++);
		}
	}
	return (dst);
}
