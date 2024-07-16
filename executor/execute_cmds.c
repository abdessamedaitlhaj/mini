/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 19:09:09 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/16 11:04:19 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	child_process(t_data *data, t_cmd *cmd)
{
	char	**args;


	args = allocate_cmd_args(data, cmd);
	if (!args)
		return ;
	exec_cmd(data, cmd->cmd, args);
}

void sigquit_handler(int sig) {
	(void)sig;
    printf("Ignoring SIGQUIT\n");
}

int	create_process(t_data *data, t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	
	pid = fork();
	if (pid == -1)
		return (error_two("fork"));
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, sig_handler);
		child_process(data, cmd);
	}
	else if (pid > 0)
	{
		if (waitpid(pid, &status, 0) == -1)
			return (error_two("waitpid"));
		get_status(&status, data);
	}
	return (0);
}

int	execute_one_node(t_data *data)
{
	if (data->cmds[0].files)
	{
		if (init_fds(data, &data->cmds[0]))
			return (ft_close_two(data->fd_in, data->fd_out));
	}
	if (data->cmds[0].files)
	{
		if (data->cmds[0].infile || data->cmds[0].heredoc)
			dup_file(INFILE, data->fd_in);
		if (data->cmds[0].outfile || data->cmds[0].append)
			dup_file(OUTFILE, data->fd_out);
		ft_close_two(data->fd_in, data->fd_out);
	}
	if (is_builtin(&data->cmds[0]))
		data->exit_status = ft_exec_builtin(&data->cmds[0], data);
	else
		create_process(data, &data->cmds[0]);
	return (0);
}

int	fork_process(t_data *data, int i, int *fd, int prev_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		error_one(fd, "fork", prev_fd);
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, sig_handler);
		dup_redir(data, i, fd, prev_fd);
		if (is_builtin(&data->cmds[i]))
		{
			data->exit_status = ft_exec_builtin(&data->cmds[i], data);
			exit(data->exit_status);
		}
		else
			child_process(data, &data->cmds[i]);
	}
	return (0);
}

int	execute_multiple_nodes(t_data *data)
{
	int	i;
	int	fd[2];
	int	prev_fd;

	i = -1;
	prev_fd = -2;
	fd[0] = -2;
	fd[1] = -2;
	while (++i < data->counter_command)
	{
		data->fd_in = -2;
		data->fd_out = -2;
		if (data->cmds[i].files)
		{
			if (init_fds(data, &data->cmds[i]))
			{
				ft_close_two(data->fd_in, data->fd_out);
				error_one(fd, NULL, prev_fd);
				data->exit_status = 1;
				continue ;
			}
		}
		if (!data->cmds[i].cmd[0])
		{
			ft_close_two(data->fd_in, data->fd_out);
			error_one(fd, NULL, prev_fd);
			continue ;
		}
		if (i < data->counter_command - 1)
			if (pipe(fd) == -1)
			{
				ft_close_two(data->fd_in, data->fd_out);
				error_one(fd, "pipe", prev_fd);
			}
		fork_process(data, i, fd, prev_fd);
		ft_close_two(data->fd_in, data->fd_out);
		if (i > 0)
			close(prev_fd);
		if (i < data->counter_command - 1)
		{
			prev_fd = fd[0];
			close(fd[1]);
		}
	}
	return (0);
}

int	execute_cmds(t_data *data)
{
	int	status;
	int stdin_copy;
	int stdout_copy;

	stdin_copy = dup(0);
	stdout_copy = dup(1);
	if (stdin_copy == -1 || stdout_copy == -1)
		return (error_two("dup"));
	// if (!data->cmds[0].cmd[0])
	// 	return (0);
	if (data->counter_command == 1)
		execute_one_node(data);
	else
	{
		execute_multiple_nodes(data);
		while (waitpid(-1, &status, 0) > 0)
			get_status(&status, data);
	}
	if (dup2(stdin_copy, STDIN_FILENO) == -1 || \
		dup2(stdout_copy, STDOUT_FILENO) == -1)
			return (error_two("dup2"));
	close(stdin_copy);
	close(stdout_copy);
	return (0);
}