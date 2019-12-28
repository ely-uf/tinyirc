#ifndef __CHANNAME_H
# define __CHANNAME_H

# include <stdbool.h>

int     channame_cmp(char *n1, char *n2);
bool    channame_valid(char *name);

#endif
