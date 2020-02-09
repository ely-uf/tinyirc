#include <sys/select.h>
#include "conn.h"
#include "conn_vlist.h"
#include "ircmsg.h"
#include "server_conn.h"

void    server_conn_cb_recv(void *c, void *readfdset)
{
    fd_set  *rd_fd_set = readfdset;
    t_conn  *conn = c;

    if (FD_ISSET(conn->fd, rd_fd_set) && !conn->disconnecting)
        conn_recv(conn);
}

void    server_conn_cb_send(void *c, void *writefdset)
{
    fd_set  *wr_fd_set = writefdset;
    t_conn  *conn = c;

    if (FD_ISSET(conn->fd, wr_fd_set))
        conn_send(conn);
}

void    server_conn_cb_msg_handle(void *c, void *unused)
{
    t_conn  *conn = c;

    (void)unused;
    tinymsg_pull(&conn->msg, &conn->readbuf);
    if (tinymsg_is_complete(&conn->msg))
        conn_msg_process(conn);
}

void    server_conn_cb_disconnect(void *c, void *unused)
{
    t_conn  * const conn = c;

    (void)unused;
    if (conn->disconnecting && buffer_is_empty(&conn->writebuf))
        server_drop_now(CONN_SERVER(conn), conn);
}

void    server_conn_cb_disconnect_now(void *c, void *unused)
{
    t_conn  * const conn = c;

    (void)unused;
    server_drop_now(CONN_SERVER(conn), conn);
}
