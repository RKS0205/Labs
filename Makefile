SRC_FILES= server.c curl.c database.c response_code.c uri_path.c check_post_request.c
SRC=$(addprefix src/, $(SRC_FILES))
OBJ=$(addprefix obj/, $(SRC_FILES:.c=.o))
OBJ_DIR=obj
LIBFT=libft/libft.a
LIBFT_DIR=libft
LIB_MJSON=mjson/src/mjson.c
LIB_MONGOOSE=mongoose/mongoose.c
FLAGS= `mysql_config --cflags --libs` -lcurl
NAME=api

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ_DIR) $(OBJ)
	gcc  $(OBJ) $(LIBFT) $(LIB_MJSON) $(LIB_MONGOOSE) -o $(NAME) $(FLAGS)

obj/%.o: src/%.c
	gcc -c $< -o $@ $(FLAGS)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJ)
	rm -rf $(OBJ_B)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -rf $(NAME)
	rm -rf $(NAME_B)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
