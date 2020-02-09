#include "conn_vlist.h"
#include "server_conn.h"
#include "ircnick.h"

static bool conn_find_by_name_cmp(void *c, void *n)
{
    t_conn  *conn = c;
    char    *nick = n;

    return (ircnick_cmp(CONN_UDATA(conn)->name, nick) == 0);
}

t_conn  *conn_find_by_name(VLIST_OF(t_conn) *vl, char *name)
{
    return vlist_find(vl, conn_find_by_name_cmp, name);
}
