#include <arpa/inet.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "server.h"
#include "logger.h"

#define TINYIRC_DEFPORT 6667
#define TINYIRC_DEFADDR INADDR_ANY
#define TINYIRC_DEFBACKLOG  32

static int  server_socket_open(t_server *server)
{
    server->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server->sock < 0)
    {
        LOG(L_ERROR, "Failed to open socket.\n");
        return (1);
    }
    return (0);
}

static int  server_socket_bind(t_server *server, in_addr_t ip, uint16_t port)
{
   struct sockaddr_in   addr;
   int                  ret;

   addr = (struct sockaddr_in){
       .sin_family = AF_INET,
       .sin_port = htons(port),
       .sin_addr.s_addr = htonl(ip)
   };
   ret = bind(server->sock, (struct sockaddr*)&addr, sizeof(addr));
   if (ret)
       LOG(L_ERROR, "Failed to bind socket to %s:%hu.\n",
               inet_ntoa((struct in_addr){ip}), port);
   return (ret);
}

static int  server_socket_listen(t_server *server, int backlog)
{
    const int ret = listen(server->sock, backlog);

    if (ret)
        LOG(L_ERROR, "Failed to listen on a socket.");
    return (ret);
}

int         server_close(t_server *server)
{
    return (close(server->sock));
}

int         server_open(t_server *server)
{
    int ret;

    VLIST_INIT(t_conn, &server->clients);
    ret = server_socket_open(server);
    if (ret)
        return (ret);

    ret = server_socket_bind(server, TINYIRC_DEFADDR, TINYIRC_DEFPORT);
    if (ret)
    {
        server_close(server);
        return (ret);
    }

    ret = server_socket_listen(server, TINYIRC_DEFBACKLOG);
    if (ret)
        server_close(server);
    return (ret);
}
