/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_ceiling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 18:47:55 by plurlene          #+#    #+#             */
/*   Updated: 2021/02/09 19:04:11 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

static void	draw_floor_ceiling(t_vars *vars, t_floor_ceiling *t)
{
	while (t->j < vars->screen.width)
	{
		t->cellX = (int)t->floorX;
		t->cellY = (int)t->floorY;
		t->tx = (int)(vars->tex_floor.width * (t->floorX - t->cellX))\
		& (vars->tex_floor.width - 1);
		t->ty = (int)(vars->tex_floor.height * (t->floorY - t->cellY))\
		& (vars->tex_floor.height - 1);
		t->floorX += t->floorStepX;
		t->floorY += t->floorStepY;
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

	t.rayDirX0 = vars->player.dir_x - vars->player.plane_x;
	t.rayDirY0 = vars->player.dir_y - vars->player.plane_y;
	t.rayDirX1 = vars->player.dir_x + vars->player.plane_x;
	t.rayDirY1 = vars->player.dir_y + vars->player.plane_y;
	t.posZ = 0.5 * vars->screen.height;
	t.i = 0;
	while (t.i < vars->screen.height)
	{
		t.p = t.i - vars->screen.height / 2;
		t.rowDistance = t.posZ / t.p;
		t.floorStepX = t.rowDistance * (t.rayDirX1 - t.rayDirX0) /\
		vars->screen.width;
		t.floorStepY = t.rowDistance * (t.rayDirY1 - t.rayDirY0) /\
		vars->screen.width;
		t.floorX = vars->player.x + t.rowDistance * t.rayDirX0;
		t.floorY = vars->player.y + t.rowDistance * t.rayDirY0;
		t.j = 0;
		draw_floor_ceiling(vars, &t);
		t.i++;
	}
}
