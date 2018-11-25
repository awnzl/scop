#include "scop.h"


/*
for now next is work well:
open_file
read_content
read_data
free_content
read_vertices
read_indixes
*/

static void	free_content(t_list **list)
{
	t_list *v;
	t_list *f;

	v = list[0];
	f = list[1];
	if (list)
	{
		while (v)
		{
			v = list[0]->next;
			free(list[0]);
			list[0] = v;
		}
		while (f)
		{
			f = list[1]->next;
			free(list[1]);
			list[1] = f;
		}
	}
	free(list);
}

static FILE	*open_file(const char *filename)
{
	FILE	*fl;

	if (!(fl = fopen(filename, "r")))
	{
		perror("PARSER::OPENFILE::ERROR");
		return NULL;
	}
	return fl;
}

static void		store_data(GLuint *vsz,
							GLuint *isz,
							t_list **vertex_face,
							const char *line)
{
	int		idx;

	idx = line[0] == 'v' ? 0 : 1;
	vertex_face[idx]->data = ft_strdup(line);
	vertex_face[idx]->next = ft_lstnew(NULL, 0);
	vertex_face[!idx ? 2 : 3] = vertex_face[idx];
	vertex_face[idx] = vertex_face[idx]->next;
	!idx ? ++(*vsz) : ++(*isz);
}

static void		read_content(GLuint *vsz,
								GLuint *isz,
								t_list ***content,
								const char *filename)
{
	FILE	*file;
	t_list	**vertex_face;
	char	*line;
	size_t	size;

	line = NULL;
	file = open_file(filename);
	*content = malloc(sizeof(t_list) * 2);
	vertex_face = malloc(sizeof(t_list) * 4);
	(*content)[0] = ft_lstnew(NULL, 0);
	(*content)[1] = ft_lstnew(NULL, 0);
	vertex_face[0] = (*content)[0];
	vertex_face[1] = (*content)[1];
	while (getline(&line, &size, file) != -1)
		if (line[0] == 'v' || line[0] == 'f')
			store_data(vsz, isz, vertex_face, line);
	fclose(file);
	vertex_face[0] = vertex_face[2]->next;
	vertex_face[2]->next = NULL;
	free(vertex_face[0]);
	vertex_face[0] = vertex_face[3]->next;
	vertex_face[3]->next = NULL;
	free(vertex_face[0]);
	free(vertex_face);
}

/*
	if the idea with polygons with any count of vertices won't work, then this function
   	maight be adapted for reading of verticies and faces depending on is_float value.
	Faces will be triangulated before.
*/
static GLfloat	*read_vertices(t_list *content, const GLuint size, const int is_float)
{
	GLfloat *verticies;
	GLfloat *head;
	char	**tmp;
	int		j;

	verticies = (GLfloat*)malloc_wrp(sizeof(GLfloat) * size * 3);
	head = verticies;
	while (content)
	{
		if (!(tmp = ft_strsplit(content->data, ' ')))
			exit(EXIT_FAILURE);
		j = 0;
		while (tmp[++j])
			*head++ = atof(tmp[j]);//head[++idx] = is_float ? atof(tmp[j]) : atoi(tmp[j]) //read the comment befour function
		while (--j >= 0)
			free(tmp[j]);
		free(tmp);
		content = content->next;
	}
	return (verticies);
}

static GLuint	num_of_indices(t_list *content)
{
	GLuint	ret;
	GLuint	itmp;
	GLuint	j;
	char	**tmp;

	ret = 0;
	while (content)
	{
		tmp = ft_strsplit(content->data, ' ');
		itmp = 0;
		while (tmp[++itmp])
			++ret;
		j = 0;
		while (tmp[j])
			free(tmp[j++]);
		free(tmp);
		content = content->next;
	}
	return (ret);
}

static GLuint	*read_indices(t_list *content, GLuint *const size)
{
	GLuint	*indices;
	GLuint	*head;
	int		idx;
	int		j;
	char	**arr;

	*size = num_of_indices(content);
	indices = (GLuint*)malloc_wrp(sizeof(GLuint) * (*size));
	head = indices;
	idx = 0;
	while (idx < *size)
	{
		if (!(arr = ft_strsplit(content->data, ' ')))
			exit(EXIT_FAILURE);
		j = 0;
		while (arr[++j])
			*head++ = atoi(arr[j]);
		idx += j - 1;
		while (--j >= 0)
			free(arr[j]);
		free(arr);
		content = content->next;
	}
	return (indices);
}

void		get_object_data(const char *filename)
{
	GLfloat	*vertices;
	GLuint	*indices;
	t_list	**content;
	GLuint	v_size;
	GLuint	i_size;

	v_size = 0;
	i_size = 0;
	read_content(&v_size, &i_size, &content, filename);
	vertices = read_vertices(content[0], v_size, 1);
	indices = read_indices(content[1], &i_size);//i_size need to be updeted in reader
	free_content(content);
}

// /*
// 1. read to the string array v and f
// 2. then parse arrays

// vvvvviiiiiiiiiiiiiiiiiiiiii
// */

// /* 	test main */
// /*
// gcc source/parser.c source/error.c source/funcs.c libft/libft.a -o test_parser \
//     -I ~/.brew/include -I ./libft -L ~/.brew/lib -lGLEW -lGLFW
// */

// int main()
// {
//     GLfloat	*vertices;
// 	GLuint	*indices;
// 	t_list	**content;
// 	t_list	*tmp;
// 	GLuint	v_size;
// 	GLuint	i_size;

//     v_size = 0;
// 	i_size = 0;
// 	ft_putendl("before reading of content");
// 	read_content(&v_size, &i_size, &content, "./resources/42.obj");
// 	ft_putendl("content is readed");
// 	ft_putstr("vertices num: ");
// 	ft_putnbr(v_size);
// 	ft_putendl("");
// 	ft_putstr("num of face lines: ");
// 	ft_putnbr(i_size);
// 	ft_putendl("");

//     tmp = content[0];
//     while (tmp)
//     {
//         ft_putstr(tmp->data);
//         tmp = tmp->next;
//     }
//     tmp = content[1];
//     while (tmp)
//     {
//         ft_putstr(tmp->data);
//         tmp = tmp->next;
//     }

// 	ft_putendl("data is printed");

// 	vertices = read_vertices(content[0], v_size, 1);
// 	printf("num of verties: %u\n", v_size);
// 	int i = -1;
// 	while(++i < v_size * 3)
// 		printf("i: %i, float: %f\n", i, vertices[i]);

// 	indices = read_indices(content[1], &i_size);//i_size need to be updeted in reader
// 	i = -1;
// 	while (++i < i_size)
// 		printf("i: %i, int: %i\n", i, indices[i]);

// 	free_content(content);

// 	ft_putendl("end");

//     return 0;
// }