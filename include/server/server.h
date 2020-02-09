#ifndef __SERVER_H
# define __SERVER_H

# include <netinet/ip.h>
# include <sys/select.h>
# include "vlist.h"
# include "conn.h"
# include "channel.h"

typedef struct  s_server
{
    bool                alive;
    int                 sock;
    short               port;
    VLIST_OF(t_conn)    clients;
    VLIST_OF(t_channel) channels;
    int                 maxfd;
    fd_set              readset;
    fd_set              writeset;
    fd_set              exceptset;
}               t_server;

int             server_open(t_server *server);
int             server_serve(t_server *server);
int             server_accept(t_server *server);
int             server_drop(t_server *serv, t_conn *conn);
int             server_drop_now(t_server *serv, t_conn *conn);
int             server_close(t_server *server);

#endif
