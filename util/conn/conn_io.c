#include <stdbool.h>
#include <errno.h>
#include "conn.h"
#include "logger.h"

void    conn_recv(t_conn *conn)
{
    char    buf[4096];
    ssize_t recvb;
    ssize_t put;
    bool    more;

    more = true;
    while (more)
    {
        recvb = recv(conn->fd, &buf, sizeof(buf), 0);
        if (recvb <= 0)
        {
            if (recvb == 0 || errno != EINTR)
                conn->ops->on_error(conn, CONN_ERR_READ);
            return ;
        }
        LOG(L_DEBUG, "Read %zu bytes.\n", (size_t)recvb);
        put = buffer_put(&conn->readbuf, buf, recvb);
        if (put != recvb)
        {
            LOG(L_WARN, "Possible OOM! Dropping connection.\n");
            conn->ops->on_error(conn, CONN_ERR_READ);
            return ;
        }
        if ((size_t)recvb < sizeof(buf))
            more = false;
    }
}

void    conn_send(t_conn *conn)
{
    char    buf[4096];
    ssize_t sendb;
    ssize_t pulled;

    while (!buffer_is_empty(&conn->writebuf))
    {
        pulled = buffer_pull(&conn->writebuf, buf, sizeof(buf));
again:
        sendb = send(conn->fd, buf, pulled, 0);
        if (sendb == -1)
        {
            if (errno == EINTR)
                goto again;
            conn->ops->on_error(conn, CONN_ERR_WRITE);
            return ;
        }
        if (sendb != pulled)
        {
            /*
             *  TODO: Put back? Flush?
             */
            LOG(L_WARN, "Data lost due to inability to send it all.\n");
            break ;
        }
    }
}
