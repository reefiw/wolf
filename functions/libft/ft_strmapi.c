/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 17:40:18 by plurlene          #+#    #+#             */
/*   Updated: 2020/11/07 17:17:55 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	int		i;
	char	*res;

	i = -1;
	if (!s)
		return (0);
	while (s[++i])
		;
	if (!(res = (char *)malloc(sizeof(char) * i + 1)))
		return (0);
	res[i] = '\0';
	i = -1;
	while (s[++i])
		res[i] = f(i, s[i]);
	return (res);
}
