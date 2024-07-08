/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-hara <ael-hara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 12:08:33 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/07 10:23:22 by ael-hara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strjoin2(char *s1, char *s2, char *s3)
{
	int		len;
	int		i;
	char	*str;

	if (!s1 || !s2 || !s3)
		return (NULL);
	i = 0;
	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	str = (char *)malloc(len + 1);
	if (!str)
		return (NULL);
	while (*s1)
		str[i++] = *s1++;
	while (*s2)
		str[i++] = *s2++;
	while (*s3)
		str[i++] = *s3++;
	str[i] = '\0';
	return (str);
}

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
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
		if (access(path, F_OK | X_OK) == 0)
			return (free_arr(paths), path);
		free(path);
		i++;
	}
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
	// while (1)
	// 	;
	if (execve(path, args, data->envp) == -1)
	{
		write(2, "minishell: ", 11);
		write(2, cmd, ft_strlen(cmd));
		write(2, ": command not found\n", 20);
		exit(127);
	}
}