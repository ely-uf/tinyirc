#include <stdio.h>
#include "ircmsg.h"
#include "logger.h"
#include "server_conn.h"

static int  response_numeric_fill(t_conn *user, unsigned short response)
{
    char cmdbuf[512];
    t_ircmsg const  respmsg = {
        .command = cmdbuf,
    };

    snprintf(cmdbuf, sizeof(cmdbuf), "%03hu", response);
    return (ircmsg_send((t_ircmsg*)&respmsg, user));
}

int     response_numeric(t_conn *user, unsigned short response)
{
    if (response > 999)
    {
        LOG(L_WARN, "Trying to send invalid response code %hu.\n", response);
        return (1);
    }
    return response_numeric_fill(user, response);
}
