/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 19:09:09 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/08 00:07:34 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_close(int fd)
{
	if (close(fd) == -1)
	{
		perror("close");
		return (1);
	}
	return (0);
}

int	error_one(int *fd, char *str, int prev_fd)
{

	if (prev_fd != -1)
		if (ft_close(prev_fd))
			return (1);
	if (fd)
	{
		if (ft_close(fd[0]) || ft_close(fd[1]))
			return (1);
	}
	if (str)
		perror(str);
	return (1);
}

int	error_two(char *str)
{
	perror(str);
	return (1);
}

int	open_files(char *file, int index)
{
	int fd;

	fd = -12;
	if (index == 0)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (index == 1)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (index == 2)
		fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (-1);
	}
	return (fd);
}

int	dup_files(t_cmd *cmd, int *fd)
{
	int	i;

	i = 0;
	while (cmd->files[i])
	{
		if (cmd->files[i]->type == OUTFILE || cmd->files[i]->type == APPEND)
		{
			if (dup2(fd[i], 1) == -1)
				return (error_one(NULL, "dup2", fd[i]));
		}
		else if (cmd->files[i]->type == INFILE || \
			cmd->files[i]->type == HEREDOC)
		{
			if (dup2(fd[i], 0) == -1)
				return (error_one(NULL, "dup2", fd[i]));
		}
		if (ft_close(fd[i]))
			return (1);
		i++;
	}
	return (0);
}

int	setting_redir(t_data *data, t_cmd *cmd)
{
	int	i;
	int	*fd;
	int count_files;

	count_files = cmd->infile + cmd->outfile + cmd->append;
	fd = (int *)ft_malloc(sizeof(int) * count_files, &data->allocated);
	if (!fd)
		return (1);
	i = 0;
	while (cmd->files[i])
	{
		if (cmd->files[i]->expand_error)
		{
			printf("adsa\n");
			return (1);
		}
		if (cmd->files[i]->type == OUTFILE)
			fd[i] = open_files(cmd->files[i]->file, 0);
		else if (cmd->files[i]->type == APPEND)
			fd[i] = open_files(cmd->files[i]->file, 1);
		else if (cmd->files[i]->type == INFILE)
			fd[i] = open_files(cmd->files[i]->file, 2);
		else if (cmd->files[i]->type == HEREDOC)
			fd[i] = cmd->files[i]->fd;
		if (fd[i] == -1)
			return (1);
		i++;
	}
	if (dup_files(cmd, fd))
		return (1);
	return (0);
}

char **allocate_cmd_args(t_data *data, t_cmd *cmd)
{
	char	**args;
	int		i;
	int		j;

	i = 0;
	j = 0;
	args = ft_malloc(sizeof(char *) * (cmd->args_number + 2), &data->allocated);
	if (!args)
		return (NULL);
	args[0] = ft_strdup(cmd->cmd, &data->allocated);
	if (!args[0])
		return (NULL);
	i = 1;
	while (j < cmd->args_number)
	{
		args[i] = cmd->args[j];
		i++;
		j++;
	}
	args[i] = NULL;
	return (args);
}

int	is_builtin(t_cmd *cmd)
{
	if (ft_strcmp(cmd->cmd, "cd") == 0 ||ft_strcmp(cmd->cmd, "pwd") == 0 \
	|| ft_strcmp(cmd->cmd, "echo") == 0 || ft_strcmp(cmd->cmd, "env") == 0 \
	|| ft_strcmp(cmd->cmd, "export") == 0 || \
	ft_strcmp(cmd->cmd, "unset") == 0 \
	||ft_strcmp(cmd->cmd, "exit") == 0)
		return (1);
	return (0);
}

int	other_builtins(t_data *data, t_cmd *cmd)
{
	if (ft_strcmp(cmd->cmd, "env") == 0)
		return (ft_env(data->env));
	else if (ft_strcmp(cmd->cmd, "export") == 0)
		return (ft_export(cmd->args, data));
	else if (ft_strcmp(cmd->cmd, "unset") == 0)
		return (ft_unsetenv(cmd->args[1], data));
	else if (ft_strcmp(cmd->cmd, "exit") == 0)
		ft_exit(cmd->args[0]);
	return (0);
}

