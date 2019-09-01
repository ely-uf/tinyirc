#ifndef __SERVER_CONN_H
# define __SERVER_CONN_H

# include "conn.h"
# include "server.h"
# include "user_data.h"

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

#endif
