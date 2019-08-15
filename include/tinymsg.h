#ifndef __TINYMSG_H
# define __TINYMSG_H

# include <stddef.h>
# include "const.h"
# include "buffer.h"

typedef struct  s_tinymsg
{
    char    buf[TINYIRC_MSG_LEN];
    size_t  len;
}               t_tinymsg;

void            tinymsg_clear(t_tinymsg *msg);
void            tinymsg_pull(t_tinymsg *msg, t_buffer *readbuf);
bool            tinymsg_is_complete(t_tinymsg *msg);
ssize_t         tinymsg_extract(t_tinymsg *msg, char *dst);

#endif
