/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 19:09:09 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/15 17:28:14 by aait-lha         ###   ########.fr       */
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

int	execute_one_node(t_data *data)
{
	t_cmd	*cmd;
	pid_t	pid;
	int		status;

	status = 0;
	if (data->cmds[0].files)
	{
		if (init_fds(data, &data->cmds[0]))
			return (ft_close_two(data->fd_in, data->fd_out));
	}
	cmd = &data->cmds[0];
	if (!cmd->cmd[0])
		return (0);
	if (cmd->files)
	{
		if (cmd->infile || cmd->heredoc)
			dup_file(INFILE, data->fd_in);
		if (cmd->outfile || cmd->append)
			dup_file(OUTFILE, data->fd_out);
		ft_close_two(data->fd_in, data->fd_out);
	}
	if (is_builtin(cmd))
		data->exit_status = ft_exec_builtin(cmd, data);
	else
	{
		pid = fork();
		if (pid == -1)
			return (error_two("fork"));
		if (pid == 0)
			child_process(data, cmd);
		else if (pid > 0)
		{
			if (waitpid(pid, &status, 0) == -1)
				return (error_two("waitpid"));
			if (WIFEXITED(status))
				data->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				data->exit_status = WTERMSIG(status) + 128;
			if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
				printf("quit: %d\n", WTERMSIG(status));
		}
	}
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
		data->exit_status = 0;
		data->fd_in = -2;
		data->fd_out = -2;
		if (data->cmds[i].files)
		{
			if (init_fds(data, &data->cmds[i]))
			{
				ft_close_two(data->fd_in, data->fd_out);
				error_one(fd, NULL, prev_fd);
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
	status = 0;
	// if (!data->cmds[0].cmd[0])
	// 	return (0);
	if (data->counter_command == 1)
		execute_one_node(data);
	else
	{
		execute_multiple_nodes(data);
		while (waitpid(-1, &status, 0) > 0)
		{
			if (WIFEXITED(status))
				data->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				data->exit_status = WTERMSIG(status) + 128;
			if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
				printf("quit: %d\n", WTERMSIG(status));
		}
	}
	if (dup2(stdin_copy, STDIN_FILENO) == -1 || \
		dup2(stdout_copy, STDOUT_FILENO) == -1)
			return (error_two("dup2"));
	close(stdin_copy);
	close(stdout_copy);
	return (0);
}