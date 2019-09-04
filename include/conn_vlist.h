#ifndef __CONN_VLIST_H
# define __CONN_VLIST_H

# include "conn.h"
# include "vlist.h"

void    conn_read_cb(void *conn, void *readfdset);
void    conn_write_cb(void *conn, void *writefdset);
void    conn_msg_handle_cb(void *conn, void *unused);
void    conn_disconnect_cb(void *conn, void *unused);
void    conn_disconnect_now_cb(void *conn, void *unused);

t_conn  *conn_find_by_name(VLIST_OF(t_conn) *vl, char *name);

#endif
