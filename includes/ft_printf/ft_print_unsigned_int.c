/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsigned_int.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgiordan <vgiordan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/04 13:46:05 by vgiordan          #+#    #+#             */
/*   Updated: 2022/11/09 10:31:41 by vgiordan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	getnbsizeu(unsigned int nb)
{
	int	i;

	i = 0;
	while (nb > 0)
	{
		nb /= 10;
		i++;
	}
	return (i);
}

void	ft_reverseu(char *tab, int size)
{
	int		i;
	char	temp;

	i = 0;
	while (i < size / 2)
	{
		temp = tab[i];
		tab[i] = tab[size - i - 1];
		tab[size - i - 1] = temp;
		i++;
	}
}

void	processu(unsigned int nb, int size)
{
	char	*result;
	int		i;
	int		isneg;

	isneg = 0;
	i = 0;
	result = malloc((size + 1) * sizeof(char));
	while (nb > 0)
	{
		result[i++] = nb % 10 + '0';
		nb /= 10;
	}
	ft_reverseu(result, size);
	if (isneg)
		result[i + 1] = '\0';
	else
		result[i] = '\0';
	ft_putstr(result);
	free(result);
}

int	ft_print_unsigned_int(unsigned int nb)
{
	int		size;

	if (nb == 0)
	{
		ft_printchar('0');
		return (1);
	}
	size = getnbsizeu(nb);
	processu(nb, size);
	return (size);
}
