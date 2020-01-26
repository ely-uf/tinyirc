#ifndef __VLIST_H
# define __VLIST_H

# include <stdlib.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <string.h>

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
        VLIST_TYPECHECK(v, type);       \
        vlist_add_gen(v, &__tmp_val);   \
    })
# define VLIST_DELETE(type, v, value) ({    \
        type    __tmp_val = value;          \
        VLIST_TYPECHECK(v, type);           \
        vlist_delete_gen(v, &__tmp_val);    \
    })
# define VLIST_INIT(type, v) vlist_init(v, sizeof(type), #type)

# define VLIST_TYPECHECK(v, el_type) ({                         \
        const char *__v_type = (v)->type;                       \
        if (strcmp(__v_type, #el_type)) {                       \
            printf("%s: Type mismatch for vlist operation!\n"   \
                   "\tExpected: %s\n"                           \
                   "\tGot: %s\n",                               \
                   __func__, #el_type, __v_type);               \
            abort();                                            \
        }                                                       \
    })
int     vlist_init(t_vlist *vlist, size_t elem_size, char *type);
int     vlist_add_gen(t_vlist *vlist, void *value_address);
int     vlist_delete_gen(t_vlist *vlist, void *value_address);
void    *vlist_find(t_vlist *vlist, bool (*fn)(void *e, void *arg), void *arg);
void    vlist_foreach(t_vlist *vlist, void (*fn)(void *e, void *ar), void *ar);
void    vlist_destroy(t_vlist *vlist);

#endif
