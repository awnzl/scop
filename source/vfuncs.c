#include "scop.h"

t_v		vector(GLfloat x, GLfloat y, GLfloat z)
{
	t_v ret;

	ret.x = x;
	ret.y = y;
	ret.z = z;
	return (ret);
}

t_v		add(t_v v1, t_v v2)
{
	v1.x = v1.x + v2.x;
	v1.y = v1.y + v2.y;
	v1.z = v1.z + v2.z;
	return (v1);
}

t_v		sub(t_v v1, t_v v2)
{
	v1.x = v1.x - v2.x;
	v1.y = v1.y - v2.y;
	v1.z = v1.z - v2.z;
	return (v1);
}

t_v		mult(t_v v1, t_v v2)
{
	v1.x = v1.x * v2.x;
	v1.y = v1.y * v2.y;
	v1.z = v1.z * v2.z;
	return (v1);
}

t_v		norm(t_v direct)
{
	double	vmod;

	vmod = sqrtf(direct.x * direct.x + direct.y * direct.y +
				direct.z * direct.z);
	direct.x = direct.x / vmod;
	direct.y = direct.y / vmod;
	direct.z = direct.z / vmod;
	return (direct);
}


t_v		cross(t_v v1, t_v v2)
{
	t_v	ret;

	ret.x = v1.y * v2.z - v1.z * v2.y;
	ret.y = v1.z * v2.x - v1.x * v2.z;
	ret.z = v1.x * v2.y - v1.y * v2.x;
	return (ret);
}

t_v		scale_n(t_v vec, GLfloat n)
{
	vec.x = vec.x * n;
	vec.y = vec.y * n;
	vec.z = vec.z * n;
	return (vec);
}

void	scale_s(t_v *vec, GLfloat n)
{
	vec->x *= n;
	vec->y *= n;
	vec->z *= n;
}

GLfloat	dot(t_v v1, t_v v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}
