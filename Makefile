NAME = pipex

CC = cc
CFLAGS = -Wall -Wextra -Werror $(INCLUDES)# -g3 -fsanitize=address,leak,undefined

RM = rm -f

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = ./pipex.c ./src/pipex_errors.c ./src/pipex_exec.c ./src/pipex_path.c
OBJ = $(SRC:.c=.o)
NORMAL_FLAG = .built

BONUS_SRC =./pipex_bonus.c ./src_bonus/pipex_errors_bonus.c ./src_bonus/pipex_exec_bonus.c ./src_bonus/pipex_heredoc_bonus.c ./src_bonus/pipex_path_bonus.c
OBJ_BONUS = $(BONUS_SRC:.c=.o)
BONUS_FLAG = .bonus_built

INCLUDES = -I./includes -I$(LIBFT_DIR)/includes

all: $(NAME)

bonus: $(BONUS_FLAG)

$(BONUS_FLAG): $(LIBFT) $(OBJ_BONUS)
	$(CC) $(CFLAGS) $(OBJ_BONUS) $(LIBFT) -o $(NAME)
	$(RM) $(NORMAL_FLAG)
	touch $(BONUS_FLAG)

$(LIBFT):
	make -C $(LIBFT_DIR) --no-print-directory

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)
	$(RM) $(BONUS_FLAG)
	touch $(NORMAL_FLAG)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C $(LIBFT_DIR) fclean --no-print-directory
	$(RM) $(OBJ) $(OBJ_BONUS)
	$(RM) $(NORMAL_FLAG) $(BONUS_FLAG)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
