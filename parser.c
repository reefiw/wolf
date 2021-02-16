/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 15:41:49 by plurlene          #+#    #+#             */
/*   Updated: 2021/02/16 17:27:23 by plurlene         ###   ########.fr       */
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

void texture_parser(char ***temp, char *str, t_tex *tex, t_vars *vars)
{
	int		i;
	char	*res_str;
	res_str = ft_strtrim(str, "");
	i = scip_whitespaces(&res_str[2], 2);
	tex->path = ft_strdup(&res_str[i]);
	if ((i = open(tex->path, O_RDONLY)) < 0)
		error_handler_clear("invalid texture\n", *temp);
	else
		close(i);
//	printf("CHECK: %s\n", str);
	get_img_and_add(*vars, tex);
	free(res_str);
}

static int parser_case(char *str, char *str_case, int n)
{
	if (ft_strnstr(str, str_case, n))
		return (1);
	return (0);
}

static void parser_switch(char ***temp, char *str, t_vars *vars)
{
	if (parser_case(str, "R", 1))
		resolution_parser(temp, str, vars);
	if (parser_case(str, "NO", 2))
		texture_parser(temp, str, &vars->tex_n, vars);
	if (parser_case(str, "SO", 2))
		texture_parser(temp, str, &vars->tex_s, vars);
	if (parser_case(str, "WE", 2))
		texture_parser(temp, str, &vars->tex_w, vars);
	if (parser_case(str, "EA", 2))
		texture_parser(temp, str, &vars->tex_e, vars);
	free(str);
}

void main_parser(char *path, t_vars *vars)
{
	int		fd;
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
	while (temp[++fd])
	{
		parser_switch(&temp, ft_strtrim(temp[fd], " "), vars);
		printf("%d) %s\n", fd, temp[fd]);
	}
//	resolution_parser(&temp, vars);
//	error_handler("it's the end!");
}
