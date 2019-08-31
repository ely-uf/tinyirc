#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "server.h"
#include "logger.h"

static bool port_valid(char *port)
{
    const size_t len = strlen(port);

    if (len == 0 || len > 5)
        return false;

    for (size_t i = 0; i < len; i++)
    {
        if (!isdigit(port[i]))
            return false;
    }
    return true;
}

static int  server_configure(t_server *server, int argc, char **argv)
{
    if (argc <= 0)
    {
        LOG(L_INFO, "No port provided. Defaulting to %i.\n", TINYIRC_DEFPORT);
        server->port = TINYIRC_DEFPORT;
        return (0);
    }

    if (!port_valid(argv[0]))
    {
        LOG(L_ERROR, "Invalid port: '%s'\n", argv[0]);
        return (1);
    }
    LOG(L_INFO, "Server port has been set to %i.\n", atoi(argv[0]));
    server->port = atoi(argv[0]);
    return (0);
}

int     main(int argc, char **argv)
{
    t_server    server;

    signal(SIGPIPE, SIG_IGN);
    if (server_configure(&server, --argc, ++argv))
        return (1);
    if (server_open(&server) != 0)
        return (1);
    server_serve(&server);
    server_close(&server);
    return (0);
}
