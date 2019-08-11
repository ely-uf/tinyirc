#ifndef __LOGGER_H
# define __LOGGER_H

# include <pthread.h>

typedef enum    e_log_level
{
    L_FATAL,
    L_ERROR,
    L_WARN,
    L_INFO,
    L_DEBUG,
}               t_log_level;

struct  s_logger;

typedef void    (t_log_fn)(struct s_logger *l,
                           t_log_level log_level,
                           const char *fmt,
                           ...);

# define LOGGER_PRIV(logger) (logger)->priv

typedef struct  s_logger
{
    t_log_fn    *log;
    t_log_level level;
    void        *priv;
}               t_logger;

t_logger    *default_logger(void);
int         logger_new(t_logger *l, t_log_fn fn, void *priv);

# define _LOG(lgr, l, f, ...) (lgr)->log((lgr), l, f, ##__VA_ARGS__)
# define LOG_WITH(lg, v, f, ...) ({     \
        typeof(lg) l = (lg);            \
        _LOG(l, v, f, ##__VA_ARGS__);   \
    })
# define LOG(lvl, fmt, ...) LOG_WITH(default_logger(), lvl, fmt, ##__VA_ARGS__)

#endif
