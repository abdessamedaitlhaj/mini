/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 00:37:39 by ael-hara          #+#    #+#             */
/*   Updated: 2024/07/30 14:11:44 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern int g_signal_flag;

void	sig_her_child(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_flag = 1;
		ft_putstr_fd("\n", STDOUT_FILENO);
		exit(4);
	}
}

void	push_line(int fd, char *limiter, t_data *data)
{
	char	*line;
	char	*content;
	content = ft_strdup("", &data->allocated);
	int pid = fork();
	if (pid == -1)
		fail_error("fork failed", &data->allocated);
	if (pid == 0)
	{
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
			content = ft_strjoin(content, line, data);
			content = ft_strjoin(content, "\n", data);
			free(line);
		}
		write(fd, content, ft_strlen(content));
		exit(0);
	}
	int child_exit_status;
	if (waitpid(pid, &child_exit_status, 0) > 0)
		fail_error("waitpid failed", &data->allocated);
	if (WIFEXITED(child_exit_status) && WEXITSTATUS(child_exit_status) == 4)
	{
		data->exit_status = 1;
		g_signal_flag = 1;
		return;
	}
}

void	empty_line(char *limiter, t_data *data)
{
	char	*line;
	int		pid;

	pid = fork();
	if (pid == -1)
		fail_error("fork failed", &data->allocated);
	if (pid == 0)
	{
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
	int child_exit_status;
	if (waitpid(pid, &child_exit_status, 0) > 0)
		fail_error("waitpid failed", &data->allocated);
	if (WIFEXITED(child_exit_status) && WEXITSTATUS(child_exit_status) == 4)
	{
		data->exit_status = 1;
		g_signal_flag = 1;
		return;
	}
}

void	push_line_expand(int fd, char *limiter, t_data *data)
{
	char	*line;
	char	*content;
	content = ft_strdup("", &data->allocated);
	int pid = fork();
	if (pid == -1)
		fail_error("fork failed", &data->allocated);
	if (pid == 0)
	{
		signal(SIGINT, sig_her_child);
		signal(SIGQUIT, SIG_IGN);
		while (1)
		{
			line = readline("> ");
			if (!line)
				break ;
			if (ft_strcmp(line, limiter) == 0 )
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
	 int child_exit_status;
	if (waitpid(pid, &child_exit_status, 0) > 0)
		fail_error("waitpid failed", &data->allocated);
	if (WIFEXITED(child_exit_status) && WEXITSTATUS(child_exit_status) == 4)
	{
		data->exit_status = 1;
		g_signal_flag = 1;
		return;
	}
}
