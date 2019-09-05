#ifndef __COMMAND_LIST_H
# define __COMMAND_LIST_H

# include "command.h"
# include "conn.h"

int     command_pass(t_conn *user, int argc, char **argv);
int     command_nick(t_conn *user, int argc, char **argv);
int     command_join(t_conn *user, int argc, char **argv);
int     command_privmsg(t_conn *user, int argc, char **argv);
int     command_connect(t_conn *user, int argc, char **argv);
int     command_quit(t_conn *user, int argc, char **argv);

#endif
