/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_and_map_parser.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/12 20:00:32 by plurlene          #+#    #+#             */
/*   Updated: 2021/03/15 18:36:53 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3d.h"

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

char		**get_head(int fd, t_vars *vars)
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
		buf = ft_strjoin1(buf, &line[k], 1);
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

static void	check_whitespace(t_map map, int i, int j)
{
	check_err(i == 0 || j == 0, "invalid map\n");
	check_err(!map.data[i + 1] || !map.data[i][j + 1], "invalid map\n");
	check_err(!map.data[i - 1] || !map.data[i][j - 1], "invalid map\n");
	check_err(!map.data[i - 1][j - 1] || !map.data[i - 1][j] ||\
	!map.data[i - 1][j + 1], "invalid map\n");
	check_err(!map.data[i + 1][j - 1] || !map.data[i + 1][j] ||\
	!map.data[i + 1][j + 1], "invalid map\n");
	check_err(map.data[i][j - 1] == ' ' ||\
	map.data[i][j + 1] == ' ', "invalid map\n");
	check_err(map.data[i - 1][j - 1] == ' ' || map.data[i - 1][j] == ' ' ||\
	map.data[i - 1][j + 1] == ' ', "invalid map\n");
	check_err(map.data[i + 1][j - 1] == ' ' || map.data[i + 1][j] == ' ' ||\
	map.data[i + 1][j + 1] == ' ', "invalid map\n");
}

void		check_map(t_vars *vars)
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
				check_whitespace(*vars->map, i, j);
			else
				check_err(!ft_strchr("1 ",\
			vars->map->data[i][j]), "invalid map\n");
			j++;
		}
		check_err(vars->map->data[i + 1] && !j, "invalid map\n");
		i++;
	}
}
