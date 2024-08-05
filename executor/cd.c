/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:04:10 by aait-lha          #+#    #+#             */
/*   Updated: 2024/08/05 23:05:59 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	change(char *path, t_data *data)
{
	if (chdir(path) == -1)
		return (common_error("minishell: cd: ", path, ""));
	ft_setenv(ft_strdup2("OLDPWD", data), data->pwd, data);
	data->pwd = getcwd(NULL, 0);
	return (0);
}

static void	cwd_dir(char *path, t_data *data)
{
	char	*tmp;
	char	*d;

	common_error("cd: error retrieving current directory: ", "getcwd", \
		"cannot access parent directories");
	tmp = ft_strjoin2(data->pwd, "/", data);
	d = ft_strjoin2(tmp, path, data);
	free(tmp);
	ft_setenv(ft_strdup2("OLDPWD", data), data->pwd, data);
	data->pwd = d;
}

static int	check_removed(char *path, t_data *data)
{
	char	*d;

	d = copy_cwd(getcwd(NULL, 0), data);
	if (!d)
	{
		cwd_dir(path, data);
		return (0);
	}
	else if (ft_strcmp(path, data->pwd) == 0)
	{
		ft_setenv(ft_strdup2("OLDPWD", data), data->pwd, data);
		return (0);
	}
	ft_setenv(ft_strdup2("OLDPWD", data), data->pwd, data);
	data->pwd = d;
	return (0);
}

char	*home_set(t_data *data)
{
	char	*home;

	home = ft_getenv("HOME", data->env);
	if (home)
		return (home);
	ft_putendl_fd("minishell: cd: HOME not set", 2);
	return (NULL);
}
int	ft_cd(char **args, t_data *data)
{
	DIR		*dir;
	char	*home;

	if (args[0] && !args[0][0])
		return (0);
	if (!args[0])
	{
		home = home_set(data);
		if (home && home[0])
		{
			if (change(home, data))
				return (1);
			return (0);
		}
		else
			return (1);
	}
	dir = opendir(args[0]);
	if (dir == NULL)
		return (common_error("minishell: cd: ", args[0], ""));
	closedir(dir);
	if (chdir(args[0]) == -1)
		return (common_error("minishell: cd: ", args[0], ""));
	check_removed(args[0], data);
	return (0);
}
