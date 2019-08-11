#ifndef __CONNECTION_H
# define __CONNECTION_H

# include <stdlib.h>
# include <stdbool.h>
# include "buffer.h"
# include "server.h"

struct s_server;

typedef struct  s_conn
{
    int             fd;
    t_buffer        readbuf;
    t_buffer        writebuf;
    struct sockaddr addr;
    socklen_t       addrlen;
    struct s_server *serv;
}               t_conn;

int             conn_create(t_conn *conn, int fd, struct s_server *serv);
void            conn_addr_set(t_conn *conn, struct sockaddr *a, socklen_t l);
void            conn_destroy(t_conn *conn);

#endif
