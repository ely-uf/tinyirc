#include <string.h>
#include "tinymsg.h"

void        tinymsg_clear(t_tinymsg *msg)
{
    memset(msg->buf, 0, TINYIRC_MSG_LEN);
    msg->len = 0;
}

void        tinymsg_pull(t_tinymsg *msg, t_buffer *readbuf)
{
    size_t  pulled;

    if (buffer_is_empty(readbuf))
        return ;
    while (!buffer_is_empty(readbuf) &&
            msg->len <= TINYIRC_MSG_LEN)
    {
        pulled = buffer_pull(readbuf,
                             &msg->buf[msg->len],
                             sizeof(msg->buf) - msg->len);
        msg->len += pulled;
    }
    if (msg->len >= TINYIRC_MSG_LEN &&
        strstr(msg->buf, TINYIRC_MSG_SEP) == NULL)
        tinymsg_clear(msg);
}

bool        tinymsg_is_complete(t_tinymsg *msg)
{
    return (strstr(msg->buf, TINYIRC_MSG_SEP) != NULL);
}

ssize_t     tinymsg_extract(t_tinymsg *msg, char dst[TINYIRC_MSG_LEN])
{
    const intptr_t  msgend = (intptr_t)strstr(msg->buf, TINYIRC_MSG_SEP)
                             + __builtin_strlen(TINYIRC_MSG_SEP);
    const ssize_t   msglen = msgend - (intptr_t)msg->buf;

    if (msgend == 0)
        return (0);
    strlcpy(dst, msg->buf, msglen + 1);
    msg->len -= msglen;
    memmove(msg->buf, msg->buf + msglen, TINYIRC_MSG_LEN - msglen);
    return msglen;
}
