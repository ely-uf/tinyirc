#ifndef __CONNECTION_H
# define __CONNECTION_H

# include <netinet/ip.h>
# include <stdlib.h>
# include <stdbool.h>
# include "buffer.h"
# include "tinymsg.h"

# define CONN_PRIV(conn) ((conn)->priv)

struct s_server;

typedef struct  s_conn
{
    int             fd;
    t_buffer        readbuf;
    t_buffer        writebuf;
    t_tinymsg       msg;
    struct sockaddr addr;
    socklen_t       addrlen;
    bool            disconnecting;
    void            *priv;
}               t_conn;

int             conn_create(t_conn *conn, int fd, struct s_server *serv);
void            conn_addr_set(t_conn *conn, struct sockaddr *a, socklen_t l);
void            conn_read(t_conn *conn);
void            conn_write(t_conn *conn);
void            conn_destroy(t_conn *conn);
void            conn_msg_process(t_conn *conn);

#endif
