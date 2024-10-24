NAME = pipex

CC = cc
CFLAGS = -Wall -Wextra -Werror $(INCLUDES) # -g3 -fsanitize=address

RM = rm -f

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = ./src/pipex.c
OBJ = $(SRC:.c=.o)

INCLUDES = -I./include -I$(LIBFT_DIR)

all: $(NAME)

$(NAME): $(OBJ)
	make -C $(LIBFT_DIR) --no-print-directory
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)
	make -C $(LIBFT_DIR) clean --no-print-directory

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_DIR) fclean --no-print-directory

re: fclean all

.PHONY: all clean fclean re
