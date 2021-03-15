/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/30 15:33:23 by plurlene          #+#    #+#             */
/*   Updated: 2020/11/07 19:17:07 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static int	check_set(char c, char const *set)
{
	int i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static int	check_start(char const *s, char const *set)
{
	int i;

	i = 0;
	while (check_set(s[i], set) && s[i])
		i++;
	return (i);
}

static int	check_end(char const *s, char const *set)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	i--;
	while (i >= 0 && check_set(s[i], set))
		i--;
	return (i);
}

char		*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	int		i_start;
	int		i_end;
	int		i;

	if (!s1)
		return (0);
	i_start = check_start(s1, set);
	i_end = check_end(s1, set);
	if (i_end - i_start < 0)
		return (ft_strdup(""));
	if (!(res = (char *)malloc(sizeof(char) * (i_end - i_start + 2))))
		return (0);
	i = 0;
	while (i_start <= i_end)
	{
		res[i] = s1[i_start];
		i++;
		i_start++;
	}
	if (res[i] != '\0')
		res[i] = '\0';
	return (res);
}
