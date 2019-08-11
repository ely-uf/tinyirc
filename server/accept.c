#include <unistd.h>
#include "logger.h"
#include "server.h"
#include "conn.h"

int     server_accept(t_server *serv)
{
    struct sockaddr addr;
    socklen_t       addrlen;
    int             newfd;
    int             ret;
    t_conn          conn;

    newfd = accept(serv->sock, &addr, &addrlen);
    if (newfd < 0)
        return (newfd);

    ret = conn_create(&conn, newfd, serv);
    if (ret)
    {
        close(newfd);
        return (ret);
    }
    conn_addr_set(&conn, &addr, addrlen);
    ret = VLIST_ADD(t_conn, &serv->clients, conn);
    return (ret);
}

int     server_drop(t_server *serv, t_conn *conn)
{
    if (serv != conn->serv)
        LOG(L_WARN, "serv does not correspond to the server set in conn.");

    close(conn->fd);
    conn_destroy(conn);
    VLIST_DELETE(t_conn, &serv->clients, *conn);

    return (0);
}
