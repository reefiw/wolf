/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 16:40:53 by plurlene          #+#    #+#             */
/*   Updated: 2020/11/01 19:02:25 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int i;
	int k;

	i = 0;
	k = -1;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			k = i;
		i++;
	}
	if ((char)c == '\0')
		return (&((char *)s)[i]);
	if (k != -1)
		return (&((char *)s)[k]);
	return (0);
}
