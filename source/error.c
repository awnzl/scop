#include "scop.h"

void	print_error_endl(const char *s)
{
	if (s)
	{
		while (*s)
			write(2, s++, 1);
		write(2, "\n", 1);
	}
}

void	print_error(const char *s)
{
	if (s)
		while (*s)
			write(2, s++, 1);
}
