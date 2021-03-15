/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 16:18:31 by plurlene          #+#    #+#             */
/*   Updated: 2020/11/01 20:02:54 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void		convert(char *s, int len, unsigned long num)
{
	int				i;
	unsigned long	temp;

	temp = num;
	i = len - 1;
	while (temp / 10 > 0)
	{
		s[i] = '0' + temp % 10;
		temp /= 10;
		i--;
	}
	s[i] = '0' + temp % 10;
}

char			*ft_itoa(int n)
{
	char	*res;
	long	i;
	int		sign;
	int		len;

	sign = n < 0 ? 1 : 0;
	i = n < 0 ? (unsigned long)n * -1 : n;
	len = 1;
	while (i / 10 > 0)
	{
		len++;
		i /= 10;
	}
	if (!(res = (char *)malloc(sizeof(char) * (len + sign + 1))))
		return (0);
	res[0] = n < 0 ? '-' : '0';
	res[len + sign] = '\0';
	i = n < 0 ? (unsigned long)n * -1 : n;
	convert(res, len + sign, i);
	return (res);
}
