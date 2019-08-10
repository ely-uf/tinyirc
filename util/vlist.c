#include "vlist.h"
#include <string.h>

static int  vlist_extend(t_vlist *v)
{
    const size_t    new_cap = v->cap * 2;
    void            *new_buf;

    new_buf = realloc(v->buffer, new_cap * v->elem_size);
    if (!new_buf)
        return (1);
    v->buffer = new_buf;
    v->cap = new_cap;
    return (0);
}

int         vlist_init(t_vlist *v, size_t elem_size, char *type)
{
    if (!v)
        return (1);
    v->buffer = malloc(elem_size * VLIST_INITIAL_CAP);
    if (!v->buffer)
        return (1);
    v->len = 0;
    v->cap = VLIST_INITIAL_CAP;
    v->elem_size = elem_size;
    strncpy((char*)v->type, type ?: "opaque", sizeof(v->type));
    return (0);
}

int     vlist_add_gen(t_vlist *v, void *value_address)
{
    void    *new_location;

    if (!value_address)
        return (1);
    if (v->len == v->cap && vlist_extend(v) != 0)
        return (1);
    new_location = &((char*)v->buffer)[v->len * v->elem_size];
    memcpy(new_location, value_address, v->elem_size);
    v->len++;
    return (0);
}

int     vlist_delete_gen(t_vlist *v, void *value_address)
{
    char    *mem;
    size_t  idx;

    if (!value_address)
        return (1);
    idx = 0;
    mem = v->buffer;
    while (idx < v->len)
    {
        if (memcmp(mem, value_address, v->elem_size) == 0)
        {
            memmove(mem, mem + v->elem_size,
                    v->len - idx * v->elem_size);
            v->len--;
            return (0);
        }
        mem += v->elem_size;
        idx++;
    }
    return (1);
}

void    *vlist_find(t_vlist *v, bool (*fn)(void *e, void *arg), void *arg)
{
    size_t  idx;
    char    *mem;

    idx = 0;
    mem = v->buffer;
    while (idx < v->len)
    {
        if (fn(mem, arg))
            return (mem);
        mem += v->elem_size;
        idx++;
    }
    return (NULL);
}

void    vlist_foreach(t_vlist *v, void (*fn)(void *e, void *arg), void *arg)
{
    size_t  idx;
    char    *mem;

    idx = 0;
    mem = v->buffer;
    while (idx < v->len)
    {
        fn(mem, arg);
        mem += v->elem_size;
        idx++;
    }
}

void    vlist_destroy(t_vlist *v)
{
    free(v->buffer);
}
