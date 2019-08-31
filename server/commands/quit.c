#include "command.h"
#include "logger.h"

int     command_quit(t_conn *user, int argc, char **argv)
{
    (void)user;
    (void)argc;
    (void)argv;
    LOG(L_INFO, "Command QUIT\n");
    server_drop(user->serv, user);
    return (0);
}
