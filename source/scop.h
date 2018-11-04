/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avenzel <avenzel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/27 18:02:18 by avenzel           #+#    #+#             */
/*   Updated: 2018/11/04 17:19:49 by avenzel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SCOP_H
# define __SCOP_H

# ifdef __APPLE__
# define GLEW_STATIC
# include <GL/glew.h>
# else
# include <glad/glad.h> //for OpenGL header files
# endif

# include <GLFW/glfw3.h>
# include <GLFW/glfw3native.h>
# include <stdlib.h>
# include <unistd.h>

#	include <stdio.h> //TODO: delete

# include "libft.h"

# include "objrepresentation.h"

typedef struct	s_scop
{
	t_obj		object;
}				t_scop;

typedef struct	s_win
{
	int			width;
	int			height;
	const char	*title;
	GLFWwindow	*win;
}				t_win;

/*
** stores the pointers to the all allocated elements and
** last index of stored element
*/
int				g_pointers_idx;
void			*g_pointers[25];
int				add_pointer(void *ptr);

t_win			g_win;

void			print_error(const char *mess);
void			print_error_endl(const char *mess);
int				init();
int				create_window(void);

#endif
