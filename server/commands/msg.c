#include "command.h"
#include "logger.h"

int     command_msg(t_conn *user, int argc, char **argv)
{
    (void)user;
    (void)argc;
    (void)argv;
    LOG(L_INFO, "Command MSG\n");
    return (0);
}
