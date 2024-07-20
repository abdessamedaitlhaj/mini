/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aait-lha <aait-lha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 13:04:10 by aait-lha          #+#    #+#             */
/*   Updated: 2024/07/19 06:59:19 by aait-lha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		ft_chdir(char *path)
{
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		return (1);
	}
	return (0);
}

int	set_home(char *home, char *pwd, t_data *data)
{
	if (home)
	{
		if (!home[0])
			return (1);
		ft_setenv("PWD", home, data);
		ft_setenv("OLDPWD", pwd, data);
		if (ft_chdir(home) == -1)
			return (1);
	}
	else
	{
		ft_putendl_fd("minishell: cd: HOME not set", 2);
		return (1);
	}
	return (0);
}

int	set_prev(char *oldpwd, char *pwd, t_data *data)
{
	if (oldpwd)
	{
		ft_setenv("PWD", oldpwd, data);
		ft_setenv("OLDPWD", pwd, data);
		if (ft_chdir(oldpwd) == -1)
			return (1);
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
	char	*pwd;
	char	*oldpwd;
	char	*home;

	pwd = ft_pwd(data);
	if (!pwd)
		return (1);
	oldpwd = ft_getenv("OLDPWD", data->env);
	home = ft_getenv("HOME", data->env);
	if (!path || !path[0] || !ft_strcmp(path, "~") || \
		!ft_strcmp(path, "--"))
			return (set_home(home, pwd, data));
	else if (path[0] == '-' && !path[1])
		return (set_prev(oldpwd, pwd, data));
	else
	{
		ft_setenv("PWD", path, data);
		ft_setenv("OLDPWD", pwd, data);
		if (ft_chdir(path) == -1)
			return (-1);
	}
	return (0);	
}