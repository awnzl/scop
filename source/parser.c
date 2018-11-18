#include "scop.h"


/*
for now next is work well:
open_file
read_content
read_data
free_content


next is need to be finished and tested:
read_vertices
read_indixes
*/

static GLfloat	*read_vertices(const t_list *content, const GLuint size, const int is_float)
{
	GLfloat *ret;
	char	**tmp;
	int		idx;
	int		j;

	ret = (GLfloat*)malloc(sizeof(GLfloat) * size * 3);
	idx = -1;
	while (content)
	{
		tmp = ft_strsplit(content->data, ' ');
		j = 0;
		while (tmp[++j])
			ret[++idx] = is_float ? atof(tmp[j]) : atoi(tmp[j]);
		content = content->next;
	}
	return (ret);
}

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
		perror("PARSER::OPEN::ERROR");
		return NULL;
	}
	return fl;
}

static void		read_data(	GLuint *vsz,
							GLuint *isz,
							FILE *file,
							t_list **content)
{
	t_list	*v;
	t_list	*f;
	char	*line;
	size_t	size;

	line = NULL;
	v = content[0];
	f = content[1];
	while (getline(&line, &size, file) != -1)
		if (line[0] == 'v')
		{
			v->data = ft_strdup(line);
			v->next = ft_lstnew(NULL, 0);
			v = v->next;
			++(*vsz);
		}
		else if (line[0] == 'f')
		{
			f->data = ft_strdup(line);
			f->next = ft_lstnew(NULL, 0);
			f = f->next;
			++(*isz);
		}
}

static void		read_content(	GLuint *vsz,
								GLuint *isz,
								t_list ***content,
								const char *filename)
{
	FILE	*file;

	file = open_file(filename);
	*content = malloc(sizeof(t_list) * 2);
	(*content)[0] = ft_lstnew(NULL, 0);
	(*content)[1] = ft_lstnew(NULL, 0);
	read_data(vsz, isz, file, *content);
	fclose(file);
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
	// vertices = read_vertices(content[0], v_size, 1);
	free_content(content);
}

/*
1. read to the string array v and f
2. then parse arrays

vvvvviiiiiiiiiiiiiiiiiiiiii
*/

// /* 	test main */
// /*
// gcc source/parser.c source/error.c libft/libft.a -o test_parser \
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

// 	// vertices = read_vertices(content, v_size, 1);
// 	//
// 	free_content(content);

//     return 0;
// }