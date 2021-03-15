/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 15:35:46 by plurlene          #+#    #+#             */
/*   Updated: 2021/02/15 19:48:40 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		e_l_i(char *str)
{
	int i;

	if (!str)
		return (-1);
	i = -1;
	while (str[++i])
		if (str[i] == '\n')
			return (i);
	return (-1);
}

int		ft_strlen1(char *str)
{
	int i;

	i = -1;
	while (str[++i])
		;
	return (i);
}

char	*ft_strjoin1(char *s1, char *s2, int bfree)
{
	int		i;
	char	*res;
	int		len;
	int		len2;

	if (!s1 || !s2)
		return (0);
	len = ft_strlen1(s1) + ft_strlen1(s2);
	if (!(res = (char *)malloc(sizeof(char) * (len + 1))))
		return (0);
	i = -1;
	len = ft_strlen1(s1);
	while (++i < len)
		res[i] = s1[i];
	len2 = ft_strlen1(s2);
	while (s2[i - len] && i - len < len2)
	{
		res[i] = s2[i - len];
		i++;
	}
	if (bfree)
		free(s1);
	res[i] = '\0';
	return (res);
}

char	*ft_strdup1(char *s1, int bfree)
{
	int		i;
	int		len;
	char	*res;

	len = ft_strlen1(s1);
	if (!(res = (char *)malloc(sizeof(char) * (len + 1))))
		return (0);
	res[len] = '\0';
	i = 0;
	while (i < len)
	{
		res[i] = s1[i];
		i++;
	}
	if (bfree)
		free(s1);
	return (res);
}
