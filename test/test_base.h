#ifndef __TEST_BASE_H
# define __TEST_BASE_H

# include <stdio.h>

# define ASSERT(cond, msg) if (!(cond)) printf("FAIL: %s [%s]\n", msg, #cond);

#endif
