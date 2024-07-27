/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:04:10 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/27 11:10:41 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	set_home(t_data *data)
{
	char	*home;

	home = ft_getenv("HOME", data->env);
	if (home)
	{
		if (!home[0])
			return (1);
		ft_setenv("PWD", home, data);
		ft_setenv("OLDPWD", ft_pwd(data), data);
		if (chdir(home) == -1)
			return (common_error("minishell: cd: ", home, ""));
	}
	else
	{
		ft_putendl_fd("minishell: cd: HOME not set", 2);
		return (1);
	}
	return (0);
}

int	set_prev(t_data *data)
{
	char	*oldpwd;

	oldpwd = ft_getenv("OLDPWD", data->env);
	if (oldpwd)
	{
		if (!oldpwd[0])
			return (1);
		ft_setenv("PWD", oldpwd, data);
		ft_setenv("OLDPWD", ft_pwd(data), data);
		if (chdir(oldpwd) == -1)
			return (common_error("minishell: cd: ", oldpwd, ""));
	}
	else
	{
		ft_putendl_fd("minishell: cd: OLDPWD not set", 2);
		return (1);
	}
	return (0);
}

int	ft_cd(char *path, t_data *data)
{
	DIR		*dir;

	if (!path || !path[0] || !ft_strcmp(path, "~") || \
		!ft_strcmp(path, "--"))
		return (set_home(data));
	else if (path[0] == '-' && !path[1])
		return (set_prev(data));
	else
	{
		dir = opendir(path);
		if (!dir)
			return (common_error("minishell: cd: ", path, ""));
		else
		{
			closedir(dir);
			ft_setenv("PWD", path, data);
			ft_setenv("OLDPWD", ft_pwd(data), data);
			if (chdir(path) == -1)
				return (common_error("minishell: cd: ", path, ""));
		}
	}
	return (0);
}
