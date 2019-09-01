#include "ircnick.h"
#include <string.h>
#include <ctype.h>

static bool ircnick_char_special(char ch)
{
    return ((ch >= 0x5B && ch <= 0x60) || (ch >= 0x7B && ch <= 0x7D));
}

bool    ircnick_valid(char *nick)
{
    const size_t    len = strlen(nick);
    size_t          i;

    if (len == 0 || len > 9)
        return (false);

    if (!isalpha(nick[0]) && !ircnick_char_special(nick[0]))
        return (false);

    i = 1;
    while (i < len)
    {
        if (!isalpha(nick[i]) && !isdigit(nick[i]) &&
            !ircnick_char_special(nick[i]) && nick[i] != '-')
            return (false);
        i++;
    }

    return (true);
}
