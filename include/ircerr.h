#ifndef __IRCERR_H
# define __IRCERR_H

# include "ircmsg.h"
# include "conn.h"

typedef enum    e_ircerr
{
    ERR_UNKNOWNCOMMAND = 421,
}               t_ircerr;

int             ircerr_build(t_ircerr err, t_ircmsg *dst);
int             ircerr_send(t_ircerr err, t_conn *to);

#endif
