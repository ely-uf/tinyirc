#include "command.h"
#include "logger.h"

int     command_nick(t_conn *user, int argc, char **argv)
{
    (void)user;
    (void)argc;
    (void)argv;
    LOG(L_INFO, "Command NICK\n");
    return (0);
}
