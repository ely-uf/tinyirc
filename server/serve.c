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


static void conn_tinymsg_process(t_conn *conn)
{
    t_ircmsg    msg = {0, 0, {0}, 0, {{0}, 0}};
    t_tinymsg   tmsg = {{0}, 0};

    tmsg.len = tinymsg_extract(&conn->msg, tmsg.buf);
    if (tmsg.len == 0)
        return ;
    if (tmsg.len == 2 && strcmp(tmsg.buf, "\r\n") == 0)
        return ;
    if (ircmsg_parse(&msg, &tmsg))
    {
        ircmsg_free(&msg);
        server_drop(CONN_SERVER(conn), conn);
        return ;
    }
    if (!ircmsg_empty(&msg))
    {
        ircmsg_dump(&msg);
        ircmsg_handle(&msg, conn);
    }
    ircmsg_free(&msg);
}

static void msg_handle_cb(void *c, void *unused)
{
    t_conn  *conn = c;

    (void)unused;
    tinymsg_pull(&conn->msg, &conn->readbuf);
    if (tinymsg_is_complete(&conn->msg))
        conn_tinymsg_process(conn);
}

static void disconnect_cb(void *c, void *unused)
{
    t_conn  * const conn = c;

    (void)unused;
    if (conn->disconnecting && buffer_is_empty(&conn->writebuf))
        server_drop_now(CONN_SERVER(conn), conn);
}

static void disconnect_now_cb(void *c, void *unused)
{
    t_conn  * const conn = c;

    (void)unused;
    server_drop_now(CONN_SERVER(conn), conn);
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
        vlist_foreach(&server->clients, conn_read_cb, &server->readset);
        vlist_foreach(&server->clients, msg_handle_cb, NULL);
        vlist_foreach(&server->clients, conn_write_cb, &server->writeset);
        vlist_foreach(&server->clients, disconnect_cb, NULL);
    }
    vlist_foreach(&server->clients, disconnect_now_cb, NULL);
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
