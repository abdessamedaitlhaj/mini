/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 12:08:33 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/15 18:05:42 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

char **allocate_cmd_args(t_data *data, t_cmd *cmd)
{
	char	**args;
	int		i;
	int		j;

	i = 0;
	j = 0;
	args = ft_malloc(sizeof(char *) * (cmd->args_number + 2), \
		&data->allocated);
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

char	*find_cmd(t_data *data, char *cmd)
{
	char	**paths;
	char	*path;
	int		i;

	i = 0;
	while (data->envp[i] && ft_strncmp(data->envp[i], "PATH=", 5) != 0)
		i++;
	if (!data->envp[i])
		return (NULL);
	paths = ft_split(data->envp[i] + 5, ':');
	if (!paths)
		(perror("malloc error"), exit(1));
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin2(paths[i], "/", cmd);
		if (!path)
			(free_arr(paths), perror("malloc error"), \
				exit(1));
		if (access(path, F_OK) == 0)
		{
		printf("%s\n", path);
			if (access(path, X_OK) == 0)
			{	
				return (free_arr(paths), path);
			}
			else
			{
			printf("dddd\n");
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(cmd, 2);
				ft_putendl_fd(": Permission denied", 2);
				data->exit_status = 126;
				exit(126);
			}
		}
		free(path);
		i++;
	}
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd, 2);
			ft_putendl_fd(": command not found", 2);
			data->exit_status = 127;
			exit(127);
	return (free_arr(paths), NULL);
}

void	exec_cmd(t_data *data, char *cmd, char **args)
{
	char	*path;

	if (access(cmd, F_OK | X_OK) == 0)
	{
		path = ft_strdup(cmd, &data->allocated);
		if (!path)
			(free(path), perror("malloc error"), exit(1));
	}
	else
		path = find_cmd(data, cmd);
	execve(path, args, data->envp);
}