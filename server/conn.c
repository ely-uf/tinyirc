#include "conn.h"
#include "buffer.h"

int     conn_create(t_conn *conn, int fd, t_server *serv)
{
    int ret;

    conn->fd = fd;
    ret = buffer_init(&conn->readbuf);
    if (ret)
        return (ret);
    ret = buffer_init(&conn->writebuf);
    if (ret)
    {
        buffer_destroy(&conn->readbuf);
        return (ret);
    }
    conn->serv = serv;
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
