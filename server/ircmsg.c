#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "ircmsg.h"
#include "logger.h"
#include "command.h"
#include "response.h"

#define IRCMSG_LEN(msg) (msg)->underlying.len
#define IRCMSG_BUF(msg) (msg)->underlying.buf

static char *create_from(char *buf, size_t len)
{
    char    *res = malloc(len + 1);
    res[len] = 0;
    memcpy(res, buf, len);
    return (res);
}

static int  ircmsg_command_parse_number(t_ircmsg *msg, size_t *offset)
{
    for (size_t i = 0; i < 3 && i + *offset < IRCMSG_LEN(msg); i++)
        if (!isdigit(IRCMSG_BUF(msg)[i + *offset]))
            return (1);
    if (IRCMSG_LEN(msg) > 3 + *offset + __builtin_strlen(TINYIRC_MSG_SEP) &&
        IRCMSG_BUF(msg)[*offset + 3] != ' ')
    {
        LOG(L_ERROR, "IRCMSG_LEN(msg) [%i] | IRCMSG_BUF(msg)[*offset + 3] [%i]\n", IRCMSG_LEN(msg), IRCMSG_BUF(msg)[*offset + 3]);
        return (1);
    }
    msg->command = create_from(&IRCMSG_BUF(msg)[*offset], 3);
    *offset += 3;
    if (*offset <= IRCMSG_LEN(msg) && IRCMSG_BUF(msg)[*offset] == ' ')
        (*offset)++;
    return (0);
}

static int  ircmsg_command_parse_string(t_ircmsg *msg, size_t *offset)
{
    size_t  i;

    for (i = 0; i + *offset < IRCMSG_LEN(msg) && !isspace(IRCMSG_BUF(msg)[i + *offset]); i++)
        if (!isalpha(IRCMSG_BUF(msg)[i + *offset]))
            return (1);

    if (i == 0)
        return (1);

    msg->command = create_from(&IRCMSG_BUF(msg)[*offset], i);
    *offset += i;
    if (*offset <= IRCMSG_LEN(msg) && IRCMSG_BUF(msg)[*offset] == ' ')
        (*offset)++;

    return (0);
}

static int  ircmsg_command_parse(t_ircmsg *msg, size_t *offset)
{
    if (ircmsg_command_parse_number(msg, offset) == 0)
        return (0);
    return (ircmsg_command_parse_string(msg, offset));
}

static bool ircmsg_nospcrlfcl(char c)
{
    return (c != 0 && c != '\r' && c != '\n' && c != ':');
}

static int  ircmsg_param_single_parse(t_ircmsg *msg, size_t *offset)
{
    size_t  i;

    for (i = 0;
         i + *offset < IRCMSG_LEN(msg) &&
         !isspace(IRCMSG_BUF(msg)[i + *offset]);
         i++)
        if (!ircmsg_nospcrlfcl(IRCMSG_BUF(msg)[i + *offset]) &&
                (i == 0 && IRCMSG_BUF(msg)[i + *offset] == ':'))
            return (1);

    if (i == 0)
        return (1);

    msg->params[msg->nparams] = create_from(&IRCMSG_BUF(msg)[*offset], i);
    msg->nparams++;
    *offset += i;
    if (*offset <= IRCMSG_LEN(msg) && IRCMSG_BUF(msg)[*offset] == ' ')
        (*offset)++;

    return (0);
}

static int  ircmsg_param_single_trailing_parse(t_ircmsg *msg, size_t *offset)
{
    size_t  i;

    if (*offset >= IRCMSG_LEN(msg) || IRCMSG_BUF(msg)[*offset] != ':')
        return (1);

    for (i = 1;
         i + *offset < IRCMSG_LEN(msg) &&
            (IRCMSG_BUF(msg)[i + *offset] == ' ' ||
             IRCMSG_BUF(msg)[i + *offset] == ':' ||
             ircmsg_nospcrlfcl(IRCMSG_BUF(msg)[i + *offset]));
         i++);
    if (i == 1)
        return (1);

    msg->params[msg->nparams] = create_from(&IRCMSG_BUF(msg)[*offset + 1], i - 1);
    msg->nparams++;
    *offset += i;
    if (*offset <= IRCMSG_LEN(msg) && IRCMSG_BUF(msg)[*offset] == ' ')
        (*offset)++;

    return (0);
}

