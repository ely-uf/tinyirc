#include "buffer.h"

int     buffer_init(t_buffer *buf)
{
    buf->first = NULL;
    buf->last = NULL;
    buf->bytes = 0;
    return (0);
}

bool    buffer_is_empty(t_buffer *buf)
{
    return (buf->bytes == 0);
}

ssize_t buffer_put(t_buffer *buf, const char *data, size_t len)
{
    ssize_t         put;
    size_t          put_once;
    t_buffer_block  *currblk;

    put = 0;
    if (!buf->first)
    {
        buf->first = buffer_block_new();
        if (!buf->first)
            return (-1);
        buf->last = buf->first;
    }
    currblk = buf->last;
    while ((size_t)put < len)
    {
        if (buffer_block_full(currblk))
        {
            currblk->next = buffer_block_new();
            if (!currblk->next)
                return (put ? put : -1);
            currblk = currblk->next;
            buf->last = currblk;
        }
        put_once = buffer_block_put(currblk, data + put, len - put);
        put += put_once;
        buf->bytes += put_once;
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
    while ((size_t)pulled < len)
    {
        if (!currblk)
        {
            buf->first = NULL;
            buf->last = NULL;
            return (pulled);
        }
        pulled_once = buffer_block_pull(currblk, data + pulled, len - pulled);
        if (!pulled_once)
        {
            next = currblk->next;
            free(currblk);
            currblk = next;
            buf->first = currblk;
        }
        pulled += pulled_once;
        buf->bytes -= pulled_once;
    }
    return (pulled);
}

void    buffer_destroy(t_buffer *buf)
{
    t_buffer_block  *currblk;
    t_buffer_block  *next;

    currblk = buf->first;
    while (currblk)
    {
        next = currblk->next;
        free(currblk);
        currblk = next;
    }
    *buf = (t_buffer){ NULL, NULL, 0 };
}
