/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 15:35:49 by plurlene          #+#    #+#             */
/*   Updated: 2020/12/08 17:54:12 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H

# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <limits.h>

int		get_next_line(int fd, char **line);
int		ft_strlen(char *str);
int		e_l_i(char *str);
char	*ft_strjoin(char *s1, char *s2, int bfree);
char	*ft_strdup(char *s1, int bfree);

#endif

#ifndef BUFFER_SIZE

# define BUFFER_SIZE 1

#endif

#ifndef BS

# define BS BUFFER_SIZE

#endif

#ifndef OPEN_MAX

# define OPEN_MAX 1024

#endif
