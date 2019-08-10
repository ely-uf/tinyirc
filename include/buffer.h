#ifndef __BUFFER_H
# define __BUFFER_H

# include <stdlib.h>
# include <stdbool.h>
# include <sys/types.h>
# include "buffer_block.h"

typedef struct  s_buffer
{
    t_buffer_block  *first;
    t_buffer_block  *last;
    size_t          bytes;
}               t_buffer;

int     buffer_init(t_buffer *buf);
ssize_t buffer_put(t_buffer *buf, const char *data, size_t len);
ssize_t buffer_pull(t_buffer *buf, char *data, size_t len);
bool    buffer_is_empty(t_buffer *buf);

#endif
