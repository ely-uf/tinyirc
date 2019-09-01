#include "command.h"
#include "logger.h"
#include "server_conn.h"

int     command_quit(t_conn *user, int argc, char **argv)
{
    (void)argc;
    (void)argv;
    LOG(L_INFO, "Command QUIT\n");
    server_drop(CONN_SERVER(user), user);
    return (0);
}
