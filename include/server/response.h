#ifndef __RESPONSE_H
# define __RESPONSE_H

# include "conn.h"
# include "const.h"

int     response_numeric(t_conn *user, t_resp_code resp, int ac, char **av);

#endif
