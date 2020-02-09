NAME = server
SRC = main.c open.c serve.c accept.c server_conn.c \
	  server_conn_cb.c ircmsg.c command.c response.c
CMD_SRC = join.c privmsg.c nick.c pass.c quit.c help.c
HANDLER_SRC = err_alreadyregistred.c err_needmoreparams.c \
			  err_nonicknamegiven.c err_erroneusnickname.c\
			  err_nicknameinuse.c err_unknowncommand.c \
			  err_notregistered.c err_notexttosend.c \
			  err_norecipient.c err_nosuchnick.c
AUX_SRC = ../util/logger.c ../util/buffer.c ../util/vlist.c \
		  ../util/buffer_block.c ../util/tinymsg.c ../util/ircnick.c \
		  ../util/irccasecmp.c ../util/conn/conn.c ../util/conn/conn_io.c \
		  ../util/conn/conn_vlist.c
OBJ = $(SRC:.c=.o) $(AUX_SRC:.c=.o) \
	$(addprefix commands/,$(CMD_SRC:.c=.o)) \
	$(addprefix responses/,$(HANDLER_SRC:.c=.o))
CFLAGS = -Wall -Werror -Wextra -I ../include -I ../include/server
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

run: $(NAME)
	./$(NAME)

.PHONY = all clean fclean re run