static int  ircmsg_params_parse(t_ircmsg *msg, size_t *offset)
{
    while ((size_t)msg->nparams < sizeof(msg->params) / sizeof(msg->params[0]))
    {
        if (ircmsg_param_single_parse(msg, offset) != 0)
            break ;
    }
    if (IRCMSG_BUF(msg)[*offset] == ':')
        ircmsg_param_single_trailing_parse(msg, offset);
    /*
     *  Params are optional. Even if parse fails, we backtrack.
     */
    return (0);
}

static int  ircmsg_crlf_parse(t_ircmsg *msg, size_t *offset)
{
    if (*offset + 1 > IRCMSG_LEN(msg))
    {
        LOG(L_DEBUG, "There is no space for crlf.\n");
        return (1);
    }
    if (IRCMSG_BUF(msg)[*offset] != '\r' ||
        IRCMSG_BUF(msg)[*offset + 1] != '\n')
    {
        LOG(L_DEBUG, "Invalid crlf [%#hhx %#hhx].\n",
                IRCMSG_BUF(msg)[*offset], IRCMSG_BUF(msg)[*offset + 1]);
        return (1);
    }
    *offset += 2;
    return (0);
}

int         ircmsg_parse(t_ircmsg *msg, t_tinymsg *from)
{
    int     ret;
    size_t  offset;

    if (from->len < __builtin_strlen(TINYIRC_MSG_SEP))
        return (1);
    msg->underlying = *from;
    offset = 0;
    ret = ircmsg_command_parse(msg, &offset);
    if (ret != 0)
    {
        LOG(L_INFO, "Invalid command.\n");
        return (ret);
    }
    ircmsg_params_parse(msg, &offset);
    return ircmsg_crlf_parse(msg, &offset);
}

void        ircmsg_dump(t_ircmsg *msg)
{
    LOG(L_INFO, "IRC Message:\n");
    LOG(L_INFO, "\tCommand: %s\n", msg->command);
    for (short i = 0; i < msg->nparams; i++)
        LOG(L_INFO, "\t\t[%hi] = %s\n", i, msg->params[i]);
}

bool        ircmsg_empty(t_ircmsg *msg)
{
    return (msg->underlying.len == __builtin_strlen(TINYIRC_MSG_SEP) &&
            strcmp(msg->underlying.buf, TINYIRC_MSG_SEP) == 0);
}

void        ircmsg_free(t_ircmsg *msg)
{
    free(msg->command);
    for (short i = 0; i < msg->nparams; i++)
        free(msg->params[i]);
}

void        ircmsg_handle(t_ircmsg *msg, t_conn *user)
{
    t_command const *cmd = command_lookup(msg->command);

    if (!cmd)
    {
        LOG(L_ERROR, "Invalid command: %s.\n", msg->command);
        response_numeric(user, ERR_UNKNOWNCOMMAND, 1,
                (char*[2]){ msg->command, NULL });
        return ;
    }

    command_execute(cmd, user, msg->nparams, msg->params);
}

ssize_t     ircmsg_serialize(t_ircmsg *msg, char *buf, size_t size)
{
    size_t  offset;
    short   cmd;

    if (msg->prefix)
        offset = snprintf(buf, size, ":%s %s", msg->prefix, msg->command);
    else
        offset = snprintf(buf, size, "%s", msg->command);
    cmd = 0;
    while (cmd < msg->nparams)
    {
        if (offset >= size)
        {
            LOG(L_INFO, "Message with '%s' is too big!\n", msg->command);
            return (-1);
        }
        offset += snprintf(&buf[offset], size - offset, cmd == msg->nparams - 1
                ? " :%s" : " %s", msg->params[cmd]);
        cmd++;
    }
    if (offset >= size - 2)
    {
        LOG(L_INFO, "No space left for '%s'. Truncating.\n", msg->command);
        offset = size - 3;
    }
    offset += snprintf(&buf[offset], size - offset, "\r\n");
    return (offset);
}

ssize_t     ircmsg_send(t_ircmsg *msg, t_conn *user)
{
    char    buf[TINYIRC_MSG_LEN];
    ssize_t size;

    size = ircmsg_serialize(msg, buf, sizeof(buf));
    if (size <= 0)
        return (size);
    LOG(L_INFO, "Serialized message: %s", buf);
    return (buffer_put(&user->writebuf, buf, size));
}
