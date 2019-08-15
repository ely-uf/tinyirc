#include <signal.h>
#include "server.h"

int     main(void)
{
    t_server    server;

    signal(SIGPIPE, SIG_IGN);
    if (server_open(&server) != 0)
        return (1);
    server_serve(&server);
    server_close(&server);
    return (0);
}
