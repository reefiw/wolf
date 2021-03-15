/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simplest_drawing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 16:36:36 by plurlene          #+#    #+#             */
/*   Updated: 2021/03/15 14:54:35 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

unsigned int	get_darker_color(unsigned int color, double size)
{
	if (size <= 1)
		return (color);
	return ((int)(((0xFF0000 & color) >> 16) / size) << 16) +\
	((int)(((0x00FF00 & color) >> 8) / size) << 8) +\
	((int)(0x0000FF & color) / size);
}

void			put_pixel(t_image *img, int x, int y, int color)
{
	char *pixel;

	pixel = img->addr + (y * img->size_line + x * (img->bits_per_pixel / 8));
	*(unsigned int *)pixel = color;
}

void			fill_back(t_image *img, t_vars *vars)
{
	int i;
	int j;

	j = -1;
	while (++j < vars->screen.width)
	{
		i = -1;
		while (++i < vars->screen.height / 2)
			put_pixel(img, j, i, vars->color_ceiling);
		while (i < vars->screen.height)
		{
			put_pixel(img, j, i, vars->color_floor);
			i++;
		}
	}
}
