/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:04:10 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/30 11:41:16 by aait-lha         ###   ########.fr       */
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

int	special_path(char *path, t_data *data)
{
	char	*home;
	char	*oldpwd;

	if (ft_strcmp(path, "~") == 0 || ft_strcmp(path, "~/") == 0 \
		|| ft_strcmp(path, "--") == 0 || !path)
	{
		home = home_set(data);
		if (home && home[0])
		{
			if (chdir(home) == -1)
				return (common_error("minishell: cd: ", home, ""));
			ft_setenv(ft_strdup2("OLDPWD", data), ft_getenv("PWD", data->env), data);
			ft_setenv(ft_strdup2("PWD", data), \
			ft_strdup2(copy_cwd(getcwd(NULL, 0), data), data), data);
			return (1);
		}
	}
	else if (ft_strcmp(path, "-") == 0)
	{
		oldpwd = oldpwd_set(data);
		if (oldpwd && oldpwd[0])
		{
			if (chdir(oldpwd) == -1)
				return (common_error("minishell: cd: ", oldpwd, ""));
			ft_setenv(ft_strdup2("OLDPWD", data), ft_getenv("PWD", data->env), data);
			ft_setenv(ft_strdup2("PWD", data), copy_cwd(getcwd(NULL, 0), data), data);
			return (1);
		}
	}
	return (0);
}

int	ft_cd(char *path, t_data *data)
{
	//DIR		*dir;
	char	*d;
	//char	*tmp;

	// if (special_path(path, data))
	// 	return (0);
	// dir = opendir(path);
	// if (!dir)
	// 	return (common_error("minishell: cd: ", path, ""));
	// else
	// 	closedir(dir);
	if (chdir(path) == -1)
		return (common_error("minishell: cd: ", path, ""));
	d = copy_cwd(getcwd(NULL, 0), data);
	// if (!d)
	// {
	// 	tmp = ft_strjoin(ft_getenv("PWD", data->env), "/", data);
	// 	d = ft_strjoin(tmp, path, data);
	// 	free(tmp);
	// 	ft_setenv(ft_strdup2("OLDPWD", data), ft_getenv("PWD", data->env), data);
	// 	ft_setenv(ft_strdup2("PWD", data), d, data);
	// 	return (0);
	// }
	// else if (ft_strcmp(path, ft_getenv("PWD", data->env)) == 0)
	// {
	// 	ft_setenv(ft_strdup2("OLDPWD", data), ft_getenv("PWD", data->env), data);
	// 	return (0);
	// }
	ft_setenv(ft_strdup2("OLDPWD", data), ft_strdup2(ft_getenv("PWD", data->env), data), data);
	ft_setenv(ft_strdup2("PWD", data), d, data);
	return (0);
}
