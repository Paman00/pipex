NAME = pipex

CC = cc
CFLAGS = -Wall -Wextra -Werror $(INCLUDES) -g3 # -fsanitize=address

RM = rm -f

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

SRC = ./src/pipex.c # ./src/ft_cmd.c ./src/pipex_cmd.c
OBJ = $(SRC:.c=.o)

BONUS_SRC =./src/pipex_bonus.c # ./src/ft_cmd.c ./src/pipex_cmd_bonus.c ./src/pipex_heredoc_bonus.c
OBJ_BONUS = $(BONUS_SRC:.c=.o)

INCLUDES = -I./include -I$(LIBFT_DIR)

all: $(NAME)

bonus: $(OBJ_BONUS)
	make -C $(LIBFT_DIR) re --no-print-directory
	$(CC) $(CFLAGS) $(OBJ_BONUS) $(LIBFT) -o $(NAME)

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
