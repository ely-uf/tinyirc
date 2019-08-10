NAME = test
TESTED_SRC = ../util/buffer_block.c ../util/buffer.c
SRC = test_main.c $(TESTED_SRC) test_buffer_block.c test_buffer.c
OBJ = $(SRC:.c=.o)
CFLAGS = -Wall -Werror -Wextra -pedantic -std=c11 -I ../include
CC = gcc

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
