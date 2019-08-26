#include "command.h"
#include "command_list.h"

const   t_command   g_command_list[] = {
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
        "CONNECT",
        command_connect,
    },
    {
        "QUIT",
        command_quit,
    },
};
