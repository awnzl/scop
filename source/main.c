#include "scop.h"

const GLchar *vertex_code =
"\
#version 440 core\
\
layout (location = 0) in vec3 position;\
layout (location = 1) in vec3 col;\
\
out vec4 vertexColor;\
\
void main()\
{\
   gl_Position = vec4(position.x, position.y, position.z, 1.0);\
   vertexColor = vec4(col, 1.0f);\
}\
";

const GLchar *fragment_code =
"\
#version 440 core\
\
in vec4 vertexColor;\
\
out vec4 color;\
\
void main()\
{\
   color = vertexColor;\
}\
";

static void		print_usage()
{
	ft_putendl("./scop file_name.obj");
}

static int		init_scop()
{
	g_pointers_idx = -1;
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

static int		load_shader_program()
{
	GLuint	vertex;
    GLuint	fragment;
    GLint	success;
    GLchar	infoLog[512];
	GLint	program_id;


	vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_code, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        ft_putstr("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
		ft_putendl(infoLog);
    };

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_code, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        ft_putstr("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
		ft_putendl(infoLog);
    };

	program_id = glCreateProgram();
    glAttachShader(program_id, vertex);
    glAttachShader(program_id, fragment);
    glLinkProgram(program_id);

    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program_id, 512, NULL, infoLog);
        ft_putstr("ERROR::SHADER::PROGRAM::LINKING_FAILED\n");
		ft_putendl(infoLog);
    }

    // Удаляем шейдеры, поскольку они уже добавлены в программу и нам больше не нужны.
    glDeleteShader(vertex);
    glDeleteShader(fragment);
	return program_id;
}

void			run(const char *filename)
{
	if (!init_scop() || !init())
		return ;

    GLfloat normalized_vertices[] =
    {
         0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,  // Верхний правый угол
         0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,  // Нижний правый угол
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,  // Нижний левый угол
        -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f   // Верхний левый угол
    };

    GLuint indices[] =
    {  // Помните, что мы начинаем с 0!
        0, 1, 3,   // Первый треугольник
        1, 2, 3    // Второй треугольник
    };

//создаем буферный объект
    GLuint VBO;//vertex buffer object
    glGenBuffers(1, &VBO);

    //создаем буферный объект для индексов
    GLuint EBO;
    glGenBuffers(1, &EBO);

    int shader_program_id = load_shader_program();

    GLuint VAO;//vertex array object
    glGenVertexArrays(1, &VAO);
    //Сначала привязываем VAO
    glBindVertexArray(VAO);
    //копируем вершины в буфер для OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normalized_vertices), normalized_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    //применяем VAO
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //bind EBO - здесь же отвязался VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //copy indecses to buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Отвязываем VAO, дабы не изменить случайно, настраивая другие VAO.
    //Вообще, VAO нужно привязывать перед операцией и отвязывать сразу после оной.
    glBindVertexArray(0);

    //отвязываем EBO после отвязывания VAO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //теперь всякий раз, когда привязываем VAO, будет привязан VBO и EBO
    glBindVertexArray(VAO);//нету смысла это делать каждый раз в цикле, если здесь используется только один VAO 
    //но если в цикле разные VAO в разный момент времени привязываются и отвязываются, то перед использованием 
    //следующего предыдущий следует отвязать...

	while (!glfwWindowShouldClose(g_win.win))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program_id);

		glfwPollEvents();
		glfwSwapBuffers(g_win.win);
	}

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
