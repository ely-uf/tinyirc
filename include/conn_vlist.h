#ifndef __CONN_VLIST_H
# define __CONN_VLIST_H

# include "conn.h"
# include "vlist.h"

t_conn  *conn_find_by_name(VLIST_OF(t_conn) *vl, char *name);

#endif
