#include "command.h"
#include "command_list.h"
#include "logger.h"
#include "response.h"
#include "server_conn.h"
#include <string.h>

static t_command const  g_command_list[] = {
    {
        "PASS",
        command_pass,
    },
    {
        "NICK",
        command_nick,
    },
    {
        "JOIN",
        command_join,
    },
    {
        "MSG",
        command_msg,
    },
    {
        "QUIT",
        command_quit,
    },
};

t_command const *command_lookup(char const *name)
{
    for (size_t i = 0; i < sizeof(g_command_list) / sizeof(g_command_list[0]);  i++)
    {
        if (strcmp(name, g_command_list[i].cmdname) == 0)
            return &g_command_list[i];
    }

    return (NULL);
}

int             command_execute(t_command const *cmd,
                                t_conn *user,
                                int argc,
                                char **argv)
{
    if (cmd == NULL)
    {
        LOG(L_ERROR, "Trying to execute NULL command.\n");
        return (-1);
    }
    if (strcmp(cmd->cmdname, "PASS") && strcmp(cmd->cmdname, "NICK") &&
        !CONN_UDATA(user)->name[0])
    {
        response_numeric(user, ERR_NOTREGISTERED, 0, 0);
        return (-1);
    }
    return (cmd->fn(user, argc, argv));
}
