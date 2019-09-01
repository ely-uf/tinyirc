#ifndef __RESPONSE_H
# define __RESPONSE_H

# include "conn.h"

typedef enum    e_resp_code
{
    RPL_TOPIC = 332,

    ERR_NOSUCHCHANNEL = 403,
    ERR_TOOMANYCHANNELS = 405,
    ERR_UNKNOWNCOMMAND = 421,
    ERR_NONICKNAMEGIVEN = 431,
    ERR_ERRONEUSNICKNAME = 432,
    ERR_NICKNAMEINUSE = 433,
    ERR_NICKCOLLISION = 436,
    ERR_UNAVAILRESOURCE = 437,
    ERR_NOTONCHANNEL = 442,
    ERR_NEEDMOREPARAMS = 461,
    ERR_ALREADYREGISTRED = 462,
    ERR_CHANNELISFULL = 471,
    ERR_INVITEONLYCHAN = 473,
    ERR_BANNEDFROMCHAN = 474,
    ERR_BADCHANNELKEY = 475,
    ERR_BADCHANMASK = 476,
    ERR_RESTRICTED = 484,
}               t_resp_code;

int     response_numeric(t_conn *user, t_resp_code resp, int ac, char **av);

#endif
