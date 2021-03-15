/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 14:57:39 by plurlene          #+#    #+#             */
/*   Updated: 2020/11/07 16:39:14 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char			*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	char	*res;
	int		len;

	if (!s1 || !s2)
		return (0);
	len = ft_strlen((char *)s1) + ft_strlen((char *)s2);
	if (!(res = (char *)malloc(sizeof(char) * (len + 1))))
		return (0);
	i = 0;
	while (i < (int)ft_strlen((char *)s1))
	{
		res[i] = s1[i];
		i++;
	}
	while (i - ft_strlen((char *)s1) < ft_strlen((char *)s2))
	{
		res[i] = s2[i - ft_strlen((char *)s1)];
		i++;
	}
	if (res[i] != '\0')
		res[i] = '\0';
	return (res);
}
