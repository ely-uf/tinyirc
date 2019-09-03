#include <stdio.h>
#include "ircmsg.h"
#include "logger.h"
#include "response.h"
#include "responses_list.h"
#include "server_conn.h"

t_response_handler  g_resp_handlers[] = {
    {ERR_ALREADYREGISTRED, err_alreadyregistred},
    {ERR_NEEDMOREPARAMS, err_needmoreparams},
    {ERR_NONICKNAMEGIVEN, err_nonicknamegiven},
    {ERR_ERRONEUSNICKNAME, err_erroneusnickname},
    {ERR_NICKNAMEINUSE, err_nicknameinuse},
    {ERR_UNKNOWNCOMMAND, err_unknowncommand},
    {ERR_NOTREGISTERED, err_notregistered},
    {0, NULL}
};

static t_response_fn    response_handler_find(t_resp_code code)
{
    size_t  i;

    i = 0;
    while (true)
    {
        if (g_resp_handlers[i].code == 0)
            return (NULL);
        if (g_resp_handlers[i].code == code)
            return (g_resp_handlers[i].handler);
        i++;
    }
}

static int  response_numeric_do(t_conn *user,
                                t_resp_code response,
                                int argc,
                                char **argv)
{
    t_response_fn   handler;

    handler = response_handler_find(response);
    if (!handler)
    {
        LOG(L_WARN, "Tried to respond with unknown code: %u\n", response);
        return (1);
    }

    return handler(user, argc, argv);
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
