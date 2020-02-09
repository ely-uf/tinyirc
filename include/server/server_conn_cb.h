#ifndef __SERVER_CONN_CB_H
# define __SERVER_CONN_CB_H

# include "conn.h"
# include "vlist.h"

void    server_conn_cb_recv(void *conn, void *readfdset);
void    server_conn_cb_send(void *conn, void *writefdset);
void    server_conn_cb_msg_handle(void *conn, void *unused);
void    server_conn_cb_disconnect(void *conn, void *unused);
void    server_conn_cb_disconnect_now(void *conn, void *unused);

#endif
