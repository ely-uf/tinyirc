#include "command.h"
#include "logger.h"

int     command_pass(t_conn *user, int argc, char **argv)
{
    (void)user;
    (void)argc;
    (void)argv;
    LOG(L_INFO, "Command PASS\n");
    return (0);
}
