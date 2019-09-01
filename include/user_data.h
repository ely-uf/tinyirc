#ifndef __USER_DATA_H
# define __USER_DATA_H

# include "const.h"

typedef struct  s_udata
{
    char        name[TINYIRC_NICKLEN + 1];
}               t_udata;

#endif
