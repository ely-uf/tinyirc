#include "conn.h"
#include "server_conn.h"
#include "conn_vlist.h"
#include "buffer.h"
#include "logger.h"
#include "ircmsg.h"
#include <string.h>

t_conn      *server_conn_find_by_name(t_server *server, char *nick)
{
    return conn_find_by_name(&server->clients, nick);
}

int     server_conn_priv_init(t_conn *conn, void *serv)
{
    CONN_PRIV(conn) = (t_server_conn*)malloc(sizeof(t_server_conn));
    if (!CONN_PRIV(conn))
        return (1);
    CONN_SERVER(conn) = serv;
    memset(CONN_UDATA(conn), 0, sizeof(*CONN_UDATA(conn)));
    return (0);
}

int     server_conn_priv_destroy(t_conn *conn)
{
    free(CONN_PRIV(conn));
    CONN_PRIV(conn) = NULL;
    return (0);
}

int     server_conn_on_error(t_conn *conn, t_conn_err error)
{
    if (error == CONN_ERR_READ || error == CONN_ERR_WRITE)
    {
        server_drop_now(CONN_SERVER(conn), conn);
    }
    else
    {
        server_drop(CONN_SERVER(conn), conn);
    }
    return (0);
}

int     server_conn_on_message(t_conn *conn, t_ircmsg *msg)
{
    ircmsg_handle(msg, conn);
    return (0);
}

static const t_conn_ops     g_serv_conn_ops = {
    .init = server_conn_priv_init,
    .destroy = server_conn_priv_destroy,
    .on_error = server_conn_on_error,
    .on_message = server_conn_on_message,
};

int     server_conn_create(t_conn *conn, int fd, t_server *srv)
{
    return conn_create(conn, fd, &g_serv_conn_ops, srv);
}
