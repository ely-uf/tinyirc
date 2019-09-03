#include "conn.h"
#include "ircmsg.h"
#include "response.h"
#include "logger.h"
#include <stdio.h>

int     err_notexttosend(t_conn *conn, int argc, char **argv)
{
    char        cmdbuf[4];
    t_ircmsg    response = {
        .command = cmdbuf,
        .nparams = 1,
        .params[0] = "No text to send",
    };

    (void)argc;
    (void)argv;
    snprintf(cmdbuf, sizeof(cmdbuf), "%03u", ERR_NOTEXTTOSEND);
    return ircmsg_send(&response, conn);
}
