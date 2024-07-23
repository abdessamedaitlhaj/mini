/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 12:08:33 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/23 17:22:52 by aait-lha         ###   ########.fr       */
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
	args[0] = ft_strdup(cmd->cmd, &data->allocated);
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
	while (tmp && ft_strncmp(tmp->content, "PATH=", 5) != 0)
		tmp = tmp->next;
	if (!tmp)
		no_such_file(data, cmd);
	paths = ft_split_str(tmp->content + 5, ":", data);
	return (paths);
}

char	*check_access(t_data *data, char *path, char *cmd)
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
			return (path);
		else
			perm_denied(cmd);
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
	while (paths && paths[++i])
	{
		path = ft_strjoin2(paths[i], "/", cmd, data);
		if (check_access(data, path, cmd))
			return (path);
	}
	return (NULL);
}
