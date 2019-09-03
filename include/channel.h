#ifndef __CHANNEL_H
# define __CHANNEL_H

# include "conn.h"
# include "server.h"
# include "const.h"
# include "vlist.h"

typedef struct  s_channel
{
    char                name[TINYIRC_CHANNAMELEN + 1];
    VLIST_OF(t_conn)    users;
}               t_channel;

int             channel_init(t_channel *chan, char *name);
int             channel_join(t_channel *chan, t_conn *user);
int             channel_leave(t_channel *chan, t_conn *user);
int             channel_send(t_channel *chan, t_conn *sender, char *message);
int             channel_destroy(t_channel *chan);

#endif
