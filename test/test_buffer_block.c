#include "buffer_block.h"
#include "test_base.h"
#include <stdlib.h>
#include <string.h>

int     test_buffer_block_new(void)
{
    t_buffer_block  *new;

    new = buffer_block_new();
    ASSERT(new != NULL, "buffer_block_new should allocate a new block");
    ASSERT(!new->next, "freshly allocated block should not have 'next' field");
    ASSERT(new->data_start == new->data, "freshly allocated block should have"
            " valid 'data_start' field");
    ASSERT(new->data_start == new->data_end, "data_start and data_end should"
            " be equal in a new block");
    free(new);
    return (0);
}

int     test_buffer_block_put(void)
{
    t_buffer_block  *blk;
    const char      zero_buf[BUF_BLK_LEN] = {0};
    size_t          put;

    blk = buffer_block_new();
    put = buffer_block_put(blk, (char*)zero_buf, BUF_BLK_LEN);
    ASSERT(put == BUF_BLK_LEN, "Empty block could be filled entirely");
    ASSERT(memcmp(zero_buf, BUF_BLK_DATA(blk), BUF_BLK_LEN) == 0,
            "buffer_block_put should not corrupt data");
    put = buffer_block_put(blk, (char*)zero_buf, 1);
    ASSERT(put == 0, "buffer_block_put should not write out of memory");
    free(blk);
    blk = buffer_block_new();
    put = buffer_block_put(blk, "123", 3);
    ASSERT(put == 3, "buffer_block_put should report the size of data written"
            " correctly");
    put += buffer_block_put(blk, "45678", 5);
    ASSERT(put == 8, "buffer_block_put can properly append data");
    ASSERT(memcmp("12345678", BUF_BLK_DATA(blk), 8) == 0,
            "buffer_block_put does not corrupt data on append");
    free(blk);
    return (0);
}

int     test_buffer_block_pull(void)
{
    t_buffer_block  *blk;
    char            buf[32];
    size_t          pull;

    memset(buf, 0, sizeof(buf));
    blk = buffer_block_new();
    buffer_block_put(blk, "12345", 5);
    pull = buffer_block_pull(blk, buf, sizeof(buf));
    ASSERT(pull == 5, "buffer_block_pull reports valid amount of data pulled");
    ASSERT(memcmp(buf, "12345", 5) == 0,
            "buffer_block_pull returns data correctly");
    buffer_block_put(blk, "12345", 5);
    pull = buffer_block_pull(blk, buf, 3);
    ASSERT(pull == 3, "buffer_block_pull does not exceed the buffer provided");
    ASSERT(memcmp(buf, "123", 3) == 0, "buffer_block_pull writes correctly to"
            " limited buffers");
    pull = buffer_block_pull(blk, buf + 3, 2);
    ASSERT(pull == 2, "buffer_block_pull reports proper length of leftover "
            "data returned");
    ASSERT(memcmp(buf, "12345", 5) == 0, "buffer_block_pull returns data "
            "continuously");
    free(blk);
    return (0);
}

int     test_buffer_block_full(void)
{
    t_buffer_block  *blk;
    const char      zero_buf[BUF_BLK_LEN] = {0};

    blk = buffer_block_new();
    buffer_block_put(blk, zero_buf, BUF_BLK_LEN);
    ASSERT(buffer_block_full(blk), "buffer_block_full correctly detects if "
            "the particular block can no longer contain data");
    free(blk);
    return (0);
}

int     test_buffer_block(void)
{
    int ret;

    ret = 0;
    ret |= test_buffer_block_new();
    ret |= test_buffer_block_put();
    ret |= test_buffer_block_pull();
    ret |= test_buffer_block_full();
    return (ret);
}
