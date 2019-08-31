#include "command.h"
#include "logger.h"

int     command_join(t_conn *user, int argc, char **argv)
{
    (void)user;
    (void)argc;
    (void)argv;
    LOG(L_INFO, "Command JOIN\n");
    return (0);
}
