#include "conn.h"
#include "ircmsg.h"
#include "response.h"
#include "logger.h"
#include <stdio.h>

int     err_needmoreparams(t_conn *conn, int argc, char **argv)
{
    char        cmdbuf[4];
    t_ircmsg    response = {
        .command = cmdbuf,
        .nparams = 2,
        .params[0] = argv[0],
        .params[1] = "Not enough parameters",
    };

    if (argc < 1)
    {
        LOG(L_ERROR, "Invalid use of %s: not enough arguments.\n", __func__);
        return (1);
    }
    snprintf(cmdbuf, sizeof(cmdbuf), "%03u", ERR_NEEDMOREPARAMS);
    return ircmsg_send(&response, conn);
}
