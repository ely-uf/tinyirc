SERVER_DIR = server
SERVER_BUILT = .stamp_built_server

RM = rm
TOUCH = touch

all: server

clean:
	$(MAKE) -C $(SERVER_DIR) clean

fclean: clean
	$(MAKE) -C $(SERVER_DIR) fclean
	$(RM) $(SERVER_BUILT)

re: fclean all

server: $(SERVER_BUILT)
$(SERVER_BUILT):
	$(MAKE) -C $(SERVER_DIR)
	$(TOUCH) $(SERVER_BUILT)

run: $(SERVER_BUILT)
	$(MAKE) -C $(SERVER_DIR) run

.PHONY = all server clean fclean re run
