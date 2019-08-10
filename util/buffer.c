#include "buffer.h"

int     buffer_init(t_buffer *buf)
{
    buf->first = NULL;
    buf->last = NULL;
    buf->bytes = 0;
    return (0);
}

ssize_t buffer_put(t_buffer *buf, const char *data, size_t len)
{
    ssize_t         put;
    size_t          put_once;
    t_buffer_block  *currblk;

    put = 0;
    currblk = buf->last;
    while (len)
    {
        if (buffer_block_full(currblk))
        {
            currblk->next = buffer_block_new();
            if (!currblk->next)
                return (put ? put : -1);
            currblk = currblk->next;
        }
        put_once = buffer_block_put(currblk, data, len);
        put += put_once;
        data += put_once;
        len -= put_once;
    }
    return (put);
}

ssize_t buffer_pull(t_buffer *buf, char *data, size_t len)
{
    ssize_t         pulled;
    size_t          pulled_once;
    t_buffer_block  *currblk;
    t_buffer_block  *next;

    pulled = 0;
    currblk = buf->first;
    while (len)
    {
        if (!currblk)
            return (pulled);
        pulled_once = buffer_block_pull(currblk, data, len);
        if (!pulled_once)
        {
            next = currblk->next;
            free(currblk);
            currblk = next;
        }
        pulled += pulled_once;
        data += pulled_once;
        len -= pulled_once;
    }
    return (pulled);
}
