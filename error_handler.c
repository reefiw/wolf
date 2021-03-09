/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plurlene <plurlene@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/11 16:03:49 by plurlene          #+#    #+#             */
/*   Updated: 2021/03/09 17:14:53 by plurlene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

static void print_header()
{
	write(1, "Error\n", 7);
}

void error_handler(char *str_err)
{
	int i;

	print_header();
	i = -1;
	while (str_err[++i])
		write(1, &str_err[i], 1);
	exit(0);
}

void error_handler_clear(char *str_err, char **bfree)
{
	int i;

	i = 0;
	while (bfree[i])
	{
		free(bfree[i]);
		i++;
	}
	free(bfree[i]);
	free(bfree);
	error_handler(str_err);
}
