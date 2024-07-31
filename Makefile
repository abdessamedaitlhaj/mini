CC = cc
S =  -fsanitize=address -ggdb3
CFLAGS =
NAME = minishell
RM = rm -f
READLINEDIR    =    $(shell brew --prefix readline)
SRCS = 	minishell.c 					\
		parser/parsing_line.c 			\
		parser/parsing_line_utils.c		\
		parser/parsing_line_utils2.c	\
		parser/parsing_line_utils3.c	\
		parser/parsing_line_utils4.c	\
		parser/pipe.c 					\
		parser/quotes.c					\
		parser/split.c 					\
		parser/utils.c					\
		parser/utils1.c					\
		parser/utils2.c					\
		parser/utils3.c					\
		parser/utils4.c					\
		parser/ft_malloc.c				\
		parser/parsing.c 				\
		parser/parsing_utils1.c 		\
		parser/parsing_utils2.c 		\
		parser/expanding.c				\
		parser/expanding_utils.c		\
		parser/expanding_utils2.c		\
		parser/expanding_utils3.c		\
		parser/parsing_redir.c			\
		parser/heredoc.c				\
		parser/heredoc_utils.c			\
		executor/utils_one.c			\
		executor/export.c				\
		executor/env_utils.c			\
		executor/unsetenv.c				\
		executor/utils_built.c			\
		executor/env.c					\
		executor/echo.c					\
		executor/cd.c					\
		executor/handle_cmds.c			\
		executor/execute_cmd.c			\
		executor/find_exec.c			\
		executor/exit.c					\
		executor/errors_one.c			\
		executor/errors_two.c			\
		executor/inits.c				\
		executor/check_builtins.c		\
		executor/dup_fds.c				\
		executor/sigs.c					\
		executor/errors_three.c			\
		executor/utils_two.c			

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) 
	@$(CC) $(CFLAGS) $(OBJS) -lreadline  -o $(NAME) -L$(READLINEDIR)/lib

%.o: %.c includes/minishell.h
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ -I$(READLINEDIR)/include

clean:
	@$(RM) -f $(OBJS)

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus