#include "conn.h"
#include "buffer.h"
#include "logger.h"

int     conn_create(t_conn *conn, int fd, t_server *serv)
{
    int ret;

    conn->fd = fd;
    ret = buffer_init(&conn->readbuf);
    if (ret)
    {
        return (ret);
        LOG(L_ERROR, "Failed to initialize read buffer for a new connection\n");
    }
    ret = buffer_init(&conn->writebuf);
    if (ret)
    {
        LOG(L_ERROR, "Failed to initialize wr buffer for a new connection\n");
        buffer_destroy(&conn->readbuf);
        return (ret);
    }
    conn->serv = serv;
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
    conn->serv = NULL;
}
