CC = cc
S =  -fsanitize=address -ggdb3
CFLAGS = -Wall -Wextra -Werror
NAME = minishell
READLINEDIR    =    $(shell brew --prefix readline)
SRCS = 	minishell.c 					\
		parser/parsing_line.c 			\
		parser/pipe.c 					\
		parser/quotes.c					\
		parser/split.c 					\
		parser/utils.c					\
		parser/utils1.c					\
		parser/ft_malloc.c				\
		parser/utils2.c					\
		parser/parsing.c 				\
		parser/expanding.c				\
		parser/parsing_line_utils.c		\
		parser/utils3.c					\
		parser/parsing_redir.c			\
		parser/heredoc.c				\
		executor/utils_one.c			\
		executor/export.c				\
		executor/unsetenv.c				\
		executor/utils_built.c			\
		executor/pwd.c					\
		executor/env.c					\
		executor/echo.c					\
		executor/cd.c					\
		executor/execute_cmds.c			\
		executor/find_exec.c			\
		executor/ft_split.c				\
		executor/exit.c					\
		executor/errors.c				\
		executor/redirections.c			\
		executor/check_builtins.c		\
		executor/dup_fds.c				\
		executor/sigs.c				

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -lreadline -o $(NAME) -L$(READLINEDIR)/lib

%.o: %.c includes/minishell.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(READLINEDIR)/include

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus