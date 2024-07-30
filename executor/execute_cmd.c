/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 22:16:07 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/30 09:42:47 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	fork_process(t_data *data, int i, int *fd, int *prev_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		close_streams(&data->fd_in, &data->fd_out, data);
		if (i > 0 && i < data->counter_command - 1)
			ft_close(prev_fd, data);
		close_pipe(fd, data);
		fail_error("fork", &data->allocated);
	}
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, sig_handler);
		dup_redir(data, i, fd, *prev_fd);
		if (is_builtin(&data->cmds[i]))
			exit(ft_exec_builtin(&data->cmds[i], data));
		else
			exec_cmd(data, &data->cmds[i]);
	}
	return (0);
}

int	create_process(t_data *data, t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	status = 0;
	data->envp = get_env_array(data->env, data);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close_streams(&data->fd_in, &data->fd_out, data);
		return (1);
	}
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, sig_handler);
		exec_cmd(data, cmd);
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		close_streams(&data->fd_in, &data->fd_out, data);
		fail_error("waitpid", &data->allocated);
	}
	get_status(data, status);
	return (data->exit_status);
}

void	exec_cmd(t_data *data, t_cmd *cmd)
{
	char	*path;
	char	**args;

	args = allocate_cmd_args(data, cmd);
	path = NULL;
	if (!cmd->cmd[0])
		cmd_not_found(cmd->cmd, data);
	if (ft_strchr(cmd->cmd, '/') && access(cmd->cmd, F_OK) == 0)
	{
		if (is_dir(cmd->cmd) == 1)
			dir_error(data, cmd->cmd);
		if (access(cmd->cmd, X_OK) != 0)
			perm_denied(cmd->cmd);
		path = ft_strdup(cmd->cmd, &data->allocated);
	}
	else if (ft_strchr(cmd->cmd, '/') && access(cmd->cmd, F_OK) == -1)
		no_such_file(data, cmd->cmd);
	else
		path = find_cmd(data, cmd->cmd);
	if (!path)
		cmd_not_found(cmd->cmd, data);
	execve(path, args, data->envp);
}
