#include "scop.h"

const GLchar *vertex_code =
"#version 410 core\n\
layout (location = 0) in vec3 position;\n\
out vec4 vertexColor;\n\
\n\
// uniform mat4 view;\n\
// uniform mat4 model;\n\
// uniform mat4 projection;\n\
\n\
void main()\n\
{\n\
//    gl_Position = projection * view * vec4(position, 1.0f);\n\
    gl_Position = vec4(position, 1.0f);\n\
   vertexColor = vec4(1.0f);\
}";

const GLchar *fragment_code =
"#version 410 core\n\
in vec4 vertexColor;\n\
out vec4 color;\n\
\n\
void main()\n\
{\n\
   color = vertexColor;\n\
}";

static void		print_usage()
{
	ft_putendl("./scop file_name.obj");
}

static int		init_scop()
{
	g_pointers_idx = -1;
	g_win.width = 1024;
	g_win.height = 800;
	g_win.title = "SCOP";
    g_scop.v_databuf_size = 0;
	g_scop.f_databuf_size = 0;
    g_scop.f_databuf_pos = 0;

    g_scop.is_pol_mod = 0;
    g_scop.xpos = 0.0f;
    g_scop.ypos = 0.0f;
    g_scop.delta_time = 0.0f;
    g_scop.last_time = 0.0f;
    g_scop.fov = 45.0f;
    g_scop.light_x = 0.7f;
    g_scop.light_y = 0.7f;
    g_scop.light_z = 0.7f;
    g_scop.delta_time = 0.0f;
    g_scop.last_time = 0.0f;

    g_scop.cam_pos = vector(0.0f , 0.0f , 3.0f);
    g_scop.cam_front = vector(0.0f , 0.0f , -1.0f);
    g_scop.cam_target = vector(0.0f, 0.0f, 0.0f);
    g_scop.cam_dir = norm(sub(g_scop.cam_pos, g_scop.cam_target));
    g_scop.cam_right = norm(cross(vector(0.0f, 1.0f, 0.0f), g_scop.cam_dir));
    g_scop.cam_up = norm(cross(g_scop.cam_dir, g_scop.cam_right));

	if (!init())
		return (0);

	return (1);
}

static void		free_scop()
{
	int idx;

    ft_putendl("free_scop function is called");
	idx = -1;
	while (++idx <= g_pointers_idx)
		free(g_pointers[idx]);
    g_pointers_idx = -1;
	glfwTerminate();
    // system("leaks scop");
}

static GLuint	load_shader_program()
{
	GLuint	vertex;
    GLuint	fragment;
    GLint	success;
    GLchar	infoLog[512];
	GLuint	program_id;


	vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_code, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        //TODO: replace all messages from learnOpenGL (because pandora!!!)
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        print_error_endl("ERROR::SHADER::VERTEX::COMPILATION_FAILED");
		print_error_endl(infoLog);
    };

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_code, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        print_error_endl("ERROR::SHADER::VERTEX::COMPILATION_FAILED");
		print_error_endl(infoLog);
    };

	program_id = glCreateProgram();
    glAttachShader(program_id, vertex);
    glAttachShader(program_id, fragment);
    glLinkProgram(program_id);

    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program_id, 512, NULL, infoLog);
        print_error_endl("ERROR::SHADER::PROGRAM::LINKING_FAILED");
		print_error_endl(infoLog);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
	return program_id;
}

void			run(const char *filename)
{
    atexit(&free_scop);
	if (!init_scop())
		return ;

    glfwSetKeyCallback(g_win.win, key_callback);
    // glfwSetCursorPosCallback(g_win.win, mouse_callback);
    // glfwSetScrollCallback(g_win.win, scroll_callback);

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    GLfloat *vertices;
    GLuint *indices;
    get_object_data(filename, &vertices, &indices);

    GLuint shader_program_id = load_shader_program();
    GLuint buffer;
    GLuint vao;
    GLuint ebo;
    GLuint vbo;

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, g_scop.v_databuf_size * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_scop.f_databuf_size * sizeof(GLuint), indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);



    glBindVertexArray(vao);//нету смысла это делать каждый раз в цикле, если здесь используется только один VAO 
	while (!glfwWindowShouldClose(g_win.win))
	{
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // //матрица вида, построенная перемножением матрицы трех осей и положения камеры
        // glm::mat4 view = cam.GetViewMatrix();
        // //матрица проекции
        // glm::mat4 projection = glm::perspective(glm::radians(fov), (float)(winWidth / winHeight), 0.1f, 100.0f);

		glUseProgram(shader_program_id);
//!!!!!!!Don't forget to uncomment shader code!!! (for this matirces)
        // glUniformMatrix4fv(glGetUniformLocation(shader_program_id, "projection"), 1, GL_FALSE, &projection[0][0]);
        // glUniformMatrix4fv(glGetUniformLocation(shader_program_id, "view"), 1, GL_FALSE, &view[0][0]);
        // glUniformMatrix4fv(glGetUniformLocation(shader_program_id, "model"), 1, GL_FALSE, &model[0][0]);

        // render Cube
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, g_scop.f_databuf_size, GL_UNSIGNED_INT, (void*)0);

		glfwSwapBuffers(g_win.win);
	}
    glBindVertexArray(0);

}

int		main(int ac, char **av)
{
	if (ac < 2)
		print_usage();
	else
		run(av[1]);


	free_scop();
	return 0;
}
