/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 16:36:45 by plurlene          #+#    #+#             */
/*   Updated: 2021/02/09 16:41:10 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

void	new_image(t_vars *vars)
{
	vars->img.img = NULL;
	vars->img.addr = NULL;
	vars->img.bits_per_pixel = 0;
	vars->img.size_line = 0;
	vars->img.endian = 0;
}

void	init_player(t_vars *vars)
{
	vars->player.x = 2.5;
	vars->player.y = 3.5;
	vars->player.dir_x = 1;
	vars->player.dir_y = 0;
	vars->player.plane_x = 0;
	vars->player.plane_y = 0.66;
}

void	init_tex(t_tex *tex, int width, int height)
{
	tex->width = width;
	tex->height = height;
	tex->addr = NULL;
	tex->bbp = 0;
	tex->size_line = 0;
	tex->img = NULL;
}
