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
# include <math.h>
# include <stdbool.h>

#	include <stdio.h> //TODO: delete

# include "libft.h"

# include "objrepresentation.h"

# define RADIAN(a) (0.01745329 * a)

/*
** x,y,z, r,g,b, tx, ty
*/
# define VERT_SIZE_MULTIPLICATOR (3 + 3 + 2)

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

typedef struct	s_face
{
	float		x;
	float		y;
	float		z;
}				t_f;

/*
**	v_databuf_size is contain a quantity of vertices (xyz) -> v_databuf_size * 3
**	f_databuf_size is contain a num of indices
*/
typedef struct	s_scope_state
{
	GLuint		v_databuf_size;//TODO: rename to num_of_vertices
	GLuint		f_databuf_size;//TODO: rename to num_of_indices
	GLuint		f_databuf_pos;//TODO: unused, rm it

	int			keys[1024];
	int			is_pol_mod; // = false;
	GLfloat		xpos; // = 0.0f;
	GLfloat		ypos; // = 0.0f;
	GLfloat		rotation_v_angle;//
	GLfloat		rotation_h_angle;//
	GLfloat		delta_time; // = 0.0f;
	GLfloat		last_time; // = 0.0f;
	GLfloat		fov; // = 45.0f;
	GLfloat		light_x; // = 0.0f;
	GLfloat		light_y; // = 0.0f;
	GLfloat		light_z; // = 0.0f;

	t_v			cam_pos;
	t_v			cam_front;
	t_v			cam_target;
	t_v			cam_dir;
	t_v			cam_right;
	t_v			cam_up;
	GLfloat		view[16];
	GLfloat		rotation_v[16];
	GLfloat		rotation_h[16];
	GLfloat		projection[16];
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
void			get_object_data(const char *filename, GLfloat **v, GLuint **i);
int				init(void);
int				create_window(void);
void			*malloc_wrp(size_t size);

void			key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void			mouse_callback(GLFWwindow* window, double xp, double yp);
void			scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void			mouse_movements();
void			keyboard_movements(t_v *cam_pos, t_v *cam_front, t_v *cam_up);

void			update_matrices();
void			proj_matrix();


/*
** vector's functions
*/
t_v				vector(GLfloat x, GLfloat y, GLfloat z);
t_v				add(t_v v1, t_v v2);
t_v				sub(t_v v1, t_v v2);
t_v				mult(t_v v1, t_v v2);
t_v				norm(t_v direct);
t_v				cross(t_v v1, t_v v2);

t_v				scale_n(t_v, GLfloat);
void			scale_s(t_v*, GLfloat);
GLfloat			dot(t_v v1, t_v v2);

#endif