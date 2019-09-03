#ifndef __CHANNEL_USERS
# define __CHANNEL_USERS

# include <stdbool.h>

typedef struct  s_chan_user_q
{
    void        *data;
    bool        value;
}               t_chan_user_q;

int             channel_has_user(void *chan, void *chan_user_q);

#endif