int	ft_exec_builtin(t_cmd *cmd, t_data *data)
{
	if (ft_strcmp(cmd->cmd, "cd") == 0)
		return (ft_cd(cmd->args[0], data));
	else if (ft_strcmp(cmd->cmd, "pwd") == 0)
	{
		if (!ft_pwd(data))
			return (1);
		printf("%s\n", ft_pwd(data));
	}
	else if (ft_strcmp(cmd->cmd, "echo") == 0)  
		return (ft_echo(cmd->args, cmd->args_number, data->exit_status));
	return (other_builtins(data, cmd));
	return (0);
}

int	child_process(t_data *data, t_cmd *cmd)
{
	char	**args;

	args = allocate_cmd_args(data, cmd);
	if (!args)
		return (1);
	exec_cmd(data, cmd->cmd, args);
	return (0);
}

int	dup_cmd_in(int *fd, int prev_fd)
{
	if (dup2(prev_fd, 0) == -1)
		return (error_one(fd, "dup2", prev_fd));
	if (ft_close(prev_fd))
		return (1);
	return (0);
}

int	dup_cmd_out(int *fd)
{
	if (ft_close(fd[0]))
		return (1);
	if (dup2(fd[1], 1) == -1)
		return (error_one(fd, "dup2", -1));
	if (ft_close(fd[1]))
		return (1);
	return (0);
}

int	dup_redir(t_data *data, int i, int *fd, int prev_fd)
{
	if (i > 0)
		if (dup_cmd_in(fd, prev_fd))
			return (1);
	if (i < data->counter_command - 1)
		if (dup_cmd_out(fd))
			return (1);
	return (0);
}

int	fork_process(t_data *data, int i, int *fd, int prev_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (error_one(fd, "fork", prev_fd));
	if (pid == 0)
	{
		if (dup_redir(data, i, fd, prev_fd))
			return (1);
		if (child_process(data, &data->cmds[i]))
			return (1);
	}
	return (0);
}

int	execute_one_node(t_data *data)
{
	t_cmd	*cmd;      
	pid_t	pid;
	int		status;

	status = 0;
	if (data->cmds[0].files)
		if (setting_redir(data, &data->cmds[0]))
			return (1);
	cmd = &data->cmds[0];
	if (is_builtin(cmd))
	{
		status = ft_exec_builtin(cmd, data);
		if (status == -1)
			return (1);
	}
	else
	{
		pid = fork();
		if (pid == -1)
			return (error_two("fork"));
		if (pid == 0)
		{
			if (child_process(data, cmd))
				return (1);
		}
		else if (pid > 0)
		{
			if (waitpid(pid, &status, 0) == -1)
				return (error_two("waitpid"));
			data->exit_status = status;
		}
	}
	return (status);
}

int	execute_multiple_nodes(t_data *data)
{
	int	i;
	int	fd[2];
	int	prev_fd;

	i = -1;
	prev_fd = -1;
	while (++i < data->counter_command)
	{
		if (i < data->counter_command - 1)
			if (pipe(fd) == -1)
				return (error_one(fd, "pipe", prev_fd));
		if (setting_redir(data, &data->cmds[i]))
			return (1);
		if (fork_process(data, i, fd, prev_fd))
			return (1);
		if (i > 0)
			if (ft_close(prev_fd))
				return (1);
		if (i < data->counter_command - 1)
		{
			prev_fd = fd[0];
			if (ft_close(fd[1]))
				return (1);
		}
	}
	return (0);
}

void	check_exit(t_data *data)
{
	if (ft_strcmp(data->cmds[0].cmd, "exit") == 0 && \
		data->cmds[0].args_number == 0)
	{
		ft_putendl_fd("exit", 1);
		exit(0);
	}
}

int	execute_cmds(t_data *data)
{
	int	stdin_copy;
	int	stdout_copy;
	int	status;

	status = 0;
	stdin_copy = dup(0);
	stdout_copy = dup(1);
	if (stdin_copy == -1 || stdout_copy == -1)
		return (error_two("dup error"));
	check_exit(data);
	if (data->counter_command == 1)
	{
		status = execute_one_node(data);
		if (status == -1)
			return (1);
	}
	else
	{
		status = execute_multiple_nodes(data);
		if (status == -1)
			return (1);
		while (waitpid(-1, &data->exit_status, 0) > 0)
		;
	}
	if (dup2(stdin_copy, 0) == -1 || dup2(stdout_copy, 1) == -1)
		return (error_two("dup2 error22"));
	if (ft_close(stdin_copy) == 1 || ft_close(stdout_copy) == 1)
		return (1);
	return (status);
}