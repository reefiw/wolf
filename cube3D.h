/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube3D.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 15:41:38 by plurlene          #+#    #+#             */
/*   Updated: 2020/12/16 00:44:24 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUBE3D_H

# define CUBE3D_H

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
	float	view_angle;
	float	fov;
}				t_player;

typedef struct	s_map
{
	char	*data;
	int		width;
	int		height;
}				t_map;

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <math.h>

#endif
