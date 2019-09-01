#include <stdio.h>
#include "ircmsg.h"
#include "logger.h"
#include "response.h"
#include "responses_list.h"
#include "server_conn.h"

t_response_handler  g_resp_handlers[] = {
    {NULL, NULL}
};

static int  response_numeric_do(t_conn *user,
                                t_resp_code response,
                                int argc,
                                char **argv)
{
    char cmdbuf[512];
    t_ircmsg const  respmsg = {
        .command = cmdbuf,
    };

    (void)argc;
    (void)argv;
    snprintf(cmdbuf, sizeof(cmdbuf), "%03hu", (unsigned short)response);
    return (ircmsg_send((t_ircmsg*)&respmsg, user));
}

int     response_numeric(t_conn *user, t_resp_code response, int ac, char **av)
{
    if ((short)response > 999)
    {
        LOG(L_WARN, "Trying to send invalid response code %hu.\n", response);
        return (1);
    }
    return response_numeric_do(user, response, ac, av);
}
