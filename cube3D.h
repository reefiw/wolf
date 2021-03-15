/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3D.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 15:41:38 by plurlene          #+#    #+#             */
/*   Updated: 2021/03/12 20:10:58 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE3D_H

# define CUBE3D_H

# define SPEED 0.5
# define RSPEED 0.1

typedef struct	s_map_list
{
	char *data;
	void *next;
}				t_map_list;


typedef struct	s_image
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		bytes_pp;
	int		size_line;
	int		endian;
}				t_image;

typedef struct	s_player
{
	float	x;
	float	y;
	float	dir_x;
	float	dir_y;
	float	plane_x;
	float	plane_y;
	float	fov;
}				t_player;

typedef struct	s_map
{
	char	**data;
	int		width;
	int		height;
	int		player_x;
	int		player_y;
}				t_map;

typedef struct	s_tex
{
	int		width;
	int		height;
	void	*img;
	void	*addr;
	int		bbp;
	int		size_line;
	int		endian;
	char	*path;
}				t_tex;

typedef struct s_sprite
{
	float	x;
	float	y;
	float	len;
}				t_sprite;

typedef struct	s_screen
{
	int		width;
	int		height;
}				t_screen;

typedef struct	s_vars {
	void		*mlx;
	void		*mlx_window;
	double		*z_buffer;
	int			num_sprites;
	int			color_floor;
	int			color_ceiling;
	t_map		*map;
	t_player	player;
	t_image		img;
	t_tex		tex_sprite;
	t_tex		tex_e;
	t_tex		tex_s;
	t_tex		tex_n;
	t_tex		tex_w;
	t_tex		tex_floor;
	t_tex		tex_ceiling;
	t_screen	screen;
	t_sprite	**sprites;
}				t_vars;

typedef struct	s_dda {
	double			ray_dir_x;
	double			ray_dir_y;
	double			camera_x;
	double			sideDist_x;
	double			sideDist_y;
	double			deltaDist_x;
	double			deltaDist_y;
	double			perpWallDist;
	double			wallX;
	double			step;
	double			texPos;
	int				lineHeight;
	int				drawStart;
	int				drawEnd;
	int				map_x;
	int				map_y;
	int				step_x;
	int				step_y;
	int				hit;
	int				side;
	int				texX;
	int				texY;
	int				i;
	unsigned int	color;
	t_tex			*tex;
}					t_dda;

typedef struct	s_draw_sprites
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
}				t_draw_sprites;

typedef struct	s_floor_ceiling
{
	int				i;
	int				j;
	int				p;
	int				cellX;
	int				cellY;
	int				tx;
	int				ty;
	unsigned int	color;
	double			rayDirX0;
	double			rayDirY0;
	double			rayDirX1;
	double			rayDirY1;
	double			posZ;
	double			rowDistance;
	double			floorStepX;
	double			floorStepY;
	double			floorX;
	double			floorY;
}				t_floor_ceiling;


int			key_hook(int key_code, void *param);
void		fill_back(t_image *img, t_vars *vars);
void		put_image(t_vars *vars);
void		set_minimap2(t_player player, t_image *img, t_map *map, int d_x, int d_y);
void		new_image(t_vars *vars);
void		init_player(t_vars *vars);
void		init_tex(t_tex *tex, int width, int height);
void		put_pixel(t_image *img, int x, int y, int color);
void		put_sprites(t_vars *vars);
void		put_floor_ceiling(t_vars *vars);
t_tex		*get_tex(t_vars *vars, int side);
unsigned	int get_darker_color(unsigned int color, double size);
void		error_handler(char *str_err);
void		error_handler_clear(char *str_err, char **bfree);
void		main_parser(char *path, t_vars *vars);
void		get_img_and_add(t_vars vars, t_tex *tex);
void		do_screenshot(t_vars *vars);
void		check_err(int bool, char *err);
int			scip_whitespaces(char *str, int s);
int			scip_digits(char *str, int flag, int bias);
void		clear_str_arr(char **arr);
int			ft_isnum(char *str);
int			ft_char_num(char *str, int c);
void		color_parser(char *str, t_vars *vars, int *color);
void		texture_parser(char *str, t_tex *tex, t_vars *vars);
void		resolution_parser(char *str, t_vars *vars);
char		**get_head(int fd, t_vars *vars);
void		check_map(t_vars *vars);
void		parse_player(t_vars *vars);
void		init_sprites(t_vars *vars);


# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <math.h>
# include <mlx.h>
# include <unistd.h>
# include "GNL/get_next_line.h"
# include "libft/libft.h"

#endif
