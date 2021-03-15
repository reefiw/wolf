/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 18:20:39 by plurlene          #+#    #+#             */
/*   Updated: 2021/03/10 16:53:30 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

static void	calc_side_dist(t_vars *vars, t_dda *t)
{
	if (t->ray_dir_x < 0)
	{
		t->step_x = -1;
		t->sideDist_x = (vars->player.x - t->map_x) * t->deltaDist_x;
	}
	else
	{
		t->step_x = 1;
		t->sideDist_x = (t->map_x + 1.0 - vars->player.x) * t->deltaDist_x;
	}
	if (t->ray_dir_y < 0)
	{
		t->step_y = -1;
		t->sideDist_y = (vars->player.y - t->map_y) * t->deltaDist_y;
	}
	else
	{
		t->step_y = 1;
		t->sideDist_y = (t->map_y + 1.0 - vars->player.y) * t->deltaDist_y;
	}
}

static void	find_hit(t_vars *vars, t_dda *t)
{
	t->hit = 0;
	while (t->hit == 0)
	{
		if (t->sideDist_x < t->sideDist_y)
		{
			t->side = 0;
			t->sideDist_x += t->deltaDist_x;
			t->map_x += t->step_x;
		}
		else
		{
			t->side = 1;
			t->sideDist_y += t->deltaDist_y;
			t->map_y += t->step_y;
		}
		if (vars->map->data[t->map_y][t->map_x] == '1')
			t->hit = 1;
	}
}

static void	calc_before_draw(t_vars *vars, t_dda *t)
{
	if (t->side == 0)
		t->perpWallDist = (t->map_x - vars->player.x +\
		(1 - t->step_x) / 2) / t->ray_dir_x;
	else
		t->perpWallDist = (t->map_y - vars->player.y +\
		(1 - t->step_y) / 2) / t->ray_dir_y;
	t->lineHeight = (int)(vars->screen.height / t->perpWallDist);
	t->drawStart = -t->lineHeight / 2 + vars->screen.height / 2;
	t->drawStart = t->drawStart < 0 ? 0 : t->drawStart;
	t->drawEnd = t->lineHeight / 2 + vars->screen.height / 2;
	t->drawEnd = t->drawEnd >= vars->screen.height ?\
	vars->screen.height - 1 : t->drawEnd;
	if (t->side == 0)
		t->wallX = vars->player.y + t->perpWallDist * t->ray_dir_y;
	else
		t->wallX = vars->player.x + t->perpWallDist * t->ray_dir_x;
	t->wallX -= floor(t->wallX);
	if (t->side == 0)
		if (t->ray_dir_x < 0)
			t->side = 2;
	if (t->side == 1)
		if (t->ray_dir_y < 0)
			t->side = 3;
	t->tex = get_tex(vars, t->side);
	t->texX = (int)(t->wallX * (float)t->tex->width);
}

static void	drawing_walls(t_vars *vars, t_dda *t)
{
	if ((t->side == 0 || t->side == 2) && t->ray_dir_x > 0)
		t->texX = t->tex->width - t->texX - 1;
	if ((t->side == 1 || t->side == 3) && t->ray_dir_y < 0)
		t->texX = t->tex->width - t->texX - 1;
	t->step = 1.0 * t->tex->height / t->lineHeight;
	t->texPos = (t->drawStart - vars->screen.height / 2 +\
	t->lineHeight / 2) * t->step;
	while (t->drawStart < t->drawEnd)
	{
		t->texY = (int)t->texPos & (t->tex->height - 1);
		t->color = *(unsigned int *)(t->tex->addr +\
		(t->texY * t->tex->size_line + t->texX * (t->tex->bbp / 8)));
		t->texPos += t->step;
		t->color = get_darker_color(t->color, t->perpWallDist / 1.5);
		put_pixel(&vars->img, t->i, t->drawStart, t->color);
		t->drawStart++;
	}
	vars->z_buffer[t->i] = t->perpWallDist;
}

void		put_image(t_vars *vars)
{
	t_dda t;

//	fill_back(&vars->img, vars);
	put_floor_ceiling(vars);
	t.i = 0;
	while (t.i < vars->screen.width)
	{
		t.camera_x = 2 * t.i / (double)vars->screen.width - 1;
		t.ray_dir_x = vars->player.dir_x + vars->player.plane_x * t.camera_x;
		t.ray_dir_y = vars->player.dir_y + vars->player.plane_y * t.camera_x;
		t.map_x = (int)vars->player.x;
		t.map_y = (int)vars->player.y;
		t.deltaDist_x = fabs(1 / t.ray_dir_x);
		t.deltaDist_y = fabs(1 / t.ray_dir_y);
		calc_side_dist(vars, &t);
		find_hit(vars, &t);
		calc_before_draw(vars, &t);
		drawing_walls(vars, &t);
		t.i++;
	}
	put_sprites(vars);
}
