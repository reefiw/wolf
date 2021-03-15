/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   screenshot.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/09 14:02:53 by plurlene          #+#    #+#             */
/*   Updated: 2021/03/15 17:14:59 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

static void			get_head_bmp(int fd, t_vars *vars)
{
	unsigned char	header[54];
	int				header_size;

	header_size = 54 + vars->img.bytes_pp * vars->screen.height\
	* vars->screen.width;
	ft_bzero(header, 54);
	header[0] = 'B';
	header[1] = 'M';
	header[2] = (unsigned char)header_size;
	header[3] = (unsigned char)(header_size >> 8);
	header[4] = (unsigned char)(header_size >> 16);
	header[5] = (unsigned char)(header_size >> 24);
	header[10] = 54;
	header[14] = 40;
	header[18] = (unsigned char)vars->screen.width;
	header[19] = (unsigned char)(vars->screen.width >> 8);
	header[20] = (unsigned char)(vars->screen.width >> 16);
	header[21] = (unsigned char)(vars->screen.width >> 24);
	header[22] = vars->screen.height;
	header[23] = (unsigned char)(vars->screen.height >> 8);
	header[24] = (unsigned char)(vars->screen.height >> 16);
	header[25] = (unsigned char)(vars->screen.height >> 24);
	header[26] = 1;
	header[28] = vars->img.bits_per_pixel;
	write(fd, header, 54);
}

static unsigned int	get_pixel(t_image *img, int x, int y)
{
	char *pixel;

	pixel = img->addr + (y * img->size_line + x * img->bytes_pp);
	return (*(unsigned int *)pixel);
}

static void			get_body(int fd, t_vars *vars)
{
	int				i;
	int				j;
	int				pixel;
	unsigned char	pixel_line[vars->screen.width * vars->img.bytes_pp];

	i = vars->screen.height;
	while (--i >= 0)
	{
		j = -1;
		while (++j < vars->screen.width)
		{
			pixel = get_pixel(&vars->img, j, i);
			pixel_line[j * vars->img.bytes_pp] = (unsigned char)pixel;
			pixel_line[j * vars->img.bytes_pp + 1] =\
			(unsigned char)(pixel >> 8);
			pixel_line[j * vars->img.bytes_pp + 2] =\
			(unsigned char)(pixel >> 16);
			pixel_line[j * vars->img.bytes_pp + 3] =\
			(unsigned char)(pixel >> 24);
		}
		write(fd, pixel_line, vars->screen.width * vars->img.bytes_pp);
	}
}

void				do_screenshot(t_vars *vars)
{
	int fd;

	fd = open("./screenshot.bmp",\
	O_CREAT | O_TRUNC | O_RDWR, S_IWRITE | S_IREAD);
	if (fd == -1)
		error_handler("Fail screenshot");
	vars->img.bytes_pp = vars->img.bits_per_pixel / 8;
	get_head_bmp(fd, vars);
	get_body(fd, vars);
	close(fd);
	exit(0);
}
