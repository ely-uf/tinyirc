#ifndef __IRCMSG_H
# define __IRCMSG_H

# include <stdbool.h>
# include "conn.h"
# include "const.h"
# include "tinymsg.h"

typedef struct  s_ircmsg
{
    char        *prefix;
    char        *command;
    char        *params[15];
    short       nparams;
    t_tinymsg   underlying;
}               t_ircmsg;

int     ircmsg_parse(t_ircmsg *msg, t_tinymsg *from);
bool    ircmsg_empty(t_ircmsg *msg);
void    ircmsg_dump(t_ircmsg *msg);
void    ircmsg_handle(t_ircmsg *msg, t_conn *conn);
ssize_t ircmsg_send(t_ircmsg *msg, t_conn *conn);
void    ircmsg_free(t_ircmsg *msg);

#endif
