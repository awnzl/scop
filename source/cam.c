#include "scop.h"

void mouse_movements(t_v *cam_front)
{
	GLfloat yaw = -90.0f;
	GLfloat pitch = 0.0f;
	GLfloat sensitivity = 0.01f;

	yaw += (g_scop.xpos * sensitivity);
	pitch += (-g_scop.ypos * sensitivity);// Обратный порядок вычитания потому что оконные Y-координаты возрастают с верху вниз

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	*cam_front = norm(vector(cos(pitch) * cos(yaw),
						sin(pitch), cos(pitch) * sin(yaw)));
}

void keyboard_movements(t_v *cam_pos, t_v *cam_front, t_v *cam_up)
{
	GLfloat current_frame = (GLfloat)glfwGetTime();
	g_scop.delta_time = current_frame - g_scop.last_time;
	g_scop.last_time = current_frame;

	GLfloat cam_speed = 5.0f * g_scop.delta_time;
	if (g_scop.keys[GLFW_KEY_W])
		*cam_pos = add(*cam_pos, scale_n(*cam_front, cam_speed));
	if (g_scop.keys[GLFW_KEY_S])
		*cam_pos = sub(*cam_pos, scale_n(*cam_front, cam_speed));
	if (g_scop.keys[GLFW_KEY_A])
		*cam_pos = scale_n(norm(cross(*cam_front, *cam_up)), -cam_speed);
	if (g_scop.keys[GLFW_KEY_D])
		*cam_pos = scale_n(norm(cross(*cam_front, *cam_up)), cam_speed);
	if (g_scop.keys[GLFW_KEY_Q])
		*cam_pos = sub(*cam_pos, scale_n(*cam_up, cam_speed));
	if (g_scop.keys[GLFW_KEY_E])
		*cam_pos = add(*cam_pos, scale_n(*cam_up, cam_speed));
	if (g_scop.keys[GLFW_KEY_LEFT])
		g_scop.light_x -= 0.025f;
	if (g_scop.keys[GLFW_KEY_RIGHT])
		g_scop.light_x += 0.025f;
	if (g_scop.keys[GLFW_KEY_DOWN])
		g_scop.light_y -= 0.025f;
	if (g_scop.keys[GLFW_KEY_UP])
		g_scop.light_y += 0.025f;
	if (g_scop.keys[GLFW_KEY_PAGE_DOWN])
		g_scop.light_z += 0.025f;
	if (g_scop.keys[GLFW_KEY_PAGE_UP])
		g_scop.light_z -= 0.025f;
	if (g_scop.keys[GLFW_KEY_M])
	{
		// draw in wireframe
		// glPolygonMode(GL_FRONT_AND_BACK, isPolygonMode ? GL_LINE : GL_FILL);
		// isPolygonMode = isPolygonMode ? false : true;
	}
}

GLfloat	*view_matrix()
{
	mouse_movements(&g_scop.cam_front);
	keyboard_movements(&g_scop.cam_pos, &g_scop.cam_front, &g_scop.cam_up);

	//mat4 is an array of 16 GLfloats

	return look_at(g_scop.cam_pos, add(g_scop.cam_pos, g_scop.cam_front), g_scop.cam_up);
}