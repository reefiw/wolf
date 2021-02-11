/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simplest_drawing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 16:36:36 by plurlene          #+#    #+#             */
/*   Updated: 2021/02/09 16:40:47 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

void	put_pixel(t_image *img, int x, int y, int color)
{
	char *pixel;

	pixel = img->addr + (y * img->size_line + x * (img->bits_per_pixel / 8));
	*(unsigned int *)pixel = color;
}

void	draw_square(t_image *img, int width, int x, int y, int color)
{
	int i;
	int j;

	i = -1;
	while (++i < width)
	{
		j = -1;
		while (++j < width)
			put_pixel(img, x + i, y + j, color);
	}
}

void	fill_back(t_image *img)
{
	int i;
	int j;

	j = -1;
	while (++j < 1920)
	{
		i = -1;
		while (++i < 1080 / 2)
			put_pixel(img, j, i, 0x8FFF0000);
		while (i < 1080)
		{
			put_pixel(img, j, i, 0xAFFFFFFF);
			i++;
		}
	}
}

void	set_minimap2(t_player player, t_image *img,\
		t_map *map, int d_x, int d_y)
{
	int d;
	int i;
	int k;
	int j;

	d = 20;
	k = 0;
	i = 0;
	while (i < map->height)
	{
		j = 0;
		while (j < map->width)
		{
			if (map->data[i][j] == '1')
				draw_square(img, d, d_x + (((k / d) % map->width) * d),\
				d_y + ((((k / d)) / map->width) * d), 0xFFFFFFF);
			k += d;
			j++;
		}
		i++;
	}
	draw_square(img, 10, d_x + (player.x * d), d_y + (player.y * d), 0xFF0000);
}
