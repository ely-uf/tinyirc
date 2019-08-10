#include "buffer.h"
#include "test_base.h"
#include <string.h>

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
    ssize_t     pulled;
    const char  zero_buf[32] = {0};
    char        pull_buf[32];
    char        big_buffer[BUF_BLK_LEN * 4] = {0};

    buffer_init(&buf);
    buffer_put(&buf, zero_buf, sizeof(zero_buf));

    pulled = buffer_pull(&buf, pull_buf, sizeof(pull_buf));
    ASSERT(pulled == sizeof(zero_buf), "buffer_pull should pull all the data it has if size allows it");
    ASSERT(memcmp(pull_buf, zero_buf, sizeof(zero_buf)) == 0, "buffer_pull should return uncorrupted data");
    pulled = buffer_pull(&buf, pull_buf, sizeof(pull_buf));
    ASSERT(pulled == 0, "buffer_pull has nothing to pull if buffer is empty");
    ASSERT(memcmp(pull_buf, zero_buf, sizeof(zero_buf)) == 0, "buffer_pull should not modify the buffer passed to it in case it has nothing to pull");

    buffer_put(&buf, big_buffer, sizeof(big_buffer));
    pulled = 0;
    for (size_t pulled_once;
         (pulled_once = buffer_pull(&buf, pull_buf, sizeof(pull_buf))) != 0;
         pulled += pulled_once);

    ASSERT(pulled == sizeof(big_buffer), "buffer_pull should handle large amounts of data being pulled");
    ASSERT(buffer_is_empty(&buf), "Buffer should be empty");

    return (0);
}

int     test_buffer_is_empty(void)
{
    t_buffer    buf;
    char        pull_buf[16];

    buffer_init(&buf);
    ASSERT(buffer_is_empty(&buf), "Freshly initialized buffer is empty.");
    buffer_put(&buf, "test", 4);
    ASSERT(!buffer_is_empty(&buf), "Buffer is not empty after valid buffer_put.");
    buffer_pull(&buf, pull_buf, sizeof(pull_buf));
    ASSERT(buffer_is_empty(&buf), "Buffer is empty after all data being pulled from it.");
    return (0);
}

int     test_buffer(void)
{
    int     ret;

    ret = 0;
    ret |= test_buffer_init();
    ret |= test_buffer_put();
    ret |= test_buffer_pull();
    ret |= test_buffer_is_empty();

    return (ret);
}
