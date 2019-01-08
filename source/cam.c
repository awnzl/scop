#include "scop.h"

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
		*cam_pos = sub(*cam_pos,
						scale_n(norm(cross(*cam_up, *cam_front)), cam_speed));
	if (g_scop.keys[GLFW_KEY_D])
		*cam_pos = add(*cam_pos,
						scale_n(norm(cross(*cam_up, *cam_front)), cam_speed));
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

/*
** [         g_scop.cam_right.x          g_scop.cam_up.x          g_scop.cam_dir.x  0 ]
** [         g_scop.cam_right.y          g_scop.cam_up.y          g_scop.cam_dir.y  0 ]
** [         g_scop.cam_right.z          g_scop.cam_up.z          g_scop.cam_dir.z  0 ]
** [ dot(g_scop.cam_right,-g_scop.cam_pos)  dot(g_scop.cam_up,-g_scop.cam_pos)  dot(g_scop.cam_dir,-g_scop.cam_pos)  1 ]
*/
static void update_view_matrix()
{
	g_scop.view[0] = g_scop.cam_right.x;
	g_scop.view[1] = g_scop.cam_up.x;
	g_scop.view[2] = g_scop.cam_dir.x;
	g_scop.view[4] = g_scop.cam_right.y;
	g_scop.view[5] = g_scop.cam_up.y;
	g_scop.view[6] = g_scop.cam_dir.y;
	g_scop.view[8] = g_scop.cam_right.z;
	g_scop.view[9] = g_scop.cam_up.z;
	g_scop.view[10] = g_scop.cam_dir.z;
	g_scop.view[12] = dot(add(g_scop.cam_pos, g_scop.cam_front), g_scop.cam_right);
	g_scop.view[13] = dot(add(g_scop.cam_pos, g_scop.cam_front), g_scop.cam_up);
	g_scop.view[14] = dot(add(g_scop.cam_pos, g_scop.cam_front), g_scop.cam_dir);
	g_scop.view[15] = 1;
}

/*
** Rotation
** cos   0     sin   0
** 0     1     0     0
** -sin  0     cos   0
** 0     0     0     1
*/
static void update_rotate_matrix()
{
    g_scop.rotation[0] = cos(g_scop.rotation_angle);
    g_scop.rotation[2] = sin(g_scop.rotation_angle);
    g_scop.rotation[5] = 1;
    g_scop.rotation[8] = -g_scop.rotation[2];
    g_scop.rotation[10] = g_scop.rotation[0];
    g_scop.rotation[15] = 1;
}

void	update_matrices()
{
	keyboard_movements(&g_scop.cam_pos, &g_scop.cam_front, &g_scop.cam_up);
	update_view_matrix();
	update_rotate_matrix();
}

void	proj_matrix()
{
	GLfloat fov;

	fov = tan(RADIAN(g_scop.fov) / 2);
	g_scop.projection[0] = (g_win.width / g_win.height) / fov;
	g_scop.projection[5] = 1.0f / fov;
	g_scop.projection[10] = 100.1f / 99.9f;
	g_scop.projection[11] = 1.0f;
	g_scop.projection[14] = -2.0f * 100.0f * 0.1f / 99.9f;
	g_scop.projection[15] = 0.0f;
}