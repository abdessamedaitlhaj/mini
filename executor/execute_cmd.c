/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 22:16:07 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/20 17:00:38 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../includes/minishell.h"

int	fork_process(t_data *data, int i, int *fd, int *prev_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		close_streams(&data->fd_in, &data->fd_out, data);
		close_pipe(fd, prev_fd, data);
		fail_error("fork", data);
	}
	if (pid == 0)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, sig_handler);
		dup_redir(data, i, fd, *prev_fd);
		if (is_builtin(&data->cmds[i]))
			exit(ft_exec_builtin(&data->cmds[i], data));
		else
			child_process(data, &data->cmds[i]);
	}
	return (0);
}

void	child_process(t_data *data, t_cmd *cmd)
{
	char	**args;

	args = allocate_cmd_args(data, cmd);
	if (!args)
		return ;
	exec_cmd(data, cmd->cmd, args);
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
		child_process(data, cmd);
	}
	if (waitpid(pid, &status, 0) == -1)
	{
		close_streams(&data->fd_in, &data->fd_out, data);
		fail_error("waitpid", data);
		return (1);
	}
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		data->exit_status = WTERMSIG(status) + 128;
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGQUIT)
	{
		ft_putstr_fd("Quit: ", STDOUT_FILENO);
		ft_putendl_fd(ft_itoa(WTERMSIG(status), data), STDOUT_FILENO);
	}
	return (0);
}

void	exec_cmd(t_data *data, char *cmd, char **args)
{
	char	*path;

	path = NULL;
	if (!cmd[0])
		cmd_not_found(cmd);
	if (ft_strchr(cmd, '/') && access(cmd, F_OK) == 0)
	{
		if (is_dir(cmd) == 1)
			dir_error(data, cmd);
		if (access(cmd, X_OK) != 0)
			perm_denied(cmd);
		path = ft_strdup(cmd, &data->allocated);
		if (!path)
			(free(path), perror("malloc error"), exit(1));
	}
	else if (ft_strchr(cmd, '/') && access(cmd, F_OK) == -1)
		no_such_file(data, cmd);
	else
		path = find_cmd(data, cmd);
	if (!path)
		cmd_not_found(cmd);
	execve(path, args, data->envp);
}