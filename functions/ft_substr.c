/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 13:49:06 by plurlene          #+#    #+#             */
/*   Updated: 2021/02/11 16:25:53 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*res;
	unsigned int	i;

	if (s[0] == '\0')
		return (ft_strdup(s));
	if (!(res = (char *)ft_calloc(sizeof(char), (len + 1))) || \
	!s)
		return (0);
	i = start;
	while (i - start < len && s[i] != '\0')
	{
		res[i - start] = s[i];
		i++;
	}
	res[len] = '\0';
	return (res);
}
