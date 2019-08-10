#include "buffer.h"
#include "test_base.h"

static size_t   buffer_blocks_in_buffer(t_buffer *buf)
{
    size_t          i;
    t_buffer_block  *blk;

    i = 0;
    blk = buf->first;
    while (blk)
    {
        i++;
        blk = blk->next;
    }
    return (i);
}

int     test_buffer_init(void)
{
    t_buffer    buf;

    buffer_init(&buf);
    ASSERT(buf.bytes == 0, "Freshly initialized buffer should not "
            "have any data");
    ASSERT(buf.first == NULL, "Fresh buffer should not contain any data");
    ASSERT(buf.first == buf.last, "Fresh buffer should not contain any data");
    return (0);
}

int     test_buffer_put(void)
{
    t_buffer    buf;
    ssize_t     put;
    const char  zero_buf[BUF_BLK_LEN / 2] = {0};

    buffer_init(&buf);
    put = buffer_put(&buf, zero_buf, sizeof(zero_buf));
    ASSERT(put == sizeof(zero_buf), "buffer_put puts all the data it can fit");
    ASSERT(buffer_blocks_in_buffer(&buf) == 1, "buffer_put does not create "
            "superficial blocks");
    put += buffer_put(&buf, zero_buf, sizeof(zero_buf));
    put += buffer_put(&buf, zero_buf, sizeof(zero_buf));
    ASSERT(put == sizeof(zero_buf) * 3, "buffer_put is unstoppable");
    ASSERT(buffer_blocks_in_buffer(&buf) == 2, "buffer_put extends internal "
            "buffer block chain");
    return (0);
}

int     test_buffer_pull(void)
{
    t_buffer    buf;

    buffer_init(&buf);

    return (0);
}

int     test_buffer(void)
{
    int     ret;

    ret = 0;
    ret |= test_buffer_init();
    ret |= test_buffer_put();
    ret |= test_buffer_pull();

    return (ret);
}
