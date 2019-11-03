#ifndef __CHAN_VLIST_H
# define __CHAN_VLIST_H

# include "vlist.h"
# include "channel.h"

t_channel   *chan_find_by_name(VLIST_OF(t_channel) *vl, char *name);

#endif
