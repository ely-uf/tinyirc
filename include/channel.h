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

#endif
