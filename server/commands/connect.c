#include "command.h"
#include "logger.h"

int     command_connect(t_conn *user, int argc, char **argv)
{
    (void)user;
    (void)argc;
    (void)argv;
    LOG(L_INFO, "Command CONNECT\n");
    return (0);
}
