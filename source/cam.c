#include "scop.h"

void keyboard_movements(t_v *cam_pos, t_v *cam_front, t_v *cam_up)
{
	GLfloat current_frame = (GLfloat)glfwGetTime();
	g_scop.delta_time = current_frame - g_scop.last_time;
	g_scop.last_time = current_frame;
	g_scop.tex_key_time += g_scop.delta_time;
	g_scop.pol_key_time += g_scop.delta_time;

	GLfloat cam_speed = 5.0f * g_scop.delta_time;
	if (g_scop.keys[GLFW_KEY_S])
		*cam_pos = add(*cam_pos, scale_n(*cam_front, cam_speed));
	if (g_scop.keys[GLFW_KEY_W])
		*cam_pos = sub(*cam_pos, scale_n(*cam_front, cam_speed));
	if (g_scop.keys[GLFW_KEY_D])
		*cam_pos = sub(*cam_pos,
						scale_n(norm(cross(*cam_up, *cam_front)), cam_speed));
	if (g_scop.keys[GLFW_KEY_A])
		*cam_pos = add(*cam_pos,
						scale_n(norm(cross(*cam_up, *cam_front)), cam_speed));
	if (g_scop.keys[GLFW_KEY_Q])
		*cam_pos = sub(*cam_pos, scale_n(*cam_up, cam_speed));
	if (g_scop.keys[GLFW_KEY_E])
		*cam_pos = add(*cam_pos, scale_n(*cam_up, cam_speed));
	if (g_scop.keys[GLFW_KEY_P] && g_scop.pol_key_time > 0.2f)
	{
		glPolygonMode(GL_FRONT_AND_BACK, g_scop.is_pol_mod ? GL_LINE : GL_FILL);
		g_scop.is_pol_mod = !g_scop.is_pol_mod;
		g_scop.pol_key_time = 0.0f;
	}
	if (g_scop.keys[GLFW_KEY_O] && g_scop.tex_key_time > 0.2f)
	{
		g_scop.is_texture = !g_scop.is_texture;
		g_scop.tex_key_time = 0.0f;
	}
	if (g_scop.keys[GLFW_KEY_C])
		g_scop.rotation_h_angle += 0.05;
	if (g_scop.keys[GLFW_KEY_F])
		g_scop.rotation_h_angle -= 0.05;
	if (g_scop.keys[GLFW_KEY_R])
		g_scop.rotation_v_angle += 0.05;
	if (g_scop.keys[GLFW_KEY_T])
		g_scop.rotation_v_angle -= 0.05;
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
** cos   0     sin   0
** 0     1     0     0
** -sin  0     cos   0
** 0     0     0     1
**
** 1     0     0     0
** 0     cos   -sin  0
** 0     sin   cos   0
** 0     0     0     1
**
*/
static void update_rotate_matrix()
{
    g_scop.rotation_v[0] = cos(g_scop.rotation_v_angle);
    g_scop.rotation_v[2] = sin(g_scop.rotation_v_angle);
    g_scop.rotation_v[5] = 1;
    g_scop.rotation_v[8] = -g_scop.rotation_v[2];
    g_scop.rotation_v[10] = g_scop.rotation_v[0];
    g_scop.rotation_v[15] = 1;
	g_scop.rotation_h[0] = 1;
	g_scop.rotation_h[5] = cos(g_scop.rotation_h_angle);
	g_scop.rotation_h[6] = -sin(g_scop.rotation_h_angle);
	g_scop.rotation_h[9] = -g_scop.rotation_h[6];
	g_scop.rotation_h[10] = g_scop.rotation_h[5];
	g_scop.rotation_h[15] = 1;
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

	fov = tan(RADIAN(g_scop.fov) / 2.0f);
	g_scop.projection[0] = (g_win.width / g_win.height) / fov;
	g_scop.projection[5] = 1.0f / fov;
	g_scop.projection[10] = 100.1f / 99.9f;
	g_scop.projection[11] = 1.0f;
	g_scop.projection[14] = -2.0f * 100.0f * 0.1f / 99.9f;
	g_scop.projection[15] = 0.0f;
}