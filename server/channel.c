#include "channel.h"
#include "logger.h"
#include "conn_vlist.h"
#include "server_conn.h"
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
    int ret;

    if (conn_find_by_name(&chan->users, CONN_UDATA(user)->name))
    {
        LOG(L_INFO, "User %s is already in the %s channel.\n",
                CONN_UDATA(user)->name, chan->name);
        return (1);
    }

    ret = VLIST_ADD(t_conn, &chan->users, *user);
    if (ret)
    {
        LOG(L_ERROR, "Failed to add user [%s] "
            "to the channel's [%s] users vlist.\n", CONN_UDATA(user)->name,
            chan->name);
        return (ret);
    }

    ret = VLIST_ADD(t_channel, &CONN_UDATA(user)->channels, *chan);
    if (ret)
    {
        LOG(L_ERROR, "Failed to add channel [%s] to the user's [%s] "
            "channels vlist.\n", chan->name, CONN_UDATA(user)->name);
        return (ret);
    }

    return (0);
}

int         channel_leave(t_channel *chan, t_conn *user)
{
    t_conn  *conn;

    conn = conn_find_by_name(&chan->users, CONN_UDATA(user)->name);
    if (conn == NULL)
    {
        LOG(L_INFO, "User %s is trying to leave the channel (%s)"
            "they do not belong to.\n", CONN_UDATA(user)->name, chan->name);
        return (1);
    }
    return (0);
}
