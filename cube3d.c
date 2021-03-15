/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 14:24:28 by plurlene          #+#    #+#             */
/*   Updated: 2021/03/12 20:11:22 by plurlene         ###   ########.fr       */
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
		return	(&vars->tex_w);
	if (side == 1)
		return (&vars->tex_n);
	if (side == 2)
		return (&vars->tex_e);
	if (side == 3)
		return (&vars->tex_s);
	return (&vars->tex_e);
}

void get_img_and_add(t_vars vars, t_tex *tex)
{
	tex->img = mlx_xpm_file_to_image(vars.mlx, tex->path, &tex->width, &tex->height);
	tex->addr = mlx_get_data_addr(tex->img, &tex->bbp, &tex->size_line, &tex->endian);
}

int main(int argc, char **argv)
{
	int			fd;
	t_vars		vars;

	vars.tex_floor.path = "./textures/mossy.xpm";
	vars.tex_ceiling.path = "./textures/wood.xpm";
	check_err(argc < 2 || argc > 3, "Incorrect arguments");
	vars.screen.width = 1;
	vars.screen.height = 1;
	vars.mlx = mlx_init();
	main_parser(argv[1], &vars);
	new_image(&vars);
	vars.img.img = mlx_new_image(vars.mlx, vars.screen.width, vars.screen.height);
	vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel, &vars.img.size_line, &vars.img.endian);
	get_img_and_add(vars, &vars.tex_ceiling);
	get_img_and_add(vars, &vars.tex_floor);
	put_image(&vars);
	if (argc == 3)
	{
		if (ft_strnstr("--save", argv[2], 6))
			do_screenshot(&vars);
		else error_handler("Incorrect arguments");
	}
	vars.mlx_window = mlx_new_window(vars.mlx, vars.screen.width, vars.screen.height, "cube3D");
	mlx_put_image_to_window(vars.mlx, vars.mlx_window, vars.img.img, 0, 0);
	mlx_hook(vars.mlx_window, 2, 1L<<0, key_hook, &vars);
	mlx_loop(vars.mlx);
}
