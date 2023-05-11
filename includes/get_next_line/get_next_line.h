/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgiordan <vgiordan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/05 13:19:30 by victorgiord       #+#    #+#             */
/*   Updated: 2022/11/09 14:21:27 by vgiordan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdio.h>
# include <strings.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>

# ifndef BUFFER_SIZE
#  define  BUFFER_SIZE 4
# endif

char	*get_next_line(int fd);
char	*add_buffer_to_line(char *line, int fd);
char	*ft_strnjoin(char const *s1, char const *s2, int n);
char	*ft_strjoin(char const *s1, char const *s2);

#endif