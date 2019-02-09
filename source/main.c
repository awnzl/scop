#include "scop.h"

const GLchar *vertex_code =
"#version 410 core\n\
layout (location = 0) in vec3 aPosition;\n\
layout (location = 1) in vec3 aColor;\n\
layout (location = 2) in vec2 aTexCoord;\n\
flat out vec4 vertexColor;\n\
out vec2 txCoord;\n\
\n\
uniform mat4 view;\n\
// uniform mat4 model;\n\
uniform mat4 projection;\n\
uniform mat4 rot_v;\n\
uniform mat4 rot_h;\n\
\n\
\n\
void main()\n\
{\n\
    vec4 rot = rot_h * vec4(aPosition, 1.0f);\n\
    rot *= rot_v;\n\
    gl_Position = projection * view * rot;\n\
    txCoord = aTexCoord;\n\
    vertexColor = vec4(aColor, 1.0f);\n\
}";

const GLchar *fragment_code =
"#version 410 core\n\
uniform sampler2D txSampler;\n\
uniform bool isTexture;\n\
flat in vec4 vertexColor;\n\
in vec2 txCoord;\n\
out vec4 color;\n\
\n\
void main()\n\
{\n\
    color = isTexture ? texture(txSampler, txCoord) : vertexColor;\n\
}";

