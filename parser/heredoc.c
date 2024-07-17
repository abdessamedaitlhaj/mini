/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 00:37:39 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/17 08:33:20 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void    push_line(int fd, char *limiter)
// {
// 	char    *line;

// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line)
// 			break ;
// 		if (ft_strcmp(line, limiter) == 0)
// 		{
// 			free(line);
// 			break ;
// 		}
// 		write(fd, line, ft_strlen(line));
// 		free(line);
// 	}
// }

void	push_line(int fd, char *limiter, t_data *data)
{
	char	*line;
	char	*content;

	content = ft_strdup("", &data->allocated);
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
		content = ft_strjoin(content, line, data);
		content = ft_strjoin(content, "\n", data);
		free(line);
	}
	write(fd, content, ft_strlen(content));
}

void	empty_line(char *limiter)
{
	char	*line;

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



void	push_line_expand(int fd, char *limiter, t_data *data)
{
	char	*line;
	char	*content;

	content = ft_strdup("", &data->allocated);
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
		content = ft_strjoin(content, line, data);
		content = ft_strjoin(content, "\n", data);
		free(line);
	}
	content = expanding_inside(content, data);
	write(fd, content, ft_strlen(content));
}
