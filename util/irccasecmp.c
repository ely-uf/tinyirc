#include "irccasecmp.h"
#include <stdlib.h>

static int  irc_tolower(unsigned char c)
{
    if ((c >= 'A' && c <= 'Z') || (c >= '[' && c <= ']'))
        return (c + 32);

    if (c == '~')
        return ('^');

    return (c);
}

int     irccasecmp(unsigned char *n1, unsigned char *n2)
{
    int res;

    if (n1 == n2)
        return (0);

    while ((res = irc_tolower(*n1) - irc_tolower(*n2++)) == 0)
        if (*n1++ == '\0')
            break ;

    return (res);
}
