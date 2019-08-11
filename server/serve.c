#include <sys/select.h>
#include <sys/time.h>
#include "server.h"
#include "logger.h"
#include "conn.h"

static int server_prepare(t_server *server)
{
    (void)server;
    return (0);
}

static void server_fdset_conn_set(void *conn, void *fdset)
{
    t_conn  *self = conn;
    fd_set  *fd_set = fdset;

    FD_SET(self->fd, fd_set);
}

static void server_conn_fd_max(void *conn, void *max)
{
    t_conn  *self = conn;
    int     *maxfd = max;

    if (self->fd > *maxfd)
        *maxfd = self->fd;
}

void        server_fdsets_setup(t_server *server)
{
    FD_ZERO(&server->readset);
    FD_ZERO(&server->writeset);
    FD_ZERO(&server->exceptset);
    FD_SET(server->sock, &server->readset);
    vlist_foreach(&server->clients, server_fdset_conn_set, &server->readset);
    vlist_foreach(&server->clients, server_fdset_conn_set, &server->writeset);
    vlist_foreach(&server->clients, server_fdset_conn_set, &server->exceptset);
    vlist_foreach(&server->clients, server_conn_fd_max, &server->maxfd);
}

int server_do_serve(t_server *server)
{
    struct timeval  timeout;
    int             ret;

    while (1)
    {
        server_fdsets_setup(server);
        timeout = (struct timeval){0, 0};
        ret = select(server->maxfd,
                     &server->readset,
                     &server->writeset,
                     &server->exceptset,
                     &timeout);
        /*
         *  TODO
         */
    }
}

int server_serve(t_server *server)
{
    int ret;

    ret = server_prepare(server);
    if (ret)
    {
        LOG(L_ERROR, "Failed to prepare server.\n");
        return (ret);
    }
    return (server_do_serve(server));
}
