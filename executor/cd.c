/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:04:10 by aait-lha          #+#    #+#             */
/*   Updated: 2024/08/01 20:13:38 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*home_set(t_data *data)
{
	char	*home;

	home = ft_getenv("HOME", data->env);
	if (home)
		return (home);
	ft_putendl_fd("minishell: cd: HOME not set", 2);
	return (NULL);
}

char	*oldpwd_set(t_data *data)
{
	char	*oldpwd;

	oldpwd = ft_getenv("OLDPWD", data->env);
	if (oldpwd)
		return (oldpwd);
	ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
	return (NULL);
}

int	change(char *path, t_data *data)
{
	if (chdir(path) == -1)
		return (common_error("minishell: cd: ", path, ""));
	ft_setenv(ft_strdup2("OLDPWD", data), \
	ft_strdup2(ft_getenv("PWD", data->env), data), data);
	ft_setenv(ft_strdup2("PWD", data), getcwd(NULL, 0), data);
	return (0);
}

void	cwd_dir(char *path, char *d, t_data *data)
{
	char	*tmp;

	common_error("cd: error retrieving current directory: ", "getcwd",\
		"cannot access parent directories");
	tmp = ft_strjoin2(ft_getenv("PWD", data->env), "/", data);
	d = ft_strjoin2(tmp, path, data);
	free(tmp);
	ft_setenv(ft_strdup2("OLDPWD", data), \
	ft_strdup2(ft_getenv("PWD", data->env), data), data);
	ft_setenv(ft_strdup2("PWD", data), d, data);
}

int	check_removed(char *path, t_data *data)
{
	char	*d;

	d = getcwd(NULL, 0);
	if (!d)
	{
		cwd_dir(path, d, data);
		return (1);
	}
	else if (ft_strcmp(path, ft_getenv("PWD", data->env)) == 0)
	{
		ft_setenv(ft_strdup2("OLDPWD", data), \
		ft_strdup2(ft_getenv("PWD", data->env), data), data);
		return (0);
	}
	ft_setenv(ft_strdup2("OLDPWD", data), \
	ft_strdup2(ft_getenv("PWD", data->env), data), data);
	ft_setenv(ft_strdup2("PWD", data), d, data);
	return (0);
}

int	special_path(char *path, t_data *data)
{
	char	*home;
	char	*oldpwd;

	if ((ft_strcmp(path, "~") == 0 && !path[1]) || !path || !path[0])
	{
		home = home_set(data);
		if (home && home[0])
			if (change(home, data))
				return (1);
	}
	else if (ft_strcmp(path, "-") == 0 && !path[1])
	{
		oldpwd = oldpwd_set(data);
		if (oldpwd && oldpwd[0])
		{
			ft_putendl_fd(oldpwd, 1);
			if (change(oldpwd, data))
				return (1);
		}
	}
	else
		return (1);
	return (0);
}

int	ft_cd(char **args, t_data *data)
{
	DIR		*dir;
	int		flag;

	if (args[1])
		return (0);
	flag = special_path(args[0], data);
	if (!flag)
		return (0);
	if (chdir(args[0]) == -1)
		return (common_error("minishell: cd: ", args[0], ""));
	dir = opendir(args[0]);
	if (!dir)
		return (common_error("minishell: cd: ", args[0], ""));
	else
		closedir(dir);
	if (check_removed(args[0], data))
		return (1);
	return (0);
}

