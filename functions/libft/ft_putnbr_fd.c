/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 17:56:41 by plurlene          #+#    #+#             */
/*   Updated: 2020/11/01 19:01:03 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void		recurs(long n, int fd)
{
	char c;

	if (n >= 10)
	{
		recurs(n / 10, fd);
		c = '0' + n % 10;
		write(fd, &c, 1);
	}
	else
	{
		c = '0' + n % 10;
		write(fd, &c, 1);
	}
}

void			ft_putnbr_fd(int n, int fd)
{
	long b;

	b = n;
	if (n == 0)
		write(fd, "0", 1);
	else
	{
		if (n < 0)
		{
			b *= -1;
			write(fd, "-", 1);
		}
		recurs(b, fd);
	}
}
