#ifndef __CONNECTION_H
# define __CONNECTION_H

# include <stdlib.h>
# include <stdbool.h>
# include <netinet/ip.h>
# include "buffer.h"
# include "tinymsg.h"

# define CONN_PRIV(conn) ((conn)->priv)

struct s_conn;
struct s_ircmsg;

typedef enum    e_conn_err
{
    CONN_ERR_WRITE,
    CONN_ERR_READ,
    CONN_ERR_PARSE,
}               t_conn_err;

typedef struct  s_conn_ops
{
    int     (*init)(struct s_conn *conn, void *priv);
    int     (*destroy)(struct s_conn *conn);
    int     (*on_error)(struct s_conn *conn, t_conn_err err);
    int     (*on_message)(struct s_conn *conn, struct s_ircmsg *msg);
}               t_conn_ops;

typedef struct  s_conn
{
    int                 fd;
    t_buffer            readbuf;
    t_buffer            writebuf;
    t_tinymsg           msg;
    struct sockaddr     addr;
    socklen_t           addrlen;
    bool                disconnecting;
    void                *priv;
    const t_conn_ops    *ops;
}               t_conn;

int             conn_create(t_conn *conn, int fd, const t_conn_ops *ops, void *priv);
void            conn_addr_set(t_conn *conn, struct sockaddr *a, socklen_t l);
void            conn_recv(t_conn *conn);
void            conn_send(t_conn *conn);
void            conn_destroy(t_conn *conn);
void            conn_msg_process(t_conn *conn);

#endif
