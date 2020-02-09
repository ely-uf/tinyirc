#include "conn.h"
#include "server_conn.h"
#include "buffer.h"
#include "logger.h"
#include "ircmsg.h"
#include <string.h>
#include <assert.h>

int     conn_create(t_conn *conn, int fd, const t_conn_ops *ops, void *priv)
{
    int ret;

    assert(ops && ops->init && ops->destroy && ops->on_error && ops->on_message);
    conn->fd = fd;
    ret = buffer_init(&conn->readbuf);
    if (ret)
    {
        LOG(L_ERROR, "Failed to initialize read buffer for a new connection\n");
        return (ret);
    }
    ret = buffer_init(&conn->writebuf);
    if (ret)
    {
        LOG(L_ERROR, "Failed to initialize wr buffer for a new connection\n");
        buffer_destroy(&conn->readbuf);
        return (ret);
    }
    ret = ops->init(conn, priv);
    if (ret)
    {
        buffer_destroy(&conn->readbuf);
        buffer_destroy(&conn->writebuf);
        return (ret);
    }
    conn->msg = (t_tinymsg){{0},0};
    conn->ops = ops;
    return (0);
}

void    conn_addr_set(t_conn *conn, struct sockaddr *addr, socklen_t len)
{
    conn->addrlen = len;
    conn->addr = *addr;
}

void    conn_destroy(t_conn *conn)
{
    /*
     *  We assume that fd has been closed by the server.
     */
    buffer_destroy(&conn->readbuf);
    buffer_destroy(&conn->writebuf);
    conn->fd = -1;
    conn->ops->destroy(conn);
    CONN_PRIV(conn) = NULL;
}

void    conn_msg_process(t_conn *conn)
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
        conn->ops->on_error(conn, CONN_ERR_PARSE);
        return ;
    }
    if (!ircmsg_empty(&msg))
    {
        ircmsg_dump(&msg);
        conn->ops->on_message(conn, &msg);
    }
    ircmsg_free(&msg);
}
