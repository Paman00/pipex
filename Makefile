NAME = pipex

CC = cc
CFLAGS = -Wall -Wextra -Werror $(INCLUDES) # -g3 -fsanitize=address,leak,undefined

RM = rm -f

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = ./pipex.c ./src/pipex_errors.c ./src/pipex_exec.c ./src/pipex_path.c
OBJ = $(SRC:.c=.o)

BONUS_SRC =./pipex_bonus.c ./src_bonus/pipex_errors_bonus.c ./src_bonus/pipex_exec_bonus.c ./src_bonus/pipex_heredoc_bonus.c ./src_bonus/pipex_path_bonus.c
OBJ_BONUS = $(BONUS_SRC:.c=.o)

INCLUDES = -I./includes -I$(LIBFT_DIR)/includes

all: $(NAME)

#bonus: $(OBJ_BONUS)
#	make -C $(LIBFT_DIR) re --no-print-directory
#	$(CC) $(CFLAGS) $(OBJ_BONUS) $(LIBFT) -o $(NAME)

$(NAME): $(OBJ)
	make -C $(LIBFT_DIR) re --no-print-directory
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ) $(OBJ_BONUS)
	make -C $(LIBFT_DIR) clean --no-print-directory

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_DIR) fclean --no-print-directory

re: fclean all

.PHONY: all clean fclean re bonus
