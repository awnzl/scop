#include "scop.h"

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
		exit(EXIT_FAILURE);
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

void		get_object_data(const char *filename, GLfloat **vertices, GLuint **indices)
{
	t_list	**content;

	read_content(&g_scop.v_databuf_size, &g_scop.f_databuf_size, &content, filename);
	*vertices = read_vertices(content[0], g_scop.v_databuf_size, 1);
	*indices = read_indices(content[1], &g_scop.f_databuf_size);
	g_scop.f_databuf_pos = g_scop.v_databuf_size;
	free_content(content);
}