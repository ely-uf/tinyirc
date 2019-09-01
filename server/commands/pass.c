#include "command.h"
#include "logger.h"
#include "response.h"
#include "server_conn.h"
#include <stdio.h>

int     command_pass(t_conn *user, int argc, char **argv)
{
    LOG(L_INFO, "Command PASS\n");
    if (CONN_UDATA(user)->pass[0] != 0)
    {
        response_numeric(user, ERR_ALREADYREGISTRED, argc, argv);
        return (1);
    }
    if (argc != 1)
    {
        response_numeric(user, ERR_NEEDMOREPARAMS, argc, argv);
        return (1);
    }
    snprintf(CONN_UDATA(user)->pass, sizeof(CONN_UDATA(user)->pass),
            "%s", argv[0]);
    return (0);
}
