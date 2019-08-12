#ifndef __CONN_VLIST_H
# define __CONN_VLIST_H

# include "conn.h"
# include "vlist.h"

void    conn_read_cb(void *conn, void *readfdset);
void    conn_write_cb(void *conn, void *writefdset);

#endif
