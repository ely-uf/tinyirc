#include "conn.h"
#include "ircmsg.h"
#include "response.h"
#include <stdio.h>

int     err_alreadyregistred(t_conn *conn, int argc, char **argv)
{
    char        cmdbuf[4];
    t_ircmsg    response = {
        .command = cmdbuf,
        .nparams = 1,
        .params[0] = "Unauthorized command (already registered)"
    };

    (void)argc;
    (void)argv;
    snprintf(cmdbuf, sizeof(cmdbuf), "%03u", ERR_ALREADYREGISTRED);
    return ircmsg_send(&response, conn);
}
