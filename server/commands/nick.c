#include "command.h"
#include "logger.h"
#include "response.h"
#include "server_conn.h"
#include <string.h>
#include <stdio.h>

typedef struct  s_nick_query
{
    t_conn      *querier;
    char        *nick;
    bool        in_use;
}               t_nick_query;

static bool nick_valid(char *nickname)
{
    return (bool)nickname;
}

static void nick_in_use_cb(void *c, void *a)
{
    t_nick_query    *query = a;
    t_conn          *conn = c;

    if (query->querier == conn || query->in_use)
        return ;

    if (strcmp(CONN_UDATA(conn)->name, query->nick) == 0)
        query->in_use = true;
}

static bool nick_in_use(t_conn *conn, char *nickname)
{
    t_nick_query query = {
        .querier = conn,
        .nick = nickname,
        .in_use = false
    };
    vlist_foreach(&CONN_SERVER(conn)->clients, nick_in_use_cb, &query);

    return (query.in_use);
}

int     command_nick(t_conn *user, int argc, char **argv)
{
    (void)user;
    (void)argc;
    (void)argv;
    LOG(L_INFO, "Command NICK\n");
    if (argc != 1)
    {
        response_numeric(user, ERR_NONICKNAMEGIVEN, argc, argv);
        return (1);
    }
    if (!nick_valid(argv[0]))
    {
        response_numeric(user, ERR_ERRONEUSNICKNAME, argc, argv);
        return (1);
    }
    if (nick_in_use(user, argv[0]))
    {
        /*
         *  TODO: Should pass nickname to the response.
         */
        response_numeric(user, ERR_NICKNAMEINUSE, argc, argv);
        return (1);
    }
    snprintf(CONN_UDATA(user)->name, sizeof(CONN_UDATA(user)->name),
            "%s", argv[0]);
    return (0);
}
