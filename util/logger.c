#include "logger.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>

const char  *log_lvl_strtable[] = {
    "FATAL",
    "ERROR",
    "WARN",
    "INFO",
    "DEBUG"
};

int         logger_new(struct s_logger *l, t_log_fn fn, void *priv)
{
    if (!l || !fn)
        return (1);
    l->level = L_WARN;
    LOGGER_PRIV(l) = priv;
    l->log = fn;
    return (0);
}

static void default_logger_fn(t_logger *l, t_log_level lvl, const char *f, ...)
{
    va_list lst;

    if (lvl < L_FATAL || lvl > l->level)
        return ;

    printf("[%s] ", log_lvl_strtable[lvl]);
    va_start(lst, f);
    vprintf(f, lst);
    va_end(lst);
}

t_logger    *default_logger(void)
{
    static t_logger def;
    static bool     initialized;

    if (!initialized)
        logger_new(&def, default_logger_fn, NULL);
    return (&def);
}
