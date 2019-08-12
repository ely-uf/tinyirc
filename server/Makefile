NAME = server
SRC = main.c open.c serve.c accept.c conn.c conn_io.c
AUX_SRC = ../util/logger.c ../util/buffer.c ../util/vlist.c ../util/buffer_block.c
OBJ = $(SRC:.c=.o) $(AUX_SRC:.c=.o)
CFLAGS = -Wall -Werror -Wextra -I ../include
CC = gcc

ifeq (${DEBUG},y)
	CFLAGS += -g
endif

all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
