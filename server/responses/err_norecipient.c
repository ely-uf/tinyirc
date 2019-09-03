#include "conn.h"
#include "ircmsg.h"
#include "response.h"
#include "logger.h"
#include <stdio.h>

int     err_norecipient(t_conn *conn, int argc, char **argv)
{
    char        cmdbuf[4];
    char        parambuf[506];
    t_ircmsg    response = {
        .command = cmdbuf,
        .nparams = 1,
        .params[0] = parambuf,
    };

    if (argc < 1)
    {
        LOG(L_ERROR, "Invalid use of %s: not enough arguments.\n", __func__);
        return (1);
    }
    snprintf(parambuf, sizeof(parambuf), "No recipient given (%s)", argv[0]);
    snprintf(cmdbuf, sizeof(cmdbuf), "%03u", ERR_NORECIPIENT);
    return ircmsg_send(&response, conn);
}
