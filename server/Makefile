NAME = server
SRC = main.c open.c serve.c accept.c conn.c conn_io.c conn_vlist.c \
	  ircmsg.c command.c response.c
CMD_SRC = join.c msg.c nick.c pass.c quit.c
HANDLER_SRC = err_alreadyregistred.c err_needmoreparams.c \
			  err_nonicknamegiven.c err_erroneusnickname.c\
			  err_nicknameinuse.c
AUX_SRC = ../util/logger.c ../util/buffer.c ../util/vlist.c \
		  ../util/buffer_block.c ../util/tinymsg.c
OBJ = $(SRC:.c=.o) $(AUX_SRC:.c=.o) \
	$(addprefix commands/,$(CMD_SRC:.c=.o)) \
	$(addprefix responses/,$(HANDLER_SRC:.c=.o))
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
