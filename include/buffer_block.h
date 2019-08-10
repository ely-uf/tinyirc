#ifndef __BUFFER_BLOCK_H
# define __BUFFER_BLOCK_H

# include <stdbool.h>
# include <sys/types.h>

# define BUF_BLK_LEN    4096
# define BUF_BLK_END(bufblk) (&(bufblk)->data[BUF_BLK_LEN])
# define BUF_BLK_DATA(bufblk) ((bufblk)->data_start)

typedef struct  s_buffer_block
{
    char                    data[BUF_BLK_LEN];
    char                    *data_start;
    char                    *data_end;
    struct s_buffer_block   *next;
}               t_buffer_block;

t_buffer_block  *buffer_block_new(void);
bool            buffer_block_full(t_buffer_block *bufblk);

size_t          buffer_block_put(t_buffer_block *bufblk,
                                const char *data,
                                size_t l);
size_t          buffer_block_pull(t_buffer_block *bufblk, char *buf, size_t l);

#endif
