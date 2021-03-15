/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_texture_resolution_parser.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 19:54:07 by plurlene          #+#    #+#             */
/*   Updated: 2021/03/12 20:09:03 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

void		resolution_parser(char *str, t_vars *vars)
{
	int		i;
	int		mw;
	int		mh;
	char	*res_str;

	mlx_get_screen_size(vars->mlx, &mw, &mh);
	res_str = ft_strtrim(str, " ");
	check_err(res_str[0] != 'R', "invalid resolution\n");
	i = scip_whitespaces(&res_str[1], 1);
	check_err(!ft_isdigit(res_str[i]), "invalid resolution\n");
	vars->screen.width = 0;
	vars->screen.width = ft_atoi(&res_str[i]);
	vars->screen.width = vars->screen.width > mw ? mw : vars->screen.width;
	while (ft_isdigit(res_str[i]))
		i++;
	i = scip_whitespaces(&res_str[i], i);
	check_err(!ft_isdigit(res_str[i]), "invalid resolution\n");
	vars->screen.height = 0;
	vars->screen.height = ft_atoi(&res_str[i]);
	vars->screen.height = vars->screen.height > mh ? mh : vars->screen.height;
	check_err(!vars->screen.height || !vars->screen.width, "bad resolution\n");
	while (ft_isdigit(res_str[i]))
		i++;
	check_err(res_str[i] != '\0', "invalid resolution\n");
	free(res_str);
}

void		texture_parser(char *str, t_tex *tex, t_vars *vars)
{
	int		i;
	char	*res_str;

	check_err(tex->width && tex->height, "invalid texture\n");
	res_str = ft_strtrim(str, " ");
	i = scip_whitespaces(&res_str[2], 2);
	tex->path = ft_strdup(&res_str[i]);
	if ((i = open(tex->path, O_RDONLY)) < 0)
		error_handler("invalid texture\n");
	else
		close(i);
	get_img_and_add(*vars, tex);
	check_err(!tex->img || !tex->addr, "invalid texture\n");
	free(res_str);
}

static void	check_color_space_format\
	(int i, char **colors, char *temp_str, char *str)
{
	check_err(i != 3 && i != 0, "invalid color\n");
	if (!i)
	{
		colors = ft_split(str, ' ');
		i = 0;
		while (colors[i])
		{
			temp_str = ft_strtrim(colors[i], " ");
			check_err(!ft_isnum(temp_str), "invalid color\n");
			i++;
			free(temp_str);
		}
		clear_str_arr(colors);
		check_err(i != 3 && i != 0, "invalid color\n");
	}
}

static void	check_color_str(char *str)
{
	char	**colors;
	char	*temp_str;
	int		i;

	check_err(!str[0] || ft_char_num(str, ',') > 2, "invalid color\n");
	colors = ft_split(str, ',');
	i = 0;
	while (colors[i] && colors[1])
	{
		temp_str = ft_strtrim(colors[i], " ");
		check_err(!ft_isnum(temp_str), "invalid color\n");
		free(temp_str);
		i++;
	}
	clear_str_arr(colors);
	check_color_space_format(i, colors, temp_str, str);
}

void		color_parser(char *str, t_vars *vars, int *color)
{
	int				i;
	int				num;
	char			*res_str;

	check_color_str(&str[1]);
	res_str = ft_strtrim(str, " ");
	i = scip_whitespaces(&res_str[2], 2);
	num = 0;
	if (ft_isdigit(res_str[i]))
		num = ft_atoi(&res_str[i]);
	check_err(num > 255, "invalid color\n");
	*color = num << 16;
	i = scip_digits(&res_str[i], 1, i);
	i = scip_digits(&res_str[i], 0, i);
	if (ft_isdigit(res_str[i]))
		num = ft_atoi(&res_str[i]);
	check_err(num > 255, "invalid color\n");
	*color = *color | num << 8;
	i = scip_digits(&res_str[i], 1, i);
	i = scip_digits(&res_str[i], 0, i);
	if (ft_isdigit(res_str[i]))
		num = ft_atoi(&res_str[i]);
	check_err(num > 255, "invalid color\n");
	*color = *color | num;
	free(res_str);
}
