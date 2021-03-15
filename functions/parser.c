/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 15:41:49 by plurlene          #+#    #+#             */
/*   Updated: 2021/03/15 18:47:07 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

static int	parser_case(char *str, char *str_case, int n)
{
	if (ft_strnstr(str, str_case, n))
		return (1);
	return (0);
}

static int	parser_switch(char *str, t_vars *vars)
{
	int k;

	k = 0;
	if (parser_case(str, "R ", 2) && (++k))
		resolution_parser(str, vars);
	if (parser_case(str, "NO ", 3) && (++k))
		texture_parser(str, &vars->tex_n, vars);
	if (parser_case(str, "SO ", 3) && (++k))
		texture_parser(str, &vars->tex_s, vars);
	if (parser_case(str, "WE ", 3) && (++k))
		texture_parser(str, &vars->tex_w, vars);
	if (parser_case(str, "EA ", 3) && (++k))
		texture_parser(str, &vars->tex_e, vars);
	if (parser_case(str, "S ", 2) && (++k))
		texture_parser(str, &vars->tex_sprite, vars);
	if (parser_case(str, "F ", 2) && !vars->color_floor && (++k))
		color_parser(str, vars, &vars->color_floor);
	if (parser_case(str, "C ", 2) && !vars->color_ceiling && (++k))
		color_parser(str, vars, &vars->color_ceiling);
	return (k);
}

void		main_parser(char *path, t_vars *vars)
{
	int		fd;
	int		k;
	char	**temp;

	check_err(ft_strncmp(&path[ft_strlen(path) - 4], ".cub", 4),\
	"invalid extension\n");
	check_err((fd = open(path, O_RDONLY)) < 1, "can't open file\n");
	vars->map = (t_map *)malloc(sizeof(t_map));
	temp = get_head(fd, vars);
	init_sprites(vars);
	parse_player(vars);
	close(fd);
	k = -1;
	while (vars->map->data[++k])
		vars->map->height = k + 1;
	check_map(vars);
	fd = -1;
	k = 0;
	while (temp[++fd] && (k += parser_switch(temp[fd], vars)))
		check_err(k > 8, "invalid file\n");
	clear_str_arr(temp);
	vars->z_buffer = (double *)malloc(sizeof(double) * vars->screen.width);
	check_err(fd != 8 || k != 8, "invalid file\n");
}
