/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 14:23:09 by plurlene          #+#    #+#             */
/*   Updated: 2020/11/07 18:01:34 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static int	ft_substr_num(char const *s, char c)
{
	int		i;
	int		substr_num;
	int		flag;

	i = -1;
	flag = 0;
	substr_num = 0;
	while (s[++i])
	{
		if (s[i] != c && flag == 0)
			flag = 1;
		if (s[i] != c && flag == 1)
		{
			substr_num++;
			flag = 2;
		}
		if (s[i] == c)
			flag = 0;
	}
	return (substr_num);
}

static int	ft_substr_length(char const *s, char c)
{
	int		i;

	i = 0;
	while (s[i] != c && s[i] != '\0')
		i++;
	return (i);
}

static void	ft_free_arr(char **arr, int i)
{
	while (i >= 0)
	{
		if (arr[i])
			free(arr[i--]);
	}
	if (arr)
		free(arr);
}

static char	**ft_main_code(char **res, char const *s, char c)
{
	int i;
	int flag;
	int j;

	flag = 0;
	j = 0;
	i = -1;
	while (s[++i] != '\0')
	{
		if (s[i] != c && flag == 0)
			flag = 1;
		if (s[i] != c && flag == 1)
		{
			flag = 2;
			if (!(res[j] = ft_substr(&s[i], 0, ft_substr_length(&s[i], c))))
			{
				ft_free_arr(res, j - 1);
				return (0);
			}
			j++;
		}
		if (s[i] == c)
			flag = 0;
	}
	return (res);
}

char		**ft_split(char const *s, char c)
{
	char	**res;

	if (!s)
		return (0);
	if (!(res = (char **)malloc(sizeof(char *) * (ft_substr_num(s, c) + 1))))
		return (0);
	res[ft_substr_num(s, c)] = 0;
	res = ft_main_code(res, s, c);
	return (res);
}
