#include "server_conn.h"
#include "conn_vlist.h"

t_conn      *server_conn_find_by_name(t_server *server, char *nick)
{
    return conn_find_by_name(&server->clients, nick);
}
