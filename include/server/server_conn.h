#ifndef __SERVER_CONN_H
# define __SERVER_CONN_H

# include "conn.h"
# include "server.h"
# include "user_data.h"
# include "server_conn_cb.h"

# define CONN_SERVER(conn) (((t_server_conn*)CONN_PRIV(conn))->srv)
# define CONN_UDATA(conn)  (&((t_server_conn*)CONN_PRIV(conn))->udata)

/*
**  The server is always an external pointer while udata belongs
**  to this exact connection.
*/
typedef struct  s_server_conn
{
    t_server    *srv;
    t_udata     udata;
}               t_server_conn;

int             server_conn_create(t_conn *conn, int fd, t_server *server);
t_conn          *server_conn_find_by_name(t_server *srv, char *nick);

#endif
