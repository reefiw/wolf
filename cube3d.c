/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 14:24:28 by plurlene          #+#    #+#             */
/*   Updated: 2021/03/05 17:48:47 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

unsigned int get_darker_color(unsigned int color, double size)
{
	if (size <= 1)
		return (color);
	return ((int)(((0xFF0000 & color) >> 16) / size) << 16) +
			((int)(((0x00FF00 & color) >> 8) / size) << 8) +
			((int)(0x0000FF & color) / size);
}

t_tex	*get_tex(t_vars *vars, int side)
{
	if (side == 0)
		return	(&vars->tex_n);
	if (side == 1)
		return (&vars->tex_s);
	if (side == 2)
		return (&vars->tex_w);
	if (side == 3)
		return (&vars->tex_e);
	return (&vars->tex_e);
}

void	init_sprites(t_vars *vars)
{
	int i;
	int j;
	int len;

	i = 0;
	len = 0;
	while (i < vars->map->height)
	{
		j = 0;
		while (j < vars->map->width)
		{
			if (vars->map->data[i][j] == '2')
				len++;
			j++;
		}
		i++;
	}
	vars->sprites = (t_sprite **)malloc(sizeof(t_sprite *) * (len + 1));
	vars->sprites[len] = NULL;
	vars->num_sprites = len;
	i = 0;
	len = 0;
	while (i < vars->map->height)
	{
		j = 0;
		while (j < vars->map->width)
		{
			if (vars->map->data[i][j] == '2')
			{
				vars->sprites[len] = (t_sprite *)malloc(sizeof(t_sprite));
				vars->sprites[len]->x = j + 0.5;
				vars->sprites[len]->y = i + 0.5;
				vars->sprites[len]->len = 0;
				len++;
			}
			j++;
		}
		i++;
	}
}

void get_img_and_add(t_vars vars, t_tex *tex)
{
	tex->img = mlx_xpm_file_to_image(vars.mlx, tex->path, &tex->width, &tex->height);
	tex->addr = mlx_get_data_addr(tex->img, &tex->bbp, &tex->size_line, &tex->endian);
}

int main(void)
{
	int			fd;
	t_vars		vars;

//	init_player(&vars);
	vars.tex_floor.path = "./textures/mossy.xpm";
	vars.tex_ceiling.path = "./textures/wood.xpm";

	fd = open("map.cub", O_RDONLY);
	vars.mlx = mlx_init();
	main_parser("map.cub", &vars);
//	init_sprites(&vars);
	new_image(&vars);
	printf("w: %d h: %d\n", vars.screen.width, vars.screen.height);
	vars.img.img = mlx_new_image(vars.mlx, vars.screen.width, vars.screen.height);
	vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel, &vars.img.size_line, &vars.img.endian);
	get_img_and_add(vars, &vars.tex_ceiling);
	get_img_and_add(vars, &vars.tex_floor);
	fill_back(&vars.img);
	printf("w: %d h: %d\n", vars.screen.width, vars.screen.height);
	put_image(&vars);
	set_minimap2(vars.player, &vars.img, vars.map, 1320, 580);
	vars.mlx_window = mlx_new_window(vars.mlx, vars.screen.width, vars.screen.height, "cube3D");
	mlx_put_image_to_window(vars.mlx, vars.mlx_window, vars.img.img, 0, 0);
	mlx_hook(vars.mlx_window, 2, 1L<<0, key_hook, &vars);
	mlx_loop(vars.mlx);
}
