/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 18:20:39 by plurlene          #+#    #+#             */
/*   Updated: 2021/03/15 17:07:02 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

static void	calc_side_dist(t_vars *vars, t_dda *t)
{
	if (t->ray_dir_x < 0)
	{
		t->step_x = -1;
		t->sidedist_x = (vars->player.x - t->map_x) * t->deltadist_x;
	}
	else
	{
		t->step_x = 1;
		t->sidedist_x = (t->map_x + 1.0 - vars->player.x) * t->deltadist_x;
	}
	if (t->ray_dir_y < 0)
	{
		t->step_y = -1;
		t->sidedist_y = (vars->player.y - t->map_y) * t->deltadist_y;
	}
	else
	{
		t->step_y = 1;
		t->sidedist_y = (t->map_y + 1.0 - vars->player.y) * t->deltadist_y;
	}
}

static void	find_hit(t_vars *vars, t_dda *t)
{
	t->hit = 0;
	while (t->hit == 0)
	{
		if (t->sidedist_x < t->sidedist_y)
		{
			t->side = 0;
			t->sidedist_x += t->deltadist_x;
			t->map_x += t->step_x;
		}
		else
		{
			t->side = 1;
			t->sidedist_y += t->deltadist_y;
			t->map_y += t->step_y;
		}
		if (vars->map->data[t->map_y][t->map_x] == '1')
			t->hit = 1;
	}
}

static void	calc_before_draw(t_vars *vars, t_dda *t)
{
	if (t->side == 0)
		t->wall_dist = (t->map_x - vars->player.x +\
		(1 - t->step_x) / 2) / t->ray_dir_x;
	else
		t->wall_dist = (t->map_y - vars->player.y +\
		(1 - t->step_y) / 2) / t->ray_dir_y;
	t->line_h = (int)(vars->screen.height / t->wall_dist);
	t->drawstart = -t->line_h / 2 + vars->screen.height / 2;
	t->drawstart = t->drawstart < 0 ? 0 : t->drawstart;
	t->drawend = t->line_h / 2 + vars->screen.height / 2;
	t->drawend = t->drawend >= vars->screen.height ?\
	vars->screen.height - 1 : t->drawend;
	if (t->side == 0)
		t->wallx = vars->player.y + t->wall_dist * t->ray_dir_y;
	else
		t->wallx = vars->player.x + t->wall_dist * t->ray_dir_x;
	t->wallx -= floor(t->wallx);
	if (t->side == 0)
		if (t->ray_dir_x < 0)
			t->side = 2;
	if (t->side == 1)
		if (t->ray_dir_y < 0)
			t->side = 3;
	t->tex = get_tex(vars, t->side);
	t->tex_x = (int)(t->wallx * (float)t->tex->width);
}

static void	drawing_walls(t_vars *vars, t_dda *t)
{
	if ((t->side == 0 || t->side == 2) && t->ray_dir_x > 0)
		t->tex_x = t->tex->width - t->tex_x - 1;
	if ((t->side == 1 || t->side == 3) && t->ray_dir_y < 0)
		t->tex_x = t->tex->width - t->tex_x - 1;
	t->step = 1.0 * t->tex->height / t->line_h;
	t->texpos = (t->drawstart - vars->screen.height / 2 +\
	t->line_h / 2) * t->step;
	while (t->drawstart < t->drawend)
	{
		t->tex_y = (int)t->texpos & (t->tex->height - 1);
		t->color = *(unsigned int *)(t->tex->addr +\
		(t->tex_y * t->tex->size_line + t->tex_x * (t->tex->bbp / 8)));
		t->texpos += t->step;
		t->color = get_darker_color(t->color, t->wall_dist / 1.5);
		put_pixel(&vars->img, t->i, t->drawstart, t->color);
		t->drawstart++;
	}
	vars->z_buffer[t->i] = t->wall_dist;
}

void		put_image(t_vars *vars)
{
	t_dda t;

	fill_back(&vars->img, vars);
	put_floor_ceiling(vars);
	t.i = 0;
	while (t.i < vars->screen.width)
	{
		t.camera_x = 2 * t.i / (double)vars->screen.width - 1;
		t.ray_dir_x = vars->player.dir_x + vars->player.plane_x * t.camera_x;
		t.ray_dir_y = vars->player.dir_y + vars->player.plane_y * t.camera_x;
		t.map_x = (int)vars->player.x;
		t.map_y = (int)vars->player.y;
		t.deltadist_x = fabs(1 / t.ray_dir_x);
		t.deltadist_y = fabs(1 / t.ray_dir_y);
		calc_side_dist(vars, &t);
		find_hit(vars, &t);
		calc_before_draw(vars, &t);
		drawing_walls(vars, &t);
		t.i++;
	}
	put_sprites(vars);
}
