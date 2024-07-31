/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 01:55:39 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/31 08:27:19 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int	g_signal_flag;

void	push_expand_loop(char *limiter, char *content, t_data *data, int fd)
{
	char	*line;

	signal(SIGINT, sig_her_child);
	signal(SIGQUIT, SIG_IGN);
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
	exit(0);
}

void	push_line_expand(int fd, char *limiter, t_data *data)
{
	char	*content;
	int		pid;
	int		child_exit_status;

	content = ft_strdup("", &data->allocated);
	pid = fork();
	if (pid == -1)
		fail_error("fork failed", &data->allocated);
	if (pid == 0)
		push_expand_loop(limiter, content, data, fd);
	waitpid(pid, &child_exit_status, 0);
	if (WIFEXITED(child_exit_status) && WEXITSTATUS(child_exit_status) == 4)
	{
		data->exit_status = 1;
		g_signal_flag = 1;
		return ;
	}
}
