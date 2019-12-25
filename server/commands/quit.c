#include "command.h"
#include "logger.h"
#include "server_conn.h"

int     command_quit(t_conn *user, int argc, char **argv)
{
    t_ircmsg    message = {
        .command = "ERROR",
        .nparams = 1,
        .params = { "Bye!" },
    };
    (void)argc;
    (void)argv;
    LOG(L_INFO, "Command QUIT\n");
    ircmsg_send(&message, user);
    server_drop(CONN_SERVER(user), user);
    return (0);
}
