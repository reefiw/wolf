/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3D.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 15:41:38 by plurlene          #+#    #+#             */
/*   Updated: 2021/01/27 20:20:42 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE3D_H

# define CUBE3D_H

# define SPEED 0.5
# define RSPEED 0.1

typedef struct	s_image
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
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
	t_map		*map;
	t_player	*player;
	t_image		*img;
	t_tex		*tex_sprite;
	t_tex		*tex_e;
	t_tex		*tex_s;
	t_tex		*tex_n;
	t_tex		*tex_w;
	t_tex		*tex_floor;
	t_tex		*tex_ceiling;
	t_screen	*screen;
	t_sprite	**sprites;
}				t_vars;

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <math.h>

#endif
