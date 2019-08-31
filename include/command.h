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

t_command const *command_lookup(const char *name);
int             command_execute(t_command const *c,
                                t_conn *u,
                                int ac,
                                char **av);

#endif
