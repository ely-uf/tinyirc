#include "conn.h"
#include "ircmsg.h"
#include "response.h"
#include "logger.h"
#include <stdio.h>

int     err_nicknameinuse(t_conn *conn, int argc, char **argv)
{
    char        cmdbuf[4];
    t_ircmsg    response = {
        .command = cmdbuf,
        .nparams = 2,
        .params[0] = argv[0],
        .params[1] = "Nickname is already in use"
    };

    if (argc < 1)
    {
        LOG(L_ERROR, "Invalid use of %s: not enough arguments.\n", __func__);
        return (1);
    }
    snprintf(cmdbuf, sizeof(cmdbuf), "%03u", ERR_NICKNAMEINUSE);
    return ircmsg_send(&response, conn);
}
