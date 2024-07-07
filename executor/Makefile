CC = cc
CFLAGS =
NAME = minishell
SRC = ../main.c  ../parsing_line.c ../pipe.c ../quotes.c ../split.c \
		../utils.c ../utils1.c ../ft_malloc.c ../utils2.c ../parsing.c \
		 ../expanding.c ../parsing_line_utils.c ../utils3.c ../parsing_redir.c ../heredoc.c

SRCS =  utils_one.c export.c \
		unsetenv.c utils_built.c \
		pwd.c env.c echo.c cd.c \
		execute_cmds.c find_exec.c \
		ft_split.c exit.c

OBJ = $(SRC:.c=.o)
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(OBJS)
	$(CC) $(CFLAGS) $(OBJ) $(OBJS) -lreadline -o $(NAME)

%.o: %.c executor.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(OBJS)
	
fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus