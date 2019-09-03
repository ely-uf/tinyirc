#include "server_conn.h"
#include "ircnick.h"

static bool server_conn_do_find_by_name(void *u, void *n)
{
    t_conn  * const user = u;
    char    * const nick = n;

    return (ircnick_cmp(CONN_UDATA(user)->name, nick) == 0);
}

t_conn      *server_conn_find_by_name(t_server *server, char *nick)
{
    return vlist_find(&server->clients, server_conn_do_find_by_name, nick);
}
