#include "channame.h"
#include <stdlib.h>

static int  channame_lower(char a)
{
    if ((a >= 'A' && a <= 'Z') || (a >= '[' && a <= '^'))
        return a + 32;
    return a;
}

int         channame_cmp(char *n1, char *n2)
{
    int result;

    while (*n1 || *n2)
    {
        result = channame_lower(*n1++) - channame_lower(*n2++);
        if (result)
            return result;
    }

    return 0;
}

static bool chan_char_valid(unsigned char c)
{
    return (c >= 0x1 && c <= 0x6) || (c >= 0x8 && c <= 0x9)     ||
           (c >= 0xB && c <= 0xC) || (c >= 0xE && c <= 0x1F)    ||
           (c >= 0x21 && c <= 0x2B) || (c >= 0x2D && c <= 0x39) ||
           (c >= 0x3B && c <= 0xFF);
}

bool        channame_valid(char *name)
{
    if (name[0] != '#' && name[0] != '+' && name[0] != '&')
        return false;
    if (name[1] == ':')
        return false;
    for (size_t i = 1; name[i]; i++)
    {
        if (!chan_char_valid(name[i]) && name[i] != ':')
            return false;
    }
    return true;
}
