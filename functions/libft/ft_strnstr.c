/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 11:17:13 by plurlene          #+#    #+#             */
/*   Updated: 2020/11/03 20:18:55 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*cycles(char *str, char *to_find, size_t i, size_t n)
{
	size_t j;

	j = 0;
	while (str[i] != '\0')
	{
		if (str[i] == to_find[j])
		{
			while (to_find[j] != '\0')
			{
				if (str[i + j] == to_find[j] && i + j < n)
				{
					j++;
					if (to_find[j] == '\0')
						return (&str[i]);
				}
				else
				{
					j = 0;
					break ;
				}
			}
		}
		i++;
	}
	return (0);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	char *r;

	if (!needle[0])
		return ((char *)haystack);
	r = cycles((char *)haystack, (char *)needle, 0, len);
	return (r);
}
