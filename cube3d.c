/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 14:24:28 by plurlene          #+#    #+#             */
/*   Updated: 2021/01/28 18:53:15 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <unistd.h>
#include "cube3D.h"
#include "GNL/get_next_line.h"

void new_image(t_vars *vars)
{
	vars->img.img = NULL;
	vars->img.addr = NULL;
	vars->img.bits_per_pixel = 0;
	vars->img.size_line = 0;
	vars->img.endian = 0;
}

void init_player(t_vars *vars)
{
	vars->player.x = 2.5;
	vars->player.y = 3.5;
	vars->player.dir_x = 1;
	vars->player.dir_y = 0;
	vars->player.plane_x = 0;
	vars->player.plane_y = 0.66;
}

void init_tex(t_tex *tex, int width, int height)
{
	tex->width = width;
	tex->height = height;
	tex->addr = NULL;
	tex->bbp = 0;
	tex->size_line = 0;
	tex->img = NULL;
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
		x = vars->player.x + (i * cos(dir_angle));
		y = vars->player.y + (i * sin(dir_angle));
		if (vars->map->data[(int)y][(int)x] == '1')
			break ;
		i += 0.01;
	}
	k = -1;
	i = i > 0 && i < 1 ? 1 : i;
	int column_height = 1080 / i;
	if (i != 0)
		while (++k < column_height)
			put_pixel(&vars->img, d_y, (1080 / 2) - (column_height / 2) + k, 0x00AAAAA0);
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
		return	(&vars->tex_n);
	if (side == 1)
		return (&vars->tex_s);
	if (side == 2)
		return (&vars->tex_w);
	if (side == 3)
		return (&vars->tex_e);
	return (&vars->tex_e);
}

void	put_sprites(t_vars *vars)
{
	t_sprite	*temp_sprite;
	int			i;
	int			j;
	int			d;
	int			stripe;
	int			spriteScreenX;
	int			spriteHeight;
	int			spriteWidth;
	int			drawStartY;
	int			drawStartX;
	int			drawEndX;
	int			drawEndY;
	int			texX;
	int			texY;
	double		spriteX;
	double		spriteY;
	double		invDet;
	double		transformX;
	double		transformY;
	unsigned int	color;

	i = 0;
	while (i < vars->num_sprites)
	{
		vars->sprites[i]->len = (vars->player.x - vars->sprites[i]->x) * (vars->player.x - vars->sprites[i]->x)\
		+ (vars->player.y - vars->sprites[i]->y) * (vars->player.y - vars->sprites[i]->y);
		i++;
	}
	i = 0;
	while (i < vars->num_sprites)
	{
		j = vars->num_sprites - 1;
		while (j > 0)
		{
			if (vars->sprites[j]->len > vars->sprites[j - 1]->len)
			{
				temp_sprite = vars->sprites[j - 1];
				vars->sprites[j - 1] = vars->sprites[j];
				vars->sprites[j] =  temp_sprite;
			}
			j--;
		}
		i++;
	}
	i = 0;
	while (i < vars->num_sprites)
	{
		spriteX = vars->sprites[i]->x - vars->player.x;
		spriteY = vars->sprites[i]->y - vars->player.y;
		invDet = 1.0 / (vars->player.plane_x * vars->player.dir_y - vars->player.dir_x * vars->player.plane_y);
		transformX = invDet * (vars->player.dir_y * spriteX - vars->player.dir_x * spriteY);
		transformY = invDet * (-vars->player.plane_y * spriteX + vars->player.plane_x * spriteY);
		spriteScreenX = (int)((vars->screen.width / 2) * (1 + transformX / transformY));
		spriteHeight = abs((int)(vars->screen.height / transformY));
		drawStartY = -spriteHeight / 2 + vars->screen.height / 2;
		if (drawStartY < 0)
			drawStartY = 0;
		drawEndY = spriteHeight / 2 + vars->screen.height / 2;
		if (drawEndY >= vars->screen.height)
			drawEndY = vars->screen.height - 1;
		spriteWidth = abs((int)(vars->screen.height / transformY));
		drawStartX = -spriteWidth / 2 + spriteScreenX;
		if (drawStartX < 0)
			drawStartX = 0;
		drawEndX = spriteWidth / 2 + spriteScreenX;
		if (drawEndX >= vars->screen.width)
			drawEndX = vars->screen.width - 1;
		stripe = drawStartX;
		while (stripe < drawEndX)
		{
			texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * vars->tex_sprite.width / spriteWidth) / 256;
			if (transformY > 0 && stripe > 0 && stripe < vars->screen.width && transformY < vars->z_buffer[stripe])
			{
				j = drawStartY;
				while (j < drawEndY)
				{
					d = j * 256 - vars->screen.height * 128 + spriteHeight * 128;
					texY = ((d * vars->tex_sprite.height) / spriteHeight) / 256;
					color = *(unsigned int *)(vars->tex_sprite.addr + (texY * vars->tex_sprite.size_line + texX * (vars->tex_sprite.bbp / 8)));
					if ((color & 0x00FFFFFF) != 0)
						put_pixel(&vars->img, stripe, j, color);
					j++;
				}
			}
			stripe++;
		}
		i++;
	}
}

void	put_floor_ceiling(t_vars *vars)
{
	int i;
	int j;
	int p;
	int cellX;
	int cellY;
	int tx;
	int ty;
	unsigned int color;

	double rayDirX0;
	double rayDirY0;
	double rayDirX1;
	double rayDirY1;
	double posZ;
	double rowDistance;
	double floorStepX;
	double floorStepY;
	double floorX;
	double floorY;

	rayDirX0 = vars->player.dir_x - vars->player.plane_x;
	rayDirY0 = vars->player.dir_y - vars->player.plane_y;
	rayDirX1 = vars->player.dir_x + vars->player.plane_x;
	rayDirY1 = vars->player.dir_y + vars->player.plane_y;

	posZ = 0.5 * vars->screen.height / 2;
	i = 0;
	while (i < vars->screen.height)
	{
		p = i - vars->screen.height / 2;
		rowDistance = posZ / p;
		floorStepX = rowDistance * (rayDirX1 - rayDirX0) / vars->screen.width;
		floorStepY = rowDistance * (rayDirY1 - rayDirY0) / vars->screen.width;
		floorX = vars->player.x + rowDistance * rayDirX0;
		floorY = vars->player.y + rowDistance * rayDirY0;
		j = 0;
		while (j < vars->screen.width)
		{
			cellX = (int)floorX;
			cellY = (int)floorY;
			tx = (int)(vars->tex_floor.width * (floorX - cellX)) & (vars->tex_floor.width - 1);
			ty = (int)(vars->tex_floor.height * (floorY - cellY)) & (vars->tex_floor.height - 1);
			floorX += floorStepX;
			floorY += floorStepY;
			color = *(unsigned int *)(vars->tex_floor.addr + (ty * vars->tex_floor.size_line + tx * (vars->tex_floor.bbp / 8)));
			color = get_darker_color(color, rowDistance);
			put_pixel(&vars->img, j, i, color);
			color = *(unsigned int *)(vars->tex_ceiling.addr + (ty * vars->tex_ceiling.size_line + tx * (vars->tex_ceiling.bbp / 8)));
			color = get_darker_color(color, rowDistance);
			put_pixel(&vars->img, j, vars->screen.height - i - 1, color);
			j++;
		}
		i++;
	}
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
	int		texX;
	int		texY;
	int i;
	unsigned int color;
	t_tex	*tex;
	put_floor_ceiling(vars);

	i = 0;
	while (i < vars->screen.width)
	{
		camera_x = 2 * i / (double)vars->screen.width - 1;
		ray_dir_x = vars->player.dir_x + vars->player.plane_x * camera_x;
		ray_dir_y = vars->player.dir_y + vars->player.plane_y * camera_x;

		map_x = (int)vars->player.x;
		map_y = (int)vars->player.y;

		deltaDist_x = fabs(1 / ray_dir_x);
		deltaDist_y = fabs(1 / ray_dir_y);

		if (ray_dir_x < 0)
		{
			step_x = -1;
			sideDist_x = (vars->player.x - map_x) * deltaDist_x;
		}
		else
		{
			step_x = 1;
			sideDist_x = (map_x + 1.0 - vars->player.x) * deltaDist_x;
		}
		if (ray_dir_y < 0)
		{
			step_y = -1;
			sideDist_y = (vars->player.y - map_y) * deltaDist_y;
		}
		else
		{
			step_y = 1;
			sideDist_y = (map_y + 1.0 - vars->player.y) * deltaDist_y;
		}
		hit = 0;
		while (hit == 0)
		{
			if (sideDist_x < sideDist_y)
			{
				side = 0;
				sideDist_x += deltaDist_x;
				map_x += step_x;
			}
			else
			{
				side = 1;
				sideDist_y += deltaDist_y;
				map_y += step_y;
			}
			if (vars->map->data[map_y][map_x] == '1')
				hit = 1;
		}

		if (side == 0)
			perpWallDist = (map_x - vars->player.x + (1 - step_x) / 2) / ray_dir_x;
		else
			perpWallDist = (map_y - vars->player.y + (1 - step_y) / 2) / ray_dir_y;
		lineHeight = (int) (vars->screen.height / perpWallDist);
		drawStart = -lineHeight / 2 + vars->screen.height / 2;
		drawStart = drawStart < 0 ? 0 : drawStart;
		drawEnd = lineHeight / 2 + vars->screen.height / 2;
		drawEnd = drawEnd >= vars->screen.height ? vars->screen.height - 1 : drawEnd;

		if (side == 0)
			wallX = vars->player.y + perpWallDist * ray_dir_y;
		else
			wallX = vars->player.x + perpWallDist * ray_dir_x;
		wallX -= floor(wallX);
		if (side == 0)
			if (ray_dir_x < 0)
				side = 2;
		if (side == 1)
			if (ray_dir_y < 0)
				side = 3;
		tex = get_tex(vars, side);
		texX = (int)(wallX * (float)tex->width);
		if ((side == 0 || side == 2) && ray_dir_x > 0)
			texX = tex->width - texX - 1;
		if ((side == 1 || side == 3) && ray_dir_y < 0)
			texX = tex->width - texX - 1;
		step = 1.0 * tex->height / lineHeight;
		texPos = (drawStart - vars->screen.height / 2 + lineHeight / 2) * step;
		while(drawStart < drawEnd)
		{
			texY = (int)texPos & (tex->height - 1);
			color = *(unsigned int *)(tex->addr + (texY * tex->size_line + texX * (tex->bbp / 8)));
			texPos += step;
			color = get_darker_color(color, perpWallDist / 1.5);
			put_pixel(&vars->img, i, drawStart, color);
			drawStart++;
		}
		vars->z_buffer[i] = perpWallDist;
		i++;
	}
	put_sprites(vars);
}

void get_map2(int fd, t_vars *vars)
{
	t_map	*map;
	char *buf;
	char *buf2;
	int i;
	int j;

	buf = NULL;
	buf2 = NULL;
	get_next_line(fd, &buf);
	i = 0;
	if (buf[0] == 'R')
		i = 1;
	vars->screen.width = 0;
	vars->screen.height= 0;
	while (buf[++i] >= '0' && buf[i] <= '9')
		vars->screen.width = vars->screen.width * 10 + buf[i] - '0';
	while (buf[++i] >= '0' && buf[i] <= '9')
		vars->screen.height = vars->screen.height * 10 + buf[i] - '0';
	free(buf);
	vars->z_buffer = (double *)malloc(sizeof(double) * vars->screen.width);
	get_next_line(fd, &buf);
	vars->tex_n.path = ft_strdup(&buf[3], 0);
	free(buf);
	get_next_line(fd, &buf);
	vars->tex_s.path = ft_strdup(&buf[3], 0);
	free(buf);
	get_next_line(fd, &buf);
	vars->tex_w.path = ft_strdup(&buf[3], 0);
	free(buf);
	get_next_line(fd, &buf);
	vars->tex_e.path = ft_strdup(&buf[3], 0);
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

void	init_sprites(t_vars *vars)
{
	int i;
	int j;
	int len;

	i = 0;
	len = 0;
	while (i < vars->map->height)
	{
		j = 0;
		while (j < vars->map->width)
		{
			if (vars->map->data[i][j] == '2')
				len++;
			j++;
		}
		i++;
	}
	vars->sprites = (t_sprite **)malloc(sizeof(t_sprite *) * (len + 1));
	vars->sprites[len] = NULL;
	vars->num_sprites = len;
	i = 0;
	len = 0;
	while (i < vars->map->height)
	{
		j = 0;
		while (j < vars->map->width)
		{
			if (vars->map->data[i][j] == '2')
			{
				vars->sprites[len] = (t_sprite *)malloc(sizeof(t_sprite));
				vars->sprites[len]->x = j + 0.5;
				vars->sprites[len]->y = i + 0.5;
				vars->sprites[len]->len = 0;
				len++;
			}
			j++;
		}
		i++;
	}
}

void	set_minimap2(t_player player, t_image *img, t_map *map, int d_x, int d_y)
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

	draw_square(img, 10, d_x + (player.x * d), d_y + (player.y * d), 0xFF0000);
}

int key_hook(int key_code, void *param)
{
	float tempDir;
	float tempPlane;
	float dir_x;
	float dir_y;
	t_vars *vars;


	vars = (t_vars *)param;
	dir_x = vars->player.dir_x;
	dir_y = vars->player.dir_y;
	if (vars->player.dir_x == 1)
		dir_x -= 0.1;
	if (vars->player.dir_y == 1)
		dir_y -= 0.1;
	if (vars->player.dir_x == -1)
		dir_x += 0.1;
	if (vars->player.dir_y == -1)
		dir_y += 0.1;
	if (key_code == 13)
	{
		if (vars->map->data[(int)(vars->player.y)][(int)(vars->player.x + dir_x * SPEED)] != '1')
			vars->player.x += dir_x * SPEED;
		if (vars->map->data[(int)(vars->player.y + dir_y * SPEED)][(int)(vars->player.x)] != '1')
			vars->player.y += vars->player.dir_y * SPEED;
	}
	if (key_code == 1)
	{
		if (vars->map->data[(int)(vars->player.y)][(int)(vars->player.x - dir_x * SPEED)] != '1')
			vars->player.x -= vars->player.dir_x * SPEED;
		if (vars->map->data[(int)(vars->player.y - dir_y * SPEED)][(int)(vars->player.x)] != '1')
			vars->player.y -= vars->player.dir_y * SPEED;
	}
	if (key_code == 2)
	{
		if (vars->map->data[(int)vars->player.y][(int)(vars->player.x + vars->player.plane_x * SPEED)] != '1')
			vars->player.x += vars->player.plane_x * SPEED;
		if (vars->map->data[(int)(vars->player.y + vars->player.plane_y * SPEED)][(int)(vars->player.x)] != '1')
			vars->player.y += vars->player.plane_y * SPEED;
	}
	if (key_code == 0)
	{
		if (vars->map->data[(int)vars->player.y][(int)(vars->player.x - vars->player.plane_x * SPEED)] != '1')
			vars->player.x -= vars->player.plane_x * SPEED;
		if (vars->map->data[(int)(vars->player.y - vars->player.plane_y * SPEED)][(int)(vars->player.x)] != '1')
			vars->player.y -= vars->player.plane_y * SPEED;
	}
	if (key_code == 123)
	{
		tempDir = vars->player.dir_x;
		vars->player.dir_x = vars->player.dir_x * cos(-RSPEED) - vars->player.dir_y * sin(-RSPEED);
		vars->player.dir_y = tempDir * sin(-RSPEED) + vars->player.dir_y * cos(-RSPEED);
		tempPlane = vars->player.plane_x;
		vars->player.plane_x = vars->player.plane_x * cos(-RSPEED) - vars->player.plane_y * sin(-RSPEED);
		vars->player.plane_y = tempPlane * sin(-RSPEED) + vars->player.plane_y * cos(-RSPEED);
	}
	if (key_code == 124)
	{
		tempDir = vars->player.dir_x;
		vars->player.dir_x = vars->player.dir_x * cos(RSPEED) - vars->player.dir_y * sin(RSPEED);
		vars->player.dir_y = tempDir * sin(RSPEED) + vars->player.dir_y * cos(RSPEED);
		tempPlane = vars->player.plane_x;
		vars->player.plane_x = vars->player.plane_x * cos(RSPEED) - vars->player.plane_y * sin(RSPEED);
		vars->player.plane_y = tempPlane * sin(RSPEED) + vars->player.plane_y * cos(RSPEED);
	}
	fill_back(&vars->img);
	put_image(vars);
	set_minimap2(vars->player, &vars->img, vars->map, 1320, 580);
	mlx_clear_window(vars->mlx, vars->mlx_window);
	mlx_put_image_to_window(vars->mlx, vars->mlx_window, vars->img.img, 0, 0);
	return (1);
}

void get_img_and_add(t_vars vars, t_tex *tex)
{
	tex->img = mlx_xpm_file_to_image(vars.mlx, tex->path, &tex->width, &tex->height);
	tex->addr = mlx_get_data_addr(tex->img, &tex->bbp, &tex->size_line, &tex->endian);
}

int main(void)
{
	int			fd;
	t_vars		vars;

	init_player(&vars);
	init_tex(&vars.tex_e, 64, 64);
	init_tex(&vars.tex_s, 64, 64);
	init_tex(&vars.tex_w, 64, 64);
	init_tex(&vars.tex_n, 64, 64);
	init_tex(&vars.tex_floor, 64, 64);
	init_tex(&vars.tex_ceiling, 64, 64);
	init_tex(&vars.tex_sprite, 64, 64);
	vars.tex_sprite.path = "./textures/barrel.xpm";
	vars.tex_floor.path = "./textures/mossy.xpm";
	vars.tex_ceiling.path = "./textures/wood.xpm";

	fd = open("map.cub", O_RDONLY);
	get_map2(fd, &vars);
	init_sprites(&vars);
	vars.mlx = mlx_init();
	new_image(&vars);
	vars.img.img = mlx_new_image(vars.mlx, vars.screen.width, vars.screen.height);
	vars.img.addr = mlx_get_data_addr(vars.img.img, &vars.img.bits_per_pixel, &vars.img.size_line, &vars.img.endian);

	get_img_and_add(vars, &vars.tex_e);
	get_img_and_add(vars, &vars.tex_s);
	get_img_and_add(vars, &vars.tex_w);
	get_img_and_add(vars, &vars.tex_n);
	get_img_and_add(vars, &vars.tex_sprite);
	get_img_and_add(vars, &vars.tex_ceiling);
	get_img_and_add(vars, &vars.tex_floor);

	fill_back(&vars.img);

	put_image(&vars);
	set_minimap2(vars.player, &vars.img, vars.map, 1320, 580);
	vars.mlx_window = mlx_new_window(vars.mlx, vars.screen.width, vars.screen.height, "cube3D");
	mlx_put_image_to_window(vars.mlx, vars.mlx_window, vars.img.img, 0, 0);
	mlx_hook(vars.mlx_window, 2, 1L<<0, key_hook, &vars);
	mlx_loop(vars.mlx);
}
