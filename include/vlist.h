#ifndef __VLIST_H
# define __VLIST_H

# include <stdlib.h>
# include <pthread.h>
# include <stdbool.h>

typedef struct  s_vlist
{
    void        *buffer;
    size_t      len;
    size_t      cap;
    size_t      elem_size;
    const char  type[128];
}               t_vlist;

# define VLIST_INITIAL_CAP  16

# define VLIST_OF(type) t_vlist
# define VLIST_ADD(type, v, value) ({   \
        type    __tmp_val = value;      \
        vlist_add_gen(v, &__tmp_val);   \
    })
# define VLIST_DELETE(type, v, value) ({    \
        type    __tmp_val = value;          \
        vlist_delete_gen(v, &__tmp_val);    \
    })
# define VLIST_INIT(type, v) vlist_init(v, sizeof(type), #type)

int     vlist_init(t_vlist *vlist, size_t elem_size, char *type);
int     vlist_add_gen(t_vlist *vlist, void *value_address);
int     vlist_delete_gen(t_vlist *vlist, void *value_address);
void    *vlist_find(t_vlist *vlist, bool (*fn)(void *e, void *arg), void *arg);
void    vlist_foreach(t_vlist *vlist, void (*fn)(void *e, void *ar), void *ar);
void    vlist_destroy(t_vlist *vlist);

#endif
