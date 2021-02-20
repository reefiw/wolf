/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 15:41:49 by plurlene          #+#    #+#             */
/*   Updated: 2021/02/20 20:15:53 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

static int scip_whitespaces(char *str, int s)
{
	int i;

	i = 0;
	while (str[i] == ' ')
		i++;
	return (i + s);
}

static int scip_digits(char *str, int flag, int bias)
{
	int i;

	i = 0;
	if (flag)
	{
		while (ft_isdigit(str[i]))
			i++;
	}
	else
	{
		while (!ft_isdigit(str[i]))
			i++;
	}
	return (i + bias);
}

void resolution_parser(char ***temp, char *str, t_vars *vars)
{
	int		i;
	char	*res_str;

	res_str = ft_strtrim(str, " ");
	if (res_str[0] != 'R')
		error_handler_clear("invalid resolution\n", *temp);
	i = scip_whitespaces(&res_str[1], 1);
	if (!ft_isdigit(res_str[i]))
		error_handler_clear("invalid resolution\n", *temp);
	vars->screen.width = 0;
	vars->screen.width = ft_atoi(&res_str[i]);
	while (ft_isdigit(res_str[i]))
		i++;
	i = scip_whitespaces(&res_str[i], i);
	if (!ft_isdigit(res_str[i]))
		error_handler_clear("invalid resolution\n", *temp);
	vars->screen.height = 0;
	vars->screen.height = ft_atoi(&res_str[i]);
	if (!vars->screen.height || !vars->screen.width)
		error_handler_clear("invalid resolution\n", *temp);
	while (ft_isdigit(res_str[i]))
		i++;
	if (res_str[i] != '\0')
		error_handler_clear("invalid resolution\n", *temp);
	free(res_str);
}

int texture_parser(char ***temp, char *str, t_tex *tex, t_vars *vars)
{
	int		i;
	char	*res_str;

	res_str = ft_strtrim(str, " ");
	i = scip_whitespaces(&res_str[2], 2);
	tex->path = ft_strdup(&res_str[i]);
	if ((i = open(tex->path, O_RDONLY)) < 0)
		error_handler_clear("invalid texture\n", *temp);
	else
		close(i);
	get_img_and_add(*vars, tex);
	free(res_str);
	return (1);
}

static void clear_str_arr(char **arr)
{
	int i;

	i = 0;
	while (arr[i])
	{
		printf("CHECKs: %s\n", arr[i]);
		free(arr[i]);
		i++;
	}
	free(arr);
}

static int ft_isnum(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]))
			i++;
		else
			return (0);
	}
	return (1);
}

static void check_color_str(char ***temp, char *str)
{
	char **colors;
	char *temp_str;
	int i;

	colors = ft_split(str, ',');
	i = 0;
	while (colors[i])
	{
		temp_str = ft_strtrim(colors[i], " ");
		if (!ft_isnum(temp_str))
		{
			free(temp_str);
			clear_str_arr(colors);
			error_handler_clear("invalid color\n", *temp);
		}
		free(temp_str);
		i++;
	}
	clear_str_arr(colors);
	if (i != 3 && i != 0)
		error_handler_clear("invalid color\n", *temp);
	if (!i)
	{
		colors = ft_split(str, ' ');
		i = 0;
		while (colors[i])
		{
			i++;
			if (!ft_isnum(temp_str))
			{
				clear_str_arr(colors);
				error_handler_clear("invalid color\n", *temp);
			}
		}
		if (i != 3 && i != 0)
			error_handler_clear("invalid color\n", *temp);
	}
}

static void color_parser(char ***temp, char *str, t_vars *vars, int *color)
{
	int				i;
	int				num;
	char			*res_str;

	(void)temp;
	(void)vars;
	*color = 0;
	check_color_str(temp, &str[1]);
	res_str = ft_strtrim(str, " ");
	i = scip_whitespaces(&res_str[2], 2);
	num = 0;
	if (ft_isdigit(str[i]))
		num = ft_atoi(&str[i]);
	if (num > 255)
		error_handler_clear("invalid color\n", *temp);
	*color = num << 16;
	i = scip_digits(&str[i], 1, i);
	i = scip_digits(&str[i], 0, i);
	if (ft_isdigit(str[i]))
		num = ft_atoi(&str[i]);
	if (num > 255)
		error_handler_clear("invalid color\n", *temp);
	*color = *color | num << 8;
	i = scip_digits(&str[i], 1, i);
	i = scip_digits(&str[i], 0, i);
	if (ft_isdigit(str[i]))
		num = ft_atoi(&str[i]);
	if (num > 255)
		error_handler_clear("invalid color\n", *temp);
	*color = *color | num;
	free(res_str);
}

static int parser_case(char *str, char *str_case, int n)
{
	if (ft_strnstr(str, str_case, n))
		return (1);
	return (0);
}

static int parser_switch(char ***temp, char *str, t_vars *vars)
{
	int k;

	k = 0;
	if (parser_case(str, "R", 1) && (k = 1))
		resolution_parser(temp, str, vars);
	if (parser_case(str, "NO", 2) && (k = 1))
		texture_parser(temp, str, &vars->tex_n, vars);
	if (parser_case(str, "SO", 2) && (k = 1))
		texture_parser(temp, str, &vars->tex_s, vars);
	if (parser_case(str, "WE", 2) && (k = 1))
		texture_parser(temp, str, &vars->tex_w, vars);
	if (parser_case(str, "EA", 2) && (k = 1))
		texture_parser(temp, str, &vars->tex_e, vars);
	if (parser_case(str, "S", 1) && (k = 1))
		texture_parser(temp, str, &vars->tex_sprite, vars);
	if (parser_case(str, "F", 1) && (k = 1))
		color_parser(temp, str, vars, &vars->color_floor);
	if (parser_case(str, "C", 1) && (k = 1))
		color_parser(temp, str, vars, &vars->color_ceiling);
	free(str);
	return (k);
}

void main_parser(char *path, t_vars *vars)
{
	int		fd;
	int		k;
	char	*buf;
	char	*line;
	char	**temp;

	(void)path;
	(void)vars;
	if (ft_strncmp(&path[ft_strlen(path) - 4], ".cub", 4))
		error_handler("invalid extension\n");
	if	((fd = open(path, O_RDONLY)) < 1)
		error_handler("can't open file\n");
	line = NULL;
	buf = (char *)malloc(1);
	buf[0] = '\0';
	while (get_next_line(fd, &line))
	{
		buf = ft_strjoin1(buf, "\n", 1);
		buf = ft_strjoin1(buf, line, 1);
		free(line);
	}
	free(line);
	temp = ft_split(buf, '\n');
	fd = -1;
	k = 0;
	while (temp[++fd])
	{
		k += parser_switch(&temp, ft_strtrim(temp[fd], " "), vars);
		printf("%d) %s\n", fd, temp[fd]);
	}
	vars->z_buffer = (double *)malloc(sizeof(double) * vars->screen.width);
	if (k != 8)
		error_handler("invalid file\n");
}
