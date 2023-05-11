/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printhexa.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgiordan <vgiordan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 09:46:08 by vgiordan          #+#    #+#             */
/*   Updated: 2022/11/09 11:52:03 by vgiordan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_hex_len(unsigned int nb)
{
	int	len;

	len = 0;
	while (nb > 0)
	{
		len++;
		nb = nb / 16;
	}
	return (len);
}

static int	ft_print_hexa_process(unsigned int nb, char *base)
{
	if (nb > 0)
	{
		ft_print_hexa_process(nb / 16, base);
		if (ft_printchar(base[nb % 16]) == -1)
			exit(-1);
	}
	return (ft_hex_len(nb));
}

int	ft_print_hexa(unsigned int nb, char *base)
{
	if (nb == 0)
		return (ft_printchar('0'));
	return (ft_print_hexa_process(nb, base));
}
