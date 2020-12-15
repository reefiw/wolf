/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 14:24:28 by plurlene          #+#    #+#             */
/*   Updated: 2020/12/16 00:57:38 by plurlene         ###   ########.fr       */
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

t_player *init_player(float cord_x, float cord_y, float n_angle_view, float n_fov)
{
	t_player *new_player;

	new_player = (t_player *)malloc(sizeof(t_player));
	new_player->x = cord_x;
	new_player->y = cord_y;
	new_player->view_angle = n_angle_view;
	new_player->fov = n_fov;
	return (new_player);
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

void	cast_ray(t_image *img, t_player *player, t_map *map, float view_angle, int d_y)
{
	float	i;
	float	x;
	float	y;
	int		k;

	i = 0.0;
//	k = ft_strlen(map->data);
	while (1)
	{
		x = player->x + (i * cos(view_angle));
		y = player->y + (i * sin(view_angle));
		if (map->data[((int)x + (int)y * map->width)] == '1') {
			printf("x: %d y: %d\n", (int)x, (int)y);
			break ;
		}
//		printf("x: %f y: %f\n", x, y);
		i += 0.001;
	}
	// printf("i: %f\n", i);
	k = -1;
	i = i > 0 && i < 1 ? 1 : i;
	int column_height = 1080 / i;
	if (i != 0)
		while (++k < column_height)
			put_pixel(img, d_y, (1080 / 2) - (column_height / 2) + k, 0x00AAAAA0);
}

void	put_image(t_image *img, t_player *player, t_map *map)
{
	float angle;
	int k;

	k = 0;
	while (++k < 1920)
	{
		angle = player->view_angle - player->fov / 2 + (player->fov * k) / 1920.0f;
		cast_ray(img, player, map, angle, k);
		// angle += i;
	}
	// cast_ray(img, player, map, player->view_angle, 1920 / 2);
}

t_map	*get_map(int fd)
{
	t_map	*map;
	char	*buf;

	buf = NULL;
	map = (t_map *)malloc(sizeof(t_map));
	map->data = (char *)malloc(1);
	map->data[0] = '\0';
	get_next_line(fd, &buf);
	map->data = ft_strjoin(map->data, buf, 1);
	map->width = ft_strlen(map->data);
	map->height = 1;
	free(buf);
	while (get_next_line(fd, &buf))
	{
		map->data = ft_strjoin(map->data, buf, 1);
		map->height++;
		free(buf);
	}
	if (buf)
		free(buf);
	return (map);
}

void	set_minimap(t_player *player, t_image *img, t_map *map, int d_x, int d_y)
{
	int d;
	int k;
	float i;
	float x;
	float y;

	k = 0;
	d = 60;
	i = 1;
	while (i <= d * 3)
	{
		x = player->x * d + d / 2 + (i * cos(player->view_angle));
		y = player->y * d + d / 2 + (i * sin(player->view_angle));
		i += 0.1;
		put_pixel(img, d_x + (int)x, d_y + (int)y, 0xFF0000);
	}
	i = 1;
	while (i <= d * 3)
	{
		x = player->x * d + d / 2 + (i * cos(player->view_angle - player->fov / 2));
		y = player->y * d + d / 2 + (i * sin(player->view_angle - player->fov / 2));
		i += 0.1;
		put_pixel(img, d_x + (int)x, d_y + (int)y, 0xA50000);
	}
	i = 1;
	while (i <= d * 3)
	{
		x = player->x * d + d / 2 + (i * cos(player->view_angle + player->fov / 2));
		y = player->y * d + d / 2 + (i * sin(player->view_angle + player->fov / 2));
		i += 0.1;
		put_pixel(img, d_x + (int)x, d_y + (int)y, 0xA50000);
	}
	while (map->data[k / d])
	{
		if (map->data[k / d] == '1')
			draw_square(img, d, d_x + (k % (map->width * d)), d_y + ((k / (map->height * d + d)) * d), 0xFFFFFFF);
		k += d;
	}
	draw_square(img, 10, d_x + (player->x * d) + 25, d_y + (player->y * d) + 25, 0xFF0000);
}

int main(void)
{
	void		*mlx;
	void		*mlx_window;
	int			fd;
	t_image		*img;
	t_map		*map;
	t_player	*player;
	const float pi = 3.14;

	player = init_player(3.5, 3.8, (7 * pi) / 4, pi / 3);

	fd = open("map.cub", O_RDONLY);
	map = get_map(fd);

	mlx = mlx_init();
	img = new_image(NULL, NULL, 0, 0, 0);
	img->img = mlx_new_image(mlx, 1920, 1080);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, &img->size_line, &img->endian);

	fill_back(img);
	put_image(img, player, map);
	set_minimap(player, img, map, 1520, 680);

	mlx_window = mlx_new_window(mlx, 1920, 1080, "cube3D");
	mlx_put_image_to_window(mlx, mlx_window, img->img, 0, 0);

	mlx_loop(mlx);
}
