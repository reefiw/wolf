/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_and_sprite_parser.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 20:04:34 by plurlene          #+#    #+#             */
/*   Updated: 2021/03/12 20:06:30 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

static void	player_case(char s, t_player *player)
{
	player->plane_x = 0.66;
	player->plane_y = 0.66;
	if (s == 'N')
	{
		player->dir_x = 0;
		player->dir_y = -1;
	}
	if (s == 'E')
	{
		player->dir_x = 1;
		player->dir_y = 0;
	}
	if (s == 'W')
	{
		player->dir_x = -1;
		player->dir_y = 0;
	}
	if (s == 'S')
	{
		player->dir_x = 0;
		player->dir_y = 1;
	}
	player->plane_x *= player->dir_y * -1;
	player->plane_y *= player->dir_x;
}

void		parse_player(t_vars *vars)
{
	int i;
	int j;
	int k;

	i = -1;
	k = 0;
	while (vars->map->data[++i])
	{
		j = -1;
		while (vars->map->data[i][++j])
		{
			if (ft_strchr("NSWE", vars->map->data[i][j]))
			{
				vars->player.x = j + 0.5;
				vars->player.y = i + 0.5;
				player_case(vars->map->data[i][j], &vars->player);
				k++;
			}
		}
	}
	check_err(k != 1, "invalid map\n");
}

static void	put_sprites_in_array(t_vars *vars)
{
	int i;
	int j;
	int sprite_num;

	i = -1;
	sprite_num = 0;
	while (vars->map->data[++i])
	{
		j = -1;
		while (vars->map->data[i][++j])
			if (vars->map->data[i][j] == '2')
			{
				vars->sprites[sprite_num] =\
				(t_sprite *)malloc(sizeof(t_sprite));
				vars->sprites[sprite_num]->x = j + 0.5;
				vars->sprites[sprite_num]->y = i + 0.5;
				vars->sprites[sprite_num]->len = 0;
				sprite_num++;
			}
	}
}

void		init_sprites(t_vars *vars)
{
	int i;
	int j;
	int num_sprites;

	num_sprites = 0;
	i = -1;
	while (vars->map->data[++i])
	{
		j = -1;
		while (vars->map->data[i][++j])
			if (vars->map->data[i][j] == '2')
				num_sprites++;
	}
	vars->sprites = (t_sprite **)malloc(sizeof(t_sprite *) * (num_sprites + 1));
	vars->sprites[num_sprites] = NULL;
	vars->num_sprites = num_sprites;
	put_sprites_in_array(vars);
}
