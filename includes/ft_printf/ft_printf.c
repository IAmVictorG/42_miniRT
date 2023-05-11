/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgiordan <vgiordan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 13:09:00 by victorgiord       #+#    #+#             */
/*   Updated: 2022/11/09 10:29:56 by vgiordan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	formats(const char format, va_list args)
{
	int	size;

	if (format == 's')
		size = ft_putstr(va_arg(args, char *));
	else if (format == 'c')
		size = ft_printchar(va_arg(args, int));
	else if (format == 'd' || format == 'i')
		size = ft_putnbr(va_arg(args, int));
	else if (format == 'x')
		size = ft_print_hexa(va_arg(args, unsigned int), BASEHEXAMIN);
	else if (format == 'X')
		size = ft_print_hexa(va_arg(args, unsigned int), BASEHEXAMAJ);
	else if (format == 'u')
		size = ft_print_unsigned_int(va_arg(args, unsigned int));
	else if (format == 'p')
		size = ft_print_pointer(va_arg(args, unsigned long long));
	else if (format == '%')
		size = ft_printchar('%');
	else
		size = ft_printchar(format);
	return (size);
}

int	ft_printf(const char *s, ...)
{
	va_list	args;
	int		len;
	int		temp;

	va_start(args, s);
	len = 0;
	while (*s)
	{
		if (*s == '%')
			temp = formats(*(++s), args);
		else
			temp = ft_printchar(*s);
		if (temp == -1)
			return (-1);
		len += temp;
		s++;
	}
	va_end(args);
	return (len);
}

/*int	main(int argc, char const *argv[])
{
	char	*c;

	(void) argc;
	(void) argv;
	c = "ATAT";
	printf("Me : \n");
	printf("	%d\n", ft_printf("%d", 012));
	printf("\nExpected : \n");
	printf("	%d\n", printf("%d", 012));
	return (0);
}
*/