#ifndef __SERVER_H
# define __SERVER_H

# include <netinet/ip.h>
# include <sys/select.h>
# include "vlist.h"
# include "conn.h"

typedef struct  s_server
{
    int                 sock;
    VLIST_OF(t_conn)    clients;
    int                 maxfd;
    fd_set              readset;
    fd_set              writeset;
    fd_set              exceptset;
}               t_server;

int             server_open(t_server *server);
int             server_serve(t_server *server);
int             server_close(t_server *server);

#endif
