#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include "server.h"
#include "logger.h"
#include "conn.h"
#include "conn_vlist.h"
#include "server_conn.h"
#include "tinymsg.h"
#include "ircmsg.h"
#include "const.h"

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
    server->maxfd = server->sock;
    vlist_foreach(&server->clients, server_fdset_conn_set, &server->readset);
    vlist_foreach(&server->clients, server_fdset_conn_set, &server->writeset);
    vlist_foreach(&server->clients, server_fdset_conn_set, &server->exceptset);
    vlist_foreach(&server->clients, server_conn_fd_max, &server->maxfd);
}

int server_do_serve(t_server *server)
{
    struct timeval  timeout;
    int             ret;

    while (server->alive)
    {
        server_fdsets_setup(server);
        timeout = (struct timeval){0, 100};
        ret = select(server->maxfd + 1,
                     &server->readset,
                     &server->writeset,
                     &server->exceptset,
                     &timeout);
        if (ret == 0)
            continue ;
        if (ret == -1 && errno != EINTR && errno != EAGAIN)
        {
            LOG(L_ERROR, "select: %s\n", strerror(errno));
            break ;
        }
        if (FD_ISSET(server->sock, &server->readset))
            server_accept(server);
        vlist_foreach(&server->clients, server_conn_cb_recv, &server->readset);
        vlist_foreach(&server->clients, server_conn_cb_msg_handle, NULL);
        vlist_foreach(&server->clients, server_conn_cb_send, &server->writeset);
        vlist_foreach(&server->clients, server_conn_cb_disconnect, NULL);
    }
    vlist_foreach(&server->clients, server_conn_cb_disconnect_now, NULL);
    return (ret);
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
