/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_sprites.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 18:35:29 by plurlene          #+#    #+#             */
/*   Updated: 2021/02/09 18:46:45 by plurlene         ###   ########.fr       */
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
	t->spriteX = vars->sprites[t->i]->x - vars->player.x;
	t->spriteY = vars->sprites[t->i]->y - vars->player.y;
	t->invDet = 1.0 / (vars->player.plane_x * vars->player.dir_y -\
	vars->player.dir_x * vars->player.plane_y);
	t->transformX = t->invDet * (vars->player.dir_y * t->spriteX -\
	vars->player.dir_x * t->spriteY);
	t->transformY = t->invDet * (-vars->player.plane_y * t->spriteX +\
	vars->player.plane_x * t->spriteY);
	t->spriteScreenX = (int)((vars->screen.width / 2) * (1 +\
	t->transformX / t->transformY));
	t->spriteHeight = abs((int)(vars->screen.height / t->transformY));
	t->drawStartY = -t->spriteHeight / 2 + vars->screen.height / 2;
	if (t->drawStartY < 0)
		t->drawStartY = 0;
	t->drawEndY = t->spriteHeight / 2 + vars->screen.height / 2;
	if (t->drawEndY >= vars->screen.height)
		t->drawEndY = vars->screen.height - 1;
	t->spriteWidth = abs((int)(vars->screen.height / t->transformY));
	t->drawStartX = -t->spriteWidth / 2 + t->spriteScreenX;
	if (t->drawStartX < 0)
		t->drawStartX = 0;
	t->drawEndX = t->spriteWidth / 2 + t->spriteScreenX;
	if (t->drawEndX >= vars->screen.width)
		t->drawEndX = vars->screen.width - 1;
	t->stripe = t->drawStartX;
}

static void	draw_sprites(t_vars *vars, t_draw_sprites *t)
{
	while (t->stripe < t->drawEndX)
	{
		t->texX = (int)(256 * (t->stripe - (-t->spriteWidth / 2 +\
		t->spriteScreenX)) * vars->tex_sprite.width / t->spriteWidth) / 256;
		if (t->transformY > 0 && t->stripe > 0 && t->stripe <\
		vars->screen.width && t->transformY < vars->z_buffer[t->stripe])
		{
			t->j = t->drawStartY;
			while (t->j < t->drawEndY)
			{
				t->d = t->j * 256 - vars->screen.height * 128 +\
				t->spriteHeight * 128;
				t->texY = ((t->d * vars->tex_sprite.height) /\
				t->spriteHeight) / 256;
				t->color = *(unsigned int *)(vars->tex_sprite.addr + (t->texY *\
				vars->tex_sprite.size_line + t->texX *\
				(vars->tex_sprite.bbp / 8)));
				if ((t->color & 0x00FFFFFF) != 0)
					put_pixel(&vars->img, t->stripe, t->j, t->color);
				t->j++;
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
