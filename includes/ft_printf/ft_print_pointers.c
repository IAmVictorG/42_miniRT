/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_pointers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgiordan <vgiordan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 13:24:53 by vgiordan          #+#    #+#             */
/*   Updated: 2022/11/09 10:30:06 by vgiordan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_ptr_len(unsigned long long nb)
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

static int	ft_print_ptr_process(unsigned long long nb, char *base)
{
	if (nb > 0)
	{
		ft_print_ptr_process(nb / 16, base);
		if (ft_printchar(base[nb % 16]) == -1)
			return (-1);
	}
	return (ft_ptr_len(nb));
}

int	ft_print_ptr(unsigned long long nb, char *base)
{
	if (nb == 0)
		return (ft_printchar('0'));
	return (ft_print_ptr_process(nb, base));
}

int	ft_print_pointer(unsigned long long pt)
{
	if (pt == 0)
		return (ft_putstr("0x") + ft_printchar('0'));
	return (ft_putstr("0x") + ft_print_ptr(pt, BASEHEXAMIN));
}
