/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 12:08:33 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/19 10:56:25 by aait-lha         ###   ########.fr       */
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

char	**get_paths(t_data *data, char *cmd)
{
	char	**paths;
	t_list	*tmp;

	tmp = data->env;
	if (!tmp)
		no_such_file(data, cmd);
	while (tmp && ft_strncmp(tmp->content, "PATH=", 5) != 0)
		tmp = tmp->next;
	if (!tmp)
		return (NULL);
	paths = ft_split(tmp->content + 5, ':');
	if (!paths)
		(perror("malloc error"), exit(1));
	return (paths);
}

char	*check_access(t_data *data, char *path, char **paths, char *cmd)
{
	(void)data;
	if (access(path, F_OK) == 0)
	{
		if ((cmd[0] == '.' && !cmd[1]) || (cmd[0] == '.' && \
			cmd[1] == '.' && !cmd[2]))
			cmd_not_found(cmd);
		if (is_dir(path) == 1)
			dir_error(data, cmd);
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

	paths = get_paths(data, cmd);
	i = -1;
	while (paths[++i])
	{
		path = ft_strjoin2(paths[i], "/", cmd);
		if (!path)
			(free_arr(paths), perror("malloc error"), \
				exit(1));
		if (check_access(data, path, paths, cmd))
			return (path);
		free(path);
	}
	free_arr(paths);
	return (NULL);
}
