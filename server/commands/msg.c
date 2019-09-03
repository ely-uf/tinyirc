#include "command.h"
#include "logger.h"
#include "ircmsg.h"
#include "server_conn.h"
#include "response.h"

int     command_msg(t_conn *user, int argc, char **argv)
{
    t_ircmsg    message = {
        .prefix = CONN_UDATA(user)->name,
        .command = "PRIVMSG",
        .nparams = argc,
    };
    t_conn      *recipient;

    if (argc < 2)
    {
        response_numeric(user, argc ? ERR_NOTEXTTOSEND : ERR_NORECIPIENT,
                1, (char*[]){ "PRIVMSG" });
        return (1);
    }

    for (int i = 0; i < argc; i++)
        message.params[i] = argv[i];
    /*
     *  XXX: Channel name?
     */
    recipient = server_conn_find_by_name(CONN_SERVER(user), argv[0]);
    if (!recipient)
    {
        response_numeric(user, ERR_NOSUCHNICK, 1, (char*[]){ argv[0] });
        return (1);
    }
    ircmsg_send(&message, recipient);
    return (0);
}
