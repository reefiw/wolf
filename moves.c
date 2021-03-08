/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 16:36:49 by plurlene          #+#    #+#             */
/*   Updated: 2021/03/08 20:22:28 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

static void	move_up_down(int key_code, t_vars *vars, float dir_x, float dir_y)
{
	if (key_code == 13)
	{
		if (vars->map->data[(int)(vars->player.y)]\
		[(int)(vars->player.x + dir_x * SPEED)] != '1')
			vars->player.x += dir_x * SPEED;
		if (vars->map->data[(int)(vars->player.y + dir_y * SPEED)]\
		[(int)(vars->player.x)] != '1')
			vars->player.y += vars->player.dir_y * SPEED;
	}
	if (key_code == 1)
	{
		if (vars->map->data[(int)(vars->player.y)]\
		[(int)(vars->player.x - dir_x * SPEED)] != '1')
			vars->player.x -= vars->player.dir_x * SPEED;
		if (vars->map->data[(int)(vars->player.y - dir_y * SPEED)]\
		[(int)(vars->player.x)] != '1')
			vars->player.y -= vars->player.dir_y * SPEED;
	}
}

static void	move_left_right(int key_code, t_vars *vars)
{
	if (key_code == 2)
	{
		if (vars->map->data[(int)vars->player.y]\
		[(int)(vars->player.x + vars->player.plane_x * SPEED)] != '1')
			vars->player.x += vars->player.plane_x * SPEED;
		if (vars->map->data[(int)(vars->player.y + \
		vars->player.plane_y * SPEED)][(int)(vars->player.x)] != '1')
			vars->player.y += vars->player.plane_y * SPEED;
	}
	if (key_code == 0)
	{
		if (vars->map->data[(int)vars->player.y]\
		[(int)(vars->player.x - vars->player.plane_x * SPEED)] != '1')
			vars->player.x -= vars->player.plane_x * SPEED;
		if (vars->map->data[(int)(vars->player.y - \
		vars->player.plane_y * SPEED)][(int)(vars->player.x)] != '1')
			vars->player.y -= vars->player.plane_y * SPEED;
	}
}

static void	rotation_by_factor(t_vars *vars, int factor)
{
	float temp_dir;
	float temp_plane;

	temp_dir = vars->player.dir_x;
	vars->player.dir_x = vars->player.dir_x * cos(RSPEED * factor)\
	- vars->player.dir_y * sin(RSPEED * factor);
	vars->player.dir_y = temp_dir * sin(RSPEED * factor)\
	+ vars->player.dir_y * cos(RSPEED * factor);
	temp_plane = vars->player.plane_x;
	vars->player.plane_x = vars->player.plane_x * cos(RSPEED * factor)\
	- vars->player.plane_y * sin(RSPEED * factor);
	vars->player.plane_y = temp_plane * sin(RSPEED * factor)\
	+ vars->player.plane_y * cos(RSPEED * factor);
}

static void	rotation_left_right(int key_code, t_vars *vars)
{
	if (key_code == 123)
	{
		rotation_by_factor(vars, -1);
	}
	if (key_code == 124)
	{
		rotation_by_factor(vars, 1);
	}
}

int			key_hook(int key_code, void *param)
{
	float	dir_x;
	float	dir_y;
	t_vars	*vars;

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
	move_up_down(key_code, vars, dir_x, dir_y);
	move_left_right(key_code, vars);
	rotation_left_right(key_code, vars);
	return (1);
}
