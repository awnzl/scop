/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avenzel <avenzel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/27 18:02:18 by avenzel           #+#    #+#             */
/*   Updated: 2018/11/24 14:51:24 by avenzel          ###   ########.fr       */
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

typedef struct	s_vertex
{
	float		x;
	float		y;
	float		z;
	// float		w;
}				t_v;

typedef struct	s_vertex_normal
{
	float		i;
	float		j;
	float		k;
}				t_vn;

typedef struct	s_texture_vertex
{
	float		u;
	float		v;
	float		w;
}				t_vt;

/*
  f 1 2 3
  f 3/1 4/2 5/3
  f 6/4/1 3/5/3 7/6/5
  f 6//1 3//3 7//5
  ???
  ниже структура не верная.
  face может состоять из трёх и более индексов вершин (смотри вики)
  плюс нужно сохранить и остальную информацию, если есть
 */
typedef struct	s_face
{
	float		x;
	float		y;
	float		z;
}				t_f;

typedef struct	s_scope_state
{
	GLuint		v_databuf_size;
	GLuint		f_databuf_size;
	GLuint		f_databuf_pos;
}				t_state;

/*
** stores the pointers to the all allocated elements and
** last index of stored element and they will be deleted
** in case of abort (if I will implement this :D)
*/
int				g_pointers_idx;
void			*g_pointers[1024];
int				add_pointer(void *ptr);
/*
** or do not use this idea?
*/

t_win			g_win;
t_state			g_scop;

void			print_error(const char *mess);
void			print_error_endl(const char *mess);
void			get_object_data(const char *filename);
int				init();
int				create_window(void);
void			*malloc_wrp(size_t size);

#endif
