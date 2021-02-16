/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 16:03:49 by plurlene          #+#    #+#             */
/*   Updated: 2021/02/16 15:32:05 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

static void print_header()
{
	write(2, "Error\n", 7);
}

void error_handler(char *str_err)
{
	int i;

	print_header();
	i = -1;
	while (str_err[++i])
		write(2, &str_err[i], 1);
	exit(1);
}

void error_handler_clear(char *str_err, void *bfree)
{
	free(bfree);
	error_handler(str_err);
}
