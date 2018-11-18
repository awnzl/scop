#include "scop.h"

const GLchar *vertex_code =
"#version 410 core\n\
layout (location = 0) in vec3 position;\n\
layout (location = 1) in vec3 col;\n\
out vec4 vertexColor;\n\
\n\
void main()\n\
{\n\
   gl_Position = vec4(position, 1.0f);\n\
   vertexColor = vec4(col, 1.0f);\
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

	if (!init())
		return (0);

	return (1);
}

static int		free_scop()
{
	int idx;

	idx = -1;
	while (++idx <= g_pointers_idx)
		free(g_pointers[idx]);

	glfwTerminate();
	return (1);
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

    // Удаляем шейдеры, поскольку они уже добавлены в программу и нам больше не нужны.
    glDeleteShader(vertex);
    glDeleteShader(fragment);
	return program_id;
}

void			run(const char *filename)
{
	if (!init_scop())
		return ;

    get_object_data(filename);
// /* debug --- */
// ft_putendl("init done");
// /* --- debug */
    GLfloat normalized_vertices[] =
    {
         0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,  // Верхний правый угол
         0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,  // Нижний правый угол
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,  // Нижний левый угол
        -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f   // Верхний левый угол
    };

    GLuint indices[] =
    {  // Помните, что мы начинаем с 0!
        0, 1, 2,   // Первый треугольник
        0, 2, 3    // Второй треугольник
    };

    GLuint shader_program_id = load_shader_program();
//создаем буферный объект
    GLuint VBO;//vertex buffer object

    glGenBuffers(1, &VBO);

    //создаем буферный объект для индексов
    GLuint EBO;
    glGenBuffers(1, &EBO);

    GLuint VAO;//vertex array object
    glGenVertexArrays(1, &VAO);

    //Сначала привязываем VAO
    glBindVertexArray(VAO);
    //копируем вершины в буфер для OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normalized_vertices), normalized_vertices, GL_STATIC_DRAW);
    //bind EBO - здесь же отвязался VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //copy indecses to buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //применяем VAO
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    //Отвязываем VAO, дабы не изменить случайно, настраивая другие VAO.
    //Вообще, VAO нужно привязывать перед операцией и отвязывать сразу после оной.
    glBindVertexArray(0);
    //отвязываем EBO после отвязывания VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	while (!glfwWindowShouldClose(g_win.win))
	{
    //теперь всякий раз, когда привязываем VAO, будет привязан VBO и EBO
    glBindVertexArray(VAO);//нету смысла это делать каждый раз в цикле, если здесь используется только один VAO 
    //но если в цикле разные VAO в разный момент времени привязываются и отвязываются, то перед использованием 
    //следующего предыдущий следует отвязать...
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program_id);
        // glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, (void*)0);
		glfwPollEvents();
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
