#include <sys/select.h>
#include "conn.h"
#include "conn_vlist.h"

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
