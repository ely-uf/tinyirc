#ifndef __RESPONSES_LIST
# define __RESPONSES_LIST

# include "conn.h"

typedef int     (*t_response_fn)(t_conn *conn, int argc, char **argv);
typedef struct  s_response_handler
{
    unsigned short  code;
    t_response_fn   handler;
}               t_response_handler;

extern t_response_handler   *g_response_handlers;

int err_alreadyregistred(t_conn *conn, int argc, char **argv);
int err_needmoreparams(t_conn *conn, int argc, char **argv);
int err_nonicknamegiven(t_conn *conn, int argc, char **argv);
int err_erroneusnickname(t_conn *conn, int argc, char **argv);
int err_nicknameinuse(t_conn *conn, int argc, char **argv);
int err_unknowncommand(t_conn *conn, int argc, char **argv);
int err_notregistered(t_conn *conn, int argc, char **argv);

#endif
