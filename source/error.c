/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avenzel <avenzel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/04 16:51:55 by avenzel           #+#    #+#             */
/*   Updated: 2018/11/04 16:52:17 by avenzel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void	print_error_endl(const char *s)
{
	if (s)
	{
		while (*s)
			write(2, s++, 1);
		write(2, "\n", 1);
	}
}

void	print_error(const char *s)
{
	if (s)
		while (*s)
			write(2, s++, 1);
}
