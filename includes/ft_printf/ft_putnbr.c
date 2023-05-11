/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgiordan <vgiordan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 22:39:05 by victorgiord       #+#    #+#             */
/*   Updated: 2022/11/09 10:30:07 by vgiordan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	getnbsize(int nb)
{
	int	i;

	i = 0;
	if (nb < 0)
		nb = -nb;
	while (nb > 0)
	{
		nb /= 10;
		i++;
	}
	return (i);
}

void	ft_reverse(char *tab, int size)
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

static void	process(int nb, int size)
{
	char	*result;
	int		i;
	int		isneg;

	isneg = 0;
	i = 0;
	result = malloc((size + 1) * sizeof(char));
	if (nb < 0)
	{
		nb = -nb;
		result[size - 1] = '-';
		isneg = 1;
	}
	while (nb > 0)
	{
		result[i++] = nb % 10 + '0';
		nb /= 10;
	}
	ft_reverse(result, size);
	if (isneg)
		result[i + 1] = '\0';
	else
		result[i] = '\0';
	ft_putstr(result);
	free(result);
}

int	ft_putnbr(int nb)
{
	int		size;

	if (nb == 0)
	{
		ft_printchar('0');
		return (1);
	}
	size = getnbsize(nb);
	if (nb < 0)
		size++;
	if (nb == -2147483648)
	{
		if (ft_putstr("-2147483648") == -1)
			return (-1);
		return (11);
	}
	else
		process(nb, size);
	return (size);
}