static int		init_scop()
{
	g_pointers_idx = -1;
	g_win.width = 640;
	g_win.height = 640;
	g_win.title = "SCOP";
    g_scop.v_databuf_size = 0;
	g_scop.f_databuf_size = 0;

    g_scop.is_pol_mod = false;
    g_scop.is_texture = false;
    g_scop.xpos = 0.0f;
    g_scop.ypos = 0.0f;
    g_scop.rotation_v_angle = RADIAN(0.0f);
    g_scop.rotation_h_angle = RADIAN(0.0f);
    g_scop.fov = 45.0f;
    g_scop.delta_time = 0.0f;
    g_scop.last_time = 0.0f;
    g_scop.tex_key_time = 1.0f;
    g_scop.pol_key_time = 1.0f;

    g_scop.cam_pos = vector(0.0f , 0.0f , 8.0f);
    g_scop.cam_front = norm(vector(0.0f , 0.0f , -1.0f));
    g_scop.cam_target = norm(vector(0.0f, 0.0f, 1.0f));
    g_scop.cam_dir = norm(sub(g_scop.cam_pos, g_scop.cam_target));
    g_scop.cam_right = norm(cross(vector(0.0f, 1.0f, 0.0f), g_scop.cam_dir));
    g_scop.cam_up = norm(cross(g_scop.cam_dir, g_scop.cam_right));
    ft_memset(g_scop.view, 0, sizeof(GLfloat) * 16);
    ft_memset(g_scop.rotation_v, 0, sizeof(GLfloat) * 16);
    ft_memset(g_scop.rotation_h, 0, sizeof(GLfloat) * 16);
    ft_memset(g_scop.projection, 0, sizeof(GLfloat) * 16);
    proj_matrix();

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
    // system("leaks Scop");
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
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        print_error_endl("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED");
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

/*
** moves vertices' coordinates to the needed positions and copies
** colors values to the needed positions
**
** incoming vertices' array contains all data in one row
** outcoming array contain three values for vertices' coordinates,
** three values for cololr and two not set cells for texture coordinates
*/
static void     colored_data(GLfloat *vertices, GLfloat *colors)//colors if need
{
    int idx;
    int vidx;
    GLfloat tmp[g_scop.v_databuf_size * VERT_SIZE_MULTIPLICATOR];

    idx = 0;
    vidx = 0;//index for vertices
    int mark = 0;
    float key = 0;
    while (idx < g_scop.v_databuf_size * VERT_SIZE_MULTIPLICATOR)
    {
        tmp[idx++] = vertices[vidx++];
        tmp[idx++] = vertices[vidx++];
        tmp[idx++] = vertices[vidx++];

        // key = ((float)rand())/(float)(RAND_MAX) * .8;
        // key = 0.3 + key * 0.5;
        // tmp[idx++] = key;
        // tmp[idx++] = key;
        // tmp[idx++] = key;
        int itmp = idx;
        tmp[idx++] = (1.0f - tmp[itmp - 2]) / 2.0f + 0.2f;
        tmp[idx++] = (1.0f - tmp[itmp - 1]) / 2.0f + 0.2f;
        tmp[idx++] = (1.0f - tmp[itmp]) / 2.0f + 0.2f;
        // if (toggle)
        // {
        //     tmp[idx++] = .1f;
        //     tmp[idx++] = .1f;
        //     tmp[idx++] = .1f;

        //     toggle = false;
        // }
        // else
        // {

        //     tmp[idx++] = .8f;
        //     tmp[idx++] = .8f;
        //     tmp[idx++] = .8f;
        //     toggle = true;
        // }

        // if (mark == 0) //grayscaling
        // {
        //     tmp[idx++] = .1f;
        //     tmp[idx++] = .1f;
        //     tmp[idx++] = .1f;
        //     mark++;
        // }
        // else if (mark == 1)
        // {
        //     tmp[idx++] = .8f;
        //     tmp[idx++] = .8f;
        //     tmp[idx++] = .8f;
        //     mark++;
        // }
        // else if (mark == 2)
        // {
        //     tmp[idx++] = .5f;
        //     tmp[idx++] = .5f;
        //     tmp[idx++] = .5f;
        //     mark = 0;
        // }
        idx += 2;
    }

    while (--idx >= 0)
        vertices[idx] = tmp[idx];
}

static void     texture_coordinates(GLfloat *vertices)
{
    GLuint  idx;

    idx = 6;
    while (idx < g_scop.v_databuf_size * VERT_SIZE_MULTIPLICATOR)
    {
        vertices[idx] = 0.5 + atan2f(vertices[idx - 4], vertices[idx - 6]) / (2.0 * M_PI);
        vertices[idx + 1] = acosf(vertices[idx - 5] /
            sqrt(vertices[idx - 6] * vertices[idx - 6] +
            vertices[idx - 5] * vertices[idx - 5] +
            vertices[idx - 4] * vertices[idx - 4])) / M_PI;
        idx += 8;
    }
}

void            get_colors(GLfloat **colors)
{
    int idx;

    *colors = (GLfloat*)malloc_wrp(sizeof(GLfloat) * g_scop.v_databuf_size * 3);
    idx = 0;
    while (idx < g_scop.v_databuf_size * 3)
    {
        (*colors)[idx++] = 0.8f;
        (*colors)[idx++] = 0.5f;
        (*colors)[idx++] = 0.1f;
    }
}

/*
** shifts the starting point to the center of the object along given axis
** int nom_of_axis: 0 - x, 1 - y, 2 - z
*/
static void     centralize(GLfloat *vertices, int axis_number)
{
    double  min;
    double  max;
    double  median;
    int     idx;

    min = __DBL_MAX__;
    max = -__DBL_MAX__;
    idx = axis_number;
    while (idx < g_scop.v_databuf_size * 3)
    {
        if (vertices[idx] > max)
            max = vertices[idx];
        if (vertices[idx] < min)
            min = vertices[idx];
        idx += 3;
    }
    median = (min + max) / 2.0;
    idx = axis_number;
    while (idx < g_scop.v_databuf_size * 3)
    {
        vertices[idx] = vertices[idx] - median;
        idx += 3;
    }
}

static GLubyte  *texture_image(GLuint *w, GLuint *h)
{
    *w = 300;
    *h = 300;

    FILE *file = fopen("./resources/texture_pixmap","rb");

    if(file == NULL)
    {
        printf("Error!");
        exit(1);
    }

    GLubyte buf[4];
    GLubyte *ret = malloc_wrp(*w * *h * 4);
    GLuint  ridx = 0;
    GLuint  bytes;
    while ((bytes = fread(buf, 1, sizeof buf, file)) > 0)
    {
        int idx = -1;
        while (++idx < bytes)
            ret[ridx++] = buf[idx];
    }

    fclose(file);

    return ret;
}

void			run(const char *filename)
{
    atexit(&free_scop);
	if (!init_scop())
		return ;

    glfwSetInputMode(g_win.win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(g_win.win, key_callback);
    glfwSetCursorPosCallback(g_win.win, mouse_callback);
    // glfwSetScrollCallback(g_win.win, scroll_callback);

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    GLfloat *vertices;
    GLuint *indices;
    get_object_data(filename, &vertices, &indices);
    centralize(vertices, 2);
    centralize(vertices, 1);

    // array for the colors data. Need to implement function, wich will be
    // store colors data depending on source of color info
    GLfloat *colors;
    // get_colors(colors);//TODO: implement
    // this data will be needed for colored_data()

    printf("vsize: %i; fnum: %i, verts' array size: %i\n", g_scop.v_databuf_size, g_scop.f_databuf_size, g_scop.f_databuf_size * VERT_SIZE_MULTIPLICATOR);

    GLuint shader_program_id = load_shader_program();
    GLuint buffer;
    GLuint vao;
    GLuint ebo;
    GLuint vbo;
    GLuint textr;

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);
    glGenTextures(1, &textr);

    glBindTexture(GL_TEXTURE_2D, textr);
    // texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLuint width;
    GLuint height;
    GLubyte *image = texture_image(&width, &height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    colored_data(vertices, colors);// function to fill vertices and color info into TODO: implement copying from colors
    texture_coordinates(vertices);
    glBufferData(GL_ARRAY_BUFFER, g_scop.v_databuf_size * VERT_SIZE_MULTIPLICATOR * sizeof(GLfloat), vertices, GL_STATIC_DRAW);


    //vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERT_SIZE_MULTIPLICATOR * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERT_SIZE_MULTIPLICATOR * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    //texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, VERT_SIZE_MULTIPLICATOR * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_scop.f_databuf_size * sizeof(GLuint), indices, GL_STATIC_DRAW);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    glBindTexture(GL_TEXTURE_2D, textr);
    glBindVertexArray(vao);
	while (!glfwWindowShouldClose(g_win.win))
	{
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);

        update_matrices();

		glUseProgram(shader_program_id);
        glUniformMatrix4fv(glGetUniformLocation(shader_program_id, "projection"), 1, GL_FALSE, g_scop.projection);
        glUniformMatrix4fv(glGetUniformLocation(shader_program_id, "view"), 1, GL_FALSE, g_scop.view);
        glUniformMatrix4fv(glGetUniformLocation(shader_program_id, "rot_v"), 1, GL_FALSE, g_scop.rotation_v);
        glUniformMatrix4fv(glGetUniformLocation(shader_program_id, "rot_h"), 1, GL_FALSE, g_scop.rotation_h);
        glUniform1i(glGetUniformLocation(shader_program_id, "isTexture"), g_scop.is_texture);

        glDrawElements(GL_TRIANGLES, g_scop.f_databuf_size, GL_UNSIGNED_INT, (void*)0);

		glfwSwapBuffers(g_win.win);
	}
    glBindVertexArray(0);

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

}

int		main(int ac, char **av)
{
	if (ac < 2)
		ft_putendl("./scop file_name.obj");
	else
		run(av[1]);


	free_scop();
	return 0;
}
