#include <sys/select.h>
#include "conn.h"
#include "conn_vlist.h"
#include "server_conn.h"

void    conn_read_cb(void *c, void *readfdset)
{
    fd_set  *rd_fd_set = readfdset;
    t_conn  *conn = c;

    if (FD_ISSET(conn->fd, rd_fd_set) && !conn->disconnecting)
        conn_read(conn);
}

void    conn_write_cb(void *c, void *writefdset)
{
    fd_set  *wr_fd_set = writefdset;
    t_conn  *conn = c;

    if (FD_ISSET(conn->fd, wr_fd_set))
        conn_write(conn);
}

void    conn_msg_handle_cb(void *c, void *unused)
{
    t_conn  *conn = c;

    (void)unused;
    tinymsg_pull(&conn->msg, &conn->readbuf);
    if (tinymsg_is_complete(&conn->msg))
        conn_msg_process(conn);
}

void    conn_disconnect_cb(void *c, void *unused)
{
    t_conn  * const conn = c;

    (void)unused;
    if (conn->disconnecting && buffer_is_empty(&conn->writebuf))
        server_drop_now(CONN_SERVER(conn), conn);
}

void    conn_disconnect_now_cb(void *c, void *unused)
{
    t_conn  * const conn = c;

    (void)unused;
    server_drop_now(CONN_SERVER(conn), conn);
}
