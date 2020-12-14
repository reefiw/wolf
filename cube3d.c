/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 14:24:28 by plurlene          #+#    #+#             */
/*   Updated: 2020/12/14 19:58:40 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <unistd.h>
#include "cube3D.h"

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

t_player *init_player(int cord_x, int cord_y, float n_angle_view, float n_fov)
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

void	cast_ray(t_image *img, t_player *player, int *map, int view_angle, int d_y)
{
	float	i;
	float	x;
	float	y;
	int		k;

	i = 0;

	while ((int)i < 5)
	{
		x = player->x + i * cos(view_angle);
		y = player->y + i * sin(view_angle);
		if (map[(int)x + (int)y * 5] != 0)
			break ;
		i += 0.1;
	}
	k = 0;
	printf ("CHECK: %f\n", i);
	if ((int)i != 0)
		while (k++ < 1080 / (int)i)
			put_pixel(img, d_y, (1080 / 2) - ((1080 / (int)i) / 2) + k , 0x00AAAAA0);
}

void	put_image(t_image *img, t_player *player, int *map)
{
	float angle;
	float i;
	int k;

	k = 0;
	i = player->fov / 1920;
	angle = player->view_angle - player->fov / 2;
//	cast_ray(img, player, map, player->view_angle, 1920 / 2);
	while (k < 1920)
	{
		cast_ray(img, player, map, angle, k);
		k++;
		angle += i;
	}
}

int		*get_map(void)
{
	int buf[25] = {1,1,1,1,1,\
                   1,0,0,0,1,\
				   1,0,0,1,1,\
				   1,0,0,0,1,\
				   1,1,1,1,1};
	int *map;
	int i;

	i = -1;
	map = (int *)malloc(sizeof(int) * 25);
	while (++i < 25)
		map[i] = buf[i];
	return (map);
}

int main(void)
{
	void		*mlx;
	void		*mlx_window;
	int			*map;
	t_image		*img;
	t_player	*player;

	player = init_player(1, 1, 0, 1.5);
	map = get_map();

	mlx = mlx_init();

	img = new_image(NULL, NULL, 0, 0, 0);
	img->img = mlx_new_image(mlx, 1920, 1080);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, &img->size_line, &img->endian);

	fill_back(img);
	put_image(img, player, map);

	mlx_window = mlx_new_window(mlx, 1920, 1080, "cube3D");
	mlx_put_image_to_window(mlx, mlx_window, img->img, 0, 0);

	mlx_loop(mlx);
	mlx_destroy_window(mlx, mlx_window);
}
