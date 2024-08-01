/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 22:16:07 by aait-lha          #+#    #+#             */
/*   Updated: 2024/08/01 15:11:14 by aait-lha         ###   ########.fr       */
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
	waitpid(pid, &status, 0);
	get_status(data, status);
	return (0);
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
