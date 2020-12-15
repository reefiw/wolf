/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/26 15:35:41 by plurlene          #+#    #+#             */
/*   Updated: 2020/12/08 16:16:16 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	del(char **reliquo)
{
	if (*reliquo)
		free(*reliquo);
	return (1);
}

static int	getres(char **reliquo, char **line, int *rs)
{
	int		end_i;
	char	*out;

	out = NULL;
	end_i = e_l_i(*reliquo) < 0 ? ft_strlen(*reliquo) : e_l_i(*reliquo);
	if (e_l_i(*reliquo) < 0)
	{
		if (!(*line = ft_strdup(*reliquo, 0)) && del(reliquo))
			return (1);
		del(reliquo);
		*reliquo = NULL;
		*rs = 0;
	}
	else
	{
		(*reliquo)[end_i] = '\0';
		*line = end_i != 0 ? ft_strdup(*reliquo, 0) : ft_strjoin("\0", "\0", 0);
		if ((*rs = end_i + 1) && !(*line) && del(reliquo))
			return (1);
	}
	if (*reliquo && !(out = ft_strdup(&((*reliquo)[*rs]), 0)) && del(reliquo))
		return (1);
	del(reliquo);
	*reliquo = out;
	return (0);
}

static int	read_descriptor(char **reliquo, char **buf, int *rs, int fd)
{
	while (e_l_i(reliquo[fd]) < 0 && (*rs = read(fd, *buf, BS)))
	{
		if (*rs < 0)
			return (1);
		(*buf)[*rs] = '\0';
		if (!(reliquo[fd] = ft_strjoin(reliquo[fd], *buf, 1)) && del(reliquo))
			return (1);
	}
	return (0);
}

static int	validate(char **line, int fd, char **buf, char **reliquo)
{
	if (BS <= 0 || !line || fd < 0 || (!(*buf = (char *)malloc(BS + 1))))
		return (1);
	if (!(reliquo)[fd] && !(reliquo[fd] = ft_strdup("", 0)))
		return (1);
	return (0);
}

int			get_next_line(int fd, char **line)
{
	static	char	*reliquo[OPEN_MAX];
	char			*buf;
	int				rs;

	if (validate(line, fd, &buf, reliquo))
		return (-1);
	if (!(rs = 0) && e_l_i(reliquo[fd]) > 0)
	{
		if (getres(&reliquo[fd], line, &rs))
			return (-1);
	}
	else
	{
		if (read_descriptor(reliquo, &buf, &rs, fd))
			return (-1);
		if (getres(&reliquo[fd], line, &rs))
			return (-1);
	}
	free(buf);
	rs = !rs ? 0 : 1;
	return (rs);
}
