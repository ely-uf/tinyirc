#include "conn.h"
#include "server_conn.h"
#include "buffer.h"
#include "logger.h"
#include <string.h>

int     conn_priv_init(t_conn *conn, t_server *serv)
{
    CONN_PRIV(conn) = (t_server_conn*)malloc(sizeof(t_server_conn));
    if (!CONN_PRIV(conn))
        return (1);
    CONN_SERVER(conn) = serv;
    memset(CONN_UDATA(conn), 0, sizeof(*CONN_UDATA(conn)));
    return (0);
}

int     conn_create(t_conn *conn, int fd, t_server *serv)
{
    int ret;

    conn->fd = fd;
    ret = buffer_init(&conn->readbuf);
    if (ret)
    {
        LOG(L_ERROR, "Failed to initialize read buffer for a new connection\n");
        return (ret);
    }
    ret = buffer_init(&conn->writebuf);
    if (ret)
    {
        LOG(L_ERROR, "Failed to initialize wr buffer for a new connection\n");
        buffer_destroy(&conn->readbuf);
        return (ret);
    }
    ret = conn_priv_init(conn, serv);
    if (ret)
    {
        buffer_destroy(&conn->readbuf);
        buffer_destroy(&conn->writebuf);
        return (ret);
    }
    conn->msg = (t_tinymsg){{0},0};
    return (0);
}

void    conn_addr_set(t_conn *conn, struct sockaddr *addr, socklen_t len)
{
    conn->addrlen = len;
    conn->addr = *addr;
}

void    conn_destroy(t_conn *conn)
{
    /*
     *  We assume that fd has been closed by the server.
     */
    buffer_destroy(&conn->readbuf);
    buffer_destroy(&conn->writebuf);
    conn->fd = -1;
    free(CONN_PRIV(conn));
    CONN_PRIV(conn) = NULL;
}
