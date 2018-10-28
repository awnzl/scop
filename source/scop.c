/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: avenzel <avenzel@student.unit.ua>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/27 19:37:51 by avenzel           #+#    #+#             */
/*   Updated: 2018/10/27 19:37:54 by avenzel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scop.h"

void		print_error(const char *mess)
{
	ft_putendl(mess);
}

void		glfw_error_callback(int error, const char* description)
{
    ft_putendl(description);
}

void		framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	g_win.width = width;
	g_win.height = height;
	glViewport(0, 0, width, height);
}

static int	init_glfw(void)
{
	glfwSetErrorCallback(glfw_error_callback);

	if (!glfwInit())
	{
		print_error("GLFW::INIT::ERROR\n");
		return (0);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return (1);
}

static int	create_window(void)
{
	g_win.width = 1024;
	g_win.height = 800;
	g_win.title = "SCOP";
	g_win.win = glfwCreateWindow(g_win.width, g_win.height, g_win.title, 0, 0);

	if (g_win.win == NULL)
	{
		print_error("GLFW::CREATEWINDOW::ERROR\n");
		return (0);
	}

	glfwMakeContextCurrent(g_win.win);
	glViewport(0, 0, g_win.width, g_win.height);
	glfwSetFramebufferSizeCallback(g_win.win, framebuffer_size_callback);

	return (1);
}

int			init(void)
{
	if (!init_glfw() || !create_window())
		return (0);
	return (1);
}
