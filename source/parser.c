#include "scop.h"

typedef	struct	s_indices_data
{
	GLuint					idxs[3];
	struct s_indices_data	*next;
}				t_indt;

static void		free_content(t_list **list)
{
	t_list *v;
	t_list *f;

	if (list)
	{
		v = list[0];
		f = list[1];
		while (v)
		{
			v = v->next;
			free(list[0]->data);
			free(list[0]);
			list[0] = v;
		}
		while (f)
		{
			f = f->next;
			free(list[1]->data);
			free(list[1]);
			list[1] = f;
		}
		free(list);
	}
}

static FILE	*open_file(const char *filename)
{
	FILE	*fl;

ft_putstr("Filename: ");
ft_putendl(filename);
	if (!(fl = fopen(filename, "r")))
	{
		perror("PARSER::OPENFILE::ERROR");
		exit(EXIT_FAILURE);
	}
	return (fl);
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
		if ((line[0] == 'v' || line[0] == 'f') && line[1] == ' ')
			store_data(vsz, isz, vertex_face, line);
	fclose(file);
	free(line);
	vertex_face[0] = vertex_face[2]->next;
	vertex_face[2]->next = NULL;
	free(vertex_face[0]);
	vertex_face[0] = vertex_face[3]->next;
	vertex_face[3]->next = NULL;
	free(vertex_face[0]);
	free(vertex_face);
}

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
			*head++ = atof(tmp[j]);
		while (--j >= 0)
			free(tmp[j]);
		free(tmp);
		content = content->next;
	}
	return (verticies);
}

static t_indt	*triangulate(t_indt *indata, GLuint *all_size, GLuint size, char **arr)
{
	indata->idxs[0] = atoi(arr[1]) - 1;//1
	indata->idxs[1] = atoi(arr[2]) - 1;//2
	indata->idxs[2] = atoi(arr[3]) - 1;//3
	if (size == 5)
	{
		(*all_size) += 2;
		indata->next = (t_indt*)malloc(sizeof(t_indt));
		indata = indata->next;
		indata->next = NULL;
		// 3 2 4 or 4 2 3 ???
		indata->idxs[0] = atoi(arr[4]) - 1;
		indata->idxs[1] = atoi(arr[2]) - 1;
		indata->idxs[2] = atoi(arr[3]) - 1;
	}
	indata->next = (t_indt*)malloc(sizeof(t_indt));
	indata->next->next = NULL;
	return (indata->next);
}

/*
**	returns number of the indices and fill list with indices values
*/
static GLuint	num_of_indices(t_list *content, t_indt	**indata)
{
	GLuint	ret;
	int		itmp;
	char	**arr;
	t_indt	*tmp;

	*indata = (t_indt*)malloc(sizeof(t_indt));
	(*indata)->next = NULL;
	tmp = *indata;
	ret = 0;
	while (content)
	{
		arr = ft_strsplit(content->data, ' ');
		itmp = 0;
		while (arr[++itmp])
			++ret;
		tmp = triangulate(tmp, &ret, itmp, arr);
		while (--itmp >= 0)
			free(arr[itmp]);
		free(arr);
		content = content->next;
	}
	return (ret);
}

static GLuint	*read_indices(t_list *content, GLuint *size)
{
	GLuint	*indices;
	int		idx;
	t_indt	*idxvalues;
	t_indt	*tmp;

	*size = num_of_indices(content, &idxvalues);
	indices = (GLuint*)malloc_wrp(sizeof(GLuint) * (*size));
	idx = -1;
	while (idxvalues)
	{
		tmp = idxvalues;
		indices[++idx] = idxvalues->idxs[0];
		indices[++idx] = idxvalues->idxs[1];
		indices[++idx] = idxvalues->idxs[2];
		idxvalues = idxvalues->next;
		free(tmp);
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