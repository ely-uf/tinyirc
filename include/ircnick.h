#ifndef __IRCNICK_H
# define __IRCNICK_H

# include <stdbool.h>

bool    ircnick_valid(char *nick);
int     ircnick_cmp(char *n1, char *n2);

#endif
