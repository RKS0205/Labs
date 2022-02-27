API_SRC_FILES= server.c curl.c database.c response_code.c uri_path.c check_post_request.c
CLI_SRC_FILES= cli.c str_check.c get_next_line.c database.c curl.c
A_SRC=$(addprefix src/, $(API_SRC_FILES))
C_SRC=$(addprefix src/, $(CLI_SRC_FILES))
A_OBJ=$(addprefix obj/, $(API_SRC_FILES:.c=.o))
C_OBJ=$(addprefix obj/, $(CLI_SRC_FILES:.c=.o))
OBJ_DIR=obj
LIBFT=libft/libft.a
LIBFT_DIR=libft
LIB_MJSON=mjson/src/mjson.c
LIB_MONGOOSE=mongoose/mongoose.c
FLAGS= `mysql_config --cflags --libs` -lcurl
API_NAME=api
CLI_NAME=cli

all: $(API_NAME) $(CLI_NAME)

$(API_NAME): $(LIBFT) $(OBJ_DIR) $(A_OBJ)
	gcc  $(A_OBJ) $(LIBFT) $(LIB_MJSON) $(LIB_MONGOOSE) -o $(API_NAME) $(FLAGS)

$(CLI_NAME): $(LIBFT) $(OBJ_DIR) $(C_OBJ)
	gcc  $(C_OBJ) $(LIBFT) $(LIB_MJSON) $(LIB_MONGOOSE) -o $(CLI_NAME) $(FLAGS)

obj/%.o: src/%.c
	gcc -c $< -o $@ $(FLAGS)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -rf $(A_OBJ)
	rm -rf $(C_OBJ)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -rf $(API_NAME)
	rm -rf $(CLI_NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
