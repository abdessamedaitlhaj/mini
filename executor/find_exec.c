/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 12:08:33 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/16 11:05:28 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

char	**get_paths(t_data *data)
{
	char	**paths;
	int i;

	i = 0;
	while (data->envp[i] && ft_strncmp(data->envp[i], "PATH=", 5) != 0)
		i++;
	if (!data->envp[i])
		return (NULL);
	paths = ft_split(data->envp[i] + 5, ':');
	if (!paths)
		(perror("malloc error"), exit(1));
	return (paths);
}

char	*check_access(char *path, char **paths, char *cmd)
{
	if (access(path, F_OK) == 0)
	{
		if (access(path, X_OK) == 0)
			return (free_arr(paths), path);
		else
		{
			free(path);
			free_arr(paths);
			perm_denied(cmd);
		}
	}
	return (NULL);
}

char	*find_cmd(t_data *data, char *cmd)
{
	char	**paths;
	char	*path;
	int		i;

	paths = get_paths(data);
	i = -1;
	while (paths[++i])
	{
		path = ft_strjoin2(paths[i], "/", cmd);
		if (!path)
			(free_arr(paths), perror("malloc error"), \
				exit(1));
		if (check_access(path, paths, cmd))
			return (path);
		free(path);
	}
	free_arr(paths);
	cmd_not_found(cmd);
	return (NULL);
}

void	exec_cmd(t_data *data, char *cmd, char **args)
{
	char	*path;

	path = NULL;
	if (ft_strchr(cmd, '/') && access(cmd, F_OK) == 0)
	{
		if (access(cmd, X_OK) != 0)
			perm_denied(cmd);
		path = ft_strdup(cmd, &data->allocated);
		if (!path)
			(free(path), perror("malloc error"), exit(1));
	}
	else if (ft_strchr(cmd, '/') && access(cmd, F_OK) == -1)
		cmd_not_found(cmd);
	else
		path = find_cmd(data, cmd);
	if (is_dir(path) == 1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(" : is a directory\n", 2);
		data->exit_status = 126;
		exit(126);
	}
	execve(path, args, data->envp);
	printf("comd not found\n");
}
