/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgiordan <vgiordan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 14:05:19 by victorgiord       #+#    #+#             */
/*   Updated: 2022/11/09 10:30:06 by vgiordan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdio.h>
# include <stdarg.h>
# include <unistd.h>
# include <strings.h>
# include <stdlib.h>

# ifndef BASEHEXAMIN
#  define BASEHEXAMIN "0123456789abcdef"
# endif
# ifndef BASEHEXAMAJ
#  define BASEHEXAMAJ "0123456789ABCDEF"
# endif

int		ft_printf(const char *s, ...);
int		ft_putstr(char *str);
int		ft_putnbr(int nbr);
int		ft_printchar(char c);
void	ft_reverse(char *tab, int size);
int		ft_print_hexa(unsigned int nb, char *base);
int		ft_print_pointer(unsigned long long pt);
int		ft_print_unsigned_int(unsigned int nb);

#endif