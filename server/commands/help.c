#include "command.h"
#include "logger.h"

static int  command_help_error(t_conn *user, char *cmdname)
{
    t_ircmsg    message = {
        .prefix = "=SERVER=",
        .command = "ERROR",
        .nparams = 2,
        .params = {
            "Unable to get HELP for command: ",
            cmdname,
        },
    };

    return ircmsg_send(&message, user);
}

static int  command_help_single(t_conn *user, t_command const *command)
{
    t_ircmsg    message = {
        .prefix  = "=SERVER=",
        .command = "PRIVMSG",
        .nparams = 1,
        .params = { (char*)command->cmdname },
    };

    ircmsg_send(&message, user);

    message.params[0] = (char*)command->syntax;
    ircmsg_send(&message, user);

    message.params[0] = (char*)command->description;
    ircmsg_send(&message, user);

    message.params[0] = "---";
    ircmsg_send(&message, user);

    return (0);
}

static int  command_help_all(t_conn *user)
{
    size_t          commands_n;
    t_command const *commands = command_list(&commands_n);
    int             ret;

    for (size_t i = 0; i < commands_n; i++)
    {
        ret = command_help_single(user, &commands[i]);
        if (ret)
            return (ret);
    }
    return (0);
}

static int  command_help_single_by_name(t_conn *user, char *cmdname)
{
    t_command const *command = command_lookup(cmdname);

    if (!command)
    {
        command_help_error(user, cmdname);
        return (1);
    }

    return command_help_single(user, command);
}

int     command_help(t_conn *user, int argc, char **argv)
{
    int ret;

    if (argc == 0)
        return command_help_all(user);

    for (int i = 0; i < argc; i++)
    {
        ret = command_help_single_by_name(user, argv[i]);
        if (ret)
            return (ret);
    }
    return (0);
}
