#include "channel.h"
#include "logger.h"
#include <stdio.h>

int         channel_init(t_channel *chan, char *name)
{
    int     ret;

    LOG(L_INFO, "Creating new channel [%s].\n", name);
    snprintf(chan->name, sizeof(chan->name), "%s", name);
    ret = VLIST_INIT(t_conn, &chan->users);
    if (ret)
    {
        LOG(L_WARN, "Failed to create the channel.\n");
        return (ret);
    }
    return (0);
}

int         channel_join(t_channel *chan, t_conn *user)
{
   
}
