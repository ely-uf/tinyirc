#include <stdbool.h>
#include <errno.h>
#include "logger.h"
#include "server.h"
#include "server_conn.h"

void    conn_read(t_conn *conn)
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
                server_drop(CONN_SERVER(conn), conn);
            return ;
        }
        LOG(L_DEBUG, "Read %zu bytes.\n", (size_t)recvb);
        put = buffer_put(&conn->readbuf, buf, recvb);
        if (put != recvb)
        {
            LOG(L_WARN, "Possible OOM! Dropping connection.\n");
            server_drop(CONN_SERVER(conn), conn);
            return ;
        }
        if ((size_t)recvb < sizeof(buf))
            more = false;
    }
}

void    conn_write(t_conn *conn)
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
            server_drop(CONN_SERVER(conn), conn);
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
