/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 14:24:28 by plurlene          #+#    #+#             */
/*   Updated: 2021/01/19 16:32:13 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <unistd.h>
#include "cube3D.h"
#include "GNL/get_next_line.h"

t_image	*new_image(void *n_img, char *n_addr, int n_bits_per_pixel, int n_size_line, int n_endian)
{
	t_image *new_image;

	new_image = (t_image *)malloc(sizeof(t_image));
	new_image->img = n_img;
	new_image->addr = n_addr;
	new_image->bits_per_pixel = n_bits_per_pixel;
	new_image->size_line = n_size_line;
	new_image->endian = n_endian;
	return (new_image);
}

t_screen *new_screen(int width, int height)
{
	t_screen *new_screen;

	if (!(new_screen = (t_screen *)malloc(sizeof(t_screen))))
		return (NULL);
	new_screen->width = width;
	new_screen->height = height;
	return(new_screen);
}

t_player *init_player(float n_x, float n_y, float n_dir_x, float n_dir_y, float n_plane_x, float n_plane_y)
{
	t_player *new_player;

	new_player = (t_player *)malloc(sizeof(t_player));
	new_player->x = n_x;
	new_player->y = n_y;
	new_player->dir_x = n_dir_x;
	new_player->dir_y = n_dir_y;
	new_player->plane_x = n_plane_x;
	new_player->plane_y = n_plane_y;

	return (new_player);
}

t_tex *init_tex(int width, int height)
{
	t_tex *new_tex;

	new_tex = (t_tex *)malloc(sizeof(t_tex));
	new_tex->width = width;
	new_tex->height = height;
	return (new_tex);
}

void	put_pixel(t_image *img, int x, int y, int color)
{
	char *pixel;
	pixel = img->addr + (y * img->size_line + x * (img->bits_per_pixel / 8));
	*(unsigned int *)pixel = color;
}

void	draw_square(t_image *img, int width, int x, int y, int color)
{
	int i;
	int j;

	i = -1;
	while (++i < width)
	{
		j = -1;
		while (++j < width)
			put_pixel(img, x + i, y + j, color);
	}
}

void	fill_back(t_image *img)
{
	int i;
	int j;

	j = -1;
	while (++j < 1920)
	{
		i = -1;
		while (++i < 1080 / 2)
			put_pixel(img, j, i, 0x8FFF0000);
		while (i < 1080)
		{
			put_pixel(img, j, i, 0xAFFFFFFF);
			i++;
		}
	}
}

void	cast_ray(t_vars *vars, float dir_angle, int d_y)
{
	float	i;
	float	x;
	float	y;
	int		k;

	i = 0.0;
	while (1)
	{
		x = vars->player->x + (i * cos(dir_angle));
		y = vars->player->y + (i * sin(dir_angle));
		if (vars->map->data[(int)y][(int)x] == '1')
			break ;
		i += 0.01;
	}
	k = -1;
	i = i > 0 && i < 1 ? 1 : i;
	int column_height = 1080 / i;
	if (i != 0)
		while (++k < column_height)
			put_pixel(vars->img, d_y, (1080 / 2) - (column_height / 2) + k, 0x00AAAAA0);
}

unsigned int color_handler(int map_x, int map_y, t_vars *vars, int side)
{
	unsigned int color;

	color = 0x00AAAAA0;
	if (vars->map->data[map_y][map_x] == 1)
		color = 0x00AAAAA0;
	if (side == 1)
		color = color / 2;
	return color;
}

void	put_image(t_vars *vars)
{
	double ray_dir_x;
	double ray_dir_y;
	double camera_x;
	double sideDist_x;
	double sideDist_y;
	double deltaDist_x;
	double deltaDist_y;
	double perpWallDist;
	double wallX;
	double step;
	double texPos;

	int		lineHeight;
	int		drawStart;
	int		drawEnd;
	int		map_x;
	int		map_y;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
	int		texNum;
	int		texX;
	int		texY;
	int i;

//	unsigned int **buffer[vars->screen->height][vars->screen->width];

	i = 0;
	while (i < vars->screen->width)
	{
		camera_x = 2 * i / (double)vars->screen->width - 1;
		ray_dir_x = vars->player->dir_x + vars->player->plane_x * camera_x;
		ray_dir_y = vars->player->dir_y + vars->player->plane_y * camera_x;

		map_x = (int)vars->player->x;
		map_y = (int)vars->player->y;

		deltaDist_x = fabs(1 / ray_dir_x);
		deltaDist_y = fabs(1 / ray_dir_y);

		if (ray_dir_x < 0)
		{
			step_x = -1;
			sideDist_x = (vars->player->x - map_x) * deltaDist_x;
		}
		else
		{
			step_x = 1;
			sideDist_x = (map_x + 1.0 - vars->player->x) * deltaDist_x;
		}
		if (ray_dir_y < 0)
		{
			step_y = -1;
			sideDist_y = (vars->player->y - map_y) * deltaDist_y;
		}
		else
		{
			step_y = 1;
			sideDist_y = (map_y + 1.0 - vars->player->y) * deltaDist_y;
		}
		hit = 0;
		while (hit == 0)
		{
			if (sideDist_x < sideDist_y)
			{
				sideDist_x += deltaDist_x;
				map_x += step_x;
				side = 0;
			}
			else
			{
				sideDist_y += deltaDist_y;
				map_y += step_y;
				side = 1;
			}
			if (vars->map->data[map_y][map_x] == '1')
				hit = 1;
		}

		if (side == 0)
			perpWallDist = (map_x - vars->player->x + (1 - step_x) / 2) / ray_dir_x;
		else
			perpWallDist = (map_y - vars->player->y + (1 - step_y) / 2) / ray_dir_y;
		lineHeight = (int) (vars->screen->height / perpWallDist);
		drawStart = -lineHeight / 2 + vars->screen->height / 2;
		drawStart = drawStart < 0 ? 0 : drawStart;
		drawEnd = lineHeight / 2 + vars->screen->height / 2;
		drawEnd = drawEnd >= vars->screen->height ? vars->screen->height - 1 : drawEnd;

		texNum = vars->map->data[map_y][map_x] - 1;
		if (side == 0)
			wallX = vars->player->y + perpWallDist * ray_dir_y;
		else
			wallX = vars->player->x + perpWallDist * ray_dir_x;
		wallX -= floor(wallX);
		texX = (int)(wallX * (float)vars->tex->width);
		if (side == 0 && ray_dir_x > 0)
			texX = vars->tex->width = texX - 1;
		if (side == 0 && ray_dir_y < 0)
			texX = vars->tex->width - texX - 1;
		step = 1.0 * vars->tex->height / lineHeight;
		texPos = (drawStart - vars->screen->height / 2 + lineHeight / 2) * step;

		while(drawStart < drawEnd)
		{
			texY = (int)texPos & (vars->tex->height - 1);
			texPos += step;

			put_pixel(vars->img, i, drawStart, color_handler(map_x, map_y, vars, side));
			drawStart++;
		}
		i++;
	}
}

void get_map2(int fd, t_vars *vars)
{
	t_map	*map;
	t_screen *screen;
	char *buf;
	char *buf2;
	int i;
	int j;

	buf = NULL;
	buf2 = NULL;
	screen = (t_screen *)malloc(sizeof(t_screen));
	vars->screen = screen;
	get_next_line(fd, &buf);
	i = 0;
	if (buf[0] == 'R')
		i = 1;
	vars->screen->width = 0;
	vars->screen->height= 0;
	while (buf[++i] >= '0' && buf[i] <= '9')
		vars->screen->width = vars->screen->width * 10 + buf[i] - '0';
	while (buf[++i] >= '0' && buf[i] <= '9')
		vars->screen->height = vars->screen->height * 10 + buf[i] - '0';
	free(buf);
	get_next_line(fd, &buf);
	vars->tex->n = ft_strdup(&buf[3], 0);
	free(buf);
	get_next_line(fd, &buf);
	vars->tex->s = ft_strdup(&buf[3], 0);
	free(buf);
	get_next_line(fd, &buf);
	vars->tex->w = ft_strdup(&buf[3], 0);
	free(buf);
	get_next_line(fd, &buf);
	vars->tex->e = ft_strdup(&buf[3], 0);
	free(buf);
	get_next_line(fd, &buf);
	map = (t_map *)malloc(sizeof(t_map));
	map->width = ft_strlen(buf);
	map->height = 1;
	while (get_next_line(fd, &buf2))
	{
		buf = ft_strjoin(buf, buf2, 1);
		map->height++;
		free(buf2);
	}
	map->data = (char **)malloc(sizeof(char *) * map->height + 1);
	map->data[map->height] = 0;
	i = 0;
	j = 0;
	while (i < map->height)
	{
		map->data[i] = (char *)malloc(map->width + 1);
		while (j < map->width)
		{
			map->data[i][j] = *(buf++);
			j++;
		}
		map->data[i][j] = 0;
		j = 0;
		i++;
	}
	i = i * map->width;
	while (i-- > 0)
		buf--;
	if (buf2)
		free(buf2);
	if (buf)
		free(buf);
	vars->map = map;
}

void	set_minimap2(t_player *player, t_image *img, t_map *map, int d_x, int d_y)
{
	int d;
	int i;
	int k;
	int j;

	d = 20;
	k = 0;
	i = 0;
	while (i < map->height)
	{
		j = 0;
		while (j < map->width)
		{
			if (map->data[i][j] == '1')
				draw_square(img, d, d_x + (((k / d) % map->width) * d), d_y + ((((k / d)) / map->width) * d), 0xFFFFFFF);
			k += d;
			j++;
		}
		i++;
	}

	draw_square(img, 10, d_x + (player->x * d), d_y + (player->y * d), 0xFF0000);
}

int key_hook(int key_code, void *param)
{
	float tempDir;
	float tempPlane;
	t_vars *vars;

	vars = (t_vars *)param;
	if (key_code == 13)
	{
		if (vars->map->data[(int)(vars->player->y)][(int)(vars->player->x + vars->player->dir_x * SPEED)] != '1')
			vars->player->x += vars->player->dir_x * SPEED;
		if (vars->map->data[(int)(vars->player->y + vars->player->dir_y * SPEED)][(int)(vars->player->x)] != '1')
			vars->player->y += vars->player->dir_y * SPEED;
	}
	if (key_code == 1)
	{
		if (vars->map->data[(int)(vars->player->y)][(int)(vars->player->x - vars->player->dir_x * SPEED)] != '1')
			vars->player->x -= vars->player->dir_x * SPEED;
		if (vars->map->data[(int)(vars->player->y - vars->player->dir_y * SPEED)][(int)(vars->player->x)] != '1')
			vars->player->y -= vars->player->dir_y * SPEED;
	}
	if (key_code == 2)
	{
		if (vars->map->data[(int)vars->player->y][(int)(vars->player->x + vars->player->plane_x * SPEED)] != '1')
			vars->player->x += vars->player->plane_x * SPEED;
		if (vars->map->data[(int)(vars->player->y + vars->player->plane_y * SPEED)][(int)(vars->player->x)] != '1')
			vars->player->y += vars->player->plane_y * SPEED;
	}
	if (key_code == 0)
	{
		if (vars->map->data[(int)vars->player->y][(int)(vars->player->x - vars->player->plane_x * SPEED)] != '1')
			vars->player->x -= vars->player->plane_x * SPEED;
		if (vars->map->data[(int)(vars->player->y - vars->player->plane_y * SPEED)][(int)(vars->player->x)] != '1')
			vars->player->y -= vars->player->plane_y * SPEED;
	}
	if (key_code == 123)
	{
		tempDir = vars->player->dir_x;
		vars->player->dir_x = vars->player->dir_x * cos(-RSPEED) - vars->player->dir_y * sin(-RSPEED);
		vars->player->dir_y = tempDir * sin(-RSPEED) + vars->player->dir_y * cos(-RSPEED);
		tempPlane = vars->player->plane_x;
		vars->player->plane_x = vars->player->plane_x * cos(-RSPEED) - vars->player->plane_y * sin(-RSPEED);
		vars->player->plane_y = tempPlane * sin(-RSPEED) + vars->player->plane_y * cos(-RSPEED);
	}
	if (key_code == 124)
	{
		tempDir = vars->player->dir_x;
		vars->player->dir_x = vars->player->dir_x * cos(RSPEED) - vars->player->dir_y * sin(RSPEED);
		vars->player->dir_y = tempDir * sin(RSPEED) + vars->player->dir_y * cos(RSPEED);
		tempPlane = vars->player->plane_x;
		vars->player->plane_x = vars->player->plane_x * cos(RSPEED) - vars->player->plane_y * sin(RSPEED);
		vars->player->plane_y = tempPlane * sin(RSPEED) + vars->player->plane_y * cos(RSPEED);
	}
	fill_back(vars->img);
	put_image(vars);
	set_minimap2(vars->player, vars->img, vars->map, 1320, 580);
	mlx_clear_window(vars->mlx, vars->mlx_window);
	mlx_put_image_to_window(vars->mlx, vars->mlx_window, vars->img->img, 0, 0);
	return (1);
}

int main(void)
{
	int			fd;
	t_vars		*vars;

	vars = (t_vars *)malloc(sizeof(t_vars));

	vars->player = init_player(2.5, 3.5, 1, 0, 0, 0.66);
	vars->tex = init_tex(64, 64);
	fd = open("map.cub", O_RDONLY);
	get_map2(fd, vars);

//	vars->screen = new_screen(1920, 1080);
	vars->mlx = mlx_init();
	vars->img = new_image(NULL, NULL, 0, 0, 0);
	vars->img->img = mlx_new_image(vars->mlx, vars->screen->width, vars->screen->height);
	vars->img->addr = mlx_get_data_addr(vars->img->img, &vars->img->bits_per_pixel, &vars->img->size_line, &vars->img->endian);

	fill_back(vars->img);

	put_image(vars);
	set_minimap2(vars->player, vars->img, vars->map, 1320, 580);
	vars->mlx_window = mlx_new_window(vars->mlx, vars->screen->width, vars->screen->height, "cube3D");
	mlx_put_image_to_window(vars->mlx, vars->mlx_window, vars->img->img, 0, 0);
	mlx_hook(vars->mlx_window, 2, 1L<<0, key_hook, vars);
	mlx_loop(vars->mlx);
}
