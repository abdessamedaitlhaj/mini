/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 00:37:39 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/31 13:59:38 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int	g_signal_flag;

void	sig_her_child(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_flag = 1;
		ft_putstr_fd("\n", STDOUT_FILENO);
		exit(4);
	}
}

void	push_loop(char *limiter, char *content, t_data *data, int fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			rl_on_new_line();
			break ;
		}
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
	exit(0);
}

void	push_line(int fd, char *limiter, t_data *data)
{
	char	*content;
	int		pid;
	int		child_exit_status;

	content = ft_strdup("", &data->allocated);
	pid = fork();
	if (pid == -1)
		fail_error("fork failed", &data->allocated);
	if (pid == 0)
		push_loop(limiter, content, data, fd);
	waitpid(pid, &child_exit_status, 0);
	if (WIFEXITED(child_exit_status) && WEXITSTATUS(child_exit_status) == 4)
	{
		data->exit_status = 1;
		g_signal_flag = 1;
		return ;
	}
}

void	empty_line_loop(char *limiter)
{
	char	*line;

	signal(SIGINT, sig_her_child);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			rl_on_new_line();
			break ;
		}
		if (ft_strcmp(line, limiter) == 0)
		{
			free(line);
			break ;
		}
		free(line);
	}
	exit(0);
}

void	empty_line(char *limiter, t_data *data)
{
	int		pid;
	int		child_exit_status;

	pid = fork();
	if (pid == -1)
		fail_error("fork failed", &data->allocated);
	if (pid == 0)
		empty_line_loop(limiter);
	waitpid(pid, &child_exit_status, 0);
	if (WIFEXITED(child_exit_status) && WEXITSTATUS(child_exit_status) == 4)
	{
		data->exit_status = 1;
		g_signal_flag = 1;
		return ;
	}
}
