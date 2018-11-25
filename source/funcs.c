#include "scop.h"

void *malloc_wrp(size_t size)
{
    void *ret;

    if (!(ret = malloc(size)))
        exit(EXIT_FAILURE);
    if (g_pointers_idx < 1024)
        g_pointers[++g_pointers_idx] = ret;
    else
    {
        print_error_endl("MALLOC_WRP::ERROR: g_pointers is full");
        exit(EXIT_FAILURE);
    }
    return (ret);
}