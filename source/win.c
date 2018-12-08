#include "scop.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            g_scop.keys[key] = 1;
        else if (action == GLFW_RELEASE)
            g_scop.keys[key] = 0;
    }
}

void mouse_callback(GLFWwindow* window, double xp, double yp)
{
    g_scop.xpos = (GLfloat)xp;
    g_scop.ypos = (GLfloat)yp;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    g_scop.fov -= (GLfloat)yoffset * 5.0f;
    if (g_scop.fov <= 1.0f)
        g_scop.fov = 1.0f;
    if (g_scop.fov >= 45.0f)
        g_scop.fov = 45.0f;
}