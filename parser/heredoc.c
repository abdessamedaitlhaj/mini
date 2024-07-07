
#include "../includes/minishell.h"

void    push_line(int fd, char *limiter)
{
	char    *line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
}
void    empty_line(char *limiter)
{
	char    *line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		free(line);
	}
}



void    push_line_expand(int fd, char *limiter, t_data *data)
{
	char    *line;
	char    *content = ft_strdup("", &data->allocated);
	int flag = 0;
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		flag = 1 ;
		if (flag)
			content = ft_strjoin(content, "\n", data);
		content = ft_strjoin(content, line, data);
		free(line);
	}
	content = expanding_inside(content, data);
	write(fd, content, ft_strlen(content));

}
