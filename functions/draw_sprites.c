/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprites.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 18:35:29 by plurlene          #+#    #+#             */
/*   Updated: 2021/03/15 17:07:29 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

static void	sort_sprites(t_vars *vars, t_draw_sprites *t)
{
	t->i = 0;
	while (t->i < vars->num_sprites)
	{
		vars->sprites[t->i]->len = (vars->player.x - vars->sprites[t->i]->x)\
		* (vars->player.x - vars->sprites[t->i]->x)\
		+ (vars->player.y - vars->sprites[t->i]->y) * (vars->player.y\
		- vars->sprites[t->i]->y);
		t->i++;
	}
	t->i = 0;
	while (t->i < vars->num_sprites)
	{
		t->j = vars->num_sprites - 1;
		while (t->j > 0)
		{
			if (vars->sprites[t->j]->len > vars->sprites[t->j - 1]->len)
			{
				t->temp_sprite = vars->sprites[t->j - 1];
				vars->sprites[t->j - 1] = vars->sprites[t->j];
				vars->sprites[t->j] = t->temp_sprite;
			}
			t->j--;
		}
		t->i++;
	}
}

static void	calc_before_draw(t_vars *vars, t_draw_sprites *t)
{
	t->spritex = vars->sprites[t->i]->x - vars->player.x;
	t->spritey = vars->sprites[t->i]->y - vars->player.y;
	t->invdet = 1.0 / (vars->player.plane_x * vars->player.dir_y -\
	vars->player.dir_x * vars->player.plane_y);
	t->transformx = t->invdet * (vars->player.dir_y * t->spritex -\
	vars->player.dir_x * t->spritey);
	t->transformy = t->invdet * (-vars->player.plane_y * t->spritex +\
	vars->player.plane_x * t->spritey);
	t->spritescreenx = (int)((vars->screen.width / 2) * (1 +\
	t->transformx / t->transformy));
	t->sprite_h = abs((int)(vars->screen.height / t->transformy));
	t->drawstarty = -t->sprite_h / 2 + vars->screen.height / 2;
	if (t->drawstarty < 0)
		t->drawstarty = 0;
	t->drawendy = t->sprite_h / 2 + vars->screen.height / 2;
	if (t->drawendy >= vars->screen.height)
		t->drawendy = vars->screen.height - 1;
	t->sprite_w = abs((int)(vars->screen.height / t->transformy));
	t->drawstartx = -t->sprite_w / 2 + t->spritescreenx;
	if (t->drawstartx < 0)
		t->drawstartx = 0;
	t->drawendx = t->sprite_w / 2 + t->spritescreenx;
	if (t->drawendx >= vars->screen.width)
		t->drawendx = vars->screen.width - 1;
	t->stripe = t->drawstartx;
}

static void	draw_sprites(t_vars *vars, t_draw_sprites *t)
{
	while (t->stripe < t->drawendx)
	{
		t->tex_x = (int)(256 * (t->stripe - (-t->sprite_w / 2 +\
		t->spritescreenx)) * vars->tex_sprite.width / t->sprite_w) / 256;
		if (t->transformy > 0 && t->stripe > 0 && t->stripe <\
		vars->screen.width && t->transformy < vars->z_buffer[t->stripe])
		{
			t->j = t->drawstarty - 1;
			while ((++t->j) < t->drawendy)
			{
				t->d = t->j * 256 - vars->screen.height * 128 +\
				t->sprite_h * 128;
				t->tex_y = ((t->d * vars->tex_sprite.height) /\
				t->sprite_h) / 256;
				t->color = *(unsigned int *)(vars->tex_sprite.addr + \
				(t->tex_y * vars->tex_sprite.size_line + t->tex_x *\
				(vars->tex_sprite.bbp / 8)));
				t->color = \
				get_darker_color(t->color, vars->sprites[t->i]->len / 6);
				if ((t->color & 0x00FFFFFF) != 0)
					put_pixel(&vars->img, t->stripe, t->j, t->color);
			}
		}
		t->stripe++;
	}
}

void		put_sprites(t_vars *vars)
{
	t_draw_sprites t;

	sort_sprites(vars, &t);
	t.i = 0;
	while (t.i < vars->num_sprites)
	{
		calc_before_draw(vars, &t);
		draw_sprites(vars, &t);
		t.i++;
	}
}
