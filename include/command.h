#ifndef __COMMAND
# define __COMMAND

# include "conn.h"
# include "ircmsg.h"

typedef int     (*command_handler)(t_conn *user, int argc, char **argv);

typedef struct  s_command
{
    const char * const  cmdname;
    command_handler     fn;
}               t_command;

int     command_execute_from_msg(t_conn *user, t_ircmsg *msg);

#endif
