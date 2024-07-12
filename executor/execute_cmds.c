/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 19:09:09 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/13 00:03:13 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	child_process(t_data *data, t_cmd *cmd)
{
	char	**args;

	args = allocate_cmd_args(data, cmd);
	if (!args)
		return (1);
	exec_cmd(data, cmd->cmd, args);
	return (0);
}

int	execute_one_node(t_data *data)
{
	t_cmd	*cmd;
	pid_t	pid;
	int		status;
	int		error_file;

	status = 0;
	error_file = 0;
	if (data->cmds[0].files)
	{	
		if (data->cmds[0].infile || data->cmds[0].heredoc)
			error_file = setting_redir_in(&data->cmds[0]);
		if (!error_file && (data->cmds[0].outfile || data->cmds[0].append))
			setting_redir_out(&data->cmds[0]);
	}
	cmd = &data->cmds[0];
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
			data->exit_status = ft_exec_builtin(&data->cmds[i], data);
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
	int	error_file;

	i = -1;
	prev_fd = -1;
	error_file = 0;
	while (++i < data->counter_command)
	{
		if (!data->cmds[i].cmd[0])
			continue ;
		if (i < data->counter_command - 1)
			if (pipe(fd) == -1)
				error_one(fd, "pipe", prev_fd);
		if (is_builtin(&data->cmds[i]) && ft_strcmp(data->cmds[i].cmd, "exit"))
		{
			if (data->cmds[i].files)
			{
				if (data->cmds[i].infile || data->cmds[i].heredoc)
					error_file = setting_redir_in(&data->cmds[i]);
				if (!error_file && (data->cmds[i].outfile || data->cmds[i].append))
					setting_redir_out(&data->cmds[i]);
			}
			if (i < data->counter_command - 1 && dup2(fd[1], STDOUT_FILENO) == -1)
				error_three("dup2");
			data->exit_status = ft_exec_builtin(&data->cmds[i], data);
		}
		else
			fork_process(data, i, fd, prev_fd);
		if (i > 0)
			ft_close(prev_fd);
		if (i < data->counter_command - 1)
		{
			prev_fd = fd[0];
			ft_close(fd[1]);
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
	if (!data->cmds[0].cmd[0])
		return (0);
	if (data->counter_command == 1)
		execute_one_node(data);
	else
	{
		execute_multiple_nodes(data);
		while (waitpid(-1, &status, 0) > 0)
		{
			if (WIFEXITED(status))
				data->exit_status = WEXITSTATUS(status);
		}
	}
	if (dup2(stdin_copy, STDIN_FILENO) == -1 || \
		dup2(stdout_copy, STDOUT_FILENO) == -1)
			return (error_two("dup2"));
	ft_close(stdin_copy);
	ft_close(stdout_copy);
	return (0);
}