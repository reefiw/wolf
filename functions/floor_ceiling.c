/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_ceiling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 18:47:55 by plurlene          #+#    #+#             */
/*   Updated: 2021/03/15 17:05:24 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

static void	draw_floor_ceiling(t_vars *vars, t_floor_ceiling *t)
{
	while (t->j < vars->screen.width)
	{
		t->cellx = (int)t->floorx;
		t->celly = (int)t->floory;
		t->tx = (int)(vars->tex_floor.width * (t->floorx - t->cellx))\
		& (vars->tex_floor.width - 1);
		t->ty = (int)(vars->tex_floor.height * (t->floory - t->celly))\
		& (vars->tex_floor.height - 1);
		t->floorx += t->floorstepx;
		t->floory += t->floorstepy;
		t->color = *(unsigned int *)(vars->tex_floor.addr +\
		(t->ty * vars->tex_floor.size_line +\
		t->tx * (vars->tex_floor.bbp / 8)));
		t->color = get_darker_color(t->color, t->rowDistance);
		put_pixel(&vars->img, t->j, t->i, t->color);
		t->color = *(unsigned int *)(vars->tex_ceiling.addr + (t->ty\
		* vars->tex_ceiling.size_line + t->tx * (vars->tex_ceiling.bbp / 8)));
		t->color = get_darker_color(t->color, t->rowDistance);
		put_pixel(&vars->img, t->j, vars->screen.height - t->i - 1, t->color);
		t->j++;
	}
}

void		put_floor_ceiling(t_vars *vars)
{
	t_floor_ceiling t;

	t.raydirx0 = vars->player.dir_x - vars->player.plane_x;
	t.raydiry0 = vars->player.dir_y - vars->player.plane_y;
	t.raydirx1 = vars->player.dir_x + vars->player.plane_x;
	t.raydiry1 = vars->player.dir_y + vars->player.plane_y;
	t.posz = 0.5 * vars->screen.height;
	t.i = 0;
	while (t.i < vars->screen.height)
	{
		t.p = t.i - vars->screen.height / 2;
		t.rowDistance = t.posz / t.p;
		t.floorstepx = t.rowDistance * (t.raydirx1 - t.raydirx0) /\
		vars->screen.width;
		t.floorstepy = t.rowDistance * (t.raydiry1 - t.raydiry0) /\
		vars->screen.width;
		t.floorx = vars->player.x + t.rowDistance * t.raydirx0;
		t.floory = vars->player.y + t.rowDistance * t.raydiry0;
		t.j = 0;
		draw_floor_ceiling(vars, &t);
		t.i++;
	}
}
