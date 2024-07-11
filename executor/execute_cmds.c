/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 19:09:09 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/11 21:51:56 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_close(int fd)
{
	if (close(fd) == -1)
	{
		perror("close");
		exit (1);
	}
	return (0);
}

int	error_one(int *fd, char *str, int prev_fd)
{

	if (prev_fd != -1)
		ft_close(prev_fd);
	if (fd)
	{
		ft_close(fd[0]);
		ft_close(fd[1]);
	}
	if (str)
		perror(str);
	exit(1);
}

int	error_two(char *str)
{
	ft_putstr_fd("minishell: ", 2);
	perror(str);
	return (1);
}

int	open_files(char *file, int index)
{
	int fd;

	fd = -1;
	if (index == 0)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (index == 1)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (index == 2)
		fd = open(file, O_RDONLY);
	return (fd);
}

int	dup_file(t_aa file, int fd)
{
	if (file == OUTFILE || file == APPEND)
	{
		if (dup2(fd, STDOUT_FILENO) == -1)
			error_one(NULL, "dup2", fd);
	}
	else if (file == INFILE || file == HEREDOC)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			error_one(NULL, "dup2", fd);
	}
	ft_close(fd);
	return (0);
}

int	setting_redir_out(t_cmd *cmd)
{
	int	i;
	int	fd;

	i = 0;
	while (cmd->files[i])
	{
		if (cmd->files[i]->expand_error)
			return (1);
		if (cmd->files[i]->type == OUTFILE)
			fd = open_files(cmd->files[i]->file, 0);
		else if (cmd->files[i]->type == APPEND)
			fd = open_files(cmd->files[i]->file, 1);
		if (fd == -1)
			return (error_two(cmd->files[i]->file));
		i++;
	}
	dup_file(cmd->files[i - 1]->type, fd);
	return (0);
}
int	setting_redir_in(t_cmd *cmd)
{
	int	i;
	int	fd;

	i = 0;
	fd = -2;
	while (cmd->files[i])
	{
		if (cmd->files[i]->expand_error)
			return (1);
		if (cmd->files[i]->type == INFILE || cmd->files[i]->type == HEREDOC)
			fd = open_files(cmd->files[i]->file, 2);
		if (fd == -1)
			return (1);
		i++;
	}
	dup_file(cmd->files[i - 1]->type, fd);
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
	if (dup2(prev_fd, STDIN_FILENO) == -1)
		error_one(fd, "dup2", prev_fd);
	ft_close(prev_fd);
	return (0);
}

int	dup_cmd_out(int *fd)
{
	ft_close(fd[0]);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		error_one(fd, "dup2", -1);
	ft_close(fd[1]);
	return (0);
}

int	dup_redir(t_data *data, int i, int *fd, int prev_fd)
{
	if (data->cmds[i].infile || data->cmds[i].heredoc)
		setting_redir_in(&data->cmds[i]);
	else if (i > 0)
		dup_cmd_in(fd, prev_fd);
	if (data->cmds[i].outfile || data->cmds[i].append)
		setting_redir_out(&data->cmds[i]);
	else if(i < data->counter_command - 1)
		dup_cmd_out(fd);
	return (0);
}

int	execute_one_node(t_data *data)
{
	t_cmd	*cmd;
	pid_t	pid;
	int		status;

	status = 0;
	if (data->cmds[0].files)
	{	
		if (data->cmds[0].infile || data->cmds[0].heredoc)
			setting_redir_in(&data->cmds[0]);
		if (data->cmds[0].outfile || data->cmds[0].append)
			setting_redir_out(&data->cmds[0]);
	}
	cmd = &data->cmds[0];
	if (is_builtin(cmd))
		ft_exec_builtin(cmd, data);
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
			data->exit_status = status;
		}
	}
	return (status);
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
	prev_fd = -1;
	while (++i < data->counter_command)
	{
		if (i < data->counter_command - 1)
			if (pipe(fd) == -1)
				error_one(fd, "pipe", prev_fd);
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
	int	status;
	int stdin_copy;
	int stdout_copy;

	stdin_copy = dup(0);
	stdout_copy = dup(1);
	if (stdin_copy == -1 || stdout_copy == -1)
		return (error_two("dup"));
	status = 0;
	check_exit(data);
	if (data->counter_command == 1)
		execute_one_node(data);
	else
	{
		execute_multiple_nodes(data);
		while (waitpid(-1, &status, 0) > 0)
		{
			data->exit_status = WEXITSTATUS(status);
		}
	}
	if (dup2(stdin_copy, STDIN_FILENO) == -1 || \
		dup2(stdout_copy, STDOUT_FILENO) == -1)
			return (error_two("dup2"));
	return (0);
}