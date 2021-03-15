/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/28 13:57:43 by plurlene          #+#    #+#             */
/*   Updated: 2020/11/03 20:12:07 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t dst_length;
	size_t i;

	dst_length = 0;
	i = 0;
	if (!dstsize)
		return (ft_strlen(src) + dstsize);
	while (dst[dst_length] && dst_length < dstsize)
		dst_length++;
	while (src[i] && dst_length + i < dstsize - 1)
	{
		dst[dst_length + i] = src[i];
		i++;
	}
	if (i != 0)
		dst[dst_length + i] = '\0';
	return (ft_strlen(src) + dst_length);
}
