#include "server.h"

int     main(void)
{
    t_server    server;

    if (server_open(&server) != 0)
        return (1);
    return (0);
}
