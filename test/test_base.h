#ifndef __TEST_BASE_H
# define __TEST_BASE_H

# include <stdio.h>

# define ASSERT_FMT "FAIL: %s [%s]\n"
# define ASSERT_F(cond, msg) printf(ASSERT_FMT, msg, #cond); return (-1);
# define ASSERT(cond, msg) if (!(cond)) do { ASSERT_F(cond, msg); } while (0)

#endif
