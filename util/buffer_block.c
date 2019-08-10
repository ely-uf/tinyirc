#include "buffer_block.h"
#include <stdlib.h>
#include <string.h>

t_buffer_block  *buffer_block_new(void)
{
    t_buffer_block  *bufblk;

    bufblk = (t_buffer_block*)malloc(sizeof(*bufblk));
    if (!bufblk)
        return (NULL);
    bufblk->data_start = bufblk->data;
    bufblk->data_end = bufblk->data;
    bufblk->next = NULL;
    return (bufblk);
}

bool            buffer_block_full(t_buffer_block *bufblk)
{
    return (bufblk->data_end == BUF_BLK_END(bufblk));
}

size_t          buffer_block_put(t_buffer_block *bufblk,
                                const char *data,
                                size_t l)
{
    size_t can_fit;

    if (buffer_block_full(bufblk))
        return (0);

    can_fit = BUF_BLK_END(bufblk) - bufblk->data_end;
    if (can_fit > l)
        can_fit = l;
    memcpy(bufblk->data_end, data, can_fit);
    bufblk->data_end += can_fit;
    return (can_fit);
}

size_t          buffer_block_pull(t_buffer_block *bufblk, char *buf, size_t l)
{
    size_t  can_pull;

    if (bufblk->data_end == bufblk->data)
        return (0);
    can_pull = bufblk->data_end - bufblk->data_start;
    if (can_pull > l)
        can_pull = l;
    memcpy(buf, bufblk->data_start, can_pull);
    bufblk->data_start += can_pull;
    return (can_pull);
}
