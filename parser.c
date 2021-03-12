/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 15:41:49 by plurlene          #+#    #+#             */
/*   Updated: 2021/03/12 19:16:19 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

static int	scip_whitespaces(char *str, int s)
{
	int i;

	i = 0;
	while (str[i] == ' ')
		i++;
	return (i + s);
}

static int	scip_digits(char *str, int flag, int bias)
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

static void	check_err(int bool, char *err)
{
	if (bool)
		error_handler(err);
}

void	resolution_parser(char *str, t_vars *vars)
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

int	texture_parser(char *str, t_tex *tex, t_vars *vars)
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
	return (1);
}

static void	clear_str_arr(char **arr)
{
	int i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr[i]);
	free(arr);
}

static int	ft_isnum(char *str)
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
	return (i);
}

static int	ft_char_num(char *str, int c)
{
	int i;
	int num;

	i = -1;
	num = 0;
	while (str[++i])
		if (str[i] == c)
			num++;
	return (num);
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

static void	color_parser(char *str, t_vars *vars, int *color)
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
	if (parser_case(str, "C ", 2) && !vars->color_floor && (++k))
		color_parser(str, vars, &vars->color_ceiling);
	return (k);
}

static void	add_line(char ***arr, char *line)
{
	int		len;
	char	**result_arr;

	len = 0;
	while ((*arr)[len] != NULL)
		len++;
	result_arr = (char **)malloc(sizeof(char *) * (len + 2));
	result_arr[len + 1] = NULL;
	result_arr[len] = ft_strdup1(line, 1);
	while (--len >= 0)
		result_arr[len] = ft_strdup1((*arr)[len], 1);
	free(*arr);
	*arr = result_arr;
	len = 0;
}

static char	**get_map(int fd, char *first_line)
{
	char *line;
	char **result_arr;

	result_arr = (char **)malloc(sizeof(char *) * 2);
	result_arr[1] = NULL;
	result_arr[0] = ft_strdup1(first_line, 1);
	while (get_next_line(fd, &line))
		add_line(&result_arr, line);
	free(line);
	return (result_arr);
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

static void	init_sprites(t_vars *vars)
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

static void	parse_player(t_vars *vars)
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
				printf("x: %f y: %f \n", vars->player.x, vars->player.y);
				player_case(vars->map->data[i][j], &vars->player);
				k++;
			}
		}
	}
	check_err(k != 1, "invalid map\n");
}

static char	**get_head(int fd, t_vars *vars)
{
	char	*buf;
	char	*line;
	char	**temp;
	int		k;

	buf = (char *)calloc(1, 1);
	while (get_next_line(fd, &line))
	{
		k = scip_whitespaces(line, 0);
		if (ft_strchr("012", line[k]))
			break ;
		buf = ft_strjoin1(buf, "\n", 1);
		buf = ft_strjoin1(buf, line, 1);
		free(line);
	}
	if (!ft_strchr("012", line[k]))
	{
		free(line);
		free(buf);
		error_handler("invalid map\n");
	}
	vars->map->data = get_map(fd, line);
	temp = ft_split(buf, '\n');
	free(buf);
	return (temp);
}

static void	check_map(t_vars *vars)
{
	int i;
	int j;

	i = 0;
	while (vars->map->data[i])
	{
		j = 0;
		while (vars->map->data[i][j])
		{
			if (ft_strchr("02NSWE", vars->map->data[i][j]))
			{
				if (i == 0 || j == 0 || !vars->map->data[i + 1] ||\
				!vars->map->data[i][j + 1] || ft_strlen1(\
				vars->map->data[i - 1]) <= j ||\
				ft_strlen1(vars->map->data[i + 1]) <= j)
					error_handler("invalid map\n");
			}
			else
				check_err(!ft_strchr("1 ",\
			vars->map->data[i][j]), "invalid map\n");
			j++;
		}
		check_err(vars->map->data[i + 1] && !j, "invalid map\n");
		i++;
	}
}

void	main_parser(char *path, t_vars *vars)
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
