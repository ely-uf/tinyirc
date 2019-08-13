#include <unistd.h>
#include <arpa/inet.h>
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

    addrlen = sizeof(addr);
    newfd = accept(serv->sock, &addr, &addrlen);
    if (newfd < 0)
        return (newfd);

    LOG(L_INFO, "New incoming connection from: %s:%i\n",
            inet_ntoa(((struct sockaddr_in*)&addr)->sin_addr),
            ntohs(((struct sockaddr_in*)&addr)->sin_port));
    ret = conn_create(&conn, newfd, serv);
    if (ret)
    {
        LOG(L_WARN, "Failed to create a new connection.\n");
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
        LOG(L_WARN, "serv does not correspond to the server set in conn.\n");

    LOG(L_INFO, "Dropping the connection with %s:%i\n",
            inet_ntoa(((struct sockaddr_in*)&conn->addr)->sin_addr),
            ntohs(((struct sockaddr_in*)&conn->addr)->sin_port));
    close(conn->fd);
    conn_destroy(conn);
    VLIST_DELETE(t_conn, &serv->clients, *conn);

    return (0);
}
