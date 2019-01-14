#include "scop.h"

static void	glfw_error_callback(int error, const char *description)
{
	ft_putstr("GLFW::ERROR #: ");
	ft_putnbr(error);
	ft_putchar('\n');
	ft_putendl(description);
}

static void	framebuffer_size_callback(GLFWwindow *window, int width, int height)
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	return (1);
}


//TODO: move to a separeted file with callback functions
int			create_window(void)
{
	GLenum err;

	g_win.win = glfwCreateWindow(g_win.width, g_win.height, g_win.title, 0, 0);
	if (g_win.win == NULL)
	{
		print_error_endl("GLFW::CREATEWINDOW::ERROR");
		glfwTerminate();
		return (0);
	}
	glfwMakeContextCurrent(g_win.win);
	glfwSetFramebufferSizeCallback(g_win.win, framebuffer_size_callback);
	glewExperimental = GL_TRUE;
	err = glewInit();
	if (GLEW_OK != err)
	{
		print_error("GLEW::INIT::");
		print_error_endl((const char*)glewGetErrorString(err));
		glfwTerminate();
		return (0);
	}
	glfwGetFramebufferSize(g_win.win, &g_win.width, &g_win.height);
	glViewport(0, 0, g_win.width, g_win.height);
	return (1);
}

int			init(void)
{
	if (!init_glfw() || !create_window())
		return (0);
	return (1);
}
