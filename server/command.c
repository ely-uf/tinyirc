#include "command.h"
#include "command_list.h"
#include "logger.h"
#include "response.h"
#include "server_conn.h"
#include <string.h>

#define ARRAY_LEN(_arr) (sizeof(_arr) / sizeof(_arr[0]))

static t_command const  g_command_list[] = {
    {
        .cmdname = "PASS",
        .syntax = "PASS <password>",
        .description =
            "The PASS command is used to set a 'connection password'. The\n"
            "optional password can and MUST be set before any attempt to register\n"
            "the connection is made.  Currently this requires that user send a\n"
            "PASS command before sending the NICK/USER combination.",
        .fn = command_pass,
        .flags = CF_NONE,
    },
    {
        .cmdname = "NICK",
        .syntax = "NICK <nickname>",
        .description =
            "NICK command is used to give user a nickname or change the existing\n"
            "one.",
        .fn = command_nick,
        .flags = CF_NONE,
    },
    {
        .cmdname = "JOIN",
        .syntax = "JOIN ( <channel> *( \",\" <channel> ) "
                  "[ <key> *( \",\" <key> ) ] ) / \"0\"",
        .description =
            "The JOIN command is used by a user to request to start listening to\n"
            "the specific channel.  Servers MUST be able to parse arguments in the\n"
            "form of a list of target, but SHOULD NOT use lists when sending JOIN\n"
            "messages to clients.",
        .fn = command_join,
        .flags = CF_REGISTRATION_REQUIRED,
    },
    {
        .cmdname = "PRIVMSG",
        .syntax = "PRIVMSG <msgtarget> <text to be sent>",
        .description =
            "PRIVMSG is used to send private messages between users, as well as to\n"
            "send messages to channels.  <msgtarget> is usually the nickname of\n"
            "the recipient of the message, or a channel name.",
        .fn = command_privmsg,
        .flags = CF_REGISTRATION_REQUIRED,
    },
    {
        .cmdname = "QUIT",
        .syntax = "QUIT [ <Quit Message> ]",
        .description =
            "A client session is terminated with a quit message.  The server\n"
            "acknowledges this by sending an ERROR message to the client.",
        .fn = command_quit,
        .flags = CF_NONE,
    },
    {
        .cmdname = "HELP",
        .syntax = "HELP",
        .description =
            "HELP is used to get the description of available commands.",
        .fn = command_help,
        .flags = CF_NONE,
    },
};

static bool     command_requires_registration(t_command const * cmd)
{
    return cmd->flags & CF_REGISTRATION_REQUIRED;
}

t_command const *command_lookup(char const *name)
{
    for (size_t i = 0; i < ARRAY_LEN(g_command_list);  i++)
    {
        if (strcmp(name, g_command_list[i].cmdname) == 0)
            return &g_command_list[i];
    }

    return (NULL);
}

t_command const *command_list(size_t *amount)
{
    *amount = ARRAY_LEN(g_command_list);
    return g_command_list;
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
    if (command_requires_registration(cmd) && !CONN_UDATA(user)->name[0])
    {
        response_numeric(user, ERR_NOTREGISTERED, 0, 0);
        return (-1);
    }
    return (cmd->fn(user, argc, argv));
}
