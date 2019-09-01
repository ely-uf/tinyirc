#ifndef __USER_DATA_H
# define __USER_DATA_H

# include "const.h"
# include <stdbool.h>

typedef struct  s_udata
{
    char        name[TINYIRC_NICKLEN + 1];
    char        pass[510];
}               t_udata;

#endif
